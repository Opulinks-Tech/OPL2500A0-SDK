/******************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *******************************************************************************
 * @file hal_msq_dma.c
 * 
 * @brief API to control DMA for MSQ
 * 
 */


/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "opulinks.h"
#include "hal_msq_dma.h"
#include "hal_vic.h"
#include "hal_system.h"
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
T_Dma_CallBack g_tHalMsqDmaCallBack = NULL;

T_Hal_Msq_Dma_Init_fp          Hal_Msq_Dma_Init         = Hal_Msq_Dma_Init_impl;
T_Hal_Msq_Dma_Check_fp         Hal_Msq_Dma_Check        = Hal_Msq_Dma_Check_impl;
T_Hal_Msq_Dma_Config_fp        Hal_Msq_Dma_Config       = Hal_Msq_Dma_Config_impl;
T_Hal_Msq_Dma_Enable_fp        Hal_Msq_Dma_Enable       = Hal_Msq_Dma_Enable_impl;
T_Hal_Msq_Dma_Abort_fp         Hal_Msq_Dma_Abort        = Hal_Msq_Dma_Abort_impl;
T_Hal_Msq_Dma_IntMask_fp       Hal_Msq_Dma_IntMask      = Hal_Msq_Dma_IntMask_impl;
T_Hal_Msq_Dma_IntEn_fp         Hal_Msq_Dma_IntEn        = Hal_Msq_Dma_IntEn_impl;
T_Hal_Msq_Dma_IntClear_fp      Hal_Msq_Dma_IntClear     = Hal_Msq_Dma_IntClear_impl;
T_Hal_Msq_Dma_IntClearAll_fp   Hal_Msq_Dma_IntClearAll  = Hal_Msq_Dma_IntClearAll_impl;
T_Hal_Msq_Dma_CallBackSet_fp   Hal_Msq_Dma_CallBackSet  = Hal_Msq_Dma_CallBackSet_impl;

uint32_t g_u32MsqDmaTimeoutMs  = MSQ_DMA_TIMEOUT_MS;

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
*  Hal_Msq_Dma_Init
*
* DESCRIPTION:
*   1. Init MSQ_DMA
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
void Hal_Msq_Dma_Init_impl(void)
{
    Hal_Sys_MsqModuleRst(MSQ_RST_DMA);
    Hal_Sys_MsqModuleClkEn(ENABLE, MSQ_CLK_DMA);

    Hal_Vic_IntInit(MSQ_DMA_IRQn, VIC_TRIG_LEVEL_HIGH, IRQ_PRIORITY_DMA);
    
    // enable global channel enable bit of DMA
    MSQ_DMA->CFG = DMA_CFG_DMA_EN;
}
 
