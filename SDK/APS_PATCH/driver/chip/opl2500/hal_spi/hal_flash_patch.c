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
#include "hal_tick.h"
#include "cmsis_os.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define NO_FLASH           0

#define EN_FORCE_RW_MAX     1
#define RW_ONCE_MAX         256

#define FLASH_CRITICAL_ENTER()      osEnterCritical()
#define FLASH_CRITICAL_EXIT()       osExitCritical()

/*
 * Support at most one external flash
 *              | AHB mapping size
 *              | CS0 |  CS1 |  CS2 |  CS3
 * -------------+-----+------+------+------
 * Ext   | None | 8Mb |  8Mb |  8Mb |  8Mb 
 * flash | CS1  | 8Mb | 64Mb |  8Mb |  8Mb 
 *       | CS2  | 8Mb |  8Mb | 64Mb |  8Mb 
 *       | CS3  | 8Mb |  8Mb |  8Mb | 64Mb 
 */
#define FLASH_INTERNAL_MAPPING_START        0x03000000
#define FLASH_INTERNAL_MAPPING_MASK         0x000FFFFF      /* 8Mb */
#define FLASH_EXTERNAL_MAPPING_MASK         0x007FFFFF      /* 64Mb */
#define FLASH_EXTERNAL_CS1_MAPPING_START    0x03100000
#define FLASH_EXTERNAL_CS2_MAPPING_START    0x03200000
#define FLASH_EXTERNAL_CS3_MAPPING_START    0x03300000

#define FLASH_BP_BIT_MASK      0x7C


/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

                                    
typedef struct
{
    uint32_t Start;     /* The AHB memory mapping start */
    uint32_t Mask;      
}S_HAL_XIP_MAPPING;



typedef uint32_t (*T_Hal_Flash_SwWriteProtectEn_fp)(E_SpiSlave_t eSlvIdx, E_FUNC_ST eFunc);



/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
uint32_t Boot_CheckWarmBoot(void);
uint32_t _Hal_Flash_ManufDeviceId_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t *pu32Manufacturer, uint32_t *pu32MemoryType, uint32_t *pu32MemoryDensity );

uint32_t Hal_Flash_Init_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t Hal_Flash_Init_Internal_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t _Hal_Flash_WriteDoneCheck_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t Hal_Flash_AddrRead_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrRead_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrProgram_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrProgram_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
//uint32_t Hal_Flash_4KSectorAddrErase_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr);
uint32_t Hal_Flash_4KSectorAddrErase_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr);
uint32_t Hal_Flash_QuadModeEn_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8QModeEn);
uint32_t _Hal_Flash_RxSampleDelaySetup_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t _Hal_Flash_StatusSet_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8Sts_1, uint8_t u8Sts_2);

uint32_t _Hal_Flash_SwWriteProtectEn_impl(E_SpiSlave_t eSlvIdx, E_FUNC_ST eFunc);
uint32_t _Hal_Flash_SwWriteProtectEn_dummy(E_SpiSlave_t eSlvIdx, E_FUNC_ST eFunc);
void Hal_Flash_InternalFlashWpSetup_impl(void);
void Hal_Flash_InternalFlashWpSetup_dummy(void);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern uint32_t g_u32FlashPollingStatusTimeoutMs;
uint8_t g_u8Hal_ExternalFlashCs = SPI_SLAVE_MAX;

S_HAL_XIP_MAPPING g_sHal_InternalFlashMapping = {FLASH_INTERNAL_MAPPING_START, FLASH_INTERNAL_MAPPING_MASK};
S_HAL_XIP_MAPPING g_sHal_ExternalFlashMapping = {0, 0xFFFFFFFF};

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
extern osSemaphoreId g_taHalFlashSemaphoreId[SPI_IDX_MAX];
extern uint8_t g_u8aHalFlashID[SPI_IDX_MAX][SPI_SLAVE_MAX];

