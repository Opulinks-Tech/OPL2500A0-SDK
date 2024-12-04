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
 *  @file boot_sequence_patch.h 
 * 
 *  @brief Boot sequence patch
 *  
 *******************************************************************************/
#ifndef _BOOT_SEQUENCE_H_
#error "Please include boot_sequence.h. Don't directly include boot_sequence_patch.h"
#endif

#ifndef _BOOT_SEQUENCE_PATCH_H_
#define _BOOT_SEQUENCE_PATCH_H_

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
void Boot_HwPatchInit(void);
void Boot_PrepareM0PatchOK_patch(void);
void Boot_RtcSelect32kRc(void);

/**
 * @brief To write XTAL frequency to flash
 * @param u32XtalFreq[in] XTAL frequency. i.e. 20MHz = 20000000 (0x1312d00)
 */
void Boot_FixXtalFreqCfg(uint32_t u32XtalFreq);

#ifdef __cplusplus
}
#endif
#endif  /* _BOOT_SEQUENCE_PATCH_H_ */
