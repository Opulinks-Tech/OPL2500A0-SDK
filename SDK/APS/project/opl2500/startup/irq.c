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

#include "opulinks.h"
#include "cmsis_os.h"
#include "irq.h"

#include "hal_uart.h"
#include "hal_vic.h"
#include "hal_gpio.h"
#include "hal_sys_rcc.h"
#include "hal_tmr.h"
#include "hal_wdt.h"
#include "hal_i2c.h"
#include "hal_dma.h"
#include "hal_msq_dma.h"

#include "ipc.h"
#include "diag_task.h"
#include "ps.h"
__STATIC_INLINE uint32_t __get_LR(void)
{
    register uint32_t __regLR;
    __ASM {mov __regLR, __return_address()}
    return __regLR;
}



S_EXCEPTION_INFO g_sExceptionInfo; 


T_InterruptHandler  HardFault_Handler_Entry     = HardFault_Handler_Entry_impl;
T_InterruptHandler  IPC0_IRQHandler_Entry       = IPC0_IRQHandler_Entry_impl;
T_InterruptHandler  IPC1_IRQHandler_Entry       = IPC1_IRQHandler_Entry_impl;
T_InterruptHandler  IPC2_IRQHandler_Entry       = IPC2_IRQHandler_Entry_impl;
T_InterruptHandler  IPC3_IRQHandler_Entry       = IPC3_IRQHandler_Entry_impl;
T_InterruptHandler  GPIO_IRQHandler_Entry       = GPIO_IRQHandler_Entry_impl;
T_InterruptHandler  SCRT_IRQHandler_Entry       = SCRT_IRQHandler_Entry_impl;
T_InterruptHandler  MSQ_IRQHandler_Entry        = MSQ_IRQHandler_Entry_impl;
T_InterruptHandler  UARTDBG_IRQHandler_Entry    = UARTDBG_IRQHandler_Entry_impl;
T_InterruptHandler  UART0_IRQHandler_Entry      = UART0_IRQHandler_Entry_impl;
T_InterruptHandler  UART1_IRQHandler_Entry      = UART1_IRQHandler_Entry_impl;
T_InterruptHandler  I2C_IRQHandler_Entry        = I2C_IRQHandler_Entry_impl;
T_InterruptHandler  SPI0_IRQHandler_Entry       = SPI0_IRQHandler_Entry_impl;
T_InterruptHandler  SPI1_IRQHandler_Entry       = SPI1_IRQHandler_Entry_impl;
T_InterruptHandler  SPI2_IRQHandler_Entry       = SPI2_IRQHandler_Entry_impl;
T_InterruptHandler  TIM0_IRQHandler_Entry       = TIM0_IRQHandler_Entry_impl;
T_InterruptHandler  TIM1_IRQHandler_Entry       = TIM1_IRQHandler_Entry_impl;
T_InterruptHandler  WDT_IRQHandler_Entry        = WDT_IRQHandler_Entry_impl;
T_InterruptHandler  JTAG_IRQHandler_Entry       = JTAG_IRQHandler_Entry_impl;
T_InterruptHandler  APS_DMA_IRQHandler_Entry    = APS_DMA_IRQHandler_Entry_impl;
T_InterruptHandler  I2S_IRQHandler_Entry        = I2S_IRQHandler_Entry_impl;
T_InterruptHandler  SPI3_IRQHandler_Entry       = SPI3_IRQHandler_Entry_impl;
T_InterruptHandler  MSQ_DMA_IRQHandler_Entry    = MSQ_DMA_IRQHandler_Entry_impl;
T_InterruptHandler  PDM_IRQHandler_Entry        = PDM_IRQHandler_Entry_impl;
T_InterruptHandler  AUX_ADC_IRQHandler_Entry    = AUX_ADC_IRQHandler_Entry_impl;


