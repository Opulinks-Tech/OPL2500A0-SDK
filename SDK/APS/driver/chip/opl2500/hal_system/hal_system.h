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
*  Filename:
*  ---------
*  hal_system.h
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of system functions
*  Include sys-reg and AOS domains.
*
*  Author:
*  -------
*  Chung-chun Wang
******************************************************************************/

#ifndef __HAL_SYSTEM_H__
#define __HAL_SYSTEM_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>
#include <stddef.h>
#include "opulinks.h"
#include "hal_sys_rcc.h"
// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/* Spare register usage 
 * [AOS]
 * SPARE0@0x10C: IPC sync usage
 * SPARE1@0x124: HW use
 * SPARE2@0x12C: For further use
 * SPARE3@0x130: For further use
 * [SYS]
 * SPARE @0x264: XTAL frequency
 * SPARE1@0x268: APS boot debug tag
 * SPARE2@0x26C: MSQ boot debug tag
 * SPARE3@0x270:
 * SPARE4@0x274:
 * SPARE5@0x278:
 * SPARE6@0x27C:
 * SPARE7@0x280:
 * SPARE8@0x284:
 */

#define SYSTEM_SPARE_REG_XTAL_FREQ                  (SYS->SPARE)
#define SYSTEM_SPARE_REG_APS_BOOT_DEBUG_TAG         (SYS->SPARE1)
#define SYSTEM_SPARE_REG_MSQ_BOOT_DEBUG_TAG         (SYS->SPARE2)


#define HAL_SYS_GET_XTAL_FREQ()                     SYSTEM_SPARE_REG_XTAL_FREQ
#define HAL_SYS_SET_XTAL_FREQ(freq)                 (SYSTEM_SPARE_REG_XTAL_FREQ = (freq))
#define HAL_SYS_SET_BOOT_DEBUG_TAG(tag)             (SYSTEM_SPARE_REG_APS_BOOT_DEBUG_TAG = (tag))


#define RET_RAM_FW_CTRL_BIT_Pos                     (0U)
#define RET_RAM_FW_CTRL_BIT_Msk                     (0x00FFU)
#define RET_RAM_CTRL_SRC_SEL_Pos                    (8U)
#define RET_RAM_CTRL_SRC_SEL_Msk                    (0xFF00U)
#define RET_RAM_BIT_Msk                             (0xFF)
#define SYS_RET_RAM_APS_IDX_START                   0
#define SYS_RET_RAM_APS_IDX_END                     3


#define AOS_RET_HW_SETTING           (1 << AOS_RET_MUX_RET_MUX_Pos)
#define AOS_RET_FW_SETTING           (0 << AOS_RET_MUX_RET_MUX_Pos)

#define AOS_STRAP_MODE_MAX           0xF

#define AOS_SLP_MODE_EN              AOS_MODE_CTL_SLP_EN
#define AOS_SLP_MODE_TIMER           (0 << AOS_MODE_CTL_SLP_MODE_Pos)
#define AOS_SLP_MODE_DEEP            (1 << AOS_MODE_CTL_SLP_MODE_Pos)
#define AOS_SLP_MODE_EARLY_WAKE_UP   AOS_MODE_CTL_EARLY_WKUP_EN

#define AOS_SLP_TIMER_MASK_MAX       ((uint64_t)(AOS_SLP_TIMER_PRESET_H_PRESET)<<32 | AOS_SLP_TIMER_PRESET_L_PRESET)


#define SW_BUILD_TIME_STRING_LEN 20


#define HAL_SYS_FAST_STARTUP_EN(en)     (AOS->SDM_CTL = (en)?(AOS_SDM_CTL_SDM_EN|AOS_SDM_CTL_FAST_ST_EN):0)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

typedef enum 
{
    SPARE_0 = offsetof(S_AOS_Reg_t, SPARE0),
    /* SPARE_1 = offsetof(S_AOS_Reg_t, SPARE1), reserrved for HW */
    SPARE_2 = offsetof(S_AOS_Reg_t, SPARE2),
    SPARE_3 = offsetof(S_AOS_Reg_t, SPARE3)
} E_SpareRegIdx_t;

typedef enum 
{
    SLEEP_TIMER,
    SLEEP_DEEP
} E_SleepModeIdx_t;

