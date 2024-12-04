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
#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include "hal_spi.h"
#include "hal_qspi.h"
#include "hal_dma.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define FLASH_TIMEOUT_MS    				100

#define FLASH_CMD_WRITE_STATUS_REG          0x01
#define FLASH_CMD_SIO_PROGRAM               0x02
#define FLASH_CMD_SIO_READ                  0x03
#define FLASH_CMD_READ_STATUS_REG           0x05
#define FLASH_CMD_WRITE_ENABLE              0x06
#define FLASH_CMD_READ_STATUS_REG_2_MXIC    0x15
#define FLASH_CMD_SECTOR_ERASE              0x20
#define FLASH_CMD_QIO_PROGRAM               0x32
#define FLASH_CMD_READ_STATUS_REG_2_REG     0x35
#define FLASH_CMD_QIO_PROGRAM_MXIC          0x38
#define FLASH_CMD_RESET_ENABLE              0x66
#define FLASH_CMD_RESET                     0x99
#define FLASH_CMD_READ_ID_REG               0x9F
#define FLASH_CMD_QIO_READ                  0xEB
#define FLASH_CMD_ENTER_POWER_DOWN          0xB9
#define FLASH_CMD_RELEASE_PWD_RDID          0xAB

#define FLASH_STATUS_BUSY                   0x01
#define FLASH_STATUS_WEL                    0x02
#define FLASH_STATUS_QE_MXIC                0x40
#define FLASH_STATUS_QE                     0x02

#define FLASH_PAGE_SIZE                     0x100  // 256 bytes
#define FLASH_SECTOR_SIZE                   0x1000 // 4096 = 4K bytes

#define FLASH_DR_DUMMY_VAL                  0x00
#ifdef __RTL_SIMULATION__
#define FLASH_POLLING_BUSY_TIMEOUT_MS       0

#else
#define FLASH_POLLING_BUSY_TIMEOUT_MS       400 /* 400 ms for erasing 4K sector */
#endif


#define FLASH_T_RESUME_US_DEFAULT   40
      
/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef enum 
{
    NOT_SUPPORTED,
    SUPPORTED_FLASH
} E_FlashCheckStatus_t;

typedef struct
{
    uint32_t u32Speed;
    uint16_t u16RxSampleDly;
} T_HalSpiRtSetting;

typedef uint32_t (*T_Hal_Flash_Init)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
typedef uint32_t (*T_Hal_Flash_QuadModeEn)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8QModeEn);
typedef void (*T_Hal_Flash_Opcode_Config)(E_SpiSlave_t eSlvIdx, uint8_t u8UseQuadMode);
typedef uint32_t (*T_Hal_Flash_4KSectorAddrErase_Ext)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr);
typedef uint32_t (*T_Hal_Flash_4KSectorAddrErase)(E_SpiIdx_t eSpiIdx, uint32_t u32SecAddr);
//typedef uint32_t (*T_Hal_Flash_4KSectorIdxErase)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecIdx);
//typedef uint32_t (*T_Hal_Flash_PageAddrProgram)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
//typedef uint32_t (*T_Hal_Flash_PageIdxProgram)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageIdx, uint8_t u8UseQuadMode, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_PageAddrRead)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_AddrProgram_Ext)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_AddrProgram)(E_SpiIdx_t eSpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_AddrRead_Ext)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
typedef uint32_t (*T_Hal_Flash_AddrRead)(E_SpiIdx_t eSpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
typedef void (*T_Hal_Flash_Reset)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
typedef E_FlashCheckStatus_t (*T_Hal_Flash_Check_Ext)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
typedef E_FlashCheckStatus_t (*T_Hal_Flash_Check)(E_SpiIdx_t eSpiIdx);
typedef uint32_t (*T_Hal_Flash_Dma_Access)(E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size);
typedef uint32_t (*T_Hal_Flash_Dma_Read)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eTxChannel, E_DMA_CHANNEL eRxChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size, uint8_t u8UseQuadMode);
typedef uint32_t (*T_Hal_Flash_Dma_Write)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size, uint8_t u8UseQuadMode);
typedef uint32_t (*T_Hal_Flash_EnterPowerDown)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
typedef uint32_t (*T_Hal_Flash_ReleasePowerDown)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32WaitResume);
typedef uint32_t (*T_Hal_Flash_GetResumeTimeTicks)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
extern uint32_t g_u32FlashTimeoutMs;

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
extern T_Hal_Flash_Init                  Hal_Flash_Init;
extern T_Hal_Flash_QuadModeEn            Hal_Flash_QuadModeEn;
extern T_Hal_Flash_Opcode_Config         Hal_Flash_Opcode_Config;
extern T_Hal_Flash_4KSectorAddrErase_Ext Hal_Flash_4KSectorAddrErase_Ext;
extern T_Hal_Flash_4KSectorAddrErase     Hal_Flash_4KSectorAddrErase;
//extern T_Hal_Flash_4KSectorIdxErase      Hal_Flash_4KSectorIdxErase;
//extern T_Hal_Flash_PageAddrProgram       Hal_Flash_PageAddrProgram;
//extern T_Hal_Flash_PageIdxProgram        Hal_Flash_PageIdxProgram;
//extern T_Hal_Flash_PageAddrRead          Hal_Flash_PageAddrRead;
extern T_Hal_Flash_AddrProgram_Ext       Hal_Flash_AddrProgram_Ext;
extern T_Hal_Flash_AddrProgram           Hal_Flash_AddrProgram;
extern T_Hal_Flash_AddrRead_Ext          Hal_Flash_AddrRead_Ext;
extern T_Hal_Flash_AddrRead              Hal_Flash_AddrRead;
extern T_Hal_Flash_Reset                 Hal_Flash_Reset;
extern T_Hal_Flash_Check_Ext             Hal_Flash_Check_Ext;
extern T_Hal_Flash_Check                 Hal_Flash_Check;
extern T_Hal_Flash_Dma_Access            Hal_Flash_Dma_Access;
extern T_Hal_Flash_Dma_Read              Hal_Flash_Dma_Read;
extern T_Hal_Flash_Dma_Write             Hal_Flash_Dma_Write;
extern T_Hal_Flash_EnterPowerDown        Hal_Flash_EnterPowerDown;
extern T_Hal_Flash_ReleasePowerDown      Hal_Flash_ReleasePowerDown;
extern T_Hal_Flash_GetResumeTimeTicks    Hal_Flash_GetResumeTimeTicks;

