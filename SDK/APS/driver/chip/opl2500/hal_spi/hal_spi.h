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
#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

#ifdef __RTL_SIMULATION__
#define SPI_TIMEOUT_MS      0
#else
#define SPI_TIMEOUT_MS      100
#endif

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef enum 
{
    SPI_IDX_0, // SPI_0 is for QSPI now
    SPI_IDX_1, // SPI master
    SPI_IDX_2, // SPI slave
    SPI_IDX_3, // SPI master
    SPI_IDX_MAX,
} E_SpiIdx_t;

typedef enum
{
    SPI_SLAVE_0 = 0,
    SPI_SLAVE_1 = 1,
    SPI_SLAVE_2 = 2,
    SPI_SLAVE_3 = 3,
    SPI_SLAVE_MAX,
} E_SpiSlave_t;

typedef enum 
{
    SPI_DFS_04_bit = 0x03,
    SPI_DFS_05_bit = 0x04,
    SPI_DFS_06_bit = 0x05,
    SPI_DFS_07_bit = 0x06,
    SPI_DFS_08_bit = 0x07,
    SPI_DFS_09_bit = 0x08,
    SPI_DFS_10_bit = 0x09,
    SPI_DFS_11_bit = 0x0A,
    SPI_DFS_12_bit = 0x0B,
    SPI_DFS_13_bit = 0x0C,
    SPI_DFS_14_bit = 0x0D,
    SPI_DFS_15_bit = 0x0E,
    SPI_DFS_16_bit = 0x0F,
    SPI_DFS_17_bit = 0x10,
    SPI_DFS_18_bit = 0x11,
    SPI_DFS_19_bit = 0x12,
    SPI_DFS_20_bit = 0x13,
    SPI_DFS_21_bit = 0x14,
    SPI_DFS_22_bit = 0x15,
    SPI_DFS_23_bit = 0x16,
    SPI_DFS_24_bit = 0x17,
    SPI_DFS_25_bit = 0x18,
    SPI_DFS_26_bit = 0x19,
    SPI_DFS_27_bit = 0x1A,
    SPI_DFS_28_bit = 0x1B,
    SPI_DFS_29_bit = 0x1C,
    SPI_DFS_30_bit = 0x1D,
    SPI_DFS_31_bit = 0x1E,
    SPI_DFS_32_bit = 0x1F,
} E_SpiDataFrameSize_t;

typedef enum
{
    SPI_FRF_STD   = 0,      /* single-bit mode */
    SPI_FRF_DUAL  = 1,      /* dual-bit mode */
    SPI_FRF_QUAD  = 2,      /* quad-bit mode */
    SPI_FRF_OCTAL = 3,      /* octal-bit mdode */
} E_SpiFrf_t;

typedef enum
{
    SPI_TMOD_TRX         = 0,
    SPI_TMOD_TX_ONLY     = 1,
    SPI_TMOD_RX_ONLY     = 2,
    SPI_TMOD_EEPROM_READ = 3,
} E_SpiTransferMode_t;

typedef enum 
{
    SPI_CLK_PLOAR_LOW_ACT  = 0,
    SPI_CLK_PLOAR_HIGH_ACT = 1,
} E_SpiClkPolarity_t;

typedef enum 
{
    SPI_CLK_PHASE_MIDDLE = 0,
    SPI_CLK_PHASE_START  = 1,
} E_SpiClkPhase_t;

typedef enum 
{
    SPI_FMT_MOTOROLA    = 0,
    SPI_FMT_TEXAS_INSTR = 1,
    SPI_FMT_NATION_SEMI = 2,
} E_SpiFrameFormat_t;

typedef enum
{
    SPI_INST_L_0_bit  = 0, 
    SPI_INST_L_4_bit  = 1,
    SPI_INST_L_8_bit  = 2,
    SPI_INST_L_16_bit = 3,
} E_SpiInstLength_t;