typedef enum 
{
    PMU_SF_VAL_0P65V = 0,
    PMU_SF_VAL_0P73V = 1, 
    PMU_SF_VAL_0P95V = 4, 
    PMU_SF_VAL_1P10V = 6,
    
    PMU_SF_VAL_MASK = 0x7
} E_PmuSfVal_t;



typedef enum 
{
#if 1
    RET_RAM_APS_0       = 0x01,
    RET_RAM_APS_1       = 0x02,
    RET_RAM_APS_2       = 0x04,
    RET_RAM_APS_3       = 0x08,
    RET_RAM_SHARE       = 0x10,
    RET_RAM_SCRT        = 0x20,
    RET_RAM_MSQ_0       = 0x40,
    RET_RAM_MSQ_1       = 0x80,
#else
    RET_RAM_APS_00      = 0x1,
    RET_RAM_APS_01      = 0x2,
    RET_RAM_APS_02_03   = 0x4,
    RET_RAM_APS_04_05   = 0x8,
    RET_RAM_APS_06_07   = 0x10,
    RET_RAM_APS_08_09   = 0x20,
    RET_RAM_APS_10_11   = 0x40,
    RET_RAM_APS_12_13   = 0x80,
    RET_RAM_APS_14_15   = 0x100,
    RET_RAM_APS_16_17   = 0x200,
    RET_RAM_APS_18_19   = 0x400,
    RET_RAM_SHARE_00    = 0x800,
    RET_RAM_SHARE_01    = 0x1000,
    RET_RAM_SHARE_02_03 = 0x2000,
    RET_RAM_MSQ_00      = 0x4000,
    RET_RAM_MSQ_01      = 0x8000,
    RET_RAM_MSQ_02_03   = 0x10000,
    RET_RAM_SCRT_00     = 0x20000,
    RET_RAM_SCRT_01     = 0x40000,
    RET_RAM_SCRT_02_03  = 0x80000,
#endif
} E_RetRamIdx_t;

typedef enum
{
    STRAP_MODE_NULL     = 0x0,      /* b'0000 No definition */
    STRAP_MODE_DDM      = 0x1,      /* b'0001 Digital debug mode */
    STRAP_MODE_ICE      = 0x2,      /* b'0010 ICE mode */
    STRAP_MODE_JTAG     = 0x3,      /* b'0011 JTAG debug mode */
    STRAP_MODE_DFT_SCAN = 0x4,      /* b'0100 DFT Scan mode */
    STRAP_MODE_OTP      = 0x5,      /* b'0101 OTP Prog mode */
    STRAP_MODE_MBIST    = 0x6,      /* b'0110 MBIST mode */
    STRAP_MODE_AUX      = 0x7,      /* b'0111 AUX ADC debug mode  */
    STRAP_MODE_RDM      = 0x8,      /* b'1000 RF Debug mode */
    STRAP_MODE_RFM      = 0x9,      /* b'1001 Radio only mode */
    STRAP_MODE_NORMAL   = 0xA,      /* b'1010 Normal mode */
    STRAP_MODE_FT       = 0xB,      /* b'1011 FT mode */
    STRAP_MODE_AUX_EXT  = 0xC,      /* b'1100 AUX ADC EXT debug mode */
    STRAP_MODE_MPOM     = 0xD,      /* b'1101 MAC PHY only mode */
} E_StrapMode_t;

typedef struct
{
    uint32_t u32Revision;
    char szBuildTime[SW_BUILD_TIME_STRING_LEN];
} S_SW_VER;

typedef struct
{
    S_SW_VER sApsRom;
    S_SW_VER sApsPatch;
    S_SW_VER sMsqRom;
    S_SW_VER sMsqPatch;
} S_SW_VERSION_INFO;

typedef struct
{
    uint32_t u32BitIdx;
    uint32_t u32RamStart;
    uint32_t u32RamEnd;
} S_RET_RAM_MAP;

/* Power relative */
typedef uint32_t (*T_Hal_Sys_PowerManualEn)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_Sys_PowerDefaultSettings)(void);
typedef uint32_t (*T_Hal_Sys_PmuSfVolSet)(E_PmuSfVal_t eVol);
typedef uint32_t (*T_Hal_Sys_PtatEn)(uint8_t u8Enable);

/* Sleep Mode relative */
typedef void (*T_Hal_Sys_SleepInit)(void);
typedef uint64_t (*T_Hal_Sys_SleepTimerGet)(void);
typedef uint32_t (*T_Hal_Sys_SleepTimerSet)(E_SleepModeIdx_t eSleepMode, uint64_t u64WakeupTime);
typedef void (*T_Hal_Sys_SleepTrig)(void);
typedef void (*T_Hal_Sys_SleepPrepare)(void);
typedef void (*T_Hal_Sys_WakeupResume)(void);

