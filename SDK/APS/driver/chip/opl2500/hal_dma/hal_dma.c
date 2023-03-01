/******************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 ******************************************************************************
 * @file hal_dma.c
 * 
 * @brief API to control DMA for APS
 * 
 */


/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "opulinks.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_dma.h"
#include "hal_tick.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */


/*
 *************************************************************************
 *						    Enums and Structures
 *************************************************************************
 */


/*
 *************************************************************************
 *						    Typedefs
 *************************************************************************
 */


/*
 *************************************************************************
 *                           Declarations of Private Functions
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
T_Dma_CallBack g_tHalDmaCallBack[DMA_Channel_Max];
S_DMA_Channel_t * const g_pscaHalDmaCHx[DMA_Channel_Max] = {(S_DMA_Channel_t *)&DMA->SAR0, 
                                                           (S_DMA_Channel_t *)&DMA->SAR1,
                                                           (S_DMA_Channel_t *)&DMA->SAR2, 
                                                           (S_DMA_Channel_t *)&DMA->SAR3};
uint32_t g_u32DmaTimeoutMs = DMA_TIMEOUT_MS;
                                                           
T_Hal_Dma_Init_fp          Hal_Dma_Init         = Hal_Dma_Init_impl;
T_Hal_Dma_Check_fp         Hal_Dma_Check        = Hal_Dma_Check_impl;
T_Hal_Dma_Config_fp        Hal_Dma_Config       = Hal_Dma_Config_impl;
T_Hal_Dma_Enable_fp        Hal_Dma_Enable       = Hal_Dma_Enable_impl;
T_Hal_Dma_Abort_fp         Hal_Dma_Abort        = Hal_Dma_Abort_impl;
T_Hal_Dma_IntMask_fp       Hal_Dma_IntMask      = Hal_Dma_IntMask_impl;
//T_Hal_Dma_IntEn_fp         Hal_Dma_IntEn        = Hal_Dma_IntEn_impl;
T_Hal_Dma_IntClear_fp      Hal_Dma_IntClear     = Hal_Dma_IntClear_impl;
T_Hal_Dma_IntClearAll_fp   Hal_Dma_IntClearAll  = Hal_Dma_IntClearAll_impl;
T_Hal_Dma_CallBackSet_fp   Hal_Dma_CallBackSet  = Hal_Dma_CallBackSet_impl;
T_Hal_Dma_m2mXfer_fp       Hal_Dma_m2mXfer      = Hal_Dma_m2mXfer_impl;

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                           Public Functions
 *************************************************************************
 */
