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
#ifndef _IRQ_H_
#define _IRQ_H_

#include <stdint.h>
typedef enum
{
    EXCEPTION_WDT=0,
    EXCEPTION_HARD_FAULT,
} E_EXCEPTION_TYPE;

typedef struct 
{
    uint32_t u32Lr;
    uint32_t u32Psp;
    uint32_t u32Msp;
    E_EXCEPTION_TYPE eExceptionType;
} S_EXCEPTION_INFO;

// --------------------------------- ISR Entry---------------------------------
void HardFault_Handler_Entry_impl(void);
void IPC0_IRQHandler_Entry_impl(void);
void IPC1_IRQHandler_Entry_impl(void);
void IPC2_IRQHandler_Entry_impl(void);
void IPC3_IRQHandler_Entry_impl(void);
void GPIO_IRQHandler_Entry_impl(void);
void SCRT_IRQHandler_Entry_impl(void);
void MSQ_IRQHandler_Entry_impl(void);
void UARTDBG_IRQHandler_Entry_impl(void);
void UART0_IRQHandler_Entry_impl(void);
void UART1_IRQHandler_Entry_impl(void);
void I2C_IRQHandler_Entry_impl(void);
void SPI0_IRQHandler_Entry_impl(void);
void SPI1_IRQHandler_Entry_impl(void);
void SPI2_IRQHandler_Entry_impl(void);
void TIM0_IRQHandler_Entry_impl(void);
void TIM1_IRQHandler_Entry_impl(void);
void WDT_IRQHandler_Entry_impl(void);
void JTAG_IRQHandler_Entry_impl(void);
void APS_DMA_IRQHandler_Entry_impl(void);
void I2S_IRQHandler_Entry_impl(void);
void SPI3_IRQHandler_Entry_impl(void);
void MSQ_DMA_IRQHandler_Entry_impl(void);
void PDM_IRQHandler_Entry_impl(void);
void AUX_ADC_IRQHandler_Entry_impl(void);
    
    
typedef void (*T_InterruptHandler)(void);

extern T_InterruptHandler HardFault_Handler_Entry;
extern T_InterruptHandler IPC0_IRQHandler_Entry;
extern T_InterruptHandler IPC1_IRQHandler_Entry;
extern T_InterruptHandler IPC2_IRQHandler_Entry;
extern T_InterruptHandler IPC3_IRQHandler_Entry;
extern T_InterruptHandler GPIO_IRQHandler_Entry;
extern T_InterruptHandler SCRT_IRQHandler_Entry;
extern T_InterruptHandler MSQ_IRQHandler_Entry;
extern T_InterruptHandler UARTDBG_IRQHandler_Entry;
extern T_InterruptHandler UART0_IRQHandler_Entry;
extern T_InterruptHandler UART1_IRQHandler_Entry;
extern T_InterruptHandler I2C_IRQHandler_Entry;
extern T_InterruptHandler SPI0_IRQHandler_Entry;
extern T_InterruptHandler SPI1_IRQHandler_Entry;
extern T_InterruptHandler SPI2_IRQHandler_Entry;
extern T_InterruptHandler TIM0_IRQHandler_Entry;
extern T_InterruptHandler TIM1_IRQHandler_Entry;
extern T_InterruptHandler WDT_IRQHandler_Entry;
extern T_InterruptHandler JTAG_IRQHandler_Entry;
extern T_InterruptHandler APS_DMA_IRQHandler_Entry;
extern T_InterruptHandler I2S_IRQHandler_Entry;
extern T_InterruptHandler SPI3_IRQHandler_Entry;
extern T_InterruptHandler MSQ_DMA_IRQHandler_Entry;
extern T_InterruptHandler PDM_IRQHandler_Entry;
extern T_InterruptHandler AUX_ADC_IRQHandler_Entry;



extern S_EXCEPTION_INFO g_sExceptionInfo; 
#include "irq_patch.h"
#endif /* _IRQ_H_ */
