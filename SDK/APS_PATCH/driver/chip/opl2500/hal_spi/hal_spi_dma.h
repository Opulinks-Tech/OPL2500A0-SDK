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
 *  @file hal_spi_dma.h 
 * 
 *  @brief SPI DMA API
 *  
 *******************************************************************************/

#ifndef _HAL_SPI_DMA_H_
#define _HAL_SPI_DMA_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */

#include "hal_dma.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */


typedef enum
{
    SPI_DMA_STS_DONE,
    SPI_DMA_STS_ABORT,
} E_HAL_SPI_DMA_STATUS_RESULT;

typedef void (*Hal_SpiDmaSetupCb)(E_SpiIdx_t eSpiIdx, uint8_t *pu8TxData, uint8_t *pu8RxData, uint32_t u32Size);
typedef void (*Hal_SpiDmaDoneCb)(E_SpiIdx_t eSpiIdx, E_HAL_SPI_DMA_STATUS_RESULT eResult);


/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
uint32_t Hal_Spi_DmaInit(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eTxDmaCh, E_DMA_CHANNEL eRxDmaCh, Hal_SpiDmaSetupCb fpSetupCb, Hal_SpiDmaDoneCb fpDmaDoneCb);
void Hal_Spi_DmaAbort(E_SpiIdx_t eSpiIdx);

/*
 * DMA maximum trans size = 4095 = u32Size maximum
 */
uint32_t Hal_Spi_Transmit_Dma(E_SpiIdx_t eSpiIdx, uint8_t *pu8TxData, uint32_t u32Size, E_DMA_MSIZE eBlockSize);
uint32_t Hal_Spi_Receive_Dma(E_SpiIdx_t eSpiIdx, uint8_t *pu8RxData, uint32_t u32Size, E_DMA_MSIZE eBlockSize);
uint32_t Hal_Spi_TransmitReceive_Dma(E_SpiIdx_t eSpiIdx, uint8_t *pu8TxData, uint8_t *pu8RxData, uint32_t u32Size, E_DMA_MSIZE eBlockSize);


#ifdef __cplusplus
}
#endif
#endif  /* _HAL_SPI_DMA_H_ */
