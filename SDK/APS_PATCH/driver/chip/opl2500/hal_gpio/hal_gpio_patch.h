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
 *  @file opl2500_tmpl_patch.h 
 * 
 *  @brief Template patch
 *  
 *******************************************************************************/
#ifndef __HAL_GPIO_H__
#error "Please include hal_gpio.h. Don't directly include hal_gpio_patch.h"
#endif

#ifndef _HAL_GPIO_PATCH_H_
#define _HAL_GPIO_PATCH_H_

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
void Hal_Gpio_PatchInit(void);
uint32_t Hal_Gpio_Direction_patch(E_GpioIdx_t eIdx, E_GpioDirection_t eDirection);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_GPIO_PATCH_H_ */
