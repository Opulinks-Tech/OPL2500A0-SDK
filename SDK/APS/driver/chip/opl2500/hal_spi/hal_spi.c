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

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include "opulinks.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_spi.h"
#include "hal_tick.h"

#include "string.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define SPI_BAUDR_MIN       0x2
#define SPI_BAUDR_MAX       0xFFFE // 65534

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
T_Spi_CallBack g_tHalSpi1CallBack = NULL;
T_Spi_CallBack g_tHalSpi2CallBack = NULL;
T_Spi_CallBack g_tHalSpi3CallBack = NULL;

T__Hal_Spi_Master_Init_fp           _Hal_Spi_Master_Init            = _Hal_Spi_Master_Init_impl;
T__Hal_Spi_Slave_Init_fp            _Hal_Spi_Slave_Init             = _Hal_Spi_Slave_Init_impl;
T__Hal_Spi_Master_Data_Send_fp      _Hal_Spi_Master_Data_Send       = _Hal_Spi_Master_Data_Send_impl;
T__Hal_Spi_Slave_Data_Send_fp       _Hal_Spi_Slave_Data_Send        = _Hal_Spi_Slave_Data_Send_impl;
T__Hal_Spi_Master_Data_Recv_fp      _Hal_Spi_Master_Data_Recv       = _Hal_Spi_Master_Data_Recv_impl;
T__Hal_Spi_Slave_Data_Recv_fp       _Hal_Spi_Slave_Data_Recv        = _Hal_Spi_Slave_Data_Recv_impl;
T__Hal_Spi_Master_Byte_Reverse_fp   _Hal_Spi_Master_Byte_Reverse    = _Hal_Spi_Master_Byte_Reverse_impl;
T__Hal_Spi_Master_Cs_Assert_fp      _Hal_Spi_Master_Cs_Assert       = _Hal_Spi_Master_Cs_Assert_impl;
T__Hal_Spi_Master_Read_Freeze_fp    _Hal_Spi_Master_Read_Freeze     = _Hal_Spi_Master_Read_Freeze_impl;
T__Hal_Spi_Poll_Busy_fp             _Hal_Spi_Poll_Busy              = _Hal_Spi_Poll_Busy_impl;
T__Hal_Spi_Enable_fp                _Hal_Spi_Enable                 = _Hal_Spi_Enable_impl;

T_Hal_Spi_BaudRateGet_fp            Hal_Spi_BaudRateGet             = Hal_Spi_BaudRateGet_impl;
T_Hal_Spi_BaudRateSet_fp            Hal_Spi_BaudRateSet             = Hal_Spi_BaudRateSet_impl;
T_Hal_Spi_DividerUpdate_fp          Hal_Spi_DividerUpdate           = Hal_Spi_DividerUpdate_impl;
T_Hal_Spi_Init_fp                   Hal_Spi_Init                    = Hal_Spi_Init_impl;
T_Hal_Spi_Init_Ext_fp               Hal_Spi_Init_Ext                = Hal_Spi_Init_Ext_impl;
T_Hal_Spi_RxSampleDly_Set_fp        Hal_Spi_RxSampleDly_Set         = Hal_Spi_RxSampleDly_Set_impl;
T_Hal_Spi_Data_Send_fp              Hal_Spi_Data_Send               = Hal_Spi_Data_Send_impl;
T_Hal_Spi_Data_Recv_fp              Hal_Spi_Data_Recv               = Hal_Spi_Data_Recv_impl;
T_Hal_Spi_Master_Config_fp          Hal_Spi_Master_Config           = Hal_Spi_Master_Config_impl;
T_Hal_Spi_Slave_Config_fp           Hal_Spi_Slave_Config            = Hal_Spi_Slave_Config_impl;
T_Hal_Spi_Slave_Select_Enable_fp    Hal_Spi_Slave_Select_Enable     = Hal_Spi_Slave_Select_Enable_impl;
T_Hal_Spi_Dma_Config_fp             Hal_Spi_Dma_Config              = Hal_Spi_Dma_Config_impl;
T_Hal_Spi_IntMask_fp                Hal_Spi_IntMask                 = Hal_Spi_IntMask_impl;
T_Hal_Spi_IntEn_fp                  Hal_Spi_IntEn                   = Hal_Spi_IntEn_impl;
T_Hal_Spi_IntClearAll_fp            Hal_Spi_IntClearAll             = Hal_Spi_IntClearAll_impl;
T_Hal_Spi_Callback_Set_fp           Hal_Spi_Callback_Set            = Hal_Spi_Callback_Set_impl;

