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

/******************************************************************************
*  Filename:
*  ---------
*  hal_uart.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This source file defines the functions of uart .
*  Ref. document is << DesignWare DW_apb_uart Databook >>
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include <string.h>
#include "opulinks.h"
#include "hal_tick.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_uart.h"
#include "hal_pin_def.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define UART_CFG_DEF_BAUDRATE   115200
#define UART_CFG_DEF_DATA_BIT   DATA_BIT_8
#define UART_CFG_DEF_STOP_BIT   STOP_BIT_1
#define UART_CFG_DEF_PARITY     PARITY_NONE
#define UART_CFG_DEF_FLOW_CTRL  0


// the default value of Uart config
const T_HalUartConfig g_ctHal_Uart_DefaultConfig =
{
    UART_CFG_DEF_BAUDRATE,
    UART_CFG_DEF_DATA_BIT,
    UART_CFG_DEF_STOP_BIT,
    UART_CFG_DEF_PARITY,
    UART_CFG_DEF_FLOW_CTRL
};

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
T_Uart_RxCallBack               g_fpaUart_RxCallBackFuncs[UART_IDX_MAX] = {0,};
T_HalUartConfig                 g_taHalUartCurrentConfig[UART_IDX_MAX];
T_HalUartRtSetting              g_taHalUartRuntimeSetting[UART_IDX_MAX] = {0,};

T_Hal_Uart_RxIntEn              Hal_Uart_RxIntEn                = Hal_Uart_RxIntEn_impl;
T_Hal_Uart_RxIntEnStatusGet     Hal_Uart_RxIntEnStatusGet       = Hal_Uart_RxIntEnStatusGet_impl;
T_Hal_Uart_IntIdRead            Hal_Uart_IntIdRead              = Hal_Uart_IntIdRead_impl;
T_Hal_Uart_IntClear             Hal_Uart_IntClear               = Hal_Uart_IntClear_impl;
T_Hal_Uart_BaudRateDetectEn     Hal_Uart_BaudRateDetectEn       = Hal_Uart_BaudRateDetectEn_impl;
T_Hal_Uart_BaudRateDetectDone   Hal_Uart_BaudRateDetectDone     = Hal_Uart_BaudRateDetectDone_impl;
T_Hal_Uart_BaudRateSet          Hal_Uart_BaudRateSet            = Hal_Uart_BaudRateSet_impl;
T_Hal_Uart_BaudRateGet          Hal_Uart_BaudRateGet            = Hal_Uart_BaudRateGet_impl;
T_Hal_Uart_ClockUpdate          Hal_Uart_ClockUpdate            = Hal_Uart_ClockUpdate_impl;
T_Hal_Uart_Init                 Hal_Uart_Init                   = Hal_Uart_Init_impl;
T_Hal_Uart_WakeupResume         Hal_Uart_WakeupResume           = Hal_Uart_WakeupResume_impl;
T_Hal_Uart_EnStatusGet          Hal_Uart_EnStatusGet            = Hal_Uart_EnStatusGet_impl;
T_Hal_Uart_StatusGet            Hal_Uart_StatusGet              = Hal_Uart_StatusGet_impl;
T_Hal_Uart_ConfigGet            Hal_Uart_ConfigGet              = Hal_Uart_ConfigGet_impl;
T_Hal_Uart_ConfigSet            Hal_Uart_ConfigSet              = Hal_Uart_ConfigSet_impl;
T_Hal_Uart_RxCallBackFuncSet    Hal_Uart_RxCallBackFuncSet      = Hal_Uart_RxCallBackFuncSet_impl;
T_Hal_Uart_Halt                 Hal_Uart_Halt                   = Hal_Uart_Halt_impl;
T_Hal_Uart_DmaConfig            Hal_Uart_DmaConfig              = Hal_Uart_DmaConfig_impl;
T_Hal_Uart_ResetFifo            Hal_Uart_ResetFifo              = Hal_Uart_ResetFifo_impl;

// Those function are used for general case, time-out with default value(1 ms)
T_Hal_Uart_DataSend             Hal_Uart_DataSend               = Hal_Uart_DataSend_impl;
T_Hal_Uart_DataRecv             Hal_Uart_DataRecv               = Hal_Uart_DataRecv_impl;

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
T_Hal_Uart_DataSendTimeOut      Hal_Uart_DataSendTimeOut        = Hal_Uart_DataSendTimeOut_impl;
T_Hal_Uart_DataRecvTimeOut      Hal_Uart_DataRecvTimeOut        = Hal_Uart_DataRecvTimeOut_impl;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable
S_UART_Reg_t *const g_caUartRegs[UART_IDX_MAX] = {UART0, UART1, DBG_UART};
const IRQn_Type g_ecaUartIpcIrqs[UART_IDX_MAX] = {UART0_IRQn, UART1_IRQn, DBG_UART_IRQn};
const uint32_t g_cu32aUartRxdPinMask[UART_IDX_MAX] = {PIN_PTS_INMUX_A_UART0_RXD_IO_Msk, PIN_PTS_INMUX_A_UART1_RXD_IO_Msk, PIN_PTS_INMUX_A_APS_UART_RXD_IO_Msk};
const uint32_t g_cu32aUartRxdPinHigh[UART_IDX_MAX] = {
                PIN_INGRP_VAL_UART0_RXD_HIGH << PIN_PTS_INMUX_A_UART0_RXD_IO_Pos,
                PIN_INGRP_VAL_UART1_RXD_HIGH << PIN_PTS_INMUX_A_UART1_RXD_IO_Pos,
                PIN_INGRP_VAL_APS_UART_RXD_HIGH << PIN_PTS_INMUX_A_APS_UART_RXD_IO_Pos};
// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
/*************************************************************************
* FUNCTION:
*  Hal_Uart_RxIntEn
*
* DESCRIPTION:
*   1. Enable rx interrupts
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx : The index of UART. refert to E_UartIdx_t
*   2. u32IrqPriorty: IRQ priority
*   3. u8Enable : 1 for enable/0 for disable the interrupt
*
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_RxIntEn_impl(E_UartIdx_t eUartIdx, uint32_t u32IrqPriorty, uint8_t u8Enable)
{
    S_UART_Reg_t *pUart;
    uint32_t u32Ier;
    
    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    pUart = g_caUartRegs[eUartIdx];
    u32Ier = pUart->IER;

    if(u8Enable)
    {
        Hal_Vic_IntInit(g_ecaUartIpcIrqs[eUartIdx], VIC_TRIG_LEVEL_HIGH, u32IrqPriorty);
        u32Ier |= UART_IER_ERBFI;
    }
    else
    {
        u32Ier &= ~UART_IER_ERBFI;
        Hal_Vic_IntDeInit(g_ecaUartIpcIrqs[eUartIdx]);
    }
    
    pUart->IER = u32Ier;
    g_taHalUartRuntimeSetting[eUartIdx].u32IntEn = u32Ier;
    
    return RESULT_SUCCESS;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_IntIdRead
*
* DESCRIPTION:
*   1. Get interrupt ID
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx : The index of UART. refert to E_UartIdx_t
*
* RETURNS
*   Interrupt status. refer to E_UartIntId_t
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_IntIdRead_impl(E_UartIdx_t eUartIdx)
{
    uint32_t u32Result = 0XF;

    if (eUartIdx < UART_IDX_MAX)
    {
        return (g_caUartRegs[eUartIdx]->IIR & UART_IIR_IID);
    }
    return u32Result;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_IntClear
*
* DESCRIPTION:
*   1. Clear a interrupt
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx : The index of UART. refert to E_UartIdx_t
*   2. eIntID   : Interrupt status. refer to E_UartIntId_t
*   3. pu32Buff : The pointer to return data.  
*                 The return data will diffrent according to eIntID.
*                     UART_INT_MODEM_STAT : MSR (0x018)
*                     UART_INT_NONE       : none
*                     UART_INT_TX_EMPTY   : IIR (0x008)
*                     UART_INT_RX_AVALIBLE: FIFO data
*                     UART_INT_RECV_LINE  : LSR (0x014)
*                     UART_INT_BUSY       : USR (0x07C)
*                     UART_INT_CHAR       : FIFO data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_IntClear_impl(E_UartIdx_t eUartIdx, E_UartIntId_t eIntID, uint32_t *pu32Buff)
{
    S_UART_Reg_t *pUart = 0;

    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    pUart = g_caUartRegs[eUartIdx];
    // Clean module parts
    switch(eIntID)
    {
        case UART_INT_MODEM_STAT:
            *pu32Buff = pUart->MSR;
            break;
        case UART_INT_NONE:
            // Do nothing...
            break;
        case UART_INT_TX_EMPTY:
            *pu32Buff = pUart->IIR;
            break;
        case UART_INT_RX_AVALIBLE:
            *pu32Buff = pUart->RBR;
            break;
        case UART_INT_RECV_LINE:
            *pu32Buff = pUart->LSR;
            break;
        case UART_INT_BUSY:
            *pu32Buff = pUart->USR;
            break;
        case UART_INT_CHAR_TIMEOUT:
            *pu32Buff = pUart->RBR;
            break;
        default:
            return RESULT_FAIL;
    }
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Uart_BaudRateDetectEn
 *        Enable Auto Baud rate detection. Detect pattern: 0x55 ('U')
 * @note  Before enable ABR, setup the target UART and ABR at the same clocl source.
 * @param eUartIdx [in] The index of UART. refert to E_UartIdx_t
 * @return The setup status
 * @retval RESULT_SUCESS Enable success
 * @retval RESULT_FAIL Invalid command
 */
