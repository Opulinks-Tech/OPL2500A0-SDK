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
#include "hal_vic.h"
#include "hal_uart.h"
#include "hal_sys_rcc.h"
#include "hal_system.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define IPC_IRQ_MSQ_HARDFAULT       IPC_IDX_0
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef struct
{
    uint32_t Stack[8];
    uint32_t MSP;
    uint32_t LR;
} S_MSQ_HARDFAULT_STATUS;
/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
void ExceptionDumpStack(uint32_t u32RegPsp, uint32_t u32RegMsp, uint32_t u32RegLr);
void HardFault_Handler_Entry_patch(void);
void IPC0_IRQHandler_Entry_patch(void);
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
    IPC0_IRQHandler_Entry   = IPC0_IRQHandler_Entry_patch;
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
        tracer_drct_printf("Hard fault\n");
        tracer_drct_printf("0xE000ED00: %08X %08X %08X %08X\n", 
                    reg_read(0xE000ED00), reg_read(0xE000ED04), reg_read(0xE000ED08), reg_read(0xE000ED0C));
        tracer_drct_printf("0xE000ED10: %08X %08X %08X %08X\n", 
                    reg_read(0xE000ED10), reg_read(0xE000ED14), reg_read(0xE000ED18), reg_read(0xE000ED1C));
        tracer_drct_printf("0xE000ED20: %08X %08X %08X %08X\n", 
                    reg_read(0xE000ED20), reg_read(0xE000ED24), reg_read(0xE000ED28), reg_read(0xE000ED2C));
        tracer_drct_printf("0xE000ED30: %08X %08X %08X %08X\n", 
                    reg_read(0xE000ED30), reg_read(0xE000ED34), reg_read(0xE000ED38), reg_read(0xE000ED3C));
        tracer_drct_printf("0xE000E400: %08X %08X %08X %08X\n", 
                    reg_read(0xE000E400), reg_read(0xE000E404), reg_read(0xE000E408), reg_read(0xE000E40C));
        tracer_drct_printf("0xE000E410: %08X %08X %08X %08X\n", 
                    reg_read(0xE000E410), reg_read(0xE000E414), reg_read(0xE000E418), reg_read(0xE000E41C));
        
        ExceptionDumpStack(g_sExceptionInfo.u32Psp, g_sExceptionInfo.u32Msp, g_sExceptionInfo.u32Lr);
        
//        if (Hal_Sys_StrapModeRead() ==  STRAP_MODE_ICE)
//        {
//            while (1);
//        }
    }
    Hal_Sys_SwResetAll();
}



/* Used by MSQ inform MSQ hard fault */
void IPC0_IRQHandler_Entry_patch(void)
{
    S_MSQ_HARDFAULT_STATUS *pMsqHfSts;
    // Clear interrupt
    Hal_Vic_IpcIntClear(IPC_IDX_0);

    tracer_drct_printf("MSQ hard fault\n");
    
    pMsqHfSts = (S_MSQ_HARDFAULT_STATUS *)SYSTEM_SPARE_REG_MSQ_MSP;
    tracer_drct_printf("LR  = 0x%08X\n", pMsqHfSts->LR);
    tracer_drct_printf("MSP = 0x%08X\n", pMsqHfSts->MSP);
    tracer_drct_printf("Stack:\n");
    tracer_drct_printf("  0x%08X 0x%08X 0x%08X 0x%08X\n",
                        pMsqHfSts->Stack[0], pMsqHfSts->Stack[1], pMsqHfSts->Stack[2], pMsqHfSts->Stack[3]);
    tracer_drct_printf("  0x%08X 0x%08X 0x%08X 0x%08X\n\n\n",
                        pMsqHfSts->Stack[4], pMsqHfSts->Stack[5], pMsqHfSts->Stack[6], pMsqHfSts->Stack[7]);
    
    Hal_Sys_SwResetAll();
}
