/******************************************************************************
*  Copyright 2017 - 2021, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2021
******************************************************************************/

#include "ble_master_app.h"
#include "ble_hci_if.h"
#include "ble_cm_if.h"
#include "ble_smp_if.h"
#include "ble_gap_if.h"
#include "ble_gatt_if.h"
#include "ble_util.h"
#include "ble_client_app_gatt.h"
#include "ble_uuid.h"
#include "ble_adv_parser.h"
#include "ble_app_at_cmd.h"
#include "opulinks_log.h"
#include "msg.h"

// Scan duration in sec
#define DEFAULT_SCAN_DURATION                       5
// Scan interval in units of 0.625 ms
#define DEFAULT_SCAN_INTERVAL                       160  //(160 * 0.625) = 100 ms
// Scan window in units of 0.625 ms
#define DEFAULT_SCAN_WINDOW                         80   //(80 * 0.625) = 50 ms


// ADV Report types
#define GAP_ADV_RPT_TYPE_CONN_SCAN                  0       /* ADV_IND */
#define GAP_ADV_RPT_TYPE_CONN_DIR                   1       /* ADV_DIRECT_IND */
#define GAP_ADV_RPT_TYPE_SCAN                       2       /* ADV_SCAN_IND */
#define GAP_ADV_RPT_TYPE_NONCONN_NONSCAN            3       /* ADV_NONCONN_IND */
#define GAP_ADV_RPT_TYPE_SCAN_RSP                   4       /* SCAN_RSP */

// The remote service we are interested in.
#define REMOTE_SERVICE_UUID         ATT_SVC_HEART_RATE

uint16_t BleAppFindCccdHandleByUuid(uint16_t uuid);
uint16_t BleAppFindHandleByUuid(uint16_t uuid);

#define BT_BD_ADDR_HEX(addr)   addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]
#define ADDRSTR "%02x:%02x:%02x:%02x:%02x:%02x"

static BLE_APP_DATA_T gTheBle;

uint16_t g_target_uuid = REMOTE_SERVICE_UUID;
const char g_target_periph_name[] = "OPL_APP_18:41:1A:6A"; /**< If you want to connect to a peripheral using a given advertising name, type its name here. */
bool is_connect_per_addr = true;                           /**< If you want to connect to a peripheral with a given address, set this to true and put the correct address in the variable below. */

LE_BT_ADDR_T g_target_periph_addr =
{
  .type = LE_HCI_PEER_ADDR_PUBLIC,
  .addr = {0x6A, 0x1A, 0x41, 0x18, 0x4A, 0x88}
};

bool is_filter_matched = false;

/* Scan interval.
 * Range: 0x0004 to 0x4000 Default: 0x0010 (10 ms)
 * Time = N * 0.625 msec
 * Time Range: 2.5 msec to 10.24 seconds
 */

 /* Scan window. The duration of the LE scan. LE_Scan_Window shall be
 * less than or equal to LE_Scan_Interval
 * Range: 0x0004 to 0x4000 Default: 0x0010 (10 ms)
 * Time = N * 0.625 msec
 * Time Range: 2.5 msec to 10240 msec
 */

LE_GAP_SCAN_PARAM_T default_scan_params = {
    .interval      = DEFAULT_SCAN_INTERVAL,
    .window        = DEFAULT_SCAN_WINDOW,
    .type          = LE_HCI_SCAN_ACTIVE,
    .own_addr_type = LE_HCI_OWN_ADDR_PUBLIC,
    .filter_policy = LE_HCI_INIT_FILT_NONE
};


LE_GAP_CONN_PHY_T default_conn_params = {
    .interval_max   = 80,         //multiple of 1.25ms in the range of 7.5ms to 4s
    .interval_min   = 80,
    .scan_interval  = 100,
    .scan_window    = 50,
    .latency        = 0,         //Slave latency for the connection in number of connection events. Range: 0x0000 to 0x01F3 */
    .supervision_timeout = 250   //Mandatory Range: 0x000A to 0x0C80 Time = N * 10 msec
                                 //Time Range: 100 msec to 32 seconds */
};

uint16_t BleAPPFindUuidByHandle(uint16_t handle)
{
    BLE_SERVICE_T *service = gTheBle.svc;
    size_t service_count = gTheBle.svc_count;

    for (int i = 0; i < service_count; ++i)
    {
        for (int j = 0; j < service[i].attr_count; ++j) {
            BLE_ATT_ATTR_T *attr = &service[i].attr[j];

            if (attr->handle == handle) {
                return attr->uuid[0];
            }
        }
    }

    return 0;
}

uint16_t BleAppFindHandleByUuid(uint16_t uuid)
{
    BLE_SERVICE_T *service = gTheBle.svc;
    size_t service_count = gTheBle.svc_count;

    for (int i = 0; i < service_count; ++i)
    {
        for (int j = 0; j < service[i].attr_count; ++j) {
            BLE_ATT_ATTR_T *attr = &service[i].attr[j];

            if (attr->uuid[0] == uuid) {
                return attr->handle;
            }
        }
    }
    return 0;
}