typedef enum
{
    SPI_ADDR_L_0_bit  = 0x0,
    SPI_ADDR_L_4_bit  = 0x1,
    SPI_ADDR_L_8_bit  = 0x2,
    SPI_ADDR_L_12_bit = 0x3,
    SPI_ADDR_L_16_bit = 0x4,
    SPI_ADDR_L_20_bit = 0x5,
    SPI_ADDR_L_24_bit = 0x6,
    SPI_ADDR_L_28_bit = 0x7,
    SPI_ADDR_L_32_bit = 0x8,
    SPI_ADDR_L_36_bit = 0x9,
    SPI_ADDR_L_40_bit = 0xA,
    SPI_ADDR_L_44_bit = 0xB,
    SPI_ADDR_L_48_bit = 0xC,
    SPI_ADDR_L_52_bit = 0xD,
    SPI_ADDR_L_56_bit = 0xE,
    SPI_ADDR_L_60_bit = 0xF,
} E_SpiAddrLength_t;

typedef enum
{
    SPI_INST_std_ADDR_std = 0, /* Both Instruction & Address: Standard SPI mode */
    SPI_INST_std_ADDR_frf = 1, /* Instruction: Standard SPI mode; Address: specified by CTRLR0.SPI_FRF */
    SPI_INST_frf_ADDR_frf = 2, /* Both Instruction & Address: specified by CTRLR0.SPI_FRF */
} E_SpiTransferType_t;

typedef struct
{
    E_SpiFrf_t eSpi_frf;
    E_SpiDataFrameSize_t eDfs_32;
    E_SpiTransferMode_t eTmod;
    E_SpiInstLength_t eInst_l; /* Note: SPI_CTRLR0 (offset: 0xF4) is only valid when SPI_FRF != 0 */
    E_SpiAddrLength_t eAddr_l;
    E_SpiTransferType_t eTrans_type;
    uint16_t u16Ndf;
    uint8_t u8WaitCycles:5;             
    uint8_t reserved:3;
} S_SPI_Config_t;

