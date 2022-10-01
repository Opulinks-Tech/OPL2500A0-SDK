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
*  hal_system.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of system functions
*  Include sys-reg and AOS domains.
*  Ref. documents are:
*    SYS: input.xml (Last modified: 2017/10/26)
*    AOS: aos_reg_A0_171025.xml
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include <string.h>
#include "opulinks.h"
#include "hal_system.h"
#include "hal_sys_rcc.h"
#include "hal_pin.h"
#include "hal_pin_def.h"
#include "hal_pin_config.h"
#include "svn_rev.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous




#define AOS_RET_RAM_MASK             0xFF /* 8 bits*/
#define AOS_RET_RAM_TIMEOUT          0x500


//0x034-0x04C
#define SEQ_HPBG_ON_POS              10
#define SEQ_HPBG_ON                  (1 << SEQ_HPBG_ON_POS)
#define SEQ_BUCK_ON_POS              11
#define SEQ_BUCK_ON                  (1 << SEQ_BUCK_ON_POS)
#define SEQ_RET_EN_POS               12
#define SEQ_RET_EN                   (1 << SEQ_RET_EN_POS)
#define SEQ_SDM_RST_POS              13
#define SEQ_SDM_RST                  (1 << SEQ_SDM_RST_POS)
#define SEQ_SW_ON_POS                14
#define SEQ_SW_ON                    (1 << SEQ_SW_ON_POS)
#define SEQ_ISO_EN_POS               15
#define SEQ_ISO_EN                   (1 << SEQ_ISO_EN_POS)


//0x08C
#define LDO_RF_EN_POS                9
#define LDO_RF_EN_MSK                (0x1 << LDO_RF_EN_POS)
#define LDO_VCO_EN_POS               10
#define LDO_VCO_EN_MSK               (0x1 << LDO_VCO_EN_POS)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...



/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
/* Power relative */
T_Hal_Sys_PowerManualEn         Hal_Sys_PowerManualEn           = Hal_Sys_PowerManualEn_impl;
T_Hal_Sys_PowerDefaultSettings  Hal_Sys_PowerDefaultSettings    = Hal_Sys_PowerDefaultSettings_impl;
T_Hal_Sys_PmuSfVolSet           Hal_Sys_PmuSfVolSet             = Hal_Sys_PmuSfVolSet_impl;
T_Hal_Sys_PtatEn                Hal_Sys_PtatEn                  = Hal_Sys_PtatEn_impl;

/* Sleep Mode relative */
T_Hal_Sys_SleepInit             Hal_Sys_SleepInit               = Hal_Sys_SleepInit_impl;
T_Hal_Sys_SleepTimerGet         Hal_Sys_SleepTimerGet           = Hal_Sys_SleepTimerGet_impl;
T_Hal_Sys_SleepTimerSet         Hal_Sys_SleepTimerSet           = Hal_Sys_SleepTimerSet_impl;
T_Hal_Sys_SleepTrig             Hal_Sys_SleepTrig               = Hal_Sys_SleepTrig_impl;
T_Hal_Sys_SleepPrepare          Hal_Sys_SleepPrepare            = Hal_Sys_SleepPrepare_impl;
T_Hal_Sys_WakeupResume          Hal_Sys_WakeupResume            = Hal_Sys_WakeupResume_impl;

/* Pin-Mux relative*/
T_Hal_SysDisableAllRxPin        Hal_SysDisableAllRxPin          = Hal_SysDisableAllRxPin_impl;
T_Hal_SysPinMuxAppInit          Hal_SysPinMuxAppInit            = Hal_SysPinMuxAppInit_impl;
T_Hal_SysPinMuxDownloadInit     Hal_SysPinMuxDownloadInit       = Hal_SysPinMuxDownloadInit_impl;
T_Hal_SysPinMuxSpiFlashInit     Hal_SysPinMuxSpiFlashInit       = Hal_SysPinMuxSpiFlashInit_impl;
T_Hal_SysPinMuxM3UartSwitch     Hal_SysPinMuxM3UartSwitch       = Hal_SysPinMuxM3UartSwitch_impl;

/* Ret RAM relative*/
T_Hal_Sys_RetRamTurnOn          Hal_Sys_RetRamTurnOn            = Hal_Sys_RetRamTurnOn_impl;
T_Hal_Sys_RetRamCtrlBySeq       Hal_Sys_RetRamCtrlBySeq         = Hal_Sys_RetRamCtrlBySeq_impl;

/* Remap relative */
T_Hal_Sys_MsqRomRemapAll        Hal_Sys_MsqRomRemapAll          = Hal_Sys_MsqRomRemapAll_impl;

/* Miscellaneous */
T_Hal_Sys_SramDffBypass         Hal_Sys_SramDffBypass           = Hal_Sys_SramDffBypass_impl;
T_Hal_Sys_XipDffBypass          Hal_Sys_XipDffBypass            = Hal_Sys_XipDffBypass_impl;
T_Hal_Sys_SwDebugEn             Hal_Sys_SwDebugEn               = Hal_Sys_SwDebugEn_impl;
T_Hal_Sys_StrapModeRead         Hal_Sys_StrapModeRead           = Hal_Sys_StrapModeRead_impl;
T_Hal_Sys_StrapModeOverWrite    Hal_Sys_StrapModeOverWrite      = Hal_Sys_StrapModeOverWrite_impl;
T_Hal_Sys_AosIdRead             Hal_Sys_AosIdRead               = Hal_Sys_AosIdRead_impl;
T_Hal_Sys_ChipIdRead            Hal_Sys_ChipIdRead              = Hal_Sys_ChipIdRead_impl;
T_Hal_Sys_SpareRegRead          Hal_Sys_SpareRegRead            = Hal_Sys_SpareRegRead_impl;
T_Hal_Sys_SpareRegWrite         Hal_Sys_SpareRegWrite           = Hal_Sys_SpareRegWrite_impl;
T_Hal_Sys_SpareRegWait          Hal_Sys_SpareRegWait            = Hal_Sys_SpareRegWait_impl;
T_Hal_Sys_SpareRegWriteBit      Hal_Sys_SpareRegWriteBit        = Hal_Sys_SpareRegWriteBit_impl;
T_Hal_Sys_OtpRead               Hal_Sys_OtpRead                 = Hal_Sys_OtpRead_impl;


