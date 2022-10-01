/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  hal_flash.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This source file defines the functions of flash.
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "string.h"
#include "hal_flash.h"
#include "hal_flash_internal.h"
#include "hal_tick.h"
#include "cmsis_os.h"
#include "boot_sequence.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
uint32_t g_u32FlashPollingStatusTimeoutMs = FLASH_POLLING_BUSY_TIMEOUT_MS;
uint8_t g_u8aHalFlashID[SPI_IDX_MAX][SPI_SLAVE_MAX] = { {NO_FLASH, NO_FLASH, NO_FLASH, NO_FLASH},
                                                        {NO_FLASH, NO_FLASH, NO_FLASH, NO_FLASH},
                                                        {NO_FLASH, NO_FLASH, NO_FLASH, NO_FLASH}};
osSemaphoreId g_taHalFlashSemaphoreId[SPI_IDX_MAX] = {0, };
uint32_t g_u32FlashTimeoutMs = FLASH_TIMEOUT_MS;
uint32_t g_u32FlashBaseAddr = APS_XIP_MEM_BASE;
uint32_t g_u32aFlashWaitResumeUs[SPI_IDX_MAX][SPI_SLAVE_MAX] = {0,};
T_HalSpiRtSetting g_taHalSpiRuntimeSetting[SPI_IDX_MAX] = {0, };

T__Hal_Flash_ManufDeviceId                  _Hal_Flash_ManufDeviceId                    = _Hal_Flash_ManufDeviceId_impl;
T__Hal_Flash_Status1Get                     _Hal_Flash_Status1Get                       = _Hal_Flash_Status1Get_impl;
T__Hal_Flash_Status2Get                     _Hal_Flash_Status2Get                       = _Hal_Flash_Status2Get_impl;
T__Hal_Flash_StatusSet                      _Hal_Flash_StatusSet                        = _Hal_Flash_StatusSet_impl;
T__Hal_Flash_WriteDoneCheck                 _Hal_Flash_WriteDoneCheck                   = _Hal_Flash_WriteDoneCheck_impl;
T__Hal_Flash_CmdSend                        _Hal_Flash_CmdSend                          = _Hal_Flash_CmdSend_impl;
T__Hal_Flash_WriteEn                        _Hal_Flash_WriteEn                          = _Hal_Flash_WriteEn_impl;
T__Hal_Flash_RxSampleDelaySetup             _Hal_Flash_RxSampleDelaySetup               = _Hal_Flash_RxSampleDelaySetup_impl;
                                            
T__Hal_Flash_Dma_Read_SIO                   _Hal_Flash_Dma_Read_SIO                     = _Hal_Flash_Dma_Read_SIO_impl;
T__Hal_Flash_Dma_Read_QIO                   _Hal_Flash_Dma_Read_QIO                     = _Hal_Flash_Dma_Read_QIO_impl;
T__Hal_Flash_Dma_Write_SIO                  _Hal_Flash_Dma_Write_SIO                    = _Hal_Flash_Dma_Write_SIO_impl;
T__Hal_Flash_Dma_Write_QIO                  _Hal_Flash_Dma_Write_QIO                    = _Hal_Flash_Dma_Write_QIO_impl;

T_Hal_Flash_Init_Internal                   Hal_Flash_Init_Internal                     = Hal_Flash_Init_Internal_impl;
T_Hal_Flash_4KSectorAddrErase_Internal_Ext  Hal_Flash_4KSectorAddrErase_Internal_Ext    = Hal_Flash_4KSectorAddrErase_Internal_Ext_impl;
T_Hal_Flash_4KSectorAddrErase_Internal      Hal_Flash_4KSectorAddrErase_Internal        = Hal_Flash_4KSectorAddrErase_Internal_impl;
T_Hal_Flash_4KSectorIdxErase_Internal       Hal_Flash_4KSectorIdxErase_Internal         = Hal_Flash_4KSectorIdxErase_Internal_impl;
T_Hal_Flash_PageAddrProgram_Internal        Hal_Flash_PageAddrProgram_Internal          = Hal_Flash_PageAddrProgram_Internal_impl;
T_Hal_Flash_PageIdxProgram_Internal         Hal_Flash_PageIdxProgram_Internal           = Hal_Flash_PageIdxProgram_Internal_impl;
T_Hal_Flash_PageAddrRead_Internal           Hal_Flash_PageAddrRead_Internal             = Hal_Flash_PageAddrRead_Internal_impl;
T_Hal_Flash_AddrProgram_Internal_Ext        Hal_Flash_AddrProgram_Internal_Ext          = Hal_Flash_AddrProgram_Internal_Ext_impl;
T_Hal_Flash_AddrProgram_Internal            Hal_Flash_AddrProgram_Internal              = Hal_Flash_AddrProgram_Internal_impl;
T_Hal_Flash_AddrRead_Internal_Ext           Hal_Flash_AddrRead_Internal_Ext             = Hal_Flash_AddrRead_Internal_Ext_impl;
T_Hal_Flash_AddrRead_Internal               Hal_Flash_AddrRead_Internal                 = Hal_Flash_AddrRead_Internal_impl;
T_Hal_Flash_Reset_Internal                  Hal_Flash_Reset_Internal                    = Hal_Flash_Reset_Internal_impl;

