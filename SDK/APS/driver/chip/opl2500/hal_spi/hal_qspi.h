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
 *  @file hal_qspi.h
 *  @brief API to control QSPI
 ******************************************************************************/

#ifndef __HAL_QSPI_H__
#define __HAL_QSPI_H__

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
#include "hal_spi.h"
#include "hal_dma.h"

/*
 *************************************************************************
 *						    Definitions and Macros
 *************************************************************************
 */
#ifdef __RTL_SIMULATION__
#define QSPI_TIMEOUT_MS                 0
#else
#define QSPI_TIMEOUT_MS                 10 
#endif

#define IS_MEM_ADDR_SPACE(addr)                                 \
    ( (((addr) >= 0x20600000) && ((addr) <= 0x20617FFF)) ||     \
      (((addr) >= 0x00600000) && ((addr) <= 0x00617FFF)) ||     \
      (((addr) >= 0x00500000) && ((addr) <= 0x0050FFFF)) ||     \
      (((addr) >= 0x00400000) && ((addr) <= 0x0046FFFF)) )

#define IS_XIP_ADDR_SPACE(addr)                                 \
    ( (((addr) >= 0x01000000) && ((addr) <= 0x017FFFFF)) ||     \
      (((addr) >= 0x01800000) && ((addr) <= 0x01FFFFFF)) ||     \
      (((addr) >= 0x03000000) && ((addr) <= 0x037FFFFF)) ||     \
      (((addr) >= 0x03800000) && ((addr) <= 0x03FFFFFF)) )
      
#define QSPI_REMAP_ADDR                (0x0 - APS_XIP_MEM_BASE) // (0xFD000000 + 0x03000000) = 0x00000000
#define QSPI_CFG_CSSOT_NS              (5)                      // CSSOT <=> tSLCH (CS# Active Setup Time)
#define QSPI_CFG_CSEOT_NS              (5)                      // CSEOT <=> tCHSH (CS# Active Hold Time)
#define QSPI_CFG_CSDADS_NS             (255)                    // CSDADS <=> tSD2D
#define QSPI_CFG_CSDA_NS               (30)                     // CSDA <=> tSHSL (CS# High Time (read/write))

#define QSPI_DSR_SIZE_8Mb              (0x0)    // 8 M bit
#define QSPI_DSR_SIZE_64Mb             (0x1)    // 64 M bit
#define QSPI_DSR_SIZE_2Gb              (0x2)    // 2 G bit
#define QSPI_DSR_SIZE_4Gb              (0x3)    // 4 G bit

/* Interrupt Registers */
/* Bitmap of Type for Interrupt Register */
#define QSPI_BMP_MODE_FAIL             (0x0001) // Mode fail, not being used any more 
#define QSPI_BMP_TX_UNDERFLOW          (0x0002) // Underflow Detected
#define QSPI_BMP_LAST_IND_COMPLETED    (0x0004) // Controller has completed last triggered indirect operation
#define QSPI_BMP_IND_N_ACCEPTED        (0x0008) // Indirect operation was requested but could not be accepted
#define QSPI_BMP_WRITE_PROTECT_REJ     (0x0010) // Write to protected area was attempted and rejected
#define QSPI_BMP_ILLEGAL_AHB_ACCESS    (0x0020) // Illegal AHB Access Detected
#define QSPI_BMP_WATERMARK_BREACHED    (0x0040) // Indirect Transfer Watermark Level Breached
#define QSPI_BMP_RX_OVERFLOW           (0x0080) // Receive Overflow
#define QSPI_BMP_SMALL_TX_FIFO_N_FULL  (0x0100) // Small TX FIFO not full, can be ignored in non-SPI legacy mode
#define QSPI_BMP_SMALL_TX_FIFO_FULL    (0x0200) // Small TX FIFO full, can be ignored in non-SPI legacy mode
#define QSPI_BMP_SMALL_RX_FIFO_N_EMPTY (0x0400) // Small RX FIFO not empty, can be ignored in non-SPI legacy mode
#define QSPI_BMP_SMALL_RX_FIFO_FULL    (0x0800) // Small RX FIFO full, can be ignored in non-SPI legacy mode
#define QSPI_BMP_IND_READ_SRAM_FULL    (0x1000) // Indirect Read partition of SRAM is full and unable to immediately complete indirect operation
#define QSPI_BMP_POLL_CYCLES_EXPIRED   (0x2000) // The maximum number of programmed polls cycles is expired



