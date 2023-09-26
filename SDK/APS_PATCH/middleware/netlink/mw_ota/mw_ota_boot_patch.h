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
 *  @file mw_ota_boot_patch.h 
 * 
 *  @brief Template patch
 *  
 *******************************************************************************/
#ifndef _MW_OTA_BOOT_H_
#error "Please include mw_ota_boot.h. Don't directly include mw_ota_boot_patch.h"
#endif

#ifndef _MW_OTA_BOOT_PATCH_H_
#define _MW_OTA_BOOT_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

#define MW_OTA_EXT_FLASH_ENABLE        0x5A5A5A5A
#define MW_OTA_EXT_FLASH_DISABLE       0xFFFFFFFF      /* Initial value not zero, to avoid zero init function */


typedef struct
{
    uint32_t Enable;                /* OTA image on external flash is enabled or disabled. */
    uint32_t ExtFlashImageStart;    /* OTA image 1 start address on external flash, suggested to be zero */
    uint32_t ExtFlashImageSize;     /* OTA image size. The size of external flash must be larger then twice of image size */
} MW_OTA_EXT_FLASH_CONFIG;

typedef struct
{
    uint32_t ulaImageAddr[2];   // the start address of image
    uint32_t ulImageSize;       // the max size of image
} T_MwOtaLayoutInfoExtFlash;


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
void MwOta_PatchInit(void);
void MwOta_ExtFlashInit(uint32_t SpiSlaveIdx, uint32_t ExtFlashStart, uint32_t ExtFlashSize);

#ifdef __cplusplus
}
#endif
#endif  /* _MW_OTA_BOOT_PATCH_H_ */