/*************************************************************************
* FUNCTION:
*  Hal_Msq_Dma_Check
*
* DESCRIPTION:
*   1. Check whether DMA channel 0 is free
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   0 : Channel is Free
*   1 : Channel is Busy
*
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Msq_Dma_Check_impl(void)
{
    if (MSQ_DMA->CH_EN & MSQ_DMA_CH_EN_CH_EN_Msk)
        return 1;
    else
        return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Msq_Dma_Config
*
* DESCRIPTION:
*   1. Configure DMA channel 0
*
* CALLS
*
* PARAMETERS
*   1. ptInit : pointer of S_DMA_Init_t
*
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Msq_Dma_Config_impl(S_MSQ_DMA_Init_t *ptInit)
{
    uint32_t u32Temp = 0;
    
    // Parameter Checking
    if (ptInit->eSrcTrWidth >= DMA_TR_WIDTH_Max) return 1;
    if (ptInit->eDstTrWidth >= DMA_TR_WIDTH_Max) return 1;
    if (ptInit->eDinc >= DMA_INC_Max) return 1;
    if (ptInit->eSinc >= DMA_INC_Max) return 1;
    if (ptInit->eDestMsize >= DMA_MSIZE_Max) return 1;
    if (ptInit->eSrcMsize >= DMA_MSIZE_Max) return 1;
    if (ptInit->eTtfc >= DMA_TT_FC_Max) return 1;
    if (ptInit->ePrior >= DMA_CH_PRIOR_Max) return 1;
    
    // SARx must be aligned to CTLx.SRC_TR_WIDTH
    if ((ptInit->Sar & ((1<<ptInit->eSrcTrWidth)-1)) != 0) // SARx %  1/2/4 bytes
        return 1;
    // DARx must be aligned to CTLx.DST_TR_WIDTH
    if ((ptInit->Dar & ((1<<ptInit->eDstTrWidth)-1)) != 0) // DARx % 1/2/4 bytes
        return 1;
    
    MSQ_DMA->SAR0 = ptInit->Sar;
    MSQ_DMA->DAR0 = ptInit->Dar;
    
    u32Temp = MSQ_DMA->LLP0;
    u32Temp &= ~(DMA_LLP_LOC_Msk);
    u32Temp |= (ptInit->Loc << DMA_LLP_LOC_Pos);
    MSQ_DMA->LLP0 = u32Temp;
    
    u32Temp = MSQ_DMA->CTL0_L;
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
    MSQ_DMA->CTL0_L = u32Temp;
    
    u32Temp = MSQ_DMA->CTL0_H;
    u32Temp &= ~( DMA_CTL_H_BLOCK_TS_Msk |
                  DMA_CTL_H_DONE_Msk );
    u32Temp |= ( (ptInit->BlockTs << DMA_CTL_H_BLOCK_TS_Pos) |
                 (ptInit->Done << DMA_CTL_H_DONE_Pos) );
    MSQ_DMA->CTL0_H = u32Temp;
    
    u32Temp = MSQ_DMA->CFG0_L;
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
    MSQ_DMA->CFG0_L = u32Temp;
    
    u32Temp = MSQ_DMA->CFG0_H;
    u32Temp &= ~( DMA_CFG_H_FCMODE_Msk |
                  DMA_CFG_H_FIFO_MODE_Msk |
                  DMA_CFG_H_PROTCTL_Msk |
                  DMA_CFG_H_SRC_PER_Msk |
                  DMA_CFG_H_DEST_PER_Msk );
    u32Temp |= ( (ptInit->Fcmode << DMA_CFG_H_FCMODE_Pos) |
                 (ptInit->FifoMode << DMA_CFG_H_FIFO_MODE_Pos) |
                 (ptInit->Protctl << DMA_CFG_H_PROTCTL_Pos) |
                 (ptInit->SrcPer << DMA_CFG_H_SRC_PER_Pos) |
                 (ptInit->DestPer << DMA_CFG_H_DEST_PER_Pos) );
    MSQ_DMA->CFG0_H = u32Temp;
    
    u32Temp = MSQ_DMA->SGR0;
    u32Temp &= ~( DMA_SGR_SGI_Msk |
                  DMA_SGR_SGC_Msk );
    u32Temp |= ( (ptInit->Sgi << DMA_SGR_SGI_Pos) |
                 (ptInit->Sgc << DMA_SGR_SGC_Pos) );
    MSQ_DMA->SGR0 = u32Temp;
    
    u32Temp = MSQ_DMA->DSR0;
    u32Temp &= ~( DMA_DSR_DSI_Msk |
                  DMA_DSR_DSC_Msk );
    u32Temp |= ( (ptInit->Dsi << DMA_DSR_DSI_Pos) |
                 (ptInit->Dsc << DMA_DSR_DSC_Pos) );
    MSQ_DMA->DSR0 = u32Temp;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Msq_Dma_Enable
*
* DESCRIPTION:
*   1. Enable DMA channel 0
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
uint32_t Hal_Msq_Dma_Enable_impl(void)
{
    if ((MSQ_DMA->CFG & DMA_CFG_DMA_EN_Msk) == 0)
    {
        MSQ_DMA->CFG = DMA_CFG_DMA_EN;
    }
    
    MSQ_DMA->CH_EN = (MSQ_DMA_CH_EN_CH_EN_Msk | MSQ_DMA_CH_EN_CH_EN_WE_Msk);
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Msq_Dma_Abort
*
* DESCRIPTION:
*   1. Abort transfer of DMA channel 0
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
uint32_t Hal_Msq_Dma_Abort_impl(void)
{
    // Refer to 7.7 Disabling a Channel Prior to Transfer Completion
    
    // Write 1 to CFGx.CH_SUSP
    MSQ_DMA->CFG0_L |= DMA_CFG_L_CH_SUSP_Msk;
    
    // Polling CFGx.FIFO_EMPTY
    while ((MSQ_DMA->CFG0_L & DMA_CFG_L_FIFO_EMPTY_Msk) == 0) ;
    
    // Clear CFGx.CH_SUSP
    MSQ_DMA->CFG0_L &= ~(DMA_CFG_L_CH_SUSP_Msk);
    
    // Clear ChEnReg.CH_EN
    MSQ_DMA->CH_EN = (1 << MSQ_DMA_CH_EN_CH_EN_WE_Pos);
    
    // Confirm
    if (Hal_Msq_Dma_Check())
        return 1;
    else
        return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Msq_Dma_IntMask
*
* DESCRIPTION:
*  1. Mask/Unmask interrupts of DMA channel 0
*
* CALLS
*
* PARAMETERS
*   1. u8Bmp : bitmap of Mask* Interrupt Registers
*              1 = Unmask
*              0 = Mask
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Msq_Dma_IntMask_impl(uint8_t u8Bmp)
{
    uint8_t u8Tfr  = (u8Bmp & DMA_BMP_TFR) ? 1 : 0;
    uint8_t u8Blk  = (u8Bmp & DMA_BMP_BLK) ? 1 : 0;
    uint8_t u8Srct = (u8Bmp & DMA_BMP_SRC_TRAN) ? 1 : 0;
    uint8_t u8Dstt = (u8Bmp & DMA_BMP_DST_TRAN) ? 1 : 0;
    uint8_t u8Err  = (u8Bmp & DMA_BMP_ERR) ? 1 : 0;
    
    MSQ_DMA->MSK_TFR      = (u8Tfr << MSQ_DMA_MSK_TFR_INT_MSK_Pos) |
                            (1 << MSQ_DMA_MSK_TFR_INT_MSK_WE_Pos);
    
    MSQ_DMA->MSK_BLK      = (u8Blk << MSQ_DMA_MSK_BLK_INT_MSK_Pos) |
                            (1 << MSQ_DMA_MSK_BLK_INT_MSK_WE_Pos);
    
    MSQ_DMA->MSK_SRC_TRAN = (u8Srct << MSQ_DMA_MSK_SRC_TRAN_INT_MSK_Pos) |
                            (1 << MSQ_DMA_MSK_SRC_TRAN_INT_MSK_WE_Pos);
    
    MSQ_DMA->MSK_DST_TRAN = (u8Dstt << MSQ_DMA_MSK_DST_TRAN_INT_MSK_Pos) |
                            (1 << MSQ_DMA_MSK_DST_TRAN_INT_MSK_WE_Pos);
    
    MSQ_DMA->MSK_ERR      = (u8Err << MSQ_DMA_MSK_ERR_INT_MSK_Pos) |
                            (1 << MSQ_DMA_MSK_ERR_INT_MSK_WE_Pos);
}

/*************************************************************************
* FUNCTION:
*  Hal_Msq_Dma_IntEn
*
* DESCRIPTION:
*   1. Enable DMA interrupts
*
* CALLS
*
* PARAMETERS
*   1. eEnable : 1 = Enable
*                0 = Disable
*
* RETURNS
*   0 : setting complete
*   1 : error
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Msq_Dma_IntEn_impl(E_FUNC_ST eEnable)
{
    /* Write IER, enable interrupt */
    if(eEnable == ENABLE)
    {
        Hal_Vic_IntInit(MSQ_DMA_IRQn, VIC_TRIG_LEVEL_HIGH, IRQ_PRIORITY_DMA);
        
        /* Module part: Enable module */
        MSQ_DMA->CTL0_L = MSQ_DMA->CTL0_L | DMA_CTL_L_INT_EN;
    }
    else
    {
        /* Module part: Disable module */
        MSQ_DMA->CTL0_L = MSQ_DMA->CTL0_L & ~(DMA_CTL_L_INT_EN);

        Hal_Vic_IntDeInit(MSQ_DMA_IRQn);
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Msq_Dma_IntClear
*
* DESCRIPTION:
*   1. Clear the specified interrupts of DMA channnel 0
*
* CALLS
*
* PARAMETERS
*   1. u8Bmp : bitmap of Clear* Interrupt Registers
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
uint32_t Hal_Msq_Dma_IntClear_impl(uint8_t u8Bmp)
{
    uint32_t u32IntSts = 0;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;

    uint8_t u8Tfr  = (u8Bmp & DMA_BMP_TFR) ? 1 : 0;
    uint8_t u8Blk  = (u8Bmp & DMA_BMP_BLK) ? 1 : 0;
    uint8_t u8Srct = (u8Bmp & DMA_BMP_SRC_TRAN) ? 1 : 0;
    uint8_t u8Dstt = (u8Bmp & DMA_BMP_DST_TRAN) ? 1 : 0;
    uint8_t u8Err  = (u8Bmp & DMA_BMP_ERR) ? 1 : 0;
    
    uint8_t u8ReloadSrc = (MSQ_DMA->CFG0_L & DMA_CFG_L_RELOAD_SRC_Msk) >> DMA_CFG_L_RELOAD_SRC_Pos;
    uint8_t u8ReloadDst = (MSQ_DMA->CFG0_L & DMA_CFG_L_RELOAD_DST_Msk) >> DMA_CFG_L_RELOAD_DST_Pos;
    
    // since msq_dma need cross bridge, must confirm the bits are cleared
    // clear interrupt
    MSQ_DMA->CLR_TFR      = u8Tfr;
    MSQ_DMA->CLR_BLK      = u8Blk;
    MSQ_DMA->CLR_SRC_TRAN = u8Srct;
    MSQ_DMA->CLR_DST_TRAN = u8Dstt;
    MSQ_DMA->CLR_ERR      = u8Err;

    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32MsqDmaTimeoutMs;
    
    // confirm
    u32TickStart = Hal_Tick_Diff(0);
    do
    {
        u32IntSts = ( ((MSQ_DMA->STS_TFR & u8Tfr) << DMA_TFR_POS) |
                      ((MSQ_DMA->STS_SRC_TRAN & u8Srct) << DMA_SRC_TRAN_POS) |
                      ((MSQ_DMA->STS_DST_TRAN & u8Dstt) << DMA_DST_TRAN_POS) |
                      ((MSQ_DMA->STS_ERR & u8Err) << DMA_ERR_POS) );
        
        if (!u8ReloadSrc && !u8ReloadDst) // note: when AR enabled, seems cannot clear block interrupt status
            u32IntSts |= ((MSQ_DMA->STS_BLK & u8Blk) << DMA_BLK_POS);
        
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    } while (u32IntSts & u8Bmp); // bits to be cleared should not be set
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Msq_Dma_IntClearAll
*
* DESCRIPTION:
*   1. Clear any pending interrupts of DMA channnel 0
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
uint32_t Hal_Msq_Dma_IntClearAll_impl(void)
{
    uint32_t IntSts = 0;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    
    uint8_t u8ReloadSrc = (MSQ_DMA->CFG0_L & DMA_CFG_L_RELOAD_SRC_Msk) >> DMA_CFG_L_RELOAD_SRC_Pos;
    uint8_t u8ReloadDst = (MSQ_DMA->CFG0_L & DMA_CFG_L_RELOAD_DST_Msk) >> DMA_CFG_L_RELOAD_DST_Pos;
    
    // since msq_dma need cross bridge, must confirm the bits are cleared
    // clear interrupt
    MSQ_DMA->CLR_TFR      = 0x00000001;
    MSQ_DMA->CLR_BLK      = 0x00000001;
    MSQ_DMA->CLR_SRC_TRAN = 0x00000001;
    MSQ_DMA->CLR_DST_TRAN = 0x00000001;
    MSQ_DMA->CLR_ERR      = 0x00000001;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32MsqDmaTimeoutMs;
    
    // confirm
    u32TickStart = Hal_Tick_Diff(0);
    do
    {
        IntSts = (MSQ_DMA->STS_TFR | MSQ_DMA->STS_SRC_TRAN | MSQ_DMA->STS_DST_TRAN | MSQ_DMA->STS_ERR);
        
        if (!u8ReloadSrc && !u8ReloadDst) // note: when AR enabled, seems cannot clear block interrupt status
            IntSts |= MSQ_DMA->STS_BLK;
        
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    } while (IntSts);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Msq_Dma_CallBackFuncSet
*
* DESCRIPTION:
*   1. Setup DMA callback function
*
* CALLS
*
* PARAMETERS
*   1. tFunc : Callback function
*
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Msq_Dma_CallBackSet_impl(T_Dma_CallBack tFunc)
{
    g_tHalMsqDmaCallBack = tFunc;
}




/*
 *************************************************************************
 *                           Private Functions
 *************************************************************************
 */

