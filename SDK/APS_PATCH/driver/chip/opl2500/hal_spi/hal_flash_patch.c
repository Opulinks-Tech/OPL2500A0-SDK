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
#include <string.h>
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
#define PATCH_KEY               0x48435450
#define PATCH_TYPE_XIP_COLD     0x14110400
#define PATCH_TYPE_XIP_WARM     0x24110400
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
uint32_t Hal_Flash_PageAddrProgram_Internal_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
uint32_t Hal_Flash_PageAddrRead_Internal_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrProgram_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrRead_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
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
    
    Hal_Flash_PageAddrProgram_Internal = Hal_Flash_PageAddrProgram_Internal_patch;
    Hal_Flash_PageAddrRead_Internal = Hal_Flash_PageAddrRead_Internal_patch;
    Hal_Flash_AddrProgram_Internal_Ext = Hal_Flash_AddrProgram_Internal_Ext_patch;
    Hal_Flash_AddrRead_Internal_Ext = Hal_Flash_AddrRead_Internal_Ext_patch;
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
    
    
    if (g_eHal_FlashQspiXipInitResult == RESULT_SUCCESS)
    {   /* Init done, reparsing again */
        g_u32FlashBaseAddr = APS_XIP_MEM_BASE;
        g_eHal_FlashQspiXipInitResult = RESULT_FAIL;
    }
    
    while (1)
    {
        if (Hal_Flash_AddrRead_Internal_Ext(SPI_IDX_0, eSlvIdx, u32Addr, 0, sizeof(sPatchHeader), (uint8_t *)&sPatchHeader) == 1)
            break;
        u32Addr += sizeof(sPatchHeader);
        
        if (sPatchHeader.u32Key != PATCH_KEY) /* Not exist */
            break;
        
        if (sPatchHeader.u32Type == PATCH_TYPE_XIP_COLD)
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

uint32_t Hal_Flash_PageAddrProgram_Internal_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
        if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
        
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return 1;

    if (u32PageAddr & 0xFF) // Page Addr is not aligned 
        return 1;
    
    if (pu8Data == NULL)
        return 1;
    
    uint32_t u32PageSize = FLASH_PAGE_SIZE;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        uint32_t u32Addr = 0;
        
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);
        
        // polling flash status
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);

        u32Addr = FLASH_TO_XIP_ADDR(u32PageAddr & 0xFFFFFF00); // aligned page
        memcpy((void *)u32Addr, pu8Data, u32PageSize);
        return 0;
    }
    else
    {
        return Hal_Flash_PageAddrProgram_Internal_impl(eSpiIdx, eSlvIdx, u32PageAddr, u8UseQuadMode, pu8Data);
    }
}

uint32_t Hal_Flash_PageAddrRead_Internal_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return 1;

    if (u32PageAddr & 0xFF) // Page Addr is not aligned 
        return 1;
    
    if (pu8Data == NULL)
        return 1;
    
    uint32_t u32PageSize = FLASH_PAGE_SIZE;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);

        // polling flash status
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        
        // ignore u8UseQuadMode, alwasy Quad mode
        uint32_t u32Addr = FLASH_TO_XIP_ADDR(u32PageAddr);
        memcpy(pu8Data, (void *)u32Addr, u32PageSize);
        return 0;
    }
    else
    {
        return Hal_Flash_PageAddrRead_Internal_impl(eSpiIdx, eSlvIdx, u32PageAddr, u8UseQuadMode, pu8Data);
    }
}

uint32_t Hal_Flash_AddrProgram_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return 1;
    
    if (u32Size == 0)
        return 1;
    
    if (pu8Data == NULL)
        return 1;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);

        // polling flash status
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        
        uint32_t u32Addr = FLASH_TO_XIP_ADDR(u32StartAddr);
        memcpy((void *)u32Addr, pu8Data, u32Size);
        return 0;
    }
    else
    {
        return Hal_Flash_AddrProgram_Internal_Ext_impl(eSpiIdx, eSlvIdx, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
    }
}

uint32_t Hal_Flash_AddrRead_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return 1;
    
    if (u32Size == 0)
        return 1;
    
    if (pu8Data == NULL)
        return 1;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);
        
        // polling flash status
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        
        uint32_t u32Addr = FLASH_TO_XIP_ADDR(u32StartAddr);
        memcpy(pu8Data, (void *)u32Addr, u32Size);
        return 0;
    }
    else
    {
        return Hal_Flash_AddrRead_Internal_Ext_impl(eSpiIdx, eSlvIdx, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
    }
}