uint32_t g_WriteProtectControlSet = false;
T_Hal_Flash_SwWriteProtectEn_fp _Hal_Flash_SwWriteProtectEn = _Hal_Flash_SwWriteProtectEn_dummy;
T_Hal_Flash_InternalFlashWpSetup_fp Hal_Flash_InternalFlashWpSetup = Hal_Flash_InternalFlashWpSetup_dummy;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Flash_PatchInit(void)
{
    Hal_Flash_Init = Hal_Flash_Init_patch;
    Hal_Flash_Init_Internal = Hal_Flash_Init_Internal_patch;
    _Hal_Flash_WriteDoneCheck = _Hal_Flash_WriteDoneCheck_patch;
    
    Hal_Flash_AddrRead_Ext = Hal_Flash_AddrRead_Ext_patch;
    Hal_Flash_AddrRead_Internal_Ext = Hal_Flash_AddrRead_Internal_Ext_patch;
    Hal_Flash_AddrProgram_Ext = Hal_Flash_AddrProgram_Ext_patch;
    Hal_Flash_AddrProgram_Internal_Ext = Hal_Flash_AddrProgram_Internal_Ext_patch;
    //Hal_Flash_4KSectorAddrErase_Ext = Hal_Flash_4KSectorAddrErase_Ext_patch;
    Hal_Flash_4KSectorAddrErase_Internal_Ext = Hal_Flash_4KSectorAddrErase_Internal_Ext_patch;
    Hal_Flash_QuadModeEn = Hal_Flash_QuadModeEn_patch;
    _Hal_Flash_RxSampleDelaySetup = _Hal_Flash_RxSampleDelaySetup_patch;
    _Hal_Flash_StatusSet = _Hal_Flash_StatusSet_patch;
}


/* For user to read flash ID */
uint32_t Hal_Flash_ReadFlashId(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    uint32_t ret;
    uint32_t u32Manufacturer = 0, u32MemoryType = 0, u32MemoryDensity = 0;
    uint32_t FlashId = 0;
    
    ret = _Hal_Flash_ManufDeviceId(eSpiIdx, eSlvIdx, &u32Manufacturer, &u32MemoryType, &u32MemoryDensity);
    
    if (ret)
        return 0;
    
    FlashId = ((u32Manufacturer  & 0xFF) << 16) |
              ((u32MemoryType    & 0xFF) << 8 ) |
              ((u32MemoryDensity & 0xFF) << 0 );
    return FlashId;
}

/* Only for SPI0 use */
uint32_t Hal_Flash_ConvertXipAhbAddress(E_SpiSlave_t eSlvIdx, uint32_t u32FlashAddr)
{
    uint32_t u32Address = 0;
    if (eSlvIdx == SPI_SLAVE_0)
    {
        u32Address = ((u32FlashAddr - g_u32Hal_QspiRemapAddr) & g_sHal_InternalFlashMapping.Mask) + g_sHal_InternalFlashMapping.Start;
    }
    else if (eSlvIdx == g_u8Hal_ExternalFlashCs)
    {
        u32Address = ((u32FlashAddr - g_u32Hal_QspiRemapAddr) & g_sHal_ExternalFlashMapping.Mask) + g_sHal_ExternalFlashMapping.Start;
    }
    
    return u32Address;
}
/*************************************************************************
* FUNCTION:
*  Hal_WriteProtectControlSet
*
* DESCRIPTION:
*   1. To enable/disable write protect control
*      This function must be called in __Patch_EntryPoint and before SysInit_EntryPoint
*   2. This function can be called only once. i.e. The write protect control can NOT dynamically be changed.
*
* CALLS
*
* PARAMETERS
*   1. eFunc : ENABLE or DISABLE the write protect control
*
* RETURNS
*   0: setting complete (Identified SPI flash)
*   1: error  (No SPI flash)
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_WriteProtectControlSet(E_FUNC_ST eFunc)
{
    if (g_WriteProtectControlSet)
        return 1;
    if (eFunc == ENABLE)
    {
        _Hal_Flash_SwWriteProtectEn = _Hal_Flash_SwWriteProtectEn_impl;
        Hal_Flash_InternalFlashWpSetup = Hal_Flash_InternalFlashWpSetup_impl;
    }
    else
    {
        _Hal_Flash_SwWriteProtectEn = _Hal_Flash_SwWriteProtectEn_dummy;
        Hal_Flash_InternalFlashWpSetup = Hal_Flash_InternalFlashWpSetup_dummy;
    }
    g_WriteProtectControlSet = 1;
    return 0;
}
/*************************************************************************
* FUNCTION:
*  Hal_WriteProtectControlGet
*
* DESCRIPTION:
*   1. Get current write protect control status
*
* CALLS
*
* PARAMETERS
*
* RETURNS
*   0: Disable write protect control
*   1: Enable write protect control
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
E_FUNC_ST Hal_WriteProtectControlGet(void)
{
    return (_Hal_Flash_SwWriteProtectEn==_Hal_Flash_SwWriteProtectEn_impl)?ENABLE:DISABLE;
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */





