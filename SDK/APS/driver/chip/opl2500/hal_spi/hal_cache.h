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

/******************************************************************************
 *  @file hal_cache.h
 *  @brief API to control XIP Cache
 ******************************************************************************/

#ifndef __HAL_CACHE_H__
#define __HAL_CACHE_H__

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>

/*
 *************************************************************************
 *						    Definitions and Macros
 *************************************************************************
 */
#define XIP_CACHE_TIMEOUT_MS        500 // 500 ms

/*
 *************************************************************************
 *						    Enums and Structures
 *************************************************************************
 */
typedef enum
{
    eCache    = 0, 
    ePrefetch = 1,
    eHsel     = 2,
} E_XipCacheCntType_t;

#if 1
// debug usage
// add ((S_XIP_CACHE_Debug_t *) 0x3000E654) to Watch Windows
#pragma push
#pragma anon_unions
typedef struct
{
    uint32_t pref_cache_en:1;       // 0x654
    uint32_t prefetch_bypass:1;
    uint32_t cache_count_en:1;
    uint32_t cache_count_freeze:1;
    uint32_t pref_count_en:1;
    uint32_t pref_count_freeze:1;
    uint32_t hsel_count_en:1;
    uint32_t hsel_count_freeze:1;
    uint32_t pref_pipe_en:1;
    uint32_t rsvd01:3;
    uint32_t cache_mode:1;          
    uint32_t rsvd02:18;
    uint32_t cache_initial_done:1;
    uint32_t cache_hit;             // 0x658
    uint32_t cache_miss;            // 0x65C 
    uint32_t pref_hit;              // 0x660
    uint32_t pref_miss;             // 0x664
    uint32_t hsel_count;            // 0x668
    uint32_t hsel_wait_count;       // 0x66C
} S_XIP_CACHE_Debug_t;
#pragma pop
#endif 

/*
 *************************************************************************
 *						    Typedefs
 *************************************************************************
 */
typedef void (*T_Hal_Cache_ClockEnable_fp)(void);
typedef void (*T_Hal_Cache_Reset_fp)(void);
typedef void (*T_Hal_Cache_Enable_fp)(uint8_t u8Enable);
typedef void (*T_Hal_Cache_PrefetchBypass_fp)(uint8_t u8Enable);
typedef void (*T_Hal_Cache_PipelineEnable_fp)(uint8_t u8Enable);
typedef void (*T_Hal_Cache_SizeShrink_fp)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_Cache_ReadyPoll_fp)(void);
typedef void (*T_Hal_Cache_CounterEnable_fp)(E_XipCacheCntType_t eType, uint8_t u8Enable);
typedef void (*T_Hal_Cache_CounterFreeze_fp)(E_XipCacheCntType_t eType, uint8_t u8Enable);
typedef void (*T_Hal_Cache_CounterReset_fp)(E_XipCacheCntType_t eType);
typedef uint32_t (*T_Hal_Cache_CounterGet_fp)(E_XipCacheCntType_t eType, uint32_t *pu32Cnt_1, uint32_t *pu32Cnt_2);
typedef void (*T_Hal_Cache_CounterResetAll_fp)(void);

/*
 *************************************************************************
 *							Public Variables
 *************************************************************************
 */
extern T_Hal_Cache_ClockEnable_fp        Hal_Cache_ClockEnable;
extern T_Hal_Cache_Reset_fp              Hal_Cache_Reset;
extern T_Hal_Cache_Enable_fp             Hal_Cache_Enable;
extern T_Hal_Cache_PrefetchBypass_fp     Hal_Cache_PrefetchBypass;
extern T_Hal_Cache_PipelineEnable_fp     Hal_Cache_PipelineEnable;
extern T_Hal_Cache_SizeShrink_fp         Hal_Cache_SizeShrink;
extern T_Hal_Cache_ReadyPoll_fp          Hal_Cache_ReadyPoll;
extern T_Hal_Cache_CounterEnable_fp      Hal_Cache_CounterEnable;
extern T_Hal_Cache_CounterFreeze_fp      Hal_Cache_CounterFreeze;
extern T_Hal_Cache_CounterReset_fp       Hal_Cache_CounterReset;
extern T_Hal_Cache_CounterGet_fp         Hal_Cache_CounterGet;
extern T_Hal_Cache_CounterResetAll_fp    Hal_Cache_CounterResetAll;

/*
 *************************************************************************
 *							Public Functions
 *************************************************************************
 */
void Hal_Cache_Pre_Init(void);

#include "hal_cache_patch.h"
#endif /* __HAL_CACHE_H__ */
