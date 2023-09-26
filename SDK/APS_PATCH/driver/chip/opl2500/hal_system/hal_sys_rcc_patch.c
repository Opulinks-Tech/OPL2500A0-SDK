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
 *  @file hal_sys_rcc_patch.c
 * 
 *  @brief reset and clock control patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_sys_rcc.h"
#include "hal_system.h"
#include "hal_i2c.h"
#include "hal_wdt.h"
#include "hal_spi.h"
#include "hal_qspi.h"
#include "ps.h"
#include "boot_sequence.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

#define AOS_SPI_123_CLK_SEL_APS                     (AOS_SPI_CLK_SEL_SPI1_PCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI2_PCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI3_PCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI1_SCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI2_SCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI3_SCLK_EN)   
                                                     
#define SYS_UART_SCLK_FON_MSK                       (SYS_UART_SCLK_FON_UART0_CLK_FON_Msk | \
                                                     SYS_UART_SCLK_FON_UART1_CLK_FON_Msk | \
                                                     SYS_UART_SCLK_FON_APS_UART_CLK_FON_Msk | \
                                                     SYS_UART_SCLK_FON_MSQ_UART_CLK_FON_Msk)
#define SYS_UART_SCLK_FON_SET_VALUE                 (0)

#define AOS_APS_STCLK_VALUE                         ( (1 << AOS_APS_STCLK_APS_STCLK_SRC_EN_Pos)  |   \
                                                      (0 << AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos) |   \
                                                      (0x1000000 << AOS_APS_STCLK_APS_STCALIB_Pos) )
                                                      
#define configTICK_RATE_HZ                          1000



#define HAL_SLP_CLK_APS_CLK_SEL_MSK     (AOS_APS_CLK_SEL_APS_CLK_DIVN_UNGATED | \
                                         AOS_APS_CLK_SEL_APS_PCLK_DIV_SEL | \
                                         AOS_APS_CLK_SEL_APS_PCLK_DIVN_UNGATED | \
                                         AOS_APS_CLK_SEL_I2C_PCLK_EN | \
                                         AOS_APS_CLK_SEL_TMR0_PCLK_EN | \
                                         AOS_APS_CLK_SEL_TMR1_PCLK_EN | \
                                         AOS_APS_CLK_SEL_APS_CLK_DIVN_UNGATED | \
                                         AOS_APS_CLK_SEL_UART0_PCLK_EN | \
                                         AOS_APS_CLK_SEL_UART1_PCLK_EN | \
                                         AOS_APS_CLK_SEL_APS_UART_PCLK_EN | \
                                         AOS_APS_CLK_SEL_APS_DMA_HCLK_EN | \
                                         AOS_APS_CLK_SEL_I2S_PCLK_EN | \
                                         AOS_APS_CLK_SEL_JTAG_HCLK_EN)
#define HAL_SLP_CLK_APS_CLK_SEL_SET_VALUE  (0)

#define HAL_SLP_CLK_APS_DOMAIN_EN_MSK   (AOS_APS_DOMAIN_EN_PWM_CLK_EN | \
                                         AOS_APS_DOMAIN_EN_UART_ABR_CLK_EN | \
                                         AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN_HW | \
                                         AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN)
#define HAL_SLP_CLK_APS_DOMAIN_EN_SET_VALUE  (0)

#define HAL_SLP_CLK_SCRT_CLK_APS_MSK    (AOS_SCRT_CLK_APS_SCRT_SLV_CLK_APS_EN | \
                                         AOS_SCRT_CLK_APS_SCRT_GLB_CLK_APS_EN | \
                                         AOS_SCRT_CLK_APS_APSSCRT_HCLK_APS_EN | \
                                         AOS_SCRT_CLK_APS_SCRT_PCLK_APS_EN | \
                                         AOS_SCRT_CLK_APS_SCRT_CORE_CLK_EN | \
                                         AOS_SCRT_CLK_APS_APSSCRT_CLK_DIVN_EN)
#define HAL_SLP_CLK_SCRT_CLK_APS_SET_VALUE    (0)

#define HAL_SLP_CLK_APS_STCLK_MSK       (AOS_APS_STCLK_APS_STCLK_SRC_EN)
#define HAL_SLP_CLK_APS_STCLK_SET_VALUE       (0)

#define HAL_SLP_CLK_SPI_CLK_SEL_MSK     (AOS_SPI_CLK_SEL_SPI0_HCLK_EN | \
                                         AOS_SPI_CLK_SEL_SPI0_PCLK_EN | \
                                         AOS_SPI_CLK_SEL_SPI1_PCLK_EN | \
                                         AOS_SPI_CLK_SEL_SPI2_PCLK_EN | \
                                         AOS_SPI_CLK_SEL_SPI3_PCLK_EN | \
                                         AOS_SPI_CLK_SEL_SPI0_REFCLK_EN | \
                                         AOS_SPI_CLK_SEL_SPI1_SCLK_EN | \
                                         AOS_SPI_CLK_SEL_SPI2_SCLK_EN | \
                                         AOS_SPI_CLK_SEL_SPI3_SCLK_EN)