/*************************************************************************
* FUNCTION:
*  Hal_Flash_Init
*
* DESCRIPTION:
*   1. Init flash status
*       Remove RX sample delay training. Application needs to set RX sample delay when speed up SPI
*       SPI0: Hal_Qspi_RxSampleDly_Set
*       SPI1/3: Hal_Spi_RxSampleDly_Set
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t
*
* RETURNS
*   0: setting complete (Identified SPI flash)
*   1: error  (No SPI flash)
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_Init_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (0 != Hal_Flash_Init_Internal(eSpiIdx, eSlvIdx))
        return 1;

    if (g_taHalFlashSemaphoreId[eSpiIdx] == NULL)
    {        
        // create the semaphore
        return Hal_Flash_InitSemaphore(eSpiIdx);
    }
    return 0;
}

/*
 * Remove RX sample delay training. Application needs to set RX sample delay when speed up SPI
 * SPI0: Hal_Qspi_RxSampleDly_Set
 * SPI1/3: Hal_Spi_RxSampleDly_Set
 */
uint32_t Hal_Flash_Init_Internal_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    uint32_t u32Manufacturer = 0, u32MemoryType = 0, u32MemoryDensity = 0;
    
    // Read flash ID
    _Hal_Flash_ManufDeviceId(eSpiIdx, eSlvIdx, &u32Manufacturer, &u32MemoryType, &u32MemoryDensity);

    // ID Check
    if ((u32Manufacturer == NULL_0_ID) || (u32Manufacturer == NULL_1_ID))
    {
        // Not supported
        g_u8aHalFlashID[eSpiIdx][eSlvIdx] = NO_FLASH;
        return 1;
    }
    else
    {
        // Supported 
        if (eSpiIdx == SPI_IDX_0)
        {   /* Only check SPI0 */
            if (eSlvIdx == SPI_SLAVE_0)
            {   /* Internal flash */
                g_u8aHalFlashID[eSpiIdx][eSlvIdx] = u32Manufacturer;
            }
            else
            {   /* External flash */
                if (g_u8Hal_ExternalFlashCs == SPI_SLAVE_MAX)
                {   /* Not init yet */
                    Hal_Qspi_UpdateDeviceSize(eSlvIdx, QSPI_DEV_SIZE_64MBIT);
                    g_u8Hal_ExternalFlashCs = eSlvIdx;
                    if (eSlvIdx == SPI_SLAVE_1)
                        g_sHal_ExternalFlashMapping.Start = FLASH_EXTERNAL_CS1_MAPPING_START;
                    else if (eSlvIdx == SPI_SLAVE_2)
                        g_sHal_ExternalFlashMapping.Start = FLASH_EXTERNAL_CS2_MAPPING_START;
                    else /* SPI_SLAVE_3 */
                        g_sHal_ExternalFlashMapping.Start = FLASH_EXTERNAL_CS3_MAPPING_START;
                    g_sHal_ExternalFlashMapping.Mask = FLASH_EXTERNAL_MAPPING_MASK;
                    
                    g_u8aHalFlashID[eSpiIdx][eSlvIdx] = u32Manufacturer;
                }
                else
                {   /* Init done */
                    if (eSlvIdx != g_u8Hal_ExternalFlashCs)
                    {   /* Wrong init */
                        g_u8aHalFlashID[eSpiIdx][eSlvIdx] = NO_FLASH;
                        return 1;
                    }
                    else
                    {   /* Already init, skipped */
                    }
                }
            }
        }
        else
        {   /* Other SPIx */
            g_u8aHalFlashID[eSpiIdx][eSlvIdx] = u32Manufacturer;
        }
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_WriteDoneCheck
*
* DESCRIPTION:
*   1. Check flash write finished
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
uint32_t _Hal_Flash_WriteDoneCheck_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;

    uint32_t u32Status = 0;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;

    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32FlashPollingStatusTimeoutMs;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);
        
        S_QSPI_Command_t tCmd;
        memset(&tCmd, 0, sizeof(S_QSPI_Command_t));
        tCmd.Opcode = FLASH_CMD_READ_STATUS_REG;
        tCmd.RdDataEn = 1;
        tCmd.eRdDataByteNum = QSPI_Data_1_Byte;
        
        u32TickStart = Hal_Tick_Diff(0);
        do 
        {
            u32TickDiff = Hal_Tick_Diff(u32TickStart);
            if (u32TickDiff > u32TimeoutTicks)
                return 1;
                        
            Hal_Qspi_Command_Exec(&tCmd);
            u32Status = (tCmd.RdDataLow & 0x000000FF);
        } while (u32Status & FLASH_STATUS_BUSY);
        
        
        /* After SPI0 write to flash and call STIG command, it will trigger write completion feature
         * During the polling busy status state, the STIG command is not working.
         * Using the Polling Flash Status Register (0xB0) to checking the busy status */
        Hal_Qspi_WaitPollingState();
        return 0;
    }
    else // Designware_SPI
    {
        return _Hal_Flash_WriteDoneCheck_impl(eSpiIdx, eSlvIdx);
    }
}