/* ***** Register field definition **** */


/*
 *************************************************************************
 *						    Enums and Structures
 *************************************************************************
 */

typedef enum
{
    QSPI_SIO_Mode = 0,  /* Standard SPI mode */
    QSPI_DIO_Mode = 1,  /* Dual I/O SPI mode */
    QSPI_QIO_Mode = 2,  /* Quad I/O SPI mode */
} E_QSPI_IO_Mode_t;

typedef enum
{
    QSPI_Addr_1_Byte = 0,
    QSPI_Addr_2_Byte = 1,
    QSPI_Addr_3_Byte = 2,
    QSPI_Addr_4_Byte = 3,
} E_QSPI_Addr_Bytes_Num_t;

typedef enum
{
    QSPI_Data_1_Byte = 0,
    QSPI_Data_2_Byte = 1,
    QSPI_Data_3_Byte = 2,
    QSPI_Data_4_Byte = 3,
    QSPI_Data_5_Byte = 4,
    QSPI_Data_6_Byte = 5,
    QSPI_Data_7_Byte = 6,
    QSPI_Data_8_Byte = 7,
} E_QSPI_Data_Bytes_Num_t;

typedef struct
{
    // QCR (0x00)
    uint32_t ClkPolar:1;            /* Clock polarity outside SPI word */
    uint32_t ClkPhase:1;            /* Clock phase */
    uint32_t DacEn:1;               /* 1 = enable the Direct Access Contorller */
    uint32_t LegacyEn:1;            /* Legacy IP Mode Enable */
    uint32_t PeriSelDecode:1;       /* Peripheral select decode */
    uint32_t Peri_CS:4;             /* Peripheral chip select liness */
    uint32_t AddrRemapEn:1;         /* Enable AHB Address Re-mapping (Direct Access Mode Only) */
    uint32_t DecoderEn:1;           /* Enable AHB Decoder (Direct Access Mode Only) */
    // DRIR (0x04)
    E_QSPI_IO_Mode_t eInstrType;    /* Insturction Type (Global Setting) */
    // RAR (0x24)
    uint32_t RemapAddr;             /* Remap incoming AHB address to a different address used by the FLASH device */
    // DSR (0x14)
    uint8_t SizeCS0:2;              /* Size of device connected to CS[0] pin */
    uint8_t SizeCS1:2;              /* Size of device connected to CS[1] pin */
    uint8_t SizeCS2:2;              /* Size of device connected to CS[2] pin */
    uint8_t SizeCS3:2;              /* Size of device connected to CS[3] pin */
} S_QSPI_Config_t;

typedef struct
{
    // DRIR (0x04)
    uint32_t RdOpCode:8;                /* Read Opcode */
    uint32_t DDR_BitEn:1;               /* DDR Bit Enable */
    uint32_t ModeBitEn:1;               /* Mode Bit Enable */
    uint32_t RdDummy:5;                 /* Number of Dummy Clock Cycles for Read Instruction */
    E_QSPI_IO_Mode_t eRdAddrXferType;   /* Address Transfer Type */
    E_QSPI_IO_Mode_t eRdDataXferType;   /* Data Transfer Type */
    // DWIR (0x08)
    uint32_t WrOpCode:8;                /* Write Opcode */
    uint32_t WEL_Dis:1;                 /* WEL Disable */
    uint32_t WrDummy:5;                 /* Number of Dummy Clock Cycles for Write Instruction */
    E_QSPI_IO_Mode_t eWrAddrXferType;   /* Address Transfer Type */
    E_QSPI_IO_Mode_t eWrDataXferType;   /* Data Transfer Type */
   // MBR (0x28)
    uint32_t ModeBit:8;                 /* Mode bits */
    // WCCR (0x38)
    uint32_t PollOpCode:8;              /* Poll Opcode */
    uint32_t PollBitIdx:3;              /* Bit index that should be polled */
    uint32_t PollPolarity:1;            /* Polling polarity, 1: write transfer is completed if polled bit is '1' */
    uint32_t Poll_Dis:1;                /* Disable automatic polling */
    uint32_t PollCount:8;               /* Number of times the controller should expect to see a true result from the polling in successive reads of the device register */
    uint32_t PollRepDelay:8;            /* Additional delay for maintain Chip Select de-asserted during auto-polling phase */
} S_QSPI_Instruction_t;