const S_RET_RAM_MAP g_u32aSramMapping[] = {
    {0, 0x00400000, 0x00408000},
    {1, 0x00408000, 0x00418000},
    {2, 0x00418000, 0x00430000},
    {3, 0x00430000, 0x00454000},
};

uint32_t g_u32SramUsageBmp = 0xFF;  /* All RAM are used */

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable
bool g_bHal_Sys_PinmuxBootromSpiQuanEn = false;


// Sec 7: declaration of static function prototype



/***********
C Functions
***********/
// Sec 8: C Functions
/*************************************************************************
* FUNCTION:
*  Hal_Sys_SramDffBypass
*
* DESCRIPTION:
*   1. Enable SRAM FIFO bypass (Turn-off when use 150Mhz clock)
*   2. Related reg.: sys_reg 0x00C
* CALLS
*
* PARAMETERS
*   1. u8Enable: 1 for enable/ 0 for disable
*
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_SramDffBypass_impl(uint8_t u8Enable)
{
    if (u8Enable)
    {
        SYS->SRAM_BYPASS |= (SYS_SRAM_BYPASS_APS_SRAM_RD_FF1_BYPASS|SYS_SRAM_BYPASS_APS_SRAM_RD_FF2_BYPASS|SYS_SRAM_BYPASS_APS_SRAM_WR_FF_BYPASS|
                            SYS_SRAM_BYPASS_SHARE_SRAM_RD_FF1_BYPASS|SYS_SRAM_BYPASS_SHARE_SRAM_RD_FF2_BYPASS|SYS_SRAM_BYPASS_SHARE_SRAM_WR_FF_BYPASS);
    }
    else
    {
        SYS->SRAM_BYPASS &= ~(SYS_SRAM_BYPASS_APS_SRAM_RD_FF1_BYPASS|SYS_SRAM_BYPASS_APS_SRAM_RD_FF2_BYPASS|SYS_SRAM_BYPASS_APS_SRAM_WR_FF_BYPASS|
                            SYS_SRAM_BYPASS_SHARE_SRAM_RD_FF1_BYPASS|SYS_SRAM_BYPASS_SHARE_SRAM_RD_FF2_BYPASS|SYS_SRAM_BYPASS_SHARE_SRAM_WR_FF_BYPASS);
    }
    /* Avoid immediately access RAM after setting bypass SRAM */
    __ISB();
    __DSB();
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_XipDffBypass
*
* DESCRIPTION:
*   1. Enable XIP FIFO bypass (Turn-off when use 150Mhz clock)
*   2. Related reg.: sys_reg 0x00C
* CALLS
*
* PARAMETERS
*   1. u8Enable: 1 for enable/ 0 for disable
*
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_XipDffBypass_impl(uint8_t u8Enable)
{
    if (u8Enable)
    {
        SYS->SRAM_BYPASS |= SYS_SRAM_BYPASS_QSPI_RD_FF_BYPASS;
    }
    else
    {
        SYS->SRAM_BYPASS &= ~SYS_SRAM_BYPASS_QSPI_RD_FF_BYPASS;
    }
    /* Avoid immediately access XIP after setting bypass XIP */
    __ISB();
    __DSB();
    return 0;
}


