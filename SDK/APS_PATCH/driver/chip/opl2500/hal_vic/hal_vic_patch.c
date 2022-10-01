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
 *  @file hal_vic_patch.c
 * 
 *  @brief VIC patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_vic.h"

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
void Hal_Vic_SleepStore_patch(void);
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
extern S_NVIC_STORAGE g_Hal_VicStorage;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Vic_PatchInit(void)
{
    Hal_Vic_SleepStore = Hal_Vic_SleepStore_patch;
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

/*************************************************************************
* FUNCTION:
*  Hal_Vic_SleepStore
*
* DESCRIPTION:
*   1. Store NVIC setting before entering sleep
* CALLS
*
* PARAMETERS
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_SleepStore_patch(void)
{
    Hal_Vic_SleepStore_impl();
    g_Hal_VicStorage.u32IrqEn |= 1 << GPIO_IRQn;  /* GPIO IRQ might be turned off to enter sleep */
}
