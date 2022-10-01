/* *****************************************************************************
 *  Copyright 2017 - 2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file hal_system_patch.c
 * 
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "hal_system.h"
#include "svn_rev_patch.h"
#include "ipc.h"
#include "hal_vic.h"
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
void Hal_Sys_SleepPrepare_patch(void);
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

void Hal_Sys_PatchInit(void)
{
    Hal_Sys_SleepPrepare = Hal_Sys_SleepPrepare_patch;
}


void Hal_Sys_PatchVerInit(uint32_t u32VerInfoAddr)
{
    S_SW_VERSION_INFO *pVerInfo = (S_SW_VERSION_INFO *)u32VerInfoAddr;
    char *szBuildTime = SVN_TIME_NOW_PATCH;
    
    pVerInfo->sApsPatch.u32Revision = SVN_REVISION_PATCH;
    strcpy(pVerInfo->sApsPatch.szBuildTime, szBuildTime);
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

void Hal_Sys_SleepPrepare_patch(void)
{
    Hal_Sys_PtatEn(0);          // disable HPBG
    AOS->RSTS = 0xFFFFFFFF;     // clear all boot status
    
    //HAL_SYS_FAST_STARTUP_EN(1); 
    /* Don't enable fast start here.
     * It will affect XTAL clock.
     * MSQ will ENABLE it after swtich to RC */
    
    Hal_Sys_SleepClkSetup();
    Hal_Vic_SleepStore();
    Hal_Uart_SleepPrepare();
}

