/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef _BLE_SCAN_APP_H_
#define _BLE_SCAN_APP_H_

#include "ble.h"
#include "ble_msg.h"
#include "ble_gap_if.h"

#define BLE_APP_MAX_SCAN_FILTER_SIZE 20


enum
{
    APP_STATE_INIT,
    APP_STATE_IDLE,
    APP_STATE_ADVERTISING,
    APP_STATE_SCANNING,
    APP_STATE_CONNECTING,
    APP_STATE_CONNECTED,

};

enum
{
    BLE_APP_MSG_INITIALIZING = 1,
    BLE_APP_MSG_ENTER_ADVERTISING,
    BLE_APP_MSG_EXIT_ADVERTISING,
    BLE_APP_MSG_START_SCANNING,
    BLE_APP_MSG_STOP_SCANNING,

    BLE_APP_MSG_TOP
};

enum
{
	BLE_APP_SCAN_FILTER_TYPE_MAC = 1,
    BLE_APP_SCAN_FILTER_TYPE_NAME,
    BLE_APP_SCAN_FILTER_TYPE_NONE
};

typedef struct
{
    TASKPACK            task;
    UINT16              state;
    LE_GAP_SCAN_PARAM_T scan_para;
    UINT8               scan_duration;
    UINT8               scan_duplicate_filter;
    UINT8               scan_filter_type;
    UINT8               scan_filter_buf[BLE_APP_MAX_SCAN_FILTER_SIZE];
    UINT8               scan_filter_dataLen;
    LE_BT_ADDR_T        filter_addr;
} BLE_APP_DATA_T;


void BleAppInit(void);
BLE_APP_DATA_T* BleAppDev(void);

#endif
