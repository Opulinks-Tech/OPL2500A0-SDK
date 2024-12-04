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

#ifndef _BLE_APP_AT_CMD_H_
#define _BLE_APP_AT_CMD_H_

#include "ble_master_app.h"

void MasterAtCmdAdd(void);
void CmdAppSendAtCmdRsp(char *fmt, ...);

#endif