typedef struct
{
    // PSRAM_DRIR (0xC4)
    uint32_t PsramRdOpCode:8;                /* Read Opcode */
    uint32_t PsramDDR_BitEn:1;               /* DDR Bit Enable */
    uint32_t PsramModeBitEn:1;               /* Mode Bit Enable */
    uint32_t PsramRdDummy:5;                 /* Number of Dummy Clock Cycles for Read Instruction */
    E_QSPI_IO_Mode_t ePsramRdAddrXferType;   /* Address Transfer Type */
    E_QSPI_IO_Mode_t ePsramRdDataXferType;   /* Data Transfer Type */
    // PSRAM_DWIR (0xC8)
    uint32_t PsramWrOpCode:8;                /* Write Opcode */
    uint32_t PsramWEL_Dis:1;                 /* WEL Disable */
    uint32_t PsramWrDummy:5;                 /* Number of Dummy Clock Cycles for Write Instruction */
    E_QSPI_IO_Mode_t ePsramWrAddrXferType;   /* Address Transfer Type */
    E_QSPI_IO_Mode_t ePsramWrDataXferType;   /* Data Transfer Type */
} S_QSPI_Psram_Instruction_t;
/* Note: 
 * S_QSPI_Psram_Instruction is for PSRAM device which FLASH_OR_PSRAM (0xBC) is set to 1 (Flash & Psram coexistence case)
 *
 * for PSRAM only case, use S_QSPI_Instruction_t
 */

typedef struct
{
    // QDDR (0x0C)
    uint16_t Cssot;                     /* tSLCH, CS# active setup time (ns) */
    uint16_t Cseot;                     /* tCHSH, CS# active hold time (ns) */
    uint16_t Csdads;                    /* (ns) */
    uint16_t Csda;                      /* tSHSL, CS# high time (ns) */
} S_QSPI_Delay_t;

typedef struct
{
    // MBR (0x28[7:0])
    uint32_t ModeBit:8;                         /* Mode bits */
    // FCR (0x90)
    E_QSPI_Addr_Bytes_Num_t eAddrByteNum;       /* Number of Address Bytes */
    E_QSPI_Data_Bytes_Num_t eWrDataByteNum;     /* Number of Write Data Bytes */
    E_QSPI_Data_Bytes_Num_t eRdDataByteNum;     /* Number of Read Data Bytes */
    uint32_t DummyByteNum:5;                    /* Number of Dummy Bytes */
    uint32_t WrDataEn:1;                        /* Write Data Enable */
    uint32_t ModeBitEn:1;                       /* Mode Bit Enable */
    uint32_t CmdAddrEn:1;                       /* Command Address Enable */
    uint32_t RdDataEn:1;                        /* Read Data Enable */
    uint32_t Opcode:8;                          /* Command Opcode */
    // FCAR (0x94)
    uint32_t CmdAddr;                           /* Command Address */
    // FCRDRL (0xA0)
    uint32_t RdDataLow;                         /* (Output) Command Read Data (Lower byte) */
    // FCRDRU (0xA4)
    uint32_t RdDataUpper;                       /* (Output) Command Read Data (Upper byte) */
    // FCWDRL (0xA8)
    uint32_t WrDataLow;                         /* Command Write Data Lower Byte */
    // FCWDRU (0xAC)
    uint32_t WrDataUpper;                       /* Command Write Data Upper Byte */
} S_QSPI_Command_t;

