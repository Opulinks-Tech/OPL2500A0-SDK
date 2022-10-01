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

void Hal_Sys_UnusedModuleDis_patch(void);
uint32_t Hal_Sys_Ext2IoSetup_patch(E_ApsClkExt2IoSrc_t eSrc, E_ApsClkExt2IoDivn_t eDiv, E_ApsClkExt2IoEn_t eEn);
uint32_t Hal_Sys_RcHighEn_patch(E_ClkSrcRcHigh eEn);
uint32_t Hal_Sys_XtalHighEn_patch(E_ClkSrcXtalHigh eEn);
uint32_t Hal_Sys_ApsClockUpdate_patch(E_ApsClkGrp_t eApsClkGrp, uint32_t u32ClkFreq);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

extern S_APS_CLK_FREQ g_sApsClkFreq;

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

void Hal_Sys_RccPatchInit(void)
{
    g_sApsClkFreq.u32StClk  = g_sApsClkFreq.u32SysClk;
    AOS->APS_STCLK |= APS_STCLK_SKEW;
    
    
    Hal_Sys_UnusedModuleDis = Hal_Sys_UnusedModuleDis_patch;
    Hal_Sys_Ext2IoSetup = Hal_Sys_Ext2IoSetup_patch;
    Hal_Sys_RcHighEn = Hal_Sys_RcHighEn_patch;
    Hal_Sys_XtalHighEn = Hal_Sys_XtalHighEn_patch;
    Hal_Sys_ApsClockUpdate = Hal_Sys_ApsClockUpdate_patch;
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

    AOS->SCRT_CLK_APS &= ~AOS_SCRT_CLK_APS_SEL_ALL_APS;    
    AOS->APS_DOMAIN_EN &= ~AOS_APS_DOMAIN_EN_ALL_APS;
    AOS->SPI_CLK_SEL &= ~AOS_SPI_123_CLK_SEL_APS;
    AOS->APS_DM_CLK_SEL &= ~AOS_APS_DM_CLK_SEL_ALL_APS;

    AOS->CAM_XVCLK_SEL &= ~AOS_CAM_XVCLK_CLK_APS;
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

uint32_t Hal_Sys_ApsClockUpdate_patch(E_ApsClkGrp_t eApsClkGrp, uint32_t u32ClkFreq)
{
    uint32_t u32Result = Hal_Sys_ApsClockUpdate_impl(eApsClkGrp, u32ClkFreq);
    
    if (eApsClkGrp == APS_CLK_GRP_SYS_SRC)
    {
        g_sApsClkFreq.u32StClk = g_sApsClkFreq.u32SysClk;
    }
    
    return u32Result;
}