uint32_t Hal_Uart_BaudRateDetectEn_impl(E_UartAbrSrc_t eAbrUartSrc)
{   
    if (eAbrUartSrc > ABR_SRC_SEL_MAX)
        return RESULT_FAIL;
    
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_UART_ABR);
    Hal_Sys_ApsModuleRst(APS_RST_UART_ABR);
    
    SYS->ABR_2_CTRL = (ABR_MONITOR_PULSE << SYS_ABR_2_CTRL_ABR_2_MON_CNT_Pos) | 
                        (eAbrUartSrc << SYS_ABR_2_CTRL_ABR_2_SOURCE_SEL_Pos) |
                        SYS_ABR_2_CTRL_ABR_2_DETECT_EN;
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Uart_BaudRateDetectDone
 * @param pu32Cnt [out] Get the bit time cycle count
 * @param u32MilliSec [in] The timeout count, unit: ms
 * @return The detect status
 * @retval RESULT_SUCESS Detect done and pu32Div is valid number. Disable ABR clock.
 * @retval RESULT_FAIL Not detect done. Won't disable ABR clock in case the next retry waiting.
 */
uint32_t Hal_Uart_BaudRateDetectDone_impl(uint32_t *pu32Cnt, uint32_t u32MilliSec)
{
    uint32_t u32Idx, u32Cnt;
    uint32_t u32NPulseCnt= 0, u32Sum = 0;
    volatile const uint32_t *puReg = &SYS->ABR_2_T0;
    uint32_t u32TimeOutTick = 0, u32TimeOutStart = 0;
    uint32_t u32TimeoutMsMax = Hal_Tick_MilliSecMax();

    if (u32MilliSec > u32TimeoutMsMax)
        u32MilliSec = u32TimeoutMsMax;
    
    u32TimeOutTick = u32MilliSec * Hal_Tick_PerMilliSec();    
    u32TimeOutStart = Hal_Tick_Diff(0);
    while ( !(SYS->ABR_2_CTRL & SYS_ABR_2_CTRL_ABR_2_DETECT_DONE) )
    {   /* Detect ABR pattern timeout */
        if (Hal_Tick_Diff(u32TimeOutStart) > u32TimeOutTick)
            return RESULT_FAIL;
    }
    
    for (u32Idx = 0; u32Idx <= ABR_MONITOR_PULSE; u32Idx++, puReg++)
    {
        if (ABR_PERIODx_VALID(*puReg) == 0)
            continue;
            
        if (ABR_PERIODx_ORIENT(*puReg) == ABR_TX_ORIENT_NEGATIVE)
        {   /* Only sum the negative pulse */
            u32Cnt = ABR_PERIODx_CNT(*puReg);
            if ((u32Cnt <= ABR_CNT_MAX) && (u32Cnt >= ABR_CNT_MIN))
            {   /* Valid counter range */
                u32Sum += u32Cnt;
                u32NPulseCnt++;
            }
        }
    }
    
    Hal_Sys_ApsModuleClkEn(DISABLE, APS_CLK_UART_ABR);
    
    if (u32NPulseCnt)
    {
        *pu32Cnt = ( u32Sum + (u32NPulseCnt >> 1) ) / u32NPulseCnt;
        return RESULT_SUCCESS;
    }
    return RESULT_FAIL;
}