// Sec 5: declaration of global function prototype

/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable
uint32_t u32SpiSpeed[SPI_IDX_MAX] = {0,0,0,0};
uint32_t g_u32SpiTimeoutMs = SPI_TIMEOUT_MS;

// Sec 7: declaration of static function prototype

/***********
C Functions
***********/
// Sec 8: C Functions
/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Master_Init
*
* DESCRIPTION:
*   1. Init SPI module for Master
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx   : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx   : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32Baud   : Clock of SPI.
*   4. ePolar    : Polarity of SPI clock. refer to E_SpiClkPolarity_t
*   5. ePhase    : Pase of SPI clock. refer to E_SpiClkPhase_t
*   6. eFormat   : Transmit protocal. refer to E_SpiFrameFormat_t
*   7. eDataSize : Data Frame size. refer to E_SpiDataFrameSize_t
*   8. u8QModeEn : Qaud-mode select. 1 for enable/0 for disable
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Master_Init_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    S_SPI_MST_Reg_t *pSpi = NULL;
    uint32_t u32Temp = 0;
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    else
        return 1;
    
	// Disable SPI
	_Hal_Spi_Enable(eSpiIdx, 0);
    
    // Control setting
    u32Temp = pSpi->CTRLR0;
    u32Temp &= ~( SPI_MST_CTRLR0_DFS_32_Msk |
                  SPI_MST_CTRLR0_SCPOL_Msk |
                  SPI_MST_CTRLR0_SCPH_Msk |
                  SPI_MST_CTRLR0_FRF_Msk );
    u32Temp |= ( (eDataSize << SPI_MST_CTRLR0_DFS_32_Pos) |
                 (ePolar << SPI_MST_CTRLR0_SCPOL_Pos) |
                 (ePhase << SPI_MST_CTRLR0_SCPH_Pos) |
                 (eFormat << SPI_MST_CTRLR0_FRF_Pos) );
    pSpi->CTRLR0 = u32Temp;
    // Set speed
    Hal_Spi_BaudRateSet(eSpiIdx, u32Baud);    
    // Set slave number, only 1 slave at a time
    pSpi->SER = (1 << eSlvIdx);
    // Rx buffer threshold
    pSpi->RXFTLR = 5;
    // Tx buffer threshold
    pSpi->TXFTLR = 5;
    // Interrupt mask
    pSpi->IMR = 0x0;
    
    // Enable SPI
    _Hal_Spi_Enable(eSpiIdx, 1);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Slave_Init
