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

void Hal_Sys_WakeupResume_patch(void);
void Hal_Sys_SleepPrepare_patch(void);
uint32_t Hal_Sys_RetRamCtrlBySeq_patch(uint32_t u32RetRamIdxs);
uint32_t Hal_Sys_RetRamTurnOff_patch(uint32_t u32RetRamIdxs);
uint32_t Hal_Sys_RetRamTurnOn_patch(uint32_t u32RetRamIdxs);
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
extern uint32_t g_u32SramUsageBmp;




 
 
 


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Sys_PatchInit(void)
{
    Hal_Sys_WakeupResume = Hal_Sys_WakeupResume_patch;
    Hal_Sys_SleepPrepare = Hal_Sys_SleepPrepare_patch;
    Hal_Sys_RetRamCtrlBySeq = Hal_Sys_RetRamCtrlBySeq_patch;
    Hal_Sys_RetRamTurnOn = Hal_Sys_RetRamTurnOn_patch;
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

void Hal_Sys_WakeupResume_patch(void)
{
    Hal_Sys_RetRamTurnOn(RET_RAM_SCRT);
    Hal_Vic_WakeupResume();
}

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
    
    Hal_Sys_RetRamTurnOff(RET_RAM_SCRT);
}

uint32_t Hal_Sys_RetRamCtrlBySeq_patch(uint32_t u32RetRamIdxs)
{
    uint32_t u32Value;
    if( u32RetRamIdxs & (~RET_RAM_BIT_Msk) )
        return 1;
    u32Value = AOS->SRAM_PSD;
    u32Value &= ~(u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos);   /* Set SEL = 0 */
    AOS->SRAM_PSD = u32Value;
    return 0;
}

uint32_t Hal_Sys_RetRamTurnOff_patch(uint32_t u32RetRamIdxs)
{
    uint32_t u32Value;
    // Check Indexes are all in legal region
    if( u32RetRamIdxs & (~RET_RAM_BIT_Msk) )
        return 1;
    
    /* Turn off: ISO -> PSD */
    
    /* Isolation */
    u32Value = AOS->SRAM_ISO_EN;
    u32Value |= u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos;      /* Set SEL = 1 */
    u32Value |= u32RetRamIdxs << RET_RAM_FW_CTRL_BIT_Pos;       /* Set CTRL = 1 */
    AOS->SRAM_ISO_EN = u32Value;
    
    /* PSD */
    u32Value = AOS->SRAM_PSD;
    u32Value |= u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos;      /* Set SEL = 1 */
    u32Value &= ~(u32RetRamIdxs << RET_RAM_FW_CTRL_BIT_Pos);    /* Set CTRL = 0 */
    AOS->SRAM_PSD = u32Value;

    return 0;
}
uint32_t Hal_Sys_RetRamTurnOn_patch(uint32_t u32RetRamIdxs)
{
    uint32_t u32Value;
    // Check Indexes are all in legal region
    if( u32RetRamIdxs & (~RET_RAM_BIT_Msk) )
        return 1;
    
    /* Turn on: PSD -> ISO */
    /* PSD */
    u32Value = AOS->SRAM_PSD;
    u32Value |= u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos;      /* Set SEL = 1 */
    u32Value |= u32RetRamIdxs << RET_RAM_FW_CTRL_BIT_Pos;       /* Set CTRL = 1 */
    AOS->SRAM_PSD = u32Value;
    
    /* Isolation */
    u32Value = AOS->SRAM_ISO_EN;
    u32Value &= ~(u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos);   /* Set SEL = 0 */
    u32Value &= ~(u32RetRamIdxs << RET_RAM_FW_CTRL_BIT_Pos);    /* Set CTRL = 0 */
    AOS->SRAM_ISO_EN = u32Value;
    
    return 0;
}