uint16_t BleAppFindCccdHandleByUuid(uint16_t char_uuid)
{
    BLE_SERVICE_T *service = gTheBle.svc;
    size_t service_count = gTheBle.svc_count;

    for (int i = 0; i < service_count; ++i)
    {
        for (int j = 0; j < service[i].attr_count; ++j)
        {
            BLE_ATT_ATTR_T *attr = &service[i].attr[j];

            if (attr->uuid[0] == GATT_CHARACTERISTIC_UUID &&
                j + 1 < service[i].attr_count && service[i].attr[j + 1].uuid[0] == char_uuid) {

                for (int k = j + 2; k < service[i].attr_count; ++k)
                {
                    if (service[i].attr[k].uuid[0] == GATT_CHARACTERISTIC_UUID) {
                        break;
                    }

                    if (service[i].attr[k].uuid[0] == GATT_CLIENT_CHAR_CFG_UUID) {
                        return service[i].attr[k].handle;
                    }
                }
            }
        }
    }

    return 0;
}

void BleAppFindBatteryChar(void)
{
    LE_ERR_STATE rc = SYS_ERR_SUCCESS;
    BLE_SERVICE_T *svc = gTheBle.svc;

    UINT16 BatterySvcUuid = ATT_SVC_BATTERY_SERVICE;
    UINT16 i;

    for (i = 0; i < gTheBle.svc_count; i++)
    {
        if ((svc[i].format == LE_GATT_UUID16) && !MemCmp(&BatterySvcUuid, svc[i].uuid, 2))
        {
            rc = LeGattFindAllCharacteristic(gTheBle.conn_hdl, svc[i].start_hdl + 1, svc[i].end_hdl);

            if (rc == SYS_ERR_SUCCESS)
            {
                if (!gTheBle.bas)
                {
                    BLE_BAS_INFO_T *bas = PanicUnlessMalloc(sizeof(BLE_BAS_INFO_T));
                    gTheBle.bas = bas;
                    svc[i].action = GATT_ACTION_WAIT_CHAR_INFO;
                    bas->svc = &svc[i];
                }
            }
            break;
        }
    }
}

void BleAppFindHRSChar(void)
{
    LE_ERR_STATE rc = SYS_ERR_SUCCESS;
    BLE_SERVICE_T *svc = gTheBle.svc;

    UINT16 BatterySvcUuid = ATT_SVC_HEART_RATE;
    UINT16 i;

    for (i = 0; i < gTheBle.svc_count; i++)
    {
        if ((svc[i].format == LE_GATT_UUID16) && !MemCmp(&BatterySvcUuid, svc[i].uuid, 2))
        {
            rc = LeGattFindAllCharacteristic(gTheBle.conn_hdl, svc[i].start_hdl + 1, svc[i].end_hdl);

            if (rc == SYS_ERR_SUCCESS)
            {
                if (!gTheBle.bas)
                {
                    BLE_HRS_INFO_T *hrs = PanicUnlessMalloc(sizeof(BLE_HRS_INFO_T));
                    gTheBle.hrs = hrs;
                    svc[i].action = GATT_ACTION_WAIT_CHAR_INFO;
                    hrs->svc = &svc[i];
                }
            }

            break;
        }
    }
}

BOOL BleAppFindServiceCharReq(UINT16 svcUuid)
{
    BLE_APP_DATA_T *app = BleAppDev();

    if (app->state == APP_STATE_CONNECTED)
    {
        if (svcUuid == ATT_SVC_BATTERY_SERVICE)
            BleAppFindBatteryChar();
        else if (svcUuid == ATT_SVC_HEART_RATE)
            BleAppFindHRSChar();
    }
    else
        return FALSE;

    return TRUE;
}

BOOL BleAppBatteryLevelNotifyReq(UINT8 enable)
{
    BLE_APP_DATA_T *app = BleAppDev();
    LE_ERR_STATE rc = SYS_ERR_SUCCESS;
    UINT16 cccd_handle = 0;

    if (enable > 1) return FALSE;
    if (app->state != APP_STATE_CONNECTED) return FALSE;

    if (enable == 1)
    {
        UINT8 cccd[2] = {0x01, 0x00};
        cccd_handle = BleAppFindCccdHandleByUuid(ATT_CHAR_BATTERY_LEVEL);
        rc = LeGattWriteCharVal(gTheBle.conn_hdl, cccd_handle/*gTheBle.bas->bat_level->cccd->handle*/ , 2, cccd);
    }
    else
    {
        UINT8 cccd[2] = {0x00, 0x00};
        cccd_handle = BleAppFindCccdHandleByUuid(ATT_CHAR_BATTERY_LEVEL);
        rc = LeGattWriteCharVal(gTheBle.conn_hdl, cccd_handle/*gTheBle.bas->bat_level->cccd->handle*/ , 2, cccd);
    }

    BLE_APP_PRINT("LeGattWriteCharVal hdl:%d, value = %d\r\n", cccd_handle, enable);

    return (rc==SYS_ERR_SUCCESS)? TRUE : FALSE;
}

