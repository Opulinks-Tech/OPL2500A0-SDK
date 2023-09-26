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
 *  @file hal_cache_patch.h 
 * 
 *  @brief Cache patch
 *  
 *******************************************************************************/
#ifndef __HAL_CACHE_H__
#error "Please include hal_cache.h. Don't directly include hal_cache_patch.h"
#endif

#ifndef _HAL_CACHE_PATCH_H_
#define _HAL_CACHE_PATCH_H_

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
void Hal_Cache_Clear(void);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_CACHE_PATCH_H_ */