/* Pin-Mux relative*/
typedef void (*T_Hal_SysPinMuxAppInit)(void);
typedef void (*T_Hal_SysDisableAllRxPin)(void);
typedef void (*T_Hal_SysPinMuxDownloadInit)(void);
typedef void (*T_Hal_SysPinMuxSpiFlashInit)(void);
typedef void (*T_Hal_SysPinMuxM3UartSwitch)(void);

/* Ret RAM relative*/
typedef uint32_t (*T_Hal_Sys_RetRamTurnOn)(uint32_t u32RetRamIdxs);
typedef uint32_t (*T_Hal_Sys_RetRamCtrlBySeq)(uint32_t u32RetRamIdxs);



/* Remap relative */
typedef uint32_t (*T_Hal_Sys_MsqRomRemapAll)(uint8_t u8Enable);

/* Miscellaneous */
//typedef uint32_t (*T_Hal_Sys_SramDffBypass)(uint8_t u8Enable);
//typedef uint32_t (*T_Hal_Sys_XipDffBypass)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_Sys_SwDebugEn)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_Sys_StrapModeRead)(void);
typedef uint32_t (*T_Hal_Sys_StrapModeOverWrite)(uint8_t u8Enable, uint8_t u8StrapMode);
typedef uint32_t (*T_Hal_Sys_AosIdRead)(void);
typedef uint32_t (*T_Hal_Sys_ChipIdRead)(void);
typedef uint32_t (*T_Hal_Sys_SpareRegRead)(E_SpareRegIdx_t eSpareIdx, uint32_t *pu32Data);
typedef uint32_t (*T_Hal_Sys_SpareRegWrite)(E_SpareRegIdx_t eSpareIdx, uint32_t u32Data);
typedef uint32_t (*T_Hal_Sys_SpareRegWait)(E_SpareRegIdx_t eSpareIdx, uint32_t u32Mask, uint8_t bClear);
typedef uint32_t (*T_Hal_Sys_SpareRegWriteBit)(E_SpareRegIdx_t eSpareIdx, uint32_t u32Mask, uint32_t u32Value);
typedef uint8_t *(*T_Hal_Sys_OtpRead)(uint16_t u16Offset, uint8_t *u8aBuf, uint16_t u16BufSize);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
extern const S_RET_RAM_MAP g_u32aSramMapping[];
// Sec 5: declaration of global function prototype

/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype

/***********
C Functions
***********/
// Sec 8: C Functions
/* Power relative */
extern T_Hal_Sys_PowerManualEn          Hal_Sys_PowerManualEn;
extern T_Hal_Sys_PowerDefaultSettings   Hal_Sys_PowerDefaultSettings;
extern T_Hal_Sys_PmuSfVolSet            Hal_Sys_PmuSfVolSet;
extern T_Hal_Sys_PtatEn                 Hal_Sys_PtatEn;

/* Sleep Mode relative */
extern T_Hal_Sys_SleepInit              Hal_Sys_SleepInit;
extern T_Hal_Sys_SleepTimerGet          Hal_Sys_SleepTimerGet;
extern T_Hal_Sys_SleepTimerSet          Hal_Sys_SleepTimerSet;
extern T_Hal_Sys_SleepTrig              Hal_Sys_SleepTrig;
extern T_Hal_Sys_SleepPrepare           Hal_Sys_SleepPrepare;
extern T_Hal_Sys_WakeupResume           Hal_Sys_WakeupResume;

/* Pin-Mux relative*/
extern T_Hal_SysDisableAllRxPin         Hal_SysDisableAllRxPin;
extern T_Hal_SysPinMuxAppInit           Hal_SysPinMuxAppInit;
extern T_Hal_SysPinMuxDownloadInit      Hal_SysPinMuxDownloadInit;
extern T_Hal_SysPinMuxSpiFlashInit      Hal_SysPinMuxSpiFlashInit;
extern T_Hal_SysPinMuxM3UartSwitch      Hal_SysPinMuxM3UartSwitch;

/* Ret RAM relative*/
extern T_Hal_Sys_RetRamTurnOn           Hal_Sys_RetRamTurnOn;
extern T_Hal_Sys_RetRamCtrlBySeq        Hal_Sys_RetRamCtrlBySeq;