uint32_t _Hal_Flash_ManufDeviceId_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t *pu32Manufacturer, uint32_t *pu32MemoryType, uint32_t *pu32MemoryDensity );
uint32_t _Hal_Flash_Status1Get_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t *pu32Status);
uint32_t _Hal_Flash_Status2Get_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t *pu32Status);
uint32_t _Hal_Flash_StatusSet_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8Sts_1, uint8_t u8Sts_2);
uint32_t _Hal_Flash_WriteDoneCheck_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t _Hal_Flash_CmdSend_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8Opcode);
uint32_t _Hal_Flash_WriteEn_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t _Hal_Flash_RxSampleDelaySetup_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t _Hal_Flash_Dma_Read_SIO_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eTxChannel, E_DMA_CHANNEL eRxChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size);
uint32_t _Hal_Flash_Dma_Read_QIO_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size);
uint32_t _Hal_Flash_Dma_Write_SIO_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size);
uint32_t _Hal_Flash_Dma_Write_QIO_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size);

uint32_t Hal_Flash_Init_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t Hal_Flash_4KSectorAddrErase_Internal_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr);
uint32_t Hal_Flash_4KSectorAddrErase_Internal_impl(E_SpiIdx_t eSpiIdx, uint32_t u32SecAddr);
//uint32_t Hal_Flash_4KSectorIdxErase_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecIdx);
//uint32_t Hal_Flash_PageAddrProgram_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
//uint32_t Hal_Flash_PageIdxProgram_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageIdx, uint8_t u8UseQuadMode, uint8_t *pu8Data);
uint32_t Hal_Flash_PageAddrRead_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrProgram_Internal_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrProgram_Internal_impl(E_SpiIdx_t eSpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrRead_Internal_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrRead_Internal_impl(E_SpiIdx_t eSpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
void Hal_Flash_Reset_Internal_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);


uint32_t Hal_Flash_Init_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t Hal_Flash_InitSemaphore(E_SpiIdx_t eSpiIdx);
uint32_t Hal_Flash_QuadModeEn_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8QModeEn);
void Hal_Flash_Opcode_Config_impl(E_SpiSlave_t eSlvIdx, uint8_t u8UseQuadMode);
uint32_t Hal_Flash_4KSectorAddrErase_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecAddr);
uint32_t Hal_Flash_4KSectorAddrErase_impl(E_SpiIdx_t eSpiIdx, uint32_t u32SecAddr);
//uint32_t Hal_Flash_4KSectorIdxErase_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32SecIdx);
//uint32_t Hal_Flash_PageAddrProgram_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
//uint32_t Hal_Flash_PageIdxProgram_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageIdx, uint8_t u8UseQuadMode, uint8_t *pu8Data);
uint32_t Hal_Flash_PageAddrRead_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32PageAddr, uint8_t u8UseQuadMode, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrProgram_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrProgram_impl(E_SpiIdx_t eSpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrRead_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t Hal_Flash_AddrRead_impl(E_SpiIdx_t eSpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
void Hal_Flash_Reset_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
E_FlashCheckStatus_t Hal_Flash_Check_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
E_FlashCheckStatus_t Hal_Flash_Check_impl(E_SpiIdx_t eSpiIdx);
uint32_t Hal_Flash_Dma_Access_impl(E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size);
uint32_t Hal_Flash_Dma_Read_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eTxChannel, E_DMA_CHANNEL eRxChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size, uint8_t u8UseQuadMode);
uint32_t Hal_Flash_Dma_Write_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size, uint8_t u8UseQuadMode);
uint32_t Hal_Flash_EnterPowerDown_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint32_t Hal_Flash_ReleasePowerDown_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32WaitResume);
uint32_t Hal_Flash_GetResumeTimeTicks_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);


#include "hal_flash_patch.h"
#endif

