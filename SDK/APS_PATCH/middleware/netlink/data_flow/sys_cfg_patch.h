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
 *  @file sys_cfg_patch.h 
 * 
 *  @brief System configuration patch
 *  
 *******************************************************************************/
#ifndef __SYS_CFG_H__
#error "Please include sys_cfg.h. Don't directly include sys_cfg_patch.h"
#endif

#ifndef _SYS_CFG_PATCH_H_
#define _SYS_CFG_PATCH_H_

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
// Re-define T_SysCfgClkIdx
#define SYS_CFG_CLK_XTAL            ( SYS_CFG_CLK_22_MHZ )
#define SYS_CFG_CLK_XTAL_X2         ( SYS_CFG_CLK_44_MHZ )
#define SYS_CFG_CLK_VAR_078_MHZ     ( SYS_CFG_CLK_78_MHZ )
#define SYS_CFG_CLK_VAR_081_MHZ     ( SYS_CFG_CLK_81_MHZ )
#define SYS_CFG_CLK_VAR_084_MHZ     ( SYS_CFG_CLK_84_MHZ )
#define SYS_CFG_CLK_VAR_087_MHZ     ( SYS_CFG_CLK_87_MHZ )
#define SYS_CFG_CLK_VAR_090_MHZ     ( SYS_CFG_CLK_90_MHZ )
#define SYS_CFG_CLK_VAR_093_MHZ     ( SYS_CFG_CLK_93_MHZ )
#define SYS_CFG_CLK_VAR_097_MHZ     ( SYS_CFG_CLK_97_MHZ )
#define SYS_CFG_CLK_VAR_101_MHZ     ( SYS_CFG_CLK_101_MHZ )
#define SYS_CFG_CLK_VAR_106_MHZ     ( SYS_CFG_CLK_106_MHZ )
#define SYS_CFG_CLK_VAR_110_MHZ     ( SYS_CFG_CLK_110_MHZ )
#define SYS_CFG_CLK_VAR_116_MHZ     ( SYS_CFG_CLK_116_MHZ )
#define SYS_CFG_CLK_VAR_122_MHZ     ( SYS_CFG_CLK_122_MHZ )
#define SYS_CFG_CLK_VAR_128_MHZ     ( SYS_CFG_CLK_128_MHZ )
#define SYS_CFG_CLK_VAR_135_MHZ     ( SYS_CFG_CLK_135_MHZ )
#define SYS_CFG_CLK_VAR_143_MHZ     ( SYS_CFG_CLK_143_MHZ )
#define SYS_CFG_CLK_DECI_160_MHZ    ( SYS_CFG_CLK_176_MHZ )
#define SYS_CFG_CLK_DECI_080_MHZ    ( SYS_CFG_CLK_176_MHZ + 1 )

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef struct
{
    uint8_t u8TcaEn;
    uint8_t u8TcaThr;
} T_TcaCfg;


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
int sys_cfg_tca_set(void *pCfg);

void sys_cfg_patch_init(void);

#ifdef __cplusplus
}
#endif
#endif  /* _SYS_CFG_PATCH_H_ */