/*************************************************************************
* FUNCTION:
*  Hal_Uart_BaudRateSet
*
* DESCRIPTION:
*   1. Set baur-rate
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: The index of UART. refert to E_UartIdx_t
*   2. u32Baud : Baud-rate
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_BaudRateSet_impl(E_UartIdx_t eUartIdx, uint32_t u32Baud)
{
    S_UART_Reg_t *pUart = 0;
    uint32_t u32Div = 0;
    uint16_t u16DivI, u16DivF;
    uint32_t u32UartClkFreq = 0;
    uint32_t u32RxPinBkp = 0;

    // Check UART Index
    switch (eUartIdx)
    {
        case UART_IDX_0:
            Hal_Sys_ApsClockGet(APS_CLK_GRP_UART0, &u32UartClkFreq);
            break;
        case UART_IDX_1:
            Hal_Sys_ApsClockGet(APS_CLK_GRP_UART1, &u32UartClkFreq);
            break;
        case UART_IDX_DBG:
            Hal_Sys_ApsClockGet(APS_CLK_GRP_APS_UART, &u32UartClkFreq);
            break;
        default:
            return RESULT_FAIL;
    }
    
    pUart = g_caUartRegs[eUartIdx];
    

    // Caculate DIV
    // Fomula: baud rate = (uart clk freq) / (16 * divisor), and round off it (page 92)
    u32Div = (((u32UartClkFreq << UART_DIV_FRACTION_BIT_NUM) >> 4) +  (u32Baud >> 1)) / u32Baud; /* Calculate divisor, 4 LSB are fraction number */
    if ( (u32Div > ((UART_BAUD_DIV_MAX << UART_DIV_FRACTION_BIT_NUM) | UART_DIV_FRACTION_BIT_MSK)) || (u32Div == 0))
        return RESULT_FAIL;
    
    u16DivI = u32Div >> UART_DIV_FRACTION_BIT_NUM;
    u16DivF = u32Div &  UART_DIV_FRACTION_BIT_MSK;

    g_taHalUartRuntimeSetting[eUartIdx].u8DivFrac = u16DivF;
    g_taHalUartRuntimeSetting[eUartIdx].u8DivIntLow = u16DivI & UART_BAUD_DIV_L_MASK;
    g_taHalUartRuntimeSetting[eUartIdx].u8DivIntHigh = (u16DivI >> 8) & UART_BAUD_DIV_H_MASK;
    
    /* Halt TX and Disable RX input */
    Hal_Uart_Halt(eUartIdx, 1, &u32RxPinBkp);

    // Latch to Div setting mode
    pUart->LCR |= UART_LCR_DLAB;
    pUart->DLL = g_taHalUartRuntimeSetting[eUartIdx].u8DivIntLow;
    pUart->DLH = g_taHalUartRuntimeSetting[eUartIdx].u8DivIntHigh;
    pUart->DLF = u16DivF;
    pUart->LCR &= ~UART_LCR_DLAB;
    
    /* Recover TX and RX input */
    Hal_Uart_Halt(eUartIdx, 0, &u32RxPinBkp);

    g_taHalUartCurrentConfig[eUartIdx].ulBuadrate = u32Baud;
    return RESULT_SUCCESS;
}