BOOL BleAppReadBatteryLevelValue(void)
{
    BLE_APP_DATA_T *app = BleAppDev();

    LE_ERR_STATE rc = SYS_ERR_SUCCESS;

    if (app->state == APP_STATE_CONNECTED)
    {
        UINT16 uuid = ATT_CHAR_BATTERY_LEVEL;
        rc = LeGattReadCharValByUuid(app->conn_hdl, 0xc, 0xE, LE_GATT_UUID16, &uuid);
        BLE_APP_PRINT("LeGattReadCharValByUuid rc = %d\r\n", rc);
    }
    else
        return FALSE;

    return TRUE;
}

BOOL BleAppReadBatteryServiceCccd(void)
{
    BLE_APP_DATA_T *app = BleAppDev();

    LE_ERR_STATE rc = SYS_ERR_SUCCESS;

    if (app->state == APP_STATE_CONNECTED)
    {
        UINT16 cccd_handle = BleAppFindCccdHandleByUuid(ATT_CHAR_BATTERY_LEVEL);
        rc = LeGattReadLongCharVal(gTheBle.conn_hdl, cccd_handle, 0);
        BLE_APP_PRINT("LeGattReadLongCharVal rc = %d\r\n", rc);
    }
    else
        return FALSE;

    return TRUE;
}

BOOL BleAppHeartRateMeasNotifyReq(UINT8 enable)
{
    BLE_APP_DATA_T *app = BleAppDev();
    LE_ERR_STATE rc = SYS_ERR_SUCCESS;
    UINT16 cccd_handle = 0;

    if (enable > 1) return FALSE;
    if (app->state != APP_STATE_CONNECTED) return FALSE;

    if (enable == 1)
    {
        UINT8 cccd[2] = {0x01, 0x00};
        cccd_handle = BleAppFindCccdHandleByUuid(ATT_CHAR_HEART_RATE_MEAS);
        rc = LeGattWriteCharVal(gTheBle.conn_hdl, cccd_handle/*gTheBle.bas->bat_level->cccd->handle*/ , 2, cccd);
    }
    else
    {
        UINT8 cccd[2] = {0x00, 0x00};
        cccd_handle = BleAppFindCccdHandleByUuid(ATT_CHAR_HEART_RATE_MEAS);
        rc = LeGattWriteCharVal(gTheBle.conn_hdl, cccd_handle/*gTheBle.bas->bat_level->cccd->handle*/ , 2, cccd);
    }

    BLE_APP_PRINT("LeGattWriteCharVal hdl:%d, value = %d\r\n", cccd_handle, enable);

    return (rc==SYS_ERR_SUCCESS)? TRUE : FALSE;
}

BOOL BleAppReadHeartRateMeasCccd(void)
{
    BLE_APP_DATA_T *app = BleAppDev();

    LE_ERR_STATE rc = SYS_ERR_SUCCESS;

    if (app->state == APP_STATE_CONNECTED)
    {
        UINT16 cccd_handle = BleAppFindCccdHandleByUuid(ATT_CHAR_HEART_RATE_MEAS);
        rc = LeGattReadLongCharVal(gTheBle.conn_hdl, cccd_handle, 0);
        BLE_APP_PRINT("LeGattReadLongCharVal rc = %d\r\n", rc);
    }
    else
        return FALSE;

    return TRUE;
}

void BleAppCfgRemoteService(UINT16 srv_uuid)
{
    g_target_uuid = srv_uuid;
}

BOOL BleAppSetDisconnect(void)
{
    BLE_APP_DATA_T *app = BleAppDev();
    LeGapDisconnectReq(app->conn_hdl);
    return TRUE;
}

BOOL BleAppSetScaning(void)
{
    BLE_APP_DATA_T *app = BleAppDev();
    LeSendMessageAfter(&app->task, BLE_APP_MSG_START_SCANNING, 0, T_SEC(1));
    return TRUE;
}

void BleAppMemDump(char *s, UINT16 len, void *data)
{
    UINT16 i;
    UINT8 *p = data;

    if (s)
    {
        BLE_APP_PRINT("\r\n%s:<", s);
    }

    for (i = 0; i < len; i++)
    {
        if (i < len - 1)
        {
            BLE_APP_PRINT("%02x ", p[i]);
        }
        else
        {
            BLE_APP_PRINT("%02x", p[i]);
        }
    }

    if (s)
    {
        BLE_APP_PRINT(">\r\n");
    }
    else
    {
        BLE_APP_PRINT("\r\n");
    }
}
static void BleAppShowAttrInfo(void)
{
    BLE_SERVICE_T *svc = gTheBle.svc;
    UINT16 i, j;

    BLE_APP_PRINT("\r\nAttribute Table\r\n");
    BLE_APP_PRINT("\r\nHandle    format    uuid\r\n");

    for (i = 0; i < BLE_SERVICE_COUNT; i++)
    {
        if (svc[i].action == GATT_ACTION_FINISH)
        {
            if (svc[i].format == LE_GATT_UUID16)
                BLE_APP_PRINT(COLOR_GREEN"  %04x      %d       %04x\r\n"COLOR_RST, svc[i].start_hdl, svc[i].format, svc[i].uuid[0]);
            else
            {
                BLE_APP_PRINT("  %04x      %d\r\n", svc[i].start_hdl, svc[i].format);

            }

            for (j = 0; j < svc[i].attr_count; j++)
            {
                if (svc[i].format == LE_GATT_UUID16)
                    BLE_APP_PRINT(COLOR_YELLOW "  %04x      %d       %04x\r\n"COLOR_RST, svc[i].attr[j].handle, svc[i].attr[j].format, svc[i].attr[j].uuid[0]);
                else
                {
                    BLE_APP_PRINT("  %04x      %d\r\n", svc[i].attr[j].handle, svc[i].attr[j].format);

                }
            }
           }
    }

}

