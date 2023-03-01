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
#define GPIO_IDX_OUT_MAX    (GPIO_IDX_38)

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
extern uint8_t g_u8Gpio_TxEn;
extern uint8_t g_u8Gpio_RxEn;
extern uint8_t g_u8Gpio_LnaEn;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_SysDisableAllTxPeriphPinmux(void);
void Hal_ExtPa_Pin_Set(uint8_t u8Gpio_TxEn, uint8_t u8Gpio_RxEn, uint8_t u8Gpio_LnaEn);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_PIN_PATCH_H_ */
