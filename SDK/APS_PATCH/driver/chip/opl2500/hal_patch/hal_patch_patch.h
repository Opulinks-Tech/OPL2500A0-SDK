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
#ifndef _HAL_PATCH_H_
#error "Please include hal_patch.h. Don't directly include hal_patch_patch.h"
#endif

#ifndef _HAL_PATCH_PATCH_H_
#define _HAL_PATCH_PATCH_H_

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
/*
 * Hal_Patch_ScanXipPartition
 * @brief To scan XIP content start address, patch header excluded.
 *        Check the XIP content checksum valid or not.
 * @param eSlvIdx[in]  The chip select index of target flash
 * @param u32FlashAddr[in] The target flash address to be scanned
 * @param pXipStartAddr[out] The XIP partition start address, the XIP content address. The patch header not included.
 *
 * @return If the XIP partition exists or not.
 * @retval >0: XIP content size
 * @retval 0: XIP not found or checksum mismatch
 */
uint32_t Hal_Patch_ScanXipPartition(uint32_t eSlvIdx, uint32_t u32FlashAddr, uint32_t *pu32XipStartAddr);

/*
 * Hal_Patch_CopyXipContent
 * Copy XIP content to target memory
 * @param u32Flash[in] The source address in internal flash
 * @param u32DstAddr[in] The target address to be copied to
 * @param u32Size[in] How many bytes to be copied
 */
void Hal_Patch_CopyXipContent(uint32_t u32FlashAddr, uint32_t u32DstAddr, uint32_t u32Size);
#ifdef __cplusplus
}
#endif
#endif  /* _HAL_PATCH_PATCH_H_ */