T_Hal_Flash_Init                            Hal_Flash_Init                              = Hal_Flash_Init_impl;
T_Hal_Flash_QuadModeEn                      Hal_Flash_QuadModeEn                        = Hal_Flash_QuadModeEn_impl;
T_Hal_Flash_Opcode_Config                   Hal_Flash_Opcode_Config                     = Hal_Flash_Opcode_Config_impl;
T_Hal_Flash_4KSectorAddrErase_Ext           Hal_Flash_4KSectorAddrErase_Ext             = Hal_Flash_4KSectorAddrErase_Ext_impl;
T_Hal_Flash_4KSectorAddrErase               Hal_Flash_4KSectorAddrErase                 = Hal_Flash_4KSectorAddrErase_impl;
T_Hal_Flash_4KSectorIdxErase                Hal_Flash_4KSectorIdxErase                  = Hal_Flash_4KSectorIdxErase_impl;
T_Hal_Flash_PageAddrProgram                 Hal_Flash_PageAddrProgram                   = Hal_Flash_PageAddrProgram_impl;
T_Hal_Flash_PageIdxProgram                  Hal_Flash_PageIdxProgram                    = Hal_Flash_PageIdxProgram_impl;
T_Hal_Flash_PageAddrRead                    Hal_Flash_PageAddrRead                      = Hal_Flash_PageAddrRead_impl;
T_Hal_Flash_AddrProgram_Ext                 Hal_Flash_AddrProgram_Ext                   = Hal_Flash_AddrProgram_Ext_impl;
T_Hal_Flash_AddrProgram                     Hal_Flash_AddrProgram                       = Hal_Flash_AddrProgram_impl;
T_Hal_Flash_AddrRead_Ext                    Hal_Flash_AddrRead_Ext                      = Hal_Flash_AddrRead_Ext_impl;
T_Hal_Flash_AddrRead                        Hal_Flash_AddrRead                          = Hal_Flash_AddrRead_impl;
T_Hal_Flash_Reset                           Hal_Flash_Reset                             = Hal_Flash_Reset_impl;
T_Hal_Flash_Check_Ext                       Hal_Flash_Check_Ext                         = Hal_Flash_Check_Ext_impl;
T_Hal_Flash_Check                           Hal_Flash_Check                             = Hal_Flash_Check_impl;
T_Hal_Flash_Dma_Access                      Hal_Flash_Dma_Access                        = Hal_Flash_Dma_Access_impl; // Only can be used by SPI_0
T_Hal_Flash_Dma_Read                        Hal_Flash_Dma_Read                          = Hal_Flash_Dma_Read_impl;   // Only can be used by SPI_1 & SPI_3
T_Hal_Flash_Dma_Write                       Hal_Flash_Dma_Write                         = Hal_Flash_Dma_Write_impl;  // Only can be used by SPI_1 & SPI_3
T_Hal_Flash_EnterPowerDown                  Hal_Flash_EnterPowerDown                    = Hal_Flash_EnterPowerDown_impl;
T_Hal_Flash_ReleasePowerDown                Hal_Flash_ReleasePowerDown                  = Hal_Flash_ReleasePowerDown_impl;
T_Hal_Flash_GetResumeTimeTicks              Hal_Flash_GetResumeTimeTicks                = Hal_Flash_GetResumeTimeTicks_impl;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
/*************************************************************************
* FUNCTION:
*  _Hal_Flash_ManufDeviceId
*
* DESCRIPTION:
*   1. Get flash Manufacturer ID, Memory Type and Memory Density
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx           : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx           : Index of SPI slave. refer to E_SpiSlave_t
*   3. pu32Manufacturer  : Pointer to receive Manufacturer ID
*   4. pu32MemoryType    : Pointer to receive Memory Type
*   5. pu32MemoryDensity : Pointer to receive Memory Density
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_ManufDeviceId_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t *pu32Manufacturer, uint32_t *pu32MemoryType, uint32_t *pu32MemoryDensity )
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if ( (pu32Manufacturer == NULL) || (pu32MemoryType == NULL) || (pu32MemoryDensity == NULL) )
        return 1;

    if (eSpiIdx == SPI_IDX_0) // Cadence_SPI
    {
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);
        
        S_QSPI_Command_t tCmd;
        memset(&tCmd, 0, sizeof(S_QSPI_Command_t));
        tCmd.Opcode = FLASH_CMD_READ_ID_REG;
        tCmd.RdDataEn = 1;
        tCmd.eRdDataByteNum = QSPI_Data_3_Byte;
        Hal_Qspi_Command_Exec(&tCmd);
        
        *pu32Manufacturer  = (tCmd.RdDataLow & 0x000000FF);
        *pu32MemoryType    = ((tCmd.RdDataLow & 0x0000FF00) >> 8);
        *pu32MemoryDensity = ((tCmd.RdDataLow & 0x00FF0000) >> 16);
    }
    else // Designware_SPI
    {
        uint32_t u32Temp = 0;
        
        // Config SPI
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        tCfg.eSpi_frf = SPI_FRF_STD;
        tCfg.eDfs_32 = SPI_DFS_08_bit;
        tCfg.eTmod = SPI_TMOD_TRX;
        Hal_Spi_Master_Config(eSpiIdx, &tCfg);
        
        // disable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
        // Cmd
        u32Temp = FLASH_CMD_READ_ID_REG;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        u32Temp = FLASH_DR_DUMMY_VAL;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        u32Temp = FLASH_DR_DUMMY_VAL;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        u32Temp = FLASH_DR_DUMMY_VAL;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        // enable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
        // Data
        Hal_Spi_Data_Recv(eSpiIdx, &u32Temp); // dummy data
        Hal_Spi_Data_Recv(eSpiIdx, pu32Manufacturer);
        Hal_Spi_Data_Recv(eSpiIdx, pu32MemoryType);
        Hal_Spi_Data_Recv(eSpiIdx, pu32MemoryDensity);
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_Status1Get
*
* DESCRIPTION:
*   1. Get the status 0 and status-1 of flash
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx    : Index of SPI slave. refer to E_SpiSlave_t
*   3. pu32Status : Pointer to receive status-1
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_Status1Get_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t *pu32Status)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (pu32Status == NULL)
        return 1;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);
        
        S_QSPI_Command_t tCmd;
        memset(&tCmd, 0, sizeof(S_QSPI_Command_t));
        tCmd.Opcode = FLASH_CMD_READ_STATUS_REG;
        tCmd.RdDataEn = 1;
        tCmd.eRdDataByteNum = QSPI_Data_1_Byte;
        Hal_Qspi_Command_Exec(&tCmd);
        *pu32Status = (tCmd.RdDataLow & 0x000000FF);
    }
    else // Designware_SPI
    {
        uint32_t u32Temp = 0;

        // Config SPI
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        tCfg.eSpi_frf = SPI_FRF_STD;
        tCfg.eDfs_32 = SPI_DFS_08_bit;
        tCfg.eTmod = SPI_TMOD_TRX;
        Hal_Spi_Master_Config(eSpiIdx, &tCfg);
        // disable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
        // Cmd
        u32Temp = FLASH_CMD_READ_STATUS_REG;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        u32Temp = FLASH_DR_DUMMY_VAL;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        // enable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
        // Data
        Hal_Spi_Data_Recv(eSpiIdx, &u32Temp); // dummy data
        Hal_Spi_Data_Recv(eSpiIdx, pu32Status);
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_Status2Get
*
* DESCRIPTION:
*   1. Get the status-2 of flash
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx    : Index of SPI slave. refer to E_SpiSlave_t
*   3. pu32Status : Pointer to receive status-2
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_Status2Get_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t *pu32Status)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (pu32Status == NULL)
        return 1;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);
        
        S_QSPI_Command_t tCmd;
        memset(&tCmd, 0, sizeof(S_QSPI_Command_t));
        tCmd.Opcode = (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID) ? FLASH_CMD_READ_STATUS_REG_2_MXIC : FLASH_CMD_READ_STATUS_REG_2_REG;
        tCmd.RdDataEn = 1;
        tCmd.eRdDataByteNum = QSPI_Data_1_Byte;
        Hal_Qspi_Command_Exec(&tCmd);
        *pu32Status = (tCmd.RdDataLow & 0x000000FF);
    }
    else // Designware_SPI
    {
        uint32_t u32Temp = 0;

        // Config SPI
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        tCfg.eSpi_frf = SPI_FRF_STD;
        tCfg.eDfs_32 = SPI_DFS_08_bit;
        tCfg.eTmod = SPI_TMOD_TRX;
        Hal_Spi_Master_Config(eSpiIdx, &tCfg);
        // disable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
        // Cmd
        u32Temp = (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID) ? FLASH_CMD_READ_STATUS_REG_2_MXIC : FLASH_CMD_READ_STATUS_REG_2_REG;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        u32Temp = FLASH_DR_DUMMY_VAL;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        // enable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
        // Data
        Hal_Spi_Data_Recv(eSpiIdx, &u32Temp); // dummy data
        Hal_Spi_Data_Recv(eSpiIdx, pu32Status);
    }
    
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
uint32_t _Hal_Flash_StatusSet_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8Sts_1, uint8_t u8Sts_2)
{
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
        
        Hal_Qspi_Command_Exec(&tCmd);
        
        return 0;
    }
    else // Designware_SPI
    {
        uint32_t u32Temp = 0;
        
        // Config SPI
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        tCfg.eSpi_frf = SPI_FRF_STD;
        tCfg.eDfs_32 = SPI_DFS_08_bit;
        tCfg.eTmod = SPI_TMOD_TX_ONLY;
        Hal_Spi_Master_Config(eSpiIdx, &tCfg);
        // disable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
        // Cmd
        u32Temp = FLASH_CMD_WRITE_STATUS_REG;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        // Data
        u32Temp = u8Sts_1;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        u32Temp = u8Sts_2;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        // enable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
        
        return _Hal_Spi_Poll_Busy(eSpiIdx);
    }
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
uint32_t _Hal_Flash_WriteDoneCheck_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
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
    }
    else // Designware_SPI
    {
        uint32_t u32Temp = 0;

        // config SPI
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        tCfg.eSpi_frf = SPI_FRF_STD;
        tCfg.eDfs_32 = SPI_DFS_08_bit;
        tCfg.eTmod = SPI_TMOD_TRX;
        Hal_Spi_Master_Config(eSpiIdx, &tCfg);
        
        u32TickStart = Hal_Tick_Diff(0);
        do
        {
            u32TickDiff = Hal_Tick_Diff(u32TickStart);
            if (u32TickDiff > u32TimeoutTicks)
                return 1;
            
            // disable slave
            Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
            // Cmd
            u32Temp = FLASH_CMD_READ_STATUS_REG;
            Hal_Spi_Data_Send(eSpiIdx, u32Temp);
            u32Temp = FLASH_DR_DUMMY_VAL;
            Hal_Spi_Data_Send(eSpiIdx, u32Temp);
            // enable slave
            Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
            // Data
            Hal_Spi_Data_Recv(eSpiIdx, &u32Temp); // dummy data
            Hal_Spi_Data_Recv(eSpiIdx, &u32Status);
        } while (u32Status & FLASH_STATUS_BUSY);
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_CmdSend
*
* DESCRIPTION:
*   1. Send command to Flash
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx  : Index of SPI slave. refer to E_SpiSlave_t
*   3. u8Opcode : Opcode
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_CmdSend_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8Opcode)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);

        S_QSPI_Command_t tCmd;
        memset(&tCmd, 0, sizeof(S_QSPI_Command_t));
        tCmd.Opcode = u8Opcode;
        Hal_Qspi_Command_Exec(&tCmd);
        
        return 0;
    }
    else // Designware_SPI
    {
        uint32_t u32Temp = 0;
        
        // Config SPI
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        tCfg.eSpi_frf = SPI_FRF_STD;
        tCfg.eDfs_32 = SPI_DFS_08_bit;
        tCfg.eTmod = SPI_TMOD_TX_ONLY;
        Hal_Spi_Master_Config(eSpiIdx, &tCfg);

        // disable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
        // Cmd
        u32Temp = u8Opcode;
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        // enable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
        
        return _Hal_Spi_Poll_Busy(eSpiIdx);
    }
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_WriteEn
*
* DESCRIPTION:
*   1. Enable flash to write
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
uint32_t _Hal_Flash_WriteEn_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    // polling flash status
    if (_Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx))
        return 1;
    
    return _Hal_Flash_CmdSend(eSpiIdx, eSlvIdx, FLASH_CMD_WRITE_ENABLE);
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
uint32_t _Hal_Flash_RxSampleDelaySetup_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    if (eSpiIdx >= SPI_IDX_MAX) 
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX) 
        return 1;
    
    uint32_t u32StsReg = 0;
    uint16_t u16DlyNum = 0;
    uint16_t u16MaxDlyNum = 0;
    
    _Hal_Flash_WriteEn(eSpiIdx, eSlvIdx);

    do 
    {
        if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
        {
            Hal_Qspi_RxSampleDly_Set((uint8_t) u16DlyNum);
            u16MaxDlyNum = 0xF;
        }
        else // Designware_SPI
        {
            Hal_Spi_RxSampleDly_Set(eSpiIdx, u16DlyNum);
            u16MaxDlyNum = 0x4;
        }
        _Hal_Flash_Status1Get(eSpiIdx, eSlvIdx, &u32StsReg);
        
        u16DlyNum++;
        if (u16DlyNum > u16MaxDlyNum) // already go through from min. to MAX.
            return 1;
    } while ((u32StsReg & FLASH_STATUS_WEL) == 0);
    
    // save current spi speed & corresponding rx sample delay
    if (eSpiIdx == SPI_IDX_0)
        g_taHalSpiRuntimeSetting[eSpiIdx].u32Speed = Hal_Qspi_Baudrate_Get();
    else
        g_taHalSpiRuntimeSetting[eSpiIdx].u32Speed = Hal_Spi_BaudRateGet(eSpiIdx);
    
    g_taHalSpiRuntimeSetting[eSpiIdx].u16RxSampleDly = u16DlyNum;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_Dma_Read_SIO
*
* DESCRIPTION:
*   1. use DMA to read flash data (single-bit mode)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx    : Index of SPI slave. refer to E_SpiSlave_t
*   3. eTxChannel : DMA channel for TX dummy data.
*   4. eRxChannel : DMA channel for RX flash data.
*   5. u32SrcAddr : Source address of flash space.
*   6. u32DstAddr : Destination address of RAM space.
*   7. u32Size    : Size of DMA transfer. Must be multiple of 4 bytes.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_Dma_Read_SIO_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eTxChannel, E_DMA_CHANNEL eRxChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size)
{
    if ( (eSpiIdx == SPI_IDX_0) || (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (eTxChannel >= DMA_Channel_Max)
        return 1;
    
    if (eRxChannel >= DMA_Channel_Max)
        return 1;
    
    if (u32Size == 0)
        return 1;
    
    if (u32Size & 0x3) // Size must be multiple of 4 bytes
        return 1;
    
    S_SPI_MST_Reg_t *pSpi = NULL;
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    
    uint32_t u32Dummy = 0xFFFFFFFF;
    uint32_t u32Temp = 0;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    uint8_t u8Cmd = 0;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32FlashPollingStatusTimeoutMs;
    
    // config DMA
    S_DMA_Init_t tTxDma;
    memset(&tTxDma, 0, sizeof(S_DMA_Init_t));
    S_DMA_Init_t tRxDma;
    memset(&tRxDma, 0, sizeof(S_DMA_Init_t));

    if ((u32Size >> 2) > MAX_DMA_XFER_NUM)
    {
        uint32_t u32LliNum = (u32Size >> 2) / MAX_DMA_XFER_NUM;
        uint32_t u32RemainWord = (u32Size >> 2) % MAX_DMA_XFER_NUM;
        u32LliNum = (u32RemainWord > 0) ? (u32LliNum + 1) : u32LliNum;
        int i = 0;
        S_DMA_CTL_L_t *ptCtl_L = NULL;
        S_DMA_CTL_H_t *ptCtl_H = NULL;
        
        // LLI preparation
        S_DMA_LinkListItem_t tTxLli[u32LliNum];
        memset(&tTxLli[0], 0, sizeof(S_DMA_LinkListItem_t) * u32LliNum);
        S_DMA_LinkListItem_t tRxLli[u32LliNum];
        memset(&tRxLli[0], 0, sizeof(S_DMA_LinkListItem_t) * u32LliNum);
        
        // TX_DMA
        ptCtl_L = (S_DMA_CTL_L_t *) &(tTxLli[0].CTL_L);
        ptCtl_H = (S_DMA_CTL_H_t *) &(tTxLli[0].CTL_H);
        tTxLli[0].SAR = (uint32_t)&u32Dummy;
        tTxLli[0].DAR = (uint32_t)&(pSpi->DR[0]);
        tTxLli[0].LLP = ((uint32_t)(&tTxLli[1])) & 0xFFFFFFFC;
        // Ctl_L
        ptCtl_L->CTL_L_INT_EN = 1;
        ptCtl_L->CTL_L_DST_TR_WIDTH = DMA_TR_WIDTH_32_bits;
        ptCtl_L->CTL_L_SRC_TR_WIDTH = DMA_TR_WIDTH_32_bits;
        ptCtl_L->CTL_L_SRC_MSIZE = DMA_MSIZE_16;
        ptCtl_L->CTL_L_DEST_MSIZE = DMA_MSIZE_16;
        ptCtl_L->CTL_L_DINC = DMA_INC_No_Change;
        ptCtl_L->CTL_L_SINC = DMA_INC_No_Change;
        ptCtl_L->CTL_L_TT_FC = DMA_TT_m2p_FC_Dma;
        ptCtl_L->CTL_L_LLP_DST_EN = 1;
        ptCtl_L->CTL_L_LLP_SRC_EN = 1;
        // Ctl_H
        ptCtl_H->CTL_H_BLOCK_TS = MAX_DMA_XFER_NUM;
        ptCtl_H->CTL_H_DONE = 1;
        for (i = 1; i < u32LliNum; i++)
        {
            if (i != u32LliNum - 1)
            {
                memcpy(&tTxLli[i], &tTxLli[i-1], sizeof(S_DMA_LinkListItem_t));
                tTxLli[i].LLP = ((uint32_t)(&tTxLli[i+1])) & 0xFFFFFFFC;
            }
            else
            {
                memcpy(&tTxLli[i], &tTxLli[i-1], sizeof(S_DMA_LinkListItem_t));
                tTxLli[i].LLP = 0x0;
                ptCtl_L = (S_DMA_CTL_L_t *) &(tTxLli[i].CTL_L);
                ptCtl_L->CTL_L_LLP_DST_EN = 0;
                ptCtl_L->CTL_L_LLP_SRC_EN = 0;
                ptCtl_H = (S_DMA_CTL_H_t *) &(tTxLli[i].CTL_H);
                ptCtl_H->CTL_H_BLOCK_TS = u32RemainWord;
            }
        }
        // Other CFGs would be reprogrammed by LLI
        tTxDma.LlpDstEn = 1;
        tTxDma.LlpSrcEn = 1;
        if (eSpiIdx == SPI_IDX_1)
            tTxDma.DestPer = APS_DMA_ITF_SPI1_TX;
        else if (eSpiIdx == SPI_IDX_3)
            tTxDma.DestPer = APS_DMA_ITF_SPI3_TX;
        tTxDma.Loc = ((uint32_t) &tTxLli[0] >> 2);
        
        // RX_DMA
        ptCtl_L = (S_DMA_CTL_L_t *) &(tRxLli[0].CTL_L);
        ptCtl_H = (S_DMA_CTL_H_t *) &(tRxLli[0].CTL_H);
        tRxLli[0].SAR = (uint32_t)&(pSpi->DR[0]);
        tRxLli[0].DAR = 0;
        tRxLli[0].LLP = ((uint32_t)(&tRxLli[1])) & 0xFFFFFFFC;
        // Ctl_L
        ptCtl_L->CTL_L_INT_EN = 1;
        ptCtl_L->CTL_L_DST_TR_WIDTH = DMA_TR_WIDTH_32_bits;
        ptCtl_L->CTL_L_SRC_TR_WIDTH = DMA_TR_WIDTH_32_bits;
        ptCtl_L->CTL_L_SRC_MSIZE = DMA_MSIZE_16;
        ptCtl_L->CTL_L_DEST_MSIZE = DMA_MSIZE_16;
        ptCtl_L->CTL_L_DINC = DMA_INC_Increment;
        ptCtl_L->CTL_L_SINC = DMA_INC_No_Change;
        ptCtl_L->CTL_L_TT_FC = DMA_TT_p2m_FC_Dma;
        ptCtl_L->CTL_L_LLP_DST_EN = 0;
        ptCtl_L->CTL_L_LLP_SRC_EN = 1;
        // Ctl_H
        ptCtl_H->CTL_H_BLOCK_TS = MAX_DMA_XFER_NUM;
        ptCtl_H->CTL_H_DONE = 1;
        for (i = 1; i < u32LliNum; i++)
        {
            if (i != u32LliNum - 1)
            {
                memcpy(&tRxLli[i], &tRxLli[i-1], sizeof(S_DMA_LinkListItem_t));
                tRxLli[i].SAR = (uint32_t)&(pSpi->DR[0]);
                tRxLli[i].LLP = ((uint32_t)(&tRxLli[i+1])) & 0xFFFFFFFC;
            }
            else
            {
                memcpy(&tRxLli[i], &tRxLli[i-1], sizeof(S_DMA_LinkListItem_t));
                tRxLli[i].SAR = (uint32_t)&(pSpi->DR[0]);
                tRxLli[i].LLP = 0x0;
                ptCtl_L = (S_DMA_CTL_L_t *) &(tRxLli[i].CTL_L);
                ptCtl_L->CTL_L_LLP_DST_EN = 0;
                ptCtl_L->CTL_L_LLP_SRC_EN = 0;
                ptCtl_H = (S_DMA_CTL_H_t *) &(tRxLli[i].CTL_H);
                ptCtl_H->CTL_H_BLOCK_TS = u32RemainWord;
            }
        }
        // Other CFGs would be reprogrammed by LLI
        tRxDma.Dar = u32DstAddr;
        tRxDma.LlpDstEn = 0;
        tRxDma.LlpSrcEn = 1;
        if (eSpiIdx == SPI_IDX_1)
            tRxDma.SrcPer = APS_DMA_ITF_SPI1_RX;
        else if (eSpiIdx == SPI_IDX_3)
            tRxDma.SrcPer = APS_DMA_ITF_SPI3_RX;
        tRxDma.Loc = ((uint32_t) &tRxLli[0] >> 2);
    }
    else
    {
        // setup TX DMA configurations
        tTxDma.Sar = (uint32_t)&u32Dummy;
        tTxDma.Dar = (uint32_t)&(pSpi->DR[0]);
        tTxDma.eTtfc = DMA_TT_m2p_FC_Dma;
        tTxDma.eSrcTrWidth = DMA_TR_WIDTH_32_bits;
        tTxDma.eDstTrWidth = DMA_TR_WIDTH_32_bits;
        tTxDma.eSrcMsize = DMA_MSIZE_16;
        tTxDma.eDestMsize = DMA_MSIZE_16;
        tTxDma.eSinc = DMA_INC_No_Change;
        tTxDma.eDinc = DMA_INC_No_Change;
        tTxDma.BlockTs = (u32Size >> 2);
        if (eSpiIdx == SPI_IDX_1)
            tTxDma.DestPer = APS_DMA_ITF_SPI1_TX;
        else if (eSpiIdx == SPI_IDX_3)
            tTxDma.DestPer = APS_DMA_ITF_SPI3_TX;
        
        // setup RX DMA configurations
        tRxDma.Sar = (uint32_t)&(pSpi->DR[0]);
        tRxDma.Dar = u32DstAddr;
        tRxDma.eTtfc = DMA_TT_p2m_FC_Dma;
        tRxDma.eSrcTrWidth = DMA_TR_WIDTH_32_bits;
        tRxDma.eDstTrWidth = DMA_TR_WIDTH_32_bits;
        tRxDma.eSrcMsize = DMA_MSIZE_16;
        tRxDma.eDestMsize = DMA_MSIZE_16;
        tRxDma.eSinc = DMA_INC_No_Change;
        tRxDma.eDinc = DMA_INC_Increment;
        tRxDma.BlockTs = (u32Size >> 2);
        if (eSpiIdx == SPI_IDX_1)
            tRxDma.SrcPer = APS_DMA_ITF_SPI1_RX;
        else if (eSpiIdx == SPI_IDX_3)
            tRxDma.SrcPer = APS_DMA_ITF_SPI3_RX;
    }
    
    // wait for DMA TxChannel is idle
    u32TickStart = Hal_Tick_Diff(0);
    while (Hal_Dma_Check(eTxChannel))
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    Hal_Dma_Config(eTxChannel, &tTxDma);
    // wait for DMA RxChannel is idle
    u32TickStart = Hal_Tick_Diff(0);
    while (Hal_Dma_Check(eRxChannel))
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    Hal_Dma_Config(eRxChannel, &tRxDma);

    // polling flash status
    _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
    
    // Config SPI
    S_SPI_Config_t tCfg;
    memset(&tCfg, 0, sizeof(S_SPI_Config_t));
    tCfg.eSpi_frf = SPI_FRF_STD;
    tCfg.eDfs_32 = SPI_DFS_32_bit;
    tCfg.eTmod = SPI_TMOD_TRX;
    Hal_Spi_Master_Config(eSpiIdx, &tCfg);

    Hal_Spi_Dma_Config(eSpiIdx, 1, 1, 16, 15);
    
    // disable slave
    Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);

    // Cmd & Addr
    u8Cmd = FLASH_CMD_SIO_READ;
    u32Temp = (u8Cmd) | 
              ((u32SrcAddr & 0xFF0000) >> 8) | 
              ((u32SrcAddr & 0x00FF00) << 8) | 
              ((u32SrcAddr & 0x0000FF) << 24);
    pSpi->DR[0] = u32Temp;
    
    // enable DMA
    // Data (using DMA)
    Hal_Dma_Enable(eTxChannel); // TX DMA
    Hal_Dma_Enable(eRxChannel); // RX DMA
    // enable slave
    Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
    
    // wait for RX_FIFO not empty
    u32TickStart = Hal_Tick_Diff(0);
    while (!(pSpi->SR & SPI_MST_SR_RFNE))
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    
    // retrieve first 4 dummy bytes
    u32Temp = pSpi->DR[0];
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_Dma_Read_QIO
*
* DESCRIPTION:
*   1. use DMA to read flash data (Quad-bit mode)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx    : Index of SPI slave. refer to E_SpiSlave_t
*   3. eChannel   : DMA channel for RX flash data.
*   4. u32SrcAddr : Source address of flash space.
*   5. u32DstAddr : Destination address of RAM space.
*   6. u32Size    : Size of DMA transfer. Must be multiple of 4 bytes.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_Dma_Read_QIO_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size)
{
    if ( (eSpiIdx == SPI_IDX_0) || (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (eChannel >= DMA_Channel_Max)
        return 1;
     
    if (u32Size == 0)
        return 1;
    
    if (u32Size & 0x3)
        return 1;
    
    S_SPI_MST_Reg_t *pSpi = NULL;
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    
    uint32_t u32Temp = 0;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    uint8_t u8Cmd = 0;
    uint8_t u8Modebit = 0;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32FlashPollingStatusTimeoutMs;
    
    // config DMA
    S_DMA_Init_t tDma;
    memset(&tDma, 0, sizeof(S_DMA_Init_t));
    
    if ((u32Size >> 2) > MAX_DMA_XFER_NUM)
    {
        uint32_t u32LliNum = (u32Size >> 2) / MAX_DMA_XFER_NUM;
        uint32_t u32RemainWord = (u32Size >> 2) % MAX_DMA_XFER_NUM;
        u32LliNum = (u32RemainWord > 0) ? (u32LliNum + 1) : u32LliNum;
        uint32_t i = 0;
        
        /* LLI preparation */
        S_DMA_LinkListItem_t tLli[u32LliNum];
        memset(&tLli[0], 0, sizeof(S_DMA_LinkListItem_t) * u32LliNum);

        S_DMA_CTL_L_t *ptCtl_L = (S_DMA_CTL_L_t *) &(tLli[0].CTL_L);
        S_DMA_CTL_H_t *ptCtl_H = (S_DMA_CTL_H_t *) &(tLli[0].CTL_H);
        tLli[0].SAR = (uint32_t)&(pSpi->DR[0]);
        tLli[0].DAR = 0;
        tLli[0].LLP = ((uint32_t)(&tLli[1])) & 0xFFFFFFFC;
        // Ctl_L
        ptCtl_L->CTL_L_INT_EN = 1;
        ptCtl_L->CTL_L_DST_TR_WIDTH = DMA_TR_WIDTH_32_bits;
        ptCtl_L->CTL_L_SRC_TR_WIDTH = DMA_TR_WIDTH_32_bits;
        ptCtl_L->CTL_L_SRC_MSIZE = DMA_MSIZE_16;
        ptCtl_L->CTL_L_DEST_MSIZE = DMA_MSIZE_16;
        ptCtl_L->CTL_L_DINC = DMA_INC_Increment;
        ptCtl_L->CTL_L_SINC = DMA_INC_No_Change;
        ptCtl_L->CTL_L_TT_FC = DMA_TT_p2m_FC_Dma;
        ptCtl_L->CTL_L_LLP_DST_EN = 0;
        ptCtl_L->CTL_L_LLP_SRC_EN = 1;
        // Ctl_H
        ptCtl_H->CTL_H_BLOCK_TS = MAX_DMA_XFER_NUM;
        ptCtl_H->CTL_H_DONE = 1;

        for (i = 1; i < u32LliNum; i++)
        {
            if (i != u32LliNum - 1)
            {
                memcpy(&tLli[i], &tLli[i-1], sizeof(S_DMA_LinkListItem_t));
                tLli[i].SAR = (uint32_t)&(pSpi->DR[0]);
                tLli[i].LLP = ((uint32_t)(&tLli[i+1])) & 0xFFFFFFFC;
            }
            else
            {
                memcpy(&tLli[i], &tLli[i-1], sizeof(S_DMA_LinkListItem_t));
                tLli[i].SAR = (uint32_t)&(pSpi->DR[0]);
                tLli[i].LLP = 0x0;
                ptCtl_L = (S_DMA_CTL_L_t *) &(tLli[i].CTL_L);
                ptCtl_L->CTL_L_LLP_DST_EN = 0;
                ptCtl_L->CTL_L_LLP_SRC_EN = 0;
                ptCtl_H = (S_DMA_CTL_H_t *) &(tLli[i].CTL_H);
                ptCtl_H->CTL_H_BLOCK_TS = u32RemainWord;
            }
        }
        // Other CFGs would be reprogrammed by LLI
        tDma.Dar = u32DstAddr;
        tDma.LlpDstEn = 0;
        tDma.LlpSrcEn = 1;
        if (eSpiIdx == SPI_IDX_1)
            tDma.SrcPer = APS_DMA_ITF_SPI1_RX;
        else if (eSpiIdx == SPI_IDX_3)
            tDma.SrcPer = APS_DMA_ITF_SPI3_RX;
        tDma.Loc = ((uint32_t) &tLli[0] >> 2);
    }
    else
    {
        // setup DMA configurations
        tDma.Sar = (uint32_t)&(pSpi->DR[0]);
        tDma.Dar = u32DstAddr;
        tDma.eTtfc = DMA_TT_p2m_FC_Dma;
        tDma.eSrcTrWidth = DMA_TR_WIDTH_32_bits;
        tDma.eDstTrWidth = DMA_TR_WIDTH_32_bits;
        tDma.eSrcMsize = DMA_MSIZE_16;
        tDma.eDestMsize = DMA_MSIZE_16;
        tDma.eSinc = DMA_INC_No_Change;
        tDma.eDinc = DMA_INC_Increment;
        tDma.BlockTs = (u32Size >> 2);
        if (eSpiIdx == SPI_IDX_1)
            tDma.SrcPer = APS_DMA_ITF_SPI1_RX;
        else if (eSpiIdx == SPI_IDX_3)
            tDma.SrcPer = APS_DMA_ITF_SPI3_RX;
    }

    // wait for DMA Channel is idle
    u32TickStart = Hal_Tick_Diff(0);
    while (Hal_Dma_Check(eChannel))
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    Hal_Dma_Config(eChannel, &tDma);

    // polling flash status
    _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
    
    // Config SPI
    S_SPI_Config_t tCfg;
    memset(&tCfg, 0, sizeof(S_SPI_Config_t));
    tCfg.eSpi_frf = SPI_FRF_QUAD;
    tCfg.eDfs_32 = SPI_DFS_32_bit;
    tCfg.eTmod = SPI_TMOD_RX_ONLY;
    tCfg.eInst_l = SPI_INST_L_8_bit;
    tCfg.eAddr_l = SPI_ADDR_L_32_bit; // 24-bit Addr + 8-bit MODE_BITS
    tCfg.eTrans_type = SPI_INST_std_ADDR_frf;
    tCfg.u16Ndf = (u32Size >> 2) - 1;
    tCfg.u8WaitCycles = 4;
    Hal_Spi_Master_Config(eSpiIdx, &tCfg);

    // enable Receive DMA
    Hal_Spi_Dma_Config(eSpiIdx, 0, 1, 0, 15);
    // Data (using DMA)
    Hal_Dma_Enable(eChannel); // RX DMA
    
    // Cmd
    u8Cmd = FLASH_CMD_QIO_READ;
    u8Modebit = 0xFF;
    pSpi->DR[0] = (u8Cmd << 24);
    // Addr
    u32Temp = ((u32SrcAddr & 0xFF0000) >> 16) | 
              ((u32SrcAddr & 0x00FF00)) | 
              ((u32SrcAddr & 0x0000FF) << 16) | 
              (u8Modebit << 24);
    pSpi->DR[0] = u32Temp;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_Dma_Write_SIO