*
* DESCRIPTION:
*   1. Init SPI module for Slave
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx   : Index of SPI. refer to E_SpiIdx_t
*   2. ePolar    : Polarity of SPI clock. refer to E_SpiClkPolarity_t
*   3. ePhase    : Pase of SPI clock. refer to E_SpiClkPhase_t
*   4. eFormat   : Transmit protocal. refer to E_SpiFrameFormat_t
*   5. eDataSize : Data Frame size. refer to E_SpiDataFrameSize_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Slave_Init_impl(E_SpiIdx_t eSpiIdx, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize)
{
    if (eSpiIdx != SPI_IDX_2)
        return 1;
    
    uint32_t u32Temp = 0;
    
	// Disable SPI
	_Hal_Spi_Enable(eSpiIdx, 0);
    
    // Control setting
    u32Temp = SPI2->CTRLR0;
    u32Temp &= ~( SPI_SLV_CTRLR0_DFS_32_Msk |
                  SPI_SLV_CTRLR0_SCPOL_Msk |
                  SPI_SLV_CTRLR0_SCPH_Msk |
                  SPI_SLV_CTRLR0_FRF_Msk );
    u32Temp |= ( (eDataSize << SPI_SLV_CTRLR0_DFS_32_Pos) |
                 (ePolar << SPI_SLV_CTRLR0_SCPOL_Pos) |
                 (ePhase << SPI_SLV_CTRLR0_SCPH_Pos) |
                 (eFormat << SPI_SLV_CTRLR0_FRF_Pos) );
    SPI2->CTRLR0 = u32Temp;
    // Rx buffer threshold
    SPI2->RXFTLR = 0;
    // Tx buffer threshold
    SPI2->TXFTLR = 5;
    // Interrupt mask
    SPI2->IMR = 0x0;
    
    // Enable SPI
    _Hal_Spi_Enable(eSpiIdx, 1);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Master_Data_Send
*
* DESCRIPTION:
*   1. SPI master send data
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. u32Data  : Data to send
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Master_Data_Send_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Data)
{
    S_SPI_MST_Reg_t *pSpi = NULL;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    else
        return 1;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32SpiTimeoutMs;
    u32TickStart = Hal_Tick_Diff(0);
    
    // wait for TX_FIFO not full
    while ( !(pSpi->SR & SPI_MST_SR_TFNF) )
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    pSpi->DR[0] = u32Data;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Slave_Data_Send
*
* DESCRIPTION:
*   1. SPI slave send data
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. u32Data  : Data to send
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Slave_Data_Send_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Data)
{
    if (eSpiIdx != SPI_IDX_2)
        return 1;
    
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32SpiTimeoutMs;
    u32TickStart = Hal_Tick_Diff(0);
    
    // wait for TX_FIFO not full
    while ( !(SPI2->SR & SPI_SLV_SR_TFNF) )
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    SPI2->DR[0] = u32Data;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Master_Data_Recv
*
* DESCRIPTION:
*   1. SPI master receive data
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. pu32Data : Pointer to receive data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Master_Data_Recv_impl(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data)
{
    S_SPI_MST_Reg_t *pSpi = NULL;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    else
        return 1;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32SpiTimeoutMs;
    
    u32TickStart = Hal_Tick_Diff(0);
    // wait for RX_FIFO not empty
    while ( !(pSpi->SR & SPI_MST_SR_RFNE) )
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    *pu32Data = pSpi->DR[0];
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Slave_Data_Recv
*
* DESCRIPTION:
*   1. SPI slave receive data
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. pu32Data : Pointer to receive data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Slave_Data_Recv_impl(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data)
{
    if (eSpiIdx != SPI_IDX_2)
        return 1;
    
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32SpiTimeoutMs;
    u32TickStart = Hal_Tick_Diff(0);
    
    // wait for RX_FIFO not empty
    while ( !(SPI2->SR & SPI_SLV_SR_RFNE))
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    *pu32Data = SPI2->DR[0];
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Master_Byte_Reverse
*
* DESCRIPTION:
*   1. Byte-reverse for DFS=16 & DFS=32
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. u8Enable : 1 = Enable, 0 = Disable.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Master_Byte_Reverse_impl(E_SpiIdx_t eSpiIdx, uint8_t u8Enable)
{
    uint32_t u32Temp;
    S_SPI_MST_Reg_t *pSpi = NULL;
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    else
        return 1;
    
    u32Temp = pSpi->SPI_OPLR0;
    u32Temp &= ~(SPI_MST_SPI_OPLR0_BYTE_REVERSE_EN_Msk);
    u32Temp |= (u8Enable << SPI_MST_SPI_OPLR0_BYTE_REVERSE_EN_Pos);
    pSpi->SPI_OPLR0 = u32Temp;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Master_Cs_Assert
*
* DESCRIPTION:
*   1. to keep CS asserted when TX-FIFO is empty
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. u8Enable : 1 = Enable, 0 = Disable.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Master_Cs_Assert_impl(E_SpiIdx_t eSpiIdx, uint8_t u8Enable)
{
    uint32_t u32Temp;
    S_SPI_MST_Reg_t *pSpi = NULL;
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    else
        return 1;
    
    u32Temp = pSpi->SPI_OPLR0;
    u32Temp &= ~(SPI_MST_SPI_OPLR0_CS_ASSERT_EN_Msk);
    u32Temp |= (u8Enable << SPI_MST_SPI_OPLR0_CS_ASSERT_EN_Pos);
    pSpi->SPI_OPLR0 = u32Temp;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Master_Read_Freeze
*
* DESCRIPTION:
*   1. to freeze activity between flash and SPI to avoid RX-FIFO overflow
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. u8Enable : 1 = Enable, 0 = Disable.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Master_Read_Freeze_impl(E_SpiIdx_t eSpiIdx, uint8_t u8Enable)
{
    uint32_t u32Temp;
    S_SPI_MST_Reg_t *pSpi = NULL;
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    else
        return 1;
    
    u32Temp = pSpi->SPI_OPLR0;
    u32Temp &= ~(SPI_MST_SPI_OPLR0_READ_FREEZE_EN_Msk);
    u32Temp |= (u8Enable << SPI_MST_SPI_OPLR0_READ_FREEZE_EN_Pos);
    pSpi->SPI_OPLR0 = u32Temp;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Poll_Busy
*
* DESCRIPTION:
*   1. polling busy status of SPI module
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refer to E_SpiIdx_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Poll_Busy_impl(E_SpiIdx_t eSpiIdx)
{
    volatile const uint32_t *pu32Reg = NULL;
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    
    if (eSpiIdx == SPI_IDX_1)
        pu32Reg = &(SPI1->SR);
    else if (eSpiIdx == SPI_IDX_2)
        pu32Reg = &(SPI2->SR);
    else if (eSpiIdx == SPI_IDX_3)
        pu32Reg = &(SPI3->SR);
    else
        return 1;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32SpiTimeoutMs;
    u32TickStart = Hal_Tick_Diff(0);
    
    // wait for SPIx not busy
    while (*pu32Reg & SPI_MST_SR_BUSY)
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_Spi_Enable
*
* DESCRIPTION:
*   1. Enable/Disable SPI module
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. u8Enable : 1 = Enable, 0 = Disable.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_Spi_Enable_impl(E_SpiIdx_t eSpiIdx, uint8_t u8Enable)
{
    // poll busy before disable SPI module
    if (u8Enable == 0)
        _Hal_Spi_Poll_Busy(eSpiIdx);
    
    if (eSpiIdx == SPI_IDX_1)
        SPI1->SSIENR = u8Enable;
    else if (eSpiIdx == SPI_IDX_2)
        SPI2->SSIENR = u8Enable;
    else if (eSpiIdx == SPI_IDX_3)
        SPI3->SSIENR = u8Enable;
    else
        return 1;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_BaudRateGet
*
* DESCRIPTION:
*   1. Get baud-rate of SPI
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*
* RETURNS
*   Baud-rate value. 1 for error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_BaudRateGet_impl(E_SpiIdx_t eSpiIdx)
{   
    S_SPI_MST_Reg_t *pSpi = NULL;
    uint32_t u32RefClk = 0;
    
    if (eSpiIdx == SPI_IDX_1)
    {
        pSpi = SPI1;
        Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI1, &u32RefClk);
    }
    else if (eSpiIdx == SPI_IDX_3)
    {
        pSpi = SPI3;
        Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI3, &u32RefClk);
    }
    else
    {
        return 1;
    }
    
    return u32RefClk / pSpi->BAUDR;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_BaudRateSet
*
* DESCRIPTION:
*   1. Set baud-rate of SPI
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. u32Baud  : Clock of SPI.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_BaudRateSet_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Baud)
{
    S_SPI_MST_Reg_t *pSpi = NULL;
    uint32_t u32Div = 0;
    uint32_t u32RefClk = 0;
    uint32_t u32SsiEnr;
    
    if (u32Baud == 0)
        return 1;
    
    if (eSpiIdx == SPI_IDX_1)
    {
        pSpi = SPI1;
        Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI1, &u32RefClk);
    }
    else if (eSpiIdx == SPI_IDX_3)
    {
        pSpi = SPI3;
        Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI3, &u32RefClk);
    }
    else
    {
        return 1;
    }
    
    /* Disable SPI first */
    u32SsiEnr = pSpi->SSIENR;
    pSpi->SSIENR = 0;
    
    /* Find closest even value. Bit 0 is always truncated
     * Table 6-11 BAUDR, range is 0x0002~0xFFFE, even number only (ie. last bit always truncated) */
    u32Div = ( u32RefClk + u32Baud - 1 )/ u32Baud;
    
    if( u32Div < SPI_BAUDR_MIN )
        u32Div = SPI_BAUDR_MIN;
    else if( u32Div > SPI_BAUDR_MAX )
        u32Div = SPI_BAUDR_MAX;
    
    /* Set baud rate and store it */
    pSpi->BAUDR = u32Div;
    u32SpiSpeed[eSpiIdx] = u32Baud;
    
    /* Recover SPI */
    pSpi->SSIENR = u32SsiEnr;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_DividerUpdate
*
* DESCRIPTION:
*   1. Update SPI divider
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_DividerUpdate_impl(E_SpiIdx_t eSpiIdx)
{
    if (eSpiIdx >= SPI_IDX_MAX)
        return 1;
    
    Hal_Spi_BaudRateSet(eSpiIdx, u32SpiSpeed[eSpiIdx]);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Init
*
* DESCRIPTION:
*   1. Init SPI module
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. u32Baud  : Clock of SPI.
*   3. ePolar   : Polarity of SPI clock. refer to E_SpiClkPolarity_t
*   4. ePhase   : Pase of SPI clock. refer to E_SpiClkPhase_t
*   5. eFormat  : Transmit protocal. refer to E_SpiFrameFormat_t
*   6. eDataSize: Transmit data size 
*   7. u8QModeEn: Qaud-mode select. 1 for enable/0 for disable
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Init_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn)
{
    return Hal_Spi_Init_Ext(eSpiIdx, SPI_SLAVE_0, u32Baud, ePolar, ePhase, eFormat, eDataSize, u8QModeEn);
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Init_Ext
*
* DESCRIPTION:
*   1. Init SPI module (New function)
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx   : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx   : Index of SPI slave. refer to E_SpiSlave_t
*   3. u32Baud   : Clock of SPI.
*   4. ePolar    : Polarity of SPI clock. refer to E_SpiClkPolarity_t
*   5. ePhase    : Pase of SPI clock. refer to E_SpiClkPhase_t
*   6. eFormat   : Transmit protocal. refer to E_SpiFrameFormat_t
*   7. eDataSize : Transmit data size 
*   8. u8QModeEn : Qaud-mode select. 1 for enable/0 for disable
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Init_Ext_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint32_t u32Baud, E_SpiClkPolarity_t ePolar, E_SpiClkPhase_t ePhase, E_SpiFrameFormat_t eFormat, E_SpiDataFrameSize_t eDataSize, uint32_t u8QModeEn)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (eSpiIdx == SPI_IDX_1)
    {
        Hal_Sys_Spi1SrcSelect(APS_CLK_SPI1_SRC_XTAL, APS_CLK_SPI1_DIV_1);
        Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_SPI_1);
        Hal_Sys_ApsModuleRst(APS_RST_SPI_1);
        return _Hal_Spi_Master_Init(eSpiIdx, eSlvIdx, u32Baud, ePolar, ePhase, eFormat, eDataSize, u8QModeEn);
    }
    else if (eSpiIdx == SPI_IDX_2)
    {
        Hal_Sys_Spi2SrcSelect(APS_CLK_SPI2_SRC_XTAL, APS_CLK_SPI2_DIV_1);
        Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_SPI_2);
        Hal_Sys_ApsModuleRst(APS_RST_SPI_2);
        return _Hal_Spi_Slave_Init(eSpiIdx, ePolar, ePhase, eFormat, eDataSize);
    }
    else if (eSpiIdx == SPI_IDX_3)
    {
        Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_XTAL, APS_CLK_SPI3_DIV_1);
        Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_SPI_3);
        Hal_Sys_ApsModuleRst(APS_RST_SPI_3);
        return _Hal_Spi_Master_Init(eSpiIdx, eSlvIdx, u32Baud, ePolar, ePhase, eFormat, eDataSize, u8QModeEn);
    }
    else
    {
        return 1;
    }
}