// --------------------------------- ISR Entry---------------------------------
void ExceptionDumpStack(uint32_t u32RegPsp, uint32_t u32RegMsp, uint32_t u32RegLr)
{   
    uint32_t *pu32Psp = (uint32_t *)u32RegPsp;
    uint32_t *pu32Msp = (uint32_t *)u32RegMsp;
    
    tracer_drct_printf("LR=0x%08X(", u32RegLr);
    if ((u32RegLr == 0xFFFFFFF1) || (u32RegLr == 0xFFFFFFF9) || (u32RegLr == 0xFFFFFFE1) || (u32RegLr == 0xFFFFFFE9))
        tracer_drct_printf("MSP");
    else if ((u32RegLr == 0xFFFFFFFD) || (u32RegLr == 0xFFFFFFED))
        tracer_drct_printf("PSP");
    else 
        tracer_drct_printf("?");
    
    tracer_drct_printf(")\n      PSP      MSP\n");
    tracer_drct_printf("R0  : %08X %08X\n", *(pu32Psp+0), *(pu32Msp+0));
    tracer_drct_printf("R1  : %08X %08X\n", *(pu32Psp+1), *(pu32Msp+1));
    tracer_drct_printf("R2  : %08X %08X\n", *(pu32Psp+2), *(pu32Msp+2));
    tracer_drct_printf("R3  : %08X %08X\n", *(pu32Psp+3), *(pu32Msp+3));
    tracer_drct_printf("R12 : %08X %08X\n", *(pu32Psp+4), *(pu32Msp+4));
    tracer_drct_printf("LR  : %08X %08X\n", *(pu32Psp+5), *(pu32Msp+5));
    tracer_drct_printf("PC  : %08X %08X\n", *(pu32Psp+6), *(pu32Msp+6));
    tracer_drct_printf("xPSR: %08X %08X\n", *(pu32Psp+7), *(pu32Msp+7));

    tracer_drct_printf("Current task: %s\n\n\n", pcTaskGetName(osThreadGetId()));
}
void HardFault_Handler_Entry_impl(void)
{
    tracer_drct_printf("Hard fault\nE000ED28: %08X %08X", reg_read(0xE000ED28), reg_read(0xE000ED2C));
    tracer_drct_printf(" %08X %08X %08X %08X\n\n", reg_read(0xE000ED30), reg_read(0xE000ED34), reg_read(0xE000ED38), reg_read(0xE000ED3C));
    ExceptionDumpStack(g_sExceptionInfo.u32Psp, g_sExceptionInfo.u32Msp, g_sExceptionInfo.u32Lr);
    while (Hal_Uart_StatusGet(UART_IDX_DBG, UART_STATUS_TX_EMPTY) == RESET);
    Hal_Sys_SwResetAll();
}

void IPC0_IRQHandler_Entry_impl(void)
{
    // Clear interrupt
    Hal_Vic_IpcIntClear(IPC_IDX_0);
	
    //IPC0 bit0
    #ifdef IPC_SUT
    ipc_peer_ready(1, 1);
    #endif
}

void IPC1_IRQHandler_Entry_impl(void)
{
    // Clear interrupt
    Hal_Vic_IpcIntClear(IPC_IDX_1);
	
    //IPC1 bit1
    #ifdef IPC_SUT
    ipc_peer_ready(1, 0);
    #endif
}

void IPC2_IRQHandler_Entry_impl(void)
{
    // Clear interrupt
    Hal_Vic_IpcIntClear(IPC_IDX_2);

    //IPC1 bit2
    ipc_proc();
}

void IPC3_IRQHandler_Entry_impl(void)
{
    // Clear interrupt
    Hal_Vic_IpcIntClear(IPC_IDX_3);

    ps_lock_gpio_apply();
}

void GPIO_IRQHandler_Entry_impl(void)
{
    E_GpioIdx_t eGpioIdx = GPIO_IDX_00;
    uint64_t u64Status = 0;

    ps_update_io_time();

    // Get status
    u64Status = Hal_Vic_GpioIntStatRead();

    if( Hal_Vic_MmFactorResumeByGpioIntStatRead() & u64Status )
    {
        // Update regsiter value. Hardware clock recovery automatically.
        Hal_Sys_ApsClkMmFactorSet(APS_MMFACTOR_DEFAULT);
    }
    
    for(eGpioIdx = GPIO_IDX_00; eGpioIdx < g_u32Hal_IoPkgNum; eGpioIdx++)
    {
        if( u64Status & (1LL << eGpioIdx) )
        {
            // Hook here...
            if(g_taHalVicGpioCallBack[eGpioIdx] != 0)
                g_taHalVicGpioCallBack[eGpioIdx](eGpioIdx);
            
            // Clear module interrupt
            Hal_Vic_GpioIntClear(eGpioIdx);
        }
    }

    // Clear VIC interrupt
    Hal_Vic_IntClear(GPIO_IRQn);
}

