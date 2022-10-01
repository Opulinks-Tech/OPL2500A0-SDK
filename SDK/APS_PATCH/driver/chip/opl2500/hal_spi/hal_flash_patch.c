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
 *  @file hal_flash_patch.c
 * 
 *  @brief FLASH API patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
#include "hal_flash.h"
#include "hal_flash_internal.h"
#include "hal_qspi.h"
#include "mw_ota.h"
#include "cmsis_os.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define PATCH_KEY          0x48435450
#define PATCH_TYPE_XIP     0x14110400
#define NO_FLASH           0
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

typedef struct
{
    uint32_t u32Key;
    uint32_t u32Type;
    uint32_t u32Size;
    uint32_t u32Sum;
} S_Hal_Patch_Header_t;
/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
uint32_t Hal_Flash_Init_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

E_RESULT_COMMON g_eHal_FlashQspiXipInitResult = RESULT_FAIL;
/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */

extern uint32_t g_u32FlashBaseAddr;
extern osSemaphoreId g_taHalFlashSemaphoreId[SPI_IDX_MAX];
extern uint8_t g_u8aHalFlashID[SPI_IDX_MAX][SPI_SLAVE_MAX];
 
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_FlashPatchInit(void)
{
    Hal_Flash_Init = Hal_Flash_Init_patch;
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

/**
 * Hal_FlashQspiXipInit
 * @brief According to patch frames setting, find XIP content offset in flash.
 *        Then using the offset to set QSPI remap setting
 * @param eSlvIdx [in] Slave index of SPI0 (only SPI0 support XIP)
 * @param u32StartAddr [in] Searching flash start address
 * @return Init XIP success or not
 * @retval RESULT_SUCCESS or RESULT_FAIL
 */
E_RESULT_COMMON Hal_FlashQspiXipInit(E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr)
{
    uint32_t u32Addr = u32StartAddr;
    E_RESULT_COMMON eResult = RESULT_FAIL;
    S_Hal_Patch_Header_t sPatchHeader;
    
    
    if (g_u8aHalFlashID[SPI_IDX_0][eSlvIdx] == NO_FLASH)
        return eResult;
    
    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[SPI_IDX_0], osWaitForever);
    
    while (1)
    {
        if (Hal_Flash_AddrRead_Internal_Ext(SPI_IDX_0, eSlvIdx, u32Addr, 0, sizeof(sPatchHeader), (uint8_t *)&sPatchHeader) == 1)
            break;
        u32Addr += sizeof(sPatchHeader);
        
        if (sPatchHeader.u32Key != PATCH_KEY) /* Not exist */
            break;
        
        if (sPatchHeader.u32Type == PATCH_TYPE_XIP)
        {   /* XIP content */
            Hal_QSpi_UpdateRemap(u32Addr);
            g_u32FlashBaseAddr = APS_XIP_MEM_BASE - u32Addr;
            g_eHal_FlashQspiXipInitResult = RESULT_SUCCESS;
            eResult = RESULT_SUCCESS;
            break;
        }
        
        u32Addr += __REV(sPatchHeader.u32Size) + sizeof(uint32_t);
    }
    
    osSemaphoreRelease(g_taHalFlashSemaphoreId[SPI_IDX_0]);
    return eResult;
}

uint32_t Hal_Flash_Init_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    g_u8aHalFlashID[eSpiIdx][eSlvIdx] = NO_FLASH;
    return Hal_Flash_Init_impl(eSpiIdx, eSlvIdx);
}

