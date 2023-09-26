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
#include "cmsis_os.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define NO_FLASH           0
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
uint32_t Hal_Flash_Init_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t Hal_Flash_PageAddrProgram_Internal_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
uint32_t Hal_Flash_PageAddrRead_Internal_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrProgram_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrRead_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_4KSectorAddrErase_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr);
uint32_t Hal_Flash_QuadModeEn_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8QModeEn);
uint32_t _Hal_Flash_RxSampleDelaySetup_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
uint32_t g_u32aHal_Flash_BaseAddress[SPI_SLAVE_MAX] = {0xFF000000, 0xFF000000, 0xFF000000};
/* g_u32aHal_Flash_BaseAddress: Initialized value affect equal to 0, but avoid using zero init
 * Because zero init function runs later then the variables be used */

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */


extern osSemaphoreId g_taHalFlashSemaphoreId[SPI_IDX_MAX];
extern uint8_t g_u8aHalFlashID[SPI_IDX_MAX][SPI_SLAVE_MAX];

 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Flash_PatchInit(void)
{
    Hal_Flash_Init = Hal_Flash_Init_patch;
    
    Hal_Flash_PageAddrProgram_Internal = Hal_Flash_PageAddrProgram_Internal_patch;
    Hal_Flash_PageAddrRead_Internal = Hal_Flash_PageAddrRead_Internal_patch;
    Hal_Flash_AddrProgram_Internal_Ext = Hal_Flash_AddrProgram_Internal_Ext_patch;
    Hal_Flash_AddrRead_Internal_Ext = Hal_Flash_AddrRead_Internal_Ext_patch;
    Hal_Flash_4KSectorAddrErase_Internal_Ext = Hal_Flash_4KSectorAddrErase_Internal_Ext_patch;
    Hal_Flash_QuadModeEn = Hal_Flash_QuadModeEn_patch;
    _Hal_Flash_RxSampleDelaySetup = _Hal_Flash_RxSampleDelaySetup_patch;
}

void Hal_Flash_QspiBaseAddrSet(E_SpiSlave_t eSlvIdx, uint32_t BaseAddr)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return;
    
    g_u32aHal_Flash_BaseAddress[eSlvIdx] = BaseAddr;
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

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

        u32Addr = FLASH_TO_XIP_ADDR(eSlvIdx, u32PageAddr & 0xFFFFFF00); // aligned page
        memcpy((void *)u32Addr, pu8Data, u32PageSize);
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        Hal_Qspi_RestoreXipCs();
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
        uint32_t u32Addr = FLASH_TO_XIP_ADDR(eSlvIdx, u32PageAddr);
        memcpy(pu8Data, (void *)u32Addr, u32PageSize);
        Hal_Qspi_RestoreXipCs();
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
        
        uint32_t u32Addr = FLASH_TO_XIP_ADDR(eSlvIdx, u32StartAddr);
        memcpy((void *)u32Addr, pu8Data, u32Size);
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        Hal_Qspi_RestoreXipCs();
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
        
        uint32_t u32Addr = FLASH_TO_XIP_ADDR(eSlvIdx, u32StartAddr);
        memcpy(pu8Data, (void *)u32Addr, u32Size);
        Hal_Qspi_RestoreXipCs();
        return 0;
    }
    else
    {
        return Hal_Flash_AddrRead_Internal_Ext_impl(eSpiIdx, eSlvIdx, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
    }
}

/* Wait flash erase done to avoid XIP running fail */
uint32_t Hal_Flash_4KSectorAddrErase_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr)
{
    uint32_t ret;
    ret = Hal_Flash_4KSectorAddrErase_Internal_Ext_impl(eSpiIdx, eSlvIdx, u32SecAddr);
    if (ret == 0)
    {
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
    }
    if (eSpiIdx == SPI_IDX_0)
        Hal_Qspi_RestoreXipCs();
    return ret;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_QuadModeEn
*
* DESCRIPTION:
*   1. Enable flash into Quad mode
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx   : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx   : Index of SPI slave. refer to E_SpiSlave_t
*   3. u8QModeEn : Quad-mode select. 1 for enable/0 for disable
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_QuadModeEn_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8QModeEn)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    uint8_t u8Sts_1 = 0;
    uint8_t u8Sts_2 = 0;
    uint8_t u8IsQModeEn = 0;
    
    
    _Hal_Flash_Status1Get(eSpiIdx, eSlvIdx, (uint32_t *)&u8Sts_1);
    _Hal_Flash_Status2Get(eSpiIdx, eSlvIdx, (uint32_t *)&u8Sts_2);
    // read Status Register first
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID)
    {
        u8IsQModeEn = (u8Sts_1 & FLASH_STATUS_QE_MXIC) ? 1 : 0;
    }
    else
    {
        u8IsQModeEn = (u8Sts_2 & FLASH_STATUS_QE) ? 1 : 0;
    }
    
    // write Status Register if current QE bit is not set
    if (u8QModeEn && !u8IsQModeEn)
    {
        if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID)
        {
            u8Sts_1 |= FLASH_STATUS_QE_MXIC;
        }
        else
        {
            u8Sts_2 |= FLASH_STATUS_QE;
        }
        
        _Hal_Flash_StatusSet(eSpiIdx, eSlvIdx, u8Sts_1, u8Sts_2);
        return _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx); // poll busy
    }
    else if (!u8QModeEn && u8IsQModeEn)
    {
        if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID)
        {
            u8Sts_1 &= ~FLASH_STATUS_QE_MXIC;
        }
        else
        {
            u8Sts_2 &= ~FLASH_STATUS_QE;
        }
        _Hal_Flash_StatusSet(eSpiIdx, eSlvIdx, u8Sts_1, u8Sts_2);
        return _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx); // poll busy
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_RxSampleDelaySetup
*
* DESCRIPTION:
*   1. setup Rx Sample Dealy by utiling WEL=1 (bit 1) after WriteEnable (06h) command
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_RxSampleDelaySetup_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    uint32_t ret;
    
    ret = _Hal_Flash_RxSampleDelaySetup_impl(eSpiIdx, eSlvIdx);
    
    if (ret == 1)
        return 1;
    
    /* Recover WEL */
    _Hal_Flash_CmdSend(eSpiIdx, eSlvIdx, FLASH_CMD_WRITE_DISABLE);
    return 0;
}