void SCRT_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void MSQ_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void UARTDBG_IRQHandler_Entry_impl(void)
{
    // Note: Level-sensitive interrupt
    uint32_t u32Temp = 0;
    uint32_t u32InterruptId = 0;

    // Get status
    u32InterruptId = Hal_Uart_IntIdRead(UART_IDX_DBG);

    // Clear module interrupt 
    Hal_Uart_IntClear(UART_IDX_DBG, (E_UartIntId_t)u32InterruptId, &u32Temp);

    // Clear VIC interrupt
    Hal_Vic_IntClear(DBG_UART_IRQn);

    if((u32InterruptId == UART_INT_RX_AVALIBLE) || (u32InterruptId == UART_INT_CHAR_TIMEOUT))
    {
        // ---- Rx interrupt case ----
        // Hook here ...
        if( DbgUart_RxCallBack != 0 )
            DbgUart_RxCallBack(u32Temp);
    }
}

void UART0_IRQHandler_Entry_impl(void)
{
    // Note: Level-sensitive interrupt
    uint32_t u32Temp = 0;
    uint32_t u32InterruptId = 0;

    // Get status
    u32InterruptId = Hal_Uart_IntIdRead(UART_IDX_0);

    // Clear module interrupt 
    Hal_Uart_IntClear(UART_IDX_0, (E_UartIntId_t)u32InterruptId, &u32Temp);

    // Clear VIC interrupt
    Hal_Vic_IntClear(UART0_IRQn);

    if((u32InterruptId == UART_INT_RX_AVALIBLE) || (u32InterruptId == UART_INT_CHAR_TIMEOUT))
    {
        // ---- Rx interrupt case ----
        // Hook here ...
        if( Uart_0_RxCallBack != 0 )
            Uart_0_RxCallBack(u32Temp);
    }
}

void UART1_IRQHandler_Entry_impl(void)
{
    // Note: Level-sensitive interrupt
    uint32_t u32Temp = 0;
    uint32_t u32InterruptId = 0;

    // Get status
    u32InterruptId = Hal_Uart_IntIdRead(UART_IDX_1);

    // Clear module interrupt 
    Hal_Uart_IntClear(UART_IDX_1, (E_UartIntId_t)u32InterruptId, &u32Temp);

    // Clear VIC interrupt
    Hal_Vic_IntClear(UART1_IRQn);

    if((u32InterruptId == UART_INT_RX_AVALIBLE) || (u32InterruptId == UART_INT_CHAR_TIMEOUT))
    {
        // ---- Rx interrupt case ----
        // Hook here ...
        if( Uart_1_RxCallBack != 0 )
            Uart_1_RxCallBack(u32Temp);
    }
}

void I2C_IRQHandler_Entry_impl(void)
{
    // handle the I2C interrupt
    Hal_I2c_IntHandler();
    
    // Clear VIC interrupt
    Hal_Vic_IntClear(I2C_IRQn);
}

void SPI0_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void SPI1_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void SPI2_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void TIM0_IRQHandler_Entry_impl(void)
{
    // Clear module interrupt
    Hal_Tmr_IntClear(TMR_IDX_0);
    
    // Clear VIC part
    Hal_Vic_IntClear(TMR0_IRQn);
    
    // handle the timer 0 interrupt
    if(Tmr_CallBack[TMR_IDX_0] != NULL)
        Tmr_CallBack[TMR_IDX_0](TMR_IDX_0);
}

void TIM1_IRQHandler_Entry_impl(void)
{
    // Clear module interrupt
    Hal_Tmr_IntClear(TMR_IDX_1);
    
    // Clear VIC part
    Hal_Vic_IntClear(TMR1_IRQn);
    
    // handle the timer 1 interrupt
    if(Tmr_CallBack[TMR_IDX_1] != NULL)
        Tmr_CallBack[TMR_IDX_1](TMR_IDX_1);
}

void WDT_IRQHandler_Entry_impl(void)
{
    // Clear module interrupt
    Hal_Wdt_Clear();

    // Clear VIC part
    Hal_Vic_IntClear(WDT_IRQn);

    // handle the watchdog interrupt
    if(Wdt_CallBack != NULL)
        Wdt_CallBack();
}

