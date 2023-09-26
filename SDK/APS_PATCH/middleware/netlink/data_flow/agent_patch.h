/******************************************************************************
*  Copyright 2017 - 2023, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2023
******************************************************************************/

#ifndef __AGENT_H__
#error "Please include agent.h. Don't directly include agent_patch.h"
#endif

#ifndef __AGENT_PATCH_H__
#define __AGENT_PATCH_H__

#include "data_flow.h"

//typedef enum
//{
//    M3_MSG_RSP = 0,
//    M3_MSG_RF_CFG_SET,
//    M3_MSG_TS_CFG_SET,
//    M3_MSG_AUXADC_CFG_SET,
//    M3_MSG_AUXADC_REQ,

//    M3_MSG_MAX
//} T_M3MsgType;

typedef enum
{
    RF_EVT_COME = RF_EVT_MAX,

    RF_EVT_MAX_NEW,
} T_RfCmdEvtType_EXT;

#define M3_MSG_TCA_SET      M3_MSG_MAX
#define M3_MSG_PATCH_MAX    (M3_MSG_MAX + 1)

void Agent_PatchInit(void);

#endif //#ifndef __AGENT_PATCH_H__
