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

#ifndef __AGENT_H__
#define __AGENT_H__


typedef enum
{
    AGENT_MSG_M0 = 0,
    AGENT_MSG_UART1,
    AGENT_MSG_AT_OUTPUT,

    AGENT_MSG_MAX
} T_AgentMsgType;

typedef struct
{
    uint32_t u32Type;   // T_AgentMsgType
    uint32_t u32ParamLen;
    uint8_t *pu8Param;
} T_AgentMsg;

typedef struct
{
    uint32_t u32Type;   // T_AgentMsgType
    uint32_t u32ParamLen;
    uint8_t pu8Param[];
} T_AgentMsgInt;

typedef enum
{
    M3_MSG_RSP = 0,
    M3_MSG_RF_CFG_SET,
    M3_MSG_TS_CFG_SET,
    M3_MSG_AUXADC_CFG_SET,
    M3_MSG_AUXADC_REQ,

    M3_MSG_MAX
} T_M3MsgType;

typedef enum
{
    M0_MSG_RSP = 0,
    M0_MSG_RF_CFG_WRITE,
    M0_MSG_RF_CFG_READ,

    M0_MSG_MAX
} T_M0MsgType;

typedef struct
{
    uint8_t u8Type;     // T_M3MsgType
    uint8_t u8Req;
    uint8_t u8Status;
    uint8_t u8Padding;

    uint8_t u8aData[32];
} T_M3CommMsg;

typedef struct
{
    uint8_t u8Type;     // T_M0MsgType
    uint8_t u8Req;
    uint8_t u8Status;
    uint8_t u8Padding;

    uint8_t u8aData[32];
} T_M0CommMsg;


// internal
typedef void (*T_AgentTaskMainFp)(void *pParam);
typedef int (*T_AgentAtOutputHandleFp)(void* pdata);
typedef void (*T_Rf_ScaDump)(uint8_t u8NumMode, uint8_t u8NumFreq, uint16_t u16FreqBase, uint32_t u32TableAddr);
typedef void (*T_Ow_TableList_fp)(uint32_t u32Addr, uint32_t u32TableNum);

// external
typedef void (*T_AgengCommFp)(void);
typedef int (*T_AgengMsgSendFp)(T_AgentMsg *ptMsg);
typedef int (*T_AgentDataHandleFp)(uint32_t u32Type, uint8_t *u8aData, uint32_t u32DataLen, void *pParam);
typedef void (*T_AgentDataHandleRegFp)(T_AgentDataHandleFp fpHandle, void *pParam);

extern T_AgentTaskMainFp agent_task_main;
extern T_AgentAtOutputHandleFp agent_at_output_handle;
extern T_Rf_ScaDump Rf_ScaDump;
extern T_Ow_TableList_fp Ow_TableList;

extern T_AgengCommFp agent_init;
extern T_AgengMsgSendFp agent_msg_send;
extern T_AgentDataHandleRegFp agent_data_handle_reg;

void agent_task_main_impl(void *pParam);
void agent_init_impl(void);
int agent_msg_send_impl(T_AgentMsg *ptInputMsg);
void agent_data_handle_reg_impl(T_AgentDataHandleFp fpHandle, void *pParam);
void Rf_ScaDump_impl(uint8_t u8NumMode, uint8_t u8NumFreq, uint16_t u16FreqBase, uint32_t u32TableAddr);
void Ow_TableList_impl(uint32_t u32Addr, uint32_t u32TableNum);
int agent_at_output_handle_impl(void* pdata);


#endif //#ifndef __AGENT_H__

