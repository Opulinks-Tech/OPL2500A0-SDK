/* *****************************************************************************
 *  Copyright 2017-2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file wpa2_main.h
 * 
 *  @brief 
 *  
 ******************************************************************************/
 
#ifndef _WPA2_MAIN_H_
#define _WPA2_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "utils/common.h"
#include "common/bss.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define WPA2_STA_CMD_QUEUE_SIZE 3

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef enum {
    WPA2_STA_CMD_SCAN,
    WPA2_STA_CMD_CONNECT,
    WPA2_STA_CMD_DISCONNECT,
} wpa2_sta_cmd_e;

typedef enum {
    WPA2_STA_TASK_INIT,
    WPA2_STA_TASK_DEINIT,
    WPA2_STA_TASK_WAKEUP,
    WPA2_STA_TASK_CONN_ABORT,
    WPA2_STA_TASK_SCAN_ABORT,
    WPA2_STA_TASK_PREPARE_WPA,
    WPA2_STA_TASK_RX_AUTH_START,
#ifdef CONFIG_APS_HANDLE_AUTH_ASSOC
    WPA2_STA_TASK_RX_MGMT_FRAME,
#endif
    WPA2_STA_TASK_RX_AUTH,
    WPA2_STA_TASK_RX_ASSOC,
    WPA2_STA_TASK_RX_EAPOL,
    WPA2_STA_TASK_RX_DEAUTH,
} wpa2_sta_task_e;

typedef enum {
    WPA2_STA_CMD_STATE_IDLE,
    WPA2_STA_CMD_STATE_CANCELED,
    WPA2_STA_CMD_STATE_DONE,
} wpa2_sta_cmd_state_e;

typedef int (*wpa2_sta_cmd_user_callback)(void *argu, void *user_ctx);
typedef int (*wpa2_sta_cmd_do_handler)(void *p_h);
typedef int (*wpa2_sta_notify_user_disconnect_handler)(void *argu, void *user_ctx);

typedef struct {
    uint32_t msg_id;
    void    *buf;
    uint32_t buf_len;
    int      status;
} wpa2_sta_task_msg_t;

typedef struct {
    size_t head;
    size_t tail;
    size_t size;
    void** data;
} wpa2_queue_t;

typedef struct {
    int state;
    uint32_t timeout_sec;
    struct os_time timeout;
#ifdef CONFIG_APS_HANDLE_AUTH_ASSOC
    uint32_t auth_count_down;
    uint32_t assoc_count_down;
#endif
    wpa2_sta_cmd_e cmd;
    wpa2_sta_cmd_do_handler do_h;
    wpa2_sta_cmd_do_handler is_canceled_h;
    wpa2_sta_cmd_do_handler is_timeout_h;
    wpa2_sta_cmd_do_handler is_done_h;
    wpa2_sta_cmd_user_callback user_cb;
    void *p_user_data;
    int return_val;
} wpa2_sta_cmd_cfg_t;

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
int wpa2_sta_init(void);
int wpa2_sta_deinit(void);
int wpa2_sta_is_init(void);
int wpa2_sta_notify_user_disconnect_cb(wpa2_sta_notify_user_disconnect_handler cb);
int wpa2_sta_cmd_scan_start(void *argu, uint32_t timeout_sec, int (*cb)(void *argu, void *user_ctx));
int wpa2_sta_cmd_scan_stop(void *argu, uint32_t timeout_sec, wpa2_sta_cmd_user_callback cb);
int wpa2_sta_cmd_connect_start(void *argu, uint32_t timeout_sec, wpa2_sta_cmd_user_callback cb);
int wpa2_sta_cmd_connect_stop(void *argu, uint32_t timeout_sec, wpa2_sta_cmd_user_callback cb);
int wpa2_sta_cmd_disconnect(void *argu, uint32_t timeout_sec, wpa2_sta_cmd_user_callback cb);
int wpa2_sta_cmd_state(struct wpa_bss **bss);
void wpa2_sta_deauthenticate(uint8_t reason_code);;

#ifdef __cplusplus
}
#endif
#endif /* _WPA2_MAIN_H_ */
