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
 *  @file hal_dma.h
 *  @brief API to control DMA
 ******************************************************************************/

#ifndef __HAL_DMA_H__
#define __HAL_DMA_H__

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>


/*
 *************************************************************************
 *						    Definitions and Macros
 *************************************************************************
 */
#define MAX_DMA_XFER_NUM         4095 // max number of single DMA transfer is 4095

#define DMA_TIMEOUT_MS           1000

/* Interrupt Registers */
/* Bitmap of Channel for Interrupt Register */
#define DMA_BMP_INT_CH0         (0x01)
#define DMA_BMP_INT_CH1         (0x02)
#define DMA_BMP_INT_CH2         (0x04)
#define DMA_BMP_INT_CH3         (0x08)

/* Bitmap of Type for Interrupt Register */
#define DMA_TFR_POS             (0)
#define DMA_BLK_POS             (1)
#define DMA_SRC_TRAN_POS        (2)
#define DMA_DST_TRAN_POS        (3)
#define DMA_ERR_POS             (4)
#define DMA_BMP_TFR             (1 << DMA_TFR_POS)
#define DMA_BMP_BLK             (1 << DMA_BLK_POS)
#define DMA_BMP_SRC_TRAN        (1 << DMA_SRC_TRAN_POS)
#define DMA_BMP_DST_TRAN        (1 << DMA_DST_TRAN_POS)
#define DMA_BMP_ERR             (1 << DMA_ERR_POS)

/*
 *************************************************************************
 *						    Enums and Structures
 *************************************************************************
 */
typedef enum {
    APS_DMA_ITF_UART0_TX = 0,       /* 0  */
    APS_DMA_ITF_UART0_RX,           /* 1  */
    APS_DMA_ITF_UART1_TX,           /* 2  */
    APS_DMA_ITF_UART1_RX,           /* 3  */
    APS_DMA_ITF_I2C_TX,             /* 4  */
    APS_DMA_ITF_I2C_RX,             /* 5  */
    APS_DMA_ITF_SPI1_TX,            /* 6  */
    APS_DMA_ITF_SPI1_RX,            /* 7  */
    APS_DMA_ITF_SPI2_TX,            /* 8  */
    APS_DMA_ITF_SPI2_RX,            /* 9  */
    APS_DMA_ITF_I2S_TX,             /* 10 */
    APS_DMA_ITF_I2S_RX,             /* 11 */
    APS_DMA_ITF_SPI3_TX,            /* 12 */
    APS_DMA_ITF_SPI3_RX,            /* 13 */
    APS_DMA_ITF_PDM_RX,             /* 14 */
}E_APS_DMA_HW_ITF;


typedef enum
{
    DMA_Channel_0   = 0,
    DMA_Channel_1   = 1,
    DMA_Channel_2   = 2,
    DMA_Channel_3   = 3,
    
    DMA_Channel_Max = 4,
} E_DMA_CHANNEL; 

// SRC_TR_WIDTH & DST_TR_WIDTH of CTLx
typedef enum
{
    DMA_TR_WIDTH_8_bits    = 0x0,
    DMA_TR_WIDTH_16_bits   = 0x1,
    DMA_TR_WIDTH_32_bits   = 0x2,
/*  
    // not support
    DMA_TR_WIDTH_64_bits   = 0x3, 
    DMA_TR_WIDTH_128_bits  = 0x4,
    DMA_TR_WIDTH_256_bits  = 0x5,
    DMA_TR_WIDTH_256_bits2 = 0x6,
    DMA_TR_WIDTH_256_bits3 = 0x7,
*/    
    DMA_TR_WIDTH_Max       = 0x3,
} E_DMA_TR_WIDTH;

// SINC & DINC of CTLx
typedef enum
{
    DMA_INC_Increment  = 0x0,
    DMA_INC_Decrement  = 0x1,
    DMA_INC_No_Change  = 0x2,
    DMA_INC_No_Change2 = 0x3,
    
    DMA_INC_Max        = 0x4,
} E_DMA_INC;

// SRC_MSIZE & DEST_MSIZE of CTLx
typedef enum
{
    DMA_MSIZE_1   = 0x0, /* Number of data items to be transferred is   1 */
    DMA_MSIZE_4   = 0x1, /* Number of data items to be transferred is   4 */
    DMA_MSIZE_8   = 0x2, /* Number of data items to be transferred is   8 */
    DMA_MSIZE_16  = 0x3, /* Number of data items to be transferred is  16 */
    DMA_MSIZE_32  = 0x4, /* Number of data items to be transferred is  32 */
    DMA_MSIZE_64  = 0x5, /* Number of data items to be transferred is  64 */
    DMA_MSIZE_128 = 0x6, /* Number of data items to be transferred is 128 */
    DMA_MSIZE_256 = 0x7, /* Number of data items to be transferred is 256 */
    
    DMA_MSIZE_Max = 0x8,
} E_DMA_MSIZE;

// TT_FC of CTLx
typedef enum 
{
    DMA_TT_m2m_FC_Dma      = 0x0, /* Memory to Memory and Flow Controller is DW_ahb_dmac                    */
    DMA_TT_m2p_FC_Dma      = 0x1, /* Memory to Peripheral and Flow Controller is DW_ahb_dmac                */
    DMA_TT_p2m_FC_Dma      = 0x2, /* Peripheral to Memory and Flow Controller is DW_ahb_dmac                */
    DMA_TT_p2p_FC_Dma      = 0x3, /* Peripheral to Peripheral and Flow Controller is DW_ahb_dmac            */
    DMA_TT_p2m_FC_Peri     = 0x4, /* Peripheral to Memory and Flow Controller is Peripheral                 */
    DMA_TT_p2p_FC_Src_Peri = 0x5, /* Peripheral to Peripheral and Flow Controller is Source Peripheral      */
    DMA_TT_m2p_FC_Peri     = 0x6, /* Memory to Peripheral and Flow Controller is Peripheral                 */
    DMA_TT_p2p_FC_Dst_Peri = 0x7, /* Peripheral to Peripheral and Flow Controller is Destination Peripheral */
    
    DMA_TT_FC_Max          = 0x8,
} E_DMA_TT_FC;

