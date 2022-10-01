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
 *  @file hal_pin_patch.h 
 * 
 *  @brief PIN function patch
 *  
 *******************************************************************************/
#ifndef __HAL_PIN_H__
#error "Please include hal_pin.h. Don't directly include hal_pin_patch.h"
#endif

#ifndef _HAL_PIN_PATCH_H_
#define _HAL_PIN_PATCH_H_

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

void Hal_SysDisableAllTxPeriphPinmux(void);
#ifdef __cplusplus
}
#endif
#endif  /* _HAL_PIN_PATCH_H_ */
