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

/********************************************
Declaration of Global Variables & Functions
********************************************/
EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT0 = 0x01;
const int GOT_IP_BIT1         = 0x02;

extern T_TracerTaskInfo *g_ptTracerExtTaskInfo;
extern uint32_t g_dwTracerQueueNum;
extern uint32_t g_dwTracerQueueSize;


int dut_main_proc(int argc, char **argv);


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
osThreadId app_task_id;
static const char *TAG="[WTS]:";

static int wifi_event_handler_cb(wifi_event_id_t event_id, void *data, uint16_t length)
{
    switch(event_id) {
    case WIFI_EVENT_STA_CONNECTED:
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT0);
        CtrlWifi_PsStateForce(STA_MODE_PERFORMANCE, 0);
        LOGI(TAG, "Wi-Fi Connected");
        lwip_net_start(WIFI_MODE_STA);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        LOGI(TAG, "Wi-Fi Disonnected");
        xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT0);
        break;
    case WIFI_EVENT_SCAN_COMPLETE:
        break;
    case WIFI_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT0);
        xEventGroupSetBits(wifi_event_group, GOT_IP_BIT1);
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

void initial_wifi_config(void)
{
    wifi_event_group = xEventGroupCreate();
    wifi_auto_connect_set_mode(0); //Disable Auto Connect
}


void dut_main_proc_task(void)
{
    int argc = 3;
    char *argv[3];
    argv[0] = "wfa_dut";
    argv[1] = "eth0";
    argv[2] = "8000";
    dut_main_proc(argc, argv);
}

void user_app_entry(void *args)
{
    /* initial internal log level */
    initial_internal_log_level();
    
    /* Tcpip stack and net interface initialization,  dhcp client process initialization. */
    lwip_network_init(WIFI_MODE_STA);
    
    /* wfa_dut main */
    dut_main_proc_task();

    while (1) {
        osDelay(2000);
    }
//    vTaskDelete(NULL);
}

void creat_task_to_init_network(void)
{
    osThreadDef_t task_def;
    
    /* Create task */
    task_def.name = "user_app";
    task_def.stacksize = OS_TASK_STACK_SIZE_APP;
    task_def.tpriority = OS_TASK_PRIORITY_APP;
    task_def.pthread = user_app_entry;
    app_task_id = osThreadCreate(&task_def, (void*)NULL);

    if(app_task_id == NULL)
    {
        LOGE(TAG, "user_app Task create fail \r\n");
    }
    else
    {
        LOGI(TAG, "user_app Task create successful \r\n");
    }
}

void wts_dut_main_init(void)
{
    /* Event Loop Initialization */
    wifi_event_loop_init((wifi_event_cb_t)wifi_event_handler_cb);

    /* Initialize wifi stack and register wifi init complete event handler */
    wifi_init(NULL, NULL);
    
    /* Initial Wifi Setting for this example */
    initial_wifi_config();
    
    /* Initial Network process */
    creat_task_to_init_network();
    
    /* Initial/Register commands */
    console_init();
}

int wts_dut_is_wifi_connected(void)
{
    int bits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT0, 0, 1, 0);
    if (bits & WIFI_CONNECTED_BIT0)
        return true;
    else
        return false;
}

int wts_dut_is_got_ip(void)
{
    int bits = xEventGroupWaitBits(wifi_event_group, GOT_IP_BIT1, 0, 1, 0);
    if (bits & GOT_IP_BIT1)
        return true;
    else
        return false;
}

