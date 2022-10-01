/******************************************************************************
*  Copyright 2017 - 2021, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "msg.h"
#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_RAW /* don't build if not configured for use in lwipopts.h */
#include "lwip/init.h"
#include "lwip/mem.h"
#include "lwip/icmp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"
#include "lwip/ip.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include "ping/ping_sock.h"

const static char *TAG = "ping";

#define PING_CHECK(a, str, goto_tag, ret_value, ...)                              \
    do                                                                            \
    {                                                                             \
        if (!(a))                                                                 \
        {                                                                         \
            log_print(LOG_HIGH_LEVEL, TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__);   \
            ret = ret_value;                                                      \
            goto goto_tag;                                                        \
        }                                                                         \
    } while (0)

#define PING_CHECK_OPTLEN(optlen, opttype) do { if ((optlen) < sizeof(opttype)) { return -1; }}while(0)

#define PING_TIME_DIFF_MS(_end, _start) ((uint32_t)(((_end).tv_sec - (_start).tv_sec) * 1000 + \
                                                    ((_end).tv_usec - (_start).tv_usec) / 1000))

#define PING_CHECK_START_TIMEOUT_MS (1000)

#define PING_FLAGS_INIT (1 << 0)
#define PING_FLAGS_START (1 << 1)

typedef struct {
    int sock;
    struct sockaddr_storage target_addr;
    TaskHandle_t ping_task_hdl;
    struct icmp_echo_hdr *packet_hdr;
    ip_addr_t recv_addr;
    uint32_t recv_len;
    uint32_t icmp_pkt_size;
    uint32_t count;
    uint32_t run_count;
    uint32_t transmitted;
    uint32_t received;
    uint32_t interval_ms;
    uint32_t elapsed_time_ms;
    uint32_t total_time_ms;
    uint8_t ttl;
    uint32_t flags;
    void (*on_ping_success)(void *hdl, void *args);
    void (*on_ping_timeout)(void *hdl, void *args);
    void (*on_ping_end)(void *hdl, void *args);
    void *cb_args;
} ping_t;

typedef struct _ping_option {
    ip_addr_t ping_target;
    uint32_t ping_count;
    uint32_t ping_rcv_timeout;
    uint32_t ping_delay;
    uint32_t interface;
    size_t ping_data_len;
    uint16_t ping_id;
    uint8_t ping_tos;
    ping_found_fn ping_res_fn;
    ping_found    ping_res;
    void    *ping_reserve;
} ping_option;

static ping_option ping_option_info[1];

static int ping_gettimeofday(struct timeval *tv, void *tzp)
{
    uint32_t t = xTaskGetTickCount();
    tv->tv_sec = t / 1000;
    tv->tv_usec = (t % 1000) * 1000;
    return 0;
}

static int ping_send(ping_t *ep)
{
    int ret = 0;
    ep->packet_hdr->seqno++;
    ep->run_count++;
    /* generate checksum since "seqno" has changed */
    ep->packet_hdr->chksum = 0;
    if (ep->packet_hdr->type == ICMP_ECHO) {
        ep->packet_hdr->chksum = inet_chksum(ep->packet_hdr, ep->icmp_pkt_size);
    }

    int sent = sendto(ep->sock, ep->packet_hdr, ep->icmp_pkt_size, 0,
                      (struct sockaddr *)&ep->target_addr, sizeof(ep->target_addr));

    if (sent != (int)ep->icmp_pkt_size) {
        int opt_val;
        socklen_t opt_len = sizeof(opt_val);
        getsockopt(ep->sock, SOL_SOCKET, SO_ERROR, &opt_val, &opt_len);
        log_print(LOG_HIGH_LEVEL, TAG, "send error=%d, %d\r\n", sent, opt_val);
        ret = -1;
    } else {
        ep->transmitted++;
    }
    return ret;
}

static int ping_receive(ping_t *ep)
{
    char buf[64]; // 64 bytes are enough to cover IP header and ICMP header
    int len = 0;
    struct sockaddr_storage from;
    int fromlen = sizeof(from);
    uint16_t data_head = 0;

    while ((len = recvfrom(ep->sock, buf, sizeof(buf), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen)) > 0) {
        if (from.ss_family == AF_INET) {
            // IPv4
            struct sockaddr_in *from4 = (struct sockaddr_in *)&from;
            inet_addr_to_ip4addr(ip_2_ip4(&ep->recv_addr), &from4->sin_addr);
            IP_SET_TYPE_VAL(ep->recv_addr, IPADDR_TYPE_V4);
            data_head = (uint16_t)(sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr));
        }
        if (len >= data_head) {
            if (IP_IS_V4_VAL(ep->recv_addr)) {              // Currently we process IPv4
                struct ip_hdr *iphdr = (struct ip_hdr *)buf;
                struct icmp_echo_hdr *iecho = (struct icmp_echo_hdr *)(buf + (IPH_HL(iphdr) * 4));
                if ((iecho->id == ep->packet_hdr->id) && (iecho->seqno == ep->packet_hdr->seqno)) {
                    ep->received++;
                    ep->ttl = iphdr->_ttl;
                    ep->recv_len = lwip_ntohs(IPH_LEN(iphdr)) - data_head;  // The data portion of ICMP
                    return len;
                }
            }
        }
        fromlen = sizeof(from);
    }
    // if timeout, len will be -1
    return len;
}

