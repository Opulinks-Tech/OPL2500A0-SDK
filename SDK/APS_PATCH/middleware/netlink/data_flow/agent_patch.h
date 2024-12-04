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

typedef enum
{
    RF_EVT_COME = RF_EVT_MAX,

    RF_EVT_MAX_NEW,
} T_RfCmdEvtType_EXT;

typedef enum
{
    M3_MSG_TCA_SET = M3_MSG_MAX,
    M3_MSG_MSQ_IPC_DESC_SET,
    M3_MSG_MSQ_SYSCLK_SET,

    M3_MSG_PTA_SET,
    M3_MSG_BCN_EN_SET,
    M3_MSG_DTO_SET,
    M3_MSG_AMPDU_SET,
    M3_MSG_RA_SET,

    M3_MSG_MAX_PATCH
} T_M3MsgTypePatch;

typedef enum
{
    M0_MSG_AUTO_ARP_SET = M0_MSG_MAX,

    M0_MSG_MAX_PATCH
} T_M0MsgTypePatch;

typedef struct
{
    uint32_t u32Timeout;
    uint8_t u8Enable;
} T_AutoArpCfg;

void Agent_PatchInit(void);

#endif //#ifndef __AGENT_PATCH_H__