/*************************************************************************
* FUNCTION:
*  Hal_Sys_SwDebugEn
*
* DESCRIPTION:
*   1. Enable SWD debug function
*   2. Related reg.: sys_reg 0x014
* CALLS
*
* PARAMETERS
*   1. u8Enable: 1 for enable/ 0 for disable
*
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_SwDebugEn_impl(uint8_t u8Enable)
{
    if(u8Enable)
        SYS->SW_DBG_EN |= SYS_SW_DBG_EN_APS_DBGEN;
    else
        SYS->SW_DBG_EN &= ~SYS_SW_DBG_EN_APS_DBGEN;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_StrapModeRead
*
* DESCRIPTION:
*   1. Get boot 4-bit strap mode
*   2. Related reg.: sys_reg 0x018
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   LBS 4 bits are strap mode
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_StrapModeRead_impl(void)
{
    return (SYS->BOOT_STS & AOS_STRAP_MODE_CTL_MODE_OV_ROM_VAL);
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_StrapModeOverWrite
*
* DESCRIPTION:
*   1. Over-wrtie strap mode by reg.
*   2. Related reg.: AOS 0x100
* CALLS
*
* PARAMETERS
*   1. u8Enable    : 1 for enable/ 0 for disable
*   2. u8StrapMode : strap mode
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_StrapModeOverWrite_impl(uint8_t u8Enable, uint8_t u8StrapMode)
{
    if(u8Enable)
    {
        if(u8StrapMode > AOS_STRAP_MODE_MAX)
            return 1;
        // Clear all field
        AOS->STRAP_MODE_CTL &= ~AOS_STRAP_MODE_CTL_MODE_OV_ROM_VAL_Msk;
        // Set new value
        AOS->STRAP_MODE_CTL |= (AOS_STRAP_MODE_CTL_MODE_OV_ROM_EN|u8StrapMode);
    }else{
        AOS->STRAP_MODE_CTL &= ~AOS_STRAP_MODE_CTL_MODE_OV_ROM_EN_Msk;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_AosIdRead
*
* DESCRIPTION:
*   1. Get AOS ID
*   2. Related reg.: AOS 0x120
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   uint32_t for AOS ID
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_AosIdRead_impl(void)
{
    return AOS->AOS_ID;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_ChipIdRead
*
* DESCRIPTION:
*   1. [7:0]: HW ver. (ECO)
*   2. Related reg.: sys_reg 0x504[27:20]
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   uint32_t for HW ver.
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_ChipIdRead_impl(void)
{
    return (AOS->MSQ_RMP_MSK[2] & AOS_MSQ_RMP_MSK_ECO_ID) >> AOS_MSQ_RMP_MSK_ECO_ID_Pos;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_PowerManualEn
*
* DESCRIPTION:
*   1. Enable/disable power relative setting manual control
*   2. Related reg.: AOS 0x000
* CALLS
*
* PARAMETERS
*   1. u8Enable : 1 for enable/ 0 for disable
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_PowerManualEn_impl(uint8_t u8Enable)
{
    if(u8Enable)
        AOS->RET_MUX = AOS_RET_FW_SETTING;
    else
        AOS->RET_MUX = AOS_RET_HW_SETTING;
	return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_PowerDefaultSettings
*
* DESCRIPTION:
*   the default settings of power part
* CALLS
*
* PARAMETERS
*
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_PowerDefaultSettings_impl(void)
{
    volatile uint32_t tmp;
    
    // Enable PowerManual
    Hal_Sys_PowerManualEn(1);

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_PmuSfVolset
*
* DESCRIPTION:
*   1. PMU_SF voltage select
*   2. Related reg.: AOS 0x024
* CALLS
*
* PARAMETERS
*   1. eVol : PMU_SF voltage select. Refer to E_PmuSfVal_t
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_PmuSfVolSet_impl(E_PmuSfVal_t eVol)
{
    switch(eVol)
    {
        case PMU_SF_VAL_0P65V:
        case PMU_SF_VAL_0P73V:
        case PMU_SF_VAL_0P95V: 
        case PMU_SF_VAL_1P10V:
            AOS->PMU_SF_VAL_CTL = eVol;
            break;
        default:
            return 1;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_PtatEn
*
* DESCRIPTION:
*   1. Enable/Disable PTAT
*   2. Related reg.: AOS 0x028
*
* CALLS
*
* PARAMETERS
*   1. u8Enable : 0 for disable, 1 for enable
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_PtatEn_impl(uint8_t u8Enable)
{
    if(u8Enable)
        AOS->HPBG_CTL |= AOS_HPBG_CTL_PTAT_EN;
    else
        AOS->HPBG_CTL &= ~AOS_HPBG_CTL_PTAT_EN_Msk;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_SleepInit
*
* DESCRIPTION:
*   1. Sleep mode initail
*   2. Related reg.: AOS 0x01C, 0x034~0x050, and 0x058 ~0x078
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Sys_SleepInit_impl(void)
{
#if 0
    // Need make rising pulse. So clean bit first
    AOS->MODE_CTL &= ~AOS_MODE_CTL_SLP_EN;

	/* SEQ Configuration from Joe
    // HW provide setting to make pulse
    AOS->PS_TIMER_PRESET = 0x3FF; // PS 0b' 11 1111 1111
    AOS->ON1_TIME        = 0xB420; // ON_1 0b' 0 | 1010 01 | 00 0010 0000
    AOS->ON2_TIME        = 0xBC40; // ON_2 0b' 0 | 1010 11 | 00 0100 0000
    AOS->ON3_TIME        = 0xFC60; // ON_3 0b' 0 | 1111 11 | 00 0110 0000
    AOS->ON4_TIME        = 0xFC80; // ON_4 0b' 0 | 1111 11 | 00 1000 0000
    AOS->ON5_TIME        = 0xFCA0; // ON_5 0b' 0 | 1111 11 | 00 1010 0000
    AOS->ON6_TIME        = 0xDCC0; // ON_6 0b' 0 | 1101 11 | 00 1100 0000
    AOS->ON7_TIME        = 0x4FFE; // ON_7 0b' 0 | 0010 11 | 11 1111 1110
    AOS->CPOR_N_ON_TIME  = 0x4FFE; // CPOR 0b' 0100 1111 1111 1110

    AOS->SPS_TIMER_PRESET = 0x0B0; // SPS 0b' 00 0000 1011 0000
    AOS->SON1_TIME        = 0x1DC20; // SON_1 0b' 1 | 1101 11 | 00 0010 0000
    AOS->SON2_TIME        = 0x1FC40; // SON_2 0b' 1 | 1111 11 | 00 0100 0000
    AOS->SON3_TIME        = 0x0FC45; // SON_3 0b' 0 | 1111 11 | 00 0100 0101
    AOS->SON4_TIME        = 0x0FC50; // SON_4 0b' 0 | 1111 11 | 00 0101 0000
    AOS->SON5_TIME        = 0x0BC60; // SON_5 0b' 0 | 1011 11 | 00 0110 0000
    AOS->SON6_TIME        = 0x0B480; // SON_6 0b' 0 | 1011 01 | 00 1000 0000
    AOS->SON7_TIME        = 0x0B0A0; // SON_7 0b' 0 | 1011 00 | 00 1010 0000
    AOS->SCPOR_N_ON_TIME  = 0x0040; //SCPOR 0b' 0000 0000 0100 0000
	*/

	// SEQ Configuration from Joe with Luke
    // HW provide setting to make pulse
    AOS->PS_TIMER_PRESET = 0x0005; // PS 0b' 11 1111 1111
    AOS->ON1_TIME        = 0x9401; // ON_1 0b' 0 | 1010 01 | 00 0010 0000
    AOS->ON2_TIME        = 0x9C02; // ON_2 0b' 0 | 1010 11 | 00 0100 0000
    AOS->ON3_TIME        = 0xDC03; // ON_3 0b' 0 | 1111 11 | 00 0110 0000
    AOS->ON4_TIME        = 0xDC03; // ON_4 0b' 0 | 1111 11 | 00 1000 0000
    AOS->ON5_TIME        = 0xDC03; // ON_5 0b' 0 | 1111 11 | 00 1010 0000
    AOS->ON6_TIME        = 0xDC03; // ON_6 0b' 0 | 1101 11 | 00 1100 0000
    AOS->ON7_TIME        = 0x6C04; // ON_7 0b' 0 | 0010 11 | 11 1111 1110
    AOS->CPOR_N_ON_TIME  = 0x0405; // CPOR 0b' 0100 1111 1111 1110

    AOS->SPS_TIMER_PRESET = 0x0006; // SPS 0b' 00 0000 1011 0000
    AOS->SON1_TIME        = 0xCC01; // SON_1 0b' 1 | 1101 11 | 00 0010 0000
    AOS->SON2_TIME        = 0x8C02; // SON_2 0b' 1 | 1111 11 | 00 0100 0000
    AOS->SON3_TIME        = 0x9C03; // SON_3 0b' 0 | 1111 11 | 00 0100 0101
    AOS->SON4_TIME        = 0x9404; // SON_4 0b' 0 | 1111 11 | 00 0101 0000
    AOS->SON5_TIME        = 0x9005; // SON_5 0b' 0 | 1011 11 | 00 0110 0000
    AOS->SON6_TIME        = 0x9005; // SON_6 0b' 0 | 1011 01 | 00 1000 0000
    AOS->SON7_TIME        = 0x9005; // SON_7 0b' 0 | 1011 00 | 00 1010 0000
    AOS->SCPOR_N_ON_TIME  = 0x0006; //SCPOR 0b' 0000 0000 0100 0000
