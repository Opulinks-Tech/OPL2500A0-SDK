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

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define WPA2_STA_CMD_QUEUE_SIZE 2

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef enum {
    WPA2_STA_SCAN = 0,
} wpa2_sta_cmd_e;

typedef enum {
    WPA2_STA_TASK_WAKEUP = 0,
    WPA2_STA_TASK_SLEEP,
} wpa2_sta_task_e;

typedef struct {
    u32 event_id;
    u32 buf_len;
    u8  *buf;
} wpa2_message_type_t;

typedef struct {
    size_t head;
    size_t tail;
    size_t size;
    void** data;
} wpa2_queue_t;

typedef struct {
    uint32_t cmd_type;
    uint32_t timeout;
    int (*handler)(void *argu);
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
int wpa2_sta_init(void *arg);
int wpa2_sta_deinit(void);
int wpa2_sta_is_init(void);
int wpa2_sta_cmd_register_timeout(uint32_t cmd_type, uint32_t time_sec, int (*cb)(void *argu));

#ifdef __cplusplus
}
#endif
#endif /* _WPA2_MAIN_H_ */