/*************************************************************************
* FUNCTION:
*  Hal_Uart_BaudRateGet
*
* DESCRIPTION:
*   1. Get baur-rate.
*
* CALLS
*
* PARAMETERS
*
* RETURNS
*   Baud-rate
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_BaudRateGet_impl(E_UartIdx_t eUartIdx)
{
    if (eUartIdx >= UART_IDX_MAX)
        return 0;
    
    return g_taHalUartCurrentConfig[eUartIdx].ulBuadrate;
}


/*************************************************************************
* FUNCTION:
*  Hal_Uart_ClockUpdate
*
* DESCRIPTION:
*   1. Update UART clock, called when UART clock source changed
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: The index of UART. refert to E_UartIdx_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Uart_ClockUpdate_impl(E_UartIdx_t eUartIdx)
{
    if (g_taHalUartRuntimeSetting[eUartIdx].u8Enable)
        Hal_Uart_BaudRateSet(eUartIdx, g_taHalUartCurrentConfig[eUartIdx].ulBuadrate);
}



/*************************************************************************
* FUNCTION:
*  Hal_Uart_Init
*
* DESCRIPTION:
*   1. uart initail function
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx   : The index of UART. refert to E_UartIdx_t
*   2. u32Baud    : Baud-rate
*   3. u8Bits     : Data bits. refert to E_UartDataBit_t
*   4. u8Parity   : Parity mode. refer to E_UartParity_t
*   5. u8StopBit  : Stop bits. refert to E_UartStopBit_t 
*   6. u8EnFlowCtl: Enable/Disable CTS and RTS. 1 for enable/0 for disable 
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_Init_impl(E_UartIdx_t eUartIdx, uint32_t u32Baud, E_UartDataBit_t u8Bits, E_UartParity_t u8Parity, E_UartStopBit_t u8StopBit, uint8_t u8EnFlowCtl)
{
    T_HalUartConfig tConfig = {
        .ulBuadrate = u32Baud,
        .ubDataBit = u8Bits,
        .ubStopBit = u8StopBit,
        .ubParity = u8Parity,
        .ubFlowCtrl = u8EnFlowCtl,
    };
    
    switch (eUartIdx)
    {
        case UART_IDX_0:
            Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_UART_0);
            Hal_Sys_ApsModuleRst(APS_RST_UART_0);
            break;
        case UART_IDX_1:
            Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_UART_1);
            Hal_Sys_ApsModuleRst(APS_RST_UART_1);
            break;
        case UART_IDX_DBG:
            Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_DBG_UART);
            Hal_Sys_ApsModuleRst(APS_RST_DBG_UART);
            break;
        default:
            return RESULT_FAIL;
    }
    g_taHalUartRuntimeSetting[eUartIdx].u8Enable = 0;
    
    if (0 != Hal_Uart_ConfigSet(eUartIdx, &tConfig))
        return RESULT_FAIL;
    
    /* FIFO control */
    g_taHalUartRuntimeSetting[eUartIdx].u32FifoCtrl = UART_FCR_CFG_NON_DMA;
    g_caUartRegs[eUartIdx]->FCR = g_taHalUartRuntimeSetting[eUartIdx].u32FifoCtrl;
    
    /* IER, disable all interrupts */
    g_taHalUartRuntimeSetting[eUartIdx].u32IntEn = 0;
    g_caUartRegs[eUartIdx]->IER = 0;
    
    g_taHalUartRuntimeSetting[eUartIdx].u8Enable = 1;
    
    return RESULT_SUCCESS;
}