#if 1
// debug usage
// add ((S_XIP_Debug_t *) XIP_CTRL_BASE) to Watch Windows
#pragma push
#pragma anon_unions
typedef struct 
{
    /* Offset: 0x000  (R/W) QSPI Configuration Register */
    uint32_t QCR_QSPI_En:1;
    uint32_t QCR_CPOL:1;
    uint32_t QCR_CPHA:1;
    uint32_t QCR_PHY_Mode:1;
    uint32_t QCR_rsv1:3;
    uint32_t QCR_DAC_En:1;
    uint32_t QCR_Legacy_IP_Mode:1;
    uint32_t QCR_Peripheral_Select_Decode:1;
    uint32_t QCR_Peripheral_Chip_Select_Lines:4;
    uint32_t QCR_WP:1;
    uint32_t QCR_DMA_Peri_Interface_En:1;
    uint32_t QCR_AHB_Addr_Remap_En:1;
    uint32_t QCR_Enter_XIP_Mode_Next_R:1;
    uint32_t QCR_Enter_XIP_Mode_Imme:1;
    uint32_t QCR_Master_Baud_Div:4;
    uint32_t QCR_AHB_Decoder_En:1;
    uint32_t QCR_DTR_Protocol_En:1;
    uint32_t QCR_Pipeline_PHY_Mode_En:1;
    uint32_t QCR_rsv2:5;
    uint32_t QCR_Idle_Sts:1;
    /* Offset: 0x004  (R/W) Device Read Instruction Regsiter */
    uint32_t DRIR_Read_Opcode:8;
    uint32_t DRIR_Instruction_Type:2;
    uint32_t DRIR_DDR_Bit_En:1;
    uint32_t DRIR_rsv1:1;
    uint32_t DRIR_Addr_Transfer_Type:2;
    uint32_t DRIR_rsv2:2;
    uint32_t DRIR_Data_Transfer_Type:2;
    uint32_t DRIR_rsv3:2;
    uint32_t DRIR_Mode_Bit_En:1;
    uint32_t DRIR_rsv4:3;
    uint32_t DRIR_Dummy_Clock_Cycle_Num:5;
    uint32_t DRIR_rsv5:3;
    /* Offset: 0x008  (R/W) Device Write Instruction Register */
    uint32_t DWIR_Write_Opcode:8;
    uint32_t DWIR_WEL_Dis:1;
    uint32_t DWIR_rsv1:3;
    uint32_t DWIR_Addr_Transfer_Type:2;
    uint32_t DWIR_rsv2:2;
    uint32_t DWIR_Data_Transfer_Type:2;
    uint32_t DWIR_rsv3:6;
    uint32_t DWIR_Dummy_Clock_Cycle_Num:5;
    uint32_t DWIR_rsv4:3;
    /* Offset: 0x00C  (R/W) QSPI Device Delay Register */
    uint32_t QDDR_CSSOT:8;
    uint32_t QDDR_CSEOT:8;
    uint32_t QDDR_CSDADS:8;
    uint32_t QDDR_CSDA:8;
    /* Offset: 0x010  (R/W) Read Data Capture Register */
    uint32_t RDCR_Adapted_Loopback_Clock_Circuit_Bypass:1;
    uint32_t RDCR_Delay_Read_Data_Capturing_Logic:4;
    uint32_t RDCR_Sample_Edge_Sel:1;
    uint32_t RDCR_rsv1:10;
    uint32_t RDCR_Delay_Tx_Data:4;
    uint32_t RDCR_rsv2:12;
    /* Offset: 0x014  (R/W) Device Size Register */
    uint32_t DSR_Addr_Bytes:4;
    uint32_t DSR_Bytes_Per_Device_Page:12;
    uint32_t DSR_Bytes_Per_Block:5;
    uint32_t DSR_Flash_0_Size:2;
    uint32_t DSR_Flash_1_Size:2;
    uint32_t DSR_Flash_2_Size:2;
    uint32_t DSR_Flash_3_Size:2;
    uint32_t DSR_rsv1:3;
    /* Offset: 0x018  (R/W) SRAM Partitioin Configuration Register */
    uint32_t SPCR_Indirect_Read_Size:12;
    uint32_t SPCR_rsv1:20;
    /* Offset: 0x01C  (R/W) Indirect AHB Address Trigger Regsiter */
    uint32_t IAATR;
    /* Offset: 0x020  (R/W) DMA Peripheral Register */
    uint32_t DPR_Bytes_Single_Type_Request:4;
    uint32_t DPR_rsv1:4;
    uint32_t DPR_Bytes_Burst_Type_Request:4;
    uint32_t DPR_rsv2:20;
    /* Offset: 0x024  (R/W) Remap Address Regsiter */
    uint32_t RAR;
    /* Offset: 0x028  (R/W) Mode Bit Register */
    uint32_t MBR_Mode:8;
    uint32_t MBR_rsv1:24;
    /* Offset: 0x02C  (R/ ) SRAM Fill Level Register */
    uint32_t SFLR_Indirect_Read_Partition:16;
    uint32_t SFLR_Indirect_Write_Partition:16;
    /* Offset: 0x030  (R/W) TX Threshold Register */
    uint32_t TTR_Level:1; // FIXME: related to TX FIFO address width, confirm with HW
    uint32_t TTR_rsv1:31;
    /* Offset: 0x034  (R/W) RX Threshold Register */
    uint32_t RTR_Level:1; // FIXME: related to RX FIFO address width, confirm with HW
    uint32_t RTR_rsv1:31;
    /* Offset: 0x038  (R/W) Write Completion Control Register */
    uint32_t WCCR_Opcode:8;
    uint32_t WCCR_Polling_Bit_Index:3;
    uint32_t WCCR_rsv1:2;
    uint32_t WCCR_Polling_Polarity:1;
    uint32_t WCCR_Polling_Dis:1;
    uint32_t WCCR_rsv2:1;
    uint32_t WCCR_Poll_Count:8;
    uint32_t WCCR_Polling_Repetition_Delay:8;
    /* Offset: 0x03C  (R/W) Polling Expiration Register */
    uint32_t PER;
    /* Offset: 0x040  (R/W) Interrupt Status Register */
    uint32_t ISR_Mode_Fail:1;
    uint32_t ISR_Underflow:1;
    uint32_t ISR_Last_Triggered_Indirect_Completed:1;
    uint32_t ISR_Indirect_Not_Accepted:1;
    uint32_t ISR_Write_Protect_Rejected:1;
    uint32_t ISR_Illegal_AHB_Access:1;
    uint32_t ISR_Indirect_Transfer_Watermark_Level_Breached:1;
    uint32_t ISR_Receive_Overflow:1;
    uint32_t ISR_Small_TX_FIFO_Not_Full:1;
    uint32_t ISR_Small_TX_FIFO_Full:1;
    uint32_t ISR_Small_RX_FIFO_Not_Empty:1;
    uint32_t ISR_Small_RX_FIFO_Full:1;
    uint32_t ISR_Indirect_Read_Partition_of_Sram_Full:1;
    uint32_t ISR_Max_Num_Poll_Cycle_Expired:1;
    uint32_t ISR_rsv1:18;
    /* Offset: 0x044  (R/W) Interrupt Mask Register */
    uint32_t IMR_Mode_Fail:1;
    uint32_t IMR_Underflow:1;
    uint32_t IMR_Last_Triggered_Indirect_Completed:1;
    uint32_t IMR_Indirect_Not_Accepted:1;
    uint32_t IMR_Write_Protect_Rejected:1;
    uint32_t IMR_Illegal_AHB_Access:1;
    uint32_t IMR_Indirect_Transfer_Watermark_Level_Breached:1;
    uint32_t IMR_Receive_Overflow:1;
    uint32_t IMR_Small_TX_FIFO_Not_Full:1;
    uint32_t IMR_Small_TX_FIFO_Full:1;
    uint32_t IMR_Small_RX_FIFO_Not_Empty:1;
    uint32_t IMR_Small_RX_FIFO_Full:1;
    uint32_t IMR_Indirect_Read_Partition_of_Sram_Full:1;
    uint32_t IMR_Max_Num_Poll_Cycle_Expired:1;
    uint32_t IMR_rsv1:18;
    /* Offset: 0x048 ~ 0x04C */
    uint32_t RESERVED0[2];
    /* Offset: 0x050  (R/W) Lower Write Protection Register */
    uint32_t LWPR;
    /* Offset: 0x054  (R/W) Upper Write Protection Register */
    uint32_t UWPR;
    /* Offset: 0x058  (R/W) Write Protection Register */
    uint32_t WPR_Inversion_Bit:1;
    uint32_t WPR_En:1;
    uint32_t WPR_rsv1:30;
    /* Offset: 0x05C */
    uint32_t RESERVED1;
    /* Offset: 0x060  (R/W) Indirect Read Transfer Register */
    uint32_t IRTR_Start_Indirect_Read:1;
    uint32_t IRTR_Cancel_Indirect_Read:1;
    uint32_t IRTR_Indirect_Read_Sts:1;
    uint32_t IRTR_Sram_Full_Sts:1;
    uint32_t IRTR_Two_Indirect_Read_Queued_Sts:1;
    uint32_t IRTR_Indirect_Completion_Sts:1;
    uint32_t IRTR_Indirect_Completion_Num:2;
    uint32_t IRTR_rsv1:24;
    /* Offset: 0x064  (R/W) Indirect Read Transfer Watermark Register */
    uint32_t IRTWR;
    /* Offset: 0x068  (R/W) Indirect Read Transfer Start Address Register */
    uint32_t IRTSAR;
    /* Offset: 0x06C  (R/W) Indirect Read Transfer Number Bytes Register */
    uint32_t IRTNBR;
    /* Offset: 0x070  (R/W) Indirect Write Transfer Register */
    uint32_t IWTR_Start_Indirect_Write:1;
    uint32_t IWTR_Cancel_Indirect_Write:1;
    uint32_t IWTR_Indirect_Write_Sts:1;
    uint32_t IWTR_rsv1:1;
    uint32_t IWTR_Two_Indirect_Write_Queued_Sts:1;
    uint32_t IWTR_Indirect_Completion_Sts:1;
    uint32_t IWTR_Indirect_Completion_Num:2;
    uint32_t IWTR_rsv2:24;
    /* Offset: 0x074  (R/W) Indirect Write Transfer Watermark Register */
    uint32_t IWTWR;
    /* Offset: 0x078  (R/W) Indirect Write Transfer Start Address Register */
    uint32_t IWTSAR;
    /* Offset: 0x07C  (R/W) Indirect Write Transfer Count Register */
    uint32_t IWTCR;
    /* Offset: 0x080  (R/W) Indirect AHB Trigger Address Range Register */
    uint32_t IATARR_Width:4;
    uint32_t IATARR_rsv1:28;
    /* Offset: 0x084 ~ 0x8C */
    uint32_t RESERVED2[3];
    /* Offset: 0x090  (R/W) Flash Command Register */
    uint32_t FCR_Execute:1;
    uint32_t FCR_Execute_Sts:1;
    uint32_t FCR_rsv1:5;
    uint32_t FCR_Dummy_Bytes_Num:5;
    uint32_t FCR_Write_Data_Bytes_Num:3;
    uint32_t FCR_Write_Data_En:1;
    uint32_t FCR_Addr_Bytes_Num:2;
    uint32_t FCR_Mode_Bit_En:1;
    uint32_t FCR_Command_Addr_En:1;
    uint32_t FCR_Read_Data_Bytes_Num:3;
    uint32_t FCR_Read_Data_En:1;
    uint32_t FCR_Command_Opcode:8;
    /* Offset: 0x094  (R/W) Flash Command Address Registers */
    uint32_t FCAR;
    /* Offset: 0x098 ~ 0x9C */
    uint32_t RESERVED3[2];
    /* Offset: 0x0A0  (R/ ) Flash Command Read Data Register (Lower) */
    uint32_t FCRDRL;
    /* Offset: 0x0A4  (R/ ) Flash Command Read Data Register (Upper) */
    uint32_t FCRDRU;
    /* Offset: 0x0A8  (R/W) Flash Command Write Data Register (Lower) */
    uint32_t FCWDRL;
    /* Offset: 0x0AC  (R/W) Flash Command Write Data Register (Upper) */
    uint32_t FCWDRU;
    /* Offset: 0x0B0  (R/ ) Polling Flash Status Register */
    uint32_t PFSR_Flash_Sts:8;
    uint32_t PFSR_Polling_Sts_Valid:1;
    uint32_t PFSR_rsv1:23;
    /* Offset: 0x0B4 */
    uint32_t RESERVED4;
    /* Offset: 0x0B8 */
    uint32_t UPDATAES_EN_Oen_Update_En_Reg:1;
    uint32_t UPDATAES_EN_Device_Size_Update_En_Reg:1;
    uint32_t UPDATAES_EN_Calc_Cs_Update_En_Reg:1;
    uint32_t UPDATAES_EN_rsv1:29;
    /* Offset: 0xBC */
    uint32_t FLASH_OR_PSRAM:4;
    uint32_t FLASH_OR_PSRAM_rsv1:28;
    /* Offset: 0xC0 */
    uint32_t AHB_DEC_ACCESS:1;
    uint32_t AHB_DEC_ACCESS_rsv1:31;
    /* Offset: 0xC4 */
    uint32_t PSRAM_DRIR_Read_Opcode:8;
    uint32_t PSRAM_DRIR_Instruction_Type:2;
    uint32_t PSRAM_DRIR_DDR_Bit_En:1;
    uint32_t PSRAM_DRIR_rsv1:1;
    uint32_t PSRAM_DRIR_Addr_Transfer_Type:2;
    uint32_t PSRAM_DRIR_rsv2:2;
    uint32_t PSRAM_DRIR_Data_Transfer_Type:2;
    uint32_t PSRAM_DRIR_rst3:2;
    uint32_t PSRAM_DRIR_Mode_Bit_En:1;
    uint32_t PSRAM_DRIR_rsv4:3;
    uint32_t PSRAM_DRIR_Dummy_Clock_Cycle_Num:5;
    uint32_t PSRAM_DRIR_rsv5:3;
    /* Offset: 0xC8 (R/W) PSRAM Device Write Instruction Register */
    uint32_t PSRAM_DWIR_Write_Opcode:8;
    uint32_t PSRAM_DWIR_WEL_Dis:1;
    uint32_t PSRAM_DWIR_rsv1:3;
    uint32_t PSRAM_DWIR_Addr_Transfer_Type:2;
    uint32_t PSRAM_DWIR_rsv2:2;
    uint32_t PSRAM_DWIR_Data_Transfer_Type:2;
    uint32_t PSRAM_DWIR_rsv3:6;
    uint32_t PSRAM_DWIR_Dummy_Clock_Cycle_Num:5;
    uint32_t PSRAM_DWIR_rsv4:3;
} S_XIP_Debug_t;
#pragma pop
#endif 