#else
    // Set LDO controlled by SEQ
    AOS->LDO_CTL = AOS->LDO_CTL & (~LDO_RF_EN_MSK & ~LDO_VCO_EN_MSK);

    // Set RetRAM voltage
    AOS->RET_SF_VAL_CTL = 0x05;

    // Set current limit
    AOS->PMU_CTRL = (AOS->PMU_CTRL & ~AOS_PMU_CTRL_PMU_CSA_SEL_Msk) | (3 << AOS_PMU_CTRL_PMU_CSA_SEL_Pos) | AOS_PMU_CTRL_PMU_BUCKC_CLIMIT;    

    // Need make rising pulse. So clean bit first
    AOS->MODE_CTL &= ~(AOS_MODE_CTL_SLP_EN_Msk | AOS_MODE_CTL_SLP_MODE_Msk | AOS_MODE_CTL_EARLY_WKUP_EN_Msk);

    // HW provide setting to make pulse
    AOS->ON1_TIME         = 0x0001 | SEQ_ISO_EN                           | SEQ_RET_EN               | SEQ_HPBG_ON;
    AOS->ON2_TIME         = 0x0002 | SEQ_ISO_EN                           | SEQ_RET_EN | SEQ_BUCK_ON | SEQ_HPBG_ON;
    AOS->ON3_TIME         = 0x0008 | SEQ_ISO_EN | SEQ_SW_ON               | SEQ_RET_EN | SEQ_BUCK_ON | SEQ_HPBG_ON;
    AOS->ON4_TIME         = 0x000E              | SEQ_SW_ON | SEQ_SDM_RST              | SEQ_BUCK_ON | SEQ_HPBG_ON;
    AOS->ON5_TIME         = AOS->ON4_TIME;
    AOS->ON6_TIME         = AOS->ON4_TIME;
    AOS->ON7_TIME         = AOS->ON4_TIME;
    AOS->CPOR_N_ON_TIME   = 0x0410;
    AOS->PS_TIMER_PRESET  = AOS->CPOR_N_ON_TIME & 0x3FF;
	
    AOS->SON1_TIME        = 0x0001 | SEQ_ISO_EN | SEQ_SW_ON               | SEQ_RET_EN | SEQ_BUCK_ON | SEQ_HPBG_ON;
    AOS->SON2_TIME        = 0x0002 | SEQ_ISO_EN                           | SEQ_RET_EN;
    AOS->SON3_TIME        = AOS->SON2_TIME;
    AOS->SON4_TIME        = AOS->SON2_TIME;
    AOS->SON5_TIME        = AOS->SON2_TIME;
    AOS->SON6_TIME        = AOS->SON2_TIME;
    AOS->SON7_TIME        = AOS->SON2_TIME;
    AOS->SCPOR_N_ON_TIME  = 0x0003;
    AOS->SPS_TIMER_PRESET = AOS->SCPOR_N_ON_TIME & 0x3FF;

    // Config Retention SRAM controlled by FW
    AOS->SRAM_ISO_EN      = 0x0000FF00;
    AOS->SRAM_PSD         = 0x0000FF00;
