/******************************************************************************
*  Copyright 2017 - 2023, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2023
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  mw_fim_default_group01_patch.c
*
*  Project:
*  --------
*  OPL2500 Project - the Flash Item Management (FIM) implement file
*
*  Description:
*  ------------
*  This implement file is include the Flash Item Management (FIM) function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim_default_group01_patch.h"
#include "mw_fim_default.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable



// the default value of rf cfg
const T_RfCfg g_tMwFimDefaultRfConfig_patch =
{
    0xB0       // u8HighPwrStatus
};

// the default value of rf cfg
const T_AdcGpioOffset g_tMwFimDefaultAdcOffset = { 0 };

// the address buffer of Uart config
extern uint32_t g_ulaMwFimAddrBufferUartConfig[MW_FIM_UART_CFG_NUM];

// the address buffer of Tracer
extern uint32_t g_ulaMwFimAddrBufferTracerConfig[MW_FIM_TRACER_CFG_NUM];
extern uint32_t g_ulaMwFimAddrBufferTracerIntTaskInfo[MW_FIM_TRACER_INT_TASK_INFO_NUM];
extern uint32_t g_ulaMwFimAddrBufferTracerExtTaskInfo[MW_FIM_TRACER_EXT_TASK_INFO_NUM];

// the address buffer of rf config
extern uint32_t g_ulaMwFimAddrBufferRfConfig[MW_FIM_RF_CFG_NUM];

// the address buffer of adc offset
uint32_t g_ulaMwFimAddrBufferAdcOffset[MW_FIM_ADC_OFFSET_NUM];

// the information table of group 01
const T_MwFimFileInfo g_taMwFimGroupTable01_patch[] =
{
    {MW_FIM_IDX_GP01_UART_CFG            , MW_FIM_UART_CFG_NUM            , MW_FIM_UART_CFG_SIZE            , (uint8_t*)&g_ctHal_Uart_DefaultConfig   , g_ulaMwFimAddrBufferUartConfig},

    {MW_FIM_IDX_GP01_TRACER_CFG          , MW_FIM_TRACER_CFG_NUM          , MW_FIM_TRACER_CFG_SIZE          , NULL                                    , g_ulaMwFimAddrBufferTracerConfig},
    {MW_FIM_IDX_GP01_TRACER_INT_TASK_INFO, MW_FIM_TRACER_INT_TASK_INFO_NUM, MW_FIM_TRACER_INT_TASK_INFO_SIZE, NULL                                    , g_ulaMwFimAddrBufferTracerIntTaskInfo},
    {MW_FIM_IDX_GP01_TRACER_EXT_TASK_INFO, MW_FIM_TRACER_EXT_TASK_INFO_NUM, MW_FIM_TRACER_EXT_TASK_INFO_SIZE, NULL                                    , g_ulaMwFimAddrBufferTracerExtTaskInfo},

    {MW_FIM_IDX_GP01_RF_CFG              , MW_FIM_RF_CFG_NUM              , MW_FIM_RF_CFG_SIZE              , (uint8_t*)&g_tMwFimDefaultRfConfig_patch, g_ulaMwFimAddrBufferRfConfig},

    {MW_FIM_IDX_GP01_ADC_OFFSET          , MW_FIM_ADC_OFFSET_NUM          , MW_FIM_ADC_OFFSET_SIZE          , (uint8_t*)&g_tMwFimDefaultAdcOffset     , g_ulaMwFimAddrBufferAdcOffset},

    // the end, don't modify and remove it
    {0xFFFFFFFF,            0x00,              0x00,               NULL,                            NULL}
};


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
void MwFim_Group01_patch( void )
{
    MwFim_GroupInfoUpdate(0, 1, (T_MwFimFileInfo *)g_taMwFimGroupTable01_patch);
    MwFim_GroupVersionUpdate(0, 1, MW_FIM_VER01+2); // <-- Updated Ver. here !!
}
