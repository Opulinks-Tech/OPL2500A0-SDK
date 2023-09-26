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
 *  @file hal_cache_patch.c
 * 
 *  @brief Cache patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_cache.h"

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
*                           Declarations of Private Functions
*************************************************************************
*/

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 




 
 
 
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
/*
 * To reset and restore cache control
 * When SPI chip select change, the cache needs to be cleared.
 */
void Hal_Cache_Clear(void)
{
    uint32_t CacheCtrl;
    
    if (!(SYS->XIP_CACHE_CONTROL & SYS_XIP_CACHE_CONTROL_XIP_PREF_CACHE_EN))
        return;
    
    CacheCtrl = SYS->XIP_CACHE_CONTROL;
    SYS->XIP_CACHE_CONTROL = 0;
    __ISB();
    __DSB();
    SYS->XIP_CACHE_CONTROL = CacheCtrl;
    Hal_Cache_ReadyPoll();
    __ISB();
    __DSB();
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