static void BleAppFindInfo(void)
{
    LE_ERR_STATE rc = SYS_ERR_SUCCESS;
    BLE_SERVICE_T *svc = gTheBle.svc;
    UINT16 i;

    for (i = 0; i < BLE_SERVICE_COUNT; i++)
    {
        if ((svc[i].action == GATT_ACTION_FIND_ATTR) && (svc[i].start_hdl < svc[i].end_hdl))
        {
            BLE_APP_PRINT("GATT find all characteristic descriptor\r\n");
            rc = LeGattFindAllCharDescriptor(gTheBle.conn_hdl, svc[i].start_hdl + 1, svc[i].end_hdl);

            if (rc == SYS_ERR_SUCCESS)
            {
                gTheBle.curr_svc = &svc[i];
                svc[i].action = GATT_ACTION_WAIT_INFO;
                svc[i].attr_count = 0;

                return;
            }
        }
    }

    gTheBle.curr_svc = 0;
    gTheBle.svc_idx = 0;

    BleAppShowAttrInfo();

    //read peer device name char 0x2A00
    uint16_t handle = BleAppFindHandleByUuid(ATT_CHAR_DEVICE_NAME);
    rc = LeGattReadLongCharVal(gTheBle.conn_hdl, handle, 0);

#if 0
{
    UINT16 char_uuid = 0x2A37; // Heart Rate Measurement Characteristic UUID
    uint16_t handle, cccd_handle;

    handle = BleAppFindHandleByUuid(char_uuid);
    if (handle) {
        printf("Handle for characteristic UUID 0x%04X is 0x%04X\n", char_uuid, handle);
    } else {
        printf("Handle not found for characteristic UUID 0x%04X\n", char_uuid);
    }

    cccd_handle = BleAppFindCccdHandleByUuid(char_uuid);
    printf("CCCD handle for characteristic UUID 0x%04X is 0x%04X\n", char_uuid, cccd_handle);
}
#endif
}

void BleAppGattInfoClean(void)
{
    BLE_SERVICE_T *svc = gTheBle.svc;
    size_t service_count = gTheBle.svc_count;

    for (int i = 0; i < service_count; ++i)
    {
         BLE_ATT_ATTR_T *attr = &svc[i].attr[0];

        if (attr) {
            //BLE_APP_PRINT("free attr handle %d\n", attr->handle);
            FreeMem(attr);
        }
    }

    memset(&gTheBle.svc, 0, sizeof(gTheBle.svc));
    if (LeQueueCount(&BleAppDev()->attrq)) LeQueueBroken(&BleAppDev()->attrq);
}

void BleAppGetAdvNameStr(UINT8* data, UINT8 len, UINT8* namebuf)
{
    UINT8 data_pos = 0;
    UINT8 adlen,adtype;
    while(data_pos < len)
    {
        adlen = data[data_pos];
        adtype = data[data_pos+1];
        if(adtype == GAP_ADTYPE_LOCAL_NAME_SHORT || adtype == GAP_ADTYPE_LOCAL_NAME_COMPLETE)
        {
            MemCopy(namebuf,&data[data_pos+2],adlen-1);
            break;
        }
        else
            data_pos += 1+adlen;
    }
}