static void ping_thread(void *args)
{
    ping_t *ep = (ping_t *)(args);
//    TickType_t last_wake;
    struct timeval start_time, end_time;
    int recv_ret;

    while (1) {
        /* wait for ping start signal */
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(PING_CHECK_START_TIMEOUT_MS))) {
            /* initialize runtime statistics */
            ep->packet_hdr->seqno = 0;
            ep->transmitted = 0;
            ep->received = 0;
            ep->total_time_ms = 0;

//            last_wake = xTaskGetTickCount();
            while ((ep->flags & PING_FLAGS_START) && ((ep->count == 0) || (ep->run_count < ep->count))) {
                ping_send(ep);
                ping_gettimeofday(&start_time, NULL);
                recv_ret = ping_receive(ep);
                ping_gettimeofday(&end_time, NULL);
                ep->elapsed_time_ms = PING_TIME_DIFF_MS(end_time, start_time);
                ep->total_time_ms += ep->elapsed_time_ms;
                if (recv_ret >= 0) {
                    if (ep->on_ping_success) {
                        ep->on_ping_success((void *)ep, ep->cb_args);
                    }
                } else {
                    if (ep->on_ping_timeout) {
                        ep->on_ping_timeout((void *)ep, ep->cb_args);
                    }
                }
                if (ep->elapsed_time_ms < ep->interval_ms)
                    vTaskDelay((ep->interval_ms - ep->elapsed_time_ms) / portTICK_PERIOD_MS);
//                if (pdMS_TO_TICKS(ep->interval_ms)) {
//                    vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(ep->interval_ms)); // to get a more accurate delay
//                }
            }
            /* batch of ping operations finished */
            if (ep->on_ping_end) {
                ep->on_ping_end((void *)ep, ep->cb_args);
            }
        } else {
            // check if ping has been de-initialized
            if (!(ep->flags & PING_FLAGS_INIT)) {
                break;
            }
        }
    }
    /* before exit task, free all resources */
    close(ep->sock);
    if (ep->packet_hdr) {
        free(ep->packet_hdr);
    }
    free(ep);
    vTaskDelete(NULL);
}

int ping_new_session(const ping_config_t *config, const ping_callbacks_t *cbs, void **hdl_out)
{
    int ret = 0;
    ping_t *ep = NULL;
    BaseType_t xReturned;
    char *d;

    PING_CHECK(config, "ping config can't be null", err, -1);
    PING_CHECK(hdl_out, "ping handle can't be null", err, -1);

    ep = calloc(1, sizeof(ping_t));
    PING_CHECK(ep, "no memory for ping object", err, 0x101);

    /* set INIT flag, so that ping task won't exit (must set before create ping task) */
    ep->flags |= PING_FLAGS_INIT;

    /* create ping thread */
    xReturned = xTaskCreate(ping_thread, "opl_ping", config->task_stack_size, ep,
                                       config->task_prio, &ep->ping_task_hdl);
    PING_CHECK(xReturned == pdTRUE, "create ping task failed", err, 0x101);

    /* callback functions */
    if (cbs) {
        ep->cb_args = cbs->cb_args;
        ep->on_ping_end = cbs->on_ping_end;
        ep->on_ping_timeout = cbs->on_ping_timeout;
        ep->on_ping_success = cbs->on_ping_success;
    }
    /* set parameters for ping */
    ep->recv_addr = config->target_addr;
    ep->count = config->count;
    ep->run_count = 0;
    ep->interval_ms = config->interval_ms;
    ep->icmp_pkt_size = sizeof(struct icmp_echo_hdr) + config->data_size;
    ep->packet_hdr = calloc(1, ep->icmp_pkt_size);
    PING_CHECK(ep->packet_hdr, "no memory for echo packet", err, 0x101);
    /* set ICMP type and code field */
    ep->packet_hdr->code = 0;
    /* ping id should be unique, treat task handle as ping ID */
    ep->packet_hdr->id = ((uint32_t)ep->ping_task_hdl) & 0xFFFF;
    /* fill the additional data buffer with some data */
    d = (char *)(ep->packet_hdr) + sizeof(struct icmp_echo_hdr);
    for (uint32_t i = 0; i < config->data_size; i++) {
        d[i] = 'A' + i;
    }

    /* create socket */
    if IP_IS_V4(&config->target_addr) {
        ep->sock = socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP);
    }

    PING_CHECK(ep->sock >= 0, "create socket failed: %d\r\n", err, -1, ep->sock);

    struct timeval timeout;
    timeout.tv_sec = config->timeout_ms / 1000;
    timeout.tv_usec = (config->timeout_ms % 1000) * 1000;
    /* set receive timeout */
    setsockopt(ep->sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    /* set tos */
    setsockopt(ep->sock, IPPROTO_IP, IP_TOS, &config->tos, sizeof(config->tos));

    /* set socket address */
    if (IP_IS_V4(&config->target_addr)) {
        struct sockaddr_in *to4 = (struct sockaddr_in *)&ep->target_addr;
        to4->sin_family = AF_INET;
        inet_addr_from_ip4addr(&to4->sin_addr, ip_2_ip4(&config->target_addr));
        ep->packet_hdr->type = ICMP_ECHO;
    }
    /* return ping handle to user */
    *hdl_out = (void *)ep;
    return 0;
err:
    if (ep) {
        if (ep->sock >= 0) {
            close(ep->sock);
        }
        if (ep->packet_hdr) {
            free(ep->packet_hdr);
        }
        if (ep->ping_task_hdl) {
            vTaskDelete(ep->ping_task_hdl);
        }
        free(ep);
    }
    return ret;
}