#if 1  // debug usage
// add ((S_SPI_MST_Debug_t *) SPI1_BASE) to Watch Windows
#pragma push
#pragma anon_unions
typedef struct
{
    /* Offset: 0x000 (R/W) Control Register 0                                                               */
    uint32_t CTRLR0_DFS:4;
    uint32_t CTRLR0_FRF:2;
    uint32_t CTRLR0_SCPH:1;
    uint32_t CTRLR0_SCPOL:1;
    uint32_t CTRLR0_TMOD:2;
    uint32_t CTRLR0_SLV_OE:1;
    uint32_t CTRLR0_SRL:1;
    uint32_t CTRLR0_CFS:4;
    uint32_t CTRLR0_DFS_32:5;
    uint32_t CTRLR0_SPI_FRF:2;
    uint32_t CTRLR0_rsv0:1;
    uint32_t CTRLR0_SSTE:1;
    uint32_t CTRLR0_rsv1:7;
    /* Offset: 0x004 (R/W) Control Register 1                                                               */
    uint32_t CTRLR1_NDF:16;
    uint32_t CTRLR1_rsv:16;
    /* Offset: 0x008 (R/W) SSI Enable Register                                                              */
    uint32_t SSIENR_SSI_EN:1;
    uint32_t SSIENR_rsv:31;
    /* Offset: 0x00C (R/W) Microwire Control Register                                                       */
    uint32_t MWCR_MWMOD:1;
    uint32_t MWCR_MDD:1;
    uint32_t MWCR_MHS:1;
    uint32_t MWCR_rsv:29;
    /* Offset: 0x010 (R/W) Slave Enable Register                                                            */
    uint32_t SER_SER:4;
    uint32_t SER_rsv:28;
    /* Offset: 0x014 (R/W) Baud Rate Select                                                                 */
    uint32_t BAUDR_SCKDV:16;
    uint32_t BAUDR_rsv:16;
    /* Offset: 0x018 (R/W)Transmit FIFO Threshold Level                                                    */
    uint32_t TXFTLR_TFT:4;
    uint32_t TXFTLR_rsv:28;
    /* Offset: 0x01C (R/W) Receive FIFO Threshold Level                                                     */
    uint32_t RXFTLR_TFT:4;
    uint32_t RXFTLR_rsv:28;
    /* Offset: 0x020 (R/ ) Transmit FIFO Level Register                                                     */
    uint32_t TXFLR_TXTFL:5;
    uint32_t TXFLR_rsv:27;
    /* Offset: 0x024 (R/ ) Receive FIFO Level Register                                                      */
    uint32_t RXFLR_RXTFL:5;
    uint32_t RXFLR_rsv:27;
    /* Offset: 0x028 (R/ ) Status Register                                                                  */
    uint32_t SR_BUSY:1;
    uint32_t SR_TFNF:1;
    uint32_t SR_TFE:1;
    uint32_t SR_RFNE:1;
    uint32_t SR_RFF:1;
    uint32_t SR_TXE:1;
    uint32_t SR_DCOL:1;
    uint32_t SR_rsv:25;
    /* Offset: 0x02C (R/W) Interrupt Mask Register                                                          */
    uint32_t IMR_TXEIM:1;
    uint32_t IMR_TXOIM:1;
    uint32_t IMR_RXUIM:1;
    uint32_t IMR_RXOIM:1;
    uint32_t IMR_RXFIM:1;
    uint32_t IMR_MSTIM:1;
    uint32_t IMR_rsv:26;
    /* Offset: 0x030 (R/ ) Interrupt Status Register                                                        */
    uint32_t ISR_TXEIS:1;
    uint32_t ISR_TXOIS:1;
    uint32_t ISR_RXUIS:1;
    uint32_t ISR_RXOIS:1;
    uint32_t ISR_RXFIS:1;
    uint32_t ISR_MSTIS:1;
    uint32_t ISR_rsv:26;
    /* Offset: 0x034 (R/ ) Raw Interrupt Status Register                                                    */
    uint32_t RISR_TXEIR:1;
    uint32_t RISR_TXOIR:1;
    uint32_t RISR_RXUIR:1;
    uint32_t RISR_RXOIR:1;
    uint32_t RISR_RXFIR:1;
    uint32_t RISR_MSTIR:1;
    uint32_t RISR_rsv:26;
    /* Offset: 0x038 (R/ ) Transmit FIFO Overflow Interrupt Clear Register                                  */
    uint32_t TXOICR_TXOICR:1;
    uint32_t TXOICR_rsv:31;
    /* Offset: 0x03C (R/ ) Receive FIFO Overflow Interrupt Clear Register                                   */
    uint32_t RXOICR_RXOICR:1;
    uint32_t RXOICR_rsv:31;
    /* Offset: 0x040 (R/ ) Receive FIFO Underflow Interrupt Clear Register                                  */
    uint32_t RXUICR_RXUICR:1;
    uint32_t RXUICR_rsv:31;
    /* Offset: 0x044 (R/ ) Multi-Master Interrupt Clear Register                                            */
    uint32_t MSTICR_MSTICR:1;
    uint32_t MSTICR_rsv:31;
    /* Offset: 0x048 (R/ ) Interrupt Clear Register                                                         */
    uint32_t ICR_ICR:1;
    uint32_t ICR_rsv:31;
    /* Offset: 0x04C (R/W) DMA Control Register                                                             */
    uint32_t DMACR_RDMAE:1;
    uint32_t DMACR_TDMAE:1;
    uint32_t DMACR_rsv:30;
    /* Offset: 0x050 (R/W) DMA Transmit Data Level                                                          */
    uint32_t DMATDLR_DMATDL:5;
    uint32_t DMATDLR_rsv:27;
    /* Offset: 0x054 (R/W) DMA Receive Data Level                                                           */
    uint32_t DMARDLR_DMARDL:6;
    uint32_t DMARDLR_rsv:26;
    /* Offset: 0x058 (R/ ) Identification Register                                                          */
    uint32_t IDR_IDCODE;
    /* Offset: 0x05C (R/ ) coreKit version ID Register                                                      */
    uint32_t SSI_VERSION_ID;
    /* Offset: 0x060 ~ 0x0EC (R/W) Data Register                                                            */
    uint32_t DR[36];
    /* Offset: 0x0F0 (R/W) RX Sample Delay Register                                                         */
    uint32_t RX_SAMPLE_DLY_RSD:8;
    uint32_t RX_SAMPLE_DLY_rsv:24;
    /* Offset: 0x0F4 (R/W) SPI Control Register                                                             */
    uint32_t SPI_CTRLR0_TRANS_TYPE:2;
    uint32_t SPI_CTRLR0_ADDR_L:4;
    uint32_t SPI_CTRLR0_rsv0:2;
    uint32_t SPI_CTRLR0_INST_L:2;
    uint32_t SPI_CTRLR0_rsv1:1;
    uint32_t SPI_CTRLR0_WAIT_CYCLES:5;
    uint32_t SPI_CTRLR0_SPI_DDR_EN:1;
    uint32_t SPI_CTRLR0_INST_DDR_EN:1;
    uint32_t SPI_CTRLR0_SPI_RXDS_EN:1;
    uint32_t SPI_CTRLR0_rsv2:13;
    /* Offset: 0x0F8 (R/W) SPI Opulinks Proprietary Register */
    uint32_t SPI_OPLR0_BYTE_REVERSE_EN:1;
    uint32_t SPI_OPLR0_CS_ASSERT_EN:1;
    uint32_t SPI_OPLR0_READ_FREEZE_EN:1;
    uint32_t SPI_OPLR0_OEN_EN:1;
    uint32_t SPI_OPLR0_rsv:28;
} S_SPI_MST_Debug_t;
#pragma pop
#endif