/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrRead_Ext
*
* DESCRIPTION:
*   1. Read n bytes from the start address
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32StartAddr  : Start address
*   4. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   5. u32Size       : Data size
*   6. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_AddrRead_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    uint8_t ubRet = 1;
        
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return ubRet;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return ubRet;
    
    if ((eSlvIdx != SPI_SLAVE_0) && (eSlvIdx != g_u8Hal_ExternalFlashCs))
        return ubRet;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return ubRet;
    
    if (u32Size == 0)
        return ubRet;
    
    if (pu8Data == NULL)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[eSpiIdx], osWaitForever);
    if (eSpiIdx == SPI_IDX_0)
    {
        int64_t remain_size = u32Size;
        while (remain_size > 0)
        {
            uint32_t read_size = (remain_size < RW_ONCE_MAX)?remain_size:RW_ONCE_MAX;
            ubRet = Hal_Flash_AddrRead_Internal_Ext(eSpiIdx, eSlvIdx, u32StartAddr, u8UseQuadMode, read_size, pu8Data);
            if (ubRet)
                break;
            u32StartAddr += RW_ONCE_MAX;
            pu8Data += RW_ONCE_MAX;
            remain_size -= RW_ONCE_MAX;
        }
    }
    else
    {
        ubRet = Hal_Flash_AddrRead_Internal_Ext(eSpiIdx, eSlvIdx, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
    }
    
    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[eSpiIdx]);
    return ubRet;
}
/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrProgram_Ext
*
* DESCRIPTION:
*   1. Program(write) n bytes from the start address
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32StartAddr  : Start address
*   4. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   5. u32Size       : Data size
*   6. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_AddrProgram_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    uint8_t ubRet = 1;
    
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return ubRet;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return ubRet;
    
    if ((eSlvIdx != SPI_SLAVE_0) && (eSlvIdx != g_u8Hal_ExternalFlashCs))
        return ubRet;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return ubRet;
    
    if (u32Size == 0)
        return ubRet;
    
    if (pu8Data == NULL)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[eSpiIdx], osWaitForever);
    if (eSpiIdx == SPI_IDX_0)
    {
        int64_t remain_size = u32Size;
        while (remain_size > 0)
        {
            uint32_t write_size = (remain_size < RW_ONCE_MAX)?remain_size:RW_ONCE_MAX;
            ubRet = Hal_Flash_AddrProgram_Internal_Ext(eSpiIdx, eSlvIdx, u32StartAddr, u8UseQuadMode, write_size, pu8Data);
            if (ubRet)
                break;
            u32StartAddr += RW_ONCE_MAX;
            pu8Data += RW_ONCE_MAX;
            remain_size -= RW_ONCE_MAX;
        }
    }
    else
    {
        ubRet = Hal_Flash_AddrProgram_Internal_Ext(eSpiIdx, eSlvIdx, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
    }

    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[eSpiIdx]);
    return ubRet;
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
        uint32_t u32Addr, u32EndAddr;
        uint32_t u32TempSize;
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        if (eSlvIdx == SPI_SLAVE_0)
        {
            _Hal_Flash_SwWriteProtectEn(eSlvIdx, DISABLE);
            u32EndAddr = g_sHal_InternalFlashMapping.Start + g_sHal_InternalFlashMapping.Mask;
        }
        else
        {
            u32EndAddr = g_sHal_ExternalFlashMapping.Start + g_sHal_ExternalFlashMapping.Mask;
        }
        u32Addr = FLASH_TO_XIP_ADDR(eSlvIdx, u32StartAddr);
        if (u32Addr + u32Size > u32EndAddr)
        {   /* Cross AHB region, seperate to two parts */
            uint8_t u8Value;
            u32TempSize = u32EndAddr - u32Addr + 1;
            FLASH_CRITICAL_ENTER();
            memcpy((void *)u32Addr, pu8Data, u32TempSize);
            /* Second part */
            u32Addr = FLASH_TO_XIP_ADDR(eSlvIdx, u32StartAddr + u32TempSize);
            u32Size -= u32TempSize;
            pu8Data += u32TempSize;
            memcpy((void *)u32Addr, pu8Data, u32Size);
            u8Value = *(volatile uint8_t *)u32Addr;  //Write completion to wait busy status
            (void)u8Value;
            FLASH_CRITICAL_EXIT();
        }
        else
        {
            uint8_t u8Value;
            FLASH_CRITICAL_ENTER();
            memcpy((void *)u32Addr, pu8Data, u32Size);
            u8Value = *(volatile uint8_t *)u32Addr;  //Write completion to wait busy status
            (void)u8Value;
            FLASH_CRITICAL_EXIT();
        }
        if (eSlvIdx == SPI_SLAVE_0)
            _Hal_Flash_SwWriteProtectEn(eSlvIdx, ENABLE);
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
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        uint32_t u32Addr, u32EndAddr;
        uint32_t u32TempSize;
        
        if (eSlvIdx == SPI_SLAVE_0)
        {
            u32EndAddr = g_sHal_InternalFlashMapping.Start + g_sHal_InternalFlashMapping.Mask;
        }
        else
        {
            u32EndAddr = g_sHal_ExternalFlashMapping.Start + g_sHal_ExternalFlashMapping.Mask;
        }
        u32Addr = FLASH_TO_XIP_ADDR(eSlvIdx, u32StartAddr);
        if (u32Addr + u32Size > u32EndAddr)
        {   /* Cross AHB region, seperate to two parts */
            u32TempSize = u32EndAddr - u32Addr + 1;
            memcpy(pu8Data, (void *)u32Addr, u32TempSize);
            /* Second part */
            u32Addr = FLASH_TO_XIP_ADDR(eSlvIdx, u32StartAddr + u32TempSize);
            u32Size -= u32TempSize;
            pu8Data += u32TempSize;
            memcpy(pu8Data, (void *)u32Addr, u32Size);
        }
        else
        {
            memcpy(pu8Data, (void *)u32Addr, u32Size);
        }
        
        
        return 0;
    }
    else
    {
        return Hal_Flash_AddrRead_Internal_Ext_impl(eSpiIdx, eSlvIdx, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
    }
}