#endif
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_SleepTimerGet
*
* DESCRIPTION:
*   1. Setup a sleep event
*   2. Related reg.: AOS 0x014 and 0x018
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   uint64_t 37-bits of timer, Max: 0x1F FFFFFFFF
* GLOBALS AFFECTED
*
*************************************************************************/
uint64_t Hal_Sys_SleepTimerGet_impl(void)
{
    uint64_t u64Time_1 = 0;
    uint64_t u64Time_2 = 0;
    
    do{
        u64Time_1 = (uint64_t)(AOS->SLP_TIMER_CURR_H)<<32 | AOS->SLP_TIMER_CURR_L;
        u64Time_2 = (uint64_t)(AOS->SLP_TIMER_CURR_H)<<32 | AOS->SLP_TIMER_CURR_L;
    }while(u64Time_1 != u64Time_2);
    
    return u64Time_1;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_SleepTimerSet
*
* DESCRIPTION:
*   1. Setup a sleep event
*   2. Related reg.: AOS 0x004, 0x014 and 0x018
* CALLS
*
* PARAMETERS
*   1. eSleepMode    : Sleep mode select. refert to  E_SleepModeIdx_t
*   2. u64WakeupTime : 37-bits of timer, Max: 0x1F FFFFFFFF
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_SleepTimerSet_impl(E_SleepModeIdx_t eSleepMode, uint64_t u64WakeupTime)
{
    switch(eSleepMode)
    {
        case SLEEP_TIMER:
            AOS->MODE_CTL |= AOS_SLP_MODE_TIMER;
            break;
        case SLEEP_DEEP:
            AOS->MODE_CTL |= AOS_SLP_MODE_DEEP;
            break;
        default:
            return 1;
    }
    //Always need early wake (wake up by interrupt)
    AOS->MODE_CTL |= AOS_MODE_CTL_EARLY_WKUP_EN;

    if(u64WakeupTime > AOS_SLP_TIMER_MASK_MAX)
        return 1;
    AOS->SLP_TIMER_PRESET_L = u64WakeupTime & AOS_SLP_TIMER_PRESET_L_PRESET_Msk;
    AOS->SLP_TIMER_PRESET_H = (u64WakeupTime>>32) & AOS_SLP_TIMER_PRESET_H_PRESET_Msk;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_SleepTrig
*
* DESCRIPTION:
*   1. Sleep mode trigger
*   2. Related reg.: AOS 0x004
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Sys_SleepTrig_impl(void)
{
    AOS->MODE_CTL |= AOS_MODE_CTL_SLP_EN;
}


/*************************************************************************
* FUNCTION:
*  Hal_Sys_SleepPrepare
*
* DESCRIPTION:
*   1. Prepare register storage before sleep
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Sys_SleepPrepare_impl(void)
{
    Hal_Sys_PtatEn(0);          // disable HPBG
    AOS->RSTS = 0xFFFFFFFF;     // clear all boot status
    HAL_SYS_FAST_STARTUP_EN(1); // enable fast-start mechanism
    Hal_Sys_SleepClkSetup();
    Hal_Vic_SleepStore();
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_WakeupResume
*
* DESCRIPTION:
*   1. Resume GPIO pinmux
*   2. Resume NVIC
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Sys_WakeupResume_impl(void)
{
    Hal_Vic_WakeupResume();
}


/**
 * @brief Disable all RX pin.
 *        Call this function before setup pinmux.
 *        Then after setup, only the needed pins are enabled.
 */
void Hal_SysDisableAllRxPin_impl(void)
{
    /* UART0 */
    Hal_Pin_Config(PIN_TYPE_UART0_CTS_N_HIGH);
    Hal_Pin_Config(PIN_TYPE_UART0_RXD_HIGH);
    /* UART1 */
    Hal_Pin_Config(PIN_TYPE_UART1_CTS_N_HIGH);
    Hal_Pin_Config(PIN_TYPE_UART1_RXD_HIGH);
    /* APS UART */
    Hal_Pin_Config(PIN_TYPE_APS_UART_CTS_N_HIGH);
    Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_HIGH);
    /* MSQ UART */
    Hal_Pin_Config(PIN_TYPE_MSQ_UART_CTS_N_HIGH);
    Hal_Pin_Config(PIN_TYPE_MSQ_UART_RXD_HIGH);
    /* I2C */
    Hal_Pin_Config(PIN_TYPE_I2C_SCL_IN_HIGH);
    Hal_Pin_Config(PIN_TYPE_I2C_SDA_IN_HIGH);
    /* JTAG */
    Hal_Pin_Config(PIN_TYPE_TDI_HIGH);
    Hal_Pin_Config(PIN_TYPE_TCK_LOW);
    Hal_Pin_Config(PIN_TYPE_TMS_HIGH);
    /* ICE */
    Hal_Pin_Config(PIN_TYPE_APS_SWDI_HIGH);
    Hal_Pin_Config(PIN_TYPE_APS_SWCLK_LOW);
    Hal_Pin_Config(PIN_TYPE_MSQ_SWDI_HIGH);
    Hal_Pin_Config(PIN_TYPE_MSQ_SWCLK_LOW);
    
    /* SPI0 */
    Hal_Pin_Config(PIN_TYPE_SPI0_RXD0_HIGH);
    Hal_Pin_Config(PIN_TYPE_SPI0_RXD1_HIGH);
    Hal_Pin_Config(PIN_TYPE_SPI0_RXD2_HIGH);
    Hal_Pin_Config(PIN_TYPE_SPI0_RXD3_HIGH);
    /* SPI1 */
    Hal_Pin_Config(PIN_TYPE_SPI1_RXD0_HIGH);
    Hal_Pin_Config(PIN_TYPE_SPI1_RXD1_HIGH);
    Hal_Pin_Config(PIN_TYPE_SPI1_RXD2_HIGH);
    Hal_Pin_Config(PIN_TYPE_SPI1_RXD3_HIGH);
    /* SPI2 */
    Hal_Pin_Config(PIN_TYPE_SPI2_CLK_LOW);
    Hal_Pin_Config(PIN_TYPE_SPI2_CS_IN_HIGH);
    Hal_Pin_Config(PIN_TYPE_SPI2_RXD_HIGH);
    /* SPI3 */
    Hal_Pin_Config(PIN_TYPE_SPI3_RXD0_HIGH);
    Hal_Pin_Config(PIN_TYPE_SPI3_RXD1_HIGH);
    Hal_Pin_Config(PIN_TYPE_SPI3_RXD2_HIGH);
    Hal_Pin_Config(PIN_TYPE_SPI3_RXD3_HIGH);
    /* OTP */
    Hal_Pin_Config(PIN_TYPE_OTP_OPM_MODE_LOW);
    /* I2S */
    Hal_Pin_Config(PIN_TYPE_I2S_SDI0_HIGH);
    /* PDM */
    Hal_Pin_Config(PIN_TYPE_PDM_RXD_HIGH);
    /* Ext. In clock */
    Hal_Pin_Config(PIN_TYPE_CLK_EXT_APS_LOW);
    Hal_Pin_Config(PIN_TYPE_CLK_EXT_MSQ_LOW);
    /* Ext. Out clock En */
    Hal_Pin_Config(PIN_TYPE_EXT_2IO_CLK_EN_LOW);
    
    /* CAM */
    Hal_Pin_Config(PIN_TYPE_CAM_PCLK_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_HREF_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_VSYNC_IN_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_HSYNC_IN_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA0_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA1_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA2_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA3_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA4_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA5_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA6_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA7_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA8_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA9_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA10_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA11_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA12_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA13_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA14_HIGH);
    Hal_Pin_Config(PIN_TYPE_CAM_DATA15_HIGH);
    
    /* PSRAM */
    Hal_Pin_Config(PIN_TYPE_PSRAM_RXD0_HIGH);
    Hal_Pin_Config(PIN_TYPE_PSRAM_RXD1_HIGH);
    Hal_Pin_Config(PIN_TYPE_PSRAM_RXD2_HIGH);
    Hal_Pin_Config(PIN_TYPE_PSRAM_RXD3_HIGH);
}

/*************************************************************************
* FUNCTION:
*  Hal_SysPinMuxAppInit
*
* DESCRIPTION:
*   1. Pin-Mux initial for application stage
*   2. Related reg.: AOS 0x090 ~ 0x0DC
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_SysPinMuxAppInit_impl(void)
{
    Hal_SysDisableAllRxPin();

    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_0);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_1);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_2);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_3);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_4);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_5);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_6);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_7);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_8);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_9);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_10);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_11);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_12);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_13);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_14);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_15);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_16);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_17);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_18);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_19);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_20);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_21);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_22);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_23);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_24);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_25);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_26);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_27);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_28);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_29);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_30);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_31);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_32);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_33);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_34);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_35);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_36);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_37);

    /* SiP IO */
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_SIP_39);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_SIP_40);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_SIP_41);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_SIP_42);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_SIP_43);
    Hal_Pin_Config(HAL_PIN_TYPE_DEFAULT_IO_SIP_44);
    
    Hal_Pin_UpdatePsramCfg();
}
/*************************************************************************
* FUNCTION:
*  Hal_SysPinMuxDownloadInit
*
* DESCRIPTION:
*   1. Pin-Mux initial for download stage
*   2. Related reg.: AOS 0x090 ~ 0x0DC
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_SysPinMuxDownloadInit_impl(void)
{
    Hal_SysPinMuxM3UartSwitch();
    Hal_SysPinMuxSpiFlashInit();
}

/*************************************************************************
* FUNCTION:
*  Hal_SysPinMuxSpiFlashInit
*
* DESCRIPTION:
*   1. Pin-Mux initial for SPI flash
*   2. Related reg.: AOS 0x090 ~ 0x0DC
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_SysPinMuxSpiFlashInit_impl(void)
{
    /* SPI0 */
    /* GPIO12(CS), GPIO13(CLK) */
    Hal_Pin_Config(PIN_TYPE_SPI0_CS0_IO12);
    Hal_Pin_Config(PIN_TYPE_SPI0_CLK_IO13);
    
    if (g_bHal_Sys_PinmuxBootromSpiQuanEn == true)
    {   /* Quad bit mode */
        /* GPIO14(IO0), GPIO15(IO1) */
        Hal_Pin_Config(PIN_TYPE_SPI0_IO0_IO14);
        Hal_Pin_Config(PIN_TYPE_SPI0_IO1_IO15);
        
        /* GPIO10(IO2), GPIO11(IO3) */
        Hal_Pin_Config(PIN_TYPE_SPI0_IO2_IO10);
        Hal_Pin_Config(PIN_TYPE_SPI0_IO3_IO11);
    }
    else
    {   /* Single bit mode */
        /* GPIO14(MOSI), GPIO15(MISO) */
        Hal_Pin_Config(PIN_TYPE_SPI0_TXD0_IO14);
        Hal_Pin_Config(PIN_TYPE_SPI0_RXD1_IO15);

        /* GPIO10(WP), IO11(HOLD) */
        Hal_Pin_Config(PIN_TYPE_GPIO_OUT_HIGH_IO10);
        Hal_Pin_Config(PIN_TYPE_GPIO_OUT_HIGH_IO11);
    }
    
}

