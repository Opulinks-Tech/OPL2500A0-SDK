/******************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 ******************************************************************************
 * @file hal_cache.c
 * 
 * @brief API to control XIP Cache for APS
 * 
 */

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "opulinks.h"
#include "hal_sys_rcc.h"
#include "hal_cache.h"
#include "hal_tick.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/*
 *************************************************************************
 *						    Enums and Structures
 *************************************************************************
 */

/*
 *************************************************************************
 *						    Typedefs
 *************************************************************************
 */

/*
 *************************************************************************
 *                           Declarations of Private Functions
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
T_Hal_Cache_ClockEnable_fp        Hal_Cache_ClockEnable;
T_Hal_Cache_Reset_fp              Hal_Cache_Reset;
T_Hal_Cache_Enable_fp             Hal_Cache_Enable;
T_Hal_Cache_PrefetchBypass_fp     Hal_Cache_PrefetchBypass;
T_Hal_Cache_PipelineEnable_fp     Hal_Cache_PipelineEnable;
T_Hal_Cache_SizeShrink_fp         Hal_Cache_SizeShrink;
T_Hal_Cache_ReadyPoll_fp          Hal_Cache_ReadyPoll;
T_Hal_Cache_CounterEnable_fp      Hal_Cache_CounterEnable;
T_Hal_Cache_CounterFreeze_fp      Hal_Cache_CounterFreeze;
T_Hal_Cache_CounterReset_fp       Hal_Cache_CounterReset;
T_Hal_Cache_CounterGet_fp         Hal_Cache_CounterGet;
T_Hal_Cache_CounterResetAll_fp    Hal_Cache_CounterResetAll;

uint32_t g_u32XipCacheTimeoutMs = XIP_CACHE_TIMEOUT_MS;

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */

/*
 *************************************************************************
 *                           Public Functions
 *************************************************************************
 */

