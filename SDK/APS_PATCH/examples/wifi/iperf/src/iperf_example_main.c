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
#include "iperf_example_main.h"
#include "iperf.h"
#include "sys_common_api.h"
#include "wifi_api.h"
#include "wifi_types.h"
#include "wifi_event.h"
#include "wifi_event_handler.h"
#include "msg.h"
#include "opulinks_log.h"
#include "controller_wifi.h"
#include "ipc.h"
#include "sys_cfg.h"

/*
#define APP_TX_DESC_NUM     32
#define APP_RX_DESC_NUM     24

#define APP_DESC_POOL_SIZE  ((APP_TX_DESC_NUM + APP_RX_DESC_NUM) * IPC_WIFI_DESC_SIZE)
#define APP_MEM_POOL_SIZE   (APP_TX_DESC_NUM * IPC_WIFI_APS_TX_MEM_BLK_SIZE + (APP_RX_DESC_NUM - 1) * IPC_WIFI_APS_RX_MEM_BLK_SIZE + IPC_WIFI_APS_RX_LAST_MEM_BLK_SIZE)
*/

/********************************************
Declaration of Global Variables & Functions
********************************************/
EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT0    = 0x01;
const int DISCONNECTED_BIT1 = 0x02;

extern T_TracerTaskInfo *g_ptTracerExtTaskInfo;

static void initial_internal_log_level(void);

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
osThreadId app_task_id;
static const char *TAG="IPERF";

/*
static uint8_t g_u8aAppDescPool[APP_DESC_POOL_SIZE] __attribute__((aligned(16))) = {0};
static uint8_t g_u8aAppMemPool[APP_MEM_POOL_SIZE] __attribute__((aligned(4))) = {0};
*/

static int wifi_event_handler_cb(wifi_event_id_t event_id, void *data, uint16_t length)
{
    switch(event_id) {
    case WIFI_EVENT_INIT_COMPLETE:
        /* initial internal log level */
        initial_internal_log_level();

        LOGI(TAG, "Wi-Fi Init Complete");

        /* Tcpip stack and net interface initialization,  dhcp client process initialization. */
        lwip_network_init(WIFI_MODE_STA);
        break;
    case WIFI_EVENT_STA_CONNECTED:
        LOGI(TAG, "Wi-Fi Connected");
        wifi_config_set_ps(WIFI_PS_NONE);
        lwip_net_start(WIFI_MODE_STA);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        LOGI(TAG, "Wi-Fi Disonnected ret %d", *((uint8_t*)data));
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT0);
        xEventGroupSetBits(wifi_event_group, DISCONNECTED_BIT1);
        break;
    case WIFI_EVENT_SCAN_COMPLETE:
        break;
    case WIFI_EVENT_STA_GOT_IP:
        LOGI(TAG, "Wi-Fi Got IP");
        xEventGroupClearBits(wifi_event_group, DISCONNECTED_BIT1);
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT0);
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
/*
    int i;
    for (i=0; i<15; i++) {
        tracer_log_level_set(i, 0);
    }
*/
    tracer_log_level_set(8, 4);
    
    strncpy(g_ptTracerExtTaskInfo[0].baName, IPERF_CLIENT_TASK_NAME, 
           (strlen(IPERF_CLIENT_TASK_NAME) > 15) ? 15 : strlen(IPERF_CLIENT_TASK_NAME));
    strncpy(g_ptTracerExtTaskInfo[1].baName, IPERF_SERVER_TASK_NAME, 
           (strlen(IPERF_SERVER_TASK_NAME) > 15) ? 15 : strlen(IPERF_SERVER_TASK_NAME));
    strncpy(g_ptTracerExtTaskInfo[2].baName, IPERF_REPORT_TASK_NAME, 
           (strlen(IPERF_REPORT_TASK_NAME) > 15) ? 15 :  strlen(IPERF_REPORT_TASK_NAME));
    
    g_ptTracerExtTaskInfo[0].bLevel = 3;
    g_ptTracerExtTaskInfo[1].bLevel = 3;
    g_ptTracerExtTaskInfo[2].bLevel = 7;
} 

static void initial_wifi_config(void)
{
    wifi_event_group = xEventGroupCreate();
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
    option.tcpip.tcpip_mbox_size     = 32;
    option.tcpip.tcp_recv_mbox_size  = 32;
    option.tcpip.udp_recv_mbox_size  = 32;
    option.tcpip.raw_recv_mbox_size  = 32;
    option.tcpip.accept_mbox_size    = 32;
    option.retrans.mode              = OPL_LWIP_TCP_COARSE_GRAINED;

    opl_lwip_option_set(OPL_LWIP_CUSTOMER, &option);

    opl_lwip_mtu_update = option.ip.mtu;

}

/*
static void ipc_buf_num_update(void)
{
    T_IpcDescCfg tCfg = {0};

    tCfg.u32DescPool = (uint32_t)g_u8aAppDescPool;
    tCfg.u32MemPool = (uint32_t)g_u8aAppMemPool;
    tCfg.u32ApsTxNum = APP_TX_DESC_NUM;
    tCfg.u32ApsRxNum = APP_RX_DESC_NUM;

    ipc_desc_update(&tCfg);
    sys_cfg_ipc_desc_set(&tCfg, sizeof(tCfg));
}
*/

void AppInit(void)
{
    /* Event Loop Initialization */
    wifi_event_loop_init((wifi_event_cb_t)wifi_event_handler_cb);

    /* Initialize wifi stack and register wifi init complete event handler */
    wifi_init(NULL, NULL);
    
    /* Initial Wifi Setting for this example */
    initial_wifi_config();

    initial_lwip_options();

    /* Initial/Register commands */
    console_init();

    /* Update number of IPC APS TX/RX buffer */
    //ipc_buf_num_update();

    printf("\n ===========================================\n");
    printf(" |     Start to test WiFi throughput       |\n");
    printf(" |  1. Print 'help' to get command list    |\n");
    printf(" |  2. Setup WiFi connection               |\n");
    printf(" |  3. Run iperf to test throughput        |\n");
    printf(" ===========================================\n");
}
