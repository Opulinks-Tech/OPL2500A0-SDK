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
 *  @file irq_patch.c
 * 
 *  @brief IRQ patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "irq.h"
#include "hal_uart.h"
#include "hal_sys_rcc.h"

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
void HardFault_Handler_Entry_patch(void);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


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

void Irq_PatchInit(void)
{
    HardFault_Handler_Entry = HardFault_Handler_Entry_patch;
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */


void HardFault_Handler_Entry_patch(void)
{
    Hal_Sys_ResetStatusClear(0xFFFFFFFF);
    if (Hal_Uart_EnStatusGet(UART_IDX_DBG))
    {
        Hal_Uart_WakeupResume(UART_IDX_DBG);
        HardFault_Handler_Entry_impl();
    }
    else
    {
        Hal_Sys_SwResetAll();
    }
}

