/******************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *******************************************************************************
 * @file hal_sys_rcc.c
 * 
 * @brief API to Reset and Clock Control module
 * 
 */

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "opulinks.h"
#include "hal_sys_rcc.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_i2c.h"
#include "hal_wdt.h"
#include "hal_spi.h"
#include "hal_qspi.h"
#include "hal_uart.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/* Check XTAL ready */
#define CHECK_XTAL_COUNT            220                         /* 10 us */
#define CHECK_XTAL_TOLERANCE        (CHECK_XTAL_COUNT * 0.1)
#define CHECK_XTAL_SUCCESS_COUNT    2

#define OSC_CAL_CTUNE_WAIT_TICKS    100
#define OSC_CAL_CEN_WAIT_TICKS      10000
#define RC_CAL_START_POINT          0x80

#define RC_GT_XTAL                  ((int32_t)1)
#define RC_EQ_XTAL                  ((int32_t)0)
#define RC_LT_XTAL                  ((int32_t)-1)
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

/* Setup function */
T_Hal_Sys_ClkInit                   Hal_Sys_ClkInit             = Hal_Sys_ClkInit_impl;
T_Hal_Sys_RegBitDefSetup            Hal_Sys_RegBitDefSetup      = Hal_Sys_RegBitDefSetup_impl;
    
/* Module reset */
T_Hal_Sys_SwResetAll                Hal_Sys_SwResetAll          = Hal_Sys_SwResetAll_impl;
T_Hal_Sys_ApsModuleRst              Hal_Sys_ApsModuleRst        = Hal_Sys_ApsModuleRst_impl;
T_Hal_Sys_MsqModuleRst              Hal_Sys_MsqModuleRst        = Hal_Sys_MsqModuleRst_impl;

/* Module enable */
T_Hal_Sys_ApsModuleClkEn            Hal_Sys_ApsModuleClkEn      = Hal_Sys_ApsModuleClkEn_impl;
T_Hal_Sys_MsqModuleClkEn            Hal_Sys_MsqModuleClkEn      = Hal_Sys_MsqModuleClkEn_impl;
T_Hal_Sys_UnusedModuleDis           Hal_Sys_UnusedModuleDis     = Hal_Sys_UnusedModuleDis_impl;

/* Source clock */
T_Hal_Sys_RtcClkCfg                 Hal_Sys_RtcClkCfg           = Hal_Sys_RtcClkCfg_impl;
T_Hal_Sys_RcHighEn                  Hal_Sys_RcHighEn            = Hal_Sys_RcHighEn_impl;
T_Hal_Sys_XtalHighEn                Hal_Sys_XtalHighEn          = Hal_Sys_XtalHighEn_impl;
T_Hal_Sys_FromRcHighCheck           Hal_Sys_FromRcHighCheck     = Hal_Sys_FromRcHighCheck_impl;
T_Hal_Sys_FromXtalHighCheck         Hal_Sys_FromXtalHighCheck   = Hal_Sys_FromXtalHighCheck_impl;
T_Hal_Sys_RcCal                     Hal_Sys_RcCal               = Hal_Sys_RcCal_impl;
T_Hal_Sys_RcCalCompare              Hal_Sys_RcCalCompare        = Hal_Sys_RcCalCompare_impl;
T_Hal_Sys_CheckXtalReady            Hal_Sys_CheckXtalReady      = Hal_Sys_CheckXtalReady_impl;

/* System Clock */
T_Hal_Sys_SleepClkSetup             Hal_Sys_SleepClkSetup       = Hal_Sys_SleepClkSetup_impl;
T_Hal_Sys_WakeupClkResume           Hal_Sys_WakeupClkResume     = Hal_Sys_WakeupClkResume_impl;
T_Hal_Sys_ApsClkTreeSetup           Hal_Sys_ApsClkTreeSetup     = Hal_Sys_ApsClkTreeSetup_impl;
T_Hal_Sys_ClkChangeApply            Hal_Sys_ApsClkChangeApply   = Hal_Sys_ApsClkChangeApply_impl;
T_Hal_Sys_ApsClockUpdate            Hal_Sys_ApsClockUpdate      = Hal_Sys_ApsClockUpdate_impl;
T_Hal_Sys_ApsClockGet               Hal_Sys_ApsClockGet         = Hal_Sys_ApsClockGet_impl;
T_Hal_Sys_SwitchShelterSet          Hal_Sys_SwitchShelterSet    = Hal_Sys_SwitchShelterSet_impl;
    
/* Peripheral clock */
T_Hal_Sys_PwmSrcSelect              Hal_Sys_PwmSrcSelect        = Hal_Sys_PwmSrcSelect_impl;
T_Hal_Sys_ScrtSrcSelect             Hal_Sys_ScrtSrcSelect       = Hal_Sys_ScrtSrcSelect_impl;
T_Hal_Sys_StDivSelect               Hal_Sys_StDivSelect         = Hal_Sys_StDivSelect_impl;
T_Hal_Sys_Spi0SrcSelect             Hal_Sys_Spi0SrcSelect       = Hal_Sys_Spi0SrcSelect_impl;
T_Hal_Sys_Spi1SrcSelect             Hal_Sys_Spi1SrcSelect       = Hal_Sys_Spi1SrcSelect_impl;
T_Hal_Sys_Spi2SrcSelect             Hal_Sys_Spi2SrcSelect       = Hal_Sys_Spi2SrcSelect_impl;
T_Hal_Sys_Spi3SrcSelect             Hal_Sys_Spi3SrcSelect       = Hal_Sys_Spi3SrcSelect_impl;
T_Hal_Sys_I2sSrcSelect              Hal_Sys_I2sSrcSelect        = Hal_Sys_I2sSrcSelect_impl;
T_Hal_Sys_Uart0SrcSelect            Hal_Sys_Uart0SrcSelect      = Hal_Sys_Uart0SrcSelect_impl;
T_Hal_Sys_Uart1SrcSelect            Hal_Sys_Uart1SrcSelect      = Hal_Sys_Uart1SrcSelect_impl;
T_Hal_Sys_ApsUartSrcSelect          Hal_Sys_ApsUartSrcSelect    = Hal_Sys_ApsUartSrcSelect_impl;
T_Hal_Sys_UartAbrSrcSelect          Hal_Sys_UartAbrSrcSelect    = Hal_Sys_UartAbrSrcSelect_impl;
T_Hal_Sys_PdmSrcSelect              Hal_Sys_PdmSrcSelect        = Hal_Sys_PdmSrcSelect_impl;
T_Hal_Sys_Ext2IoSetup               Hal_Sys_Ext2IoSetup         = Hal_Sys_Ext2IoSetup_impl;
T_Hal_Sys_CamXvClkSrcSelect         Hal_Sys_CamXvClkSrcSelect   = Hal_Sys_CamXvClkSrcSelect_impl;

/* APS clock modulator */
T_Hal_Sys_ApsClkMmFactorSet         Hal_Sys_ApsClkMmFactorSet   = Hal_Sys_ApsClkMmFactorSet_impl;
T_Hal_Sys_ApsClkMmFactorGet         Hal_Sys_ApsClkMmFactorGet   = Hal_Sys_ApsClkMmFactorGet_impl;



S_APS_CLK_FREQ g_sApsClkFreq;


/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */


/* ******************************* Setup function ******************************* */
/**
 * @brief Hal_Sys_ClkInit
 *        Init Clock frequency
 */
void Hal_Sys_ClkInit_impl(uint32_t u32XtalFreq)
{
    uint32_t u32DmClkReg = AOS->APS_DM_CLK_SEL;
    
    HAL_SYS_SET_XTAL_FREQ(u32XtalFreq);   
    
    g_sApsClkFreq.u32MMFactor   = APS_MMFACTOR_DEFAULT;
    g_sApsClkFreq.u32ExtApsClk  = CLK_SRC_FREQ_EXT_APS_CLK_DEF;
    g_sApsClkFreq.u32XtalClk    = u32XtalFreq;
    g_sApsClkFreq.u32SysClk     = CLK_SRC_FREQ_RC;
    g_sApsClkFreq.u32SysSrcClk  = CLK_SRC_FREQ_RC;
    g_sApsClkFreq.u32Pclk       = CLK_SRC_FREQ_RC;
    g_sApsClkFreq.u32PwmClk     = CLK_SRC_FREQ_32K_RC;
    g_sApsClkFreq.u32ScrtClk    = CLK_SRC_FREQ_RC;
    g_sApsClkFreq.u32Spi0Clk    = CLK_SRC_FREQ_RC;
    g_sApsClkFreq.u32Spi1Clk    = CLK_SRC_FREQ_RC;
    g_sApsClkFreq.u32Spi2Clk    = CLK_SRC_FREQ_RC;
    g_sApsClkFreq.u32Spi3Clk    = CLK_SRC_FREQ_RC;
    g_sApsClkFreq.u32I2sClk     = CLK_SRC_FREQ_160M_BB/APS_CLK_I2S_DIVN_GET(u32DmClkReg);
    g_sApsClkFreq.u32Uart0Clk   = g_sApsClkFreq.u32XtalClk;
    g_sApsClkFreq.u32Uart1Clk   = g_sApsClkFreq.u32XtalClk;
    g_sApsClkFreq.u32ApsUartClk = g_sApsClkFreq.u32XtalClk;
    g_sApsClkFreq.u32UartAbrClk = g_sApsClkFreq.u32XtalClk;
    g_sApsClkFreq.u32PdmIoClk   = CLK_SRC_FREQ_160M_BB/APS_CLK_PDM_IO_DIVN_GET(u32DmClkReg);
    g_sApsClkFreq.u32PdmClk     = CLK_SRC_FREQ_160M_BB/APS_CLK_PDM_DIVN_GET(u32DmClkReg);
    g_sApsClkFreq.u32Ext2IoClk  = g_sApsClkFreq.u32XtalClk/APS_CLK_EXT2IO_DIVN_GET(AOS->APS_DOMAIN_EN);
    g_sApsClkFreq.u32Tmr0ExtClk = g_sApsClkFreq.u32XtalClk/APS_CLK_TMR_CLK_DIV;
    g_sApsClkFreq.u32Tmr1ExtClk = g_sApsClkFreq.u32XtalClk/APS_CLK_TMR_CLK_DIV;
    g_sApsClkFreq.u32StClk      = g_sApsClkFreq.u32XtalClk/APS_CLK_ST_DIVN_GET(AOS->APS_STCLK);
    g_sApsClkFreq.u32CamXvClk   = g_sApsClkFreq.u32XtalClk;
    SystemCoreClockSet(u32XtalFreq);  
}

