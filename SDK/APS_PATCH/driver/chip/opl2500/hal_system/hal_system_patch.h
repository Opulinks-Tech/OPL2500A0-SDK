/* *****************************************************************************
 *  Copyright 2017 - 2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file hal_system_patch.h 
 * 
 *  
 *******************************************************************************/
#ifndef __HAL_SYSTEM_H__
#error "Please include hal_system.h. Don't directly include hal_system_patch.h"
#endif

#ifndef _HAL_SYSTEM_PATCH_H_
#define _HAL_SYSTEM_PATCH_H_


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
#define Hal_Sys_RetRamTurnOff   Hal_Sys_RetRamTurnOff_patch
    
    
#define SYSTEM_SPARE_REG_SYSTEM_STATUS                  (SYS->SPARE3)
#define HAL_SYS_SET_SYSTEM_STATUS(bitpos, bitvalue)     (BIT_SET(SYSTEM_SPARE_REG_SYSTEM_STATUS, (bitpos), (bitvalue)?1:0))
    
#define SYSTEM_SPARE_REG_MSQ_MSP                        (SYS->SPARE4)
    
#define SYSTEM_STATUS_XTAL_ALIGNED_BIT                  (0UL)
#define SYSTEM_STATUS_XTAL_ALIGNED_TO_MHZ_DONE          1
#define SYSTEM_STATUS_XTAL_ALIGNED_KEPT                 0

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
void Hal_Sys_PatchInit(void);
void Hal_Sys_PatchVerInit(uint32_t u32VerInfoAddr);


#ifdef __cplusplus
}
#endif
#endif  /* _HAL_SYSTEM_PATCH_H_ */