/*************************************************************************
* FUNCTION:
*  Hal_SysPinMuxM3UartSwitch
*
* DESCRIPTION:
*   1. Pin-Mux for download stage
*   2. Related reg.: AOS 0x090 ~ 0x0DC
* CALLS
*
* PARAMETERS
*   None
* RETURNS
*   None
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_SysPinMuxM3UartSwitch_impl(void)
{
// APS_dbg_uart
    // IO2(RX), IO0(TX)
    Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_IO2 | PIN_INMODE_IO2_PULL_UP);
    Hal_Pin_Config(PIN_TYPE_APS_UART_TXD_IO0);
    
// UART1
    // IO5(TX), IO1(RX)
    Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_IO1 | PIN_INMODE_IO1_PULL_UP);
    Hal_Pin_Config(PIN_TYPE_APS_UART_TXD_IO5);
}


/*************************************************************************
* FUNCTION:
*  Hal_Sys_RetRamTurnOff
*
* DESCRIPTION:
*   1. Turn off the given RetRam blocks. Ref. to E_RetRamIdx_t
*   2. Related reg.: AOS 0x0E0 ,0x0E4 and 0x118
*
* @warning Don't access related RAM after turn off. Including stack access.
* CALLS
*
* PARAMETERS
*   1. u32RetRamIdxs : Multi index combined. index refer to E_RetRamIdx_t
* RETURNS
*   0: setting complete
*   1: error 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Sys_RetRamTurnOff(uint32_t u32RetRamIdxs )
{
    // Check Indexes are all in legal region
    if( u32RetRamIdxs & (~RET_RAM_BIT_Msk) )
        return 1;
    
    /* Make sure in FW control */
    AOS->SRAM_PSD    = (AOS->SRAM_PSD | (u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos)) & ~u32RetRamIdxs;
    AOS->SRAM_ISO_EN = (AOS->SRAM_ISO_EN | (u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos)) & ~u32RetRamIdxs;
    
    /* Turn OFF */
    AOS->SRAM_PSD    |= u32RetRamIdxs;
    AOS->SRAM_ISO_EN |= u32RetRamIdxs;
    
    /* Wait turn off done */
    while ((AOS->SRAM_RET_ACK & u32RetRamIdxs) != 0);

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_RetRamTurnOn
*
* DESCRIPTION:
*   1. Turn on the given RetRam blocks. Ref. to E_RetRamIdx_t
*   2. Related reg.: AOS 0x0E0 ,0x0E4 and 0x118
* CALLS
*
* PARAMETERS
*   1. u32RetRamIdxs : Multi index combined. index refer to E_RetRamIdx_t
* RETURNS
*   0: setting complete
*   1: error 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Sys_RetRamTurnOn_impl(uint32_t u32RetRamIdxs)
{
    // Check Indexes are all in legal region
    if( u32RetRamIdxs & (~RET_RAM_BIT_Msk) )
        return 1;
    
    AOS->SRAM_PSD    = (AOS->SRAM_PSD & ~u32RetRamIdxs) | (u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos);
    while ((AOS->SRAM_RET_ACK & u32RetRamIdxs) != u32RetRamIdxs);
    AOS->SRAM_ISO_EN = (AOS->SRAM_ISO_EN & ~u32RetRamIdxs) | (u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos);

    return 0;
}