/**
 * @brief Hal_Sys_RegBitDefSetup
 *        Setup register and bit by pre-defined setting
 * @param eFlag[in] ::SET to set bits or ::RESET to reset bits
 * @param u32Def[in] The register and bit definition. Format as ::S_REG_BIT_DEF
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_RegBitDefSetup_impl(E_FLAG_STS eFlag, uint32_t u32Def)
{
    S_REG_BIT_DEF *psRegBit = (S_REG_BIT_DEF *)&u32Def;
    volatile uint32_t *pReg;
    uint32_t u32Mask;
    
    if (!psRegBit->u32ValidDef)
        return RESULT_FAIL;
    
    if (psRegBit->u32SetType == REG_BDEF_TYPE_ONE_REG)
    {
        pReg = (volatile uint32_t *)(AOS_BASE + psRegBit->RegOfst);
        u32Mask = 1 << psRegBit->BitPos0 |
                  1 << psRegBit->BitPos1 |
                  1 << psRegBit->BitPos2;
        if(eFlag == SET)
            *pReg |= u32Mask;
        else
            *pReg &= ~u32Mask;
    }
    else /* REG_BDEF_TYPE_TWO_REG */
    {
        pReg = (volatile uint32_t *)(AOS_BASE + psRegBit->RegOfst0);
        u32Mask = 1 << psRegBit->BitOfst0;
        if(eFlag == SET)
            *pReg |= u32Mask;
        else
            *pReg &= ~u32Mask;
        
        pReg = (volatile uint32_t *)(AOS_BASE + psRegBit->RegOfst1);
        u32Mask = 1 << psRegBit->BitOfst1;
        if(eFlag == SET)
            *pReg |= u32Mask;
        else
            *pReg &= ~u32Mask;
    }
    return RESULT_SUCCESS;
}


/* ******************************* Module reset ********************************* */
/**
 * @brief Hal_Sys_SwResetAll
 *        Reset whole chip
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_SwResetAll_impl(void)
{
    SYS->SW_RESET_EN |= SYS_SW_RESET_EN_SWRST_EN;
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_ApsModuleRst
 *        Reset APS peripheral.
 * @param eModule[in] ::E_ApsRstModule_t Reset APS module. One module once
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_ApsModuleRst_impl(E_ApsRstModule_t eModule)
{
    uint32_t u32Timeout = RCC_SWRST_STATUS_TIMEOUT;
    AOS->APS_SWRST = eModule;

    /* Bit value 1 means reset done, wait all module reset done */
    while ((AOS->APS_SWRST_ST & eModule) != eModule)
    {
        if (--u32Timeout == 0)
            break;
    }
    
	return (u32Timeout == 0)?RESULT_FAIL:RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_MsqModuleRst
 *        Reset MSQ peripherals
 * @param eModule[in] ::E_MsqRstModule_t Reset APS module
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_MsqModuleRst_impl(E_MsqRstModule_t eModule)
{
    uint32_t u32Timeout = RCC_SWRST_STATUS_TIMEOUT;
    AOS->MSQ_SWRST = eModule;
    
    /* Bit value 1 means reset done, wait all module reset done */
    while ((AOS->MSQ_SWRST_ST & eModule) != eModule)
    {
        if (--u32Timeout == 0)
            break;
    }

	return (u32Timeout == 0)?RESULT_FAIL:RESULT_SUCCESS;
}


/* ******************************* Module enable ******************************** */
/**
 * @brief Hal_Sys_ApsModuleClkEn
 *        APS module clock enable/disable.
 * @param u8Enable[in] ::ENABLE or ::DISABLE the module
 *                     NOT support OR multi module definition.
 * @param eModule[in] The APS module. Refer to ::E_ApsClkEnModule_t
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_ApsModuleClkEn_impl(uint8_t u8Enable, E_ApsClkEnModule_t eModule)
{
    if (eModule == APS_CLK_ALL)
    {
        if (u8Enable == ENABLE)
        {
            AOS->APS_CLK_SEL |= AOS_APS_CLK_SEL_ALL_APS;
            AOS->APS_DOMAIN_EN |= AOS_APS_DOMAIN_EN_ALL_APS;
            AOS->SPI_CLK_SEL |= AOS_SPI_CLK_SEL_ALL_APS;
            AOS->APS_DM_CLK_SEL |= AOS_APS_DM_CLK_SEL_ALL_APS;
            AOS->APS_STCLK |= AOS_APS_STCLK_ALL_APS;
            AOS->SCRT_CLK_APS |= AOS_SCRT_CLK_APS_SEL_ALL_APS;
            AOS->CAM_XVCLK_SEL |= AOS_CAM_XVCLK_CLK_APS;
        }
        else
        {
            AOS->APS_CLK_SEL &= ~AOS_APS_CLK_SEL_ALL_APS;
            AOS->APS_DOMAIN_EN &= ~AOS_APS_DOMAIN_EN_ALL_APS;
            AOS->SPI_CLK_SEL &= ~AOS_SPI_CLK_SEL_ALL_APS;
            AOS->APS_DM_CLK_SEL &= ~AOS_APS_DM_CLK_SEL_ALL_APS;
            AOS->APS_STCLK &= ~AOS_APS_STCLK_ALL_APS;
            AOS->SCRT_CLK_APS &= ~AOS_SCRT_CLK_APS_SEL_ALL_APS;
            AOS->CAM_XVCLK_SEL &= ~AOS_CAM_XVCLK_CLK_APS;
        }
        return RESULT_SUCCESS;
    } 
    
    return Hal_Sys_RegBitDefSetup((u8Enable==ENABLE)?SET:RESET, eModule);
}

/**
 * @brief Hal_Sys_MsqModuleClkEn
 *        MSQ module clock enable/disable.
 * @param u8Enable[in] ::ENABLE or ::DISABLE the module
 *                     Support OR multi module definition.
 * @param eModule[in] The MSQ module. Refer to ::E_MsqClkEnModule_t *
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_MsqModuleClkEn_impl(uint8_t u8Enable, E_MsqClkEnModule_t eModule)
{
    if (u8Enable == ENABLE)
    {
        AOS->MSQ_CLK_SEL |= eModule;
    }
    else
    {
        AOS->MSQ_CLK_SEL &= ~eModule;
    }
    
    return RESULT_SUCCESS;
}

/**
 * @brief To gate perpherials clock in system initialization.
 *        When enable perpherial, API needs to turn on the clock.
 *
 * Peripherals:
 *    [APS_CLK_SEL]
 *      - I2C
 *      - TMR 0/1
 *      - WDT
 *      - UART 0/1
 *      - I2S
 *      - DbgUart
 *      - Jtag
 *    [APS_DOMAIN_EN]
 *      - ABR
 *      - Pwm
 *      - EXT 2 IO
 *    [SCRT_CLK_APS]
 *      - SCRT global
 *    [SPI_CLK_SEL]
 *      - SPI 0/1/2/3
 *    [APS_DM_CLK_SEL]
 *      - I2S
 *      - XIP Cache, Prefetch
 *      - PDM
 *    [CAM_XVCLK_SEL]
 *      - XVCLK
 */
void Hal_Sys_UnusedModuleDis_impl(void)
{
    uint32_t u32DisApsClk;
    
    u32DisApsClk = AOS_APS_CLK_SEL_ALL_APS & ~AOS_APS_CLK_SEL_APS_UART_PCLK_EN;
    
    if (Hal_Sys_StrapModeRead() != STRAP_MODE_NORMAL)
        u32DisApsClk &= ~AOS_APS_CLK_SEL_JTAG_HCLK_EN;


    AOS->APS_CLK_SEL &= ~u32DisApsClk;

    AOS->SCRT_CLK_APS &= ~AOS_SCRT_CLK_APS_SEL_ALL_APS;    
    AOS->APS_DOMAIN_EN &= ~AOS_APS_DOMAIN_EN_ALL_APS;
    AOS->SPI_CLK_SEL &= ~AOS_SPI_CLK_SEL_ALL_APS;
    AOS->APS_DM_CLK_SEL &= ~AOS_APS_DM_CLK_SEL_ALL_APS;

    AOS->CAM_XVCLK_SEL &= ~AOS_CAM_XVCLK_CLK_APS;
}


/* ******************************* Source clock ********************************* */

/**
 * @brief Hal_Sys_RtcClkCfg
 *        Configure RTC clock and it source
 * @param psCfg[in] The RTC source configuration
 *      psCfg->eRtcSrc (E_MsqClkRtc_t)
 *              MSQ_CLK_RTC_OFF                 - Disable RTC timer
 *              MSQ_CLK_RTC_ON_LPO              - Select LPO (32k RC) as RTC source. (Reset default)
 *                                                Needs to turn on LPO in psRtcCfg->eLpoCtrl
 *              MSQ_CLK_RTC_ON_32K_XTAL         - Select 32k XTAL as RTC source.
 *                                                Needs to turn on 32k XTAL in psRtcCfg->eXtal32kCtrl 
 *      psCfg->eLpoCtrl (E_ClkSrcLpo_t)
 *              CLK_LPO_CTRL_BY_SEQ - Controlled by SEQ
 *              CLK_LPO_CTRL_OFF    - Controlled by FW, turn off LPO
 *              CLK_LPO_CTRL_ON     - Controlled by FW, turn on LPO
 *      psCfg->eXtal32kCtrl (E_ClkSrcXtal32k_t)
 *              XTAL_32k_BY_SEQ     - Controlled by SEQ
 *              XTAL_32k_OFF        - Controlled by FW, turn off 32k XTAL
 *              XTAL_32k_ON         - Controlled by FW, turn on 32k XTAL
 *              XTAL_32k_BYPS_ON    - Controlled by FW, turn on 32k XTAL and set as bypass
 *      psCfg->u8XtalGm Gain for 32k XTAL. Maximum is 0xF
 *      psCfg->u8XtalCap Capatior for 32k XTAL. Maximum is 0xF
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_RtcClkCfg_impl(S_RTC_CLK_CFG *psCfg)
{
    /* Disable RTC timer first */
    AOS->OSC_SEL &= ~MSQ_CLK_RTC_MSK;
    
    /* Setup Source and other config */
    AOS->OSC_CTL = (AOS->OSC_CTL & ~(CLK_LPO_CTRL_MSK | XTAL_32k_MSK | AOS_OSC_CTL_32K_XTAL_GM_Msk | AOS_OSC_CTL_32K_XTAL_CAP_Msk)) |
                    psCfg->eLpoCtrl | psCfg->eXtal32kCtrl | 
                    ((psCfg->u8XtalGm << AOS_OSC_CTL_32K_XTAL_GM_Pos) & AOS_OSC_CTL_32K_XTAL_GM_Msk) |
                    ((psCfg->u8XtalCap << AOS_OSC_CTL_32K_XTAL_CAP_Pos) & AOS_OSC_CTL_32K_XTAL_CAP_Msk);
    
    AOS->OSC_SEL |= psCfg->eRtcSrc;
    
    return RESULT_SUCCESS;
}
/**
 * @brief Hal_Sys_RcHighEn
 *        High speed RC enable
 * @param eEn[in] To configure the high speed RC enable mode
 *              RC_HIGH_OFF 
 *              RC_HIGH_ON
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_RcHighEn_impl(E_ClkSrcRcHigh eEn)
{
    #if !defined(OPL2500_FPGA)
    if (eEn == RC_HIGH_ON)
    {
        RF->CLKGEN_CTL0 |= RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB;
        RF->PU_VAL |= RF_PU_VAL_PU_RC_OSC;
    }
    else
    {
        RF->CLKGEN_CTL0 &= ~RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB;
        RF->PU_VAL &= ~RF_PU_VAL_PU_RC_OSC;
    }
    #endif
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_XtalHighEn
 *        High speed XTAL enable
 * @param eEn[in] To configure the XTAL enable mode
 *              XTAL_HIGH_OFF
 *              XTAL_HIGH_ON
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_XtalHighEn_impl(E_ClkSrcXtalHigh eEn)
{
    #if !defined(OPL2500_FPGA)
    if (eEn == XTAL_HIGH_ON)
    {
        RF->CLKGEN_CTL0 |= RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB;
        RF->PU_VAL |= RF_PU_VAL_PU_XTAL;
    }
    else
    {
        RF->CLKGEN_CTL0 &= ~RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB;
        RF->PU_VAL &= ~RF_PU_VAL_PU_XTAL;
    }
    #endif
    return RESULT_SUCCESS;
}


/**
 * @brief Hal_Sys_FromRcHighCheck
 *        Check which module is from high speed RC and enabled
 * @return Module bit map.
 *         Bit order refer to E_ModuleFromRcHigh_t
 */