/*
 *************************************************************************
 *						    Typedefs
 *************************************************************************
 */
typedef void (*T_Qspi_Callback)(void);

typedef void (*T_Hal_Qspi_Init_fp)(E_SpiSlave_t eSlvIdx, uint32_t u32Sclk);
typedef void (*T_Hal_Qspi_Config_fp)(S_QSPI_Config_t *ptInit);
typedef void (*T_Hal_Qspi_Peri_Select_fp)(E_SpiSlave_t eSlvIdx);
typedef void (*T_Hal_Qspi_Instruction_Setup_fp)(S_QSPI_Instruction_t *ptInst);
typedef void (*T_Hal_Qspi_Delay_Setup_fp)(uint32_t u32Refclk, uint32_t u32Sclk, S_QSPI_Delay_t *ptDelay);
typedef void (*T_Hal_Qspi_RxSampleDly_Set_fp)(uint8_t u8DlyNum);
typedef void (*T_Hal_Qspi_Command_Exec_fp)(S_QSPI_Command_t *ptCmd);
typedef uint32_t (*T_Hal_Qspi_Dma_Access_fp)(E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size); 
typedef uint32_t (*T_Hal_Qspi_Baudrate_Set_fp)(uint32_t u32Baud);
typedef uint32_t (*T_Hal_Qspi_Baudrate_Get_fp)(void);
typedef uint32_t (*T_Hal_Qpsi_Idle_Poll_fp)(void);
typedef void (*T_Hal_Qspi_Divider_Update_fp)(void);
typedef void (*T_Hal_Qspi_Psram_Instruction_Setup_fp)(S_QSPI_Psram_Instruction_t *ptInst);
typedef void (*T_Hal_Qspi_Psram_RxSampleDly_Set_fp)(uint8_t u8DlyNum);
typedef uint32_t (*T_Hal_Qspi_Psram_Coexistence_Setup_fp)(uint8_t u8Enable, E_SpiSlave_t eSlvIdx);
typedef void (*T_Hal_Qspi_Enable_fp)(uint8_t u8Enable);