#if 0
//Same as _impl
/*************************************************************************
* FUNCTION:
*  Hal_Flash_4KSectorAddrErase_Ext
*
* DESCRIPTION:
*   1. Erase a sector (4 KB)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx    : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32SecAddr : Address of sector (must sector aligned, LSBs truncated)
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_4KSectorAddrErase_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr)
{
    uint8_t ubRet = 1;

    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return ubRet;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return ubRet;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[eSpiIdx], osWaitForever);

    if (0 != Hal_Flash_4KSectorAddrErase_Internal_Ext(eSpiIdx, eSlvIdx, u32SecAddr))
    {
        goto done;
    }

    ubRet = 0;

done:

    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[eSpiIdx]);
    return ubRet;
}
#endif

/* Wait flash erase done to avoid XIP running fail */
uint32_t Hal_Flash_4KSectorAddrErase_Internal_Ext_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr)
{
    uint32_t ret;
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return 1;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        uint32_t u32Addr;
        uint8_t u8Value;
        u32Addr = Hal_Flash_ConvertXipAhbAddress(eSlvIdx, u32SecAddr);
        
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);
        
        S_QSPI_Command_t tCmd;
        memset(&tCmd, 0, sizeof(S_QSPI_Command_t));
        tCmd.Opcode = FLASH_CMD_SECTOR_ERASE;
        tCmd.CmdAddrEn = 1;
        tCmd.eAddrByteNum = QSPI_Addr_3_Byte;
        tCmd.CmdAddr = (u32SecAddr & 0xFFFFF000); // aligned
        
        if (eSlvIdx == SPI_SLAVE_0)
            _Hal_Flash_SwWriteProtectEn(eSlvIdx, DISABLE);
        
        // write enable
        _Hal_Flash_WriteEn(eSpiIdx, eSlvIdx);
        
        FLASH_CRITICAL_ENTER();
        Hal_Qspi_Command_Exec(&tCmd);
        if (eSlvIdx == SPI_SLAVE_0)
        {
            *(uint8_t *)u32Addr = 0xFF;
            FLASH_CRITICAL_EXIT();
            u8Value = *(volatile uint8_t *)u32Addr;  //Write completion to wait busy status
            (void)u8Value;
            _Hal_Flash_SwWriteProtectEn(eSlvIdx, ENABLE);
        }
        else
        {
            FLASH_CRITICAL_EXIT();
            _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        }
        return 0;
    }
    else
    {
        ret = Hal_Flash_4KSectorAddrErase_Internal_Ext_impl(eSpiIdx, eSlvIdx, u32SecAddr);
    }
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
    
    uint32_t ret=0;
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
    }
    return ret;
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

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_StatusSet
*
* DESCRIPTION:
*   1. Set the status register of flash
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t
*   3. u8Sts_1 : value of status register-1
*   4. u8Sts_2 : value of status register-2
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_StatusSet_patch(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8Sts_1, uint8_t u8Sts_2)
{
    uint32_t u32Addr;
    uint8_t u8Value;
    uint32_t ret = 0;
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    // write enable
    if (_Hal_Flash_WriteEn(eSpiIdx, eSlvIdx))
        return 1;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);
        
        S_QSPI_Command_t tCmd;
        memset(&tCmd, 0, sizeof(S_QSPI_Command_t));
        tCmd.Opcode = FLASH_CMD_WRITE_STATUS_REG;
        tCmd.WrDataEn = 1;
        tCmd.eWrDataByteNum = QSPI_Data_2_Byte;
        tCmd.WrDataLow = u8Sts_1 | (u8Sts_2 << 8);
        
        u32Addr = FLASH_TO_XIP_ADDR(eSlvIdx, 0);
        u8Value = *(uint8_t *)u32Addr;
        FLASH_CRITICAL_ENTER();
        Hal_Qspi_Command_Exec(&tCmd);
        if (eSlvIdx == SPI_SLAVE_0)
        {
            *(uint8_t *)u32Addr = u8Value;
            FLASH_CRITICAL_EXIT();
            u8Value = *(volatile uint8_t *)u32Addr;  //Write completion to wait busy status
            (void)u8Value;
        }
        else
        {
            FLASH_CRITICAL_EXIT();
            _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        }
        ret = 0;
    }
    else // Designware_SPI
    {
        ret = _Hal_Flash_StatusSet_impl(eSpiIdx, eSlvIdx, u8Sts_1, u8Sts_2);
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
    }
    return ret;
}


