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

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

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
 




 
 
 
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Uart_PatchInit(void)
{
    Hal_Uart_WakeupResume = Hal_Uart_WakeupResume_patch;
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