static void BleAppAdvReportHandler(UINT8 is_rsp,UINT8 rpt_type,BD_ADDR addr,UINT8 addrType,INT8 rssi,UINT8 dataLen,UINT8 *data)
{
    BLE_APP_DATA_T *app = BleAppDev();

    UINT8 addrstr[2][7] = {"Public","Random"};
    UINT8 namestr[31]={0};
    UINT8 datastr[62]={0};
    UINT8* p = datastr;


    if (app->state == (APP_STATE_CONNECTED | APP_STATE_SCANNING))
        goto DIR_REPORT;


    if((app->scan_filter_type == BLE_APP_SCAN_FILTER_TYPE_MAC) && memcmp(addr,app->filter_addr.addr,6))
        return;

    if(is_rsp && !dataLen)
       return;

    if((app->scan_filter_type == BLE_APP_SCAN_FILTER_TYPE_MAC) && !memcmp(addr,app->filter_addr.addr,6))
    {
        BLE_APP_PRINT("target peripheral address match\r\n");
        app->filter_addr.type = addrType;
        is_filter_matched = true;
    }
    else if ((app->scan_filter_type == BLE_APP_SCAN_FILTER_TYPE_NAME) && ble_advdata_name_find(data, dataLen, g_target_periph_name))
    {
        BLE_APP_PRINT("Name match, send connect_request\r\n");
        memcpy(app->filter_addr.addr, addr, 6);
        app->filter_addr.type = addrType;
        is_filter_matched = true;
        LeCancelAllMessage(&app->task, BLE_APP_MSG_STOP_SCANNING);
        LeGapSetScanDisable();
    }
    else if ((app->scan_filter_type == BLE_APP_SCAN_FILTER_TYPE_UUID) && ble_advdata_uuid_find(data, dataLen, &g_target_uuid))
    {
        BLE_APP_PRINT("UUID match, send connect_request\r\n");
        memcpy(app->filter_addr.addr, addr, 6);
        app->filter_addr.type = addrType;
        is_filter_matched = true;

        LeCancelAllMessage(&app->task, BLE_APP_MSG_STOP_SCANNING);
        LeGapSetScanDisable();
    }

    if (!is_filter_matched && app->scan_filter_type != BLE_APP_SCAN_FILTER_TYPE_NONE)
        return;

    parse_advertising_data(data, dataLen);

DIR_REPORT:

    //Print scan response data or advertising data
    LeHtcUtilOctetNumToHexStr(data, dataLen, &p);

    BLE_APP_PRINT("\r\nAddr:%02X:%02X:%02X:%02X:%02X:%02X, RSSI:%02d dB, Address type:%s, %s \r\n", BT_BD_ADDR_HEX(addr), rssi, addrstr[addrType], namestr);
    BLE_APP_PRINT("%s Data <%02d>:%s\r\n",is_rsp?"RSP":"ADV", dataLen, datastr);
}


static void BleAppSetScanPara(UINT8 type, UINT8 own_addr_type, LE_BT_ADDR_T *peer_addr, UINT8 filter_type)
{
    BLE_APP_DATA_T *app = BleAppDev();

    app->scan_para.type = type;
    app->scan_para.interval = 160;  //(160 * 0.625) = 100 ms
    app->scan_para.window = 80;     //(80  * 0.625) = 50 ms
    app->scan_para.own_addr_type = own_addr_type;
    app->scan_para.filter_policy = LE_HCI_SCAN_FILT_NONE;

    if (peer_addr && filter_type == BLE_APP_SCAN_FILTER_TYPE_MAC)
    {
        LeGapWhiteListClear();
        LeGapWhiteListAdd(peer_addr->type, peer_addr->addr);
        app->scan_para.filter_policy |= LE_HCI_SCAN_FILT_WHITELIST;
    }

    LeGapSetScanParameter(&app->scan_para);
}

