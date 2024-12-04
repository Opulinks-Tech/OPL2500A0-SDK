/* *****************************************************************************
 *  Copyright 2017 - 2022, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2022
 *
 *******************************************************************************
 *
 *  @file hal_spi_dma.c
 * 
 *  @brief SPI DMA API
 * 
 *******************************************************************************/




/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "hal_spi.h"
#include "hal_spi_dma.h"
#include "hal_vic.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define DMA_MAX_BLOCKTS         4095

#define SPI_DMA_MSIZE_MAX       DMA_MSIZE_32
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef enum
{
    SPI_DMA_OFF=0,
    SPI_DMA_TX_ONLY,
    SPI_DMA_RX_ONLY,
    SPI_DMA_TRX,
} E_HAL_SPI_DMA_MODE;


typedef struct
{
    uint32_t Abort;
    uint32_t Running;
    uint32_t Mode;
    E_DMA_CHANNEL eDmaTxCh;
    E_DMA_CHANNEL eDmaRxCh;
    S_SPI_MST_Reg_t *pSpi;
    E_SpiSlave_t eSlvIdx;
    Hal_SpiDmaSetupCb fpDmaSetupCb;
    Hal_SpiDmaDoneCb fpDmaDoneCb;
    E_DMA_MSIZE eBlockSize;
    uint32_t u32aBufferSize;
    uint8_t *pu8aTxBuffer;
    uint8_t *pu8aRxBuffer;
} S_HAL_SPI_DMA_CTRL;
/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/


void Hal_Spi_DmaCallbackTxOnly_Spi0(void);
void Hal_Spi_DmaCallbackTxOnly_Spi1(void);
void Hal_Spi_DmaCallbackTxOnly_Spi2(void);
void Hal_Spi_DmaCallbackTxOnly_Spi3(void);
void Hal_Spi_DmaCallbackTxOnly(E_SpiIdx_t eSpiIdx);
void Hal_Spi_DmaCallbackRxOnly_Spi0(void);
void Hal_Spi_DmaCallbackRxOnly_Spi1(void);
void Hal_Spi_DmaCallbackRxOnly_Spi2(void);
void Hal_Spi_DmaCallbackRxOnly_Spi3(void);
void Hal_Spi_DmaCallbackRxOnly(E_SpiIdx_t eSpiIdx);
void Hal_Spi_DmaCallbackTrx_Spi0(void);
void Hal_Spi_DmaCallbackTrx_Spi1(void);
void Hal_Spi_DmaCallbackTrx_Spi2(void);
void Hal_Spi_DmaCallbackTrx_Spi3(void);
void Hal_Spi_DmaCallbackTrx(E_SpiIdx_t eSpiIdx);
void Hal_Spi_DmaTrxNext(E_SpiIdx_t eSpiIdx);
void _Hal_Spi_DmaTxOnly(E_SpiIdx_t eSpiIdx);
void _Hal_Spi_DmaRxOnly(E_SpiIdx_t eSpiIdx);
void _Hal_Spi_DmaTrx(E_SpiIdx_t eSpiIdx);
void _Hal_Spi_DmaFlushDummy(E_SpiIdx_t eSpiIdx);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
uint32_t g_Hal_SpiTxDummy=0x5A;
S_HAL_SPI_DMA_CTRL g_Hal_SpiDmaCtrl[SPI_IDX_MAX];
/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
const uint16_t g_u16BlockSizeMap[SPI_DMA_MSIZE_MAX + 1] = {
    /* DMA_MSIZE_1   */ 1,
    /* DMA_MSIZE_4   */ 4,
    /* DMA_MSIZE_8   */ 8,
    /* DMA_MSIZE_16  */ 16,
    /* DMA_MSIZE_32  */ 32
};
 