void JTAG_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void APS_DMA_IRQHandler_Entry_impl(void)
{
    E_DMA_CHANNEL eChannel = DMA_Channel_Max;
    uint32_t sts_int = DMA->STS_INT;
    uint32_t sts = 0;
    
    if (sts_int & DMA_STS_INT_TFR_Msk)
        sts = DMA->STS_TFR;
    else if (sts_int & DMA_STS_INT_BLK_Msk)
        sts = DMA->STS_BLK;
    else if (sts_int & DMA_STS_INT_SRCT_Msk)
        sts = DMA->STS_SRC_TRAN;
    else if (sts_int & DMA_STS_INT_DSTT_Msk)
        sts = DMA->STS_DST_TRAN;
    else if (sts_int & DMA_STS_INT_ERR_Msk)
    {
        // TODO
    }
    
    if (sts & DMA_BMP_INT_CH0)
        eChannel = DMA_Channel_0;
    else if (sts & DMA_BMP_INT_CH1)
        eChannel = DMA_Channel_1;
    else if (sts & DMA_BMP_INT_CH2)
        eChannel = DMA_Channel_2;
    else if (sts & DMA_BMP_INT_CH3)
        eChannel = DMA_Channel_3;
    
    if (g_tHalDmaCallBack[eChannel] != NULL)
    {
        g_tHalDmaCallBack[eChannel]();
    }
    else
    {
        // Clear module interrupt
        Hal_Dma_IntClearAll();
        
        // Clear VIC part
        Hal_Vic_IntClear(APS_DMA_IRQn);
    }
}

void I2S_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void SPI3_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void MSQ_DMA_IRQHandler_Entry_impl(void)
{
    if (g_tHalMsqDmaCallBack != NULL)
    {
        g_tHalMsqDmaCallBack();
    }
    else
    {
        // Clear module interrupt
        Hal_Msq_Dma_IntClearAll();
        
        // Clear VIC part
        Hal_Vic_IntClear(MSQ_DMA_IRQn);
    }
}

void PDM_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

void AUX_ADC_IRQHandler_Entry_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

// --------------------------------- ISR ---------------------------------
void HardFault_Handler(void)
{
    g_sExceptionInfo.eExceptionType = EXCEPTION_HARD_FAULT;
    g_sExceptionInfo.u32Lr = __get_LR();
    g_sExceptionInfo.u32Msp = __get_MSP();
    g_sExceptionInfo.u32Psp = __get_PSP();
    
    HardFault_Handler_Entry();
}

void IPC0_IRQHandler(void)
{
    IPC0_IRQHandler_Entry();
}

void IPC1_IRQHandler(void)
{
    IPC1_IRQHandler_Entry();
}

void IPC2_IRQHandler(void)
{
    IPC2_IRQHandler_Entry();
}

void IPC3_IRQHandler(void)
{
    IPC3_IRQHandler_Entry();
}

void GPIO_IRQHandler(void)
{
    GPIO_IRQHandler_Entry();
}  

void SCRT_IRQHandler(void)
{
    SCRT_IRQHandler_Entry();
}  

void MSQ_IRQHandler(void)
{
    MSQ_IRQHandler_Entry();
}

void UARTDBG_IRQHandler(void)
{
    UARTDBG_IRQHandler_Entry();
}

void UART0_IRQHandler(void)
{
    UART0_IRQHandler_Entry();
}

void UART1_IRQHandler(void)
{
    UART1_IRQHandler_Entry();
}

void I2C_IRQHandler(void)
{
    I2C_IRQHandler_Entry();
}

void SPI0_IRQHandler(void)
{
    SPI0_IRQHandler_Entry();
}

void SPI1_IRQHandler(void)
{
    SPI1_IRQHandler_Entry();
}

void SPI2_IRQHandler(void)
{
    SPI2_IRQHandler_Entry();
}

void TIM0_IRQHandler(void)
{
    TIM0_IRQHandler_Entry();
}

void TIM1_IRQHandler(void)
{
    TIM1_IRQHandler_Entry();
}

void WDT_IRQHandler(void)
{
    g_sExceptionInfo.eExceptionType = EXCEPTION_WDT;
    g_sExceptionInfo.u32Lr = __get_LR();
    g_sExceptionInfo.u32Msp = __get_PSP();
    g_sExceptionInfo.u32Psp = __get_MSP();
    WDT_IRQHandler_Entry();
}

void JTAG_IRQHandler(void)
{
    JTAG_IRQHandler_Entry();
}

void APS_DMA_IRQHandler(void)
{
    APS_DMA_IRQHandler_Entry();
}

void I2S_IRQHandler(void)
{
    I2S_IRQHandler_Entry();
}

void SPI3_IRQHandler(void)
{
    SPI3_IRQHandler_Entry();
}

void MSQ_DMA_IRQHandler(void)
{
    MSQ_DMA_IRQHandler_Entry();
}


void DET_XTAL_IRQHandler(void)
{
    Hal_Vic_IntClear(DET_XTAL_IRQn);
    /* TODO */
}

void PDM_IRQHandler(void)
{
    PDM_IRQHandler_Entry();
}

void AUX_ADC_IRQHandler(void)
{
    AUX_ADC_IRQHandler_Entry();
}