/*************************************************************************
* FUNCTION:
*  Hal_Dma_Init
*
* DESCRIPTION:
*   1. Init APS_DMA
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Dma_Init_impl(void)
{
    Hal_Sys_ApsModuleRst(APS_RST_DMA);
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_DMA);
    
    Hal_Vic_IntInit(APS_DMA_IRQn, VIC_TRIG_LEVEL_HIGH, IRQ_PRIORITY_DMA);
    
    // enable global channel enable bit of DMA
    DMA->CFG = DMA_CFG_DMA_EN;
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_Check
*
* DESCRIPTION:
*   1. Check whether the specified DMA channel is free
*
* CALLS
*
* PARAMETERS
*   1. eChannel : specify which DMA channel
*
* RETURNS
*   0 : Channel is Free
*   1 : Channel is Busy
*   2 : Invalid parameter
*
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Dma_Check_impl(E_DMA_CHANNEL eChannel)
{
    if (eChannel >= DMA_Channel_Max)
        return 2;
    
    if (DMA->CH_EN & (1 << (APS_DMA_CH_EN_CH_EN_Pos + eChannel)))
        return 1;
    else 
        return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_Config
*
* DESCRIPTION:
*   1. Configure the specified DMA channel
*
* CALLS
*
* PARAMETERS
*   1. eChannel : specify which DMA channel
*   2. ptInit : pointer of configurations
*
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Dma_Config_impl(E_DMA_CHANNEL eChannel, S_DMA_Init_t *ptInit)
{
    // Parameter Checking
    if (ptInit->eSrcTrWidth >= DMA_TR_WIDTH_Max) return 1;
    if (ptInit->eDstTrWidth >= DMA_TR_WIDTH_Max) return 1;
    if (ptInit->eDinc >= DMA_INC_Max) return 1;
    if (ptInit->eSinc >= DMA_INC_Max) return 1;
    if (ptInit->eTtfc >= DMA_TT_FC_Max) return 1;
    if (ptInit->ePrior >= DMA_CH_PRIOR_Max) return 1;
    
    // SARx must be aligned to CTLx.SRC_TR_WIDTH
    if ((ptInit->Sar & ((1<<ptInit->eSrcTrWidth)-1)) != 0) // SARx %  1/2/4 bytes
        return 1;
    // DARx must be aligned to CTLx.DST_TR_WIDTH
    if ((ptInit->Dar & ((1<<ptInit->eDstTrWidth)-1)) != 0) // DARx % 1/2/4 bytes
        return 1;
    
    if (eChannel >= DMA_Channel_Max)
        return 1;
    
    uint32_t u32Temp = 0;
    S_DMA_Channel_t *CHx = g_pscaHalDmaCHx[eChannel];
    
    CHx->SAR = ptInit->Sar;
    CHx->DAR = ptInit->Dar;
    
    u32Temp = CHx->LLP;
    u32Temp &= ~(DMA_LLP_LOC_Msk);
    u32Temp |= (ptInit->Loc << DMA_LLP_LOC_Pos);
    CHx->LLP = u32Temp;
    
    u32Temp = CHx->CTL_L;
    u32Temp &= ~( DMA_CTL_L_DST_TR_WIDTH_Msk |
                  DMA_CTL_L_SRC_TR_WIDTH_Msk |
                  DMA_CTL_L_DINC_Msk |
                  DMA_CTL_L_SINC_Msk |
                  DMA_CTL_L_DEST_MSIZE_Msk |
                  DMA_CTL_L_SRC_MSIZE_Msk |
                  DMA_CTL_L_SRC_GATHER_EN_Msk |
                  DMA_CTL_L_DST_SCATTER_EN_Msk |
                  DMA_CTL_L_TT_FC_Msk |
                  DMA_CTL_L_LLP_DST_EN_Msk |
                  DMA_CTL_L_LLP_SRC_EN_Msk );
    u32Temp |= ( (ptInit->eDstTrWidth << DMA_CTL_L_DST_TR_WIDTH_Pos) |
                 (ptInit->eSrcTrWidth << DMA_CTL_L_SRC_TR_WIDTH_Pos) |
                 (ptInit->eDinc << DMA_CTL_L_DINC_Pos) |
                 (ptInit->eSinc << DMA_CTL_L_SINC_Pos) |
                 (ptInit->eDestMsize << DMA_CTL_L_DEST_MSIZE_Pos) |
                 (ptInit->eSrcMsize << DMA_CTL_L_SRC_MSIZE_Pos) |
                 (ptInit->SrcGatherEn << DMA_CTL_L_SRC_GATHER_EN_Pos) |
                 (ptInit->DstScatterEn << DMA_CTL_L_DST_SCATTER_EN_Pos) |
                 (ptInit->eTtfc << DMA_CTL_L_TT_FC_Pos) |
                 (ptInit->LlpDstEn << DMA_CTL_L_LLP_DST_EN_Pos) |
                 (ptInit->LlpSrcEn << DMA_CTL_L_LLP_SRC_EN_Pos) );
    CHx->CTL_L = u32Temp;
    
    u32Temp = CHx->CTL_H;
    u32Temp = ~( DMA_CTL_H_BLOCK_TS_Msk |
                 DMA_CTL_H_DONE_Msk );
    u32Temp |= ( (ptInit->BlockTs << DMA_CTL_H_BLOCK_TS_Pos) |
                 (ptInit->Done << DMA_CTL_H_DONE_Pos) );
    CHx->CTL_H = u32Temp;
    
    CHx->SSTAT = ptInit->SrcStsAddr;
    CHx->DSTAT = ptInit->DstStsAddr;
    CHx->SSTATAR = ptInit->Sstatar;
    CHx->DSTATAR = ptInit->Dstatar;
    
    u32Temp = CHx->CFG_L;
    u32Temp &= ~( DMA_CFG_L_CH_PRIOR_Msk |
                  DMA_CFG_L_HS_SEL_DST_Msk |
                  DMA_CFG_L_HS_SEL_SRC_Msk |
                  DMA_CFG_L_DST_HS_POL_Msk |
                  DMA_CFG_L_SRC_HS_POL_Msk |
                  DMA_CFG_L_RELOAD_SRC_Msk |
                  DMA_CFG_L_RELOAD_DST_Msk );
    u32Temp |= ( (ptInit->ePrior << DMA_CFG_L_CH_PRIOR_Pos) |
                 (ptInit->HsSelDst << DMA_CFG_L_HS_SEL_DST_Pos) |
                 (ptInit->HsSelSrc << DMA_CFG_L_HS_SEL_SRC_Pos) |
                 (ptInit->DstHsPol << DMA_CFG_L_DST_HS_POL_Pos) |
                 (ptInit->SrcHsPol << DMA_CFG_L_SRC_HS_POL_Pos) |
                 (ptInit->ReloadSrc << DMA_CFG_L_RELOAD_SRC_Pos) |
                 (ptInit->ReloadDst << DMA_CFG_L_RELOAD_DST_Pos) );
    CHx->CFG_L = u32Temp;
    
    u32Temp = CHx->CFG_H;
    u32Temp &= ~( DMA_CFG_H_FCMODE_Msk |
                  DMA_CFG_H_FIFO_MODE_Msk |
                  DMA_CFG_H_PROTCTL_Msk |
                  DMA_CFG_H_DS_UPD_EN_Msk |
                  DMA_CFG_H_SS_UPD_EN_Msk |
                  DMA_CFG_H_SRC_PER_Msk |
                  DMA_CFG_H_DEST_PER_Msk );
    u32Temp |= ( (ptInit->Fcmode << DMA_CFG_H_FCMODE_Pos) |
                 (ptInit->FifoMode << DMA_CFG_H_FIFO_MODE_Pos) |
                 (ptInit->Protctl << DMA_CFG_H_PROTCTL_Pos) |
                 (ptInit->DsUpdEn << DMA_CFG_H_DS_UPD_EN_Pos) |
                 (ptInit->SsUpdEn << DMA_CFG_H_SS_UPD_EN_Pos) |
                 (ptInit->SrcPer << DMA_CFG_H_SRC_PER_Pos) |
                 (ptInit->DestPer << DMA_CFG_H_DEST_PER_Pos) );
    CHx->CFG_H = u32Temp;
    
    u32Temp = CHx->SGR;
    u32Temp &= ~( DMA_SGR_SGI_Msk |
                  DMA_SGR_SGC_Msk );
    u32Temp |= ( (ptInit->Sgi << DMA_SGR_SGI_Pos) |
                 (ptInit->Sgc << DMA_SGR_SGC_Pos) );
    CHx->SGR = u32Temp;
    
    u32Temp = CHx->DSR;
    u32Temp &= ~( DMA_DSR_DSI_Msk |
                  DMA_DSR_DSC_Msk );
    u32Temp |= ( (ptInit->Dsi << DMA_DSR_DSI_Pos) |
                 (ptInit->Dsc << DMA_DSR_DSC_Pos) );
    CHx->DSR = u32Temp;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_Enable
*
* DESCRIPTION:
*   1. Enable the specified DMA Channel
*
* CALLS
*
* PARAMETERS
*   1. eChannel : specify which DMA channel
*
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Dma_Enable_impl(E_DMA_CHANNEL eChannel)
{
    if (eChannel >= DMA_Channel_Max)
        return 1;
    
    if ((DMA->CFG & DMA_CFG_DMA_EN_Msk) == 0)
    {
        DMA->CFG = DMA_CFG_DMA_EN;
    }
    
    DMA->CH_EN = (1 << (APS_DMA_CH_EN_CH_EN_Pos + eChannel)) |
                 (1 << (APS_DMA_CH_EN_CH_EN_WE_Pos + eChannel));
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_Abort
*
* DESCRIPTION:
*   1. Abort transfer of the specified DMA channel
*
* CALLS
*
* PARAMETERS
*   1. eChannel : specify which DMA channel
*
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Dma_Abort_impl(E_DMA_CHANNEL eChannel)
{
    if (eChannel >= DMA_Channel_Max)
        return 1;
    
    S_DMA_Channel_t *CHx = g_pscaHalDmaCHx[eChannel];
    
    // Refer to 7.7 Disabling a Channel Prior to Transfer Completion
    
    // Write 1 to CFGx.CH_SUSP
    CHx->CFG_L |= DMA_CFG_L_CH_SUSP_Msk;
    
    // Polling CFGx.FIFO_EMPTY
    while ((CHx->CFG_L & DMA_CFG_L_FIFO_EMPTY_Msk) == 0);
    
    // Clear CFGx.CH_SUSP
    CHx->CFG_L &= ~(DMA_CFG_L_CH_SUSP_Msk);
    
    // Clear ChEnReg.CH_EN
    DMA->CH_EN = (1 << (APS_DMA_CH_EN_CH_EN_WE_Pos + eChannel));
    
    if (Hal_Dma_Check(eChannel))
        return 1;
    else
        return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_IntMask
*
* DESCRIPTION:
*  1. Mask/Unmask interrupts of the specified DMA channel
*
* CALLS
*
* PARAMETERS
*   1. eChannel : specify which DMA channel
*   2. u8Bmp : bitmap of Mask* Interrupt Registers
*              1 = Unmask
*              0 = Mask
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Dma_IntMask_impl(E_DMA_CHANNEL eChannel, uint8_t u8Bmp)
{
    if (eChannel >= DMA_Channel_Max)
        return;
    
    uint8_t u8Tfr  = (u8Bmp & DMA_BMP_TFR) ? 1 : 0;
    uint8_t u8Blk  = (u8Bmp & DMA_BMP_BLK) ? 1 : 0;
    uint8_t u8Srct = (u8Bmp & DMA_BMP_SRC_TRAN) ? 1 : 0;
    uint8_t u8Dstt = (u8Bmp & DMA_BMP_DST_TRAN) ? 1 : 0;
    uint8_t u8Err  = (u8Bmp & DMA_BMP_ERR) ? 1 : 0;
    
    DMA->MSK_TFR = (u8Tfr << (APS_DMA_MSK_TFR_INT_MSK_Pos + eChannel)) |
                   (1 << (APS_DMA_MSK_TFR_INT_MSK_WE_Pos + eChannel));
    
    DMA->MSK_BLK = (u8Blk << (APS_DMA_MSK_BLK_INT_MSK_Pos + eChannel)) |
                   (1 << (APS_DMA_MSK_BLK_INT_MSK_WE_Pos + eChannel));
    
    DMA->MSK_SRC_TRAN = (u8Srct << (APS_DMA_MSK_SRC_TRAN_INT_MSK_Pos + eChannel)) |
                        (1 << (APS_DMA_MSK_SRC_TRAN_INT_MSK_WE_Pos + eChannel));
    
    DMA->MSK_DST_TRAN = (u8Dstt << (APS_DMA_MSK_DST_TRAN_INT_MSK_Pos + eChannel)) |
                        (1 << (APS_DMA_MSK_DST_TRAN_INT_MSK_WE_Pos + eChannel));
    
    DMA->MSK_ERR = (u8Err << (APS_DMA_MSK_ERR_INT_MSK_Pos + eChannel)) |
                   (1 << (APS_DMA_MSK_ERR_INT_MSK_WE_Pos + eChannel));
}


///*************************************************************************
//* FUNCTION:
//*  Hal_Dma_IntEn
//*
//* DESCRIPTION:
//*   1. Enable/Disable the DMA interrupts
//*
//* CALLS
//*
//* PARAMETERS
//*   1. u8Enable : 1 = Enable
//*                 0 = Disable
//*   2. u8Bmp : bitmap of Channels
//*              1 = Apply Change
//*              0 = No Change
//*
//* RETURNS
//*   0 : setting complete
//*   1 : error
//* GLOBALS AFFECTED
//* 
//*************************************************************************/
//uint32_t Hal_Dma_IntEn_impl(uint8_t u8Enable, uint8_t u8Bmp)
//{
//    uint8_t u8Ch0En = (u8Bmp & DMA_BMP_INT_CH0) ? 1 : 0;
//    uint8_t u8Ch1En = (u8Bmp & DMA_BMP_INT_CH1) ? 1 : 0;
//    uint8_t u8Ch2En = (u8Bmp & DMA_BMP_INT_CH2) ? 1 : 0;
//    uint8_t u8Ch3En = (u8Bmp & DMA_BMP_INT_CH3) ? 1 : 0;
//    
//    /* Write IER, enable interrupt */
//    if(u8Enable == ENABLE)
//    {
//        Hal_Vic_IntInit(APS_DMA_IRQn, VIC_TRIG_LEVEL_HIGH, IRQ_PRIORITY_DMA);
//        
//        /* Module part: Enable module */
//        DMA->CTL0_L = DMA->CTL0_L | (u8Ch0En << DMA_CTL_L_INT_EN_Pos);
//        DMA->CTL1_L = DMA->CTL1_L | (u8Ch1En << DMA_CTL_L_INT_EN_Pos);
//        DMA->CTL2_L = DMA->CTL2_L | (u8Ch2En << DMA_CTL_L_INT_EN_Pos);
//        DMA->CTL3_L = DMA->CTL3_L | (u8Ch3En << DMA_CTL_L_INT_EN_Pos);
//    }
//    else
//    {
//        /* Module part: Disable module */
//        DMA->CTL0_L = DMA->CTL0_L & ~(u8Ch0En << DMA_CTL_L_INT_EN_Pos);
//        DMA->CTL1_L = DMA->CTL1_L & ~(u8Ch1En << DMA_CTL_L_INT_EN_Pos);
//        DMA->CTL2_L = DMA->CTL2_L & ~(u8Ch2En << DMA_CTL_L_INT_EN_Pos);
//        DMA->CTL3_L = DMA->CTL3_L & ~(u8Ch3En << DMA_CTL_L_INT_EN_Pos);