// CH_PRIOR of CFGx
typedef enum
{
    DMA_CH_PRIOR_Lowest  = 0x0,
    DMA_CH_PRIOR_1       = 0x1,
    DMA_CH_PRIOR_2       = 0x2,
    DMA_CH_PRIOR_3       = 0x3,
    DMA_CH_PRIOR_4       = 0x4,
    DMA_CH_PRIOR_5       = 0x5,
    DMA_CH_PRIOR_6       = 0x6,
    DMA_CH_PRIOR_Highest = 0x7,
    
    DMA_CH_PRIOR_Max     = 0x8,
} E_DMA_CH_PRIOR;

typedef struct
{
    uint32_t CTL_L_INT_EN:1;
    uint32_t CTL_L_DST_TR_WIDTH:3;
    uint32_t CTL_L_SRC_TR_WIDTH:3;
    uint32_t CTL_L_DINC:2;
    uint32_t CTL_L_SINC:2;
    uint32_t CTL_L_DEST_MSIZE:3;
    uint32_t CTL_L_SRC_MSIZE:3;
    uint32_t CTL_L_SRC_GATHER_EN:1;
    uint32_t CTL_L_DST_SCATTER_EN:1;
    uint32_t Rsvd_CTL:1;
    uint32_t CTL_L_TT_FC:3;
    uint32_t CTL_L_DMS:2;
    uint32_t CTL_L_SMS:2;
    uint32_t CTL_L_LLP_DST_EN:1;
    uint32_t CTL_L_LLP_SRC_EN:1;
    uint32_t Rsvd_1_CTL:3;
} S_DMA_CTL_L_t;

typedef struct
{
    uint32_t CTL_H_BLOCK_TS:12;
    uint32_t CTL_H_DONE:1;
    uint32_t Rsvd_CTL:19;
} S_DMA_CTL_H_t;


/* LLI Structure */
typedef struct
{
    uint32_t SAR;
    uint32_t DAR;
    uint32_t LLP;
    S_DMA_CTL_L_t CTL_L;
    S_DMA_CTL_H_t CTL_H;
    uint32_t SSTAT; // DMAH_CHx_STAT_SRC = 1 for APS, x = 1~3
    uint32_t DSTAT;
} S_DMA_LinkListItem_t;

// TODO: review this structure
typedef struct
{
    // SARx
    uint32_t Sar;                 /* Current Source address of DMA transfer */
    // DARx
    uint32_t Dar;                 /* Current Destination address of DMA transfer */
    // LLPx 
    uint32_t Loc:30;              /* Starting Address in Memory of next LLI */
    // CTLx_L
    E_DMA_TR_WIDTH eDstTrWidth:3; /* Destination Tranfer Width */
    E_DMA_TR_WIDTH eSrcTrWidth:3; /* Source Tranfer Width */
    E_DMA_INC eDinc:3;            /* Destination Address Increment */
    E_DMA_INC eSinc:3;            /* Source Address Increment */
    E_DMA_MSIZE eDestMsize:4;     /* Destination Burst Transaction Length */
    E_DMA_MSIZE eSrcMsize:4;      /* Source Burst Transaction Length */
    E_DMA_TT_FC eTtfc;            /* Transfer Type and Flow Control */
    uint32_t SrcGatherEn:1;       /* Source gather enable */
    uint32_t DstScatterEn:1;      /* Destination scatter enable */
    uint32_t LlpDstEn:1;          /* Block chaining is enabled on the destination side */
    uint32_t LlpSrcEn:1;          /* Block chaining is enabled on the source side */
    // CTLx_H
    uint32_t BlockTs:12;          /* Block Transfer Size */
    uint32_t Done:1;              /* Done bit */
    // SSTAT
    uint32_t SrcStsAddr;          /* Source Status Information */
    // DSTAT
    uint32_t DstStsAddr;          /* Destination Status Information */
    // SSTATAR
    uint32_t Sstatar;             /* Source Status Address Register */
    // DSTATAR
    uint32_t Dstatar;             /* Destination Status Address Register */
    // CFGx_L
    E_DMA_CH_PRIOR ePrior;        /* Channel Priority */
    uint32_t HsSelDst:1;          /* Destination Software or Hardware Handshaking Select */
    uint32_t HsSelSrc:1;          /* Source Software or Hardware Handshaking Select */
    uint32_t DstHsPol:1;          /* Destination Handshaking Interface Polarity */
    uint32_t SrcHsPol:1;          /* Source Handshaking Interface Polarity */
    uint32_t ReloadSrc:1;         /* Automatic Source Reload */
    uint32_t ReloadDst:1;         /* Automatic Destination Reload */
    // CFGx_H
    uint32_t Fcmode:1;            /* Flow Control Mode */
    uint32_t FifoMode:1;          /* FIFO Mode Select */
    uint32_t Protctl:3;           /* Protection Control */
    uint32_t DsUpdEn:1;           /* Destination Status Update Enable */
    uint32_t SsUpdEn:1;           /* Source Status Update Enable */
    uint32_t SrcPer:4;            /* Source Hardware Interface */
    uint32_t DestPer:4;           /* Destination Hardware Interface */
    // SGRx
    uint32_t Sgi:20;              /* Source Gather Interval */
    uint32_t Sgc:12;              /* Source Gather Count */
    // DSRx
    uint32_t Dsi:20;              /* Destination Scatter Interval */
    uint32_t Dsc:12;              /* Destination Scatter Count */
} S_DMA_Init_t;