uint32_t Hal_Sys_FromRcHighCheck_impl(void)
{
    uint32_t u32ModuleBmp = 0;
    
    /* APS SYSCLK */
    if ( ((AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_APS_SRC_CLK_SEL_Msk) >> AOS_APS_CLK_SEL_APS_SRC_CLK_SEL_Pos) == APS_CLK_SYS_SRC_RC)
    {   /* APS SYSCLK from RC */
        u32ModuleBmp |= 1 << FROM_RC_HIGH_APS_SYSCLK;
    }
    
    /* SCRT */
    if ( ((AOS->SCRT_CLK_APS & AOS_SCRT_CLK_APS_SCRT_SRC_CLKSEL_Msk) >> AOS_SCRT_CLK_APS_SCRT_SRC_CLKSEL_Pos) == ASP_CLK_SCRT_SRC_RC)
    {   /* Security from RC */
        if ((AOS->SCRT_CLK_APS & AOS_SCRT_CLK_APS_SCRT_GLB_CLK_APS_EN) || (AOS->SCRT_CLK_MSQ & AOS_SCRT_CLK_MSQ_SCRT_GLB_CLK_MSQ_EN))
        {   /* SCRT is enabled */
            u32ModuleBmp |= 1 << FROM_RC_HIGH_SCRT;
        }
    }
    
    /* SPI0 */
    if ( ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI0_CLKSEL_Msk) >> AOS_SPI_CLK_SEL_SPI0_CLKSEL_Pos) == APS_CLK_SPI0_SRC_RC)
    {   /* SPI0 clock from RC */
        if ( (AOS->SPI_CLK_SEL & (AOS_SPI_CLK_SEL_SPI0_HCLK_EN | AOS_SPI_CLK_SEL_SPI0_PCLK_EN | AOS_SPI_CLK_SEL_SPI0_REFCLK_EN)) == (AOS_SPI_CLK_SEL_SPI0_HCLK_EN | AOS_SPI_CLK_SEL_SPI0_PCLK_EN | AOS_SPI_CLK_SEL_SPI0_REFCLK_EN))
        {   /* SPI0 is enabled */
            u32ModuleBmp |= 1 << FROM_RC_HIGH_SPI0;
        }
    }
    
    /* SPI1 */
    if ( ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI1_CLK_SEL_Msk) >> AOS_SPI_CLK_SEL_SPI1_CLK_SEL_Pos) == APS_CLK_SPI1_SRC_RC)
    {   /* SPI1 clock from RC */
        if ( (AOS->SPI_CLK_SEL & (AOS_SPI_CLK_SEL_SPI1_PCLK_EN | AOS_SPI_CLK_SEL_SPI1_SCLK_EN)) == (AOS_SPI_CLK_SEL_SPI1_PCLK_EN | AOS_SPI_CLK_SEL_SPI1_SCLK_EN))
        {   /* SPI1 is enabled */
            u32ModuleBmp |= 1 << FROM_RC_HIGH_SPI1;
        }
    }
    
    /* SPI2 */
    if ( ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI2_CLK_SEL_Msk) >> AOS_SPI_CLK_SEL_SPI2_CLK_SEL_Pos) == APS_CLK_SPI2_SRC_RC)
    {   /* SPI2 clock from RC */
        if ( (AOS->SPI_CLK_SEL & (AOS_SPI_CLK_SEL_SPI2_PCLK_EN | AOS_SPI_CLK_SEL_SPI2_SCLK_EN)) == (AOS_SPI_CLK_SEL_SPI2_PCLK_EN | AOS_SPI_CLK_SEL_SPI2_SCLK_EN))
        {   /* SPI2 is enabled */
            u32ModuleBmp |= 1 << FROM_RC_HIGH_SPI2;
        }
    }
        
    /* SPI3 */
    if ( ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI3_CLK_SEL_Msk) >> AOS_SPI_CLK_SEL_SPI3_CLK_SEL_Pos) == APS_CLK_SPI3_SRC_RC)
    {   /* SPI3 clock from RC */
        if ( (AOS->SPI_CLK_SEL & (AOS_SPI_CLK_SEL_SPI3_PCLK_EN | AOS_SPI_CLK_SEL_SPI3_SCLK_EN)) == (AOS_SPI_CLK_SEL_SPI3_PCLK_EN | AOS_SPI_CLK_SEL_SPI3_SCLK_EN))
        {   /* SPI3 is enabled */
            u32ModuleBmp |= 1 << FROM_RC_HIGH_SPI3;
        }
    }
    
    /* EXT2IO */
    if ( ((AOS->APS_DOMAIN_EN & AOS_APS_DOMAIN_EN_EXT_2IO_CLK_SEL_Msk) >> AOS_APS_DOMAIN_EN_EXT_2IO_CLK_SEL_Pos) == APS_CLK_EXT_2_IO_SRC_RC)
    {   /* EXT2IO clock from RC */
        if ( (AOS->APS_DOMAIN_EN & (uint32_t)APS_CLK_EXT_2_IO_EN_MSK) != APS_CLK_EXT_2_IO_EN_SW_OFF)
        {   /* EXT2IO is SW enabled or PAD enable */
            u32ModuleBmp |= 1 << FROM_RC_HIGH_EXT2IO;
        }
    }
    
    /* MSQ SYSCLK */
    if ( ((AOS->MSQ_CLK_SEL & MSQ_CLK_SYS_SRC_SEL_MSK) >> MSQ_CLK_SYS_SRC_SEL_POS) == MSQ_CLK_SYS_SRC_RC)
    {   /* MSQ SYSCLK from RC */
        u32ModuleBmp |= 1 << FROM_RC_HIGH_MSQ_SYSCLK;
    }
    
    return u32ModuleBmp;
}

/**
 * @brief Hal_Sys_FromXtalHighCheck
 *        Check which module is from high speed XTAL and enabled
 * @return Module bit map.
 *         Bit order refer to E_ModuleFromXtalHigh_t
 */