/*************************************************************************
* FUNCTION:
*  Hal_Spi_RxSampleDly_Set
*
* DESCRIPTION:
*   1. to set the number of ssi_clk cycles that are delayed (from the default sample time) 
*      before the actual sample of the rxd input occurs
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. u16DlyNum : delay number of ssi_clk cycles (0 to 4)
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_RxSampleDly_Set_impl(E_SpiIdx_t eSpiIdx, uint16_t u16DlyNum)
{
    S_SPI_MST_Reg_t *pSpi = NULL;
    uint32_t u32Temp = 0;
    
    if(eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    else 
        return 1;
    
    // Disable SPI
    _Hal_Spi_Enable(eSpiIdx, 0);
    
    // Set RX_SAMPLE_DLY
    u32Temp = pSpi->RX_SAMPLE_DLY & ~(SPI_MST_RX_SAMPLE_DLY_RSD_Msk);
    u32Temp |= (u16DlyNum << SPI_MST_RX_SAMPLE_DLY_RSD_Pos);
    pSpi->RX_SAMPLE_DLY = u32Temp;
    
    // Enable SPI
    _Hal_Spi_Enable(eSpiIdx, 1);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Data_Send
*
* DESCRIPTION:
*   1. SPI send data
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. u32Data  : Data to send
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Data_Send_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Data)
{
    if ( (eSpiIdx == SPI_IDX_1) || (eSpiIdx == SPI_IDX_3) )
        return _Hal_Spi_Master_Data_Send(eSpiIdx, u32Data);
    else if (eSpiIdx == SPI_IDX_2)
        return _Hal_Spi_Slave_Data_Send(eSpiIdx, u32Data);
    else
        return 1;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Data_Recv
*
* DESCRIPTION:
*   1. SPI receive data
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. pu32Data : Pointer to receive data
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Data_Recv_impl(E_SpiIdx_t eSpiIdx, uint32_t *pu32Data)
{
    if ( (eSpiIdx == SPI_IDX_1) || (eSpiIdx == SPI_IDX_3) )
        return _Hal_Spi_Master_Data_Recv(eSpiIdx, pu32Data);
    else if(eSpiIdx == SPI_IDX_2)
        return _Hal_Spi_Slave_Data_Recv(eSpiIdx, pu32Data);
    else
        return 1;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Master_Config
*
* DESCRIPTION:
*   1. configure SPI Master
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refer to E_SpiIdx_t
*   2. ptCfg   : Pointer of SPI configuration.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Master_Config_impl(E_SpiIdx_t eSpiIdx, S_SPI_Config_t *ptCfg)
{
     S_SPI_MST_Reg_t *pSpi = NULL;
     uint32_t u32Temp = 0;
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    else
        return 1;

    // Disable SPI
    _Hal_Spi_Enable(eSpiIdx, 0);
    
    u32Temp = pSpi->CTRLR0;
    u32Temp &= ~( SPI_MST_CTRLR0_SPI_FRF_Msk |
                  SPI_MST_CTRLR0_DFS_32_Msk |
                  SPI_MST_CTRLR0_TMOD_Msk );
    u32Temp |= ( (ptCfg->eSpi_frf << SPI_MST_CTRLR0_SPI_FRF_Pos) | 
                 (ptCfg->eDfs_32 << SPI_MST_CTRLR0_DFS_32_Pos) |
                 (ptCfg->eTmod << SPI_MST_CTRLR0_TMOD_Pos) );
    pSpi->CTRLR0 = u32Temp;
    
    pSpi->CTRLR1 = (ptCfg->u16Ndf << SPI_MST_CTRLR1_NDF_Pos);
    
    u32Temp = pSpi->SPI_CTRLR0;
    u32Temp &= ~( SPI_MST_SPI_CTRLR0_WAIT_CYCLES_Msk | 
                  SPI_MST_SPI_CTRLR0_INST_L_Msk |
                  SPI_MST_SPI_CTRLR0_ADDR_L_Msk |
                  SPI_MST_SPI_CTRLR0_TRANS_TYPE_Msk );
    u32Temp |= ( (ptCfg->u8WaitCycles << SPI_MST_SPI_CTRLR0_WAIT_CYCLES_Pos) |
                 (ptCfg->eInst_l << SPI_MST_SPI_CTRLR0_INST_L_Pos) |
                 (ptCfg->eAddr_l << SPI_MST_SPI_CTRLR0_ADDR_L_Pos) |
                 (ptCfg->eTrans_type << SPI_MST_SPI_CTRLR0_TRANS_TYPE_Pos) );
    pSpi->SPI_CTRLR0 = u32Temp;
    
    // Enable SPI
    _Hal_Spi_Enable(eSpiIdx, 1);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Slave_Config
*
* DESCRIPTION:
*   1. configure SPI Slave
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refer to E_SpiIdx_t
*   2. ptCfg   : Pointer of SPI configuration.
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Slave_Config_impl(E_SpiIdx_t eSpiIdx, S_SPI_Config_t *ptCfg)
{
     uint32_t u32Temp = 0;
    
    if (eSpiIdx != SPI_IDX_2)
        return 1;

    // Disable SPI
    _Hal_Spi_Enable(eSpiIdx, 0);
    
    u32Temp = SPI2->CTRLR0;
    u32Temp &= ~( SPI_SLV_CTRLR0_SPI_FRF_Msk |
                  SPI_SLV_CTRLR0_DFS_32_Msk |
                  SPI_SLV_CTRLR0_TMOD_Msk );
    u32Temp |= ( (ptCfg->eSpi_frf << SPI_SLV_CTRLR0_SPI_FRF_Pos) | 
                 (ptCfg->eDfs_32 << SPI_SLV_CTRLR0_DFS_32_Pos) |
                 (ptCfg->eTmod << SPI_SLV_CTRLR0_TMOD_Pos) );
    SPI2->CTRLR0 = u32Temp;
    
    // Enable SPI
    _Hal_Spi_Enable(eSpiIdx, 1);
    
    return 0;
}
/*************************************************************************
* FUNCTION:
*  Hal_Spi_Slave_Select_Enable
*
* DESCRIPTION:
*   1. Enable/Disable Slave Select
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*   2. eSlvIdx  : Index of SPI slave. refer to E_SpiSlave_t
*   3. u8Enable : 1 = Enable, 0 = Disable.
*
* RETURNS
*   0: setting complete
*   1: error 
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Slave_Select_Enable_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx, uint8_t u8Enable)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
     S_SPI_MST_Reg_t *pSpi = NULL;
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    else 
        return 1;

    if (u8Enable)
        pSpi->SER = (1 << eSlvIdx); // Note: only 1 bit in this field should be set
    else
        pSpi->SER = 0;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Dma_Config
*
* DESCRIPTION:
*   1. config DMA parameters of SPI module
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refer to E_SpiIdx_t
*   2. u8TxEnable : Enable/Disable the TX_FIFO DMA channel
*   3. u8RxEnable : Enable/Disable the RX_FIFO DMA channel
*   4. u8TxDataLv : DMA transmit data level
*   5. u8RxDataLv : DMA receive data level
*
* RETURNS
*   0: setting complete
*   1: error 
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_Dma_Config_impl(E_SpiIdx_t eSpiIdx, uint8_t u8TxEnable, uint8_t u8RxEnable, uint8_t u8TxDataLv, uint8_t u8RxDataLv)
{
    u8TxDataLv = (u8TxDataLv > 31) ? 31 : u8TxDataLv;
    u8RxDataLv = (u8RxDataLv > 31) ? 31 : u8RxDataLv;
    
    if (eSpiIdx == SPI_IDX_1)
    {
        SPI1->DMATDLR = (u8TxDataLv << SPI_MST_DMATDLR_DMATDL_Pos);
        SPI1->DMARDLR = (u8RxDataLv << SPI_MST_DMARDLR_DMARDL_Pos);
        SPI1->DMACR   = (u8TxEnable << SPI_MST_DMACR_TDMAE_Pos) | (u8RxEnable << SPI_MST_DMACR_RDMAE_Pos);
    }
    else if (eSpiIdx == SPI_IDX_2)
    {
        SPI2->DMATDLR = (u8TxDataLv << SPI_SLV_DMATDLR_DMATDL_Pos);
        SPI2->DMARDLR = (u8RxDataLv << SPI_SLV_DMARDLR_DMARDL_Pos);
        SPI2->DMACR   = (u8TxEnable << SPI_SLV_DMACR_TDMAE_Pos) | (u8RxEnable << SPI_SLV_DMACR_RDMAE_Pos);
    }
    else if (eSpiIdx == SPI_IDX_3)
    {
        SPI3->DMATDLR = (u8TxDataLv << SPI_MST_DMATDLR_DMATDL_Pos);
        SPI3->DMARDLR = (u8RxDataLv << SPI_MST_DMARDLR_DMARDL_Pos);
        SPI3->DMACR   = (u8TxEnable << SPI_MST_DMACR_TDMAE_Pos) | (u8RxEnable << SPI_MST_DMACR_RDMAE_Pos);
    }
    else
    {
        return 1;
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_IntMask
*
* DESCRIPTION:
*  1. Mask/Unmask interrupts of SPI
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refer to E_SpiIdx_t
*   2. u32Bmp  : bitmap of Mask* Interrupt Registers
*                1 = Unmask
*                0 = Mask
*
* RETURNS
*   0: setting complete
*   1: error 
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_IntMask_impl(E_SpiIdx_t eSpiIdx, uint32_t u32Bmp)
{
    if (eSpiIdx == SPI_IDX_1)
        SPI1->IMR = u32Bmp;
    else if (eSpiIdx == SPI_IDX_2)
        SPI2->IMR = u32Bmp;
    else if (eSpiIdx == SPI_IDX_3)
        SPI3->IMR = u32Bmp;
    else 
        return 1;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_IntEn
*
* DESCRIPTION:
*   1. Enable SPI interrupts
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refer to E_SpiIdx_t
*   2. eEnable : 1 = Enable
*                0 = Disable
*
* RETURNS
*   0: setting complete
*   1: error 
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_IntEn_impl(E_SpiIdx_t eSpiIdx, uint8_t u8Enable)
{
    IRQn_Type eIrqType;
    uint32_t u32IrqPriority;
    
    if (eSpiIdx == SPI_IDX_1)
    {
        eIrqType = SPI1_IRQn;
        u32IrqPriority = IRQ_PRIORITY_SPI1;
    }
    else if (eSpiIdx == SPI_IDX_2)
    {
        eIrqType = SPI2_IRQn;
        u32IrqPriority = IRQ_PRIORITY_SPI2;
    }
    else if (eSpiIdx == SPI_IDX_3)
    {
        eIrqType = SPI3_IRQn;
        u32IrqPriority = IRQ_PRIORITY_SPI3;
    }
    else
    {
        return 1;
    }
    
    if (u8Enable == 1)
    {
        Hal_Vic_IntInit(eIrqType, VIC_TRIG_LEVEL_HIGH, u32IrqPriority);
    }
    else
    {
        Hal_Vic_IntDeInit(eIrqType);
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_IntClearAll
*
* DESCRIPTION:
*   1. Clear any pending interrupts of SPI
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refer to E_SpiIdx_t
*
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_IntClearAll_impl(E_SpiIdx_t eSpiIdx)
{
    if (eSpiIdx == SPI_IDX_1)
        reg_read(SPI1->ICR);
    else if (eSpiIdx == SPI_IDX_2)
        reg_read(SPI2->ICR);
    else if (eSpiIdx == SPI_IDX_3)
        reg_read(SPI3->ICR);
    else 
        return 1;
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Spi_Callback_Set
*
* DESCRIPTION:
*   1. Setup SPI callback function
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx : Index of SPI. refer to E_SpiIdx_t
*   2. tFunc   : Callback function
*
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Spi_Callback_Set_impl(E_SpiIdx_t eSpiIdx, T_Spi_CallBack tFunc)
{
    if (eSpiIdx == SPI_IDX_1)
        g_tHalSpi1CallBack = tFunc;
    else if (eSpiIdx == SPI_IDX_2)
        g_tHalSpi2CallBack = tFunc;
    else if (eSpiIdx == SPI_IDX_3)
        g_tHalSpi3CallBack = tFunc;
    else
        return;
}