typedef struct
{
    __IOM uint32_t SAR;         /* Offset: 0x000  (R/W) Source Address                      */
          uint32_t RESERVED0;   /* Offset: 0x004                                            */
    __IOM uint32_t DAR;         /* Offset: 0x008  (R/W) Destination Address Register        */
          uint32_t RESERVED1;   /* Offset: 0x00C                                            */
    __IOM uint32_t LLP;         /* Offset: 0x010  (R/W) Linked List Pointer Register        */
          uint32_t RESERVED2;   /* Offset: 0x014                                            */
    __IOM uint32_t CTL_L;       /* Offset: 0x018  (R/W) Control Register (LOW)              */
    __IOM uint32_t CTL_H;       /* Offset: 0x01C  (R/W) Control Register (HIGH)             */
    __IOM uint32_t SSTAT;       /* Offset: 0x020  (R/W) Source Status Register              */
          uint32_t RESERVED3;   /* Offset: 0x024                                            */
    __IOM uint32_t DSTAT;       /* Offset: 0x028  (R/W) Destination Status Register         */
          uint32_t RESERVED4;   /* Offset: 0x02C                                            */
    __IOM uint32_t SSTATAR;     /* Offset: 0x030  (R/W) Source Status Address Register      */
          uint32_t RESERVED5;   /* Offset: 0x034                                            */
    __IOM uint32_t DSTATAR;     /* Offset: 0x038  (R/W) Destination Status Address Register */
          uint32_t RESERVED6;   /* Offset: 0x03C                                            */
    __IOM uint32_t CFG_L;       /* Offset: 0x040  (R/W) Configuration Register (LOW)        */
    __IOM uint32_t CFG_H;       /* Offset: 0x044  (R/W) Configuration Register (HIGH)       */
    __IOM uint32_t SGR;         /* Offset: 0x048  (R/W) Source Gather Register              */
          uint32_t RESERVED7;   /* Offset: 0x04C                                            */
    __IOM uint32_t DSR;         /* Offset: 0x050  (R/W) Destination Scatter Register        */
          uint32_t RESERVED8;   /* Offset: 0x054                                            */ 
} S_DMA_Channel_t;

#if 1
// debug usage
// add ((S_DMA_Debug_t *) DMA_BASE) to Watch Windows
#pragma push
#pragma anon_unions


typedef struct
{
    uint32_t Rsvd_CFG:5;
    uint32_t CFG0_L_CH_PRIOR:3;
    uint32_t CFG0_L_CH_SUSP:1;
    uint32_t CFG0_L_FIFO_EMPTY:1;
    uint32_t CFG0_L_HS_SEL_DST:1;
    uint32_t CFG0_L_HS_SEL_SRC:1;
    uint32_t CFG0_L_LOCK_CH_L:2;
    uint32_t CFG0_L_LOCK_B_L:2;
    uint32_t CFG0_L_CH:1;
    uint32_t CFG0_L_LOCK_B:1;
    uint32_t CFG0_L_DST_HS_POL:1;
    uint32_t CFG0_L_SRC_HS_POL:1;
    uint32_t CFG0_L_MAX_ABRST:10;
    uint32_t CFG0_L_RELOAD_SRC:1;
    uint32_t CFG0_L_RELOAD_DST:1;
} S_DMA_CFG_L_t;

typedef struct
{
    uint32_t CFG0_H_FCMODE:1;
    uint32_t CFG0_H_FIFOMODE:1;
    uint32_t CFG0_H_PROTCTL:3;
    uint32_t CFG0_H_DS_UPD_EN:1;
    uint32_t CFG0_H_SRC_PER:4;
    uint32_t CFG0_H_DEST_PER:4;
    uint32_t Rsvd_1_CFG:17;
} S_DMA_CFG_H_t;

