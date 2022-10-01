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

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */



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

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_SYS_RCC_PATCH_H_ */