/* Remap relative */
extern T_Hal_Sys_MsqRomRemapAll         Hal_Sys_MsqRomRemapAll;

/* Miscellaneous */
//extern T_Hal_Sys_SramDffBypass          Hal_Sys_SramDffBypass;
//extern T_Hal_Sys_XipDffBypass           Hal_Sys_XipDffBypass;
extern T_Hal_Sys_SwDebugEn              Hal_Sys_SwDebugEn;
extern T_Hal_Sys_StrapModeRead          Hal_Sys_StrapModeRead;
extern T_Hal_Sys_StrapModeOverWrite     Hal_Sys_StrapModeOverWrite;
extern T_Hal_Sys_AosIdRead              Hal_Sys_AosIdRead;
extern T_Hal_Sys_ChipIdRead             Hal_Sys_ChipIdRead;
extern T_Hal_Sys_SpareRegRead           Hal_Sys_SpareRegRead;
extern T_Hal_Sys_SpareRegWrite          Hal_Sys_SpareRegWrite;
extern T_Hal_Sys_SpareRegWait           Hal_Sys_SpareRegWait;
extern T_Hal_Sys_SpareRegWriteBit       Hal_Sys_SpareRegWriteBit;
extern T_Hal_Sys_OtpRead                Hal_Sys_OtpRead;


//uint32_t Hal_Sys_SramDffBypass_impl(uint8_t u8Enable);
//uint32_t Hal_Sys_XipDffBypass_impl(uint8_t u8Enable);
uint32_t Hal_Sys_SwDebugEn_impl(uint8_t u8Enable);
uint32_t Hal_Sys_StrapModeRead_impl(void);
uint32_t Hal_Sys_StrapModeOverWrite_impl(uint8_t u8Enable, uint8_t u8StrapMode);
uint32_t Hal_Sys_AosIdRead_impl(void);
uint32_t Hal_Sys_ChipIdRead_impl(void);
uint32_t Hal_Sys_PowerManualEn_impl(uint8_t u8Enable);
uint32_t Hal_Sys_PowerDefaultSettings_impl(void);
uint32_t Hal_Sys_PmuSfVolSet_impl(E_PmuSfVal_t eVol);
uint32_t Hal_Sys_PtatEn_impl(uint8_t u8Enable);
void Hal_Sys_SleepInit_impl(void);
uint64_t Hal_Sys_SleepTimerGet_impl(void);
uint32_t Hal_Sys_SleepTimerSet_impl(E_SleepModeIdx_t eSleepMode, uint64_t u64WakeupTime);
void Hal_Sys_SleepTrig_impl(void);
void Hal_Sys_SleepPrepare_impl(void);
void Hal_Sys_WakeupResume_impl(void);
void Hal_SysDisableAllRxPin_impl(void);
void Hal_SysPinMuxAppInit_impl(void);
void Hal_SysPinMuxDownloadInit_impl(void);
void Hal_SysPinMuxSpiFlashInit_impl(void);
void Hal_SysPinMuxM3UartSwitch_impl(void);
uint32_t Hal_Sys_RetRamTurnOff(uint32_t u32RetRamIdxs );
uint32_t Hal_Sys_RetRamTurnOn_impl(uint32_t u32RetRamIdxs);
uint32_t Hal_Sys_RetRamCtrlBySeq_impl(uint32_t u32RetRamIdxs);
uint32_t Hal_Sys_MsqRomRemapAll_impl(uint8_t u8Enable);
uint32_t Hal_Sys_SpareRegRead_impl(E_SpareRegIdx_t eSpareIdx, uint32_t *pu32Data);
uint32_t Hal_Sys_SpareRegWrite_impl(E_SpareRegIdx_t eSpareIdx, uint32_t u32Data);
uint32_t Hal_Sys_SpareRegWait_impl(E_SpareRegIdx_t eSpareIdx, uint32_t u32Mask, uint8_t bClear);
uint32_t Hal_Sys_SpareRegWriteBit_impl(E_SpareRegIdx_t eSpareIdx, uint32_t u32Mask, uint32_t u32Value);
uint8_t *Hal_Sys_OtpRead_impl(uint16_t u16Offset, uint8_t *u8aBuf, uint16_t u16BufSize);
void Hal_Sys_RomVerInit(uint32_t u32VerInfoAddr);

#include "hal_system_patch.h"
#endif