//        Hal_Vic_IntDeInit(APS_DMA_IRQn);
//    }
//    return 0;
//}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_IntClear
*
* DESCRIPTION:
*   1. Clear the specified interrupts of DMA
*
* CALLS
*
* PARAMETERS
*   1. eChannel : specify which DMA channel
*   2. u8Bmp : bitmap of Clear* Interrupt Registers
*              1 = Clear
*              0 = No Change
*
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Dma_IntClear_impl(E_DMA_CHANNEL eChannel, uint8_t u8Bmp)
{
    if (eChannel >= DMA_Channel_Max)
        return 1;
    
    uint8_t u8Tfr  = (u8Bmp & DMA_BMP_TFR) ? 1 : 0;
    uint8_t u8Blk  = (u8Bmp & DMA_BMP_BLK) ? 1 : 0;
    uint8_t u8Srct = (u8Bmp & DMA_BMP_SRC_TRAN) ? 1 : 0;
    uint8_t u8Dstt = (u8Bmp & DMA_BMP_DST_TRAN) ? 1 : 0;
    uint8_t u8Err  = (u8Bmp & DMA_BMP_ERR) ? 1 : 0;
    
    uint32_t u32IntSts = 0;
    
    // clear interrupt
    DMA->CLR_TFR      = (u8Tfr << eChannel);
    DMA->CLR_BLK      = (u8Blk << eChannel);
    DMA->CLR_SRC_TRAN = (u8Srct << eChannel);
    DMA->CLR_DST_TRAN = (u8Dstt << eChannel);
    DMA->CLR_ERR      = (u8Err << eChannel);
        
    // Confirm
    u32IntSts = ( (DMA->STS_TFR & (u8Tfr << eChannel)) |
                  (DMA->STS_BLK & (u8Blk << eChannel)) |
                  (DMA->STS_SRC_TRAN & (u8Srct << eChannel)) |
                  (DMA->STS_DST_TRAN & (u8Dstt << eChannel)) |
                  (DMA->STS_ERR & (u8Err << eChannel)) );
                      
    if (u32IntSts)
        return 1;
    else
        return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_IntClearAll
*
* DESCRIPTION:
*   1. Clear any pending interrupts of DMA
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Dma_IntClearAll_impl(void)
{
    uint32_t u32IntSts = 0;
    
    // clear interrupt
    DMA->CLR_TFR      = 0x0000000F;
    DMA->CLR_BLK      = 0x0000000F;
    DMA->CLR_SRC_TRAN = 0x0000000F;
    DMA->CLR_DST_TRAN = 0x0000000F;
    DMA->CLR_ERR      = 0x0000000F;
    
    // Confirm
    u32IntSts = (DMA->STS_TFR | DMA->STS_BLK | DMA->STS_SRC_TRAN | DMA->STS_DST_TRAN | DMA->STS_ERR);
    
    if (u32IntSts)
        return 1;
    else
        return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_CallBackFuncSet
*
* DESCRIPTION:
*   1. Setup DMA callback function
*
* CALLS
*
* PARAMETERS
*   1. eChannel : specify which DMA channel
*   2. tFunc : Callback function
*
* RETURNS
*   None
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Dma_CallBackSet_impl(E_DMA_CHANNEL eChannel, T_Dma_CallBack tFunc)
{
    if (eChannel >= DMA_Channel_Max)
        return;
    
    g_tHalDmaCallBack[eChannel] = tFunc;
}

/*************************************************************************
* FUNCTION:
*  Hal_Dma_m2mXfer
*
* DESCRIPTION:
*   1. Do DMA memory to memory transfer
*
* CALLS
*
* PARAMETERS
*   1. eChannel : specify which DMA channel
*   2. u32Src : source address
*   3. u32Dst : destination address
*   4. u32Size : transfer size (byte), max = 4095*4 = 16380 bytes
*   5. u8PollXferEnd : if set to 1, would poll for transfer done in this function
*
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Dma_m2mXfer_impl(E_DMA_CHANNEL eChannel, uint32_t u32Src, uint32_t u32Dst, uint32_t u32Size, uint8_t u8PollXferEnd)
{
    uint32_t u32AdjSrc = u32Src;
    uint32_t u32AdjDst = u32Dst;
    uint32_t u32AdjSize = u32Size;
    uint32_t u32Adjust = 0;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    
    if (eChannel >= DMA_Channel_Max)
        return 1;
    
    if ((u32Size >> 2) > MAX_DMA_XFER_NUM)
        return 1;
    
    S_DMA_Init_t tDma = {
        .Sar = u32Src,
        .Dar = u32Dst,
        .eDstTrWidth = DMA_TR_WIDTH_32_bits,
        .eSrcTrWidth = DMA_TR_WIDTH_32_bits,
        .eTtfc = DMA_TT_m2m_FC_Dma,
        .BlockTs = (u32Size >> 2),
        .HsSelDst = 1,
        .HsSelSrc = 1,
        .Protctl = 1,
    };
    
    // src is not word-aligned, ajust it
    if (u32Src & 0x3)
    {
        u32Adjust = 4 - (u32Src & 0x3);
        memcpy((void *) u32Dst, (void *) u32Src, u32Adjust);
        u32AdjSrc = u32Src + u32Adjust;
        u32AdjDst = u32Dst + u32Adjust;
        u32AdjSize -= u32Adjust;

        tDma.Sar = u32AdjSrc;
        tDma.Dar = u32AdjDst;

        if (u32AdjDst & 0x3) // dst CANNOT be adjusted to word-aligned => 32->8 xfer
        {
            tDma.eDstTrWidth = DMA_TR_WIDTH_8_bits;
        }
        tDma.BlockTs = (u32AdjSize >> 2);
    }
    else // src is word-aligned
    {
        if (u32Dst & 0x3) // dst is not word-aligned => 32->8 xfer
        {
            tDma.eDstTrWidth = DMA_TR_WIDTH_8_bits;
        }
    }

    // Config DMA channel
    if (Hal_Dma_Config(eChannel, &tDma))
        return 1;
    
    // Enable DMA channel
    if (Hal_Dma_Enable(eChannel))
        return 1;
    
    if (u8PollXferEnd)
    {
        // Poll for DMA transfer done
        u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32DmaTimeoutMs;
        u32TickStart = Hal_Tick_Diff(0);
        while (Hal_Dma_Check(eChannel))
        {
            u32TickDiff = Hal_Tick_Diff(u32TickStart);
            if (u32TickDiff > u32TimeoutTicks) return 1;
        }
    }
    
    // Copy remain data manually
    if (u32AdjSize & 0x3)
    {
        memcpy(
            (void *) (u32AdjDst + ((u32AdjSize >> 2) << 2)), 
            (void *) (u32AdjSrc + ((u32AdjSize >> 2) << 2)), 
            (u32AdjSize & 0x3)
        );
    }
    
    return 0;
}

/*
 *************************************************************************
 *                           Private Functions
 *************************************************************************
 */

