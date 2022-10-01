/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/
/**
 ****************************************************************************** 
 * @file system_opl2500.c
 * @author Opulinks Software Team
 * @version V0.0.1
 * @date 14-September-2018
 * @brief System clock function source
 */
#include "opulinks.h"
#include "hal_system.h"
#include "hal_sys_rcc.h"
#include "hal_tick.h"


/*----------------------------------------------------------------------------
  Externals
 *----------------------------------------------------------------------------*/
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
  extern uint32_t __Vectors;
#endif

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = CLK_SRC_FREQ_RC;

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{

}

uint32_t SystemCoreClockGet (void)
{
    return SystemCoreClock;
}

void SystemCoreClockSet (uint32_t clkHz)
{
    SystemCoreClock = clkHz;
}


/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit (void)
{
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
    SCB->VTOR = (uint32_t) &__Vectors;
#endif

#if defined (__FPU_USED) && (__FPU_USED == 1U)
    SCB->CPACR |= ((3U << 10U*2U) |           /* set CP10 Full Access */
                   (3U << 11U*2U)  );         /* set CP11 Full Access */
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

    SystemCoreClock = CLK_SRC_FREQ_RC;
    
    Hal_Sys_32kXtalSetup();
    
    Hal_Tick_Init();
}
