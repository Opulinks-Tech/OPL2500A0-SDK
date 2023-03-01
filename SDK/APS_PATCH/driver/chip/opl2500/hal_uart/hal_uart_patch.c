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
}

void Hal_Uart_SleepPrepare(void)
{
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

