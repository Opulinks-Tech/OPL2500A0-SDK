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
 *  @file hal_msq_dma.h
 *  @brief API to control MSQ DMA
 ******************************************************************************/

#ifndef __HAL_MSQ_DMA_H__
#define __HAL_MSQ_DMA_H__

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
#include "opulinks.h"
#include "hal_dma.h"

/*
 *************************************************************************
 *						    Definitions and Macros
 *************************************************************************
 */
#define MSQ_DMA_TIMEOUT_MS      100
 
/* ***** Register field definition **** */
/* CFGx */
#define DMA_FIFO_NOT_EMPTY      (0UL << DMA_CFG_L_FIFO_EMPTY_Pos)
#define DMA_FIFO_EMPTY          (1UL << DMA_CFG_L_FIFO_EMPTY_Pos)
#define DMA_FCMODE_PREFETCH     (0UL << DMA_CFG_H_FCMODE_Pos)
#define DMA_FCMODE_NOT_PREFETCH (1UL << DMA_CFG_H_FCMODE_Pos)
#define DMA_HS_SEL_DST_HW       (0UL << DMA_CFG_L_HS_SEL_DST_Pos)
#define DMA_HS_SEL_DST_SW       (1UL << DMA_CFG_L_HS_SEL_DST_Pos)
#define DMA_HS_SEL_SRC_HW       (0UL << DMA_CFG_L_HS_SEL_SRC_Pos)
#define DMA_HS_SEL_SRC_SW       (1UL << DMA_CFG_L_HS_SEL_SRC_Pos)
#define DMA_HS_POL_DST_AH       (0UL << DMA_CFG_L_DST_HS_POL_Pos)
#define DMA_HS_POL_DST_AL       (1UL << DMA_CFG_L_DST_HS_POL_Pos)
#define DMA_HS_POL_SRC_AH       (0UL << DMA_CFG_L_SRC_HS_POL_Pos)
#define DMA_HS_POL_SRC_AL       (1UL << DMA_CFG_L_SRC_HS_POL_Pos)
#define DMA_FIFO_MODE_SINGLE    (0UL << DMA_CFG_H_FIFO_MODE_Pos)
#define DMA_FIFO_MODE_BURST     (1UL << DMA_CFG_H_FIFO_MODE_Pos) // TODO: not sure the naming

/* ChEnReg */
#define DMA_GLOBAL_CH_EN        (DMA_CFG_DMA_EN)

/*
 *************************************************************************
 *						    Enums and Structures
 *************************************************************************
 */

typedef enum {
    MSQ_DMA_ITF_MSQ_UART_TX = 0,    /* 0  */
    MSQ_DMA_ITF_MSQ_UART_RX,        /* 1  */
    MSQ_DMA_ITF_PDM,                /* 2  */
    MSQ_DMA_ITF_AUXADC,             /* 3  */
}E_MSQ_DMA_HW_ITF;


/* LLI Structure */
typedef struct // TODO: bit definition
{
    uint32_t SAR;
    uint32_t DAR;
    uint32_t LLP;
    uint32_t CTL_L;
    uint32_t CTL_H;
//    uint32_t SSTAT; // DMAH_CH0_STAT_SRC = 0 for MSQ
    uint32_t DSTAT;
} S_MSQ_DMA_LinkListItem_t;

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
    uint32_t SrcPer:4;            /* Source Hardware Interface */
    uint32_t DestPer:4;           /* Destination Hardware Interface */
    // SGRx 
    uint32_t Sgi:20;              /* Source Gather Interval */
    uint32_t Sgc:12;              /* Source Gather Count */
    // DSRx
    uint32_t Dsi:20;              /* Destination Scatter Interval */
    uint32_t Dsc:12;              /* Destination Scatter Count */
} S_MSQ_DMA_Init_t;

#if 1
// debug usage
// add ((S_MSQ_DMA_Debug_t *) MSQ_DMA_BASE) to Watch Windows
#pragma push
#pragma anon_unions