#define HAL_SLP_CLK_SPI_CLK_SEL_SET_VALUE   (0)

#define HAL_SLP_CLK_APS_DM_CLK_SEL_MSK  (AOS_APS_DM_CLK_SEL_I2S_SCLK_EN | \
                                         AOS_APS_DM_CLK_SEL_APSXIP_CACHE_HCLK_EN | \
                                         AOS_APS_DM_CLK_SEL_APSXIP_PREFT_HCLK_EN | \
                                         AOS_APS_DM_CLK_SEL_AUX_DSADC_PCLK_EN | \
                                         AOS_APS_DM_CLK_SEL_AUX_DSADC_CLK_EN | \
                                         AOS_APS_DM_CLK_SEL_PDM_IO_CLK_EN | \
                                         AOS_APS_DM_CLK_SEL_PDM_CLK_DIV_EN | \
                                         AOS_APS_DM_CLK_SEL_PDM_CLK_EN | \
                                         AOS_APS_DM_CLK_SEL_PDM_PCLK_EN)
#define HAL_SLP_CLK_APS_DM_CLK_SEL_SET_VALUE    (0)

#define HAL_SLP_CLK_CAM_XVCLK_SEL_MSK   (AOS_CAM_XVCLK_SEL_CAM_XVCLK_EN)
#define HAL_SLP_CLK_CAM_XVCLK_SEL_SET_VALUE     (0)



#define DET_XTAL32_EXIST_CYCLE          (4)         /* 32k xtal cycles */
#define DET_XTAL32_CYCLE                (64)        /* 32k xtal cycles */
#define DET_XTAL32_TIMEOUT              0x8000


/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef struct
{
    uint32_t u32ApsClkSel;      /* AOS->APS_CLK_SEL */
    uint32_t u32ApsDomainEn;    /* AOS->APS_DOMAIN_EN */
    uint32_t u32ScrtClkAps;     /* AOS->SCRT_CLK_APS */
    uint32_t u32ApsStClk;       /* AOS->APS_STCLK */
    uint32_t u32SpiClkSel;      /* AOS->SPI_CLK_SEL */
    uint32_t u32ApsDmClkSel;    /* AOS->APS_DM_CLK_SEL */
    uint32_t u32CamXvclkSel;    /* AOS->CAM_XVCLK_SEL */
    
} S_HAL_APS_CLK_STORAGE;
/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/

void Hal_Sys_UnusedModuleDis_patch(void);
uint32_t Hal_Sys_Ext2IoSetup_patch(E_ApsClkExt2IoSrc_t eSrc, E_ApsClkExt2IoDivn_t eDiv, E_ApsClkExt2IoEn_t eEn);
uint32_t Hal_Sys_RcHighEn_patch(E_ClkSrcRcHigh eEn);
uint32_t Hal_Sys_XtalHighEn_patch(E_ClkSrcXtalHigh eEn);
uint32_t Hal_Sys_ApsClkTreeSetup_patch(E_ApsClkSysSrc_t eSrc, E_ApsClkSrcDivn_t eSysDiv, E_ApsClkPclkDivn_t ePclkDiv);
void Hal_Sys_ApsClkChangeApply_patch(void);
uint32_t Hal_Sys_ApsClkMmFactorSet_patch(uint32_t u32Factor);
void Hal_Sys_WakeupClkResume_patch(void);
void Hal_Sys_SleepClkSetup_patch(void);
uint32_t Hal_Sys_ApsClockUpdate_patch(E_ApsClkGrp_t eApsClkGrp, uint32_t u32ClkFreq);
uint32_t Hal_Sys_Spi0SrcSelect_patch(E_ApsClkSpi0Src_t eSrc, E_ApsClkSpi0Divn_t eDiv);
uint32_t Hal_Sys_Spi1SrcSelect_patch(E_ApsClkSpi1Src_t eSrc, E_ApsClkSpi1Divn_t eDiv);
uint32_t Hal_Sys_Spi2SrcSelect_patch(E_ApsClkSpi2Src_t eSrc, E_ApsClkSpi2Divn_t eDiv);
uint32_t Hal_Sys_Spi3SrcSelect_patch(E_ApsClkSpi3Src_t eSrc, E_ApsClkSpi3Divn_t eDiv);
uint32_t Hal_Sys_CamXvClkSrcSelect_patch(E_ApsClkCamXvClkSrc_t eSrc, E_ApsClkCamXvCLKDivn_t eDiv);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

