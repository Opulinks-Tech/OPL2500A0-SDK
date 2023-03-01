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
#ifndef __HAL_DMA_H__
#error "Please include hal_dma.h. Don't directly include hal_dma_patch.h"
#endif

#ifndef _HAL_DMA_PATCH_H_
#define _HAL_DMA_PATCH_H_

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
uint32_t Hal_Dma_IntEn(uint8_t u8Enable, E_DMA_CHANNEL eChannel);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_DMA_PATCH_H_ */