typedef struct
{
    /* Channel 0 */
    uint32_t SAR0;                /* Offset: 0x000  (R/W) Source Address for Channel 0                    */
    uint32_t RESERVED0;           /* Offset: 0x004                                                        */
    uint32_t DAR0;                /* Offset: 0x008  (R/W) Destination Address Register for Channel 0      */
    uint32_t RESERVED1;           /* Offset: 0x00C                                                        */
    uint32_t LLP0_LMS:2;          /* Offset: 0x010  (R/W) Linked List Pointer Register for Channel 0      */
    uint32_t LLP0_LOC:30;
    uint32_t RESERVED2;           /* Offset: 0x014                                                        */
    uint32_t CTL0_L_INT_EN:1;     /* Offset: 0x018  (R/W) Control Register for Channel 0 (LOW)            */
    uint32_t CTL0_L_DST_TR_WIDTH:3;
    uint32_t CTL0_L_SRC_TR_WIDTH:3;
    uint32_t CTL0_L_DINC:2;
    uint32_t CTL0_L_SINC:2;
    uint32_t CTL0_L_DEST_MSIZE:3;
    uint32_t CTL0_L_SRC_MSIZE:3;
    uint32_t CTL0_L_SRC_GATHER_EN:1;
    uint32_t CTL0_L_DST_SCATTER_EN:1;
    uint32_t Rsvd_CTL:1;
    uint32_t CTL0_L_TT_FC:3;
    uint32_t CTL0_L_DMS:2;
    uint32_t CTL0_L_SMS:2;
    uint32_t CTL0_L_LLP_DST_EN:1;
    uint32_t CTL0_L_LLP_SRC_EN:1;
    uint32_t Rsvd_1_CTL:3;
    uint32_t CTL0_H_BLOCK_TS:12;  /* Offset: 0x01C  (R/W) Control Register for Channel 0 (HIGH)           */
    uint32_t CTL0_H_DONE:1;
    uint32_t Rsvd_3_CTL:19;
    uint32_t SSTAT0;              /* Offset: 0x020  (R/W) Source Status Register for Channel 0            */
    uint32_t RESERVED3;           /* Offset: 0x024                                                        */
    uint32_t DSTAT0;              /* Offset: 0x028  (R/W) Destination Status Register for Channel 0       */
    uint32_t RESERVED4;           /* Offset: 0x02C                                                        */
    uint32_t SSTATAR0;            /* Offset: 0x030  (R/W) Source Status Address Register for Channel 0    */
    uint32_t RESERVED5;           /* Offset: 0x034                                                        */
    uint32_t DSTATAR0;            /* Offset: 0x038  (R/W) Destination Status Address Register for Channel 0 */
    uint32_t RESERVED6;           /* Offset: 0x03C                                                        */
    uint32_t Rsvd_CFG:5;          /* Offset: 0x040  (R/W) Configuration Register for Channel 0 (LOW)      */
    uint32_t CFG0_L_CH_PRIOR:3;
    uint32_t CFG0_L_CH_SUSP:1;
    uint32_t CFG0_L_FIFO_EMPTY:1;
    uint32_t CFG0_L_HS_SEL_DST:1;
    uint32_t CFG0_L_HS_SEL_SRC:1;
    uint32_t CFG0_L_LOCK_CH_L:2;
    uint32_t CFG0_L_LOCK_B_L:2;
    uint32_t CFG0_L_LOCK_CH:1;
    uint32_t CFG0_L_LOCK_B:1;
    uint32_t CFG0_L_DST_HS_POL:1;
    uint32_t CFG0_L_SRC_HS_POL:1;
    uint32_t CFG0_L_MAX_ABRST:10;
    uint32_t CFG0_L_RELOAD_SRC:1;
    uint32_t CFG0_L_RELOAD_DST:1;
    uint32_t CFG0_H_FCMODE:1;     /* Offset: 0x044  (R/W) Configuration Register for Channel 0 (HIGH)     */
    uint32_t CFG0_H_FIFOMODE:1;
    uint32_t CFG0_H_PROTCTL:3;
    uint32_t CFG0_H_DS_UPD_EN:1;
    uint32_t CFG0_H_SS_UPD_EN:1;
    uint32_t CFG0_H_SRC_PER:4;
    uint32_t CFG0_H_DEST_PER:4;
    uint32_t Rsvd_3_CFG:17;
    uint32_t SGR0_SGI:20;         /* Offset: 0x048  (R/W) Source Gather Register for Channel 0            */
    uint32_t SGR0_SGC:12;
    uint32_t RESERVED7;           /* Offset: 0x04C                                                        */
    uint32_t DSR0_DSI:20;         /* Offset: 0x050  (R/W) Destination Scatter Register for Channel 0      */
    uint32_t DSR0_DSC:12;
    uint32_t RESERVED8;           /* Offset: 0x054                                                        */

    /* Channel 1 */
    uint32_t SAR1;                /* Offset: 0x058  (R/W) Source Address for Channel 1                    */
    uint32_t RESERVED9;           /* Offset: 0x05C                                                        */
    uint32_t DAR1;                /* Offset: 0x060  (R/W) Destination Address Register for Channel 1      */
    uint32_t RESERVED10;          /* Offset: 0x064                                                        */
    uint32_t LLP1_LMS:2;          /* Offset: 0x068  (R/W) Linked List Pointer Register for Channel 1      */
    uint32_t LLP1_LOC:30;
    uint32_t RESERVED11;          /* Offset: 0x06C                                                        */
    uint32_t CTL1_L_INT_EN:1;     /* Offset: 0x070  (R/W) Control Register for Channel 1 (LOW)            */
    uint32_t CTL1_L_DST_TR_WIDTH:3;
    uint32_t CTL1_L_SRC_TR_WIDTH:3;
    uint32_t CTL1_L_DINC:2;
    uint32_t CTL1_L_SINC:2;
    uint32_t CTL1_L_DEST_MSIZE:3;
    uint32_t CTL1_L_SRC_MSIZE:3;
    uint32_t CTL1_L_SRC_GATHER_EN:1;
    uint32_t CTL1_L_DST_SCATTER_EN:1;
    uint32_t Rsvd_CTL1:1;
    uint32_t CTL1_L_TT_FC:3;
    uint32_t CTL1_L_DMS:2;
    uint32_t CTL1_L_SMS:2;
    uint32_t CTL1_L_LLP_DST_EN:1;
    uint32_t CTL1_L_LLP_SRC_EN:1;
    uint32_t Rsvd_1_CTL1:3;
    uint32_t CTL1_H_BLOCK_TS:12;  /* Offset: 0x074  (R/W) Control Register for Channel 1 (HIGH)           */
    uint32_t CTL1_H_DONE:1;
    uint32_t Rsvd_3_CTL1:19;
    uint32_t SSTAT1;              /* Offset: 0x078  (R/W) Source Status Register for Channel 1            */
    uint32_t RESERVED12;          /* Offset: 0x07C                                                        */
    uint32_t DSTAT1;              /* Offset: 0x080  (R/W) Destination Status Register for Channel 1       */
    uint32_t RESERVED13;          /* Offset: 0x084                                                        */
    uint32_t SSTATAR1;            /* Offset: 0x088  (R/W) Source Status Address Register for Channel 1    */
    uint32_t RESERVED14;          /* Offset: 0x08C                                                        */
    uint32_t DSTATAR1;            /* Offset: 0x090  (R/W) Destination Status Address Register for Channel 1 */
    uint32_t RESERVED15;          /* Offset: 0x094                                                        */
    uint32_t Rsvd_CFG1:5;         /* Offset: 0x098  (R/W) Configuration Register for Channel 1 (LOW)    */
    uint32_t CFG1_L_CH_PRIOR:3;
    uint32_t CFG1_L_CH_SUSP:1;
    uint32_t CFG1_L_FIFO_EMPTY:1;
    uint32_t CFG1_L_HS_SEL_DST:1;
    uint32_t CFG1_L_HS_SEL_SRC:1;
    uint32_t CFG1_L_LOCK_CH_L:2;
    uint32_t CFG1_L_LOCK_B_L:2;
    uint32_t CFG1_L_CH:1;
    uint32_t CFG1_L_LOCK_B:1;
    uint32_t CFG1_L_DST_HS_POL:1;
    uint32_t CFG1_L_SRC_HS_POL:1;
    uint32_t CFG1_L_MAX_ABRST:10;
    uint32_t CFG1_L_RELOAD_SRC:1;
    uint32_t CFG1_L_RELOAD_DST:1;
    uint32_t CFG1_H_FCMODE:1;     /* Offset: 0x09C  (R/W) Configuration Register for Channel 1 (HIGH)     */
    uint32_t CFG1_H_FIFOMODE:1;
    uint32_t CFG1_H_PROTCTL:3;
    uint32_t CFG1_H_DS_UPD_EN:1;
    uint32_t CFG1_H_SS_UPD_EN:1;
    uint32_t CFG1_H_SRC_PER:4;
    uint32_t CFG1_H_DEST_PER:4;
    uint32_t Rsvd_3_CFG1:17;
    uint32_t SGR1_SGI:20;         /* Offset: 0x0A0 (R/W) Source Gather Register for Channel 1             */
    uint32_t SGR1_SGC:12;
    uint32_t RESERVED16;          /* Offset: 0x0A4                                                        */
    uint32_t DSR1_DSI:20;         /* Offset: 0x0A8  (R/W) Destination Scatter Register for Channel 1      */
    uint32_t DSR1_DSC:12;
    uint32_t RESERVED17;          /* Offset: 0x0AC                                                        */

    /* Channel 2 */
    uint32_t SAR2;                /* Offset: 0x0B0  (R/W) Source Address for Channel 2                    */
    uint32_t RESERVED18;          /* Offset: 0x0B4                                                        */
    uint32_t DAR2;                /* Offset: 0x0B8  (R/W) Destination Address Register for Channel 2      */
    uint32_t RESERVED19;          /* Offset: 0x0BC                                                        */
    uint32_t LLP2_LMS:2;          /* Offset: 0x0C0  (R/W) Linked List Pointer Register for Channel 2      */
    uint32_t LLP2_LOC:30;    
    uint32_t RESERVED20;          /* Offset: 0x0C4                                                        */
    uint32_t CTL2_L_INT_EN:1;     /* Offset: 0x0C8  (R/W) Control Register for Channel 2 (LOW)            */
    uint32_t CTL2_L_DST_TR_WIDTH:3;
    uint32_t CTL2_L_SRC_TR_WIDTH:3;
    uint32_t CTL2_L_DINC:2;
    uint32_t CTL2_L_SINC:2;
    uint32_t CTL2_L_DEST_MSIZE:3;
    uint32_t CTL2_L_SRC_MSIZE:3;
    uint32_t CTL2_L_SRC_GATHER_EN:1;
    uint32_t CTL2_L_DST_SCATTER_EN:1;
    uint32_t Rsvd_CTL2:1;
    uint32_t CTL2_L_TT_FC:3;
    uint32_t CTL2_L_DMS:2;
    uint32_t CTL2_L_SMS:2;
    uint32_t CTL2_L_LLP_DST_EN:1;
    uint32_t CTL2_L_LLP_SRC_EN:1;
    uint32_t CTL2_H_BLOCK_TS:12;  /* Offset: 0x0CC  (R/W) Control Register for Channel 2 (HIGH)           */
    uint32_t CTL2_H_DONE:1;
    uint32_t Rsvd_3_CTL2:19;
    uint32_t SSTAT2;              /* Offset: 0x0D0  (R/W) Source Status Register for Channel 2            */
    uint32_t RESERVED21;          /* Offset: 0x0D4                                                        */
    uint32_t DSTAT2;              /* Offset: 0x0D8  (R/W) Destination Status Register for Channel 2       */
    uint32_t RESERVED22;          /* Offset: 0x0DC                                                        */
    uint32_t SSTATAR2;            /* Offset: 0x0E0  (R/W) Source Status Address Register for Channel 2    */
    uint32_t RESERVED23;          /* Offset: 0x0E4                                                        */
    uint32_t DSTATAR2;            /* Offset: 0x0E8  (R/W) Destination Status Address Register for Channel 2 */
    uint32_t RESERVED24;          /* Offset: 0x0EC                                                        */
    uint32_t Rsvd_CFG2:5;         /* Offset: 0x0F0  (R/W) Configuration Register for Channel 2 (LOW)      */
    uint32_t CFG2_L_CH_PRIOR:3;
    uint32_t CFG2_L_CH_SUSP:1;
    uint32_t CFG2_L_FIFO_EMPTY:1;
    uint32_t CFG2_L_HS_SEL_DST:1;
    uint32_t CFG2_L_HS_SEL_SRC:1;
    uint32_t CFG2_L_LOCK_CH_L:2;
    uint32_t CFG2_L_LOCK_B_L:2;
    uint32_t CFG2_L_CH:1;
    uint32_t CFG2_L_LOCK_B:1;
    uint32_t CFG2_L_DST_HS_POL:1;
    uint32_t CFG2_L_SRC_HS_POL:1;
    uint32_t CFG2_L_MAX_ABRST:10;
    uint32_t CFG2_L_RELOAD_SRC:1;
    uint32_t CFG2_L_RELOAD_DST:1;
    uint32_t CFG2_H_FCMODE:1;     /* Offset: 0x0F4  (R/W) Configuration Register for Channel 2 (HIGH)     */
    uint32_t CFG2_H_FIFOMODE:1;
    uint32_t CFG2_H_PROTCTL:3;
    uint32_t CFG2_H_DS_UPD_EN:1;
    uint32_t CFG2_H_SS_UPD_EN:1;
    uint32_t CFG2_H_SRC_PER:4;
    uint32_t CFG2_H_DEST_PER:4;
    uint32_t Rsvd_3_CFG2:17;
    uint32_t SGR2_SGI:20;         /* Offset: 0x0F8  (R/W) Source Gather Register for Channel 2            */
    uint32_t SGR2_SGC:12;
    uint32_t RESERVED25;          /* Offset: 0x0FC                                                        */
    uint32_t DSR2_DSI:20;         /* Offset: 0x100  (R/W) Destination Scatter Register for Channel 2      */
    uint32_t DSR2_DSC:12;
    uint32_t RESERVED26;          /* Offset: 0x104                                                        */

    /* Channel 3 */
    uint32_t SAR3;                /* Offset: 0x108  (R/W) Source Address for Channel 3                    */
    uint32_t RESERVED27;          /* Offset: 0x10C                                                        */
    uint32_t DAR3;                /* Offset: 0x110  (R/W) Destination Address Register for Channel 3      */
    uint32_t RESERVED28;          /* Offset: 0x114                                                        */
    uint32_t LLP3_LMS:2;          /* Offset: 0x118  (R/W) Linked List Pointer Register for Channel 3      */
    uint32_t LLP3_LOC:30;
    uint32_t RESERVED29;          /* Offset: 0x11C                                                        */
    uint32_t CTL3_L_INT_EN:1;     /* Offset: 0x120  (R/W) Control Register for Channel 3 (LOW)            */
    uint32_t CTL3_L_DST_TR_WIDTH:3;
    uint32_t CTL3_L_SRC_TR_WIDTH:3;
    uint32_t CTL3_L_DINC:2;
    uint32_t CTL3_L_SINC:2;
    uint32_t CTL3_L_DEST_MSIZE:3;
    uint32_t CTL3_L_SRC_MSIZE:3;
    uint32_t CTL3_L_SRC_GATHER_EN:1;
    uint32_t CTL3_L_DST_SCATTER_EN:1;
    uint32_t Rsvd_CTL3:1;
    uint32_t CTL3_L_TT_FC:3;
    uint32_t CTL3_L_DMS:2;
    uint32_t CTL3_L_SMS:2;
    uint32_t CTL3_L_LLP_DST_EN:1;
    uint32_t CTL3_L_LLP_SRC_EN:1;
    uint32_t Rsvd_1_CTL3:3;
    uint32_t CTL3_H_BLOCK_TS:12;  /* Offset: 0x124  (R/W) Control Register for Channel 3 (HIGH)           */
    uint32_t CTL3_H_DONE:1;
    uint32_t Rsvd_3_CTL3:19;
    uint32_t SSTAT3;              /* Offset: 0x128  (R/W) Source Status Register for Channel 3            */
    uint32_t RESERVED30;          /* Offset: 0x12C                                                        */
    uint32_t DSTAT3;              /* Offset: 0x130  (R/W) Destination Status Register for Channel 3       */
    uint32_t RESERVED31;          /* Offset: 0x134                                                        */
    uint32_t SSTATAR3;            /* Offset: 0x138  (R/W) Source Status Address Register for Channel 3    */
    uint32_t RESERVED32;          /* Offset: 0x13C                                                        */
    uint32_t DSTATAR3;            /* Offset: 0x140  (R/W) Destination Status Address Register for Channel 3 */
    uint32_t RESERVED33;          /* Offset: 0x144                                                        */
    uint32_t Rsvd_CFG3:5;         /* Offset: 0x148  (R/W) Configuration Register for Channel 3 (LOW)      */
    uint32_t CFG3_L_CH_PRIOR:3;
    uint32_t CFG3_L_CH_SUSP:1;
    uint32_t CFG3_L_FIFO_EMPTY:1;
    uint32_t CFG3_L_HS_SEL_DST:1;
    uint32_t CFG3_L_HS_SEL_SRC:1;
    uint32_t CFG3_L_LOCK_CH_L:2;
    uint32_t CFG3_L_LOCK_B_L:2;
    uint32_t CFG3_L_CH:1;
    uint32_t CFG3_L_LOCK_B:1;
    uint32_t CFG3_L_DST_HS_POL:1;
    uint32_t CFG3_L_SRC_HS_POL:1;
    uint32_t CFG3_L_MAX_ABRST:10;
    uint32_t CFG3_L_RELOAD_SRC:1;
    uint32_t CFG3_L_RELOAD_DST:1;
    uint32_t CFG3_H_FCMODE:1;     /* Offset: 0x14C  (R/W) Configuration Register for Channel 3 (HIGH)     */
    uint32_t CFG3_H_FIFOMODE:1;
    uint32_t CFG3_H_PROTCTL:3;
    uint32_t CFG3_H_DS_UPD_EN:1;
    uint32_t CFG3_H_SS_UPD_EN:1;
    uint32_t CFG3_H_SRC_PER:4;
    uint32_t CFG3_H_DEST_PER:4;
    uint32_t Rsvd_3_CFG3:17;
    uint32_t SGR3_SGI:20;         /* Offset: 0x150  (R/W) Source Gather Register for Channel 3            */
    uint32_t SGR3_SGC:12;    
    uint32_t RESERVED34;          /* Offset: 0x154                                                        */
    uint32_t DSR3_DSI:20;         /* Offset: 0x158  (R/W) Destination Scatter Register for Channel 3      */
    uint32_t DSR3_DSC:12;
    uint32_t RESERVED35[89];      /* Offset: 0x15C ~ 2BC                                                  */          

    /* Interrupt */
    uint32_t RAW_TFR;             /* Offset: 0x2C0  (R/W) Raw Status for IntTfr Interrupt                 */
    uint32_t RESERVED36;          /* Offset: 0x2C4                                                        */
    uint32_t RAW_BLK;             /* Offset: 0x2C8  (R/W) Raw Status for IntBlock Interrupt               */
    uint32_t RESERVED37;          /* Offset: 0x2CC                                                        */
    uint32_t RAW_SRC_TRAN;        /* Offset: 0x2D0  (R/W) Raw Status for IntSrcTran Interrupt             */
    uint32_t RESERVED38;          /* Offset: 0x2D4                                                        */
    uint32_t RAW_DST_TRAN;        /* Offset: 0x2D8  (R/W) Raw Status for IntDstTran Interrupt             */
    uint32_t RESERVED39;          /* Offset: 0x2DC                                                        */
    uint32_t RAW_ERR;             /* Offset: 0x2E0  (R/W) Raw Status for IntErr Interrupt                 */
    uint32_t RESERVED40;          /* Offset: 0x2E4                                                        */
    uint32_t STS_TFR;             /* Offset: 0x2E8  (R/ ) Status for IntTfr Interrupt                     */
    uint32_t RESERVED41;          /* Offset: 0x2EC                                                        */
    uint32_t STS_BLK;             /* Offset: 0x2F0  (R/ ) Status for IntBlock Interrupt                   */
    uint32_t RESERVED42;          /* Offset: 0x2F4                                                        */
    uint32_t STS_SRC_TRAN;        /* Offset: 0x2F8  (R/ ) Status for IntSrcTran Interrupt                 */
    uint32_t RESERVED43;          /* Offset: 0x2FC                                                        */
    uint32_t STS_DST_TRAN;        /* Offset: 0x300  (R/ ) Status for IntDstTran Interrupt                 */
    uint32_t RESERVED44;          /* Offset: 0x304                                                        */
    uint32_t STS_ERR;             /* Offset: 0x308  (R/ ) Status for IntErr Interrupt                     */
    uint32_t RESERVED45;          /* Offset: 0x30C                                                        */
    uint32_t MSK_TFR;             /* Offset: 0x310  (R/W) Status for IntTfr Interrupt                     */
    uint32_t RESERVED46;          /* Offset: 0x314                                                        */
    uint32_t MSK_BLK;             /* Offset: 0x318  (R/W) Mask for IntBlock Interrupt                     */
    uint32_t RESERVED47;          /* Offset: 0x31C                                                        */
    uint32_t MSK_SRC_TRAN;        /* Offset: 0x320  (R/W) Status for IntSrcTran Interrupt                 */
    uint32_t RESERVED48;          /* Offset: 0x324                                                        */
    uint32_t MSK_DST_TRAN;        /* Offset: 0x328  (R/W) Mask for IntDstTran Interrupt                   */
    uint32_t RESERVED49;          /* Offset: 0x32C                                                        */
    uint32_t MSK_ERR;             /* Offset: 0x330  (R/W) Mask for IntErr Interrupt                       */
    uint32_t RESERVED50;          /* Offset: 0x334                                                        */
    uint32_t CLR_TFR;             /* Offset: 0x338  ( /W) Clear for IntTfr Interrupt                      */
    uint32_t RESERVED51;          /* Offset: 0x33C                                                        */
    uint32_t CLR_BLK;             /* Offset: 0x340  ( /W) Clear for IntBlock Interrupt                    */
    uint32_t RESERVED52;          /* Offset: 0x344                                                        */
    uint32_t CLR_SRC_TRAN;        /* Offset: 0x348  ( /W) Clear for IntSrcTran Interrupt                  */
    uint32_t RESERVED53;          /* Offset: 0x34C                                                        */
    uint32_t CLR_DST_TRAN;        /* Offset: 0x350  ( /W) Clear for IntDstTran Interrupt                  */
    uint32_t RESERVED54;          /* Offset: 0x354                                                        */
    uint32_t CLR_ERR;             /* Offset: 0x358  ( /W) Clear for IntErr Interrupt                      */
    uint32_t RESERVED55;          /* Offset: 0x35C                                                        */
    uint32_t STS_INT;             /* Offset: 0x360  (R/ ) Status for each Interrupt type                  */
    uint32_t RESERVED56;          /* Offset: 0x364                                                        */

    /* Software Handshake */
    uint32_t REQ_SRC;             /* Offset: 0x368  (R/W) Source Software Transaction Request register    */
    uint32_t RESERVED57;          /* Offset: 0x36C                                                        */
    uint32_t REQ_DST;             /* Offset: 0x370  (R/W) Destination Software Transaction Request register */
    uint32_t RESERVED58;          /* Offset: 0x374                                                        */
    uint32_t SGL_RQ_SRC;          /* Offset: 0x378  (R/W) Source Single Transaction Request register      */
    uint32_t RESERVED59;          /* Offset: 0x37C                                                        */
    uint32_t SGL_RQ_DST;          /* Offset: 0x380  (R/W) Destination Single Transaction Request register */
    uint32_t RESERVED60;          /* Offset: 0x384                                                        */
    uint32_t LST_SRC;             /* Offset: 0x388  (R/W) Source Last Transaction Request register        */
    uint32_t RESERVED61;          /* Offset: 0x38C                                                        */
    uint32_t LST_DST;             /* Offset: 0x390  (R/W) Destination Last Transaction Request register   */
    uint32_t RESERVED62;          /* Offset: 0x394                                                        */

    /* Miscellaneous */
    uint32_t CFG_DMA_EN:1;        /* Offset: 0x398  (R/W) DMAC Configuration Register                     */
    uint32_t Rsvd_DmaCfgReg:31;
    uint32_t RESERVED63;          /* Offset: 0x39C                                                        */
    uint32_t CH_EN_CH_EN:4;       /* Offset: 0x3A0  (R/W) DMAC Channel Enable Register                    */
    uint32_t Rsvd_ChEnReg:4;
    uint32_t CH_EN_CH_EN_WE:4;
    uint32_t Rsvd_1_ChEnReg:20;
    uint32_t RESERVED64;
    uint32_t ID;                  /* Offset: 0x3A8 */
} S_DMA_Debug_t;
#pragma pop
#endif 


