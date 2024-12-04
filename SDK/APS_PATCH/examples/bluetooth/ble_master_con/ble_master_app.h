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

#ifndef _BLE_MASTER_APP_H_
#define _BLE_MASTER_APP_H_

#include "ble.h"
#include "ble_msg.h"
#include "ble_queue.h"
#include "ble_gap_if.h"

#define ENABLE_APP_DEBUG
//#define APP_USE_PAIRING

#ifdef ENABLE_APP_DEBUG
#define BLE_APP_PRINT        printf
#else
#define BLE_APP_PRINT(...)
#endif

// Minimum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL         100
// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL         200
// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY            0
// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SUPERVERSION_TIMEOUT       500

#define BLE_APP_MAX_SCAN_FILTER_SIZE 20
#define BLE_SERVICE_COUNT        5

#define LE_APP_ROLE_GATT_CLIENT         1
#define LE_APP_ROLE_GATT_SERVER         2

enum
{
    APP_STATE_INIT,
    APP_STATE_IDLE         = (1 << 0),
    APP_STATE_ADVERTISING  = (1 << 1),
    APP_STATE_SCANNING     = (1 << 2),
    APP_STATE_CONNECTING   = (1 << 3),
    APP_STATE_CONNECTED    = (1 << 4),

};

enum
{
    BLE_APP_MSG_INITIALIZING = 1,
    BLE_APP_MSG_ENTER_ADVERTISING,
    BLE_APP_MSG_EXIT_ADVERTISING,
    BLE_APP_MSG_START_SCANNING,
    BLE_APP_MSG_STOP_SCANNING,
    BLE_APP_MSG_CONN_CANCEL,
    BLE_APP_MSG_FIND_ALL_SERVICE,
    BLE_APP_MSG_FIND_ATTR,

    BLE_APP_MSG_TOP
};

enum
{
	BLE_APP_SCAN_FILTER_TYPE_MAC = 1,
    BLE_APP_SCAN_FILTER_TYPE_NAME,
    BLE_APP_SCAN_FILTER_TYPE_UUID,
    BLE_APP_SCAN_FILTER_TYPE_NONE
};

enum
{
    GATT_ACTION_FIND_ATTR = 1,
    GATT_ACTION_WAIT_INFO,
    GATT_ACTION_WAIT_CHAR_INFO,
    GATT_ACTION_FINISH,
};

typedef struct
{
    UINT16            phandle[4];
    UINT16            handle;
    UINT8             format;
    UINT16            uuid[8];
} BLE_ATTRQ_T;

typedef struct
{
    UINT16            handle;
    UINT16            permit;
    UINT8             format;
    UINT16            uuid[8];
    UINT16            len;
    UINT8            *val;
} BLE_ATT_ATTR_T;

typedef struct
{
    UINT16            start_hdl;
    UINT16            end_hdl;
    UINT8             action;
    UINT8             format;
    UINT16            uuid[8];
    UINT16            attr_count;
    BLE_ATT_ATTR_T    *attr;
} BLE_SERVICE_T;

typedef struct
{
    UINT16            handle;
    UINT16            val_hdl;
    UINT8             property;
    UINT8             format;
    UINT16            dummy;
    UINT16            uuid[8];
} BLE_CHAR_DECL_T;

typedef struct
{
    UINT16            handle;
    UINT16            config;
} BLE_CHAR_CCCD_T;

typedef struct
{
    BLE_CHAR_DECL_T   decl;
    BLE_CHAR_CCCD_T   *cccd;
} BAS_CHAR_T;

typedef struct
{
    BLE_SERVICE_T     *svc;
    BAS_CHAR_T        *bat_level;
} BLE_BAS_INFO_T;

typedef struct
{
    BLE_CHAR_DECL_T   decl;
    BLE_CHAR_CCCD_T   *cccd;
} HRS_CHAR_T;

typedef struct
{
    BLE_SERVICE_T     *svc;
    HRS_CHAR_T        *hrm;
} BLE_HRS_INFO_T;

typedef struct
{
    TASKPACK          task;
    UINT16            state;
    UINT16            conn_hdl;
    LE_BT_ADDR_T      bt_addr;

    UINT16            curr_mtu;
    BOOL              encrypted;
    BOOL              paired;

    UINT16            min_itvl;
    UINT16            max_itvl;
    UINT16            latency;
    UINT16            sv_tmo;

    LE_QLIST_T        attrq;
    BLE_SERVICE_T     *curr_svc;
    UINT8             svc_idx;
    UINT8             svc_count;
    BLE_SERVICE_T     svc[BLE_SERVICE_COUNT];
    UINT16            find;
    UINT8             *wrd_buf;
    UINT16            wrd_len;

    //service
    BLE_BAS_INFO_T    *bas;
    BLE_HRS_INFO_T    *hrs;

    LE_GAP_CONN_PHY_T   conn_para;
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


void BleAppMemDump(char *s, UINT16 len, void *data);
void LeHtcUtilOctetNumToHexStr(void *data, UINT16 len, UINT8 **p);
BOOL LeHtcUtilStringToHexNum(UINT8 *data, UINT16 len, UINT8 *buf);

BOOL BleAppFindServiceCharReq(UINT16 svcUuid);
BOOL BleAppBatteryLevelNotifyReq(UINT8 enable);
BOOL BleAppReadBatteryLevelValue(void);
BOOL BleAppReadBatteryServiceCccd(void);
BOOL BleAppHeartRateMeasNotifyReq(UINT8 enable);
BOOL BleAppReadHeartRateMeasCccd(void);
BOOL BleAppSetDisconnect(void);
BOOL BleAppSetScaning(void);
void BleAppCfgRemoteService(UINT16 srv_uuid);

uint16_t BleAPPFindUuidByHandle(uint16_t handle);
uint16_t BleAppFindHandleByUuid(uint16_t uuid);
uint16_t BleAppFindCccdHandleByUuid(uint16_t char_uuid);


#endif