/*
 *************************************************************************
 *							Public Variables
 *************************************************************************
 */
extern T_Qspi_Callback g_tHalQspiCallBack;

extern T_Hal_Qspi_Init_fp Hal_Qspi_Init;
extern T_Hal_Qspi_Config_fp Hal_Qspi_Config;
extern T_Hal_Qspi_Peri_Select_fp Hal_Qspi_Peri_Select;
extern T_Hal_Qspi_Instruction_Setup_fp Hal_Qspi_Instruction_Setup;
extern T_Hal_Qspi_Delay_Setup_fp Hal_Qspi_Delay_Setup;
extern T_Hal_Qspi_RxSampleDly_Set_fp Hal_Qspi_RxSampleDly_Set;
extern T_Hal_Qspi_Command_Exec_fp Hal_Qspi_Command_Exec;
extern T_Hal_Qspi_Dma_Access_fp Hal_Qspi_Dma_Access;
extern T_Hal_Qspi_Baudrate_Set_fp Hal_Qspi_Baudrate_Set;
extern T_Hal_Qspi_Baudrate_Get_fp Hal_Qspi_Baudrate_Get;
extern T_Hal_Qpsi_Idle_Poll_fp Hal_Qpsi_Idle_Poll;
extern T_Hal_Qspi_Divider_Update_fp Hal_Qspi_Divider_Update;
extern T_Hal_Qspi_Psram_Instruction_Setup_fp Hal_Qspi_Psram_Instruction_Setup;
extern T_Hal_Qspi_Psram_RxSampleDly_Set_fp Hal_Qspi_Psram_RxSampleDly_Set;
extern T_Hal_Qspi_Psram_Coexistence_Setup_fp Hal_Qspi_Psram_Coexistence_Setup;
extern T_Hal_Qspi_Enable_fp Hal_Qspi_Enable;

