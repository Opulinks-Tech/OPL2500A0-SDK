#include "ble_scan_app.h"
#include "ble_hci_if.h"
#include "ble_cm_if.h"
#include "ble_gap_if.h"
#include "ble_util.h"
#include "msg.h"

#define ENABLE_APP_DEBUG


#ifdef ENABLE_APP_DEBUG

#define BLE_APP_PRINT        printf

#else

#define BLE_APP_PRINT(...)

#endif

// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     1600
// Minimum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     200

// Scan duration in sec
#define DEFAULT_SCAN_DURATION                 10
// Scan interval in units of 0.625 ms
#define DEFAULT_SCAN_INTERVAL                 160  //(160 * 0.625) = 100 ms
// Scan window in units of 0.625 ms
#define DEFAULT_SCAN_WINDOW                   80   //(80 * 0.625) = 50 ms


// ADV Report types
#define GAP_ADV_RPT_TYPE_CONN_SCAN					0		/* ADV_IND */
#define GAP_ADV_RPT_TYPE_CONN_DIR					1		/* ADV_DIRECT_IND */
#define GAP_ADV_RPT_TYPE_SCAN						2		/* ADV_SCAN_IND */
#define GAP_ADV_RPT_TYPE_NONCONN_NONSCAN			3		/* ADV_NONCONN_IND */
#define GAP_ADV_RPT_TYPE_SCAN_RSP					4		/* SCAN_RSP */


extern void LeHtcUtilOctetNumToHexStr(void *data, UINT16 len, UINT8 **p);

#define BT_BD_ADDR_HEX(addr)   addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]

static BLE_APP_DATA_T gTheBle;

LE_BT_ADDR_T whitelist_addr = { 0, {0x8C,0x8F,0x4A,0x60,0x72,0x24}};
const char ble_device_name[]= "OPL1000 ADV APP";


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
    .interval = DEFAULT_SCAN_INTERVAL,
    .window = DEFAULT_SCAN_WINDOW,
    .type = LE_HCI_SCAN_ACTIVE,
    .own_addr_type = LE_HCI_OWN_ADDR_PUBLIC,
    .filter_policy = LE_HCI_INIT_FILT_NONE
};

static void BleAppGetAdvNameStr(UINT8* data, UINT8 len, UINT8* namebuf)
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

    //UINT8 typestr[5][5] = {"ADV","DIR","SCAN","NONN","RSP"};
    UINT8 addrstr[2][7] = {"Public","Random"};
    UINT8 namestr[31]={0};
    UINT8 datastr[62]={0};
    UINT8* p = datastr;

    if((app->scan_filter_type == BLE_APP_SCAN_FILTER_TYPE_MAC) && MemCmp(addr,app->filter_addr.addr,6))
        return;

    if(is_rsp && !dataLen)
       return;

    BleAppGetAdvNameStr(data,dataLen,namestr);
    if(app->scan_filter_type == BLE_APP_SCAN_FILTER_TYPE_NAME && memcmp(namestr, app->scan_filter_buf, app->scan_filter_dataLen))
        return;

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
        case LE_CM_MSG_INIT_COMPLETE_CFM:
        {
            BLE_APP_PRINT("LE_CM_MSG_INIT_COMPLETE_CFM\r\n");
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
                app->state = APP_STATE_SCANNING;
                if (app->scan_duration)
                    LeSendMessageAfter(&app->task, BLE_APP_MSG_STOP_SCANNING, 0, T_SEC(app->scan_duration));
            }
            break;
        }

        // Scanning stop
        case LE_CM_MSG_EXIT_SCANNING_CFM:
        {
            LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm = (LE_CM_MSG_EXIT_ADVERTISING_CFM_T *)message;
            BLE_APP_PRINT("LE_CM_MSG_EXIT_SCANNING_CFM Status = %x\r\n", cfm->status);
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

        default:
        break;
    }
}

static void BleAppMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    BLE_APP_DATA_T *app = BleAppDev();
    BLE_APP_PRINT("BleAppMsgHandler id = %x\r\n", id);

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

        default:
        break;
    }
}

static void BleAppTaskHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    if ((id >= LE_CM_MSG_BASE) && (id < LE_CM_MSG_TOP))
    {
        BleAppCmMsgHandler(task, id, message);
    }
    else if ((id >= BLE_APP_MSG_INITIALIZING) && (id < BLE_APP_MSG_TOP))
    {
        BleAppMsgHandler(task, id, message);
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
    app->scan_para = default_scan_params;
    app->scan_duration = DEFAULT_SCAN_DURATION;
    app->scan_duplicate_filter = LE_HCI_SCAN_DUPLICATE_ENABLE;
    app->scan_filter_type = BLE_APP_SCAN_FILTER_TYPE_NONE;

    if (app->scan_filter_type == BLE_APP_SCAN_FILTER_TYPE_MAC)
    {
        app->filter_addr = whitelist_addr;

    }
    else if(app->scan_filter_type == BLE_APP_SCAN_FILTER_TYPE_NAME)
    {
        app->scan_filter_dataLen = strlen(ble_device_name);
        memcpy(app->scan_filter_buf, ble_device_name, app->scan_filter_dataLen);
    }

    LeHostCreateTask(&app->task, BleAppTaskHandler);
    LeSendMessage(&app->task, BLE_APP_MSG_INITIALIZING, 0);
    printf("OPL2500 BLE init completed. \r\n");
}