typedef void (*T_Spi_CallBack)(void);

typedef uint32_t (*T__Hal_Spi_Master_Init_fp)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn);
typedef uint32_t (*T__Hal_Spi_Slave_Init_fp)(E_SpiIdx_t eSpiIdx, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize);
typedef uint32_t (*T__Hal_Spi_Master_Data_Send_fp)(E_SpiIdx_t eSpiIdx, uint32_t u32Data);
typedef uint32_t (*T__Hal_Spi_Slave_Data_Send_fp)(E_SpiIdx_t eSpiIdx, uint32_t u32Data);
typedef uint32_t (*T__Hal_Spi_Master_Data_Recv_fp)(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data);
typedef uint32_t (*T__Hal_Spi_Slave_Data_Recv_fp)(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data);
typedef uint32_t (*T__Hal_Spi_Master_Byte_Reverse_fp)(E_SpiIdx_t eSpiIdx, uint8_t u8Enable);
typedef uint32_t (*T__Hal_Spi_Master_Cs_Assert_fp)(E_SpiIdx_t eSpiIdx, uint8_t u8Enable);
typedef uint32_t (*T__Hal_Spi_Master_Read_Freeze_fp)(E_SpiIdx_t eSpiIdx, uint8_t u8Enable);
typedef uint32_t (*T__Hal_Spi_Poll_Busy_fp)(E_SpiIdx_t eSpiIdx);
typedef uint32_t (*T__Hal_Spi_Enable_fp)(E_SpiIdx_t eSpiIdx, uint8_t u8Enable);

