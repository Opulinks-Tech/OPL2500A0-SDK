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
 *  @file xxx_patch.h 
 * 
 *  @brief xxx patch
 *  
 *******************************************************************************/
#ifndef __HAL_PSRAM_H__
#error "Please include hal_psram.h. Don't directly include hal_psram_patch.h"
#endif

#ifndef _HAL_PSRAM_PATCH_H_
#define _HAL_PSRAM_PATCH_H_

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
uint32_t Hal_Psram_IsValid(E_SpiSlave_t eSlvIdx);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_PSRAM_PATCH_H_ */
