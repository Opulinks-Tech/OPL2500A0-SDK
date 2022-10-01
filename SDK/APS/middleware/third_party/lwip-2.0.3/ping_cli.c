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

#include <string.h>
#include <stdlib.h>
#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_RAW /* don't build if not configured for use in lwipopts.h */
#include "msg.h"
#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "ping_cli.h"
#include "ping/ping_sock.h"

/* ping handle */
static void *g_ping = NULL;

const static char *TAG = "ping";

static void ping_cli_helper(void)
{
    printf("<s>        -  Stop the ping request.\r\n");
    printf("<count>    -  Number of echo request to send. (default 3)\r\n");
    printf("<size>     -  Payload size. (default 64 bytes)\r\n");
    printf("<timeout>  -  Timeout in milliseconds for wait reply. (default 1000)\r\n");
    printf("<delay>    -  Delay in milliseconds per echo request. (default 1000)\r\n");
}

static void on_ping_success(void *hdl, void *args)
{
    uint8_t ttl;
    uint16_t seqno;
    uint32_t elapsed_time, recv_len;
    ip_addr_t target_addr;
    ping_get_profile(hdl, PING_PROF_SEQNO, &seqno, sizeof(seqno));
    ping_get_profile(hdl, PING_PROF_TTL, &ttl, sizeof(ttl));
    ping_get_profile(hdl, PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    ping_get_profile(hdl, PING_PROF_SIZE, &recv_len, sizeof(recv_len));
    ping_get_profile(hdl, PING_PROF_TIMEGAP, &elapsed_time, sizeof(elapsed_time));
    log_print(LOG_HIGH_LEVEL, TAG, "%d bytes from %s seq=%d ttl=%d time=%d ms\r\n",
              recv_len, ipaddr_ntoa((ip_addr_t*)&target_addr), seqno, ttl, elapsed_time);
}

static void on_ping_timeout(void *hdl, void *args)
{
    uint16_t seqno;
    ip_addr_t target_addr;
    ping_get_profile(hdl, PING_PROF_SEQNO, &seqno, sizeof(seqno));
    ping_get_profile(hdl, PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    log_print(LOG_HIGH_LEVEL, TAG, "From %s seq=%d timeout\r\n", ipaddr_ntoa((ip_addr_t*)&target_addr), seqno);
}

static void on_ping_end(void *hdl, void *args)
{
    ip_addr_t target_addr;
    uint32_t transmitted;
    uint32_t received;
    uint32_t total_time_ms;
    ping_get_profile(hdl, PING_PROF_REQUEST, &transmitted, sizeof(transmitted));
    ping_get_profile(hdl, PING_PROF_REPLY, &received, sizeof(received));
    ping_get_profile(hdl, PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    ping_get_profile(hdl, PING_PROF_DURATION, &total_time_ms, sizeof(total_time_ms));
    double loss = (double)((1 - ((float)received) / transmitted) * 100);
    if (IP_IS_V4(&target_addr))
        log_print(LOG_HIGH_LEVEL, TAG, "--- %s ping statistics ---\r\n", inet_ntoa(*ip_2_ip4(&target_addr)));
    
    log_print(LOG_HIGH_LEVEL, TAG, "%d transmitted, %d received, %u lost (%.2f%% loss), average time %u ms\r\n",
              transmitted, received, (transmitted - received), loss, (total_time_ms / received));

    // delete the ping sessions, so that we clean up all resources and can create a new ping session
    // we don't have to call delete function in the callback, instead we can call delete function from other tasks
    ping_delete_session(hdl);
}

int ping_cli_handler(int len, char *param[])
{
    ping_config_t config = PING_DEFAULT_CONFIG();

    if (len < 1) {
        ping_cli_helper();
        return 0;
    }

    switch (len) {
        case 1:
        {
            if (strcmp(param[0], "h") == 0 ||
                strcmp(param[0], "-h") == 0) {
                ping_cli_helper();
                return 0;
            }
            
            if (strcmp(param[0], "s") == 0) {
                ping_stop(g_ping);
                return 0;
            }
        }
            break;
        case 2:
        {
            int count = atoi(param[1]);
            config.count = count;
        }
            break;
        case 3:
        {
            int count = atoi(param[1]);
            config.count = count;

            int size = atoi(param[2]);
            config.data_size = size;
        }
            break;
        case 4:
        {
            int count = atoi(param[1]);
            config.count = count;

            int size = atoi(param[2]);
            config.data_size = size;

            int timeout = atoi(param[3]);
            config.timeout_ms = timeout;
        }
            break;
        case 5:
        {
            int count = atoi(param[1]);
            config.count = count;

            int size = atoi(param[2]);
            config.data_size = size;

            int timeout = atoi(param[3]);
            config.timeout_ms = timeout;

            int delay = atoi(param[4]);
            config.interval_ms = delay;
        }
            break;
        default:
            break;
    }

    struct addrinfo hint = {0};
    struct addrinfo *res = NULL;
    if (getaddrinfo(param[0], NULL, &hint, &res) != 0) {
        log_print(LOG_HIGH_LEVEL, TAG, "unknown host %s\r\n", param[0]);
        freeaddrinfo(res);
        return -1;
    }

    if (res->ai_family == AF_INET) {
        struct in_addr addr4 = ((struct sockaddr_in *) (res->ai_addr))->sin_addr;
        inet_addr_to_ip4addr(ip_2_ip4(&config.target_addr), &addr4);
        config.target_addr.type = IPADDR_TYPE_V4;
    }
    freeaddrinfo(res);

    
    ping_callbacks_t cbs = {
        .cb_args     = NULL,
        .on_ping_success = on_ping_success,
        .on_ping_timeout = on_ping_timeout,
        .on_ping_end = on_ping_end,
    };

    ping_new_session(&config, &cbs, &g_ping);
    ping_start(g_ping);

    return 0;
}
#endif /* LWIP_IPV4 && LWIP_RAW */
