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

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "at_cmd.h"
#include "at_cmd_common.h"
#include "at_cmd_data_process.h"
#include "hal_flash.h"
#include "at_cmd_task.h"
#include "at_cmd_func_patch.h"

#include "agent.h"
#include "mw_fim.h"
#include "ble_app_at_cmd.h"
#include "ble_master_app.h"
#include "ble_uuid.h"


void CmdAppSendAtCmdRsp(char *fmt, ...)
{
    va_list ap;
    char string[256];

    va_start(ap, fmt);
    vsnprintf(string, 255, fmt, ap);
    va_end(ap);
    string[255] = 0;

    at_output("\r\n%s\r\n", string);
}

static void AtCmdSendAck(int ret)
{
    if(ret)
    {
        at_output("OK\r\n");
    }
    else
    {
        at_output("ERROR\r\n");
    }
}

static int ble_cmd_scan(char *buf, int len, int mode)
{
    int iRet = 0;

    if (!BleAppSetScaning()) goto done;
    iRet = 1;

done:
    AtCmdSendAck(iRet);

    return iRet;
}

static int ble_cmd_disconnect(char *buf, int len, int mode)
{
    int iRet = 0;

    if (!BleAppSetDisconnect()) goto done;
    iRet = 1;

done:
    AtCmdSendAck(iRet);

    return iRet;
}

static int ble_cmd_remote_uuid_cfg(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[2] = {0};
    UINT8 uuid[16];

    if (mode == AT_CMD_MODE_SET)
    {
        uint16_t service_uuid;
        if (!at_cmd_buf_to_argc_argv(buf, &argc, argv, 2)) goto done;

        if(argc < 2) goto done;

        len = strlen(argv[1]);

        if ((len >16 ) || !LeHtcUtilStringToHexNum((UINT8*)argv[1], len, uuid)) goto done;

        service_uuid = uuid[0] | (uuid[1] << 8);
        printf("service_uuid 0x%x\n", service_uuid);
        BleAppCfgRemoteService(service_uuid);
    }
    iRet = 1;

done:
    AtCmdSendAck(iRet);

    return iRet;
}

static int ble_cmd_bas_cccd_rw(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[2] = {0};

    if (mode == AT_CMD_MODE_SET)
    {
        uint8_t notify_enable;

        if (!at_cmd_buf_to_argc_argv(buf, &argc, argv, 2)) goto done;

        if(argc < 2) goto done;

        notify_enable = (uint8_t)strtoul(argv[1], NULL, 10);

        if (notify_enable > 1) goto done;

        if (!BleAppBatteryLevelNotifyReq(notify_enable)) goto done;
    }
    else if (mode == AT_CMD_MODE_READ)
    {
        if (!BleAppReadBatteryServiceCccd()) goto done;
    }
    iRet = 1;

done:
    AtCmdSendAck(iRet);

    return iRet;
}

static int ble_cmd_bas_level_read(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[2] = {0};

    if (!at_cmd_buf_to_argc_argv(buf, &argc, argv, 2)) goto done;

    BleAppReadBatteryLevelValue();
    iRet = 1;

done:
    AtCmdSendAck(iRet);
    return iRet;
}

/* for test only */
static int ble_cmd_service_find(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[2] = {0};
    UINT16 svcUuid = 0;

    if (!at_cmd_buf_to_argc_argv(buf, &argc, argv, 2)) goto done;

    if(argc < 2) goto done;

    if (strcmp(argv[1], "bas") == NULL) svcUuid = ATT_SVC_BATTERY_SERVICE;
    if (strcmp(argv[1], "hrs") == NULL) svcUuid = ATT_SVC_HEART_RATE;

    BleAppFindServiceCharReq(svcUuid);
    iRet = 1;

done:
    AtCmdSendAck(iRet);

    return iRet;
}

static int ble_cmd_hrs_cccd_rw(char *buf, int len, int mode)
{
    int iRet = 0;
    int argc = 0;
    char *argv[2] = {0};

    if (mode == AT_CMD_MODE_SET)
    {
        uint8_t notify_enable;

        if (!at_cmd_buf_to_argc_argv(buf, &argc, argv, 2)) goto done;

        if(argc < 2) goto done;

        notify_enable = (uint8_t)strtoul(argv[1], NULL, 10);

        if (notify_enable > 1) goto done;
        BleAppHeartRateMeasNotifyReq(notify_enable);
    }
    else if (mode == AT_CMD_MODE_READ)
    {
        BleAppReadHeartRateMeasCccd();
    }
    iRet = 1;

done:
    AtCmdSendAck(iRet);

    return iRet;
}


static at_command_t gBleAtCmdTbl[] =
{
    {"at+blediscon",  	ble_cmd_disconnect,	     "disconneciton"},
    {"at+blescanreq",  	ble_cmd_scan,	         "start scanning"},
    {"at+blecfguuid",   ble_cmd_remote_uuid_cfg, "config remote service uuid" },
    {"at+blesrvfind",  	ble_cmd_service_find,    "find battery service"},
    {"at+blebasnty",  	ble_cmd_bas_cccd_rw,     "config battery level notification"},
    {"at+blebasread",   ble_cmd_bas_level_read,  "read battery level value"},
    {"at+blehrsnty",    ble_cmd_hrs_cccd_rw,     "config heart rate measurement notification"},
    {NULL,              NULL,            		NULL},
};

void MasterAtCmdAdd(void)
{
    at_cmd_ext_tbl_reg(gBleAtCmdTbl);

    return;
}

