/* *****************************************************************************
 *  Copyright 2017 - 2022, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2022
 *
 *******************************************************************************
 *
 *  @file hal_uart_patch.c
 * 
 *  @brief UART patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "hal_uart.h"
#include "hal_sys_rcc.h"
#include "hal_tick.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define UART_DIVI_MAX_ERR_RATE_PERCENT            ((float)5.0)  /* When error rate > 5%, force fraction number */
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
void Hal_Uart_WakeupResume_patch(E_UartIdx_t eUartIdx);
uint32_t Hal_Uart_BaudRateSet_patch(E_UartIdx_t eUartIdx, uint32_t u32Baud);
uint32_t Hal_Uart_DataSendTimeOut_patch(E_UartIdx_t eUartIdx, uint32_t u32Data, uint32_t u32MilliSec);
uint32_t Hal_Uart_DataSend_patch(E_UartIdx_t eUartIdx, uint32_t u32Data);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
uint8_t g_u8Hal_WakeupResumeDone[UART_IDX_MAX] = {1, 1, 1};

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 
extern S_UART_Reg_t *const g_caUartRegs[UART_IDX_MAX];

extern T_Uart_RxCallBack g_fpaUart_RxCallBackFuncs[UART_IDX_MAX];
extern T_HalUartConfig g_taHalUartCurrentConfig[UART_IDX_MAX];
extern T_HalUartRtSetting g_taHalUartRuntimeSetting[UART_IDX_MAX];



 
 
 
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Uart_PatchInit(void)
{
    Hal_Uart_WakeupResume = Hal_Uart_WakeupResume_patch;
    Hal_Uart_BaudRateSet = Hal_Uart_BaudRateSet_patch;
    Hal_Uart_DataSendTimeOut = Hal_Uart_DataSendTimeOut_patch;
    Hal_Uart_DataSend = Hal_Uart_DataSend_patch;
}

void Hal_Uart_SleepPrepare(void)
{
    S_UART_Reg_t *pUart = 0;
    for(E_UartIdx_t eUartIdx = UART_IDX_0; eUartIdx<UART_IDX_MAX; eUartIdx++)
    {
        if( g_taHalUartRuntimeSetting[ eUartIdx ].u8Enable )
        {
            pUart = g_caUartRegs[ eUartIdx ];
            // loop until UART(TX) is done
            while(pUart->USR & UART_USR_BUSY){ __NOP(); };
        }
    }

    memset(g_u8Hal_WakeupResumeDone, 0, sizeof(g_u8Hal_WakeupResumeDone));
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

void Hal_Uart_WakeupResume_patch(E_UartIdx_t eUartIdx)
{
    if (g_u8Hal_WakeupResumeDone[eUartIdx])
        return;
    
    Hal_Uart_WakeupResume_impl(eUartIdx);
    
    g_u8Hal_WakeupResumeDone[eUartIdx] = 1;
}

/*
 * @brief Calculate baud rate error rate
 *
 *      Divider   = (Uart clock) / (16 * Baud rate)
 *      Baud rate = (Uart clock) / (16 * Divider)
 *
 *      Error rate (%) = ABS(Expected baud rate - Calculated baud rate) * 100 / (Expected baud rate)
 *
 * @param u32Baudrate[in] Expected baud rate
 * @param u32UartClk[in] UART clock source frequency
 * @param u16DivI[in] Divider integer part
 * @param u16DivF[in] Divider fractional part.
 * @return Error rate in percentage
 */
float _Hal_Uart_CalcBaudErrRate(uint32_t u32Baudrate, uint32_t u32UartClk, uint16_t u16DivI, uint16_t u16DivF)
{
    uint32_t u32CalcBaud;
    uint32_t u32BaudDiff;
    float fErrRate;
    float fDiv;
    
    fDiv = (u16DivI + (float)u16DivF/(1 << UART_DIV_FRACTION_BIT_NUM))*16;
    u32CalcBaud = (u32UartClk) / fDiv;
    u32BaudDiff = (u32CalcBaud > u32Baudrate)?(u32CalcBaud - u32Baudrate):(u32Baudrate - u32CalcBaud);
    fErrRate = ((float)u32BaudDiff/u32Baudrate) * 100;
    
    return fErrRate;
}

uint32_t Hal_Uart_BaudRateSet_patch(E_UartIdx_t eUartIdx, uint32_t u32Baud)
{
    S_UART_Reg_t *pUart = 0;
    uint32_t u32Div = 0;
    uint16_t u16DivI, u16DivF;
    uint32_t u32UartClkFreq = 0;
    uint32_t u32RxPinBkp = 0;
    float fErrRate;

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
    /* Divider = clock / (16 * baud) */
    
    /* Try divider integer part only */
    u32Div = (u32UartClkFreq + u32Baud*8) / (u32Baud * 16);
    if ((u32Div == 0) || (u32Div > UART_DIVIDER_INT_MAX_VALUE))
        return RESULT_FAIL;
    fErrRate = _Hal_Uart_CalcBaudErrRate(u32Baud, u32UartClkFreq, u32Div, 0);
    if (fErrRate < UART_DIVI_MAX_ERR_RATE_PERCENT)
    {
        u16DivI = u32Div;
        u16DivF = 0;
        goto setup;
    }
    
    /* Integer divider accuracy is not enought, try fraction number */
    u32Div = (((u32UartClkFreq << UART_DIV_FRACTION_BIT_NUM) >> 4) +  (u32Baud >> 1)) / u32Baud; /* Calculate divisor, 4 LSB are fraction number */
    if (((u32Div >> UART_DIV_FRACTION_BIT_NUM) == 0) || ((u32Div >> UART_DIV_FRACTION_BIT_NUM) > UART_DIVIDER_INT_MAX_VALUE))
        return RESULT_FAIL;
    u16DivI = u32Div >> UART_DIV_FRACTION_BIT_NUM;
    u16DivF = u32Div &  UART_DIV_FRACTION_BIT_MSK;
    fErrRate = _Hal_Uart_CalcBaudErrRate(u32Baud, u32UartClkFreq, u16DivI, u16DivF);
    if (fErrRate < UART_DIVI_MAX_ERR_RATE_PERCENT)
    {   /* Baud rate is valid */
        goto setup;
    }
    
    /* Current clock source can NOT generate expected baud rate */
    return RESULT_FAIL;
    
setup:
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
uint32_t Hal_Uart_DataSendTimeOut_patch(E_UartIdx_t eUartIdx, uint32_t u32Data, uint32_t u32MilliSec)
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

    while (1)
    {
        if (pUart->USR & UART_USR_TFNF)
        {   /* Send when TX FIFO not full */
            pUart->THR = u32Data;
            break;
        }
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
uint32_t Hal_Uart_DataSend_patch(E_UartIdx_t eUartIdx, uint32_t u32Data)
{
    S_UART_Reg_t *pUart = 0;
    uint32_t u32TimeOutCount = 0;
  
    if (eUartIdx >= UART_IDX_MAX)
        return RESULT_FAIL;
    
    pUart = g_caUartRegs[eUartIdx];
    
    while (1)
    {
        if (pUart->USR & UART_USR_TFNF)
        {   /* Send when TX FIFO not full */
            pUart->THR = u32Data;
            break;
        }
        if (u32TimeOutCount > UART_TIMEOUT_COUNT_MAX)
            return RESULT_FAIL;
        u32TimeOutCount++;
    }
    
    return RESULT_SUCCESS;

}
