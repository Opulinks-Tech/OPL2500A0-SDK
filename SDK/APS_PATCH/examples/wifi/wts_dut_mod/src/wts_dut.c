/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "console.h"
#include "cmd_parser.h"
#include "event_loop.h"
#include "event_groups.h"
#include "lwip_helper.h"
#include "sys_common_api.h"
#include "wifi_api.h"
#include "wifi_types.h"
#include "wifi_event.h"
#include "wifi_event_handler.h"
#include "msg.h"
#include "opulinks_log.h"
#include "controller_wifi.h"
#include "wts_dut_serial.h"
#include "wts_dut.h"

/********************************************
Declaration of Global Variables & Functions
********************************************/
const int WIFI_CONNECTED_BIT0 = 0x01;
const int GOT_IP_BIT1         = 0x02;

extern T_TracerTaskInfo *g_ptTracerExtTaskInfo;
extern uint32_t g_dwTracerQueueNum;
extern uint32_t g_dwTracerQueueSize;


int dut_main_proc(int argc, char **argv);


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
typedef struct
{
    uint32_t event;
    uint32_t buf_len;
    void    *buf;
} wts_dut_q_msg;

typedef struct {
    osThreadId         main_task_h;
    osMessageQId       queue_h;
    EventGroupHandle_t event_bits;
    int                main_task_pri;
    uint16_t           main_task_stack_size;
    uint16_t           main_task_queue_num;
    char               serial_buf[WTS_DUT_SERIAL_BUF_LEN];
} wts_dut_p;
wts_dut_p g_wts_dut = {0};

static const char *TAG="[WTS]:";

static int wifi_event_handler_cb(wifi_event_id_t event_id, void *data, uint16_t length)
{
    switch(event_id) {
    case WIFI_EVENT_STA_CONNECTED:
        wifi_config_set_ps(WIFI_PS_NONE);
        xEventGroupSetBits(g_wts_dut.event_bits, WIFI_CONNECTED_BIT0);
        LOGI(TAG, "Wi-Fi Connected");
//        lwip_net_start(WIFI_MODE_STA);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        LOGI(TAG, "Wi-Fi Disonnected");
        xEventGroupClearBits(g_wts_dut.event_bits, WIFI_CONNECTED_BIT0);
        break;
    case WIFI_EVENT_SCAN_COMPLETE:
        break;
    case WIFI_EVENT_STA_GOT_IP:
        xEventGroupSetBits(g_wts_dut.event_bits, WIFI_CONNECTED_BIT0);
        xEventGroupSetBits(g_wts_dut.event_bits, GOT_IP_BIT1);
        LOGI(TAG, "Wi-Fi Got IP");
        lwip_get_ip_info("st1");
        break;
    case WIFI_EVENT_STA_CONNECTION_FAILED:
        LOGI(TAG, "Wi-Fi Connected failed");
        break;
    default:
        LOGW(TAG, "Unknown Event %d", event_id);
        break;
    }
    return 0;
}

static int console_init(void)
{
    console_cmd_register(&cmd_parser_func);
    return 0;
}

static void initial_internal_log_level(void)
{
    tracer_priority_set(0);
    tracer_log_mode_set(1);
    tracer_log_level_set(255, 7);
    g_dwTracerQueueNum  = 128;
    g_dwTracerQueueSize = 128;
} 

static int cmd_buf_to_arg(int *argc, char **argv, char *pbuf)
{
    int count = 0;
    char *p = strtok(pbuf, " ");
    
    argv[count] = p;
    count++;
    
    while ((p = strtok(NULL, " ")) != NULL) {
        argv[count] = p;
        count++;
    }
    
    *argc = count;
    return 0;
}

static void initial_wifi_config(void)
{
    g_wts_dut.event_bits = xEventGroupCreate();
    wifi_auto_connect_set_mode(0); //Disable Auto Connect
}

static void initial_lwip_options(void)
{
    opl_lwip_option_t option = {0};

    option.ip.mtu            = 1500;
    option.tcp.tcp_mss       = option.ip.mtu - 40;
    option.tcp.tcp_over_size = option.tcp.tcp_mss;
    option.tcp.tcp_snd_buf   = option.tcp.tcp_mss * 20;
    option.tcp.tcp_snd_queue_len     = 4 * option.tcp.tcp_snd_buf / option.tcp.tcp_mss;
    option.tcp.tcp_snd_low_wat       = option.tcp.tcp_snd_buf - 1;
    option.tcp.tcp_snd_queue_low_wat = option.tcp.tcp_snd_queue_len / 2;
    option.tcp.tcp_receive_wnd       = option.tcp.tcp_mss * 20;
    option.tcp.tcp_wnd_update_threshold = option.tcp.tcp_mss * 4;
    option.tcp.tcp_tmr_interval      = 250;
    option.tcp.tcp_max_retry_of_sync = 4;
    option.tcp.tcp_max_retry_of_data = 12;
    option.tcpip.tcpip_mbox_size     = 16;
    option.tcpip.tcp_recv_mbox_size  = 16;
    option.tcpip.udp_recv_mbox_size  = 16;
    option.tcpip.raw_recv_mbox_size  = 16;
    option.tcpip.accept_mbox_size    = 16;
    option.retrans.mode              = OPL_LWIP_TCP_COARSE_GRAINED;

    opl_lwip_option_set(OPL_LWIP_CUSTOMER, &option);

    opl_lwip_mtu_update = option.ip.mtu;

}