static void BleAppCmMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    BLE_APP_DATA_T *app = BleAppDev();

    switch (id)
    {
        case LE_CM_MSG_CONN_UPDATE_CFM:
            BLE_APP_PRINT("LE_CM_MSG_CONN_UPDATE_CFM\r\n");
            break;
        case LE_CM_MSG_INIT_COMPLETE_CFM:
        {
            BLE_APP_PRINT("LE_CM_MSG_INIT_COMPLETE_CFM\r\n");
#ifdef APP_USE_PAIRING
            LeSmpInit(&app->task);
            LeSmpSetDefaultConfig(LE_SM_IO_CAP_NO_IO, FALSE, FALSE, TRUE);
#endif
            BleAppSetScanPara(LE_HCI_SCAN_ACTIVE, LE_HCI_OWN_ADDR_PUBLIC, &app->filter_addr, app->scan_filter_type);
            app->state = APP_STATE_IDLE;
        }
        break;

        case LE_CM_MSG_SET_ADVERTISING_DATA_CFM:
            BLE_APP_PRINT("LE_CM_MSG_SET_ADVERTISING_DATA_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_ADVERTISING_DATA_CFM_T *)message)->status);
        break;

        case LE_CM_MSG_SET_SCAN_RSP_DATA_CFM:
        {
            BLE_APP_PRINT("LE_CM_MSG_SET_SCAN_RSP_DATA_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_SCAN_RSP_DATA_CFM_T *)message)->status);
        }
        break;

        case LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM:
        {
            BLE_APP_PRINT("LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM - Status = %x\r\n", ((LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM_T *)message)->status);
        }
        break;

        case LE_CM_MSG_ENTER_ADVERTISING_CFM:
        {
            LE_CM_MSG_ENTER_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_ENTER_ADVERTISING_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_ENTER_ADVERTISING_CFM Status = %x\r\n", cfm->status);
        }
        break;

        case LE_CM_MSG_EXIT_ADVERTISING_CFM:
        {
            LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_EXIT_ADVERTISING_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_EXIT_ADVERTISING_CFM Status = %x\r\n", cfm->status);
        }
        break;

        case LE_CM_MSG_CREATE_CONNECTION_CFM:
        {
            LE_CM_MSG_CREATE_CONNECTION_CFM_T *cfm = (LE_CM_MSG_CREATE_CONNECTION_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_CREATE_CONNECTION_CFM status = %x\r\n", cfm->status);
            app->state = APP_STATE_CONNECTING;
        }
        break;

        case LE_CM_MSG_CANCEL_CONNECTION_CFM:
        {
            LE_CM_MSG_CANCEL_CONNECTION_CFM_T *cfm = (LE_CM_MSG_CANCEL_CONNECTION_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_CANCEL_CONNECTION_CFM status = %x\r\n", cfm->status);
            app->state &= ~APP_STATE_CONNECTING;
            app->state = APP_STATE_IDLE;
        }
        break;

        case LE_CM_MSG_CONNECTION_COMPLETE_IND:
        {
            LE_CM_MSG_CONNECTION_COMPLETE_IND_T *ind = (LE_CM_MSG_CONNECTION_COMPLETE_IND_T *)message;

            BLE_APP_PRINT("LE_CM_MSG_CONNECTION_COMPLETE_IND status = %x\r\n", ind->status);
            app->state &= ~APP_STATE_CONNECTING;

            if (ind->status == SYS_ERR_SUCCESS)
            {
               app->state = APP_STATE_CONNECTED;

                BLE_APP_PRINT("Peer role= %s\r\n", (ind->role == LE_APP_ROLE_GATT_SERVER) ? "Master" : "Slave");
                BLE_APP_PRINT("Peer address:\""ADDRSTR"\"(%s)\r\n", BT_BD_ADDR_HEX(ind->peer_addr),
                    ind->peer_addr_type ? "Random Address" : "Public Address");

                gTheBle.conn_hdl = ind->conn_hdl;
                gTheBle.bt_addr.type = ind->peer_addr_type;
                MemCopy(gTheBle.bt_addr.addr, ind->peer_addr, 6);

                gTheBle.max_itvl = ind->conn_interval;
                gTheBle.latency = ind->conn_latency;
                gTheBle.sv_tmo = ind->supervison_timeout;
                LeCancelAllMessage(&app->task, BLE_APP_MSG_CONN_CANCEL);

#if 0
                LE_CONN_PARA_T default_conn_params = {
                    .itv_max = 80,
                    .itv_min = 60,
                    .latency = 0,
                    .sv_timeout = 250
                };

                //1.test conn param request procedure
                LeGapConnParamRequest(gTheBle.conn_hdl, &default_conn_params);
#endif

#ifdef APP_USE_PAIRING
                LeSmpMasterPairingReq(ind->conn_hdl);;
#else
                // Start GATT service discovery
                LeSendMessageAfter(&gTheBle.task, BLE_APP_MSG_FIND_ALL_SERVICE, 0, 100);
#endif
            }
            else
            {
                gTheBle.state = APP_STATE_IDLE;
            }
        }
        break;

        case LE_CM_MSG_SIG_CONN_UPDATE_REQ:
        {
            LE_CM_MSG_SIG_CONN_UPDATE_REQ_T *req = (LE_CM_MSG_SIG_CONN_UPDATE_REQ_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_SIG_CONN_UPDATE_REQ identifier = %d\r\n", req->identifier);
            BLE_APP_PRINT("    min = %x max = %x latency = %x timeout = %x\r\n", req->interval_min, req->interval_max, req->slave_latency, req->timeout_multiplier);
            LeGapSigConnUpdateResponse(req->conn_hdl, req->identifier, TRUE);
        }
        break;

        case LE_CM_MSG_CONN_PARA_REQ:
        {
            LE_CM_MSG_CONN_PARA_REQ_T *req = (LE_CM_MSG_CONN_PARA_REQ_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_CONN_PARA_REQ min = %x max = %x latency = %x timeout = %x\r\n", req->itv_min, req->itv_max, req->latency, req->sv_tmo);
            LeGapConnParamResponse(req->conn_hdl, TRUE);
        }
        break;

        case LE_CM_MSG_CONN_UPDATE_COMPLETE_IND:
        {
            LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T *ind = (LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_CONN_UPDATE_COMPLETE_IND status = %x, itv = %x, latency = %x svt = %x\r\n", ind->status, ind->interval, ind->latency, ind->supervision_timeout);

            if (ind->status == SYS_ERR_SUCCESS)
            {
                gTheBle.max_itvl = ind->interval;
                gTheBle.latency = ind->latency;
                gTheBle.sv_tmo = ind->supervision_timeout;
            }
            else
            {
                LeGapDisconnectReq(ind->conn_hdl);
            }
        }
        break;

        case LE_CM_MSG_SET_DISCONNECT_CFM:
        {
            LE_CM_MSG_SET_DISCONNECT_CFM_T *cfm = (LE_CM_MSG_SET_DISCONNECT_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_SET_DISCONNECT_CFM conn_hdl = %x status = %x\r\n", cfm->handle, cfm->status);
        }
        break;

        case LE_CM_MSG_DISCONNECT_COMPLETE_IND:
        {
            LE_CM_MSG_DISCONNECT_COMPLETE_IND_T *ind = (LE_CM_MSG_DISCONNECT_COMPLETE_IND_T *)message;

            BLE_APP_PRINT("LE_CM_MSG_DISCONNECT_COMPLETE_IND conn_hdl = %x status = %x reason = %x \r\n", ind->conn_hdl, ind->status, ind->reason);

            BleAppGattInfoClean();
            gTheBle.state &= ~APP_STATE_CONNECTED;
            LeSendMessageAfter(&app->task, BLE_APP_MSG_START_SCANNING, 0, T_SEC(1));
        }
        break;


        case LE_CM_MSG_SET_SCAN_PARAMS_CFM:
        {
            LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_EXIT_ADVERTISING_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_SET_SCAN_PARAMS_CFM Status = %x\r\n", cfm->status);

            if (cfm->status == SYS_ERR_SUCCESS)
            {
               LeSendMessageAfter(&app->task, BLE_APP_MSG_START_SCANNING, 0, T_SEC(1));
            }
        }
        break;

        // Scanning started
        case LE_CM_MSG_ENTER_SCANNING_CFM:
        {
            LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_EXIT_ADVERTISING_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_ENTER_SCANNING_CFM Status = %x\r\n", cfm->status);

            if (cfm->status == SYS_ERR_SUCCESS)
            {
                app->state |= APP_STATE_SCANNING;
                if (app->scan_duration)
                    LeSendMessageAfter(&app->task, BLE_APP_MSG_STOP_SCANNING, 0, T_SEC(app->scan_duration));
            }
            break;
        }

        // Scanning stop
        case LE_CM_MSG_EXIT_SCANNING_CFM:
        {
            UINT8 duration = 10;
            LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_EXIT_ADVERTISING_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_EXIT_SCANNING_CFM Status = %x\r\n", cfm->status);

            app->state &= ~APP_STATE_SCANNING;

            if (is_connect_per_addr && is_filter_matched)
            {
                int rc = 0;
                BLE_APP_PRINT("filter match send connect_request\r\n");
                rc = LeGapCentralCreateConnect(app->scan_para.own_addr_type, &app->filter_addr, &app->conn_para);
                if (rc)
                {
                    BLE_APP_PRINT("LeGapCentralCreateConnect %d\r\n", rc);
                    app->state = APP_STATE_IDLE;
                }
                if (duration) {
                    LeSendMessageAfter(&app->task, BLE_APP_MSG_CONN_CANCEL, 0, T_SEC(duration));
                }
            }
            is_filter_matched = 0;
        }
        break;

        // Advertising report received
        case LE_CM_MSG_ADVERTISE_REPORT_IND:
        {
            LE_CM_MSG_ADVERTISE_REPORT_IND_T *ind = (LE_CM_MSG_ADVERTISE_REPORT_IND_T *)message;
            BleAppAdvReportHandler((ind->event_type == GAP_ADV_RPT_TYPE_SCAN_RSP),ind->event_type,ind->addr,ind->addr_type,ind->rssi,ind->len,ind->data);
        }
        break;

        case LE_CM_MSG_ADD_TO_WHITE_LIST_CFM:
        {
            LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_EXIT_ADVERTISING_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_ADD_TO_WHITE_LIST_CFM - Status = %x\r\n", cfm->status);
        }
        break;

	    case LE_CM_MSG_SET_PHY_CFM:
        {
			LE_CM_MSG_SET_PHY_CFM_T *cfm = (LE_CM_MSG_SET_PHY_CFM_T *)message;
			BLE_APP_PRINT("LE_CM_MSG_SET_PHY_CFM status = %x\r\n", cfm->status);
        }
        break;
	    case LE_CM_MSG_PHY_UPDATE_COMPLETE_IND:
		{
			LE_CM_MSG_PHY_UPDATE_COMPLETE_IND_T *ind = (LE_CM_MSG_PHY_UPDATE_COMPLETE_IND_T *)message;
			BLE_APP_PRINT("LE_CM_MSG_PHY_UPDATE_COMPLETE_IND status = %x tx = %d rx = %d\r\n", ind->status, ind->tx_phy, ind->rx_phy);
        }
        break;
        default:
        break;
    }
}

static void BleAppMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    BLE_APP_DATA_T *app = BleAppDev();
    //BLE_APP_PRINT("BleAppMsgHandler id = %x, App state =%d\r\n", id, app->state);

    switch (id)
    {
        case BLE_APP_MSG_INITIALIZING:
        {
            LeHostInit(&app->task);
        }
        break;

        case BLE_APP_MSG_STOP_SCANNING:
        {
            LE_ERR_STATE rc = LeGapSetScanDisable();

            if (rc != SYS_ERR_SUCCESS)
            {
                BLE_APP_PRINT("BLE_APP_MSG_STOP_SCANNING fail rc = %x\r\n", rc);
            }
        }
        break;

        case BLE_APP_MSG_START_SCANNING:
        {
            LE_ERR_STATE rc = LeGapSetScanEnable(app->scan_duplicate_filter);

            if (rc != SYS_ERR_SUCCESS)
            {
                BLE_APP_PRINT("BLE_APP_MSG_START_SCANNING fail rc = %x\r\n", rc);
            }
        }

        case BLE_APP_MSG_CONN_CANCEL:
        if(app->state & APP_STATE_CONNECTING)
        {
            LeGapCentralCancelConnect();
        }
        break;

        case BLE_APP_MSG_FIND_ALL_SERVICE:
        {
            LE_ERR_STATE rc = LeGattFindAllPrimaryService(gTheBle.conn_hdl);

            if (rc != SYS_ERR_SUCCESS)
            {
                BLE_APP_PRINT("BLE_APP_MSG_FIND_ALL_SERVICE fail rc = %x\r\n", rc);
            }
            BLE_APP_PRINT("GATT service discovery\r\n");
        }
        break;

        case BLE_APP_MSG_FIND_ATTR:
        {
            BleAppFindInfo();
        }
        default:
        break;
    }
}

static void BleAppSmMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    switch (id)
    {
        case LE_SMP_MSG_PAIRING_ACTION_IND:
        {
            LE_SMP_MSG_PAIRING_ACTION_IND_T *ind = (LE_SMP_MSG_PAIRING_ACTION_IND_T *)message;
            BLE_APP_PRINT("LE_SMP_MSG_PAIRING_ACTION_IND hdl = %x sc = %d action = %d\r\n", ind->conn_hdl, ind->sc, ind->action);

            LeSmpSecurityRsp(ind->conn_hdl, TRUE);
        }
        break;

        case LE_SMP_MSG_ENCRYPTION_CHANGE_IND:
        {
            LE_SMP_MSG_ENCRYPTION_CHANGE_IND_T *ind = (LE_SMP_MSG_ENCRYPTION_CHANGE_IND_T *)message;

            BLE_APP_PRINT("LE_SMP_MSG_ENCRYPTION_CHANGE_IND enable = %d\r\n", ind->enable);
            gTheBle.encrypted = ind->enable;
        }
        break;

        case LE_SMP_MSG_ENCRYPTION_REFRESH_IND:
        {
            LE_SMP_MSG_ENCRYPTION_REFRESH_IND_T *ind = (LE_SMP_MSG_ENCRYPTION_REFRESH_IND_T *)message;

            BLE_APP_PRINT("LE_SMP_MSG_ENCRYPTION_REFRESH_IND status = %x\r\n", ind->status);
        }
        break;

        case LE_SMP_MSG_PAIRING_COMPLETE_IND:
        {
            LE_SMP_MSG_PAIRING_COMPLETE_IND_T *ind = (LE_SMP_MSG_PAIRING_COMPLETE_IND_T *)message;

            BLE_APP_PRINT("LE_SMP_MSG_PAIRING_COMPLETE_IND status = %x\r\n", ind->status);

            if (ind->status == SYS_ERR_SUCCESS)
            {
                gTheBle.paired = TRUE;
                LeSendMessageAfter(&gTheBle.task, BLE_APP_MSG_FIND_ALL_SERVICE, 0, 100);

            }
        }
        break;

        default:
        break;
    }
}

static void BleAppTaskHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    if ((id >= LE_GATT_MSG_BASE) && (id < LE_GATT_MSG_TOP))
    {
        BleAppGattMsgHandler(task, id, message);
    }
    else if ((id >= BLE_APP_MSG_INITIALIZING) && (id < BLE_APP_MSG_TOP))
    {
        BleAppMsgHandler(task, id, message);
    }
    else if ((id >= LE_SMP_MSG_BASE) && (id < LE_SMP_MSG_TOP))
    {
        BleAppSmMsgHandler(task, id, message);
    }
    else if ((id >= LE_CM_MSG_BASE) && (id < LE_CM_MSG_TOP))
    {
        BleAppCmMsgHandler(task, id, message);
    }
}


BLE_APP_DATA_T* BleAppDev(void)
{
	return &gTheBle;
}

void BleAppInit(void)
{
    BLE_APP_DATA_T *app = BleAppDev();

    MemSet(app, 0, sizeof(BLE_APP_DATA_T));

    app->state = APP_STATE_INIT;
    app->conn_para = default_conn_params;
    app->scan_para = default_scan_params;
    app->scan_duration = DEFAULT_SCAN_DURATION;
    app->scan_duplicate_filter = LE_HCI_SCAN_DUPLICATE_ENABLE;
    app->scan_filter_type = BLE_APP_SCAN_FILTER_TYPE_UUID;

    if (app->scan_filter_type == BLE_APP_SCAN_FILTER_TYPE_MAC)
    {
        app->filter_addr = g_target_periph_addr;

    }
    else if(app->scan_filter_type == BLE_APP_SCAN_FILTER_TYPE_NAME)
    {
        app->scan_filter_dataLen = strlen(g_target_periph_name);
        memcpy(app->scan_filter_buf, g_target_periph_name, app->scan_filter_dataLen);
    }

    LeHostCreateTask(&app->task, BleAppTaskHandler);
    LeSendMessage(&app->task, BLE_APP_MSG_INITIALIZING, 0);

    MasterAtCmdAdd();

    printf("OPL2500 BLE init completed. \r\n");
}