extern S_APS_CLK_FREQ g_sApsClkFreq;
S_APS_CLK_FREQ_EXT g_sApsClkFreqExt = {150000000};

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 
S_HAL_APS_CLK_STORAGE g_Hal_SysApsClkStorage;
 
 
 
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Sys_RccPatchInit(void)
{
    g_sApsClkFreq.u32StClk  = g_sApsClkFreq.u32SysClk;
    AOS->APS_STCLK |= APS_STCLK_SKEW;
    
    
    Hal_Sys_UnusedModuleDis = Hal_Sys_UnusedModuleDis_patch;
    Hal_Sys_Ext2IoSetup = Hal_Sys_Ext2IoSetup_patch;
    Hal_Sys_RcHighEn = Hal_Sys_RcHighEn_patch;
    Hal_Sys_XtalHighEn = Hal_Sys_XtalHighEn_patch;
    Hal_Sys_ApsClkTreeSetup = Hal_Sys_ApsClkTreeSetup_patch;
    Hal_Sys_ApsClkChangeApply = Hal_Sys_ApsClkChangeApply_patch;
    Hal_Sys_ApsClkMmFactorSet = Hal_Sys_ApsClkMmFactorSet_patch;
    Hal_Sys_WakeupClkResume = Hal_Sys_WakeupClkResume_patch;
    Hal_Sys_SleepClkSetup = Hal_Sys_SleepClkSetup_patch;
    Hal_Sys_ApsClockUpdate = Hal_Sys_ApsClockUpdate_patch;
    Hal_Sys_Spi0SrcSelect = Hal_Sys_Spi0SrcSelect_patch;
    Hal_Sys_Spi1SrcSelect = Hal_Sys_Spi1SrcSelect_patch;
    Hal_Sys_Spi2SrcSelect = Hal_Sys_Spi2SrcSelect_patch;
    Hal_Sys_Spi3SrcSelect = Hal_Sys_Spi3SrcSelect_patch;
    Hal_Sys_CamXvClkSrcSelect = Hal_Sys_CamXvClkSrcSelect_patch;
}


/**
 * @brief Hal_Sys_DetectXtal32Enable
 *        Using XTAL detection to detect 32k XTAL exist or not
 */
void Hal_Sys_Xtal32DetectStart(void)
{
    uint32_t Det32kTimeout;uint32_t XtalFreq;
            
    /* Setup RTC source to 32k XTAL */
    
    AOS->OSC_SEL = AOS_OSC_SEL_32K_OSC_SEL | AOS_OSC_SEL_RTC_EN;
    Hal_Sys_ApsClockGet(APS_CLK_GRP_XTAL, &XtalFreq);
    Det32kTimeout = DET_XTAL32_EXIST_CYCLE * XtalFreq / CLK_SRC_FREQ_32K_XTAL;
    
    /* Start to detect */
    SYS->DET_XTAL = (Det32kTimeout << SYS_DET_XTAL_PRE_DET_32K_TO_CNT_Pos) |
                    (DET_XTAL32_CYCLE << SYS_DET_XTAL_DET_32K_CNT_Pos) |
                    SYS_DET_XTAL_DET_XTAL_CLK_EN |
                    SYS_DET_XTAL_DET_XTAL_EN;
}


void Hal_Sys_Xtal32DetectEnd(void)
{
    uint32_t u32Timeout = DET_XTAL32_TIMEOUT;
    S_RTC_CLK_CFG sRtcCfg;

    while(--u32Timeout)
    {
        if (SYS->DET_XTAL_STS & SYS_DET_XTAL_STS_PRE_DET_32K_DONE)
            break;
    }
    
    if (!u32Timeout || !(SYS->DET_XTAL_STS & SYS_DET_XTAL_STS_PRE_DET_32K_ACTIVE))
    {   /* Cannot get 32k XTAL, select 32k RC as RTC source */
        sRtcCfg.eRtcSrc = MSQ_CLK_RTC_ON_LPO;
        sRtcCfg.eLpoCtrl = CLK_LPO_CTRL_BY_SEQ;
        sRtcCfg.eXtal32kCtrl = XTAL_32k_OFF;
    }
    else
    {   /* No 32k XTAL */
        sRtcCfg.eRtcSrc = MSQ_CLK_RTC_ON_32K_XTAL;
        sRtcCfg.eLpoCtrl = CLK_LPO_CTRL_OFF;
        sRtcCfg.eXtal32kCtrl = XTAL_32k_BY_SEQ;
        Boot_RtcSelect32kRc();
    }
    SYS->DET_XTAL &= ~(SYS_DET_XTAL_DET_XTAL_EN | SYS_DET_XTAL_DET_XTAL_CLK_EN);
    __ISB();
    sRtcCfg.u8XtalGm = 0xF;
    sRtcCfg.u8XtalCap = 0x0;
    Hal_Sys_RtcClkCfg(&sRtcCfg);
}