/*************************************************************************
* FUNCTION:
*  Hal_Uart_WakeupResume
*
* DESCRIPTION:
*   1. Resume UART setting when warmboot
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx   : The index of UART. refert to E_UartIdx_t
*
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Uart_WakeupResume_impl(E_UartIdx_t eUartIdx)
{
    S_UART_Reg_t *pUart = 0;
    uint32_t u32RxPinBkp = 0;
    
    if (eUartIdx >= UART_IDX_MAX)
        return;

    if (!g_taHalUartRuntimeSetting[eUartIdx].u8Enable)
    {   /* Not enabled, skip resume */
        return;
    }
    
    pUart = g_caUartRegs[eUartIdx];
    pUart->MCR = g_taHalUartRuntimeSetting[eUartIdx].u32ModemCtrl;

    /* Halt UART */
    Hal_Uart_Halt(eUartIdx, 1, &u32RxPinBkp);
    
    /* Setup divisor */
    pUart->LCR |= UART_LCR_DLAB;
    pUart->DLL = g_taHalUartRuntimeSetting[eUartIdx].u8DivIntLow;
    pUart->DLH = g_taHalUartRuntimeSetting[eUartIdx].u8DivIntHigh;
    pUart->DLF = g_taHalUartRuntimeSetting[eUartIdx].u8DivFrac;
    pUart->LCR &= ~UART_LCR_DLAB;
    Hal_Uart_Halt(eUartIdx, 0, &u32RxPinBkp);
    
    pUart->LCR = g_taHalUartRuntimeSetting[eUartIdx].u32LineCtrl;
    pUart->FCR = g_taHalUartRuntimeSetting[eUartIdx].u32FifoCtrl;
    pUart->IER = g_taHalUartRuntimeSetting[eUartIdx].u32IntEn;

}


