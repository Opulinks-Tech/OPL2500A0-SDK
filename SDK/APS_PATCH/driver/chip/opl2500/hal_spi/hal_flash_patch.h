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
 *  @file hal_flash_patch.h 
 * 
 *  @brief Flash API patch
 *  
 *******************************************************************************/
#ifndef __HAL_FLASH_H__
#error "Please include hal_flash.h. Don't directly include hal_flash_patch.h"
#endif

#ifndef _HAL_FLASH_PATCH_H_
#define _HAL_FLASH_PATCH_H_

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

#define FLASH_CMD_WRITE_DISABLE              0x04
    
#define FLASH_TO_XIP_ADDR(slv_idx, flash_addr)              Hal_Flash_ConvertXipAhbAddress(slv_idx, flash_addr)
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */


typedef void (*T_Hal_Flash_InternalFlashWpSetup_fp)(void);

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern T_Hal_Flash_InternalFlashWpSetup_fp Hal_Flash_InternalFlashWpSetup;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void Hal_Flash_PatchInit(void);
uint32_t Hal_Flash_ReadFlashId(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t Hal_WriteProtectControlSet(E_FUNC_ST eFunc);
E_FUNC_ST Hal_WriteProtectControlGet(void);
#ifdef __cplusplus
}
#endif
#endif  /* _HAL_FLASH_PATCH_H_ */