/*
 *************************************************************************
 *						    Typedefs
 *************************************************************************
 */
typedef void (*T_Dma_CallBack)(void);

typedef void (*T_Hal_Dma_Init_fp)(void);
typedef uint32_t (*T_Hal_Dma_Check_fp)(E_DMA_CHANNEL eChannel);
typedef uint32_t (*T_Hal_Dma_Config_fp)(E_DMA_CHANNEL eChannel, S_DMA_Init_t *ptInit);
typedef uint32_t (*T_Hal_Dma_Enable_fp)(E_DMA_CHANNEL eChannel);
typedef uint32_t (*T_Hal_Dma_Abort_fp)(E_DMA_CHANNEL eChannel);
typedef void (*T_Hal_Dma_IntMask_fp)(E_DMA_CHANNEL eChannel, uint8_t u8Bmp);
typedef uint32_t (*T_Hal_Dma_IntEn_fp)(uint8_t u8Enable, uint8_t u8Bmp);
typedef uint32_t (*T_Hal_Dma_IntClear_fp)(E_DMA_CHANNEL eChannel, uint8_t u8Bmp);
typedef uint32_t (*T_Hal_Dma_IntClearAll_fp)(void);
typedef void (*T_Hal_Dma_CallBackSet_fp)(E_DMA_CHANNEL eChannel, T_Dma_CallBack tFunc);
typedef uint32_t (*T_Hal_Dma_m2mXfer_fp)(E_DMA_CHANNEL eChannel, uint32_t u32Src, uint32_t u32Dst, uint32_t u32Size, uint8_t u8PollXferEnd);