static void user_app_entry(void *args)
{
    osEvent        rxEvent;
    wts_dut_q_msg *rxMsg;

    /* initial internal log level */
    initial_internal_log_level();
    
    /* Tcpip stack and net interface initialization,  dhcp client process initialization. */
    lwip_network_init(WIFI_MODE_STA);

    while (1) {
        rxEvent = osMessageGet(g_wts_dut.queue_h, osWaitForever);
        if(rxEvent.status != osEventMessage) continue;

        rxMsg = (wts_dut_q_msg *) rxEvent.value.p;

        if (rxMsg->event == WTS_DUT_INIT_CMD) {
            int argc = 0;
            char *argv[3] = {0};
            cmd_buf_to_arg(&argc, argv, rxMsg->buf);
            dut_main_proc(argc, argv);
        }

        if (rxMsg->buf_len > 0 && rxMsg->buf) {
            free(rxMsg->buf);
            rxMsg->buf     = NULL;
            rxMsg->buf_len = 0;
        }
    }
}

static void creat_task_to_init_network(void)
{
    osThreadDef_t task_def;

    /* Create task */
    task_def.name = "user_app";
    task_def.stacksize = g_wts_dut.main_task_stack_size;
    task_def.tpriority = (osPriority)g_wts_dut.main_task_pri;
    task_def.pthread = user_app_entry;
    g_wts_dut.main_task_h = osThreadCreate(&task_def, (void*)NULL);

    if(g_wts_dut.main_task_h == NULL)
    {
        LOGE(TAG, "user_app Task create fail \r\n");
    }
    else
    {
        LOGI(TAG, "user_app Task create successful \r\n");
    }

    osMessageQDef_t queue_def = {0};
    queue_def.item_sz = sizeof( wts_dut_q_msg );
    queue_def.queue_sz = g_wts_dut.main_task_queue_num;
    g_wts_dut.queue_h = osMessageCreate(&queue_def, g_wts_dut.main_task_h);
    if (g_wts_dut.queue_h == NULL)
        LOGE(TAG, "user_app Queue create fail \r\n");
    else
        LOGI(TAG, "user_app Queue create successful \r\n");
}

void wts_dut_main_init(void)
{
    g_wts_dut.main_task_pri        = 0;
    g_wts_dut.main_task_stack_size = 512;
    g_wts_dut.main_task_queue_num  = 1;
    memset(g_wts_dut.serial_buf, 0, WTS_DUT_SERIAL_BUF_LEN);

    wts_dut_serial_config(WTS_DUT_SERIAL_BUF_LEN, (uint8_t *)&g_wts_dut.serial_buf[0]);
    wts_dut_serial_init();

    /* Event Loop Initialization */
    wifi_event_loop_init((wifi_event_cb_t)wifi_event_handler_cb);

    /* Initialize wifi stack and register wifi init complete event handler */
    wifi_init(NULL, NULL);
    
    /* Initial Wifi Setting for this example */
    initial_wifi_config();

    initial_lwip_options();

    /* Initial Network process */
    creat_task_to_init_network();

    /* Initial/Register commands */
    console_init();
}

int wts_dut_is_wifi_connected(void)
{
    int bits = xEventGroupWaitBits(g_wts_dut.event_bits, WIFI_CONNECTED_BIT0, 0, 1, 0);
    if (bits & WIFI_CONNECTED_BIT0)
        return true;
    else
        return false;
}

int wts_dut_is_got_ip(void)
{
    int bits = xEventGroupWaitBits(g_wts_dut.event_bits, GOT_IP_BIT1, 0, 1, 0);
    if (bits & GOT_IP_BIT1)
        return true;
    else
        return false;
}

int wts_dut_task_send_msg(uint32_t cmd, uint32_t buf_len, void *buf)
{
    wts_dut_q_msg *p_msg = NULL;

    p_msg = malloc(sizeof(wts_dut_q_msg));
    if (p_msg == NULL) {
        LOGE(TAG, "malloc queue fail \r\n");
        return -1;
    }

    memset(p_msg, 0, sizeof(wts_dut_q_msg));

    if (buf_len > 0 && buf) {
        p_msg->buf = malloc(buf_len);
        if (p_msg->buf == NULL) {
            LOGE(TAG, "malloc queue buf fail \r\n");
            free(p_msg);
            return -1;
        }

        memcpy(p_msg->buf, buf, buf_len);
        p_msg->buf_len = buf_len;
    }

    p_msg->event = cmd;

    if (osMessagePut(g_wts_dut.queue_h, (uint32_t)p_msg, osWaitForever) != osOK) {
        if (buf_len > 0 && p_msg->buf)
            free(p_msg->buf);
        free(p_msg);
        LOGE(TAG, "send queue fail \r\n");
        return -2;
    }

    return 0;
}
