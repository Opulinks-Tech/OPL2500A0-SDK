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
/* Spare register usage
 * [AOS]
 * SPARE0@0x10C: IPC sync usage
 * SPARE1@0x124: HW use
 * SPARE2@0x12C: For further use
 * SPARE3@0x130: For further use
 * [SYS]
 * SPARE @0x264: XTAL frequency
 * SPARE1@0x268: APS boot debug tag
 * SPARE2@0x26C: MSQ boot debug tag
 * SPARE3@0x270: [Patch] System status
 * SPARE4@0x274: [Patch] MSQ hard fault MSP
 * SPARE5@0x278: [Patch] OTA status
 * SPARE6@0x27C: [Patch] Beacon debug
 * SPARE7@0x280:
 * SPARE8@0x284:
 * [RAM]
 * 0x00400100 ~ 0x0040010F: OTA status
 * 0x00400110: Wi-Fi S2ST length
 *
 */

#define Hal_Sys_RetRamTurnOff   Hal_Sys_RetRamTurnOff_patch


#define SYSTEM_SPARE_REG_SYSTEM_STATUS                  (SYS->SPARE3)
#define HAL_SYS_SET_SYSTEM_STATUS(bitpos, bitvalue)     (BIT_SET(SYSTEM_SPARE_REG_SYSTEM_STATUS, (bitpos), (bitvalue)?1:0))

#define SYSTEM_SPARE_REG_MSQ_MSP                        (SYS->SPARE4)
#define SYSTEM_SPARE_REG_OTA_STATUS                     (SYS->SPARE5)
#define SYSTEM_SPARE_REG_BCN_DBG1                       (SYS->SPARE6)
#define SYSTEM_SPARE_REG_BCN_DBG2                       (SYS->SPARE7)

#define SYSTEM_STATUS_XTAL_ALIGNED_BIT                  (0UL)
#define SYSTEM_STATUS_XTAL_ALIGNED_TO_MHZ_DONE          1
#define SYSTEM_STATUS_XTAL_ALIGNED_KEPT                 0

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */


typedef enum
{
    WAIT_STATE_ZERO = 0,
    WAIT_STATE_ONE,
} APS_WAIT_STATE;

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

void Hal_Sys_WaitStateSet(APS_WAIT_STATE u32WaitState);
uint32_t Hal_Sys_WaitStateGet(void);
uint32_t Hal_Sys_WaitStateCalc(uint32_t u32SysClock);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_SYSTEM_PATCH_H_ */
