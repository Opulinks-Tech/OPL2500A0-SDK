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
 *  @file xxx_patch.h 
 * 
 *  @brief xxx patch
 *  
 *******************************************************************************/
#ifndef __HAL_SPI_H__
#error "Please include hal_spi.h. Don't directly include hal_spi_patch.h"
#endif

#ifndef _HAL_SPI_PATCH_H_
#define _HAL_SPI_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_spi_dma.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define HAL_SPI_FIFO_LENGTH     0x20

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

typedef enum
{
    SPI_STATUS_BUSY                     = SPI_MST_SR_BUSY,
    SPI_STATUS_TX_FIFO_NOT_FULL         = SPI_MST_SR_TFNF,
    SPI_STATUS_TX_FIFO_EMPTY            = SPI_MST_SR_TFE,
    SPI_STATUS_RX_FIFO_NOT_FULL         = SPI_MST_SR_RFNE,
    SPI_STATUS_RX_FIFO_FULL             = SPI_MST_SR_RFF,
    SPI_STATUS_SLAVE_TX_ERROR           = SPI_SLV_SR_TXE,
    SPI_STATUS_MASTER_DATA_COLLISION    = SPI_MST_SR_DCOL,
    SPI_STATUS_INVALID                  = 0xFFFF,
} E_HAL_SPI_STATUS;

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
E_HAL_SPI_STATUS Hal_Spi_ReadStatus(E_SpiIdx_t eSpiIdx);
uint32_t Hal_Spi_ResetFifo(E_SpiIdx_t eSpiIdx);
void Hal_Spi_IntHandler(E_SpiIdx_t eSpiIdx);

#ifdef __cplusplus
}
#endif
#endif  /* _HAL_SPI_PATCH_H_ */