/*************************************************************************
* FUNCTION:
*  Hal_Uart_ConfigGet
*
* DESCRIPTION:
*   1. get the current config of UART
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx    : The index of UART. refert to E_UartIdx_t
*   2. ptConfig    : the config of UART
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_ConfigGet_impl(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig)
{
    // Check UART Index
    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    // get the current config of UART
    memcpy(ptConfig, &(g_taHalUartCurrentConfig[eUartIdx]), sizeof(T_HalUartConfig));
    return RESULT_SUCCESS;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_ConfigSet
*
* DESCRIPTION:
*   1. set the current config of UART
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx    : The index of UART. refert to E_UartIdx_t
*   2. ptConfig    : the config of UART
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_ConfigSet_impl(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig)
{
    S_UART_Reg_t *pUart = 0;
    uint32_t u32Lcr = 0;

    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    pUart = g_caUartRegs[eUartIdx];

    /* Check StopBit setting */
    if ((ptConfig->ubStopBit == STOP_BIT_1P5) && (ptConfig->ubDataBit != DATA_BIT_5))
    {   /* Only Bits = 5 can set StopBit =1.5 */
        return RESULT_FAIL;
    }
    
    /* Write MCR */
    g_taHalUartRuntimeSetting[eUartIdx].u32ModemCtrl = (ptConfig->ubFlowCtrl)?(UART_MCR_AFCE | UART_MCR_RTS):0;
    pUart->MCR = g_taHalUartRuntimeSetting[eUartIdx].u32ModemCtrl;
    
    /* Write divisor */
    Hal_Uart_BaudRateSet(eUartIdx, ptConfig->ulBuadrate);
    
    /* Write LCR */
    switch (ptConfig->ubParity)
    {
        case PARITY_NONE:
            u32Lcr |= UART_PARITY_NONE;
            break;
        case PARITY_EVEN:
            u32Lcr |= UART_PARITY_EVEN;
            break;
        case PARITY_ODD:
            u32Lcr |= UART_PARITY_ODD;
            break;
        default:
            break;
    }
    switch (ptConfig->ubStopBit)
    {
        case STOP_BIT_1:
            u32Lcr |= UART_STOP_BIT_1_BIT;
            break;
        case STOP_BIT_1P5:
            u32Lcr |= UART_STOP_BIT_1p5_OR_2_BIT;
            break;
        case STOP_BIT_2:
            u32Lcr |= UART_STOP_BIT_1p5_OR_2_BIT;
            break;
        default:
            break;
    }
    switch (ptConfig->ubDataBit)
    {
        case DATA_BIT_5:
            u32Lcr |= UART_DATA_LEN_5BITS;
            break;
        case DATA_BIT_6:
            u32Lcr |= UART_DATA_LEN_6BITS;
            break;
        case DATA_BIT_7:
            u32Lcr |= UART_DATA_LEN_7BITS;
            break;
        case DATA_BIT_8:
            u32Lcr |= UART_DATA_LEN_8BITS;
            break;
        default:
            break;
    }
    g_taHalUartRuntimeSetting[eUartIdx].u32LineCtrl = u32Lcr;
    pUart->LCR = u32Lcr;
    
    // save the current config
    memcpy(&(g_taHalUartCurrentConfig[eUartIdx]), ptConfig, sizeof(T_HalUartConfig));
    return RESULT_SUCCESS;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_DataSendTimeOut