*
* DESCRIPTION:
*   1. use DMA to program flash (Single-bit mode)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx    : Index of SPI slave. refer to E_SpiSlave_t
*   3. eChannel   : DMA channel for TX data to SPI TX_FIFO.
*   4. u32SrcAddr : Source address of RAM space.
*   5. u32DstAddr : Destination address of flash space.
*   6. u32Size    : Size of DMA transfer. Must less than 1 page size (256 Byte).
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_Dma_Write_SIO_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size)
{
    if ( (eSpiIdx == SPI_IDX_0) || (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (eChannel >= DMA_Channel_Max)
        return 1;
    
    if (u32Size == 0)
        return 1;
    
    if (u32Size > FLASH_PAGE_SIZE) // Must less than 1 page size
        return 1;
    
    S_SPI_MST_Reg_t *pSpi = NULL;
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    
    uint32_t u32Temp = 0;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    uint8_t u8Cmd = 0;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32FlashPollingStatusTimeoutMs;
 
    // config DMA
    S_DMA_Init_t tDma;
    memset(&tDma, 0, sizeof(S_DMA_Init_t));

    // setup TX DMA configurations
    tDma.Sar = u32SrcAddr;
    tDma.Dar = (uint32_t)&(pSpi->DR[0]);
    tDma.eTtfc = DMA_TT_m2p_FC_Dma;
    tDma.eSrcTrWidth = DMA_TR_WIDTH_32_bits;
    tDma.eDstTrWidth = DMA_TR_WIDTH_32_bits;
    tDma.eSrcMsize = DMA_MSIZE_16;
    tDma.eDestMsize = DMA_MSIZE_16;
    tDma.eSinc = DMA_INC_Increment;
    tDma.eDinc = DMA_INC_No_Change;
    tDma.BlockTs = (u32Size >> 2);
    if (eSpiIdx == SPI_IDX_1)
        tDma.DestPer = APS_DMA_ITF_SPI1_TX;
    else if (eSpiIdx == SPI_IDX_3)
        tDma.DestPer = APS_DMA_ITF_SPI3_TX;
    
    // wait for DMA Channel is idle
    u32TickStart = Hal_Tick_Diff(0);
    while (Hal_Dma_Check(eChannel))
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    Hal_Dma_Config(eChannel, &tDma);

    // Config SPI
    S_SPI_Config_t tCfg;
    memset(&tCfg, 0, sizeof(S_SPI_Config_t));
    tCfg.eSpi_frf = SPI_FRF_STD;
    tCfg.eDfs_32 = SPI_DFS_32_bit;
    tCfg.eTmod = SPI_TMOD_TX_ONLY;
    Hal_Spi_Master_Config(eSpiIdx, &tCfg);
    
    // enable Transmit DMA
    Hal_Spi_Dma_Config(eSpiIdx, 1, 0, 16, 0);
    
    // disable slave
    Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
    // Cmd & Addr
    u8Cmd = FLASH_CMD_SIO_PROGRAM;
    u32Temp = (u8Cmd) | 
              ((u32DstAddr & 0xFF0000) >> 8) | 
              ((u32DstAddr & 0x00FF00) << 8) | 
              ((u32DstAddr & 0x0000FF) << 24);
    pSpi->DR[0] = u32Temp;
    // Data (using DMA)
    Hal_Dma_Enable(eChannel); // TX DMA
    // enable slave
    Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Flash_Dma_Write_QIO
*
* DESCRIPTION:
*   1. use DMA to program flash (Quad-bit mode)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx    : Index of SPI slave. refer to E_SpiSlave_t
*   3. eChannel   : DMA channel for TX data to SPI TX_FIFO.
*   4. u32SrcAddr : Source address of RAM space.
*   5. u32DstAddr : Destination address of flash space.
*   6. u32Size    : Size of DMA transfer. Must less than one page size (256 Byte).
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Flash_Dma_Write_QIO_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size)
{
    if ( (eSpiIdx == SPI_IDX_0) || (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (eChannel >= DMA_Channel_Max)
        return 1;
    
    if (u32Size == 0)
        return 1;
    
    if (u32Size > FLASH_PAGE_SIZE) // Must less than 1 page size
        return 1;
    
    S_SPI_MST_Reg_t *pSpi = NULL;
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    
    uint32_t u32Temp = 0;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    uint8_t u8Cmd = 0;

    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32FlashPollingStatusTimeoutMs;
    
    // config DMA
    S_DMA_Init_t tDma;
    memset(&tDma, 0, sizeof(S_DMA_Init_t));
    
    // setup DMA configurations
    tDma.Sar = u32SrcAddr;
    tDma.Dar = (uint32_t)&(pSpi->DR[0]);
    tDma.eTtfc = DMA_TT_m2p_FC_Dma;
    tDma.eSrcTrWidth = DMA_TR_WIDTH_32_bits;
    tDma.eDstTrWidth = DMA_TR_WIDTH_32_bits;
    tDma.eSrcMsize = DMA_MSIZE_16;
    tDma.eDestMsize = DMA_MSIZE_16;
    tDma.eSinc = DMA_INC_Increment;
    tDma.eDinc = DMA_INC_No_Change;
    tDma.BlockTs = (u32Size >> 2);
    if (eSpiIdx == SPI_IDX_1)
        tDma.DestPer = APS_DMA_ITF_SPI1_TX;
    else if (eSpiIdx == SPI_IDX_3)
        tDma.DestPer = APS_DMA_ITF_SPI3_TX;

    // wait for DMA Channel is idle
    u32TickStart = Hal_Tick_Diff(0);
    while (Hal_Dma_Check(eChannel))
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    Hal_Dma_Config(eChannel, &tDma);
    
    // Config SPI
    S_SPI_Config_t tCfg;
    memset(&tCfg, 0, sizeof(S_SPI_Config_t));
    tCfg.eSpi_frf = SPI_FRF_QUAD;
    tCfg.eDfs_32 = SPI_DFS_32_bit;
    tCfg.eTmod = SPI_TMOD_TX_ONLY;
    tCfg.eInst_l = SPI_INST_L_8_bit;
    tCfg.eAddr_l = SPI_ADDR_L_24_bit;
    tCfg.eTrans_type = (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID) ? SPI_INST_std_ADDR_frf : SPI_INST_std_ADDR_std;
    Hal_Spi_Master_Config(eSpiIdx, &tCfg);

    // enable Transmit DMA
    Hal_Spi_Dma_Config(eSpiIdx, 1, 0, 16, 0);

    // disable slave
    Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
    // Cmd
    u8Cmd = (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID) ? FLASH_CMD_QIO_PROGRAM_MXIC : FLASH_CMD_QIO_PROGRAM;
    u32Temp = (u8Cmd << 24);
    pSpi->DR[0] = u32Temp;
    // Addr
    u32Temp = ((u32DstAddr & 0xFF0000) >> 8) | 
              ((u32DstAddr & 0x00FF00) << 8) | 
              ((u32DstAddr & 0x0000FF) << 24);
    pSpi->DR[0] = u32Temp;
    // Data (using DMA)
    Hal_Dma_Enable(eChannel); // TX DMA
    // enable slave
    Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);

    return 0;
}



/*************************************************************************
* FUNCTION:
*  Hal_Flash_Init_Internal
*
* DESCRIPTION:
*   1. Init flash status
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t
*
* RETURNS
*   0: setting complete (Identified SPI flash)
*   1: error  (No SPI flash)
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_Init_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    uint32_t u32Manufacturer = 0, u32MemoryType = 0, u32MemoryDensity = 0;
    uint32_t u32SpiSpeed = 0;
    
    // Setup RX SAMPLE DELAY
    if (Boot_CheckWarmBoot())
    {
        /* Warm boot */
        if (eSpiIdx == SPI_IDX_0)
            u32SpiSpeed = Hal_Qspi_Baudrate_Get();
        else
            u32SpiSpeed = Hal_Spi_BaudRateGet(eSpiIdx);
        
        if (u32SpiSpeed != g_taHalSpiRuntimeSetting[eSpiIdx].u32Speed)
        {
            // if not same speed, run the RxSampleDelaySetup process all over again
            if (_Hal_Flash_RxSampleDelaySetup(eSpiIdx, eSlvIdx))
                return 1;
        }
        else
        {
            Hal_Spi_RxSampleDly_Set(eSpiIdx, g_taHalSpiRuntimeSetting[eSpiIdx].u16RxSampleDly);
        }
        return 0;
    }
    else
    {
        /* Cold boot */
        if (_Hal_Flash_RxSampleDelaySetup(eSpiIdx, eSlvIdx))
            return 1;
    }
    
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
        g_u8aHalFlashID[eSpiIdx][eSlvIdx] = u32Manufacturer;
    }

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_4KSectorAddrErase_Internal_Ext
*
* DESCRIPTION:
*   1. Erase a sector (4 KB)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
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
uint32_t Hal_Flash_4KSectorAddrErase_Internal_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return 1;
    
    if (eSpiIdx == SPI_IDX_0) // Cadence_QSPI
    {
        // Check peripheral chip select
        Hal_Qspi_Peri_Select(eSlvIdx);
        
        S_QSPI_Command_t tCmd;
        memset(&tCmd, 0, sizeof(S_QSPI_Command_t));
        tCmd.Opcode = FLASH_CMD_SECTOR_ERASE;
        tCmd.CmdAddrEn = 1;
        tCmd.eAddrByteNum = QSPI_Addr_3_Byte;
        tCmd.CmdAddr = (u32SecAddr & 0xFFFFF000); // aligned
        
        // write enable
        _Hal_Flash_WriteEn(eSpiIdx, eSlvIdx);
        Hal_Qspi_Command_Exec(&tCmd);
        
        return 0;
    }
    else // Designware_SPI
    {
        uint32_t u32Temp = 0;
        uint32_t u32Addr = 0;
        
        // write enable
        _Hal_Flash_WriteEn(eSpiIdx, eSlvIdx);

        // Config SPI
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        tCfg.eSpi_frf = SPI_FRF_STD;
        tCfg.eDfs_32 = SPI_DFS_08_bit;
        tCfg.eTmod = SPI_TMOD_TX_ONLY;
        Hal_Spi_Master_Config(eSpiIdx, &tCfg);
        
        // disable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
        // Cmd
        u32Temp = FLASH_CMD_SECTOR_ERASE;
        // Addr
        u32Addr = u32SecAddr & ~(0xFFF); // aligned
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        u32Temp = ((u32Addr >> 16) & 0xFF);
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        u32Temp = ((u32Addr >> 8) & 0xFF);
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        u32Temp = (u32Addr & 0xFF);
        Hal_Spi_Data_Send(eSpiIdx, u32Temp);
        // enable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
        
        return _Hal_Spi_Poll_Busy(eSpiIdx);
    }
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_4KSectorAddrErase_Internal
*
* DESCRIPTION:
*   1. Erase a sector (4 KB)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*   2. u32SecAddr : Address of sector (must sector aligned, LSBs truncated)
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_4KSectorAddrErase_Internal_impl(E_SpiIdx_t eSpiIdx, uint32_t u32SecAddr)
{
    return Hal_Flash_4KSectorAddrErase_Internal_Ext(eSpiIdx, SPI_SLAVE_0, u32SecAddr);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_4KSectorIdxErase_Internal
*
* DESCRIPTION:
*   1. Erase a sector (4 KB)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx    : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32SecAddr : Index of sector.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_4KSectorIdxErase_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecIdx)
{
    return Hal_Flash_4KSectorAddrErase_Internal_Ext(eSpiIdx, eSlvIdx, (u32SecIdx * FLASH_SECTOR_SIZE));
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageAddrProgram_Internal
*
* DESCRIPTION:
*   1. Program(write) a page (256 B)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32PageAddr   : Address of page. (must page aligned, LSBs truncated)
*   4. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   5. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageAddrProgram_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    if ( (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return 1;

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

        u32Addr = g_u32FlashBaseAddr + (u32PageAddr & 0xFFFFFF00); // aligned
        memcpy((void *)u32Addr, pu8Data, u32PageSize);
    }
    else // Designware_SPI
    {
        S_SPI_MST_Reg_t *pSpi = NULL;
        if (eSpiIdx == SPI_IDX_1)
            pSpi = SPI1;
        else if (eSpiIdx == SPI_IDX_3)
            pSpi = SPI3;

        uint32_t u32Temp = 0;
        uint8_t u8Cmd = 0;
        uint32_t u32TxCnt = 0;
        uint32_t *pu32Data = (uint32_t *)pu8Data;
        uint32_t u32TxNum = (u32PageSize >> 2);
        uint32_t u32TxFifoLevel = 0;
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        
        // write enable
        _Hal_Flash_WriteEn(eSpiIdx, eSlvIdx);

        // Config SPI
        // disable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
        if (u8UseQuadMode)
        {
            tCfg.eSpi_frf = SPI_FRF_QUAD;
            tCfg.eDfs_32 = SPI_DFS_32_bit;
            tCfg.eTmod = SPI_TMOD_TX_ONLY;
            tCfg.eInst_l = SPI_INST_L_8_bit;
            tCfg.eAddr_l = SPI_ADDR_L_24_bit;
            tCfg.eTrans_type = (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID) ? SPI_INST_std_ADDR_frf : SPI_INST_std_ADDR_std;
            Hal_Spi_Master_Config(eSpiIdx, &tCfg);
            
            // Cmd
            u8Cmd = (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID) ? FLASH_CMD_QIO_PROGRAM_MXIC : FLASH_CMD_QIO_PROGRAM;
            u32Temp = (u8Cmd << 24);
            pSpi->DR[0] = u32Temp;
            // Addr
            u32Temp = ((u32PageAddr & 0xFF0000) >> 8) | 
                      ((u32PageAddr & 0x00FF00) << 8) |  
                      ((u32PageAddr & 0x0000FF) << 24);
            pSpi->DR[0] = u32Temp;
        }
        else
        {
            tCfg.eSpi_frf = SPI_FRF_STD;
            tCfg.eDfs_32 = SPI_DFS_32_bit;
            tCfg.eTmod = SPI_TMOD_TX_ONLY;
            Hal_Spi_Master_Config(eSpiIdx, &tCfg);
            
            // Cmd & Addr
            u8Cmd = FLASH_CMD_SIO_PROGRAM;
            u32Temp = (u8Cmd) | 
                      ((u32PageAddr & 0xFF0000) >> 8) | 
                      ((u32PageAddr & 0x00FF00) << 8) | 
                      ((u32PageAddr & 0x0000FF) << 24);
            pSpi->DR[0] = u32Temp;
        }
        // Data
        // fill in TX_FIFO until TX_FIFO is full
        while ( (pSpi->SR & SPI_MST_SR_TFNF) && (u32TxCnt < u32TxNum) )
        {
            pSpi->DR[0] = *(pu32Data+u32TxCnt);
            u32TxCnt++;
        }
        // enable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
        while (1)
        {
            u32TxFifoLevel = (pSpi->TXFLR & SPI_MST_TXFLR_TXTFL_Msk);
            if ( (u32TxCnt+4 <= u32TxNum) && (u32TxFifoLevel < 28) )
            {
                pSpi->DR[0] = *(pu32Data+u32TxCnt);
                pSpi->DR[1] = *(pu32Data+u32TxCnt + 1);
                pSpi->DR[2] = *(pu32Data+u32TxCnt + 2);
                pSpi->DR[3] = *(pu32Data+u32TxCnt + 3);
                u32TxCnt += 4;
            }
            else if ( (u32TxCnt < u32TxNum) && (u32TxFifoLevel < 31) )
            {
                pSpi->DR[0] = *(pu32Data+u32TxCnt);
                u32TxCnt++;
            }
            
            // tx data count has reached specified number
            if (u32TxCnt >= u32TxNum) break;
        }
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageIdxProgram_Internal
*
* DESCRIPTION:
*   1. Program(write) a page (256 B)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32PageAddr   : Index of page.
*   4. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   5. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageIdxProgram_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageIdx, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    return Hal_Flash_PageAddrProgram_Internal(eSpiIdx, eSlvIdx, (u32PageIdx * FLASH_PAGE_SIZE), u8UseQuadMode, pu8Data);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageAddrRead_Internal
*
* DESCRIPTION:
*   1. Read a page (256 B) 
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32PageAddr   : Address of page.
*   4. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   5. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageAddrRead_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;

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
        uint32_t u32Addr = g_u32FlashBaseAddr + u32PageAddr;
        memcpy(pu8Data, (void *)u32Addr, u32PageSize);
    }
    else // Designware_SPI
    {
        S_SPI_MST_Reg_t *pSpi = NULL;
        if (eSpiIdx == SPI_IDX_1)
            pSpi = SPI1;
        else if (eSpiIdx == SPI_IDX_3)
            pSpi = SPI3;
        
        uint32_t u32Temp = 0;
        uint8_t u8Cmd = 0;
        uint8_t u8Modebit = 0;
        uint32_t *pu32Data = (uint32_t *)pu8Data;
        uint32_t u32RxNum = (u32PageSize >> 2);
        uint32_t u32TxCnt = 0;
        uint32_t u32RxCnt = 0;
        uint32_t u32TimeoutTicks = 0;
        uint32_t u32TickStart = 0;
        uint32_t u32TickDiff = 0;

        u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32FlashPollingStatusTimeoutMs;
    
        uint32_t u32TxFifoLevel = 0;
        
        // polling flash status
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        
        // Config SPI
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        
        if (u8UseQuadMode)
        {
            tCfg.eSpi_frf = SPI_FRF_QUAD;
            tCfg.eDfs_32 = SPI_DFS_32_bit;
            tCfg.eTmod = SPI_TMOD_RX_ONLY;
            tCfg.eInst_l = SPI_INST_L_8_bit;
            tCfg.eAddr_l = SPI_ADDR_L_32_bit; // 24-bit Addr + 8-bit MODE_BITS
            tCfg.eTrans_type = SPI_INST_std_ADDR_frf;
            tCfg.u16Ndf = u32RxNum - 1;
            tCfg.u8WaitCycles = 4;
            Hal_Spi_Master_Config(eSpiIdx, &tCfg);
            
            // Cmd
            u8Cmd = FLASH_CMD_QIO_READ;
            u8Modebit = 0xFF;
            u32Temp = (u8Cmd << 24);
            pSpi->DR[0] = u32Temp;
            // Addr & Mode_bits
            u32Temp = ((u32PageAddr & 0xFF0000) >> 16) | 
                      ((u32PageAddr & 0x00FF00)) | 
                      ((u32PageAddr & 0x0000FF) << 16) | 
                      (u8Modebit << 24);
            pSpi->DR[0] = u32Temp;
            
            // Data
            u32RxCnt = 0;
            while(1)
            {
                u32TickStart = Hal_Tick_Diff(0);
                
                // wait for RX FIFO not empty
                // while (!(pSpi->SR & SPI_MST_SR_RFNE))
                while (((pSpi->RXFLR & SPI_MST_RXFLR_RXTFL_Msk) >> SPI_MST_RXFLR_RXTFL_Pos) < 4)
                {
                    u32TickDiff = Hal_Tick_Diff(u32TickStart);
                    if (u32TickDiff > u32TimeoutTicks) return 1;
                }
                *(pu32Data + u32RxCnt)     = pSpi->DR[0];
                *(pu32Data + u32RxCnt + 1) = pSpi->DR[0];
                *(pu32Data + u32RxCnt + 2) = pSpi->DR[0];
                *(pu32Data + u32RxCnt + 3) = pSpi->DR[0];
                u32RxCnt += 4;
                // rx data count has reached specified number
                if (u32RxCnt == u32RxNum) break;
            }
        }
        else
        {
            tCfg.eSpi_frf = SPI_FRF_STD;
            tCfg.eDfs_32 = SPI_DFS_32_bit;
            tCfg.eTmod = SPI_TMOD_TRX;
            Hal_Spi_Master_Config(eSpiIdx, &tCfg);
            
            // disable slave
            Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
            // Cmd & Addr
            u8Cmd = FLASH_CMD_SIO_READ;
            u32Temp = (u8Cmd) | 
                      ((u32PageAddr & 0xFF0000) >> 8) | 
                      ((u32PageAddr & 0x00FF00) << 8) | 
                      ((u32PageAddr & 0x0000FF) << 24);
            pSpi->DR[0] = u32Temp;
            // Data
            u32TxCnt = 0;
            u32RxCnt = 0;
            u32TxFifoLevel = 0;
            // fill in TX_FIFO until TX_FIFO is full
            while ( (pSpi->SR & SPI_MST_SR_TFNF) && (u32TxCnt < u32RxNum) )
            {
                pSpi->DR[0] = FLASH_DR_DUMMY_VAL;
                u32TxCnt++;
            }
            // enable slave
            Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
            while(1)
            {
                // wait for RX FIFO not empty
                u32TickStart = Hal_Tick_Diff(0);
                while (!(pSpi->SR & SPI_MST_SR_RFNE))
                {
                    u32TickDiff = Hal_Tick_Diff(u32TickStart);
                    if (u32TickDiff > u32TimeoutTicks) return 1;
                }
                if (u32RxCnt == 0)
                    u32Temp = pSpi->DR[0]; // first 4 Bytes are dummy data
                else
                    *(pu32Data+u32RxCnt-1) = pSpi->DR[0];
                
                u32RxCnt++;
                
                u32TxFifoLevel = (pSpi->TXFLR & SPI_MST_TXFLR_TXTFL_Msk);
                if ( (u32TxCnt+4 <= u32RxNum) && (u32TxFifoLevel < 28) )
                {
                    pSpi->DR[0] = FLASH_DR_DUMMY_VAL;
                    pSpi->DR[1] = FLASH_DR_DUMMY_VAL;
                    pSpi->DR[2] = FLASH_DR_DUMMY_VAL;
                    pSpi->DR[3] = FLASH_DR_DUMMY_VAL;
                    u32TxCnt += 4;
                }
                else if ( (u32TxCnt < u32RxNum) && (u32TxFifoLevel < 31) )
                {
                    pSpi->DR[0] = FLASH_DR_DUMMY_VAL;
                    u32TxCnt++;
                }
                
                // rx data count has reached specified number
                if (u32RxCnt == (u32RxNum + 1)) break;
            }
        }
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrProgram_Internal_Ext
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
uint32_t Hal_Flash_AddrProgram_Internal_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    if ( (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return 1;
    
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
        
        uint32_t u32Addr = g_u32FlashBaseAddr + u32StartAddr;
        memcpy((void *)u32Addr, pu8Data, u32Size);
    }
    else // Designware_SPI
    {
        if (u32Size & 0x3) // Size must be multiple of 4 bytes for Designeware_SPI
            return 1;
    
        S_SPI_MST_Reg_t *pSpi = NULL;
        uint32_t u32Temp = 0;
        uint32_t u32DataIdx = 0;
        uint32_t u32DataAddr = 0;
        uint32_t u32DataSize = 0;
        uint32_t u32TxCnt = 0;
        uint32_t u32TxNum = 0;
        uint32_t u32FifoLevel = 0;
        uint32_t *pu32Data = (uint32_t *)pu8Data;
        uint8_t u8Cmd = 0;
        
        if (eSpiIdx == SPI_IDX_1)
            pSpi = SPI1;
        else if (eSpiIdx == SPI_IDX_3)
            pSpi = SPI3;
        
        // 1. handle the data size of the first page if the start address is not aligned
        u32DataAddr = u32StartAddr;
        u32DataSize = FLASH_PAGE_SIZE - (u32DataAddr & 0xFF);
        if (u32DataSize > u32Size)
            u32DataSize = u32Size;
        
        // 2. program the data
        while (u32Size > 0)
        {
            // write enable
            _Hal_Flash_WriteEn(eSpiIdx, eSlvIdx);
            
            // program
            // disable slave
            Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
            if (u8UseQuadMode)
            {
                // Config SPI
                S_SPI_Config_t tCfg;
                memset(&tCfg, 0, sizeof(S_SPI_Config_t));
                tCfg.eSpi_frf = SPI_FRF_QUAD;
                tCfg.eDfs_32 = SPI_DFS_32_bit;
                tCfg.eTmod = SPI_TMOD_TX_ONLY;
                tCfg.eInst_l = SPI_INST_L_8_bit;
                tCfg.eAddr_l = SPI_ADDR_L_24_bit;
                tCfg.eTrans_type = (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID) ? SPI_INST_std_ADDR_frf : SPI_INST_std_ADDR_std;
                Hal_Spi_Master_Config(eSpiIdx, &tCfg);
                
                // Cmd
                u8Cmd = (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID) ? FLASH_CMD_QIO_PROGRAM_MXIC : FLASH_CMD_QIO_PROGRAM;
                u32Temp = (u8Cmd << 24);
                pSpi->DR[0] = u32Temp;
                // Addr
                u32Temp = ((u32DataAddr & 0xFF0000) >> 8) | 
                          ((u32DataAddr & 0x00FF00) << 8) | 
                          ((u32DataAddr & 0x0000FF) << 24);
                pSpi->DR[0] = u32Temp;
            }
            else
            {
                S_SPI_Config_t tCfg;
                memset(&tCfg, 0, sizeof(S_SPI_Config_t));
                tCfg.eSpi_frf = SPI_FRF_STD;
                tCfg.eDfs_32 = SPI_DFS_32_bit;
                tCfg.eTmod = SPI_TMOD_TX_ONLY;
                Hal_Spi_Master_Config(eSpiIdx, &tCfg);
                // Cmd & Addr
                u8Cmd = FLASH_CMD_SIO_PROGRAM;
                u32Temp = (u8Cmd) | 
                          ((u32DataAddr & 0xFF0000) >> 8) | 
                          ((u32DataAddr & 0x00FF00) << 8) | 
                          ((u32DataAddr & 0x0000FF) << 24);
                pSpi->DR[0] = u32Temp;
            }
            // Data
            u32TxCnt = 0;
            u32FifoLevel = 0;
            u32TxNum = (u32DataSize >> 2);
            // fill in TX_FIFO until TX_FIFO is full
            while ( (pSpi->SR & SPI_MST_SR_TFNF) && (u32TxCnt < u32TxNum) )
            {
                pSpi->DR[0] = *(pu32Data+u32TxCnt);
                u32TxCnt++;
            }
            // enable slave
            Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
            while (1)
            {
                u32FifoLevel = (pSpi->TXFLR & SPI_MST_TXFLR_TXTFL_Msk);
                if ( (u32TxCnt+4 <= u32TxNum) && (u32FifoLevel < 28) )
                {
                    pSpi->DR[0] = *(pu32Data+u32TxCnt);
                    pSpi->DR[1] = *(pu32Data+u32TxCnt + 1);
                    pSpi->DR[2] = *(pu32Data+u32TxCnt + 2);
                    pSpi->DR[3] = *(pu32Data+u32TxCnt + 3);
                    u32TxCnt += 4;
                }
                else if ( (u32TxCnt < u32TxNum) && (u32FifoLevel < 31) )
                {
                    pSpi->DR[0] = *(pu32Data+u32TxCnt);
                    u32TxCnt++;
                }
                
                // tx data count has reached specified number
                if (u32TxCnt >= u32TxNum) break;
            }
            
            // 3. update the next size and address
            u32DataIdx += u32DataSize;
            u32DataAddr += u32DataSize;
            u32Size -= u32DataSize;
            u32DataSize = (u32Size > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : u32Size;
        }
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrProgram_Internal
*
* DESCRIPTION:
*   1. Program(write) n bytes from the start address
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. u32StartAddr  : Start address
*   3. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   4. u32Size       : Data size
*   5. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_AddrProgram_Internal_impl(E_SpiIdx_t eSpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    return Hal_Flash_AddrProgram_Internal_Ext(eSpiIdx, SPI_SLAVE_0, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrRead_Internal_Ext
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
uint32_t Hal_Flash_AddrRead_Internal_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
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
        
        uint32_t u32Addr = g_u32FlashBaseAddr + u32StartAddr;
        memcpy(pu8Data, (void *)u32Addr, u32Size);
    }
    else // Designware_SPI
    {
        S_SPI_MST_Reg_t *pSpi = NULL;
        
        uint32_t u32Temp = 0;
        uint8_t u8Cmd = 0;
        uint8_t u8Modebit = 0;
        uint32_t *pu32Data = (uint32_t *)pu8Data;
        uint32_t u32RxNum = (u32Size >> 2);
        uint32_t u32TxCnt = 0;
        uint32_t u32RxCnt = 0;
        uint32_t u32TimeoutTicks = 0;
        uint32_t u32TickStart = 0;
        uint32_t u32TickDiff = 0;
        uint32_t u32TxFifoLevel = 0;
        
        u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32FlashPollingStatusTimeoutMs;

        if (eSpiIdx == SPI_IDX_1)
            pSpi = SPI1;
        else if (eSpiIdx == SPI_IDX_3)
            pSpi = SPI3;

        // polling flash status
        _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx);
        
        // Config SPI
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        
        // read cmd
        if (u8UseQuadMode)
        {
            uint32_t u32RxCnt = 0;
            
            tCfg.eSpi_frf = SPI_FRF_QUAD;
            tCfg.eDfs_32 = SPI_DFS_32_bit;
            tCfg.eTmod = SPI_TMOD_RX_ONLY;
            tCfg.eInst_l = SPI_INST_L_8_bit;
            tCfg.eAddr_l = SPI_ADDR_L_32_bit; // 24-bit Addr + 8-bit MODE_BITS
            tCfg.eTrans_type = SPI_INST_std_ADDR_frf;
            tCfg.u16Ndf = u32RxNum - 1;
            tCfg.u8WaitCycles = 4;
            Hal_Spi_Master_Config(eSpiIdx, &tCfg);
            
            // Cmd
            u8Cmd = FLASH_CMD_QIO_READ;
            u8Modebit = 0xFF;
            u32Temp = (u8Cmd << 24);
            pSpi->DR[0] = u32Temp;
            // Addr
            u32Temp = ((u32StartAddr & 0xFF0000) >> 16) | 
                      ((u32StartAddr & 0x00FF00)) | 
                      ((u32StartAddr & 0x0000FF) << 16) | 
                      (u8Modebit << 24);
            pSpi->DR[0] = u32Temp;
            // Data
            while (1)
            {
                u32TickStart = Hal_Tick_Diff(0);

                if (u32RxNum - u32RxCnt >= 4)
                {
                    // wait for RX FIFO not empty
                    while (((pSpi->RXFLR & SPI_MST_RXFLR_RXTFL_Msk) >> SPI_MST_RXFLR_RXTFL_Pos) < 4)
                    {
                        u32TickDiff = Hal_Tick_Diff(u32TickStart);
                        if (u32TickDiff > u32TimeoutTicks) return 1;
                    }
                    *(pu32Data + u32RxCnt)     = pSpi->DR[0];
                    *(pu32Data + u32RxCnt + 1) = pSpi->DR[0];
                    *(pu32Data + u32RxCnt + 2) = pSpi->DR[0];
                    *(pu32Data + u32RxCnt + 3) = pSpi->DR[0];
                    u32RxCnt += 4;
                }
                else 
                {
                    // wait for RX FIFO not empty
                    while (!(pSpi->SR & SPI_MST_SR_RFNE))
                    {
                        u32TickDiff = Hal_Tick_Diff(u32TickStart);
                        if (u32TickDiff > u32TimeoutTicks) return 1;
                    }
                    *(pu32Data + u32RxCnt) = pSpi->DR[0];
                    u32RxCnt++;
                }
                
                // rx data count has reached specified number
                if (u32RxCnt == u32RxNum) break;
            }
        }
        else
        {
            tCfg.eSpi_frf = SPI_FRF_STD;
            tCfg.eDfs_32 = SPI_DFS_32_bit;
            tCfg.eTmod = SPI_TMOD_TRX;
            Hal_Spi_Master_Config(eSpiIdx, &tCfg);
            
            // disable slave
            Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 0);
            // Cmd & Addr
            u8Cmd = FLASH_CMD_SIO_READ;
            u32Temp = (u8Cmd) | 
                      ((u32StartAddr & 0xFF0000) >> 8) | 
                      ((u32StartAddr & 0x00FF00) << 8) | 
                      ((u32StartAddr & 0x0000FF) << 24);
            pSpi->DR[0] = u32Temp;
            // Data
            u32TxCnt = 0;
            u32RxCnt = 0;
            u32TxFifoLevel = 0;
            // fill in TX_FIFO until TX_FIFO is full
            while ( (pSpi->SR & SPI_MST_SR_TFNF) && (u32TxCnt < u32RxNum) )
            {
                pSpi->DR[0] = FLASH_DR_DUMMY_VAL;
                u32TxCnt++;
            }
            // enable slave
            Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
            while(1)
            {
                // wait for RX FIFO not empty
                u32TickStart = Hal_Tick_Diff(0);
                while (!(pSpi->SR & SPI_MST_SR_RFNE))
                {
                    u32TickDiff = Hal_Tick_Diff(u32TickStart);
                    if (u32TickDiff > u32TimeoutTicks) return 1;
                }
                if (u32RxCnt == 0)
                    u32Temp = pSpi->DR[0]; // first 4 bytes are dummy data
                else
                    *(pu32Data+u32RxCnt-1) = pSpi->DR[0];
                
                u32RxCnt++;
                
                u32TxFifoLevel = (pSpi->TXFLR & SPI_MST_TXFLR_TXTFL_Msk);
                if ( (u32TxCnt+4 <= u32RxNum) && (u32TxFifoLevel < 28) )
                {
                    pSpi->DR[0] = FLASH_DR_DUMMY_VAL;
                    pSpi->DR[1] = FLASH_DR_DUMMY_VAL;
                    pSpi->DR[2] = FLASH_DR_DUMMY_VAL;
                    pSpi->DR[3] = FLASH_DR_DUMMY_VAL;
                    u32TxCnt += 4;
                }
                else if ( (u32TxCnt < u32RxNum) && (u32TxFifoLevel < 31) )
                {
                    pSpi->DR[0] = FLASH_DR_DUMMY_VAL;
                    u32TxCnt++;
                }
                
                // rx data count has reached specified number
                if (u32RxCnt == (u32RxNum + 1)) break;
            }
        }
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrRead_Internal
*
* DESCRIPTION:
*   1. Read n bytes from the start address
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. u32StartAddr  : Start address
*   3. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   4. u32Size       : Data size
*   5. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_AddrRead_Internal_impl(E_SpiIdx_t eSpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    return Hal_Flash_AddrRead_Internal_Ext(eSpiIdx, SPI_SLAVE_0, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Reset
*
* DESCRIPTION:
*   1. Flash reset
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t
*
* RETURNS
*   None
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Flash_Reset_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    if ( (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return;
    
    _Hal_Flash_CmdSend(eSpiIdx, eSlvIdx, FLASH_CMD_RESET_ENABLE);
    _Hal_Flash_CmdSend(eSpiIdx, eSlvIdx, FLASH_CMD_RESET);
}



/*************************************************************************
* FUNCTION:
*  Hal_Flash_Init
*
* DESCRIPTION:
*   1. Init flash status
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
uint32_t Hal_Flash_Init_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    

    if (0 != Hal_Flash_Init_Internal(eSpiIdx, eSlvIdx))
        return 1;
        
    // check for cold boot only
    if (0 != Boot_CheckWarmBoot())
        return 0;    // the init is done in the cold boot

    // create the semaphore
    return Hal_Flash_InitSemaphore(eSpiIdx);
}
/*************************************************************************
* FUNCTION:
*  Hal_Flash_InitSemaphore
*
* DESCRIPTION:
*   1. Create semaphore for SPI
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   0: setting complete
*   1: error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_InitSemaphore(E_SpiIdx_t eSpiIdx)
{
    osSemaphoreDef_t tSemaphoreDef;

    tSemaphoreDef.dummy = 0;    // reserved, it is no used
    g_taHalFlashSemaphoreId[eSpiIdx] = osSemaphoreCreate(&tSemaphoreDef, 1);
    if (g_taHalFlashSemaphoreId[eSpiIdx] == NULL)
    {
        printf("To create the semaphore for Hal_Flash is fail.\n");
        return 1;
    }

    return 0;
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
uint32_t Hal_Flash_QuadModeEn_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8QModeEn)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    uint8_t u8Sts_1 = 0;
    uint8_t u8Sts_2 = 0;
    uint8_t u8IsQModeEn = 0;
    
    // read Status Register first
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID)
    {
        _Hal_Flash_Status1Get(eSpiIdx, eSlvIdx, (uint32_t *)&u8Sts_1);
        u8IsQModeEn = (u8Sts_1 & FLASH_STATUS_QE_MXIC) ? 1 : 0;
    }
    else
    {
        _Hal_Flash_Status2Get(eSpiIdx, eSlvIdx, (uint32_t *)&u8Sts_2);
        u8IsQModeEn = (u8Sts_2 & FLASH_STATUS_QE) ? 1 : 0;
    }
    
    // write Status Register if current QE bit is not set
    if (u8QModeEn && !u8IsQModeEn)
    {
        u8Sts_1 = (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID) ? FLASH_STATUS_QE_MXIC : 0x00;
        u8Sts_2 = (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == MACRONIX_ID) ? 0x00 : FLASH_STATUS_QE;
        _Hal_Flash_StatusSet(eSpiIdx, eSlvIdx, u8Sts_1, u8Sts_2);
        
        return _Hal_Flash_WriteDoneCheck(eSpiIdx, eSlvIdx); // poll busy
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Opcode_Config
*
* DESCRIPTION:
*   1. to setup flash read/write opcode for QSPI module (only for QSPI)
*
* CALLS
*
* PARAMETERS
*   1. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t.
*   2. u8UseQuadMode : indicate whether use Quad bit mode or not
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Flash_Opcode_Config_impl(E_SpiSlave_t eSlvIdx, uint8_t u8UseQuadMode)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return;
    
    S_QSPI_Instruction_t tInstr;
    memset(&tInstr, 0, sizeof(S_QSPI_Instruction_t));
    
    // configure QE bit of flash
    Hal_Flash_QuadModeEn(SPI_IDX_0, eSlvIdx, u8UseQuadMode);
    
    // disable QSPI
    Hal_Qspi_Enable(0);
    
    if (u8UseQuadMode)
    {
        tInstr.RdOpCode = FLASH_CMD_QIO_READ;
        tInstr.ModeBitEn = 1;
        tInstr.RdDummy = 4;
        tInstr.eRdAddrXferType = QSPI_QIO_Mode;
        tInstr.eRdDataXferType = QSPI_QIO_Mode;
        tInstr.WrOpCode = (g_u8aHalFlashID[SPI_IDX_0][eSlvIdx] == MACRONIX_ID) ? FLASH_CMD_QIO_PROGRAM_MXIC : FLASH_CMD_QIO_PROGRAM;
        tInstr.WrDummy = 0;
        tInstr.eWrAddrXferType = (g_u8aHalFlashID[SPI_IDX_0][eSlvIdx] == MACRONIX_ID) ? QSPI_QIO_Mode : QSPI_SIO_Mode;
        tInstr.eWrDataXferType = QSPI_QIO_Mode;
        tInstr.ModeBit = 0xFF;
    }
    else
    {
        tInstr.RdOpCode = FLASH_CMD_SIO_READ;
        tInstr.eRdAddrXferType = QSPI_SIO_Mode;
        tInstr.eRdDataXferType = QSPI_SIO_Mode;
        tInstr.WrOpCode = FLASH_CMD_SIO_PROGRAM;
        tInstr.eWrAddrXferType = QSPI_SIO_Mode;
        tInstr.eWrDataXferType = QSPI_SIO_Mode;
    }
    tInstr.PollOpCode = FLASH_CMD_READ_STATUS_REG;
    tInstr.PollCount = 0x01;
    Hal_Qspi_Instruction_Setup(&tInstr);
    
    // enable QSPI
    Hal_Qspi_Enable(1);
}

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
uint32_t Hal_Flash_4KSectorAddrErase_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr)
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

/*************************************************************************
* FUNCTION:
*  Hal_Flash_4KSectorAddrErase
*
* DESCRIPTION:
*   1. Erase a sector (4 KB)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refer to E_SpiIdx_t
*   2. u32SecAddr : Address of sector (must sector aligned, LSBs truncated)
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_4KSectorAddrErase_impl(E_SpiIdx_t eSpiIdx, uint32_t u32SecAddr)
{
    return Hal_Flash_4KSectorAddrErase_Ext(eSpiIdx, SPI_SLAVE_0, u32SecAddr);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_4KSectorIdxErase
*
* DESCRIPTION:
*   1. Erase a sector (4 KB)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx    : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx    : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32SecAddr : Index of sector.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_4KSectorIdxErase_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecIdx)
{
    return Hal_Flash_4KSectorAddrErase_Ext(eSpiIdx, eSlvIdx, (u32SecIdx * FLASH_SECTOR_SIZE));
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageAddrProgram
*
* DESCRIPTION:
*   1. Program(write) a page (256 B)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32PageAddr   : Address of page. (must page aligned, LSBs truncated)
*   4. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   5. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageAddrProgram_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    uint8_t ubRet = 1;
        
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return ubRet;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return ubRet;
    
    if (g_u8aHalFlashID[eSpiIdx] == NO_FLASH)
        return ubRet;
    
    if (pu8Data == NULL)
        return ubRet;
    
    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[eSpiIdx], osWaitForever);

    if (0 != Hal_Flash_PageAddrProgram_Internal(eSpiIdx, eSlvIdx, u32PageAddr, u8UseQuadMode, pu8Data))
    {
        goto done;
    }

    ubRet = 0;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[eSpiIdx]);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageIdxProgram
*
* DESCRIPTION:
*   1. Program(write) a page (256 B)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32PageAddr   : Index of page.
*   4. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   5. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageIdxProgram_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageIdx, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    return Hal_Flash_PageAddrProgram(eSpiIdx, eSlvIdx, (u32PageIdx * FLASH_PAGE_SIZE), u8UseQuadMode, pu8Data);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_PageAddrRead
*
* DESCRIPTION:
*   1. Read a page (256 B) 
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32PageAddr   : Address of page.
*   4. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   5. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_PageAddrRead_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data)
{
    uint8_t ubRet = 1;
    
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return ubRet;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return ubRet;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return ubRet;
    
    if (pu8Data == NULL)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[eSpiIdx], osWaitForever);

    if (0 != Hal_Flash_PageAddrRead_Internal(eSpiIdx, eSlvIdx, u32PageAddr, u8UseQuadMode, pu8Data))
    {
        goto done;
    }

    ubRet = 0;

done:
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
uint32_t Hal_Flash_AddrProgram_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    uint8_t ubRet = 1;
    
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return ubRet;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return ubRet;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return ubRet;
    
    if (u32Size == 0)
        return ubRet;
    
    if (pu8Data == NULL)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[eSpiIdx], osWaitForever);

    if (0 != Hal_Flash_AddrProgram_Internal_Ext(eSpiIdx, eSlvIdx, u32StartAddr, u8UseQuadMode, u32Size, pu8Data))
    {
        goto done;
    }

    ubRet = 0;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[eSpiIdx]);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrProgram
*
* DESCRIPTION:
*   1. Program(write) n bytes from the start address
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. u32StartAddr  : Start address
*   3. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   4. u32Size       : Data size
*   5. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_AddrProgram_impl(E_SpiIdx_t eSpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    return Hal_Flash_AddrProgram_Ext(eSpiIdx, SPI_SLAVE_0, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
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
uint32_t Hal_Flash_AddrRead_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    uint8_t ubRet = 1;
        
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return ubRet;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return ubRet;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return ubRet;
    
    if (u32Size == 0)
        return ubRet;
    
    if (pu8Data == NULL)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[eSpiIdx], osWaitForever);

    if (0 != Hal_Flash_AddrRead_Internal_Ext(eSpiIdx, eSlvIdx, u32StartAddr, u8UseQuadMode, u32Size, pu8Data))
    {
        goto done;
    }

    ubRet = 0;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[eSpiIdx]);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_AddrRead
*
* DESCRIPTION:
*   1. Read n bytes from the start address
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refer to E_SpiIdx_t
*   2. u32StartAddr  : Start address
*   3. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
*   4. u32Size       : Data size
*   5. pu8Data       : Data buffer
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_AddrRead_impl(E_SpiIdx_t eSpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    return Hal_Flash_AddrRead_Ext(eSpiIdx, SPI_SLAVE_0, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Reset
*
* DESCRIPTION:
*   1. Flash reset
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t
*
* RETURNS
*   None
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Flash_Reset_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        return;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return;

    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[eSpiIdx], osWaitForever);

    Hal_Flash_Reset_Internal(eSpiIdx, eSlvIdx);

    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[eSpiIdx]);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Check_Ext
*
* DESCRIPTION:
*   1. Check flash is OK or not
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t
*
* RETURNS
*   refert to E_FlashCheckStatus_t
* 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
E_FlashCheckStatus_t Hal_Flash_Check_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    if ((eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX))
        return NOT_SUPPORTED;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return NOT_SUPPORTED;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH) 
        return NOT_SUPPORTED;
    else 
        return SUPPORTED_FLASH;
}


/*************************************************************************
* FUNCTION:
*  Hal_Flash_Check
*
* DESCRIPTION:
*   1. Check flash is OK or not
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refert to E_SpiIdx_t
*
* RETURNS
*   refert to E_FlashCheckStatus_t
* 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
E_FlashCheckStatus_t Hal_Flash_Check_impl(E_SpiIdx_t eSpiIdx)
{
    return Hal_Flash_Check_Ext(eSpiIdx, SPI_SLAVE_0);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Dma_Access
*
* DESCRIPTION:
*   1. Read/Write flash data by DMA (with QSPI module)
*
* CALLS
*
* PARAMETERS
*   1. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   2. eChannel      : DMA channel.
*   3. u32SrcAddr    : Source address.
*   4. u32DstAddr    : Destination address.
*   5. u32Size       : Size of DMA transfer. (Size not over page size is recommended for programming flash, bus would be occupied by DMA while polling flash status)
* 
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_Dma_Access_impl(E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size)
{
    return Hal_Qspi_Dma_Access(eSlvIdx, eChannel, u32SrcAddr, u32DstAddr, u32Size);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Dma_Read
*
* DESCRIPTION:
*   1. Read flash data by DMA (Only avaiable for SPI_1 & SPI_3)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. eTxChannel    : DMA channel for TX dummy data.
*   4. eRxChannel    : DMA channel for RX flash data.
*   5. u32SrcAddr    : Source address of flash space.
*   6. u32DstAddr    : Destination address of RAM space.
*   7. u32Size       : Size of DMA transfer. Should less than 0xFFFF. Must be multiple of 4 bytes.
*   8. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
* 
* RETURNS
*   refert to E_FlashCheckStatus_t
* 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_Dma_Read_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eTxChannel, E_DMA_CHANNEL eRxChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size, uint8_t u8UseQuadMode)
{
    if ( (eSpiIdx == SPI_IDX_0) || (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (eTxChannel >= DMA_Channel_Max)
        return 1;
    
    if (eRxChannel >= DMA_Channel_Max)
        return 1;
    
    // u32DstAddr must be memory address space
    if ( !(IS_MEM_ADDR_SPACE(u32DstAddr)) )
        return 1;

    if (u32Size == 0)
        return 1;
    
    if (u32Size > 0xFFFF) // CTRLR1.NDF is only 16-bit
        return 1;

    if (u32Size & 0x3) // Size must be multiple of 4 bytes
        return 1;

    if (u8UseQuadMode)
        return _Hal_Flash_Dma_Read_QIO(eSpiIdx, eSlvIdx, eRxChannel, u32SrcAddr, u32DstAddr, u32Size);
    else
        return _Hal_Flash_Dma_Read_SIO(eSpiIdx, eSlvIdx, eTxChannel, eRxChannel, u32SrcAddr, u32DstAddr, u32Size);
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_Dma_Write
*
* DESCRIPTION:
*   1. Program flash by DMA (Only avaiable for SPI_1 & SPI_3)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. eTxChannel    : DMA channel for TX data to SPI TX_FIFO.
*   4. u32SrcAddr    : Source address of RAM space.
*   5. u32DstAddr    : Destination address of flash space.
*   6. u32Size       : Size of DMA transfer. Must be multiple of 4 bytes.
*   7. u8UseQuadMode : Qaud-mode select. 1 for enable/0 for disable
* 
* RETURNS
*   refert to E_FlashCheckStatus_t
* 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_Dma_Write_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size, uint8_t u8UseQuadMode)
{
    if ( (eSpiIdx == SPI_IDX_0) || (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (eChannel >= DMA_Channel_Max)
        return 1;
    
    // u32SrcAddr must be memory address space
    if ( !(IS_MEM_ADDR_SPACE(u32SrcAddr)) )
        return 1;
    
    if (u32Size == 0)
        return 1;

    if (u32Size & 0x3) // Size must be multiple of 4 bytes
        return 1;
    
    // 1. handle the data size of the first page if the start address is not aligned
    uint32_t u32RamAddr = u32SrcAddr;
    uint32_t u32FlashAddr = u32DstAddr;
    uint32_t u32DataSize = FLASH_PAGE_SIZE - (u32FlashAddr & 0xFF);
    if (u32DataSize > u32Size)
        u32DataSize = u32Size;
    
    // 2. program the data
    while (u32Size > 0)
    {
        // write enable
        _Hal_Flash_WriteEn(eSpiIdx, eSlvIdx);
        
        if (u8UseQuadMode)
        {
            if (_Hal_Flash_Dma_Write_QIO(eSpiIdx, eSlvIdx, eChannel, u32RamAddr, u32FlashAddr, u32DataSize))
                return 1;
        }
        else
        {
            if (_Hal_Flash_Dma_Write_SIO(eSpiIdx, eSlvIdx, eChannel, u32RamAddr, u32FlashAddr, u32DataSize))
                return 1;
        }
        
        // 3. update the next size and address
        u32SrcAddr += u32DataSize;
        u32FlashAddr += u32DataSize;
        u32Size -= u32DataSize;
        u32DataSize = (u32Size > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : u32Size;
    }
    
    return 0;
}


/*************************************************************************
* FUNCTION:
*  Hal_Flash_EnterPowerDown
*
* DESCRIPTION:
*   1. Force flash enter power-down mode
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
* 
* RETURNS
*   0 - Success
*   1 - Fail
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_EnterPowerDown_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{   
    if ( (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (g_u8aHalFlashID[eSpiIdx][eSlvIdx] == NO_FLASH)
        return 1;
    
    // wait the semaphore
    osSemaphoreWait(g_taHalFlashSemaphoreId[eSpiIdx], osWaitForever);
    _Hal_Flash_CmdSend(eSpiIdx, eSlvIdx, FLASH_CMD_ENTER_POWER_DOWN);

    // release the semaphore
    osSemaphoreRelease(g_taHalFlashSemaphoreId[eSpiIdx]);
    
    return 0;
}


/*************************************************************************
* FUNCTION:
*  Hal_Flash_ReleasePowerDown
*
* DESCRIPTION:
*   1. Release flash from power-down mode
*   2. This function will not accquire semaphore or check flash ID, please use this function carefully.
*      
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32WaitResume : Wait flash resume timeout. 0 - Not wait, 1 - Wait t_RES
* 
* RETURNS
*   0 - Success
*   1 - Fail
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_ReleasePowerDown_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32WaitResume)
{
    uint32_t u32DelayTicks, u32TickStart, u32TickDiff;
    
    if ( (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        return 1;
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    _Hal_Flash_CmdSend(eSpiIdx, eSlvIdx, FLASH_CMD_RELEASE_PWD_RDID);
    if (!u32WaitResume)
        return 0;
    u32TickStart = Hal_Tick_Diff(0);
    u32DelayTicks = Hal_Flash_GetResumeTimeTicks(eSpiIdx, eSlvIdx);
    do {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
    } while (u32TickDiff < u32DelayTicks);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Flash_GetResumeTimeTicks
*
* DESCRIPTION:
*   1. Get flash resume delay ticks
*      
* CALLS
*
* PARAMETERS
*   1. eSpiIdx       : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
* 
* RETURNS
*   0 - Success
*   1 - Fail
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Flash_GetResumeTimeTicks_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    uint32_t u32TRstUs = 0;
    uint32_t u32Ticks;
    
    if ( (eSpiIdx == SPI_IDX_2) || (eSpiIdx >= SPI_IDX_MAX) )
        goto done;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        goto done;

    if (g_u32aFlashWaitResumeUs[eSpiIdx][eSlvIdx])
    {   /* Specific wait resume time */
        u32TRstUs = g_u32aFlashWaitResumeUs[eSpiIdx][eSlvIdx];
    }
    else
    {
        u32TRstUs = FLASH_T_RESUME_US_DEFAULT;
    }
done:
    
    u32Ticks = u32TRstUs * ((Hal_Tick_PerMilliSec() + 1000) / 1000);
    return u32Ticks;
}