int ping_delete_session(void *hdl)
{
    int ret = 0;
    ping_t *ep = (ping_t *)hdl;
    PING_CHECK(ep, "ping handle can't be null", err, 0x102);
    /* reset init flags, then ping task will exit */
    ep->flags &= ~PING_FLAGS_INIT;
    return 0;
err:
    return ret;
}

int ping_start(void *hdl)
{
    int ret = 0;
    ping_t *ep = (ping_t *)hdl;
    PING_CHECK(ep, "ping handle can't be null", err, 0x102);
    ep->flags |= PING_FLAGS_START;
    xTaskNotifyGive(ep->ping_task_hdl);
    return 0;
err:
    return ret;
}

int ping_stop(void *hdl)
{
    int ret = 0;
    ping_t *ep = (ping_t *)hdl;
    PING_CHECK(ep, "ping handle can't be null", err, 0x102);
    ep->flags &= ~PING_FLAGS_START;
    return 0;
err:
    return ret;
}

int ping_get_profile(void *hdl, ping_profile_t profile, void *data, uint32_t size)
{
    int ret = 0;
    ping_t *ep = (ping_t *)hdl;
    const void *from = NULL;
    uint32_t copy_size = 0;
    PING_CHECK(ep, "ping handle can't be null", err, 0x102);
    PING_CHECK(data, "profile data can't be null", err, 0x102);
    switch (profile) {
    case PING_PROF_SEQNO:
        from = (void *)&ep->packet_hdr->seqno;
        copy_size = sizeof(ep->packet_hdr->seqno);
        break;
    case PING_PROF_TTL:
        from = &ep->ttl;
        copy_size = sizeof(ep->ttl);
        break;
    case PING_PROF_REQUEST:
        from = &ep->transmitted;
        copy_size = sizeof(ep->transmitted);
        break;
    case PING_PROF_REPLY:
        from = &ep->received;
        copy_size = sizeof(ep->received);
        break;
    case PING_PROF_IPADDR:
        from = &ep->recv_addr;
        copy_size = sizeof(ep->recv_addr);
        break;
    case PING_PROF_SIZE:
        from = &ep->recv_len;
        copy_size = sizeof(ep->recv_len);
        break;
    case PING_PROF_TIMEGAP:
        from = &ep->elapsed_time_ms;
        copy_size = sizeof(ep->elapsed_time_ms);
        break;
    case PING_PROF_DURATION:
        from = &ep->total_time_ms;
        copy_size = sizeof(ep->total_time_ms);
        break;
    default:
        PING_CHECK(false, "unknow profile: %d", err, 0x102, profile);
    }
    PING_CHECK(size >= copy_size, "unmatched data size for profile %d", err, 0x104, profile);
    memcpy(data, from, copy_size);
    return 0;
err:
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////
int ping_set_target(ping_target_id_t opt_id, void *opt_val, uint32_t opt_len)
{
    int ret = 0;

    if (opt_val == NULL) {
        return -1;
    }

    switch (opt_id) {
    case PING_TARGET_IP_ADDRESS:
        ipaddr_aton(opt_val, &(ping_option_info->ping_target));
        break;
    case PING_TARGET_IP_ADDRESS_COUNT:
        PING_CHECK_OPTLEN(opt_len, uint32_t);
        ping_option_info->ping_count = *(uint32_t *)opt_val;
        break;
    case PING_TARGET_IF_INDEX:
        PING_CHECK_OPTLEN(opt_len, uint32_t);
        ping_option_info->interface = *(uint32_t *)opt_val;
        break;
    case PING_TARGET_RCV_TIMEO:
        PING_CHECK_OPTLEN(opt_len, uint32_t);
        ping_option_info->ping_rcv_timeout = (*(uint32_t *)opt_val);
        break;
    case PING_TARGET_DELAY_TIME:
        PING_CHECK_OPTLEN(opt_len, uint32_t);
        ping_option_info->ping_delay = (*(uint32_t *)opt_val);
        break;
    case PING_TARGET_DATA_LEN:
        PING_CHECK_OPTLEN(opt_len, size_t);
        ping_option_info->ping_data_len = (*(size_t *)opt_val);
        break;
    case PING_TARGET_ID:
        PING_CHECK_OPTLEN(opt_len, uint16_t);
        ping_option_info->ping_id = *(uint16_t *)opt_val;
        break;
    case PING_TARGET_IP_TOS:
        PING_CHECK_OPTLEN(opt_len, u8_t);
        ping_option_info->ping_tos = *(u8_t *)opt_val;
        break;
    case PING_TARGET_RES_FN:
        ping_option_info->ping_res_fn = (ping_found_fn)opt_val;
        break;
    case PING_TARGET_RES_RESET:
        memset(&ping_option_info->ping_res, 0, sizeof(ping_option_info->ping_res));
        break;
    default:
        ret = -1;
        break;
    }

    return ret;
}

int ping_get_target(ping_target_id_t opt_id, void *opt_val, uint32_t opt_len)
{
    int ret = 0;

    if (opt_val == NULL) {
        return -1;
    }

    switch (opt_id) {
    case PING_TARGET_IP_ADDRESS:
        ip_addr_copy(*(ip_addr_t*)opt_val, ping_option_info->ping_target);
        break;
    case PING_TARGET_IP_ADDRESS_COUNT:
        PING_CHECK_OPTLEN(opt_len, uint32_t);
        *(uint32_t *)opt_val = ping_option_info->ping_count;
        break;
    case PING_TARGET_IF_INDEX:
        PING_CHECK_OPTLEN(opt_len, uint32_t);
        *(uint32_t *)opt_val = ping_option_info->interface;
        break;
    case PING_TARGET_RCV_TIMEO:
        PING_CHECK_OPTLEN(opt_len, uint32_t);
        *(uint32_t *)opt_val = ping_option_info->ping_rcv_timeout;
        break;
    case PING_TARGET_DELAY_TIME:
        PING_CHECK_OPTLEN(opt_len, uint32_t);
        *(uint32_t *)opt_val = ping_option_info->ping_delay;
        break;
    case PING_TARGET_DATA_LEN:
        PING_CHECK_OPTLEN(opt_len, size_t);
        *(size_t *)opt_val = ping_option_info->ping_data_len;
        break;
    case PING_TARGET_ID:
        PING_CHECK_OPTLEN(opt_len, uint16_t);
        *(uint16_t *)opt_val = ping_option_info->ping_id;
        break;
    case PING_TARGET_IP_TOS:
        PING_CHECK_OPTLEN(opt_len, uint16_t);
        *(uint16_t *)opt_val = ping_option_info->ping_tos;
        break;
    default:
        ret = -1;
        break;
    }

    return ret;
}

int ping_result(uint8_t res_val, uint16_t ping_len, uint32_t ping_time)
{
    int ret = 0;

    ping_option_info->ping_res.ping_err = res_val;

    if (res_val != PING_RES_FINISH) {
        ping_option_info->ping_res.bytes = ping_len;
        ping_option_info->ping_res.resp_time = ping_time;
        ping_option_info->ping_res.total_bytes += ping_len;
        ping_option_info->ping_res.send_count ++;

        if (res_val == PING_RES_TIMEOUT) {
            ping_option_info->ping_res.timeout_count ++;
        } else {
            if (!ping_option_info->ping_res.min_time || (ping_time < ping_option_info->ping_res.min_time)) {
                ping_option_info->ping_res.min_time = ping_time;
            }

            if (ping_time > ping_option_info->ping_res.max_time) {
                ping_option_info->ping_res.max_time = ping_time;
            }


            ping_option_info->ping_res.total_time += ping_time;
            ping_option_info->ping_res.recv_count ++;
        }
    }

    if (ping_option_info->ping_res_fn) {
        ping_option_info->ping_res_fn(PING_TARGET_RES_FN, &ping_option_info->ping_res);
        if (res_val == PING_RES_FINISH) {
            memset(&ping_option_info->ping_res, 0, sizeof(ping_found));
        }
    }

    return ret;
}

#endif /* LWIP_IPV4 && LWIP_RAW */