*
* DESCRIPTION:
*   1. Sent a data
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx    : Index of UART. refert to E_UartIdx_t
*   2. u32Data    : Value of data
*   3. u32MilliSec: Time-Out value. Recommand not over 10 sec (Estimate by 200Mhz system clock)
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_DataSendTimeOut_impl(E_UartIdx_t eUartIdx, uint32_t u32Data, uint32_t u32MilliSec)
{
    S_UART_Reg_t *pUart = 0;
    uint32_t u32TimeOutTick = 0;
    uint32_t u32TimeOutStart = 0;
    uint32_t u32TimeoutMsMax;

    
    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    pUart = g_caUartRegs[eUartIdx];
    pUart->THR = u32Data;
    
    u32TimeoutMsMax = Hal_Tick_MilliSecMax();
    if (u32MilliSec > u32TimeoutMsMax)
        u32MilliSec = u32TimeoutMsMax;
    u32TimeOutTick = u32MilliSec * Hal_Tick_PerMilliSec();

    u32TimeOutStart = Hal_Tick_Diff(0);

    while ( !(pUart->LSR & UART_LSR_THRE) )
    {
        // loop when TX is not empty
        if (Hal_Tick_Diff(u32TimeOutStart) > u32TimeOutTick)
            return RESULT_FAIL;
    }
    return RESULT_SUCCESS;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_DataSend
*
* DESCRIPTION:
*   1. Sent a data
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: Index of UART. refert to E_UartIdx_t
*   2. u32Data: Value of data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_DataSend_impl(E_UartIdx_t eUartIdx, uint32_t u32Data)
{
    S_UART_Reg_t *pUart = 0;
    uint32_t u32TimeOutCount = 0;
  
    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    pUart = g_caUartRegs[eUartIdx];
    pUart->THR = u32Data;

    while( !(pUart->LSR & UART_LSR_THRE) )
    {
        // loop when TX is not empty
        if(u32TimeOutCount > UART_TIMEOUT_COUNT_MAX)
            return RESULT_FAIL;
        u32TimeOutCount++;
    }
    return RESULT_SUCCESS;

}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_DataRecvTimeOut
*
* DESCRIPTION:
*   1. Receive a data
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*   2. pu32Data: pointer of data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_DataRecvTimeOut_impl(E_UartIdx_t eUartIdx, uint32_t* pu32Data, uint32_t u32MilliSec)
{
    S_UART_Reg_t *pUart = 0;
    uint32_t u32TimeOutTick = 0;
    uint32_t u32TimeOutStart = 0;
    uint32_t u32TimeoutMsMax;
    
    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    pUart = g_caUartRegs[eUartIdx];
    
    u32TimeoutMsMax = Hal_Tick_MilliSecMax();
    if (u32MilliSec > u32TimeoutMsMax)
        u32MilliSec = u32TimeoutMsMax;
    u32TimeOutTick = u32MilliSec * Hal_Tick_PerMilliSec();
    
    u32TimeOutStart = Hal_Tick_Diff(0);
    
    while( !(pUart->LSR & UART_LSR_DR) )
    {
        // loop when RX is empty
        if (Hal_Tick_Diff(u32TimeOutStart) > u32TimeOutTick)
            return RESULT_FAIL;
    }

    *pu32Data = pUart->RBR;
    return RESULT_SUCCESS;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_DataRecv
*
* DESCRIPTION:
*   1. Receive a data
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*   2. pu32Data: pointer of data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Uart_DataRecv_impl(E_UartIdx_t eUartIdx, uint32_t* pu32Data)
{
    S_UART_Reg_t *pUart = 0;
    uint32_t u32TimeOutCount = 0;

    
    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    pUart = g_caUartRegs[eUartIdx];
    while( !(pUart->LSR & UART_LSR_DR) )
    {
        // loop when RX is empty
        if(u32TimeOutCount > UART_TIMEOUT_COUNT_MAX)
            return RESULT_FAIL;
        u32TimeOutCount++;
    }

    *pu32Data = pUart->RBR;
    return RESULT_SUCCESS;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_RxCallBackFuncSet
*
* DESCRIPTION:
*   1. Setup rx callback function
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*   2. tFunc: Callback function
*
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Uart_RxCallBackFuncSet_impl(E_UartIdx_t eUartIdx, T_Uart_RxCallBack tFunc)
{
    if (eUartIdx < UART_IDX_MAX)
        g_fpaUart_RxCallBackFuncs[eUartIdx] = tFunc;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_RxIntEnStatusGet
*
* DESCRIPTION:
*   1. Get RX interrupt enable status
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*
* RETURNS
*   the status of enable rx interrupt
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint8_t Hal_Uart_RxIntEnStatusGet_impl(E_UartIdx_t eUartIdx)
{
    if (eUartIdx >= UART_IDX_MAX)
        return 0;
    
    return (g_taHalUartRuntimeSetting[eUartIdx].u32IntEn & UART_IER_ERBFI)?1:0;
}


/*************************************************************************
* FUNCTION:
*  Hal_Uart_EnStatusGet
*
* DESCRIPTION:
*   1. Get enable status
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*
* RETURNS
*   the status of enable
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint8_t Hal_Uart_EnStatusGet_impl(E_UartIdx_t eUartIdx)
{
    if (eUartIdx >= UART_IDX_MAX)
        return 0;
    
    return g_taHalUartRuntimeSetting[eUartIdx].u8Enable;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_StatusGet
*
* DESCRIPTION:
*   1. Get enable status
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*
* RETURNS
*   the status of enable. SET or RESET
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint8_t Hal_Uart_StatusGet_impl(E_UartIdx_t eUartIdx, E_UART_STATUS eStatus)
{
    uint8_t eRet = RESET;
    S_UART_Reg_t *pUart = 0;
    
    if (eUartIdx >= UART_IDX_MAX)
        return eRet;
    
    pUart = g_caUartRegs[eUartIdx];
    switch (eStatus & _UART_STATUS_REG_MSK)
    {
        case _UART_STATUS_LSR:
            eRet = (pUart->LSR & eStatus)?SET:RESET;
            break;
        case _UART_STATUS_USR:
            eRet = (pUart->USR & eStatus)?SET:RESET;
            break;
        default:
            break;
    }
    
    return eRet;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_Halt
*
* DESCRIPTION:
*   1. Flush FIFO and disable RX pin
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*   2. u8Halt: 1 to halt and 0 to recover
*   3. pu32PinBkp: Store pin cfg before halt, restore when recover
*
* RETURNS
*   the status of enable. SET or RESET
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint8_t Hal_Uart_Halt_impl(E_UartIdx_t eUartIdx, uint8_t u8Halt, uint32_t *pu32PinBkp)
{
    S_UART_Reg_t *pUart = 0;
    
    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    pUart = g_caUartRegs[eUartIdx];
    
    if (u8Halt)
    {
        /* Set RX pin to HIGH level */
        *pu32PinBkp = PIN->PTS_INMUX_A & g_cu32aUartRxdPinMask[eUartIdx];
        PIN->PTS_INMUX_A = (PIN->PTS_INMUX_A & ~g_cu32aUartRxdPinMask[eUartIdx]) | g_cu32aUartRxdPinHigh[eUartIdx];
        pUart->FCR = g_taHalUartRuntimeSetting[eUartIdx].u32FifoCtrl | UART_FCR_XFIFOR | UART_FCR_RFIFOR;
    }
    else
    {
        PIN->PTS_INMUX_A = (PIN->PTS_INMUX_A & ~g_cu32aUartRxdPinMask[eUartIdx]) | *pu32PinBkp;
    }
    
    return RESULT_SUCCESS;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_DmaConfig
*
* DESCRIPTION:
*   1. Init UART DMA mode
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*   2. eDmaMode: DMA mode 0 - single, or DMA mode 1 - multi
*   3. eTxLvl: TX watermark
*   4. eRxLvl: RX watermark
*
* RETURNS
*   the status of enable. SET or RESET
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint8_t Hal_Uart_DmaConfig_impl(E_UartIdx_t eUartIdx, E_UartDmaMode_t eDmaMode, E_UartDmaTxLevel_t eTxLvl, E_UartDmaRxLevel_t eRxLvl)
{
    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    g_taHalUartRuntimeSetting[eUartIdx].u32FifoCtrl = UART_FCR_FIFOE | eDmaMode | eTxLvl | eRxLvl;
    g_caUartRegs[eUartIdx]->FCR = g_taHalUartRuntimeSetting[eUartIdx].u32FifoCtrl;
    
    return RESULT_SUCCESS;
}

/*************************************************************************
* FUNCTION:
*  Hal_Uart_Reset
*
* DESCRIPTION:
*   1.  Reset TX/RX FIFO
*
* CALLS
*
* PARAMETERS
*   1. eUartIdx: index of UART. refert to E_UartIdx_t
*   2. u8ResetTx: 0 - no effect, 1 - Reset TX FIFO
*   3. u8ResetRx: 0 - no effect, 1 - Reset RX FIFO
*
* RETURNS
*   the status of enable. SET or RESET
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint8_t Hal_Uart_ResetFifo_impl(E_UartIdx_t eUartIdx, uint8_t u8ResetTx, uint8_t u8ResetRx)
{
    uint32_t u32Fcr;
    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    u32Fcr = g_taHalUartRuntimeSetting[eUartIdx].u32FifoCtrl;
    if (u8ResetTx)
        u32Fcr |= UART_FCR_XFIFOR;
    if (u8ResetRx)
        u32Fcr |= UART_FCR_RFIFOR;
    
    g_caUartRegs[eUartIdx]->FCR = u32Fcr;
    
    return RESULT_SUCCESS;
}