typedef uint32_t (*T_Hal_Spi_BaudRateGet_fp)(E_SpiIdx_t eSpiIdx);
typedef uint32_t (*T_Hal_Spi_BaudRateSet_fp)(E_SpiIdx_t eSpiIdx, uint32_t u32Baud);
typedef uint32_t (*T_Hal_Spi_DividerUpdate_fp)(E_SpiIdx_t eSpiIdx);
typedef uint32_t (*T_Hal_Spi_Init_fp)(E_SpiIdx_t eSpiIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn);
typedef uint32_t (*T_Hal_Spi_Init_Ext_fp)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn);
typedef uint32_t (*T_Hal_Spi_RxSampleDly_Set_fp)(E_SpiIdx_t eSpiIdx, uint16_t u16DlyNum);
typedef uint32_t (*T_Hal_Spi_Data_Send_fp)(E_SpiIdx_t eSpiIdx, uint32_t u32Data);
typedef uint32_t (*T_Hal_Spi_Data_Recv_fp)(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data);
typedef uint32_t (*T_Hal_Spi_Master_Config_fp)(E_SpiIdx_t eSpiIdx, S_SPI_Config_t *ptCfg);
typedef uint32_t (*T_Hal_Spi_Slave_Config_fp)(E_SpiIdx_t eSpiIdx, S_SPI_Config_t *ptCfg);
typedef uint32_t (*T_Hal_Spi_Slave_Select_Enable_fp)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Spi_Dma_Config_fp)(E_SpiIdx_t eSpiIdx, uint8_t u8TxEnable, uint8_t u8RxEnable, uint8_t u8TxDataLv, uint8_t u8RxDataLv);
typedef uint32_t (*T_Hal_Spi_IntMask_fp)(E_SpiIdx_t eSpiIdx, uint32_t u32Bmp);
typedef uint32_t (*T_Hal_Spi_IntEn_fp)(E_SpiIdx_t eSpiIdx, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Spi_IntClearAll_fp)(E_SpiIdx_t eSpiIdx);
typedef void (*T_Hal_Spi_Callback_Set_fp)(E_SpiIdx_t eSpiIdx, T_Spi_CallBack tFunc);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
extern T_Spi_CallBack g_tHalSpi1CallBack;
extern T_Spi_CallBack g_tHalSpi2CallBack;
extern T_Spi_CallBack g_tHalSpi3CallBack;

extern T__Hal_Spi_Master_Init_fp _Hal_Spi_Master_Init;
extern T__Hal_Spi_Slave_Init_fp _Hal_Spi_Slave_Init;
extern T__Hal_Spi_Master_Data_Send_fp _Hal_Spi_Master_Data_Send;
extern T__Hal_Spi_Slave_Data_Send_fp _Hal_Spi_Slave_Data_Send;
extern T__Hal_Spi_Master_Data_Recv_fp _Hal_Spi_Master_Data_Recv;
extern T__Hal_Spi_Slave_Data_Recv_fp _Hal_Spi_Slave_Data_Recv;
extern T__Hal_Spi_Master_Byte_Reverse_fp _Hal_Spi_Master_Byte_Reverse;
extern T__Hal_Spi_Master_Cs_Assert_fp _Hal_Spi_Master_Cs_Assert;
extern T__Hal_Spi_Master_Read_Freeze_fp _Hal_Spi_Master_Read_Freeze;
extern T__Hal_Spi_Poll_Busy_fp _Hal_Spi_Poll_Busy;
extern T__Hal_Spi_Enable_fp _Hal_Spi_Enable;