/*
 *************************************************************************
 *							Public Functions
 *************************************************************************
 */
void Hal_Qspi_Init_impl(E_SpiSlave_t eSlvIdx, uint32_t u32Sclk);
void Hal_Qspi_Config_impl(S_QSPI_Config_t *ptInit);
void Hal_Qspi_Peri_Select_impl(E_SpiSlave_t eSlvIdx);
void Hal_Qspi_Instruction_Setup_impl(S_QSPI_Instruction_t *ptInst);
void Hal_Qspi_Delay_Setup_impl(uint32_t u32Refclk, uint32_t u32Sclk, S_QSPI_Delay_t *ptDelay);
void Hal_Qspi_RxSampleDly_Set_impl(uint8_t u8DlyNum);
void Hal_Qspi_Command_Exec_impl(S_QSPI_Command_t *ptCmd);
uint32_t Hal_Qspi_Dma_Access_impl(E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size);
uint32_t Hal_Qspi_Baudrate_Set_impl(uint32_t u32Baud);
uint32_t Hal_Qspi_Baudrate_Get_impl(void);
uint32_t Hal_Qpsi_Idle_Poll_impl(void);
void Hal_Qspi_Divider_Update_impl(void);
void Hal_Qspi_Psram_Instruction_Setup_impl(S_QSPI_Psram_Instruction_t *ptInst);
void Hal_Qspi_Psram_RxSampleDly_Set_impl(uint8_t u8DlyNum);
uint32_t Hal_Qspi_Psram_Coexistence_Setup_impl(uint8_t u8Enable, E_SpiSlave_t eSlvIdx);
void Hal_Qspi_Enable_impl(uint8_t u8Enable);

#include "hal_qspi_patch.h"
#endif /* __HAL_QSPI_H__ */
