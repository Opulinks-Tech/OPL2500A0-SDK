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
 *  @file hal_uart_patch.h 
 * 
 *  @brief UART patch
 *  
 *******************************************************************************/
#ifndef __HAL_UART_H__
#error "Please include hal_uart.h. Don't directly include hal_uart_patch.h"
#endif

#ifndef _HAL_UART_PATCH_H_
#define _HAL_UART_PATCH_H_

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
#define UART_DIVIDER_INT_MAX_VALUE      0xFFFF

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
void Hal_Uart_PatchInit(void);
void Hal_Uart_SleepPrepare(void);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_UART_PATCH_H_ */