typedef struct 
{
    /* Channel 0 */
    uint32_t SAR0;                /* Offset: 0x000  (R/W) Source Address for Channel 0                    */
    uint32_t RESERVED0;           /* Offset: 0x004                                                        */
    uint32_t DAR0;                /* Offset: 0x008  (R/W) Destination Address Register for Channel 0      */
    uint32_t RESERVED1;           /* Offset: 0x00C                                                        */
    uint32_t LLP0_LMS:2;          /* Offset: 0x010  (R/W) Linked List Pointer Register for Channel 0      */
    uint32_t LLP0_LOC:30;
    uint32_t RESERVED2;           /* Offset: 0x014     */
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
    uint32_t RESERVED3[8];        /* Offset: 0x020 ~ 0x03C                                                */
    uint32_t Rsvd_CFG:5;          /* Offset: 0x040  (R/W) Configuration Register for Channel 0 (LOW)      */
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
    uint32_t Rsvd_1_CFG:10;
    uint32_t CFG0_L_RELOAD_SRC:1;
    uint32_t CFG0_L_RELOAD_DST:1;
    uint32_t CFG0_H_FCMODE:1;     /* Offset: 0x044  (R/W) Configuration Register for Channel 0 (HIGH)     */
    uint32_t CFG0_H_FIFOMODE:1;
    uint32_t CFG0_H_PROTCTL:3;
    uint32_t CFG0_H_DS_UPD_EN:1;
    uint32_t CFG0_H_SRC_PER:4;
    uint32_t CFG0_H_DEST_PER:4;
    uint32_t Rsvd_2_CFG:17;
    uint32_t SGR0_SGI:20;         /* Offset: 0x048  (R/W) Source Gather Register for Channel 0            */
    uint32_t SGR0_SGC:12;
    uint32_t RESERVED7;           /* Offset: 0x04C                                                        */
    uint32_t DSR0_DSI:20;         /* Offset: 0x050  (R/W) Destination Scatter Register for Channel 0      */
    uint32_t DSR0_DSC:12;
    uint32_t RESERVED8[155];      /* Offset: 0x054 ~ 0x2BC                                                */          
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
    uint32_t CFG_DMA_EN:1;        /* Offset: 0x398  (R/W) DMAC Configuration Register                     */
    uint32_t Rsvd_DmaCfgReg:31;
    uint32_t RESERVED63;          /* Offset: 0x39C                                                        */
    uint32_t CH_EN_CH_EN:1;       /* Offset: 0x3A0  (R/W) DMAC Channel Enable Register                    */
    uint32_t Rsvd_ChEnReg:7;
    uint32_t CH_EN_CH_EN_WE:1;
    uint32_t Rsvd_1_ChEnReg:23;
    uint32_t RESERVED64;
    uint32_t ID;                  /* Offset: 0x3A8 */
} S_MSQ_DMA_Debug_t;
#pragma pop
#endif


/*
 *************************************************************************
 *						    Typedefs
 *************************************************************************
 */
typedef void (*T_Dma_CallBack)(void);

typedef void (*T_Hal_Msq_Dma_Init_fp)(void);
typedef uint32_t (*T_Hal_Msq_Dma_Check_fp)(void);
typedef uint32_t (*T_Hal_Msq_Dma_Config_fp)(S_MSQ_DMA_Init_t *ptInit);
typedef uint32_t (*T_Hal_Msq_Dma_Enable_fp)(void);
typedef uint32_t (*T_Hal_Msq_Dma_Abort_fp)(void);
typedef void (*T_Hal_Msq_Dma_IntMask_fp)(uint8_t u8Bmp);
typedef uint32_t (*T_Hal_Msq_Dma_IntEn_fp)(E_FUNC_ST eEnable);
typedef uint32_t (*T_Hal_Msq_Dma_IntClear_fp)(uint8_t u8Bmp);
typedef uint32_t (*T_Hal_Msq_Dma_IntClearAll_fp)(void);
typedef void (*T_Hal_Msq_Dma_CallBackSet_fp)(T_Dma_CallBack tFunc);


/*
 *************************************************************************
 *							Public Variables
 *************************************************************************
 */
extern T_Dma_CallBack g_tHalMsqDmaCallBack;

extern T_Hal_Msq_Dma_Init_fp            Hal_Msq_Dma_Init;
extern T_Hal_Msq_Dma_Check_fp           Hal_Msq_Dma_Check;
extern T_Hal_Msq_Dma_Config_fp          Hal_Msq_Dma_Config;
extern T_Hal_Msq_Dma_Enable_fp          Hal_Msq_Dma_Enable;
extern T_Hal_Msq_Dma_Abort_fp           Hal_Msq_Dma_Abort;
extern T_Hal_Msq_Dma_IntMask_fp         Hal_Msq_Dma_IntMask;
extern T_Hal_Msq_Dma_IntEn_fp           Hal_Msq_Dma_IntEn;
extern T_Hal_Msq_Dma_IntClear_fp        Hal_Msq_Dma_IntClear;
extern T_Hal_Msq_Dma_IntClearAll_fp     Hal_Msq_Dma_IntClearAll;
extern T_Hal_Msq_Dma_CallBackSet_fp     Hal_Msq_Dma_CallBackSet;


/*
 *************************************************************************
 *							Public Functions
 *************************************************************************
 */
void Hal_Msq_Dma_Init_impl(void);
uint32_t Hal_Msq_Dma_Check_impl(void);
uint32_t Hal_Msq_Dma_Config_impl(S_MSQ_DMA_Init_t *ptInit);
uint32_t Hal_Msq_Dma_Enable_impl(void);
uint32_t Hal_Msq_Dma_Abort_impl(void);
void Hal_Msq_Dma_IntMask_impl(uint8_t u8Bmp);
uint32_t Hal_Msq_Dma_IntEn_impl(E_FUNC_ST eEnable);
uint32_t Hal_Msq_Dma_IntClear_impl(uint8_t u8Bmp);
uint32_t Hal_Msq_Dma_IntClearAll_impl(void);
void Hal_Msq_Dma_CallBackSet_impl(T_Dma_CallBack tFunc);


#endif  /* __HAL_MSQ_DMA_H__ */