/*************************************************************************
* FUNCTION:
*  _Hal_Flash_SwWriteProtectEn
*
* DESCRIPTION:
*   1. To enable or disable flash software write protect
*      Only support QSPI, and flash status as follows
*    Status0
*      S7 |  S6 |  S5 |  S4 |  S3 |  S2 |  S1 |  S0
*    SRP0 | BP4 | BP3 | BP2 | BP1 | BP0 | WEL | WIP
*
* CALLS
*
* PARAMETERS
*   1. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t
*   2. eFunc : ENABLE or DISABLE flash sw write protect
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_SwWriteProtectEn_impl(E_SpiSlave_t eSlvIdx, E_FUNC_ST eFunc)
{
    uint32_t u32Status[2];

    _Hal_Flash_WriteDoneCheck(SPI_IDX_0, eSlvIdx);
    _Hal_Flash_Status1Get(SPI_IDX_0, eSlvIdx, &u32Status[0]);
    _Hal_Flash_Status2Get(SPI_IDX_0, eSlvIdx, &u32Status[1]);

    if (eFunc == ENABLE)
    {
        u32Status[0] |= FLASH_BP_BIT_MASK;
    }
    else
    {
        u32Status[0] &= ~FLASH_BP_BIT_MASK;
    }
    _Hal_Flash_StatusSet(SPI_IDX_0, eSlvIdx, u32Status[0], u32Status[1]);

    return _Hal_Flash_WriteDoneCheck(SPI_IDX_0, eSlvIdx); // poll busy
}

uint32_t _Hal_Flash_SwWriteProtectEn_dummy(E_SpiSlave_t eSlvIdx, E_FUNC_ST eFunc)
{   /* Dummy for disable control write protect feature */
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_InternalFlashWP
*
* DESCRIPTION:
*   1. To enable flash software write protect when init
*
* CALLS
*
* PARAMETERS
*   1. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Flash_InternalFlashWpSetup_impl(void)
{
    uint32_t u32Status;

    _Hal_Flash_WriteDoneCheck(SPI_IDX_0, SPI_SLAVE_0);
    _Hal_Flash_Status1Get(SPI_IDX_0, SPI_SLAVE_0, &u32Status);

    if ((u32Status & FLASH_BP_BIT_MASK) != FLASH_BP_BIT_MASK)
    {
        _Hal_Flash_SwWriteProtectEn(SPI_SLAVE_0, ENABLE);
    }
}
void Hal_Flash_InternalFlashWpSetup_dummy(void){}

