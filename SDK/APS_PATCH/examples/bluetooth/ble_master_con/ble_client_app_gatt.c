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
#include "ble_client_app_gatt.h"
#include "ble_gatt_if.h"
#include "ble_uuid.h"
#include "ble_adv_parser.h"
#include "ble_app_at_cmd.h"

static UINT16 BleAppFindCharCccdHdl(BLE_SERVICE_T *svc, BLE_CHAR_DECL_T *decl)
{
    BLE_ATT_ATTR_T *attr = svc->attr;
    UINT16 s = decl->handle + 1;
    UINT16 e = 0;
    UINT16 i;

    if (!svc || !attr || !decl ) return 0;

    if ((s < svc->start_hdl) || (s > svc->end_hdl)) return 0;

    for (i = 1; i < svc->attr_count; i++)
    {
        if (attr[i].handle > s)
        {
            if ((attr[i].format == LE_GATT_UUID16) && (attr[i].uuid[0] == GATT_CHARACTERISTIC_UUID)) break;

            if ((attr[i].format == LE_GATT_UUID16) && (attr[i].uuid[0] == GATT_CLIENT_CHAR_CFG_UUID))
            {
                e = attr[i].handle;
                break;
            }
        }
    }

    return e;
}

void BleAppGattHandleCharDeclInfoInd(LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND_T *ind)
{
    BLE_APP_DATA_T *theApp = BleAppDev();
    BLE_BAS_INFO_T *bas = theApp->bas;
    BLE_SERVICE_T *svc = bas->svc;
    BAS_CHAR_T *cha = 0;

    if (svc->action != GATT_ACTION_WAIT_CHAR_INFO) return;

    if ((ind->handle <= svc->start_hdl) || (ind->handle > svc->end_hdl)) return;

    if (ind->format == LE_GATT_UUID16)
    {
        if (ind->uuid[0] == ATT_CHAR_BATTERY_LEVEL)
        {
            if (!bas->bat_level)
            {
                cha = PanicUnlessMalloc(sizeof(BAS_CHAR_T) + sizeof(BLE_CHAR_CCCD_T));
                cha->cccd = (BLE_CHAR_CCCD_T *)((UINT8 *)cha + sizeof(BAS_CHAR_T));
                bas->bat_level = cha;
            }
        }
    }
    else
    {
        //TODO
    }

//done:
    if (cha)
    {
        cha->decl.handle = ind->handle;
        cha->decl.property = ind->property;
        cha->decl.val_hdl = ind->val_hdl;
        cha->decl.format = ind->format;

        if (ind->format == LE_GATT_UUID16)
            cha->decl.uuid[0] = ind->uuid[0];
        else
            MemCopy(cha->decl.uuid, ind->uuid, 16);
    }
}

void BleAppGattHandleFindCharCfm(LE_GATT_MSG_FIND_CHARACTERISTIC_CFM_T *cfm)
{
    BLE_APP_DATA_T *theApp = BleAppDev();
    BLE_BAS_INFO_T *bas = theApp->bas;
    BLE_SERVICE_T *svc = bas->svc;

    if (!bas || svc->action != GATT_ACTION_WAIT_CHAR_INFO) return;

    if (bas->bat_level)
    {
        bas->bat_level->cccd->handle = BleAppFindCharCccdHdl(svc, &bas->bat_level->decl);
    }
    svc->action = GATT_ACTION_FINISH;
}