/*************************************************************************
* FUNCTION:
*  Hal_Sys_RetRamCtrlBySeq
*
* DESCRIPTION:
*   1. Set Ram control to SEQ
*   2. Related reg.: AOS 0x0E0 and 0x0E4
*
* CALLS
*
* PARAMETERS
*   1. u32RetRamIdxs : Multi index combined. index refer to E_RetRamIdxPatch_t
* RETURNS
*   0: setting complete
*   1: error 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Sys_RetRamCtrlBySeq_impl(uint32_t u32RetRamIdxs)
{
    u32RetRamIdxs &= RET_RAM_BIT_Msk;
    
    AOS->SRAM_ISO_EN &= ~(u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos);
    AOS->SRAM_PSD    &= ~(u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos);

    return 0;
}

/*************************************************************************
* FUNCTION:
*   Hal_SysSetSramUnusedRange
*
* DESCRIPTION:
*   Set SRAM unused range, update SRAM usage table
*   Only support one continued range
*
* PARAMETERS
*   u32StartAddr: The start address of taget range
*   u32EndAddr: The end address of target range
* RETURNS
*   none
*
*************************************************************************/
void Hal_SysSetSramUnusedRange_impl(uint32_t u32StartAddr, uint32_t u32EndAddr)
{
    int i;
    
    if (u32StartAddr > u32EndAddr)
    {   /* Swap if order not correct */
        uint32_t u32Temp = u32StartAddr;
        u32StartAddr = u32EndAddr;
        u32EndAddr = u32Temp;
    }
    
    for (i = 0; i < sizeof(g_u32aSramMapping) / sizeof(g_u32aSramMapping[0]); i++)
    {
        /*
         * Sa: u32StartAddr
         * Ea: u32EndAddr
         * S : g_u32aSramMapping[i].u32RamStart
         * E : g_u32aSramMapping[i].u32RamEnd
         *
         *                          S           E
         * Ram Mapping Domain       |-----------|
         *                 ^Sa ^Ea                                           |
         *                     ^Sa  ^Ea                   Sa <= Ea <= S < E  | Not changed, break
         *                     ----------------------------------------------+---------------------
         *                     ^Sa              ^Ea                          |
         *                     ^Sa                  ^Ea                      |
         *                          ^Sa         ^Ea                          |
         *                          ^Sa             ^Ea   Sa <= S < E <= Ea  | Tag unused
         *                     ----------------------------------------------+---------------------
         *                     ^Sa      ^Ea                                  |
         *                          ^Sa ^Ea                                  |
         *                              ^Sa ^Ea                              |
         *                              ^Sa     ^Ea                          |
         *                              ^Sa         ^Ea                      |
         *                                      ^Sa ^Ea                      |
         *                                          ^Sa ^Ea   OTHER CASE     | Not changed
         */
        if (u32EndAddr <= g_u32aSramMapping[i].u32RamStart)
        {   /* Ea <= S */
            break;
        }
        if ((u32StartAddr <= g_u32aSramMapping[i].u32RamStart) && (u32EndAddr >= g_u32aSramMapping[i].u32RamEnd))
        {   /* Sa <= S < E <= Ea */
            g_u32SramUsageBmp &= ~(1 << i);
        }
    }
}


// TODO: XTAL CAL

/*************************************************************************
* FUNCTION:
*  Hal_Sys_MsqRomRemapAll
*
* DESCRIPTION:
*   1. Enable/disable remap all MSQ ROM
*   2. Related reg.: AOS 0x11C
* CALLS
*
* PARAMETERS
*   1. u8Enable : 1 for enable/ 0 for disable
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_MsqRomRemapAll_impl(uint8_t u8Enable)
{
    if(u8Enable)
        AOS->MSQ_ROM_MAP |= AOS_MSQ_ROM_MAP_ENABLE;
    else
        AOS->MSQ_ROM_MAP &= ~AOS_MSQ_ROM_MAP_ENABLE;
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_SpareRegRead
*
* DESCRIPTION:
*   1. Read spare reg. refer to E_SpareRegIdx_t
*   2. Related reg.: AOS 0x10C, 0x124, 0x12C and 0x130
* CALLS
*
* PARAMETERS
*   1. eSpareIdx : each bit for the reset sources. Refer to E_SpareRegIdx_t
*   2. pu32Data  : Addr store read data
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_SpareRegRead_impl(E_SpareRegIdx_t eSpareIdx, uint32_t *pu32Data)
{
#if defined(OPL2500_APS)
    switch(eSpareIdx)
    {
        case SPARE_0:
        case SPARE_2:
        case SPARE_3:
            *pu32Data = reg_read(AOS_BASE + eSpareIdx);
            break;
        default:
            return 1;
    }
    return 0;
#elif defined(OPL2500_MSQ)
    *pu32Data = reg_read(AOS_BASE + eSpareIdx);
    return 1;
#endif /* OPL2500_APS elif OPL2500_MSQ */
}