const T_Dma_CallBack g_Hal_SpiDmaCallbackTxOnly[SPI_IDX_MAX] = {
    Hal_Spi_DmaCallbackTxOnly_Spi0,
    Hal_Spi_DmaCallbackTxOnly_Spi1,
    Hal_Spi_DmaCallbackTxOnly_Spi2,
    Hal_Spi_DmaCallbackTxOnly_Spi3
};
const T_Dma_CallBack g_Hal_SpiDmaCallbackRxOnly[SPI_IDX_MAX] = {
    Hal_Spi_DmaCallbackRxOnly_Spi0,
    Hal_Spi_DmaCallbackRxOnly_Spi1,
    Hal_Spi_DmaCallbackRxOnly_Spi2,
    Hal_Spi_DmaCallbackRxOnly_Spi3
};
const T_Dma_CallBack g_Hal_SpiDmaCallbackTrx[SPI_IDX_MAX] = {
    Hal_Spi_DmaCallbackTrx_Spi0,
    Hal_Spi_DmaCallbackTrx_Spi1,
    Hal_Spi_DmaCallbackTrx_Spi2,
    Hal_Spi_DmaCallbackTrx_Spi3
};
/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
/*************************************************************************
* FUNCTION:
*  Hal_Spi_DmaInit
*
* DESCRIPTION:
*   1. Init SPI DMA
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx   : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx   : Index of SPI slave. refer to E_SpiSlave_t
*   3. eTxDmaCh  : DMA channel for TX. Set to DMA_Channel_Max when not used
*   4. eRxDmaCh  : DMA channel for RX. Set to DMA_Channel_Max when not used
*   5. fpTrxDoneCb : Callback function when buffer TRX done 
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_DmaInit(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eTxDmaCh, E_DMA_CHANNEL eRxDmaCh, Hal_SpiDmaSetupCb fpSetupCb, Hal_SpiDmaDoneCb fpDmaDoneCb)
{
    E_APS_DMA_HW_ITF ePeriItfTx, ePeriItfRx;
    S_SPI_MST_Reg_t *pSpi = NULL;
    S_SPI_Config_t tCfg;
    memset(&tCfg, 0, sizeof(S_SPI_Config_t));

    if (g_Hal_SpiDmaCtrl[eSpiIdx].Running)
        goto error;
    
    memset(&g_Hal_SpiDmaCtrl[eSpiIdx], 0, sizeof(S_HAL_SPI_DMA_CTRL));
    g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh = DMA_Channel_Max;
    g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh = DMA_Channel_Max;
    
    if ((eTxDmaCh > DMA_Channel_Max) || (eRxDmaCh > DMA_Channel_Max) || (eTxDmaCh == eRxDmaCh))
        goto error;
    
    switch (eSpiIdx)
    {
        case SPI_IDX_1:
            pSpi = SPI1;
            ePeriItfTx = APS_DMA_ITF_SPI1_TX;
            ePeriItfRx = APS_DMA_ITF_SPI1_RX;
            break;
        case SPI_IDX_2:
            pSpi = (S_SPI_MST_Reg_t *)SPI2;
            ePeriItfTx = APS_DMA_ITF_SPI2_TX;
            ePeriItfRx = APS_DMA_ITF_SPI2_RX;
            break;
        case SPI_IDX_3:
            pSpi = SPI3;
            ePeriItfTx = APS_DMA_ITF_SPI3_TX;
            ePeriItfRx = APS_DMA_ITF_SPI3_RX;
            break;
        default:
            goto error;
    }
    
    Hal_Dma_Init();   
    // config DMA
    
    if (eTxDmaCh < DMA_Channel_Max)
    {
        S_DMA_Init_t tTxDma;
        if (Hal_Dma_Check(eTxDmaCh))
            goto error;
        
        // setup TX DMA configurations
        memset(&tTxDma, 0, sizeof(S_DMA_Init_t));
        //tTxDma.Sar
        tTxDma.Dar = (uint32_t) &(pSpi->DR[0]);
        tTxDma.eTtfc = DMA_TT_m2p_FC_Dma;
        tTxDma.eSrcTrWidth = DMA_TR_WIDTH_8_bits;
        tTxDma.eDstTrWidth = DMA_TR_WIDTH_8_bits;
        tTxDma.eSrcMsize = DMA_MSIZE_16;
        tTxDma.eDestMsize = DMA_MSIZE_16;
        tTxDma.eSinc = DMA_INC_Increment;
        tTxDma.eDinc = DMA_INC_No_Change;
        //tTxDma.BlockTs
        tTxDma.DestPer = ePeriItfTx;
        Hal_Dma_Config(eTxDmaCh, &tTxDma);
        g_Hal_SpiDmaCtrl[eSpiIdx].Mode = SPI_DMA_TX_ONLY;
        tCfg.eTmod = SPI_TMOD_TX_ONLY;
    }
    
    if (eRxDmaCh < DMA_Channel_Max)
    {
        S_DMA_Init_t tRxDma;
        if (Hal_Dma_Check(eRxDmaCh))
            goto error;
        // setup RX DMA configurations
        memset(&tRxDma, 0, sizeof(S_DMA_Init_t));
        tRxDma.Sar = (uint32_t) &(pSpi->DR[0]);
        //tRxDma.Dar
        tRxDma.eTtfc = DMA_TT_p2m_FC_Dma;
        tRxDma.eSrcTrWidth = DMA_TR_WIDTH_8_bits;
        tRxDma.eDstTrWidth = DMA_TR_WIDTH_8_bits;
        tRxDma.eSrcMsize = DMA_MSIZE_16;
        tRxDma.eDestMsize = DMA_MSIZE_16;
        tRxDma.eSinc = DMA_INC_No_Change;
        tRxDma.eDinc = DMA_INC_Increment;
        //tRxDma.BlockTs
        tRxDma.SrcPer = ePeriItfRx;
        Hal_Dma_Config(eRxDmaCh, &tRxDma);
        
        if (g_Hal_SpiDmaCtrl[eSpiIdx].Mode == SPI_DMA_TX_ONLY)
        {
            g_Hal_SpiDmaCtrl[eSpiIdx].Mode = SPI_DMA_TRX;
            tCfg.eTmod = SPI_TMOD_TRX;
        }
        else
        {
            g_Hal_SpiDmaCtrl[eSpiIdx].Mode = SPI_DMA_RX_ONLY;
            tCfg.eTmod = SPI_TMOD_RX_ONLY;
        }
        eSpiIdx = eSpiIdx;
    }
    
    switch (g_Hal_SpiDmaCtrl[eSpiIdx].Mode)
    {
        case SPI_DMA_TX_ONLY:
            Hal_Dma_CallBackSet(eTxDmaCh, g_Hal_SpiDmaCallbackTxOnly[eSpiIdx]);
            Hal_Dma_IntMask(eTxDmaCh, (DMA_BMP_TFR));
            Hal_Dma_IntClear(eTxDmaCh, (DMA_BMP_TFR));
            Hal_Dma_IntEn(ENABLE, eTxDmaCh);
            break;
        case SPI_DMA_RX_ONLY:
            Hal_Dma_CallBackSet(eRxDmaCh, g_Hal_SpiDmaCallbackRxOnly[eSpiIdx]);
            Hal_Dma_IntMask(eRxDmaCh, (DMA_BMP_TFR));
            Hal_Dma_IntClear(eRxDmaCh, (DMA_BMP_TFR));
            Hal_Dma_IntEn(ENABLE, eRxDmaCh);
            break;
        default: //SPI_DMA_TRX
            Hal_Dma_CallBackSet(eRxDmaCh, g_Hal_SpiDmaCallbackTrx[eSpiIdx]);
            Hal_Dma_IntMask(eRxDmaCh, (DMA_BMP_TFR));
            Hal_Dma_IntClear(eRxDmaCh, (DMA_BMP_TFR));
            Hal_Dma_IntEn(ENABLE, eRxDmaCh);
            break;
    }
    
    // Config SPI
    tCfg.eSpi_frf = SPI_FRF_STD;
    tCfg.eDfs_32 = SPI_DFS_08_bit;
    if (eSpiIdx == SPI_IDX_2)
    {
        Hal_Spi_Slave_Config(SPI_IDX_2, &tCfg);
    }
    else
    {
        Hal_Spi_Master_Config(eSpiIdx, &tCfg);
        Hal_Spi_Slave_Select_Enable(eSpiIdx, eSlvIdx, 1);
    }
    
    g_Hal_SpiDmaCtrl[eSpiIdx].pSpi = pSpi;
    g_Hal_SpiDmaCtrl[eSpiIdx].eSlvIdx = eSlvIdx;
    g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh = eTxDmaCh;
    g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh = eRxDmaCh;
    g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaSetupCb = fpSetupCb;
    g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaDoneCb = fpDmaDoneCb;
    
    return 0;
    
error:
    return 1;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Transmit_Dma
*
* DESCRIPTION:
*   1. TX SPI data with DMA
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx   : Index of SPI. refer to E_SpiIdx_t
*   2. pu8TxData : TX data pointer
*   3. u32Size   : DMA channel for RX. Set to DMA_Channel_Max when not used
*   
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Transmit_Dma(E_SpiIdx_t eSpiIdx, uint8_t *pu8TxData, uint32_t u32Size, E_DMA_MSIZE eBlockSize)
{
    S_DMA_Channel_t *DMA_TX_CH = g_pscaHalDmaCHx[g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh];
    uint32_t temp, temp_reg;
    uint8_t u8TxDtLv;
    
    if ((eSpiIdx < SPI_IDX_1) || (eSpiIdx > SPI_IDX_3))
        return 1;
    
    if (g_Hal_SpiDmaCtrl[eSpiIdx].Mode == SPI_DMA_RX_ONLY)
        return 1;
    if (eBlockSize > SPI_DMA_MSIZE_MAX)
        return 1;
    if (g_Hal_SpiDmaCtrl[eSpiIdx].Running)
        return 1;
    if (u32Size > DMA_MAX_BLOCKTS)
        return 1;
    
    g_Hal_SpiDmaCtrl[eSpiIdx].Running = 1;
    g_Hal_SpiDmaCtrl[eSpiIdx].pu8aTxBuffer = pu8TxData;
    g_Hal_SpiDmaCtrl[eSpiIdx].pu8aRxBuffer = NULL;
    g_Hal_SpiDmaCtrl[eSpiIdx].u32aBufferSize = u32Size;
    g_Hal_SpiDmaCtrl[eSpiIdx].eBlockSize = eBlockSize;
    
    DMA_TX_CH->SAR = (uint32_t)pu8TxData;
    
    temp = DMA_TX_CH->CTL_H;
    temp_reg = (temp & DMA_CTL_H_BLOCK_TS_Msk) >> DMA_CTL_H_BLOCK_TS_Pos;
    if (temp_reg != u32Size)
    {
        temp = (temp & ~DMA_CTL_H_BLOCK_TS_Msk) | (u32Size << DMA_CTL_H_BLOCK_TS_Pos);
        DMA_TX_CH->CTL_H = temp;
    }
    temp = DMA_TX_CH->CTL_L;
    temp_reg = (temp & DMA_CTL_L_DEST_MSIZE_Msk) >> DMA_CTL_L_DEST_MSIZE_Pos;
    if (temp_reg != eBlockSize)
    {
        temp = (temp & ~(DMA_CTL_L_DEST_MSIZE_Msk|DMA_CTL_L_SRC_MSIZE_Msk)) | (eBlockSize << DMA_CTL_L_DEST_MSIZE_Pos) | (eBlockSize << DMA_CTL_L_SRC_MSIZE_Pos);
        DMA_TX_CH->CTL_L = temp;
    }
    
    // enable Transmit DMA
    u8TxDtLv = g_u16BlockSizeMap[eBlockSize];
    Hal_Spi_Dma_Config(eSpiIdx, 1, 0, u8TxDtLv, 0);
    Hal_Dma_Enable(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh); // TX DMA
    
    if (g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaSetupCb)
    {
        g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaSetupCb(eSpiIdx, pu8TxData, NULL, u32Size);
    }
    return 0;
}
/*************************************************************************
* FUNCTION:
*  Hal_Spi_Receive_Dma
*
* DESCRIPTION:
*   1. RX SPI data with DMA
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx   : Index of SPI. refer to E_SpiIdx_t
*   2. pu8RxData : TX data pointer
*   3. u32Size   : DMA channel for RX. Set to DMA_Channel_Max when not used
*   
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Receive_Dma(E_SpiIdx_t eSpiIdx, uint8_t *pu8RxData, uint32_t u32Size, E_DMA_MSIZE eBlockSize)
{
    S_DMA_Channel_t *DMA_RX_CH = g_pscaHalDmaCHx[g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh];
    uint32_t temp, temp_reg;
    uint8_t u8RxDtLv;
    if ((eSpiIdx < SPI_IDX_1) || (eSpiIdx > SPI_IDX_3))
        return 1;
    if (g_Hal_SpiDmaCtrl[eSpiIdx].Mode == SPI_DMA_TX_ONLY)
        return 1;
    if (g_Hal_SpiDmaCtrl[eSpiIdx].Running)
        return 1;
    if (eBlockSize > SPI_DMA_MSIZE_MAX)
        return 1;
    if (u32Size > DMA_MAX_BLOCKTS)
        return 1;
    
    g_Hal_SpiDmaCtrl[eSpiIdx].Running = 1;
    g_Hal_SpiDmaCtrl[eSpiIdx].pu8aTxBuffer = NULL;
    g_Hal_SpiDmaCtrl[eSpiIdx].pu8aRxBuffer = pu8RxData;
    g_Hal_SpiDmaCtrl[eSpiIdx].u32aBufferSize = u32Size;
    g_Hal_SpiDmaCtrl[eSpiIdx].eBlockSize = eBlockSize;
    
    DMA_RX_CH->DAR = (uint32_t)pu8RxData;
    
    temp = DMA_RX_CH->CTL_H;
    temp_reg = (temp & DMA_CTL_H_BLOCK_TS_Msk)>> DMA_CTL_H_BLOCK_TS_Pos;
    if (temp_reg != u32Size)
    {
        temp = (temp & ~DMA_CTL_H_BLOCK_TS_Msk) | (u32Size << DMA_CTL_H_BLOCK_TS_Pos);
        DMA_RX_CH->CTL_H = temp;
    }
    temp = DMA_RX_CH->CTL_L;
    temp_reg = (temp & DMA_CTL_L_DEST_MSIZE_Msk) >> DMA_CTL_L_DEST_MSIZE_Pos;
    if (temp_reg != eBlockSize)
    {
        temp = (temp & ~(DMA_CTL_L_DEST_MSIZE_Msk|DMA_CTL_L_SRC_MSIZE_Msk)) | (eBlockSize << DMA_CTL_L_DEST_MSIZE_Pos) | (eBlockSize << DMA_CTL_L_SRC_MSIZE_Pos);
        DMA_RX_CH->CTL_L = temp;
    }

    // Data (using DMA)
    // enable Receive DMA
    u8RxDtLv = g_u16BlockSizeMap[eBlockSize] - 1;
    Hal_Spi_Dma_Config(eSpiIdx, 0, 1, 0, u8RxDtLv);
    Hal_Dma_Enable(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh); // RX DMA
    
    if (g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaSetupCb)
    {
        g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaSetupCb(eSpiIdx, NULL, pu8RxData, u32Size);
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_TransmitReceive_Dma
*
* DESCRIPTION:
*   1. RX SPI data with DMA
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx   : Index of SPI. refer to E_SpiIdx_t
*   2. pu8RxData : TX data pointer
*   3. u32Size   : DMA channel for RX. Set to DMA_Channel_Max when not used
*   
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_TransmitReceive_Dma(E_SpiIdx_t eSpiIdx, uint8_t *pu8TxData, uint8_t *pu8RxData, uint32_t u32Size, E_DMA_MSIZE eBlockSize)
{
    S_DMA_Channel_t *DMA_TX_CH = g_pscaHalDmaCHx[g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh];
    S_DMA_Channel_t *DMA_RX_CH = g_pscaHalDmaCHx[g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh];
    uint32_t temp, temp_reg;
    uint8_t u8TxDtLv, u8RxDtLv;
    
    if ((eSpiIdx < SPI_IDX_1) || (eSpiIdx > SPI_IDX_3))
        return 1;
    if (g_Hal_SpiDmaCtrl[eSpiIdx].Mode != SPI_DMA_TRX)
        return 1;
    if (g_Hal_SpiDmaCtrl[eSpiIdx].Running)
        return 1;
    if (eBlockSize > SPI_DMA_MSIZE_MAX)
        return 1;
    if (u32Size > DMA_MAX_BLOCKTS)
        return 1;
    
    g_Hal_SpiDmaCtrl[eSpiIdx].Running = 1;
    g_Hal_SpiDmaCtrl[eSpiIdx].pu8aTxBuffer = pu8TxData;
    g_Hal_SpiDmaCtrl[eSpiIdx].pu8aRxBuffer = pu8RxData;
    g_Hal_SpiDmaCtrl[eSpiIdx].u32aBufferSize = u32Size;
    g_Hal_SpiDmaCtrl[eSpiIdx].eBlockSize = eBlockSize;

    DMA_TX_CH->SAR = (uint32_t)pu8TxData;
    DMA_RX_CH->DAR = (uint32_t)pu8RxData;
    
    temp = DMA_TX_CH->CTL_H;
    temp_reg = (temp & DMA_CTL_H_BLOCK_TS_Msk) >> DMA_CTL_H_BLOCK_TS_Pos;
    if (temp_reg != u32Size)
    {
        temp = (temp & ~DMA_CTL_H_BLOCK_TS_Msk) | (u32Size << DMA_CTL_H_BLOCK_TS_Pos);
        DMA_TX_CH->CTL_H = temp;
    }
    
    temp = DMA_RX_CH->CTL_H;
    temp_reg = (temp & DMA_CTL_H_BLOCK_TS_Msk)>> DMA_CTL_H_BLOCK_TS_Pos;
    if (temp_reg != u32Size)
    {
        temp = (temp & ~DMA_CTL_H_BLOCK_TS_Msk) | (u32Size << DMA_CTL_H_BLOCK_TS_Pos);
        DMA_RX_CH->CTL_H = temp;
    }
    
    temp = DMA_TX_CH->CTL_L;
    temp_reg = (temp & DMA_CTL_L_DEST_MSIZE_Msk) >> DMA_CTL_L_DEST_MSIZE_Pos;
    if (temp_reg != eBlockSize)
    {
        temp = (temp & ~(DMA_CTL_L_DEST_MSIZE_Msk|DMA_CTL_L_SRC_MSIZE_Msk)) | (eBlockSize << DMA_CTL_L_DEST_MSIZE_Pos) | (eBlockSize << DMA_CTL_L_SRC_MSIZE_Pos);
        DMA_TX_CH->CTL_L = temp;
    }
    temp = DMA_RX_CH->CTL_L;
    temp_reg = (temp & DMA_CTL_L_DEST_MSIZE_Msk) >> DMA_CTL_L_DEST_MSIZE_Pos;
    if (temp_reg != eBlockSize)
    {
        temp = (temp & ~(DMA_CTL_L_DEST_MSIZE_Msk|DMA_CTL_L_SRC_MSIZE_Msk)) | (eBlockSize << DMA_CTL_L_DEST_MSIZE_Pos) | (eBlockSize << DMA_CTL_L_SRC_MSIZE_Pos);
        DMA_RX_CH->CTL_L = temp;
    }
    
    // Data (using DMA)
    // enable Receive DMA
    u8TxDtLv = g_u16BlockSizeMap[eBlockSize];
    u8RxDtLv = g_u16BlockSizeMap[eBlockSize] - 1;
    Hal_Spi_Dma_Config(eSpiIdx, 1, 1, u8TxDtLv, u8RxDtLv);
    Hal_Dma_Enable(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh); // RX DMA
    Hal_Dma_Enable(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh); // TX DMA
    
    if (g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaSetupCb)
    {
        g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaSetupCb(eSpiIdx, pu8TxData, pu8RxData, u32Size);
    }
    return 0;
}

void Hal_Spi_DmaAbort(E_SpiIdx_t eSpiIdx)
{
    bool bAbort = false;
    if (!g_Hal_SpiDmaCtrl[eSpiIdx].Running)
        return;
    
    g_Hal_SpiDmaCtrl[eSpiIdx].Abort = 1;
    
    if (g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh < DMA_Channel_Max)
    {
        if (Hal_Dma_Check(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh))
            bAbort = true;
        Hal_Dma_Abort(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh);
    }
    if (g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh < DMA_Channel_Max)
    {
        if (Hal_Dma_Check(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh))
            bAbort = true;
        Hal_Dma_Abort(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh);
    }
    if (bAbort && g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaDoneCb)
    {
        g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaDoneCb(eSpiIdx, SPI_DMA_STS_ABORT);
    }
    
    Hal_Spi_Dma_Config(eSpiIdx, 0, 0, 0, 0);
    Hal_Spi_ResetFifo(eSpiIdx);
    
    g_Hal_SpiDmaCtrl[eSpiIdx].pu8aTxBuffer = NULL;
    g_Hal_SpiDmaCtrl[eSpiIdx].pu8aRxBuffer = NULL;
    g_Hal_SpiDmaCtrl[eSpiIdx].u32aBufferSize = 0;
    g_Hal_SpiDmaCtrl[eSpiIdx].Running = 0;
    g_Hal_SpiDmaCtrl[eSpiIdx].Abort = 0;
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

void Hal_Spi_DmaCallbackTxOnly_Spi0(void){   /* Dummy, not support */               }
void Hal_Spi_DmaCallbackTxOnly_Spi1(void){    Hal_Spi_DmaCallbackTxOnly(SPI_IDX_1); }
void Hal_Spi_DmaCallbackTxOnly_Spi2(void){    Hal_Spi_DmaCallbackTxOnly(SPI_IDX_2); }
void Hal_Spi_DmaCallbackTxOnly_Spi3(void){    Hal_Spi_DmaCallbackTxOnly(SPI_IDX_3); }
void Hal_Spi_DmaCallbackTxOnly(E_SpiIdx_t eSpiIdx)
{
    if (DMA->STS_TFR & (1 << g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh))
    {
        Hal_Dma_IntClear(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaTxCh, DMA_BMP_TFR);
        if (g_Hal_SpiDmaCtrl[eSpiIdx].Abort)
        {
            //Do nothing, left for task level
        }
        else
        {
            g_Hal_SpiDmaCtrl[eSpiIdx].Running = 0;
            if (g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaDoneCb)
            {
                g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaDoneCb(eSpiIdx, SPI_DMA_STS_DONE);
            }
        }
    }
    Hal_Vic_IntClear(APS_DMA_IRQn);
}
void Hal_Spi_DmaCallbackRxOnly_Spi0(void){   /* Dummy, not support */               }
void Hal_Spi_DmaCallbackRxOnly_Spi1(void){    Hal_Spi_DmaCallbackRxOnly(SPI_IDX_1); }
void Hal_Spi_DmaCallbackRxOnly_Spi2(void){    Hal_Spi_DmaCallbackRxOnly(SPI_IDX_2); }
void Hal_Spi_DmaCallbackRxOnly_Spi3(void){    Hal_Spi_DmaCallbackRxOnly(SPI_IDX_3); }
void Hal_Spi_DmaCallbackRxOnly(E_SpiIdx_t eSpiIdx)
{
    if (DMA->STS_TFR & (1 << g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh))
    {
        Hal_Dma_IntClear(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh, DMA_BMP_TFR);
        if (g_Hal_SpiDmaCtrl[eSpiIdx].Abort)
        {
            //Do nothing, left for task level
        }
        else
        {
            g_Hal_SpiDmaCtrl[eSpiIdx].Running = 0;
            if (g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaDoneCb)
            {
                g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaDoneCb(eSpiIdx, SPI_DMA_STS_DONE);
            }
        }
    }
    Hal_Vic_IntClear(APS_DMA_IRQn);
}
void Hal_Spi_DmaCallbackTrx_Spi0(void){   /* Dummy, not support */              }
void Hal_Spi_DmaCallbackTrx_Spi1(void){    Hal_Spi_DmaCallbackTrx(SPI_IDX_1);   }
void Hal_Spi_DmaCallbackTrx_Spi2(void){    Hal_Spi_DmaCallbackTrx(SPI_IDX_2);   }
void Hal_Spi_DmaCallbackTrx_Spi3(void){    Hal_Spi_DmaCallbackTrx(SPI_IDX_3);   }
void Hal_Spi_DmaCallbackTrx(E_SpiIdx_t eSpiIdx)
{
    if (DMA->STS_TFR & (1 << g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh))
    {
        Hal_Dma_IntClear(g_Hal_SpiDmaCtrl[eSpiIdx].eDmaRxCh, DMA_BMP_TFR);
        if (g_Hal_SpiDmaCtrl[eSpiIdx].Abort)
        {
            //Do nothing, left for task level
        }
        else
        {
            g_Hal_SpiDmaCtrl[eSpiIdx].Running = 0;
            if (g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaDoneCb)
            {
                g_Hal_SpiDmaCtrl[eSpiIdx].fpDmaDoneCb(eSpiIdx, SPI_DMA_STS_DONE);
            }
        }
    }
    Hal_Vic_IntClear(APS_DMA_IRQn);
}
