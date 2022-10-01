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
*  hal_psram.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This source file defines the functions of psram.
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "string.h"
#include "hal_psram.h"
#include "hal_tick.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
uint8_t g_u8aHalPsramID[SPI_SLAVE_MAX] = {NO_PSRAM, NO_PSRAM, NO_PSRAM, NO_PSRAM};
uint8_t g_u8FlashPsramCoexistEn = 0;

uint32_t g_u32PsramBaseAddr = APS_XIP_MEM_BASE;
uint32_t g_u32PsramTimeoutMs = PSRAM_TIMEOUT_MS;

// Sec 5: declaration of global function prototype
T_Hal_Psram_Init                Hal_Psram_Init              = Hal_Psram_Init_impl;
T_Hal_Psram_Reset               Hal_Psram_Reset             = Hal_Psram_Reset_impl;
T_Hal_Psram_Opcode_Config       Hal_Psram_Opcode_Config     = Hal_Psram_Opcode_Config_impl;
T_Hal_Psram_DeviceId            Hal_Psram_DeviceId          = Hal_Psram_DeviceId_impl;
T_Hal_Psram_RxSampleDelay_Set   Hal_Psram_RxSampleDelay_Set = Hal_Psram_RxSampleDelay_Set_impl;
T_Hal_Psram_Dma_Access          Hal_Psram_Dma_Access        = Hal_Psram_Dma_Access_impl;

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
*  Hal_Psram_Init
*
* DESCRIPTION:
*   1. Init Psram
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refert to E_SpiIdx_t
*   2. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t
*   3. u8UseQuadMode : indicate whether use Quad bit mode or not
*   4. u8Standalone : 0 = Flash & Psram Coexistence case
*                     1 = Psram only case
*
* RETURNS
*   0: setting complete
*   1: error  (No Psram)
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Psram_Init_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8UseQuadMode, uint8_t u8Standalone)
{
    uint32_t u32Mfid = 0, u32Kgd = 0;
    
    // Only support for SPI_0 now
    if (eSpiIdx != SPI_IDX_0)
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;

    // Setup Flash&Psram Coexistence flag
    if (u8Standalone)
        g_u8FlashPsramCoexistEn = 0;
    else
        g_u8FlashPsramCoexistEn = 1;
    
    // Config Opcode
    Hal_Psram_Opcode_Config(u8UseQuadMode);

    if (!u8Standalone)
    {
        Hal_Qspi_Psram_Coexistence_Setup(1, eSlvIdx);
        g_u32PsramBaseAddr = APS_XIP_MEM_BASE + 0x00800000; // FIXME
    }
        
    // Send RESET command
    Hal_Psram_Reset(eSpiIdx, eSlvIdx);

    // Setup RX SAMPLE DELAY
    if (Hal_Psram_RxSampleDelay_Set(eSlvIdx))
        return 1;

    // Read Psram MF ID & KGD
    Hal_Psram_DeviceId(eSpiIdx, eSlvIdx, &u32Mfid, &u32Kgd);

    // MF ID & KGD Check
    if ( (u32Mfid != MF_ID) || (u32Kgd != KNOWN_GOOD_DIE) )
    {
        g_u8aHalPsramID[eSlvIdx] = NO_PSRAM;
        return 1;
    }
    else
    {
        // Supported 
        g_u8aHalPsramID[eSlvIdx] = u32Mfid;
    }

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Psram_Reset
*
* DESCRIPTION:
*   1. Psram reset
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
uint32_t Hal_Psram_Reset_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx)
{
    if (eSpiIdx != SPI_IDX_0)
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    // Check peripheral chip select
    Hal_Qspi_Peri_Select(eSlvIdx);
    
    S_QSPI_Command_t tCmd;
    memset(&tCmd, 0, sizeof(S_QSPI_Command_t));
    
    // Enable Reset
    tCmd.Opcode = 0x66;
    Hal_Qspi_Command_Exec(&tCmd);
    
    // Reset
    tCmd.Opcode = 0x99;
    Hal_Qspi_Command_Exec(&tCmd);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Psram_Opcode_Config (for PSRAM only case)
*
* DESCRIPTION:
*   1. to setup PSRAM read/write opcode for QSPI module (only for QSPI)
*
* CALLS
*
* PARAMETERS
*   1. u8UseQuadMode : indicate whether use Quad bit mode or not
*
* RETURNS
*   0: setting complete
*   1: error
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Psram_Opcode_Config_impl(uint8_t u8UseQuadMode)
{
    if (g_u8FlashPsramCoexistEn)
    {
        S_QSPI_Psram_Instruction_t tPsramInstr;
        memset(&tPsramInstr, 0, sizeof(S_QSPI_Psram_Instruction_t));
        
        if (u8UseQuadMode)
        {
            tPsramInstr.PsramRdOpCode = 0xEB;
            tPsramInstr.PsramRdDummy = 6;
            tPsramInstr.ePsramRdAddrXferType = QSPI_QIO_Mode;
            tPsramInstr.ePsramRdDataXferType = QSPI_QIO_Mode;
            tPsramInstr.PsramWrOpCode = 0x38;
            tPsramInstr.ePsramWrAddrXferType = QSPI_QIO_Mode;
            tPsramInstr.ePsramWrDataXferType = QSPI_QIO_Mode;
            tPsramInstr.PsramWEL_Dis = 1;
        }
        else
        {
          #if 1 // READ
            tPsramInstr.PsramRdOpCode = 0x03;
          #else // FAST READ
            tPsramInstr.PsramRdOpCode = 0x0B;
            tPsramInstr.PsramRdDummy = 8;
          #endif
            tPsramInstr.ePsramRdAddrXferType = QSPI_SIO_Mode;
            tPsramInstr.ePsramRdDataXferType = QSPI_SIO_Mode;
            tPsramInstr.PsramWrOpCode = 0x02;
            tPsramInstr.ePsramWrAddrXferType = QSPI_SIO_Mode;
            tPsramInstr.ePsramWrDataXferType = QSPI_SIO_Mode;
            tPsramInstr.PsramWEL_Dis = 1;
        }
        Hal_Qspi_Psram_Instruction_Setup(&tPsramInstr);
    }
    else
    {
        S_QSPI_Instruction_t tInstr;
        memset(&tInstr, 0, sizeof(S_QSPI_Instruction_t));
        
        if (u8UseQuadMode)
        {
            tInstr.RdOpCode = 0xEB;
            tInstr.RdDummy = 6;
            tInstr.eRdAddrXferType = QSPI_QIO_Mode;
            tInstr.eRdDataXferType = QSPI_QIO_Mode;
            tInstr.WrOpCode = 0x38;
            tInstr.eWrAddrXferType = QSPI_QIO_Mode;
            tInstr.eWrDataXferType = QSPI_QIO_Mode;
            tInstr.WEL_Dis = 1;
        }
        else
        {
          #if 1 // READ
            tInstr.RdOpCode = 0x03;
          #else // FAST READ
            tInstr.RdOpCode = 0x0B;
            tInstr.RdDummy = 8;
          #endif
            tInstr.eRdAddrXferType = QSPI_SIO_Mode;
            tInstr.eRdDataXferType = QSPI_SIO_Mode;
            tInstr.WrOpCode = 0x02;
            tInstr.eWrAddrXferType = QSPI_SIO_Mode;
            tInstr.eWrDataXferType = QSPI_SIO_Mode;
            tInstr.WEL_Dis = 1;
        }
        tInstr.Poll_Dis = 1; // DISABLE auto polling
        Hal_Qspi_Instruction_Setup(&tInstr);
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Psram_DeviceId
*
* DESCRIPTION:
*   1. Get Psram MF ID and Known Good Die
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx  : Index of SPI slave. refer to E_SpiSlave_t
*   3. pu32Mfid : Pointer to receive MF ID
*   4. pu32Kgd  : Pointer to receive Known Good Die
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Psram_DeviceId_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t *pu32Mfid, uint32_t *pu32Kgd)
{
    if (eSpiIdx != SPI_IDX_0)
        return 1;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (pu32Mfid == NULL)
        return 1;
    
    if (pu32Kgd == NULL)
        return 1;
    
    // Check peripheral chip select
    Hal_Qspi_Peri_Select(eSlvIdx);
    
    S_QSPI_Command_t tCmd;
    memset(&tCmd, 0, sizeof(S_QSPI_Command_t));
    tCmd.Opcode = 0x9F;
    tCmd.CmdAddrEn = 1;
    tCmd.eAddrByteNum = QSPI_Addr_3_Byte; // Addr is 24 bit, and value is don't care
    tCmd.RdDataEn = 1;
    tCmd.eRdDataByteNum = QSPI_Data_8_Byte;
    Hal_Qspi_Command_Exec(&tCmd);
    
    *pu32Mfid = (tCmd.RdDataLow & 0x000000FF);
    *pu32Kgd  = ((tCmd.RdDataLow & 0x0000FF00) >> 8);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Psram_RxSampleDelay_Set
*
* DESCRIPTION:
*   1. setup Rx Sample Dealy by write/read 0x007F_FF00 of PSRAM
*
* CALLS
*
* PARAMETERS
*   1. eSlvIdx  : Index of SPI slave. refer to E_SpiSlave_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint8_t Hal_Psram_RxSampleDelay_Set_impl(E_SpiSlave_t eSlvIdx)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    uint32_t u32Tmp = 0;
    uint32_t u32Addr = g_u32PsramBaseAddr + 0x007FFF00;
    uint32_t u32Size = 0x100;
    uint16_t u16DlyNum = 0;
    uint16_t u16MaxDlyNum = 0xF;
    uint8_t  u8IsErr;
    
    // Check peripheral chip select
    Hal_Qspi_Peri_Select(eSlvIdx);
    
    while (1)
    {
        if (g_u8FlashPsramCoexistEn)
            Hal_Qspi_Psram_RxSampleDly_Set((uint8_t) u16DlyNum);
        else    
            Hal_Qspi_RxSampleDly_Set((uint8_t) u16DlyNum);
        
        for (u32Tmp = u32Addr; u32Tmp < (u32Addr + u32Size); u32Tmp += 4)
        {
            MEM_WRITE_DW(u32Tmp, u32Tmp);
        }
        
        u8IsErr = 0;
        for (u32Tmp = u32Addr; u32Tmp < (u32Addr + u32Size); u32Tmp += 4)
        {
            if (!u8IsErr && (u32Tmp != MEM_READ_DW(u32Tmp)))
            {
                u8IsErr = 1;
            }
            
            MEM_WRITE_DW(u32Tmp, 0); // clear to 0
        }
        
        if (u8IsErr)
            u16DlyNum++;
        else 
            break;
        
        if (u16DlyNum > u16MaxDlyNum) // already go through from min. to MAX.
            return 1;
    }
    
    return 0;
}

// TODO: integrate to QSPI (flash/dma use same api)

/*************************************************************************
* FUNCTION:
*  Hal_Psram_Dma_Access
*
* DESCRIPTION:
*   1. Read/Write PSRAM data by DMA (only for QSPI)
*
* CALLS
*
* PARAMETERS
*   1. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   2. eChannel      : DMA channel.
*   3. u32SrcAddr    : Source address.
*   4. u32DstAddr    : Destination address.
*   5. u32Size       : Size of DMA transfer.
* 
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Psram_Dma_Access_impl(E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size)
{
    return Hal_Qspi_Dma_Access(eSlvIdx, eChannel, u32SrcAddr, u32DstAddr, u32Size);
}