/*************************************************************************
* FUNCTION:
*  Hal_Cache_ClockEnable
*
* DESCRIPTION:
*   1. Enable clock of XIP Cache
*   
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_ClockEnable_impl(void)
{
    Hal_Sys_ApsModuleRst(APS_RST_XIP_CACHE);
    Hal_Sys_ApsModuleRst(APS_RST_XIP_PREFETCH);
    
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_XIP_CACHE);
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_XIP_PREFETCH);
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_Reset
*
* DESCRIPTION:
*   1. Reset & Disable XIP Cache
*   
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_Reset_impl(void)
{
    SYS->XIP_CACHE_CONTROL = 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_Enable
*
* DESCRIPTION:
*   1. Enable XIP Cache
*   
* CALLS
*
* PARAMETERS
*   1. u8Enable : 1 = Enable
*                 0 = Disable
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_Enable_impl(uint8_t u8Enable)
{
    if (u8Enable)
    {
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL | SYS_XIP_CACHE_CONTROL_XIP_PREF_CACHE_EN_Msk;
        if (Hal_Cache_ReadyPoll()) 
            return;
    }
    else
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL & ~(SYS_XIP_CACHE_CONTROL_XIP_PREF_CACHE_EN_Msk);
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_PipelineEnable
*
* DESCRIPTION:
*   1. Enable pipeline of prefetch for higher clock frequency
*   
* CALLS
*
* PARAMETERS
*   1. u8Enable : 1 = Enable
*                 0 = Disable
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_PrefetchBypass_impl(uint8_t u8Enable)
{
    if (u8Enable)
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL | SYS_XIP_CACHE_CONTROL_XIP_PREFETCH_BYPASS_Msk;
    else
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL & ~(SYS_XIP_CACHE_CONTROL_XIP_PREFETCH_BYPASS_Msk);
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_PipelineEnable
*
* DESCRIPTION:
*   1. enable pipeline of prefetch for higher clock frequency
*   
* CALLS
*
* PARAMETERS
*   1. u8Enable : 1 = Enable
*                 0 = Disable
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_PipelineEnable_impl(uint8_t u8Enable)
{
    if (u8Enable)
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL | SYS_XIP_CACHE_CONTROL_XIP_PREF_OUT_PIPE_EN_Msk;
    else
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL & ~(SYS_XIP_CACHE_CONTROL_XIP_PREF_OUT_PIPE_EN_Msk);
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_SizeShrink
*
* DESCRIPTION:
*   1. Reduce Cache size to get more RAM space
*   
* CALLS
*
* PARAMETERS
*   1. u8Enable : 1 = Enable, Cache Size = 4K words
*                 0 = Disable, Cache Size = 8K words
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_SizeShrink_impl(uint8_t u8Enable)
{
    if (u8Enable)
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL | SYS_XIP_CACHE_CONTROL_XIP_CACHE_MODE_Msk;
    else
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL & ~(SYS_XIP_CACHE_CONTROL_XIP_CACHE_MODE_Msk);
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_ReadyPoll
*
* DESCRIPTION:
*   1. Poll for XIP Cache ready status
*   
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   1. 0 : success
*   2. 1 : fail, timeout
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Cache_ReadyPoll_impl(void)
{
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32XipCacheTimeoutMs;
    u32TickStart = Hal_Tick_Diff(0);
    
    while (!(SYS->XIP_CACHE_CONTROL & SYS_XIP_CACHE_CONTROL_XIP_CACHE_INITIAL_DONE_Msk))
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_CounterEnable
*
* DESCRIPTION:
*   1. Enable counters of XIP Cache
*   
* CALLS
*
* PARAMETERS
*   1. eType : type of counter, refer to E_XipCacheCntType_t.
*   2. u8Enable : 1 = Enable
*                 0 = Disable
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_CounterEnable_impl(E_XipCacheCntType_t eType, uint8_t u8Enable)
{
    uint32_t u32Mask = 0;

    switch (eType)
    {
        case eCache:       u32Mask = SYS_XIP_CACHE_CONTROL_XIP_CACHE_HIT_MISS_COUNT_EN_Msk;    break;
        case ePrefetch:    u32Mask = SYS_XIP_CACHE_CONTROL_XIP_PREF_HIT_MISS_COUNT_EN_Msk;     break;
        case eHsel:        u32Mask = SYS_XIP_CACHE_CONTROL_XIP_CACHE_HSEL_COUNT_EN_Msk;        break;
    }
    
    if (u8Enable)
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL | u32Mask;
    else
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL & ~(u32Mask);
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_CounterFreeze
*
* DESCRIPTION:
*   1. Freeze counters of XIP Cache
*   
* CALLS
*
* PARAMETERS
*   1. eType : type of counter, refer to E_XipCacheCntType_t.
*   2. u8Enable : 1 = Enable
*                 0 = Disable
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_CounterFreeze_impl(E_XipCacheCntType_t eType, uint8_t u8Enable)
{
    uint32_t u32Mask = 0;

    switch (eType)
    {
        case eCache:    u32Mask = SYS_XIP_CACHE_CONTROL_XIP_CACHE_COUNT_FREEZE_Msk;      break;
        case ePrefetch: u32Mask = SYS_XIP_CACHE_CONTROL_XIP_PREF_COUNT_FREEZE_Msk;       break;
        case eHsel:     u32Mask = SYS_XIP_CACHE_CONTROL_XIP_CACHE_HSEL_COUNT_FREEZE_Msk; break;
    }
    
    if (u8Enable)
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL | u32Mask;
    else
        SYS->XIP_CACHE_CONTROL = SYS->XIP_CACHE_CONTROL & ~(u32Mask);
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_CounterReset
*
* DESCRIPTION:
*   1. Reset counters of XIP Cache
*
* CALLS
*
* PARAMETERS
*   1. eType : type of counter, refer to E_XipCacheCntType_t.
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_CounterReset_impl(E_XipCacheCntType_t eType)
{
    Hal_Cache_CounterEnable(eType, 0);
    Hal_Cache_CounterEnable(eType, 1);
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_CounterGet
*
* DESCRIPTION:
*   1. Get counters of XIP Cache
*
* CALLS
*
* PARAMETERS
*   1. eType : type of counter, refer to E_XipCacheCntType_t.
*   2. *pu32Cnt_1 : 1st counter (hit count for eCache/ePrefetch, hsel count for eHsel)
*   3. *pu32Cnt_2 : 2nd counter (miss count for eCache/ePrefetch, hsel wait count for eHsel)
*
* RETURNS
*   1. 0 : success
*   2. 1 : fail 
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Cache_CounterGet_impl(E_XipCacheCntType_t eType, uint32_t *pu32Cnt_1, uint32_t *pu32Cnt_2)
{
    if ( (pu32Cnt_1 == NULL) || (pu32Cnt_2 == NULL) )
        return 1;
    
    switch (eType)
    {
        case eCache:
            *pu32Cnt_1 = SYS->XIP_CACHE_HIT;
            *pu32Cnt_2 = SYS->XIP_CACHE_MISS;
            break;
        case ePrefetch:
            *pu32Cnt_1 = SYS->XIP_PREFETCH_HIT;
            *pu32Cnt_2 = SYS->XIP_PREFETCH_MISS;
            break;
        case eHsel:
            *pu32Cnt_1 = SYS->XIP_CACHE_HSEL_COUNT;
            *pu32Cnt_2 = SYS->XIP_CACHE_HSEL_WAIT_COUNT;
            break;
        default:
            return 1;
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_CounterResetAll
*
* DESCRIPTION:
*   1. Reset all counters of XIP Cache
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_CounterResetAll_impl(void)
{
    Hal_Cache_CounterReset(eCache);
    Hal_Cache_CounterReset(ePrefetch);
    Hal_Cache_CounterReset(eHsel);
}

/*************************************************************************
* FUNCTION:
*  Hal_Cache_Pre_Init
*
* DESCRIPTION:
*   1. Inital of function pointers
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Cache_Pre_Init(void)
{
    Hal_Cache_ClockEnable     = Hal_Cache_ClockEnable_impl;
    Hal_Cache_Reset           = Hal_Cache_Reset_impl;
    Hal_Cache_Enable          = Hal_Cache_Enable_impl;
    Hal_Cache_PrefetchBypass  = Hal_Cache_PrefetchBypass_impl;
    Hal_Cache_PipelineEnable  = Hal_Cache_PipelineEnable_impl;
    Hal_Cache_SizeShrink      = Hal_Cache_SizeShrink_impl;
    Hal_Cache_ReadyPoll       = Hal_Cache_ReadyPoll_impl;
    Hal_Cache_CounterEnable   = Hal_Cache_CounterEnable_impl;
    Hal_Cache_CounterFreeze   = Hal_Cache_CounterFreeze_impl;
    Hal_Cache_CounterReset    = Hal_Cache_CounterReset_impl;
    Hal_Cache_CounterGet      = Hal_Cache_CounterGet_impl;
    Hal_Cache_CounterResetAll = Hal_Cache_CounterResetAll_impl;
}

/*
 *************************************************************************
 *                           Private Functions
 *************************************************************************
 */

