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
 * @file system_opl2500.h
 * @author Opulinks Software Team
 * @version V0.0.1
 * @date 14-September-2018
 * @brief System clock function header
 */

#ifndef __SYSTEM_OPL2500_H
#define __SYSTEM_OPL2500_H

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock) */


/**
  \brief Setup the microcontroller system.

   Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);


/**
  \brief  Update SystemCoreClock variable.

   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);
extern uint32_t SystemCoreClockGet (void);
extern void SystemCoreClockSet (uint32_t clkHz);
extern void SystemCoreClockDivFactorSet (uint32_t div);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_OPL2500_H */
