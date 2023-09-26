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
#ifndef __HAL_PSRAM_H__
#define __HAL_PSRAM_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include "hal_qspi.h"
#include "hal_dma.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define KNOWN_GOOD_DIE      0x5D
#define MF_ID               0x0D
#define NO_PSRAM            0

#define PSRAM_TIMEOUT_MS    500 // 500ms 

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef uint32_t (*T_Hal_Psram_Init)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8UseQuadMode, uint8_t u8Standalone);
typedef uint32_t (*T_Hal_Psram_Reset)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
typedef uint32_t (*T_Hal_Psram_Opcode_Config)(uint8_t u8UseQuadMode);
typedef uint32_t (*T_Hal_Psram_DeviceId)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t *pu32Mfid, uint32_t *pu32Kgd);
typedef uint8_t  (*T_Hal_Psram_RxSampleDelay_Set)(E_SpiSlave_t eSlvIdx);
typedef uint32_t (*T_Hal_Psram_Dma_Access)(E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
extern T_Hal_Psram_Init                Hal_Psram_Init;
extern T_Hal_Psram_Reset               Hal_Psram_Reset;
extern T_Hal_Psram_Opcode_Config       Hal_Psram_Opcode_Config;
extern T_Hal_Psram_DeviceId            Hal_Psram_DeviceId;
extern T_Hal_Psram_RxSampleDelay_Set   Hal_Psram_RxSampleDelay_Set;
extern T_Hal_Psram_Dma_Access          Hal_Psram_Dma_Access;

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
uint32_t Hal_Psram_Init_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8UseQuadMode, uint8_t u8Standalone);
uint32_t Hal_Psram_Reset_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t Hal_Psram_Opcode_Config_impl(uint8_t u8UseQuadMode);
uint32_t Hal_Psram_DeviceId_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t *pu32Mfid, uint32_t *pu32Kgd);
uint8_t Hal_Psram_RxSampleDelay_Set_impl(E_SpiSlave_t eSlvIdx);
uint32_t Hal_Psram_Dma_Access_impl(E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size);
#include "hal_psram_patch.h"
#endif