uint32_t Hal_Sys_FromXtalHighCheck_impl(void)
{
    uint32_t u32ModuleBmp = 0;
    uint32_t u32Temp;
    
    /* APS SYSCLK */
    u32Temp = (AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_APS_SRC_CLK_SEL_Msk) >> AOS_APS_CLK_SEL_APS_SRC_CLK_SEL_Pos;
    if ((u32Temp != APS_CLK_SYS_SRC_RC) && (u32Temp != APS_CLK_SYS_SRC_EXTERNAL))
    {
        /* APS SYSCLK from XTAL */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_SYSCLK;
    }
    
    /* SCRT */
    u32Temp =(AOS->SCRT_CLK_APS & AOS_SCRT_CLK_APS_SCRT_SRC_CLKSEL_Msk) >> AOS_SCRT_CLK_APS_SCRT_SRC_CLKSEL_Pos;
    if ((u32Temp != ASP_CLK_SCRT_SRC_RC) && (u32Temp != ASP_CLK_SCRT_SRC_EXTERNAL))
    {   /* Security clock from XTAL */
        if ((AOS->SCRT_CLK_APS & AOS_SCRT_CLK_APS_SCRT_GLB_CLK_APS_EN) || (AOS->SCRT_CLK_MSQ & AOS_SCRT_CLK_MSQ_SCRT_GLB_CLK_MSQ_EN))
        {   /* SCRT is enabled */
            u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_SCRT;
        }
    }
    
    /* SPI0 */
    if ( ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI0_CLKSEL_Msk) >> AOS_SPI_CLK_SEL_SPI0_CLKSEL_Pos) != APS_CLK_SPI0_SRC_RC)
    {   /* SPI0 clock from XTAL */
        if ( (AOS->SPI_CLK_SEL & (AOS_SPI_CLK_SEL_SPI0_HCLK_EN | AOS_SPI_CLK_SEL_SPI0_PCLK_EN | AOS_SPI_CLK_SEL_SPI0_REFCLK_EN)) == (AOS_SPI_CLK_SEL_SPI0_HCLK_EN | AOS_SPI_CLK_SEL_SPI0_PCLK_EN | AOS_SPI_CLK_SEL_SPI0_REFCLK_EN))
        {   /* SPI0 is enabled */
            u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_SPI0;
        }
    }
    
    /* SPI1 */
    if ( ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI1_CLK_SEL_Msk) >> AOS_SPI_CLK_SEL_SPI1_CLK_SEL_Pos) != APS_CLK_SPI1_SRC_RC)
    {   /* SPI1 clock from XTAL */
        if ( (AOS->SPI_CLK_SEL & (AOS_SPI_CLK_SEL_SPI1_PCLK_EN | AOS_SPI_CLK_SEL_SPI1_SCLK_EN)) == (AOS_SPI_CLK_SEL_SPI1_PCLK_EN | AOS_SPI_CLK_SEL_SPI1_SCLK_EN))
        {   /* SPI1 is enabled */
            u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_SPI1;
        }
    }
    
    /* SPI2 */
    if ( ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI2_CLK_SEL_Msk) >> AOS_SPI_CLK_SEL_SPI2_CLK_SEL_Pos) != APS_CLK_SPI2_SRC_RC)
    {   /* SPI2 clock from XTAL */
        if ( (AOS->SPI_CLK_SEL & (AOS_SPI_CLK_SEL_SPI2_PCLK_EN | AOS_SPI_CLK_SEL_SPI2_SCLK_EN)) == (AOS_SPI_CLK_SEL_SPI2_PCLK_EN | AOS_SPI_CLK_SEL_SPI2_SCLK_EN))
        {   /* SPI2 is enabled */
            u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_SPI2;
        }
    }
        
    /* SPI3 */
    if ( ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI3_CLK_SEL_Msk) >> AOS_SPI_CLK_SEL_SPI3_CLK_SEL_Pos) != APS_CLK_SPI3_SRC_RC)
    {   /* SPI3 clock from XTAL */
        if ( (AOS->SPI_CLK_SEL & (AOS_SPI_CLK_SEL_SPI3_PCLK_EN | AOS_SPI_CLK_SEL_SPI3_SCLK_EN)) == (AOS_SPI_CLK_SEL_SPI3_PCLK_EN | AOS_SPI_CLK_SEL_SPI3_SCLK_EN))
        {   /* SPI3 is enabled */
            u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_SPI3;
        }
    }
    
    /* I2S. Always from XTAL */
    if ((AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_I2S_PCLK_EN) && (AOS->APS_DM_CLK_SEL & AOS_APS_DM_CLK_SEL_I2S_SCLK_EN))
    {   /* I2S is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_I2S;
    }
    
    /* UART0. Always from XTAL */
    if (AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_UART0_PCLK_EN)
    {   /* UART0 is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_UART0;
    }
    
    /* UART1. Always from XTAL */
    if (AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_UART1_PCLK_EN)
    {   /* UART1 is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_UART1;
    }
    
    /* APS UART. Always from XTAL */
    if (AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_APS_UART_PCLK_EN)
    {   /* APS Debug UART is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_DBG_UART;
    }
    
    /* UART ABR. Always from XTAL */
    if (AOS->APS_DOMAIN_EN & AOS_APS_DOMAIN_EN_UART_ABR_CLK_EN)
    {   /* UART ABR is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_UART_ABR;
    }
    
    /* PDM. Always from XTAL */
    if ((AOS->APS_DM_CLK_SEL & (AOS_APS_DM_CLK_SEL_PDM_IO_CLK_EN | AOS_APS_DM_CLK_SEL_PDM_CLK_EN)) == AOS_APS_DM_CLK_SEL_PDM_CLK_EN)
    {   /* PDM is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_PDM;
    }
    
    /* EXT2IO */
    if ( ((AOS->APS_DOMAIN_EN & AOS_APS_DOMAIN_EN_EXT_2IO_CLK_SEL_Msk) >> AOS_APS_DOMAIN_EN_EXT_2IO_CLK_SEL_Pos) == APS_CLK_EXT_2_IO_SRC_XTAL)
    {   /* EXT2IO clock from XTAL */
        if ( (AOS->APS_DOMAIN_EN & (uint32_t)APS_CLK_EXT_2_IO_EN_MSK) != APS_CLK_EXT_2_IO_EN_SW_OFF)
        {   /* EXT2IO is SW enabled or PAD enable */
            u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_EXT2IO;
        }
    }
    
    /* APS Systick. Always from XTAL */
    if (AOS->APS_STCLK & AOS_APS_STCLK_APS_STCLK_SRC_EN)
    {   /* Systick is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_ST;
    }
    
    /* TMR0. Always from XTAL */
    if (AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_TMR0_PCLK_EN)
    {   /* TMR0 is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_TMR0;
    }
    
    /* TMR1. Always from XTAL */
    if (AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_TMR1_PCLK_EN)
    {   /* TMR1 is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_APS_TMR1;
    }
    
    /* CAM XVCLK. Always from XTAL */
    if (AOS->CAM_XVCLK_SEL & AOS_CAM_XVCLK_SEL_CAM_XVCLK_EN)
    {   /* CAM XVCLK is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_CAM_XVCLK;
    }
    
    /* MSQ SYSCLK */
    if ( ( (AOS->MSQ_CLK_SEL & AOS_MSQ_CLK_SEL_MSQ_SRC2_CLKSEL) == 0) &&
         ( (AOS->MSQ_CLK_SEL & AOS_MSQ_CLK_SEL_MSQ_SRC1_CLKSEL) != 0) )
    {
        /* MSQ SYSCLK from XTAL */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_MSQ_SYSCLK;
    }
    
    /* MSQ Debug UART. Always from XTAL */
    if (AOS->MSQ_CLK_SEL & AOS_MSQ_CLK_SEL_MSQ_UART_PCLK_EN)
    {   /* MSQ Debug UART is enabled */
        u32ModuleBmp |= 1 << FROM_XTAL_HIGH_MSQ_DBG_UART;
    }
    return u32ModuleBmp;
}

/* ******************************* RC calibration ******************************* */
/**
 * @brief Hal_Sys_RcCal
 *        Using XTAL to calibrate RC. XTAL and RC must be enabled before this function.
 * @warning Make sure APS and MSQ system clock not RC before call this function.
 *
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_RcCal_impl(void)
{
#if !defined(OPL2500_FPGA)
    uint32_t u32Ctune, u32CalCen;
    int32_t s32BitIdx;
    int32_t s32CmpRslt;
    
    /* 
     * Set CAL_CEN in middle as default value
     * To binary search CTUNE value
     * Check the the RC can be tuned to XTAL current CTUNE.
     * i.e. In current CTUNE, XTAL frequency in between minimum and maximum CEN
     */
    u32Ctune = 0;
    
    for (s32BitIdx = RC_CAL_OSC_CTUNE_LEN - 1; s32BitIdx >= 0; s32BitIdx--)
    {
        /* Set CTUNE value */
        BIT_SET(u32Ctune, s32BitIdx, 1);
        
        /* Check minimum CEN */
        u32CalCen = RC_CAL_CEN_MIN;
        s32CmpRslt = Hal_Sys_RcCalCompare(u32Ctune, u32CalCen, OSC_CAL_CTUNE_WAIT_TICKS);
        if (s32CmpRslt == RC_LT_XTAL)
        {   /* F_RC < F_Xtal */
            /* RC is slower than XTAL, Check maximum frequency */
            u32CalCen = RC_CAL_CEN_MAX;
            s32CmpRslt = Hal_Sys_RcCalCompare(u32Ctune, u32CalCen, OSC_CAL_CTUNE_WAIT_TICKS);
            if (s32CmpRslt == RC_GT_XTAL)
            {   /* F_RC > F_XTAL, continue to OSC_CTUNE estimated */
                break;
            }
            /* else F_RC <= F_XTAL, continue test next bit */
        }
        else
        {/* F_RC >= F_XTAL, Reset the bit */
            BIT_SET(u32Ctune, s32BitIdx, 0);
        }
    }
    
    u32CalCen = 0;
    for(s32BitIdx = RC_CAL_PATTERN_LEN-1 ; s32BitIdx >= 0; s32BitIdx--)
    {
        BIT_SET(u32CalCen, s32BitIdx, 1);
        
        /* Checking CAL_CEN boundary */
        if (u32CalCen < RC_CAL_CEN_MIN)
        {   /* Less than 0x420. Increase */
            continue;
        }
        else if (u32CalCen >= RC_CAL_CEN_MAX)
        {   /* u32CalCen > 0xBC0. Decrease CEN */
            BIT_SET(u32CalCen, s32BitIdx, 0);
            continue;
        }
        
        s32CmpRslt = Hal_Sys_RcCalCompare(u32Ctune, u32CalCen, OSC_CAL_CEN_WAIT_TICKS);
        if (s32CmpRslt == RC_EQ_XTAL)
        {   /* Matched */
            break;
        }
        else if (s32CmpRslt == RC_GT_XTAL)
        {   /* RC is faster than XTAL, decreate CAL_CEN */
            BIT_SET(u32CalCen, s32BitIdx, 0);
        }
        /*
         * RC_LT_XTAL.
         * RC is slower than XTAL, increase CAL_CEN
         */
    }
 
    /* Checking CAL_CEN boundary */
    if (u32CalCen < RC_CAL_CEN_MIN)
    {   /* Less than 0x420. Increase */
        u32CalCen = RC_CAL_CEN_MIN;
    }
    else if (u32CalCen >= RC_CAL_CEN_MAX)
    {   /* u32CalCen > 0xBC0. Decrease CEN */
        u32CalCen = RC_CAL_CEN_MAX;
    }
    /* Set CTUNE and CEN */
    AOS->CAL_CEN = (u32CalCen << AOS_CAL_CEN_ENABLE_Pos) | (u32Ctune << AOS_CAL_CEN_22MRC_OSC_CTUNE_Pos);
    
    /* Setup start point */
    if (( (AOS->CAL_CEN & AOS_CAL_CEN_ENABLE_Msk) >> AOS_CAL_CEN_ENABLE_Pos ) <= RC_CAL_START_POINT)
        AOS->CAL_STR = 0;
    else
        AOS->CAL_STR = AOS->CAL_CEN - RC_CAL_START_POINT;
#endif
    return RESULT_SUCCESS;
}

/**
 * @brief Start calculation and compare the RC and XTAL cycles
 * @param u32Ctune [in] The value to be set to RF->RC_OSC_CAL_FREQCOMP_CTL (@0x400090C0), RG_OSC_CTUNE[5:2]
 * @param u32Cen [in] The value to be set to AOS->CAL_CEN (@0x400010F0), RG_CAL_CEN [11:0]
 * @param u32WaitXtalTicks [in] Wait the ticks after enable calibration
 * @return The compare result.
 * @retval RC_EQ_XTAL (0) RC and XTAL clock matched
 * @retval RC_GT_XTAL (1) RC cycles is larger than XTAL cycles.
 * @retval RC_LT_XTAL (-1) RC cycles is less then XTAL cycles
 */
int32_t Hal_Sys_RcCalCompare_impl(uint32_t u32Ctune, uint32_t u32Cen, uint32_t u32WaitXtalTicks)
{   
    uint32_t u32XtalCnt, u32RcCnt;
    uint32_t u32OscCalCtl;

    /* Reset frequency compare */
    RF->RC_OSC_CAL_CTL = 0; /* Reset CAL */
    
    /* Set CAL fine tune */
    AOS->CAL_CEN = u32Cen;

    /* Release reset and Set Calibration coarse tune */
    u32OscCalCtl =(u32Ctune << RF_RC_OSC_CAL_CTL_OSC_CTUNE_Pos) | RF_RC_OSC_CAL_CTL_FREQ_COMP_RST_N;
    RF->RC_OSC_CAL_CTL = u32OscCalCtl;
        
    /* Start calibration counting */
    RF->RC_OSC_CAL_CTL = u32OscCalCtl | RF_RC_OSC_CAL_CTL_CAL_START;

    while (1)
    {
        u32XtalCnt = (RF->RC_OSC_CAL_OUTPUT & RF_RC_OSC_CAL_OUTPUT_COUNT_XTAL_Msk) >> RF_RC_OSC_CAL_OUTPUT_COUNT_XTAL_Pos;
        if (u32XtalCnt >= u32WaitXtalTicks)
        {
            /* Disable counting */
            RF->RC_OSC_CAL_CTL = u32OscCalCtl;
            break;
        }
    }
    
    /* Read RC clock and compare */
    u32RcCnt = (RF->RC_OSC_CAL_OUTPUT & RF_RC_OSC_CAL_OUTPUT_COUNT_RC_Msk) >> RF_RC_OSC_CAL_OUTPUT_COUNT_RC_Pos;
    
    if (u32RcCnt == u32XtalCnt)
        return RC_EQ_XTAL;
    else if (u32RcCnt > u32XtalCnt)
        return RC_GT_XTAL;
    else
        return RC_LT_XTAL;
}


/**
 * @brief Hal_Sys_CheckXtalReady
 *        Check XTAL ready status via FreqComp function of fast-start mechanism
 */
void Hal_Sys_CheckXtalReady_impl(void)
{
#if !defined(OPL2500_FPGA)
    int consecutive_success = 0;

    while (1)
    {
        // Disable
        RF->RC_OSC_CAL_CTL &= ~RF_RC_OSC_CAL_CTL_CAL_START;

        // Reset, 0:reset
        RF->RC_OSC_CAL_CTL &= ~RF_RC_OSC_CAL_CTL_FREQ_COMP_RST_N;
        RF->RC_OSC_CAL_CTL |= RF_RC_OSC_CAL_CTL_FREQ_COMP_RST_N;

        // Enable
        RF->RC_OSC_CAL_CTL |= RF_RC_OSC_CAL_CTL_CAL_START;

        while (((RF->RC_OSC_CAL_OUTPUT & RF_RC_OSC_CAL_OUTPUT_COUNT_RC_Msk) >> RF_RC_OSC_CAL_OUTPUT_COUNT_RC_Pos) < CHECK_XTAL_COUNT);

        // Disable
        RF->RC_OSC_CAL_CTL &= ~RF_RC_OSC_CAL_CTL_CAL_START;

        int cnt_xtal = RF->RC_OSC_CAL_OUTPUT & RF_RC_OSC_CAL_OUTPUT_COUNT_XTAL_Msk;
        int cnt_rc   = (RF->RC_OSC_CAL_OUTPUT & RF_RC_OSC_CAL_OUTPUT_COUNT_RC_Msk) >> RF_RC_OSC_CAL_OUTPUT_COUNT_RC_Pos;
        int cnt_diff = (cnt_xtal > cnt_rc) ? (cnt_xtal - cnt_rc) : (cnt_rc - cnt_xtal);

        if (cnt_diff <= CHECK_XTAL_TOLERANCE)
        {
            consecutive_success++;
            if (consecutive_success == CHECK_XTAL_SUCCESS_COUNT)
                break;            
        }
        else
            consecutive_success = 0;
    }
#endif
}

/**
 * @brief Hal_Sys_32kXtalSetup
 *        Setup RTC timer clock source as 32k XTAL. The clock also provide to XTAL detection and SEQ. 
 *        Called at SystemInit, assume the registers value are reset values
 */
void Hal_Sys_32kXtalSetup(void)
{
    /* Enable AOS overwrite control */
    AOS->RET_MUX = AOS_REG_OVCTL_FW;
        
    /* Setup 32k XTAL GM and CAP */
    AOS->OSC_CTL = (AOS->OSC_CTL & ~(AOS_OSC_CTL_32K_XTAL_GM_Msk | AOS_OSC_CTL_32K_XTAL_CAP_Msk)) |
                    (MSQ_CLK_32k_XTAL_GAIN << AOS_OSC_CTL_32K_XTAL_GM_Pos) | 
                    (MSQ_CLK_32k_XTAL_CAP << AOS_OSC_CTL_32K_XTAL_CAP_Pos);
    
    /* Enable 32k XTAL */
    AOS->OSC_CTL |= AOS_OSC_CTL_32K_XTAL_EN | AOS_OSC_CTL_XTAL_ON;
}


/**
 * @brief Hal_Sys_DetXtalStart
 *        To start detect XTAL clock frequency. External 32k XTAL is required.
 *        Call this function at boot rom beginning.
 *        Call Hal_Sys_DetXtalResult later, the detection take a while.
 *        The function is called by boot ROM, no need using function pointer.
 */
void Hal_Sys_DetXtalStart(void)
{
    /* Setup RTC source to 32k XTAL */
    AOS->OSC_SEL = AOS_OSC_SEL_32K_OSC_SEL | AOS_OSC_SEL_RTC_EN;
    
    /* Start to detect */
    SYS->DET_XTAL = (DET_XTAL_PRE_DET_32K_TIMEOUT << SYS_DET_XTAL_PRE_DET_32K_TO_CNT_Pos) |
                    (DET_XTAL_PRE_DET_32K_CYCLE << SYS_DET_XTAL_DET_32K_CNT_Pos) |
                    SYS_DET_XTAL_DET_XTAL_CLK_EN |
                    SYS_DET_XTAL_DET_XTAL_EN;
}

/**
 * @brief Hal_Sys_DetXtalEnd
 *        Detect XTAL clock frequency. External 32k XTAL is required.
 *        Get detection result.
 *        The function is called by boot ROM, no need using function pointer.
 * @param pu32XtalFreq [out] When detected success, store the XTAL frequency to this value
 * @return Detect status. Refer to E_DetectXtalStatus
 * @retval DET_XTAL_STS_SUCCESS Detect XTAL success.
 * @retval DET_XTAL_STS_FAIL_PRE_DET_TIMEOUT Pre-detect 32K timeout, but hardware doesn't timeout.
 * @retval DET_XTAL_STS_FAIL_NO_32K_XTAL There is not 32k XTAL, the XTAL detection will be skipped.
 * @retval DET_XTAL_STS_FAIL_DET_TIMOEUT Pre-detect 32k finished. XTAL detection timeout and cannot get done flag.
 * @retval DET_XTAL_STS_FAIL_XTAL_DISTORTION The XTAL frequency is distored.
 */
E_DetectXtalStatus Hal_Sys_DetXtalEnd(uint32_t *pu32XtalFreq)
{
    uint32_t u32Timeout = DET_XTAL_DET_POLLING_TIMEOUT;
    uint32_t u32XtalCnt=0;
    E_DetectXtalStatus eStatus;
    
    while(--u32Timeout)
    {
        if (SYS->DET_XTAL_STS & SYS_DET_XTAL_STS_PRE_DET_32K_DONE)
            break;
    }
    
    if (!u32Timeout)
    {   /* Cannot get detection 32k XTAL done */
        eStatus = DET_XTAL_STS_FAIL_PRE_DET_TIMEOUT;
        goto RCC_XTAL_DET_END;
    }
    
    if ((SYS->DET_XTAL_STS & SYS_DET_XTAL_STS_PRE_DET_32K_ACTIVE) == 0)
    {   /* No 32k XTAL */
        eStatus = DET_XTAL_STS_FAIL_NO_32K_XTAL;
        goto RCC_XTAL_DET_END;
    }
    
    while (--u32Timeout)
    {   /* Not reset timeout value, continue counting */
        if (SYS->DET_XTAL_STS & (SYS_DET_XTAL_STS_DET_XTAL_DONE | SYS_DET_XTAL_STS_DET_XTAL_DISTORTION))
        {
            break;
        }
    }
    if (!u32Timeout)
    {   /* Cannot get detection XTAL done or distortion */
        eStatus =  DET_XTAL_STS_FAIL_DET_TIMOEUT;
        goto RCC_XTAL_DET_END;
    }
    
    if (SYS->DET_XTAL_STS & SYS_DET_XTAL_STS_DET_XTAL_DISTORTION)
    {   /* XTAL distortion */
        eStatus =  DET_XTAL_STS_FAIL_XTAL_DISTORTION;
        goto RCC_XTAL_DET_END;
    }
    
    
    /* Detect XTAL finished */
    u32XtalCnt = (SYS->DET_XTAL_STS & SYS_DET_XTAL_STS_DET_XTAL_CNT_Msk) >> SYS_DET_XTAL_STS_DET_XTAL_CNT_Pos;
    *pu32XtalFreq = CLK_SRC_FREQ_32K_XTAL * u32XtalCnt / DET_XTAL_PRE_DET_32K_CYCLE;
    eStatus =  DET_XTAL_STS_SUCCESS;

RCC_XTAL_DET_END:
    SYS->DET_XTAL &= ~(SYS_DET_XTAL_DET_XTAL_EN | SYS_DET_XTAL_DET_XTAL_CLK_EN);
    return eStatus;
}


/* ******************************* System clock ********************************* */

/**
 * @brief Hal_Sys_SleepClkSetup
 *      1. Enable RC clock
 *      2. Switch shelter clock to RC
 *      3. Set System clock to shelter
 *      Then system clock becomes to RC
 *      After wakeup, resume system clock to previous setting
 */
void Hal_Sys_SleepClkSetup_impl(void)
{
    Hal_Sys_RcHighEn(RC_HIGH_ON);
    
    Hal_Sys_SwitchShelterSet(CLKTREE_SHELTER_SRC_RC);
    
    /* Move system clock to shelter source - RC */
    APS_CLK_SYS_SWITCH_TO_SHELTER();
    APS_CLK_SCRT_SWITCH_TO_SHELTER();
    
    /* Disable MMD */
    AOS->CLK_MMFACTOR_APS &= ~AOS_CLK_MMFACTOR_APS_ENABLE;
}

/**
 * @brief Hal_Sys_WakeupClkResume
 *      Resume clock tree.
 *      Because the system clock is switched to shelter before entering sleep.
 *      So switch system clock back to resume clock
 *      Then switch shelter to XTAL back
 */
void Hal_Sys_WakeupClkResume_impl(void)
{
    /* Recover MMD */
    if (g_sApsClkFreq.u32MMFactor != APS_MMFACTOR_DEFAULT)
        AOS->CLK_MMFACTOR_APS |= AOS_CLK_MMFACTOR_APS_ENABLE;
    
    /* Move system clock to target source - previous setting */
    APS_CLK_SYS_SWITCH_TO_TARGET();
    APS_CLK_SCRT_SWITCH_TO_TARGET();
    
    Hal_Sys_SwitchShelterSet(CLKTREE_SHELTER_SRC_XTAL);
    
    SystemCoreClockSet(g_sApsClkFreq.u32SysClk);
}


/**
 * @brief Hal_Sys_ApsClkTreeSetup
 *        APS clock-tree setup. It contain a source select and two divider in clock-tree
 *
 * @param eSrc[in] Select the source of clock tree. Refer to ::E_ApsClkSysSrc_t
 *                  APS_CLK_SYS_SRC_RC
 *                  APS_CLK_SYS_SRC_XTAL
 *                  APS_CLK_SYS_SRC_XTAL_X2
 *                  APS_CLK_SYS_SRC_150_VAR
 *                  APS_CLK_SYS_SRC_DECI_160M_BB
 *                  APS_CLK_SYS_SRC_EXTERNAL
 * @param eApsClkSysDiv[in] Select the source divider of clock tree. Refer to ::E_ApsClkPclkDivn_t
 *                  APS_CLK_SYS_DIV_1
 *                  APS_CLK_SYS_DIV_2
 * @param eApsClkPclkDiv[in] Select the PCLK divider. Refer to ::E_ApsClkPclkDivn_t
 *                  APS_CLK_PCLK_DIV_1
 *                  APS_CLK_PCLK_DIV_2
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_ApsClkTreeSetup_impl(E_ApsClkSysSrc_t eSrc, E_ApsClkSrcDivn_t eSysDiv, E_ApsClkPclkDivn_t ePclkDiv)
{
    uint32_t u32SysClk = 0, u32Pclk = 0;
        
    switch (eSrc)
    {
        case APS_CLK_SYS_SRC_RC:
            u32SysClk = CLK_SRC_FREQ_RC;
            break;
        case APS_CLK_SYS_SRC_XTAL:
            u32SysClk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_SYS_SRC_XTAL_X2:
            u32SysClk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case APS_CLK_SYS_SRC_150_VAR:
            u32SysClk = CLK_SRC_FREQ_150M_VAR;
            break;
        case APS_CLK_SYS_SRC_DECI_160M_BB:
            u32SysClk = CLK_SRC_FREQ_160M_BB;
            break;
        case APS_CLK_SYS_SRC_EXTERNAL:
            u32SysClk = g_sApsClkFreq.u32ExtApsClk;
            break;
        default:
            return RESULT_FAIL;
    }
    u32SysClk /= APS_CLK_SYS_DIVN_GET(eSysDiv);
    u32Pclk = u32SysClk / APS_CLK_PCLK_DIVN_GET(ePclkDiv);
    
    APS_CLK_SYS_SWITCH_TO_SHELTER();
    AOS->APS_CLK_SEL = (AOS->APS_CLK_SEL & ~(AOS_APS_CLK_SEL_APS_SRC_CLK_SEL_Msk | APS_CLK_SYS_DIV_MSK | APS_CLK_PCLK_DIV_MSK) ) |
                        (eSrc << AOS_APS_CLK_SEL_APS_SRC_CLK_SEL_Pos) | eSysDiv | ePclkDiv;
    APS_CLK_SYS_SWITCH_TO_TARGET();

    // Update system clock.
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_SYS_SRC, u32SysClk);
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_PCLK, u32Pclk);
    
    // Apply to all relative modules
    Hal_Sys_ApsClkChangeApply();
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_ApsClkChangeApply
 *        Update all system clock relative
 */
void Hal_Sys_ApsClkChangeApply_impl(void)
{
    SysTick->LOAD =( SystemCoreClockGet()/1000 ) - 1;
    
    // I2C
    if (AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_I2C_PCLK_EN)
        Hal_I2c_UpdateClk();
        
    // WDT
    if (AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_WDT_PCLK_EN)
        Hal_Wdt_UpdateClk();
}

/**
 * @brief Hal_Sys_ApsClockUpdate
 *        Updated corresponding clock golbal values
 * @param eApsClkGrp[in] The target clock to be updated
 * @param u32ClkFreq[in] The clock frequency
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_ApsClockUpdate_impl(E_ApsClkGrp_t eApsClkGrp, uint32_t u32ClkFreq)
{
    uint32_t u32ClkAddr;

    if ((uint32_t)eApsClkGrp > sizeof(g_sApsClkFreq))
    {   /* Invalid clock group */
        return RESULT_FAIL;
    }
    
    if (eApsClkGrp == APS_CLK_GRP_SYS)
    {   /* SYS_CLK is updated by SYS_SRC and MMFACTOR, cannot be modified directly */
        return RESULT_FAIL;
    }
    
    u32ClkAddr = (uint32_t)&g_sApsClkFreq + eApsClkGrp;
    *(uint32_t *)u32ClkAddr = u32ClkFreq;
    
    if (eApsClkGrp == APS_CLK_GRP_SYS_SRC)
    {
        /* Update system clock */
        g_sApsClkFreq.u32SysClk = g_sApsClkFreq.u32SysSrcClk/APS_MMFACTOR_DIVISOR * g_sApsClkFreq.u32MMFactor;
        SystemCoreClockSet(g_sApsClkFreq.u32SysClk);
    }
    
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_ApsClockGet
 *        Get corresponding clock frequency
 * @param eApsClkGrp[in] The target clock to be updated
 * @param pu32ClkFreq[out] To store the target clock frequency here
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_ApsClockGet_impl(E_ApsClkGrp_t eApsClkGrp, uint32_t *pu32ClkFreq)
{
    uint32_t u32ClkAddr;

    u32ClkAddr = (uint32_t)&g_sApsClkFreq + eApsClkGrp;
    if ((uint32_t)eApsClkGrp > sizeof(g_sApsClkFreq))
        return RESULT_FAIL;
    
    *pu32ClkFreq = *(uint32_t *)u32ClkAddr;
    return RESULT_SUCCESS;
}


/**
 * @brief Hal_Sys_SwitchShelterSet
 *        Set APS, SCRT, and MSQ shelter source
 * @param eSrc[in] The source of shelter
 *                  CLKTREE_SHELTER_SRC_RC
 *                  CLKTREE_SHELTER_SRC_XTAL
 */
void Hal_Sys_SwitchShelterSet_impl(E_ClkTreeShelterSrc_t eSrc)
{
    if (eSrc == CLKTREE_SHELTER_SRC_RC)
    {
        APS_SET_SHELTER_SRC_TO_RC();
        SCRT_SET_SHELTER_SRC_TO_RC();
    }
    else
    {
        APS_SET_SHELTER_SRC_TO_XTAL();
        SCRT_SET_SHELTER_SRC_TO_XTAL();
    }
}



/* ******************************* Peripheral clock ***************************** */
/**
 * @brief Hal_Sys_PwmSrcSelect
 *        PWM clock source select
 * @param eSrc[in] PWM clock select. Refer to ::E_ApsClkPwmSrc_t
 *                  APS_CLK_PWM_SRC_32k_CLK
 *                  APS_CLK_PWM_SRC_XTAL_HIGH
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_PwmSrcSelect_impl(E_ApsClkPwmSrc_t eSrc)
{
    uint32_t u32Clk;
    switch(eSrc)
    {
        case APS_CLK_PWM_SRC_32k_CLK:
            u32Clk = CLK_SRC_FREQ_32K_RC;
            break;
        case APS_CLK_PWM_SRC_XTAL_HIGH:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        default:
            return RESULT_FAIL;
    }

    AOS->APS_DOMAIN_EN = (AOS->APS_DOMAIN_EN & ~AOS_APS_DOMAIN_EN_PWM_CLKSEL_Msk) | ( eSrc << AOS_APS_DOMAIN_EN_PWM_CLKSEL_Pos);
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_PWM, u32Clk);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_ScrtSrcSelect
 *        Security clock source select
 * @param eSrc[in] Select the source of security clock tree. Refer to ::E_ApsClkScrtSrc_t
 *                  ASP_CLK_SCRT_SRC_RC
 *                  ASP_CLK_SCRT_SRC_XTAL        
 *                  ASP_CLK_SCRT_SRC_XTAL_X2     
 *                  ASP_CLK_SCRT_SRC_D1_180M     
 *                  ASP_CLK_SCRT_SRC_DECI_160M_BB
 *                  ASP_CLK_SCRT_SRC_EXTERNAL    
 * @param eDiv[in] Select the source divider of security clock tree. Refer to ::E_ApsClkScrtDivn_t
 *                  APS_CLK_SCRT_DIV_1
 *                  APS_CLK_SCRT_DIV_2
 *                  APS_CLK_SCRT_DIV_3
 *                  APS_CLK_SCRT_DIV_4
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_ScrtSrcSelect_impl(E_ApsClkScrtSrc_t eSrc, E_ApsClkScrtDivn_t eDiv)
{
    uint32_t u32Clk;
    
    switch(eSrc)
    {
        case ASP_CLK_SCRT_SRC_RC:
            u32Clk = CLK_SRC_FREQ_RC;
            break;
        case ASP_CLK_SCRT_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case ASP_CLK_SCRT_SRC_XTAL_X2:
            u32Clk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case ASP_CLK_SCRT_SRC_D1_180M:
            u32Clk = CLK_SRC_FREQ_D1_180M;
            break;
        case ASP_CLK_SCRT_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;
        case ASP_CLK_SCRT_SRC_EXTERNAL:
            u32Clk = g_sApsClkFreq.u32ExtApsClk;
            break;
        default:
            return RESULT_FAIL;
    }
    u32Clk /= APS_CLK_SCRT_DIVN_GET(eDiv);
    
    
    APS_CLK_SCRT_SWITCH_TO_SHELTER();
    AOS->SCRT_CLK_APS = (AOS->SCRT_CLK_APS & ~(AOS_SCRT_CLK_APS_SCRT_SRC_CLKSEL_Msk | APS_CLK_SCRT_DIV_MSK) ) | 
                    (eSrc << AOS_SCRT_CLK_APS_SCRT_SRC_CLKSEL_Pos) | eDiv;
    APS_CLK_SCRT_SWITCH_TO_TARGET();
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_SCRT, u32Clk);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_StDivSelect
 *        Select Systick divider
 *        Systick source is XTAL
 * @param eDiv[in] Select the source divider of security clock tree. Refer to ::E_ApsClkStDivn_t
 *                  APS_CLK_ST_DIV_1
 *                  APS_CLK_ST_DIV_2
 *                  APS_CLK_ST_DIV_4
 *                  APS_CLK_ST_DIV_8
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_StDivSelect_impl(E_ApsClkStDivn_t eDiv)
{
    uint32_t u32Clk;
    
    u32Clk = g_sApsClkFreq.u32XtalClk / APS_CLK_ST_DIVN_GET(eDiv);
    
    AOS->APS_STCLK = (AOS->APS_STCLK & ~AOS_APS_STCLK_APS_STCLK_SRC_SEL_Msk) | eDiv;
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_ST, u32Clk);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_Spi0SrcSelect
 *        SPI0 clock source select
 * @param eSrc[in] Select the source of SPI0 clock tree. Refer to ::E_ApsClkSpi0Src_t
 *                  APS_CLK_SPI0_SRC_RC           
 *                  APS_CLK_SPI0_SRC_XTAL         
 *                  APS_CLK_SPI0_SRC_150M_VAR     
 *                  APS_CLK_SPI0_SRC_DECI_160M_BB 
 *                  APS_CLK_SPI0_SRC_D1_200M      
 *                  APS_CLK_SPI0_SRC_EXTERNAL     
 * @param eDiv[in] Select the source divider of SPI0 clock tree. Refer to ::E_ApsClkSpi0Divn_t
 *                  APS_CLK_SPI0_DIV_1 
 *                  APS_CLK_SPI0_DIV_2 
 *                  APS_CLK_SPI0_DIV_3 
 *                  APS_CLK_SPI0_DIV_4 
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_Spi0SrcSelect_impl(E_ApsClkSpi0Src_t eSrc, E_ApsClkSpi0Divn_t eDiv)
{
    uint32_t u32Clk;
    
    switch(eSrc)
    {
        case APS_CLK_SPI0_SRC_RC:
            u32Clk = CLK_SRC_FREQ_RC;
            break;
        case APS_CLK_SPI0_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_SPI0_SRC_150M_VAR:
            u32Clk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case APS_CLK_SPI0_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;;
        case APS_CLK_SPI0_SRC_D1_200M:
            u32Clk = CLK_SRC_FREQ_D1_200M;
            break;
        case APS_CLK_SPI0_SRC_EXTERNAL:
            u32Clk = g_sApsClkFreq.u32ExtApsClk;
            break;
        default:
            return RESULT_FAIL;
    }
    u32Clk /= APS_CLK_SPI0_DIVN_GET(eDiv);
    
    AOS->SPI_CLK_SEL = (AOS->SPI_CLK_SEL & ~(AOS_SPI_CLK_SEL_SPI0_CLKSEL_Msk | AOS_SPI_CLK_SEL_SPI0_REFCLK_DIVN_Msk) ) | 
                        (eSrc << AOS_SPI_CLK_SEL_SPI0_CLKSEL_Pos) | eDiv;
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_SPI0, u32Clk);
    
    Hal_Qspi_Divider_Update();
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_Spi1SrcSelect
 *        SPI1 clock source select
 * @param eSrc[in] Select the source of SPI1 clock tree. Refer to ::E_ApsClkSpi1Src_t
 *                  APS_CLK_SPI1_SRC_RC           
 *                  APS_CLK_SPI1_SRC_XTAL         
 *                  APS_CLK_SPI1_SRC_150M_VAR     
 *                  APS_CLK_SPI1_SRC_DECI_160M_BB 
 *                  APS_CLK_SPI1_SRC_EXTERNAL     
 * @param eDiv[in] Select the source divider of SPI1 clock tree. Refer to ::E_ApsClkSpi1Divn_t
 *                  APS_CLK_SPI1_DIV_1 
 *                  APS_CLK_SPI1_DIV_2 
 *                  APS_CLK_SPI1_DIV_3 
 *                  APS_CLK_SPI1_DIV_4 
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_Spi1SrcSelect_impl(E_ApsClkSpi1Src_t eSrc, E_ApsClkSpi1Divn_t eDiv)
{
    uint32_t u32Clk;
    
    switch(eSrc)
    {
        case APS_CLK_SPI1_SRC_RC:
            u32Clk = CLK_SRC_FREQ_RC;
            break;
        case APS_CLK_SPI1_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_SPI1_SRC_150M_VAR:
            u32Clk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case APS_CLK_SPI1_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;;
        case APS_CLK_SPI1_SRC_EXTERNAL:
            u32Clk = g_sApsClkFreq.u32ExtApsClk;
            break;
        default:
            return RESULT_FAIL;
    }
    u32Clk /= APS_CLK_SPI1_DIVN_GET(eDiv);
    
    AOS->SPI_CLK_SEL = (AOS->SPI_CLK_SEL & ~(AOS_SPI_CLK_SEL_SPI1_CLK_SEL_Msk | AOS_SPI_CLK_SEL_SPI1_SCLK_DIV_Msk) ) | 
                        (eSrc << AOS_SPI_CLK_SEL_SPI1_CLK_SEL_Pos) | eDiv;
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_SPI1, u32Clk);
    
    Hal_Spi_DividerUpdate(SPI_IDX_1);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_Spi2SrcSelect
 *        SPI2 clock source select
 * @param eSrc[in] Select the source of SPI2 clock tree. Refer to ::E_ApsClkSpi2Src_t
 *                  APS_CLK_SPI2_SRC_RC           
 *                  APS_CLK_SPI2_SRC_XTAL         
 *                  APS_CLK_SPI2_SRC_150M_VAR     
 *                  APS_CLK_SPI2_SRC_DECI_160M_BB 
 *                  APS_CLK_SPI2_SRC_EXTERNAL     
 * @param eSDiv[in] Select the source divider of SPI2 clock tree. Refer to ::E_ApsClkSpi2Divn_t
 *                  APS_CLK_SPI2_DIV_1 
 *                  APS_CLK_SPI2_DIV_2 
 *                  APS_CLK_SPI2_DIV_3 
 *                  APS_CLK_SPI2_DIV_4 
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_Spi2SrcSelect_impl(E_ApsClkSpi2Src_t eSrc, E_ApsClkSpi2Divn_t eDiv)
{
    uint32_t u32Clk;
    
    switch(eSrc)
    {
        case APS_CLK_SPI2_SRC_RC:
            u32Clk = CLK_SRC_FREQ_RC;
            break;
        case APS_CLK_SPI2_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_SPI2_SRC_150M_VAR:
            u32Clk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case APS_CLK_SPI2_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;;
        case APS_CLK_SPI2_SRC_EXTERNAL:
            u32Clk = g_sApsClkFreq.u32ExtApsClk;
            break;
        default:
            return RESULT_FAIL;
    }
    u32Clk /= APS_CLK_SPI2_DIVN_GET(eDiv);
    
    AOS->SPI_CLK_SEL = (AOS->SPI_CLK_SEL & ~(AOS_SPI_CLK_SEL_SPI2_CLK_SEL_Msk | AOS_SPI_CLK_SEL_SPI2_SCLK_DIV_Msk) ) | 
                        (eSrc << AOS_SPI_CLK_SEL_SPI2_CLK_SEL_Pos) | eDiv;
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_SPI2, u32Clk);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_Spi3SrcSelect
 *        SPI3 clock source select
 * @param eSrc[in] Select the source of SPI3 clock tree. Refer to ::E_ApsClkSpi3Src_t
 *                  APS_CLK_SPI3_SRC_RC           
 *                  APS_CLK_SPI3_SRC_XTAL         
 *                  APS_CLK_SPI3_SRC_150M_VAR     
 *                  APS_CLK_SPI3_SRC_DECI_160M_BB 
 *                  APS_CLK_SPI3_SRC_EXTERNAL     
 * @param eDiv[in] Select the source divider of SPI3 clock tree. Refer to ::E_ApsClkSpi3Divn_t
 *                  APS_CLK_SPI3_DIV_1 
 *                  APS_CLK_SPI3_DIV_2 
 *                  APS_CLK_SPI3_DIV_3 
 *                  APS_CLK_SPI3_DIV_4 
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_Spi3SrcSelect_impl(E_ApsClkSpi3Src_t eSrc, E_ApsClkSpi3Divn_t eDiv)
{
    uint32_t u32Clk;
    
    switch(eSrc)
    {
        case APS_CLK_SPI3_SRC_RC:
            u32Clk = CLK_SRC_FREQ_RC;
            break;
        case APS_CLK_SPI3_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_SPI3_SRC_150M_VAR:
            u32Clk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case APS_CLK_SPI3_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;;
        case APS_CLK_SPI3_SRC_EXTERNAL:
            u32Clk = g_sApsClkFreq.u32ExtApsClk;
            break;
        default:
            return RESULT_FAIL;
    }
    u32Clk /= APS_CLK_SPI3_DIVN_GET(eDiv);
    
    AOS->SPI_CLK_SEL = (AOS->SPI_CLK_SEL & ~(AOS_SPI_CLK_SEL_SPI3_CLK_SEL_Msk | AOS_SPI_CLK_SEL_SPI3_SCLK_DIV_Msk) ) | 
                        (eSrc << AOS_SPI_CLK_SEL_SPI3_CLK_SEL_Pos) | eDiv;
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_SPI3, u32Clk);
    
    Hal_Spi_DividerUpdate(SPI_IDX_3);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_I2sSrcSelect
 *        I2S clock source select
 * @param eSrc[in] Select the source of I2S clock tree. Refer to ::E_ApsClkI2sSrc_t
 *                  APS_CLK_I2S_SRC_DECI_160M_BB           
 *                  APS_CLK_I2S_SRC_XTAL 
 * @param eDiv[in] Select the source divider of I2S clock tree. Refer to ::E_ApsClkI2sDivn_t
 *                  APS_CLK_I2S_DIV_1 
 *                  APS_CLK_I2S_DIV_2 
 *                  ...... 
 *                  APS_CLK_I2S_DIV_64 
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_I2sSrcSelect_impl(E_ApsClkI2sSrc_t eSrc, E_ApsClkI2sDivn_t eDiv)
{
    uint32_t u32Clk;
    
    switch(eSrc)
    {
        case APS_CLK_I2S_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_I2S_SRC_RC:
            u32Clk = CLK_SRC_FREQ_RC;
            break;
        case APS_CLK_I2S_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;
        default:
            return RESULT_FAIL;
    }
    u32Clk /= APS_CLK_I2S_DIVN_GET(eDiv);
    
    AOS->APS_DM_CLK_SEL = (AOS->APS_DM_CLK_SEL & ~(AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_SEL_Msk | AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Msk) ) | 
                            (eSrc << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_SEL_Pos) | eDiv;
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_I2S, u32Clk);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_Uart0SrcSelect
 *        UART0 clock source select
 * @param eSrc[in] UART0 clock select. Refer to ::E_ApsClkUart0Src_t
 *                  APS_CLK_UART0_SRC_XTAL
 *                  APS_CLK_UART0_SRC_XTAL_X2
 *                  APS_CLK_UART0_SRC_DECI_160M_BB
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_Uart0SrcSelect_impl(E_ApsClkUart0Src_t eSrc)
{
    uint32_t u32Clk;
    switch(eSrc)
    {
        case APS_CLK_UART0_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_UART0_SRC_XTAL_X2:
            u32Clk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case APS_CLK_UART0_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;
        default:
            return RESULT_FAIL;
    }

    AOS->APS_DOMAIN_EN = (AOS->APS_DOMAIN_EN & ~APS_CLK_UART0_SRC_MSK) | ( eSrc << APS_CLK_UART0_SRC_POS);
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_UART0, u32Clk);
    
    Hal_Uart_ClockUpdate(UART_IDX_0);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_Uart1SrcSelect
 *        UART1 clock source select
 * @param eSrc[in] UART1 clock select. Refer to ::E_ApsClkUart1Src_t
 *                  APS_CLK_UART1_SRC_XTAL
 *                  APS_CLK_UART1_SRC_XTAL_X2
 *                  APS_CLK_UART1_SRC_DECI_160M_BB
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_Uart1SrcSelect_impl(E_ApsClkUart1Src_t eSrc)
{
    uint32_t u32Clk;
    switch(eSrc)
    {
        case APS_CLK_UART1_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_UART1_SRC_XTAL_X2:
            u32Clk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case APS_CLK_UART1_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;
        default:
            return RESULT_FAIL;
    }

    AOS->APS_DOMAIN_EN = (AOS->APS_DOMAIN_EN & ~APS_CLK_UART1_SRC_MSK) | ( eSrc << APS_CLK_UART1_SRC_POS);
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_UART1, u32Clk);
    
    Hal_Uart_ClockUpdate(UART_IDX_1);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_ApsUartSrcSelect
 *        APS_UART clock source select
 * @param eSrc[in] APS_UART clock select. Refer to ::E_ApsClkDbgUartSrc_t
 *                  APS_CLK_DBG_UART_SRC_XTAL
 *                  APS_CLK_DBG_UART_SRC_XTAL_X2
 *                  APS_CLK_DBG_UART_SRC_DECI_160M_BB
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_ApsUartSrcSelect_impl(E_ApsClkDbgUartSrc_t eSrc)
{
    uint32_t u32Clk;
    switch(eSrc)
    {
        case APS_CLK_DBG_UART_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_DBG_UART_SRC_XTAL_X2:
            u32Clk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case APS_CLK_DBG_UART_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;
        default:
            return RESULT_FAIL;
    }

    AOS->APS_DOMAIN_EN = (AOS->APS_DOMAIN_EN & ~APS_CLK_APS_UART_SRC_MSK) | ( eSrc << APS_CLK_APS_UART_SRC_POS);
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_APS_UART, u32Clk);
    
    Hal_Uart_ClockUpdate(UART_IDX_DBG);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_UartAbrSrcSelect
 *        UART ABR clock source select
 * @param eSrc[in] UART ABR clock select. Refer to ::E_ApsClkUartAbrSrc_t
 *                  APS_CLK_UART_ABR_SRC_XTAL
 *                  APS_CLK_UART_ABR_SRC_XTAL_X2
 *                  APS_CLK_UART_ABR_SRC_DECI_160M_BB
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_UartAbrSrcSelect_impl(E_ApsClkUartAbrSrc_t eSrc)
{
    uint32_t u32Clk;
    switch(eSrc)
    {
        case APS_CLK_UART_ABR_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_UART_ABR_SRC_XTAL_X2:
            u32Clk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case APS_CLK_UART_ABR_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;
        default:
            return RESULT_FAIL;
    }

    AOS->APS_DOMAIN_EN = (AOS->APS_DOMAIN_EN & ~APS_CLK_UART_ABR_SRC_MSK) | ( eSrc << APS_CLK_UART_ABR_SRC_POS);
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_UART_ABR, u32Clk);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_PdmSrcSelect
 *        PDM and PDM IO clock source select
 * @param ePdmSrc[in] Select the source and divider of PDM clock tree. Refer to ::E_ApsClkPdmSrc_t
 *                  APS_CLK_PDM_SRC_DECI_160M_BB_DIV_1
 *                  APS_CLK_PDM_SRC_DECI_160M_BB_DIV_2
 *                  APS_CLK_PDM_SRC_DECI_160M_BB_DIV_3
 *                  APS_CLK_PDM_SRC_DECI_160M_BB_DIV_4
 *                  APS_CLK_PDM_SRC_XTAL
 * @param ePdmIoSrc[in] Select the source of PDM clock tree. Refer to ::E_ApsClkPdmIoSrc_t
 *                  APS_CLK_PDM_IO_SRC_DECI_160M_BB           
 *                  APS_CLK_PDM_IO_SRC_XTAL 
 * @param ePdmIoDiv[in] Select the source divider of I2S clock tree. Refer to ::E_ApsClkPdmIoDivn_t
 *                  APS_CLK_PDM_IO_DIV_1 
 *                  APS_CLK_PDM_IO_DIV_2 
 *                  ...... 
 *                  APS_CLK_PDM_IO_DIV_256 
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_PdmSrcSelect_impl(E_ApsClkPdmSrc_t ePdmSrc, E_ApsClkPdmIoSrc_t ePdmIoSrc, E_ApsClkPdmIoDivn_t ePdmIoDiv)
{
    uint32_t u32PdmClk, u32PdmIoClk;
    
    if ((ePdmSrc & AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_SEL_Msk) == APS_CLK_PDM_SRC_XTAL)
    {   /* From XTAL */
        u32PdmClk = g_sApsClkFreq.u32XtalClk;
    }
    else
    {   /* From 160M BB */
        u32PdmClk = CLK_SRC_FREQ_160M_BB / APS_CLK_PDM_DIVN_GET(ePdmSrc);
    }
    
    switch(ePdmIoSrc)
    {
        case APS_CLK_PDM_IO_SRC_DECI_160M_BB:
            u32PdmIoClk = CLK_SRC_FREQ_160M_BB;
            break;
        case APS_CLK_PDM_IO_SRC_XTAL:
            u32PdmIoClk = g_sApsClkFreq.u32XtalClk;
            break;
        default:
            return RESULT_FAIL;
    }
    
    u32PdmIoClk /= APS_CLK_PDM_IO_DIVN_GET(ePdmIoDiv);
    
    AOS->APS_DM_CLK_SEL = (AOS->APS_DM_CLK_SEL & ~(APS_CLK_PDM_SRC_MSK | AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_SEL_Msk | AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Msk) ) | 
                            ePdmSrc | (ePdmIoSrc << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_SEL_Pos) | ePdmIoDiv;
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_PDM, u32PdmClk);
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_PDM_IO, u32PdmIoClk);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_Ext2IoSetup
 *        Setup EXT 2 IO clock. This feature can output clock to IO pad.
 * @param eSrc[in] Select the source of EXT2IO clock. Refer to ::E_ApsClkExt2IoSrc_t
 *                  APS_CLK_EXT_2_IO_SRC_XTAL           
 *                  APS_CLK_EXT_2_IO_SRC_RC 
 * @param eDiv[in] Select the source divider of EXT2IO clock. Refer to ::E_ApsClkExt2IoDivn_t
 *                  APS_CLK_EXT_2_IO_DIV_1
 *                  APS_CLK_EXT_2_IO_DIV_2
 *                  APS_CLK_EXT_2_IO_DIV_4
 *                  APS_CLK_EXT_2_IO_DIV_8 
 * @param eEn[in] Select the EXT2IO enable source. Refer to ::E_ApsClkExt2IoEn_t
 *                  APS_CLK_EXT_2_IO_EN_SW_ON  
 *                  APS_CLK_EXT_2_IO_EN_SW_OFF 
 *                  APS_CLK_EXT_2_IO_EN_PAD    
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_Ext2IoSetup_impl(E_ApsClkExt2IoSrc_t eSrc, E_ApsClkExt2IoDivn_t eDiv, E_ApsClkExt2IoEn_t eEn)
{
    
    AOS->APS_DOMAIN_EN = (AOS->APS_DOMAIN_EN & ~(AOS_APS_DOMAIN_EN_EXT_2IO_CLK_SEL_Msk | AOS_APS_DOMAIN_EN_EXT_2IO_CLK_DIV_Msk | (uint32_t)APS_CLK_EXT_2_IO_EN_MSK) ) | 
                            ((eSrc << AOS_APS_DOMAIN_EN_EXT_2IO_CLK_SEL_Pos) | eDiv | eEn);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_CamXvClkSrcSelect
 *        CAM XV clock source select
 * @param eSrc[in] Select the source of CAM XV clock tree. Refer to ::E_ApsClkCamXvClkSrc_t
 *                  APS_CLK_CAM_XVCLK_SRC_XTAL
 *                  APS_CLK_CAM_XVCLK_SRC_XTAL_X2
 *                  APS_CLK_CAM_XVCLK_SRC_150M_VAR
 *                  APS_CLK_CAM_XVCLK_SRC_DECI_160M_BB
 *                  APS_CLK_CAM_XVCLK_SRC_EXTERNAL
 * @param eDiv[in] Select the source divider of CAM XV clock tree. Refer to ::E_ApsClkCamXvCLKDivn_t
 *                  APS_CLK_CAM_XVCLK_DIV_1 
 *                  APS_CLK_CAM_XVCLK_DIV_2 
 *                  APS_CLK_CAM_XVCLK_DIV_3 
 *                  APS_CLK_CAM_XVCLK_DIV_4 
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_CamXvClkSrcSelect_impl(E_ApsClkCamXvClkSrc_t eSrc, E_ApsClkCamXvCLKDivn_t eDiv)
{
    uint32_t u32Clk;
    
    switch(eSrc)
    {
        case APS_CLK_CAM_XVCLK_SRC_XTAL:
            u32Clk = g_sApsClkFreq.u32XtalClk;
            break;
        case APS_CLK_CAM_XVCLK_SRC_XTAL_X2:
            u32Clk = g_sApsClkFreq.u32XtalClk * 2;
            break;
        case APS_CLK_CAM_XVCLK_SRC_150M_VAR:
            u32Clk = CLK_SRC_FREQ_150M_VAR;
            break;
        case APS_CLK_CAM_XVCLK_SRC_DECI_160M_BB:
            u32Clk = CLK_SRC_FREQ_160M_BB;
            break;
        case APS_CLK_CAM_XVCLK_SRC_EXTERNAL:
            u32Clk = g_sApsClkFreq.u32ExtApsClk;
            break;
        default:
            return RESULT_FAIL;
    }
    u32Clk /= APS_CLK_CAM_XVCLK_DIVN_GET(eDiv);
    
    AOS->CAM_XVCLK_SEL = (AOS->CAM_XVCLK_SEL & ~(AOS_CAM_XVCLK_SEL_CAM_XVCLKSEL_Msk | AOS_CAM_XVCLK_SEL_CAM_XVCLK_DIVN_Msk) ) | 
                        (eSrc << AOS_CAM_XVCLK_SEL_CAM_XVCLKSEL_Pos) | eDiv;
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_CAM_XVCLK, u32Clk);
    
    return RESULT_SUCCESS;
}




/* ******************************* Clock modulator ****************************** */
/**
 * @brief Hal_Sys_ApsClkMmFactorSet
 *        Set APS all clocks slow-down. Slow down Clk = Orig Clk * u32Factor/16
 * @warning When enable MmFactor, the WDT counter will be reset to target value,
 *          and I2C will be disable and then re-initialized with new clock setting.
 *
 * @param u32Factor[in] Factor of slow-down function. Max:0x10
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_ApsClkMmFactorSet_impl(uint32_t u32Factor)
{    
    if(u32Factor>APS_MMFACTOR_MAX)
    {
        // Out of range
        return RESULT_FAIL;
    }else{
        // 0 ~ MAX, Slow-down APS clock

        // Set factor
        AOS->CLK_MMFACTOR_APS &= ~AOS_CLK_MMFACTOR_APS_FACTOR_Msk;
        AOS->CLK_MMFACTOR_APS |= u32Factor;

        // Make a rising edge to trigger
        AOS->CLK_MMFACTOR_APS &= ~AOS_CLK_MMFACTOR_APS_ENABLE;
        AOS->CLK_MMFACTOR_APS |= AOS_CLK_MMFACTOR_APS_ENABLE;
        
        g_sApsClkFreq.u32MMFactor = u32Factor;
    }

    // Update system clock divider
    g_sApsClkFreq.u32SysClk = g_sApsClkFreq.u32SysSrcClk * g_sApsClkFreq.u32MMFactor / APS_MMFACTOR_DIVISOR;
    SystemCoreClockSet(g_sApsClkFreq.u32SysClk);
    
    // Apply to all relative modules
    Hal_Sys_ApsClkChangeApply();
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_ApsClkMmFactorGet
 *        Get MMD factor value
 * @return MMDF value
 */
uint32_t Hal_Sys_ApsClkMmFactorGet_impl(void)
{
    return g_sApsClkFreq.u32MMFactor;
}



/*
 *************************************************************************
 *                           Public Functions
 *************************************************************************
 */

/**
 * @brief Hal_RCC_PreInit
 *        Initialize Reset and clock function pointers
 */
void Hal_RCC_PreInit(void)
{
    Hal_Sys_ClkInit(CLK_SRC_FREQ_XTAL);
}


/**
 * @brief Hal_Sys_ResetStatusRead
 *        Read the source of previous reset event. Refer to ::E_RstSrc_t
 * @return Reset sources, each bit for the reset sources.
 */
uint32_t Hal_Sys_ResetStatusRead(void)
{
    return AOS->RSTS;
}

/**
 * @brief Hal_Sys_ResetStatusClear
 *        Clear the source of previous reset event. Refer to E_RstSrc_t
 * @param u32Sources[in] each bit for the reset sources. Refer to E_RstSrc_t
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Sys_ResetStatusClear(uint32_t u32Sources)
{
    AOS->RSTS = u32Sources;
    return RESULT_SUCCESS;
}