/*
 *************************************************************************
 *							Public Variables
 *************************************************************************
 */
extern T_Dma_CallBack g_tHalDmaCallBack[DMA_Channel_Max];
extern S_DMA_Channel_t * const g_pscaHalDmaCHx[DMA_Channel_Max];

extern T_Hal_Dma_Init_fp            Hal_Dma_Init;
extern T_Hal_Dma_Check_fp           Hal_Dma_Check;
extern T_Hal_Dma_Config_fp          Hal_Dma_Config;
extern T_Hal_Dma_Enable_fp          Hal_Dma_Enable;
extern T_Hal_Dma_Abort_fp           Hal_Dma_Abort;
extern T_Hal_Dma_IntMask_fp         Hal_Dma_IntMask;
extern T_Hal_Dma_IntEn_fp           Hal_Dma_IntEn;
extern T_Hal_Dma_IntClear_fp        Hal_Dma_IntClear;
extern T_Hal_Dma_IntClearAll_fp     Hal_Dma_IntClearAll;
extern T_Hal_Dma_CallBackSet_fp     Hal_Dma_CallBackSet;
extern T_Hal_Dma_m2mXfer_fp         Hal_Dma_m2mXfer;

/*
 *************************************************************************
 *							Public Functions
 *************************************************************************
 */
void Hal_Dma_Init_impl(void);
uint32_t Hal_Dma_Check_impl(E_DMA_CHANNEL eChannel);
uint32_t Hal_Dma_Config_impl(E_DMA_CHANNEL eChannel, S_DMA_Init_t *ptInit);
uint32_t Hal_Dma_Enable_impl(E_DMA_CHANNEL eChannel);
uint32_t Hal_Dma_Abort_impl(E_DMA_CHANNEL eChannel);
void Hal_Dma_IntMask_impl(E_DMA_CHANNEL eChannel, uint8_t u8Bmp);
uint32_t Hal_Dma_IntEn_impl(uint8_t u8Enable, uint8_t u8Bmp);
uint32_t Hal_Dma_IntClear_impl(E_DMA_CHANNEL eChannel, uint8_t u8Bmp);
uint32_t Hal_Dma_IntClearAll_impl(void);
void Hal_Dma_CallBackSet_impl(E_DMA_CHANNEL eChannel, T_Dma_CallBack tFunc);
uint32_t Hal_Dma_m2mXfer_impl(E_DMA_CHANNEL eChannel, uint32_t u32Src, uint32_t u32Dst, uint32_t u32Size, uint8_t u8PollXferEnd);

#endif