void BleAppGattMsgHandler(TASK task, MESSAGEID id, MESSAGE message)
{
    BLE_APP_DATA_T *theApp = BleAppDev();

    switch (id)
    {
        case LE_GATT_MSG_INIT_CFM:
        {
            BLE_APP_PRINT("LE_GATT_MSG_INIT_CFM\r\n");
        }
        break;

        case LE_GATT_MSG_NOTIFY_IND:
        {
            LE_GATT_MSG_NOTIFY_IND_T *ind = (LE_GATT_MSG_NOTIFY_IND_T *)message;
            UINT16 char_uuid;

            BLE_APP_PRINT("LE_GATT_MSG_NOTIFY_IND hdl = %d\r\n", ind->handle);
#if 0
            for (int i = 0; i < ind->len; i++)
            {
                if (i < (ind->len - 1))
                    BLE_APP_PRINT("%02x ", ind->val[i]);
                else
                    BLE_APP_PRINT("%02x", ind->val[i]);
            }
            BLE_APP_PRINT("\r\n");
#endif

            char_uuid = BleAPPFindUuidByHandle(ind->handle);

            if (char_uuid == ATT_CHAR_HEART_RATE_MEAS)
            {
                BleAppMemDump("HRS", ind->len, ind->val);
                parse_hrs_value(ind->val, ind->len);
            }

            if (char_uuid == ATT_CHAR_BATTERY_LEVEL)
            {
                BleAppMemDump("BAS", ind->len, ind->val);
                BLE_APP_PRINT(COLOR_GREEN"Battery level:%d \r\n"COLOR_RST, ind->val[0]);
            }
            BLE_APP_PRINT("\r\n");
        }
        break;

        case LE_GATT_MSG_EXCHANGE_MTU_IND:
        {
            LE_GATT_MSG_EXCHANGE_MTU_IND_T *ind = (LE_GATT_MSG_EXCHANGE_MTU_IND_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_EXCHANGE_MTU_IND client mtu = %d\r\n", ind->client_rx_mtu);
            LeGattExchangeMtuRsp(ind->conn_hdl, LE_ATT_MAX_MTU);
        }
        break;

        case LE_GATT_MSG_EXCHANGE_MTU_CFM:
        {
            LE_GATT_MSG_EXCHANGE_MTU_CFM_T *cfm = (LE_GATT_MSG_EXCHANGE_MTU_CFM_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_EXCHANGE_MTU_CFM curr mtu = %d\r\n", cfm->current_rx_mtu);
            BleAppDev()->curr_mtu = cfm->current_rx_mtu;
        }
        break;

        case LE_GATT_MSG_SERVICE_INFO_IND:
        {
            LE_GATT_MSG_SERVICE_INFO_IND_T *ind = (LE_GATT_MSG_SERVICE_INFO_IND_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_SERVICE_INFO_IND uuid = %04x start = %x end = %x\r\n", ind->uuid[0], ind->start_hdl, ind->end_hdl);

            if (theApp->svc_idx < BLE_SERVICE_COUNT)
            {
                BLE_SERVICE_T *svc = &theApp->svc[theApp->svc_idx];
                theApp->svc_idx++;
                theApp->svc_count = theApp->svc_idx;

                svc->start_hdl = ind->start_hdl;
                svc->end_hdl = ind->end_hdl;
                svc->action = GATT_ACTION_FIND_ATTR;
                svc->format = ind->format;

                if (ind->format == LE_GATT_UUID16)
                    svc->uuid[0] = ind->uuid[0];
                else
                    MemCopy(svc->uuid, ind->uuid, 16);

                svc->attr = 0;
            }
        }
        break;

        case LE_GATT_MSG_FIND_ALL_PRIMARY_SERVICE_CFM:
        {
            BLE_APP_PRINT("LE_GATT_MSG_FIND_ALL_PRIMARY_SERVICE_CFM\r\n");
            LeSendMessage(&BleAppDev()->task, BLE_APP_MSG_FIND_ATTR, 0);
        }
        break;
        case LE_GATT_MSG_FIND_PRIMARY_SERVICE_BY_UUID_CFM:
        {
            BLE_APP_PRINT("LE_GATT_MSG_FIND_PRIMARY_SERVICE_BY_UUID_CFM\r\n");
            LeSendMessage(&BleAppDev()->task, BLE_APP_MSG_FIND_ATTR, 0);
        }
        break;

        case LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND:
        {
            LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND_T *ind = (LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND_T *)message;
            BLE_SERVICE_T *svc = BleAppDev()->curr_svc;
            BLE_ATTRQ_T *attr = PanicUnlessMalloc(sizeof(BLE_ATTRQ_T));

            if (!svc) return;

            if (svc->action != GATT_ACTION_WAIT_INFO) return;

            if ((ind->handle <= svc->start_hdl) || (ind->handle > svc->end_hdl)) return;

            attr->handle = ind->handle;
            attr->format = ind->format;

            if (ind->format == LE_GATT_UUID16)
                attr->uuid[0] = ind->uuid[0];
            else
                MemCopy(attr->uuid, ind->uuid, 16);

            BLE_APP_PRINT("LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND hdl = %d type = %d uuid = 0x%x\r\n", attr->handle, attr->format, attr->uuid[0]);
            LeQueuePush(&BleAppDev()->attrq, attr);
        }
        break;

        case LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM:
        {
            LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM_T *cfm = (LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM_T *)message;
            BLE_SERVICE_T *svc = BleAppDev()->curr_svc;
            UINT16 count = LeQueueCount(&BleAppDev()->attrq);
            BLE_APP_PRINT("LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM cid = %x attErr = %x count = %d\r\n", cfm->conn_hdl, cfm->att_err, count);

            if (count)
            {
                BLE_ATT_ATTR_T *attr = PanicUnlessMalloc(sizeof(BLE_ATT_ATTR_T) * count);
                BLE_ATTRQ_T *attrm;
                UINT16 idx = 0;

                svc->attr_count = count;
                svc->attr = attr;

                while (count--)
                {
                    attrm = LeQueuePop(&BleAppDev()->attrq);

                    if (attrm)
                    {
                        attr[idx].handle = attrm->handle;
                        attr[idx].format = attrm->format;

                        if (attrm->format == LE_GATT_UUID16)
                            attr[idx].uuid[0] = attrm->uuid[0];
                        else
                            MemCopy(attr[idx].uuid, attrm->uuid, 16);

                        FreeMem(attrm);
                        idx++;
                    }
                    else
                        break;
                }
            }

            svc->action = GATT_ACTION_FINISH;
            LeSendMessage(&BleAppDev()->task, BLE_APP_MSG_FIND_ATTR, 0);
        }
        break;

        case LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND:
        {
            LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND_T *ind = (LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND hdl = %d prop = 0x%x val_dhl = %d type = %d uuid = 0x%x\r\n",
                          ind->handle, ind->property, ind->val_hdl, ind->format, ind->uuid[0]);
            BleAppGattHandleCharDeclInfoInd(ind);
        }
        break;

        case LE_GATT_MSG_FIND_CHARACTERISTIC_CFM:
        {
            LE_GATT_MSG_FIND_CHARACTERISTIC_CFM_T *cfm = (LE_GATT_MSG_FIND_CHARACTERISTIC_CFM_T*)message;
            BLE_APP_PRINT("LE_GATT_MSG_FIND_CHARACTERISTIC_CFM\r\n");
            BleAppGattHandleFindCharCfm(cfm);
         }
        break;

        case LE_GATT_MSG_OPERATION_TIMEOUT:
        {
            LE_GATT_MSG_OPERATION_TIMEOUT_T *ind = (LE_GATT_MSG_OPERATION_TIMEOUT_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_OPERATION_TIMEOUT op = %x\r\n", ind->att_op);
        }
        break;

        case LE_GATT_MSG_CHARACTERISTIC_VAL_IND:
        {
            LE_GATT_MSG_CHARACTERISTIC_VAL_IND_T *ind = (LE_GATT_MSG_CHARACTERISTIC_VAL_IND_T *)message;
            BLE_APP_PRINT("LE_GATT_MSG_CHARACTERISTIC_VAL_IND att_err = %x hdl = %d offset = %d len = %d\r\n", ind->att_err, ind->handle, ind->offset, ind->len);
            theApp->find++;

            if (ind->len)
            {
                UINT16 copyLen = ind->len;

                if (!theApp->wrd_buf) theApp->wrd_buf = PanicUnlessMalloc(512);

                if ((theApp->wrd_len + copyLen) > 512) copyLen = 512 - theApp->wrd_len;

                if (copyLen)
                {
                    MemCopy(&theApp->wrd_buf[ind->offset], ind->val, copyLen);
                    theApp->wrd_len += copyLen;
                }
            }
        }
        break;


        case LE_GATT_MSG_READ_CHAR_VAL_BY_UUID_CFM:
        {
            LE_GATT_MSG_READ_CHAR_VAL_BY_UUID_CFM_T *cfm = (LE_GATT_MSG_READ_CHAR_VAL_BY_UUID_CFM_T*)message;
            BLE_APP_PRINT("LE_GATT_MSG_READ_CHAR_VAL_BY_UUID_CFM status = %x att_err = %x hdl = %d\r\n", cfm->status, cfm->att_err, cfm->err_hdl);

            if (theApp->find)
            {
                UINT8 buf[256];
                UINT8 *p = buf;

                if (theApp->wrd_len <= 116)
                {
                    LeHtcUtilOctetNumToHexStr(theApp->wrd_buf, theApp->wrd_len, &p);
                    *p = 0;
                    BLE_APP_PRINT("Read Characteristic Values:\r\nhdl: %d, len:%d, value:%s\r\n", cfm->err_hdl, theApp->wrd_len, buf);
                    CmdAppSendAtCmdRsp("+AttrValue:%d,%d,%s", cfm->conn_hdl, theApp->wrd_len, buf);
                }

                if (theApp->wrd_buf) FreeMem(theApp->wrd_buf);

                theApp->wrd_buf = 0;
                theApp->wrd_len = 0;
                theApp->find = 0;
            }
        }
        break;
        case LE_GATT_MSG_READ_LONG_CHAR_VAL_CFM:
        {
           LE_GATT_MSG_READ_LONG_CHAR_VAL_CFM_T *cfm = (LE_GATT_MSG_READ_LONG_CHAR_VAL_CFM_T*)message;
            BLE_APP_PRINT("LE_GATT_MSG_READ_LONG_CHAR_VAL_CFM status = %x att_err = %x hdl = %d\r\n", cfm->status, cfm->att_err, cfm->handle);

            if (theApp->find)
            {
                UINT8 buf[256];
                UINT8 *p = buf;

                if (theApp->wrd_len <= 116)
                {
                    LeHtcUtilOctetNumToHexStr(theApp->wrd_buf, theApp->wrd_len, &p);
                    *p = 0;
                    BLE_APP_PRINT("Read Characteristic Values:\r\nhdl: %d, len:%d, value:%s\r\n", cfm->handle, theApp->wrd_len, buf);
                    CmdAppSendAtCmdRsp("+AttrValue:%d,%d,%s", cfm->conn_hdl, theApp->wrd_len, buf);                }

                if (theApp->wrd_buf) FreeMem(theApp->wrd_buf);

                theApp->wrd_buf = 0;
                theApp->wrd_len = 0;
                theApp->find = 0;
            }
        }
        break;

        case LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM:
        {
            LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM_T *cfm = (LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM_T*)message;
            BLE_APP_PRINT("LE_GATT_MSG_READ_CHARACTERISTIC_VALUE_CFM status = %x att_err = %x hdl = %d\r\n", cfm->status, cfm->att_err, cfm->handle);

        }
        break;

	    case LE_GATT_MSG_WRITE_CHAR_VALUE_CFM:
	    {
            LE_GATT_MSG_WRITE_CHAR_VALUE_CFM_T *cfm = (LE_GATT_MSG_WRITE_CHAR_VALUE_CFM_T*)message;
            BLE_APP_PRINT("LE_GATT_MSG_WRITE_CHAR_VALUE_CFM status = %x att_err = %x hdl = %d\r\n", cfm->status, cfm->att_err, cfm->handle);
        }
        break;

        default:
        break;
    }
}

