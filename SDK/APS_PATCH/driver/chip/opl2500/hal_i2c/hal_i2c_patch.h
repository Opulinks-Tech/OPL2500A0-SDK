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
 *  @file hal_i2c_patch.h 
 * 
 *  @brief I2C module patch
 *  
 *******************************************************************************/
#ifndef __HAL_I2C_H__
#error "Please include hal_i2c.h. Don't directly include hal_i2c_patch.h"
#endif

#ifndef _HAL_I2C_PATCH_H_
#define _HAL_I2C_PATCH_H_

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
void Hal_I2c_PatchInit(void);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_I2C_PATCH_H_ */
