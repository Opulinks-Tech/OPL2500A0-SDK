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
 *  @file hal_qspi_patch.h 
 * 
 *  @brief Flash API patch
 *  
 *******************************************************************************/
#ifndef __HAL_QSPI_H__
#error "Please include hal_qspi.h. Don't directly include hal_qspi_patch.h"
#endif

#ifndef _HAL_QSPI_PATCH_H_
#define _HAL_QSPI_PATCH_H_

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
void Hal_Qspi_PatchInit(void);
void Hal_QSpi_UpdateRemap(uint32_t u32Addr);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_QSPI_PATCH_H_ */