void Hal_Sys_Xtal32CalcStart(void)
{
    /* Start to detect */
    SYS->DET_XTAL = (DET_XTAL32_CYCLE << SYS_DET_XTAL_DET_32K_CNT_Pos) |
                        SYS_DET_XTAL_DET_XTAL_CLK_EN |
                        SYS_DET_XTAL_DET_XTAL_EN |
                        SYS_DET_XTAL_PRE_DET_32K_BYPS;
}

void Hal_Sys_Xtal32CalcEnd(void)
{
    double tick32;
    uint32_t XtalCnt=0;
    uint32_t XtalFreq=0;
    uint32_t u32Timeout = DET_XTAL32_TIMEOUT;
    
    while (--u32Timeout)
    {   /* Not reset timeout value, continue counting */
        if (SYS->DET_XTAL_STS & (SYS_DET_XTAL_STS_DET_XTAL_DONE | SYS_DET_XTAL_STS_DET_XTAL_DISTORTION))
        {
            break;
        }
    }
    if (u32Timeout && !(SYS->DET_XTAL_STS & SYS_DET_XTAL_STS_DET_XTAL_DISTORTION))
    {
        Hal_Sys_ApsClockGet(APS_CLK_GRP_XTAL, &XtalFreq);
        XtalCnt = (SYS->DET_XTAL_STS & SYS_DET_XTAL_STS_DET_XTAL_CNT_Msk) >> SYS_DET_XTAL_STS_DET_XTAL_CNT_Pos;
        
        tick32 = (double)XtalCnt / (DET_XTAL32_CYCLE * ((XtalFreq + 1000000/2) / 1000000));
        ps_update_xtal32_tick_value(tick32);
    }
    SYS->DET_XTAL &= ~(SYS_DET_XTAL_DET_XTAL_EN | SYS_DET_XTAL_DET_XTAL_CLK_EN);
    
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

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
 *      - SPI 1/2/3
 *    [APS_DM_CLK_SEL]
 *      - I2S
 *      - XIP Cache, Prefetch
 *      - PDM
 *    [CAM_XVCLK_SEL]
 *      - XVCLK
 */
void Hal_Sys_UnusedModuleDis_patch(void)
{
    uint32_t u32DisApsClk;
    
    u32DisApsClk = AOS_APS_CLK_SEL_ALL_APS & ~AOS_APS_CLK_SEL_APS_UART_PCLK_EN;
    if (Hal_Sys_StrapModeRead() != STRAP_MODE_NORMAL)
        u32DisApsClk &= ~AOS_APS_CLK_SEL_JTAG_HCLK_EN;
    AOS->APS_CLK_SEL &= ~u32DisApsClk;

    AOS->APS_STCLK = AOS_APS_STCLK_VALUE;
    
    AOS->SCRT_CLK_APS &= ~AOS_SCRT_CLK_APS_SEL_ALL_APS;    
    AOS->APS_DOMAIN_EN &= ~AOS_APS_DOMAIN_EN_ALL_APS;
    AOS->SPI_CLK_SEL &= ~AOS_SPI_123_CLK_SEL_APS;
    AOS->APS_DM_CLK_SEL &= ~AOS_APS_DM_CLK_UNSED_MODULES;

    AOS->CAM_XVCLK_SEL &= ~AOS_CAM_XVCLK_CLK_APS;
    
    SYS->UART_SCLK_FON = (SYS->UART_SCLK_FON & ~SYS_UART_SCLK_FON_MSK) | SYS_UART_SCLK_FON_SET_VALUE;
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
uint32_t Hal_Sys_Ext2IoSetup_patch(E_ApsClkExt2IoSrc_t eSrc, E_ApsClkExt2IoDivn_t eDiv, E_ApsClkExt2IoEn_t eEn)
{
    AOS->APS_DOMAIN_EN |= AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN;
    Hal_Sys_Ext2IoSetup_impl(eSrc, eDiv, eEn);
    
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
uint32_t Hal_Sys_RcHighEn_patch(E_ClkSrcRcHigh eEn)
{
    if (eEn == RC_HIGH_ON)
    {
        //RF->CLKGEN_CTL0 |= RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB;
        RF->CK_GATE_CTRL = RF_CK_GATE_CTRL_RC_BB_SET;
        RF->PU_VAL |= RF_PU_VAL_PU_RC_OSC;
    }
    else
    {
        //RF->CLKGEN_CTL0 &= ~RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB;
        RF->CK_GATE_CTRL = RF_CK_GATE_CTRL_RC_BB_CLEAR;
        RF->PU_VAL &= ~RF_PU_VAL_PU_RC_OSC;
    }

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
uint32_t Hal_Sys_XtalHighEn_patch(E_ClkSrcXtalHigh eEn)
{
    if (eEn == XTAL_HIGH_ON)
    {
        //RF->CLKGEN_CTL0 |= RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB;
        RF->CK_GATE_CTRL = RF_CK_GATE_CTRL_XTAL_BB_SET;
        RF->PU_VAL |= RF_PU_VAL_PU_XTAL;
    }
    else
    {
        //RF->CLKGEN_CTL0 &= ~RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB;
        RF->CK_GATE_CTRL = RF_CK_GATE_CTRL_XTAL_BB_CLEAR;
        RF->PU_VAL &= ~RF_PU_VAL_PU_XTAL;
    }
    return RESULT_SUCCESS;
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
uint32_t Hal_Sys_ApsClkTreeSetup_patch(E_ApsClkSysSrc_t eSrc, E_ApsClkSrcDivn_t eSysDiv, E_ApsClkPclkDivn_t ePclkDiv)
{
    uint32_t u32SysClk = 0, u32Pclk = 0;
    uint8_t u8Div = 0;
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
            u8Div = Hal_Sys_VarSrcDiv_Get( VAR_SRC_150M );
            u32SysClk = DIV_ROUND( 2442, u8Div )*1000*1000; // Rounded in MHz level
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
void Hal_Sys_ApsClkChangeApply_patch(void)
{
    // I2C
    if (AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_I2C_PCLK_EN)
        Hal_I2c_UpdateClk();
        
    // WDT
    if (AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_WDT_PCLK_EN)
        Hal_Wdt_UpdateClk();
}

uint32_t Hal_Sys_StclkSetup(E_FUNC_ST eEnable, E_ApsClkStDivn_t eDiv)
{
    uint32_t u32Clk;
    
    u32Clk = g_sApsClkFreq.u32XtalClk / APS_CLK_ST_DIVN_GET(eDiv) + configTICK_RATE_HZ - 1;   /* compensate for OS tick division */
    
    AOS->APS_STCLK = (AOS->APS_STCLK & ~(AOS_APS_STCLK_APS_STCLK_SRC_SEL_Msk |AOS_APS_STCLK_APS_STCLK_SRC_EN_Msk)) | 
                     eDiv |
                     ((eEnable == ENABLE)?AOS_APS_STCLK_APS_STCLK_SRC_EN:0);
    
    Hal_Sys_ApsClockUpdate(APS_CLK_GRP_ST, u32Clk);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_VarSrcDiv_Set
 *        Set the divider of Var_150/180/200
 */
uint32_t Hal_Sys_VarSrcDiv_Set( E_ApsVarSrc_t eVarsrc, uint8_t u8Div )
{
    uint32_t u32Addr, u32Value;

    if( ( u8Div > VAR_SRC_DIV_MAX) || (u8Div < VAR_SRC_DIV_MIN) )
        return RESULT_FAIL;

    switch( eVarsrc )
    {
        case VAR_SRC_150M: u32Addr = (uint32_t)&(AOS->CLK150M_CTRL); break;
        case VAR_SRC_180M: u32Addr = (uint32_t)&(AOS->CLK180M_CTRL); break;
        case VAR_SRC_200M: u32Addr = (uint32_t)&(AOS->CLK200M_CTRL); break;

        default:
            return RESULT_FAIL;
    }
    u32Value = reg_read( u32Addr );

    if(u8Div <= VAR_SRC_DIV_BYPASS_TH)
    {
        u32Value = ( u32Value & ~(AOS_CLK150M_CTRL_CLK_150M_VAR_SEL_Msk | AOS_CLK150M_CTRL_BYPASS_STAGE4_CLK150M_Msk) ) |
                   (0x1         << AOS_CLK150M_CTRL_BYPASS_STAGE4_CLK150M_Pos) |
                   ((u8Div - 8) << AOS_CLK150M_CTRL_CLK_150M_VAR_SEL_Pos);
    }else{
        u32Value = ( u32Value & ~AOS_CLK150M_CTRL_CLK_150M_VAR_SEL_Msk ) |
                   (0x0          << AOS_CLK150M_CTRL_BYPASS_STAGE4_CLK150M_Pos) |
                   ((u8Div - 16) << AOS_CLK150M_CTRL_CLK_150M_VAR_SEL_Pos);
    }
    reg_write(u32Addr, u32Value);
    
    g_sApsClkFreqExt.u32Var150Clk = DIV_ROUND( 2442, u8Div )*1000*1000; // Rounded in MHz level
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Sys_VarSrcDiv_Get
 *        Get the divider of Var_150/180/200
 */
uint8_t Hal_Sys_VarSrcDiv_Get( E_ApsVarSrc_t eVarsrc )
{
    uint32_t u32Addr, u32Value;

    switch( eVarsrc )
    {
        case VAR_SRC_150M: u32Addr = (uint32_t)&(AOS->CLK150M_CTRL); break;
        case VAR_SRC_180M: u32Addr = (uint32_t)&(AOS->CLK180M_CTRL); break;
        case VAR_SRC_200M: u32Addr = (uint32_t)&(AOS->CLK200M_CTRL); break;

        default:
            return 0xFF;
    }
    u32Value = reg_read( u32Addr );

    if( u32Value & AOS_CLK150M_CTRL_BYPASS_STAGE4_CLK150M_Msk )
    {
        return VAR_SRC_DIV_MIN + ((u32Value & AOS_CLK150M_CTRL_CLK_150M_VAR_SEL_Msk) >> AOS_CLK150M_CTRL_CLK_150M_VAR_SEL_Pos);
    }else{
        return (VAR_SRC_DIV_BYPASS_TH + 1) + ((u32Value & AOS_CLK150M_CTRL_CLK_150M_VAR_SEL_Msk) >> AOS_CLK150M_CTRL_CLK_150M_VAR_SEL_Pos);
    }
}

/**
 * @brief Hal_Sys_Var150Src_PeriCheck
*        Check src: Var_150 was used by peripheral
 */
uint8_t Hal_Sys_Var150Src_PeriCheck( void )
{
    uint32_t u32Temp = 0;

    u32Temp = AOS->SPI_CLK_SEL;
    /* SPI 0 */
    if( ( ( u32Temp & AOS_SPI_CLK_SEL_SPI0_CLKSEL_Msk)  >> AOS_SPI_CLK_SEL_SPI0_CLKSEL_Pos ) == APS_CLK_SPI0_SRC_150M_VAR )
        return 1;
    /* SPI 1 */
    if( ( ( u32Temp & AOS_SPI_CLK_SEL_SPI1_CLK_SEL_Msk) >> AOS_SPI_CLK_SEL_SPI1_CLK_SEL_Pos ) == APS_CLK_SPI1_SRC_150M_VAR )
        return 1;
    /* SPI 2 */
    if( ( ( u32Temp & AOS_SPI_CLK_SEL_SPI2_CLK_SEL_Msk) >> AOS_SPI_CLK_SEL_SPI2_CLK_SEL_Pos ) == APS_CLK_SPI2_SRC_150M_VAR )
        return 1;
    /* SPI 3 */
    if( ( ( u32Temp & AOS_SPI_CLK_SEL_SPI3_CLK_SEL_Msk) >> AOS_SPI_CLK_SEL_SPI3_CLK_SEL_Pos ) == APS_CLK_SPI3_SRC_150M_VAR )
        return 1;

    u32Temp = AOS->CAM_XVCLK_SEL;
    /* CAM */
    if( ( ( u32Temp & AOS_CAM_XVCLK_SEL_CAM_XVCLKSEL_Msk)  >> AOS_CAM_XVCLK_SEL_CAM_XVCLKSEL_Pos ) == APS_CLK_CAM_XVCLK_SRC_150M_VAR )
        return 1;

    return 0;
}

/**
 * @brief Hal_Sys_Var180Src_PeriCheck
*        Check src: Var_180 was used by peripheral
 */
uint8_t Hal_Sys_Var180Src_PeriCheck( void )
{
    uint32_t u32Temp = 0;

    u32Temp = AOS->SCRT_CLK_APS;
    /* SECURE */
    if( ( ( u32Temp & AOS_SCRT_CLK_APS_SCRT_SRC_CLKSEL_Msk)  >> AOS_SCRT_CLK_APS_SCRT_SRC_CLKSEL_Pos ) == ASP_CLK_SCRT_SRC_D1_180M )
        return 1;

    return 0;
}

/**
 * @brief Hal_Sys_Var200Src_PeriCheck
*        Check src: Var_200 was used by peripheral
 */
uint8_t Hal_Sys_Var200Src_PeriCheck( void )
{
    uint32_t u32Temp = 0;

    u32Temp = AOS->SPI_CLK_SEL;
    /* SPI 0 */
    if( ( ( u32Temp & AOS_SPI_CLK_SEL_SPI0_CLKSEL_Msk)  >> AOS_SPI_CLK_SEL_SPI0_CLKSEL_Pos ) == APS_CLK_SPI0_SRC_D1_200M )
        return 1;

    return 0;
}

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
uint32_t Hal_Sys_ApsClkMmFactorSet_patch(uint32_t u32Factor)
{
    uint32_t u32HwMmfNext;
    
    if ((u32Factor > APS_MMFACTOR_MAX) || (u32Factor == 0))
    {   /* Out of range */
        return RESULT_FAIL;
    }
    
    u32HwMmfNext = (AOS->CLK_MMFACTOR_APS & AOS_CLK_MMFACTOR_APS_APS_CLK_MMFACTOR_NEXT_Msk) >> AOS_CLK_MMFACTOR_APS_APS_CLK_MMFACTOR_NEXT_Pos;
    if (u32HwMmfNext !=  u32Factor)
    {
        AOS->CLK_MMFACTOR_APS = u32Factor << AOS_CLK_MMFACTOR_APS_FACTOR_Pos;
        AOS->CLK_MMFACTOR_APS = AOS_CLK_MMFACTOR_APS_ENABLE | (u32Factor << AOS_CLK_MMFACTOR_APS_FACTOR_Pos);
    }
    
    g_sApsClkFreq.u32MMFactor = u32Factor;
    
    /* Update system clock divider */
    g_sApsClkFreq.u32SysClk = g_sApsClkFreq.u32SysSrcClk * g_sApsClkFreq.u32MMFactor / APS_MMFACTOR_DIVISOR;
    SystemCoreClockSet(g_sApsClkFreq.u32SysClk);
    
    /* Apply to all relative modules */
    Hal_Sys_ApsClkChangeApply();
    
    return RESULT_SUCCESS;
}


/**
 * @brief Hal_Sys_WakeupClkResume
 *      Resume clock tree.
 *      Because the system clock is switched to shelter before entering sleep.
 *      So switch system clock back to resume clock
 *      Then switch shelter to XTAL back
 */
void Hal_Sys_WakeupClkResume_patch(void)
{
    /* Recover clocks */
    AOS->APS_CLK_SEL = g_Hal_SysApsClkStorage.u32ApsClkSel;
    AOS->APS_DOMAIN_EN = g_Hal_SysApsClkStorage.u32ApsDomainEn;
    AOS->SCRT_CLK_APS = g_Hal_SysApsClkStorage.u32ScrtClkAps;
    AOS->APS_STCLK = g_Hal_SysApsClkStorage.u32ApsStClk;
    AOS->SPI_CLK_SEL = g_Hal_SysApsClkStorage.u32SpiClkSel;
    AOS->APS_DM_CLK_SEL = g_Hal_SysApsClkStorage.u32ApsDmClkSel;
    AOS->CAM_XVCLK_SEL = g_Hal_SysApsClkStorage.u32CamXvclkSel;
    
    
    /* Move system clock to target source - previous setting */
    APS_CLK_SYS_SWITCH_TO_TARGET();
    APS_CLK_SCRT_SWITCH_TO_TARGET();
    
    Hal_Sys_SwitchShelterSet(CLKTREE_SHELTER_SRC_XTAL);
    
    SystemCoreClockSet(g_sApsClkFreq.u32SysClk);
}


/**
 * @brief Hal_Sys_SleepClkSetup
 *      1. Enable RC clock
 *      2. Switch shelter clock to RC
 *      3. Set System clock to shelter
 *      Then system clock becomes to RC
 *      After wakeup, resume system clock to previous setting
 */
void Hal_Sys_SleepClkSetup_patch(void)
{
    Hal_Sys_RcHighEn(RC_HIGH_ON);
    
    Hal_Sys_SwitchShelterSet(CLKTREE_SHELTER_SRC_RC);
    
    /* Move system clock to shelter source - RC */
    APS_CLK_SYS_SWITCH_TO_SHELTER();
    APS_CLK_SCRT_SWITCH_TO_SHELTER();
    
    
    /* Disable unused clocks, recover when Hal_Sys_WakeupClkResume_patch */
    g_Hal_SysApsClkStorage.u32ApsClkSel = AOS->APS_CLK_SEL;
    g_Hal_SysApsClkStorage.u32ApsDomainEn = AOS->APS_DOMAIN_EN;
    g_Hal_SysApsClkStorage.u32ScrtClkAps = AOS->SCRT_CLK_APS;
    g_Hal_SysApsClkStorage.u32ApsStClk = AOS->APS_STCLK;
    g_Hal_SysApsClkStorage.u32SpiClkSel = AOS->SPI_CLK_SEL;
    g_Hal_SysApsClkStorage.u32ApsDmClkSel = AOS->APS_DM_CLK_SEL;
    g_Hal_SysApsClkStorage.u32CamXvclkSel = AOS->CAM_XVCLK_SEL;
    
    /* Disable unused clocks */
    AOS->APS_CLK_SEL = (AOS->APS_CLK_SEL & ~HAL_SLP_CLK_APS_CLK_SEL_MSK) | HAL_SLP_CLK_APS_CLK_SEL_SET_VALUE;
    AOS->APS_DOMAIN_EN = (AOS->APS_DOMAIN_EN & ~HAL_SLP_CLK_APS_DOMAIN_EN_MSK) | HAL_SLP_CLK_APS_DOMAIN_EN_SET_VALUE;
    AOS->SCRT_CLK_APS = (AOS->SCRT_CLK_APS & ~HAL_SLP_CLK_SCRT_CLK_APS_MSK) | HAL_SLP_CLK_SCRT_CLK_APS_SET_VALUE;
    AOS->APS_STCLK = (AOS->APS_STCLK & ~HAL_SLP_CLK_APS_STCLK_MSK) | HAL_SLP_CLK_APS_STCLK_SET_VALUE;
    AOS->SPI_CLK_SEL = (AOS->SPI_CLK_SEL & ~HAL_SLP_CLK_SPI_CLK_SEL_MSK) | HAL_SLP_CLK_SPI_CLK_SEL_SET_VALUE;
    AOS->APS_DM_CLK_SEL = (AOS->APS_DM_CLK_SEL & ~HAL_SLP_CLK_APS_DM_CLK_SEL_MSK) | HAL_SLP_CLK_APS_DM_CLK_SEL_SET_VALUE;
    AOS->CAM_XVCLK_SEL = (AOS->CAM_XVCLK_SEL & ~HAL_SLP_CLK_APS_DM_CLK_SEL_MSK) | HAL_SLP_CLK_CAM_XVCLK_SEL_SET_VALUE;
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
uint32_t Hal_Sys_ApsClockUpdate_patch(E_ApsClkGrp_t eApsClkGrp, uint32_t u32ClkFreq)
{
    uint32_t result;
    result = Hal_Sys_ApsClockUpdate_impl(eApsClkGrp, u32ClkFreq);
    if (result == RESULT_FAIL)
        return result;
    
    
    if (eApsClkGrp == APS_CLK_GRP_SYS_SRC)
    {
        if ( ((AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_APS_SRC_CLK_SEL_Msk) == (APS_CLK_SYS_SRC_150_VAR << AOS_APS_CLK_SEL_APS_SRC_CLK_SEL_Pos))
            && ((AOS->APS_CLK_SEL & AOS_APS_CLK_SEL_APS_CLK_SRC_DYN_SEL_Msk) == 0) )
        {   /* Source = 150_VAR */
            if ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI0_CLKSEL_Msk) == (APS_CLK_SPI0_SRC_150M_VAR << AOS_SPI_CLK_SEL_SPI0_CLKSEL_Pos))
            {   /* SPI0 clock is from 150_VAR */
                g_sApsClkFreq.u32Spi0Clk = u32ClkFreq;
            }
            
            if ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI1_CLK_SEL_Msk) == (APS_CLK_SPI1_SRC_150M_VAR << AOS_SPI_CLK_SEL_SPI1_CLK_SEL_Pos))
            {   /* SPI1 clock is from 150_VAR */
                g_sApsClkFreq.u32Spi1Clk = u32ClkFreq;
            }
            
            if ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI2_CLK_SEL_Msk) == (APS_CLK_SPI2_SRC_150M_VAR << AOS_SPI_CLK_SEL_SPI2_CLK_SEL_Pos))
            {   /* SPI2 clock is from 150_VAR */
                g_sApsClkFreq.u32Spi2Clk = u32ClkFreq;
            }
            
            if ((AOS->SPI_CLK_SEL & AOS_SPI_CLK_SEL_SPI3_CLK_SEL_Msk) == (APS_CLK_SPI3_SRC_150M_VAR << AOS_SPI_CLK_SEL_SPI3_CLK_SEL_Pos))
            {   /* SPI3 clock is from 150_VAR */
                g_sApsClkFreq.u32Spi3Clk = u32ClkFreq;
            }
            
            if ((AOS->CAM_XVCLK_SEL & AOS_CAM_XVCLK_SEL_CAM_XVCLKSEL_Msk) == (APS_CLK_CAM_XVCLK_SRC_150M_VAR << AOS_CAM_XVCLK_SEL_CAM_XVCLKSEL_Pos))
            {   /* CamXv clock is from 150_VAR */
                g_sApsClkFreq.u32CamXvClk = u32ClkFreq;
            }
        }
        
    }
    
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
uint32_t Hal_Sys_Spi0SrcSelect_patch(E_ApsClkSpi0Src_t eSrc, E_ApsClkSpi0Divn_t eDiv)
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
            u32Clk = g_sApsClkFreqExt.u32Var150Clk;
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
uint32_t Hal_Sys_Spi1SrcSelect_patch(E_ApsClkSpi1Src_t eSrc, E_ApsClkSpi1Divn_t eDiv)
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
            u32Clk = g_sApsClkFreqExt.u32Var150Clk;
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
uint32_t Hal_Sys_Spi2SrcSelect_patch(E_ApsClkSpi2Src_t eSrc, E_ApsClkSpi2Divn_t eDiv)
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
            u32Clk = g_sApsClkFreqExt.u32Var150Clk;
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
uint32_t Hal_Sys_Spi3SrcSelect_patch(E_ApsClkSpi3Src_t eSrc, E_ApsClkSpi3Divn_t eDiv)
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
            u32Clk = g_sApsClkFreqExt.u32Var150Clk;
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
uint32_t Hal_Sys_CamXvClkSrcSelect_patch(E_ApsClkCamXvClkSrc_t eSrc, E_ApsClkCamXvCLKDivn_t eDiv)
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
            u32Clk = g_sApsClkFreqExt.u32Var150Clk;;
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