extern T_Hal_Spi_BaudRateGet_fp Hal_Spi_BaudRateGet;
extern T_Hal_Spi_BaudRateSet_fp Hal_Spi_BaudRateSet;
extern T_Hal_Spi_DividerUpdate_fp Hal_Spi_DividerUpdate;
extern T_Hal_Spi_Init_fp Hal_Spi_Init;
extern T_Hal_Spi_Init_Ext_fp Hal_Spi_Init_Ext;
extern T_Hal_Spi_RxSampleDly_Set_fp Hal_Spi_RxSampleDly_Set;
extern T_Hal_Spi_Data_Send_fp Hal_Spi_Data_Send;
extern T_Hal_Spi_Data_Recv_fp Hal_Spi_Data_Recv;
extern T_Hal_Spi_Master_Config_fp Hal_Spi_Master_Config;
extern T_Hal_Spi_Slave_Config_fp Hal_Spi_Slave_Config;
extern T_Hal_Spi_Slave_Select_Enable_fp Hal_Spi_Slave_Select_Enable;
extern T_Hal_Spi_Dma_Config_fp Hal_Spi_Dma_Config;
extern T_Hal_Spi_IntMask_fp Hal_Spi_IntMask;
extern T_Hal_Spi_IntEn_fp Hal_Spi_IntEn;
extern T_Hal_Spi_IntClearAll_fp Hal_Spi_IntClearAll;
extern T_Hal_Spi_Callback_Set_fp Hal_Spi_Callback_Set;

// Sec 5: declaration of global function prototype

/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype
uint32_t _Hal_Spi_Master_Init_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn);
uint32_t _Hal_Spi_Slave_Init_impl(E_SpiIdx_t eSpiIdx, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize);
uint32_t _Hal_Spi_Master_Data_Send_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Data);
uint32_t _Hal_Spi_Slave_Data_Send_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Data);
uint32_t _Hal_Spi_Master_Data_Recv_impl(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data);
uint32_t _Hal_Spi_Slave_Data_Recv_impl(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data);
uint32_t _Hal_Spi_Master_Byte_Reverse_impl(E_SpiIdx_t eSpiIdx, uint8_t u8Enable);
uint32_t _Hal_Spi_Master_Cs_Assert_impl(E_SpiIdx_t eSpiIdx, uint8_t u8Enable);
uint32_t _Hal_Spi_Master_Read_Freeze_impl(E_SpiIdx_t eSpiIdx, uint8_t u8Enable);
uint32_t _Hal_Spi_Poll_Busy_impl(E_SpiIdx_t eSpiIdx);
uint32_t _Hal_Spi_Enable_impl(E_SpiIdx_t eSpiIdx, uint8_t u8Enable);
uint32_t Hal_Spi_BaudRateGet_impl(E_SpiIdx_t eSpiIdx);
uint32_t Hal_Spi_BaudRateSet_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Baud);
uint32_t Hal_Spi_DividerUpdate_impl(E_SpiIdx_t eSpiIdx);
uint32_t Hal_Spi_Init_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn);
uint32_t Hal_Spi_Init_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn);
uint32_t Hal_Spi_RxSampleDly_Set_impl(E_SpiIdx_t eSpiIdx, uint16_t u16DlyNum);
uint32_t Hal_Spi_Data_Send_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Data);
uint32_t Hal_Spi_Data_Recv_impl(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data);
uint32_t Hal_Spi_Master_Config_impl(E_SpiIdx_t eSpiIdx, S_SPI_Config_t *ptCfg);
uint32_t Hal_Spi_Slave_Config_impl(E_SpiIdx_t eSpiIdx, S_SPI_Config_t *ptCfg);
uint32_t Hal_Spi_Slave_Select_Enable_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8Enable);
uint32_t Hal_Spi_Dma_Config_impl(E_SpiIdx_t eSpiIdx, uint8_t u8TxEnable, uint8_t u8RxEnable, uint8_t u8TxDataLv, uint8_t u8RxDataLv);
uint32_t Hal_Spi_IntMask_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Bmp);
uint32_t Hal_Spi_IntEn_impl(E_SpiIdx_t eSpiIdx, uint8_t u8Enable);
uint32_t Hal_Spi_IntClearAll_impl(E_SpiIdx_t eSpiIdx);
void Hal_Spi_Callback_Set_impl(E_SpiIdx_t eSpiIdx, T_Spi_CallBack tFunc);
    
/***********
C Functions
***********/
// Sec 8: C Functions


#endif /* __HAL_SPI_H__ */
