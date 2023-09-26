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
 *  @file hal_sys_rcc_patch.h 
 * 
 *  @brief reset and clock control patch
 *  
 *******************************************************************************/
#ifndef __HAL_SYS_RCC_H__
#error "Please include hal_sys_rcc.h. Don't directly include hal_sys_rcc_patch.h"
#endif

#ifndef _HAL_SYS_RCC_PATCH_H_
#define _HAL_SYS_RCC_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define RF_CK_GATE_CTRL_SET_BIT_START               (0U)
#define RF_CK_GATE_CTRL_CLEAR_BIT_START             (16U)
#define RF_CK_GATE_CTRL_XTAL_BB_SET                 (1UL << (RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB_Pos + RF_CK_GATE_CTRL_SET_BIT_START))
#define RF_CK_GATE_CTRL_XTAL_BB_CLEAR               (1UL << (RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB_Pos + RF_CK_GATE_CTRL_CLEAR_BIT_START))
#define RF_CK_GATE_CTRL_RC_BB_SET                   (1UL << (RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB_Pos + RF_CK_GATE_CTRL_SET_BIT_START))
#define RF_CK_GATE_CTRL_RC_BB_CLEAR                 (1UL << (RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB_Pos + RF_CK_GATE_CTRL_CLEAR_BIT_START))

#define APS_STCLK_SKEW                              (1 << 24)


#define AOS_APS_DM_CLK_UNSED_MODULES                (AOS_APS_DM_CLK_SEL_I2S_SCLK_EN |           \
                                                     AOS_APS_DM_CLK_SEL_AUX_DSADC_PCLK_EN |     \
                                                     AOS_APS_DM_CLK_SEL_AUX_DSADC_CLK_EN  |     \
                                                     AOS_APS_DM_CLK_SEL_PDM_IO_CLK_EN |         \
                                                     AOS_APS_DM_CLK_SEL_PDM_CLK_DIV_EN |        \
                                                     AOS_APS_DM_CLK_SEL_PDM_CLK_EN      |       \
                                                     AOS_APS_DM_CLK_SEL_PDM_PCLK_EN)


/* APS SysTick clock */
#define APS_CLK_ST_DIVN_GET(value)                  ( 8 >> (((value) & AOS_APS_STCLK_APS_STCLK_SRC_SEL_Msk) >> AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos) )    /* 0: 8, 1:4, 2: 2, 3:1 */

#define VAR_SRC_DIV_MIN          (8)
#define VAR_SRC_DIV_BYPASS_TH    (15)
#define VAR_SRC_DIV_MAX          (31)

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
/** APS Systick divider. Systick source: XTAL */
typedef enum
{
    APS_CLK_ST_DIV_1                    = (3 << AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos),
    APS_CLK_ST_DIV_2                    = (2 << AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos),
    APS_CLK_ST_DIV_4                    = (1 << AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos),
    APS_CLK_ST_DIV_8                    = (0 << AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos),
} E_ApsClkStDivn_t;

typedef enum
{
    VAR_SRC_150M = 0,
    VAR_SRC_180M,
    VAR_SRC_200M
} E_ApsVarSrc_t;

typedef struct {
    uint32_t u32Var150Clk;
}S_APS_CLK_FREQ_EXT;
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Sys_RccPatchInit(void);
uint32_t Hal_Sys_StclkSetup(E_FUNC_ST eEnable, E_ApsClkStDivn_t eDiv);
uint32_t Hal_Sys_VarSrcDiv_Set( E_ApsVarSrc_t eVarsrc, uint8_t u8Div );
uint8_t Hal_Sys_VarSrcDiv_Get( E_ApsVarSrc_t eVarsrc );
uint8_t Hal_Sys_Var150Src_PeriCheck( void );
uint8_t Hal_Sys_Var180Src_PeriCheck( void );
uint8_t Hal_Sys_Var200Src_PeriCheck( void );
void Hal_Sys_Xtal32DetectStart(void);
void Hal_Sys_Xtal32DetectEnd(void);
void Hal_Sys_Xtal32CalcStart(void);
void Hal_Sys_Xtal32CalcEnd(void);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_SYS_RCC_PATCH_H_ */