/*************************************************************************
* FUNCTION:
*  Hal_Sys_SpareRegWrite
*
* DESCRIPTION:
*   1. Wrtie spare reg. refer to E_SpareRegIdx_t
*   2. Related reg.: AOS 0x124, 0x12C and 0x130
* CALLS
*
* PARAMETERS
*   1. eSpareIdx : each bit for the reset sources. Refer to E_SpareRegIdx_t
*   2. u32Data   : Data to wrtie
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_SpareRegWrite_impl(E_SpareRegIdx_t eSpareIdx, uint32_t u32Data)
{
#if defined(OPL2500_APS)
    switch(eSpareIdx)
    {
        case SPARE_0:
        case SPARE_2:
        case SPARE_3:
            reg_write(AOS_BASE + eSpareIdx, u32Data);
            break;
        default:
            return 1;
    }
    return 0;
#elif defined(OPL2500_MSQ)
    reg_write(AOS_BASE + eSpareIdx, u32Data);
    return 1;
#endif /* OPL2500_APS elif OPL2500_MSQ */
}


/*************************************************************************
* FUNCTION:
*  Hal_Sys_SpareRegWait
*
* DESCRIPTION:
*   1. Wrtie spare reg. refer to E_SpareRegIdx_t
*   2. Related reg.: AOS 0x124, 0x12C and 0x130
* CALLS
*
* PARAMETERS
*   1. eSpareIdx : each bit for the reset sources. Refer to E_SpareRegIdx_t
*   2. u32Mask   : Wait mask bits, wait until all bits to one
*   3. bClear    : After wait done, if to clear the bits. 1: To clear, 0: Not to clear
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_SpareRegWait_impl(E_SpareRegIdx_t eSpareIdx, uint32_t u32Mask, uint8_t bClear)
{
    volatile uint32_t *pu32Reg;
    
    /* No bits to be checked */
    if (!u32Mask)
        return 1;
    
#if defined(OPL2500_APS)    
    switch(eSpareIdx)
    {
        case SPARE_0:
        case SPARE_2:
        case SPARE_3:
            pu32Reg = (volatile uint32_t *)(AOS_BASE + eSpareIdx);
            break;
        default:
            return 1;
    }
#elif defined(OPL2500_MSQ)
    pu32Reg = (volatile uint32_t *)(AOS_BASE + eSpareIdx);
#endif /* OPL2500_APS elif OPL2500_MSQ */
    
    while (1)
    {
        if ((*pu32Reg & u32Mask) == u32Mask)
            break;
    }
    if (bClear)
    {
        *pu32Reg &= ~u32Mask;
    }
    
    return 0;
}


/*************************************************************************
* FUNCTION:
*  Hal_Sys_SpareRegWriteBit
*
* DESCRIPTION:
*   1. Wrtie spare reg. refer to E_SpareRegIdx_t
*   2. Related reg.: AOS 0x124, 0x12C and 0x130
* CALLS
*
* PARAMETERS
*   1. eSpareIdx : each bit for the reset sources. Refer to E_SpareRegIdx_t
*   2. u32Mask   : Bit to be set
*   3. u32Value  : Value to be set to the mask bits
* RETURNS
*   0: setting complete
*   1: error
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Sys_SpareRegWriteBit_impl(E_SpareRegIdx_t eSpareIdx, uint32_t u32Mask, uint32_t u32Value)
{
    volatile uint32_t *pu32Reg;
    
    /* No bits to be checked */
    if (!u32Mask)
        return 1;
    
    #if defined(OPL2500_APS)    
    switch(eSpareIdx)
    {
        case SPARE_0:
        case SPARE_2:
        case SPARE_3:
            pu32Reg = (volatile uint32_t *)(AOS_BASE + eSpareIdx);
            break;
        default:
            return 1;
    }
#elif defined(OPL2500_MSQ)
    pu32Reg = (volatile uint32_t *)(AOS_BASE + eSpareIdx);
#endif /* OPL2500_APS elif OPL2500_MSQ */
    
    *pu32Reg = (*pu32Reg & ~u32Mask) | (u32Value & u32Mask);
    
    return 0;
}


/*************************************************************************
* FUNCTION:
*  Hal_Sys_OtpRead_impl
*
* DESCRIPTION:
*   1. Get OTP data address and copy data to buffer
*
* PARAMETERS
*   u16Offset : offset of OTP data
*   u8aBuf    : buffer to read data
*   u16BufSize: size of buffer
* RETURNS
*   Non-NULL: setting complete
*   NULL: errror
* GLOBALS AFFECTED
*
*************************************************************************/
uint8_t *Hal_Sys_OtpRead_impl(uint16_t u16Offset, uint8_t *u8aBuf, uint16_t u16BufSize)
{
    uint8_t *pu8Data = NULL;
    uint16_t u16Total = sizeof(OTP->DATA);

    if(u16Offset >= u16Total)
    {
        goto done;
    }

    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_OTP);

    pu8Data = (uint8_t *)(OTP->DATA);
    pu8Data += u16Offset;

    if(u8aBuf && u16BufSize)
    {
        uint16_t u16DataSize = u16BufSize;

        if(u16Offset + u16BufSize > u16Total)
        {
            u16DataSize = u16Total - u16Offset;
        }

        memcpy(u8aBuf, pu8Data, u16DataSize);
    }

done:
    return pu8Data;
}


/*************************************************************************
* FUNCTION:
*  Hal_Sys_RomVerInit
*
* DESCRIPTION:
*   1. Fill ROM version information
*
* PARAMETERS
*   u32InfoAddr : Sw info address in IPC
* RETURNS
*
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Sys_RomVerInit(uint32_t u32VerInfoAddr)
{
    S_SW_VERSION_INFO *pVerInfo = (S_SW_VERSION_INFO *)u32VerInfoAddr;
    char *szBuildTime = SVN_TIME_NOW;
    
    pVerInfo->sApsRom.u32Revision = SVN_REVISION;
    strcpy(pVerInfo->sApsRom.szBuildTime, szBuildTime);
    pVerInfo->sApsPatch.u32Revision = 0;
    memset(pVerInfo->sApsPatch.szBuildTime, 0, sizeof(pVerInfo->sApsPatch.szBuildTime));
}
