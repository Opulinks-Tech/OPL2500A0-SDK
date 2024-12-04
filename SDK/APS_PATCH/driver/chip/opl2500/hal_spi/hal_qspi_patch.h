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
 *  @file hal_qspi_patch.h 
 * 
 *  @brief Flash API patch
 *  
 *******************************************************************************/
#ifndef __HAL_QSPI_H__
#error "Please include hal_qspi.h. Don't directly include hal_qspi_patch.h"
#endif

#ifndef _HAL_QSPI_PATCH_H_
#define _HAL_QSPI_PATCH_H_

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
#define XIP_PROTECTION_LOCK()
    
#define XIP_PROTECTION_UNLOCK()

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef enum
{
    QSPI_DEV_SIZE_8MBIT = 0,
    QSPI_DEV_SIZE_64MBIT,
} E_QSPI_DEVICE_SIZE;


/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern uint32_t g_u32Hal_QspiRemapAddr;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void Hal_Qspi_PatchInit(void);
void Hal_QSpi_UpdateRemap(uint32_t u32Addr);
uint32_t Hal_Qspi_UpdateDeviceSize(E_SpiSlave_t eSlvIdx, E_QSPI_DEVICE_SIZE eSize);
uint32_t Hal_Qspi_WaitPollingState(void);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_QSPI_PATCH_H_ */
