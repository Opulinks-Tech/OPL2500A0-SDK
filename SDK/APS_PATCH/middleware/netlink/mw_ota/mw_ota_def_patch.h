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
 *  @file mw_ota_def_patch.h 
 * 
 *  @brief OTA definition patch
 *  
 *******************************************************************************/
#ifndef _MW_OTA_DEF_H_
#error "Please include mw_ota_def.h. Don't directly include mw_ota_def_patch.h"
#endif

#ifndef _MW_OTA_DEF_PATCH_H_
#define _MW_OTA_DEF_PATCH_H_

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
// the total size is 476 KB from 0x00000000 to 0x00077000.
    // 0x00000000 ~ 0x00003000 : 12 KB, the 2nd boot agent
    // 0x00003000 ~ 0x00004000 : 4 KB, the 1st OTA header
    // 0x00004000 ~ 0x00005000 : 4 KB, the 2nd OTA header
    // 0x00005000 ~ 0x0003E000 : 228 KB, the 1st OTA image
    // 0x0003E000 ~ 0x00077000 : 228 KB, the 2nd OTA image
#define MW_OTA_HEADER_ADDR_PATCH_1      0x00003000
#define MW_OTA_HEADER_ADDR_PATCH_2      0x00004000
#define MW_OTA_IMAGE_ADDR_PATCH_1       0x00005000
#define MW_OTA_IMAGE_ADDR_PATCH_2       0x0003E000
#define MW_OTA_IMAGE_SIZE_PATCH         0x00039000  // 228 KB


/* 
 * The external flash layout will be
 *  -------------------
 *  | OTA ext image 1 |
 *  |-----------------|
 *  | OTA ext image 2 |
 *  |-----------------|
 *  | Remaining       |
 *  -------------------
 *
 * OTA support image from external flash, and user can put XIP content here.
 */
#define MW_OTA_EXT_IMAGE_ADDR_PATCH_START   0x00000000
#define MW_OTA_EXT_IMAGE_SIZE_PATCH         0x00080000

#define MW_OTA_BOOT_STATUS_DATA_ADDR        0x00400100
#define MW_OTA_BOOT_STATUS_SIGNATURE        0x2698C749


#define MW_OTA_HEADER_EXT_ENABLE            0x8762E10D  /* All other values means disable */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

typedef struct
{
    uint32_t Signature;
    uint32_t CurrOtaImagAddr;
    uint32_t OtaExtImgEnable;
    uint32_t CurrOtaExtImgAddr;
} S_MW_OTA_BOOT_STS;


typedef struct
{
    uint32_t ExtImageEnable;            /* Set to MW_OTA_HEADER_EXT_ENABLE to enable */
    uint32_t ExtImageSize;
    uint32_t ExtImageSum;
    uint32_t ExtFuncHeaderSum;          /* byte sum from ExtImageEnable to end of ExtImageSum, 12 bytes */
    
} T_MwOtaFlashHeader_ExtFlash;

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


#ifdef __cplusplus
}
#endif
#endif  /* _MW_OTA_DEF_PATCH_H_ */
