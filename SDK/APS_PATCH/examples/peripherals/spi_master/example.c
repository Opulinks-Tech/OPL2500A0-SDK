/******************************************************************************
*  Copyright 2017 - 2021, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2021
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  example.c
*
*  Project:
*  --------
*  OPL1000 Project - the example implement file
*
*  Description:
*  ------------
*  This implement file is include the main patch function and api.
*
*  Author:
*  -------
*  TW FW
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <string.h>
#include "example.h"
#include "sys_os_config.h"
#include "hal_spi.h"
#include "hal_dma.h"
#include "hal_tick.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define TEST_SPI            SPI_IDX_3
#define TEST_SPI_SLAVE      SPI_SLAVE_0

#define SEND_SPI_BUFFER_LEN     128
#define SPI_BUFF_DUMP_PER_LINE  16

#define MASTER_TX_SYNC              0xA5
#define SLAVE_TX_ACK                0x5A

#define SPI_MASTER_TX_DMA_CHANNEL   DMA_Channel_0
#define SPI_MASTER_RX_DMA_CHANNEL   DMA_Channel_1
#define SPI_BAUDRATE                10000
#define DMA_POLL_TIMEOUT_MS         (uint32_t)(3 * (1 + (SEND_SPI_BUFFER_LEN * (8*1000.0 / SPI_BAUDRATE))))
#define SPI_FIFO_MODE_DELAY_US      100
/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
osThreadId g_tAppThread_1;


// Sec 5: declaration of global function prototype
void Main_AppThread(void *argu);
#if TEST_DMA_EN
uint8_t Dma_StsPoll(E_DMA_CHANNEL eChannel);
uint32_t Spi_Master_TrxDmaInit(void);
void Spi_DmaRun(void);
#endif

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
uint8_t g_u8aSpiTxPattern[SEND_SPI_BUFFER_LEN] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
};
uint8_t g_u8aSpiRxBuffer[SEND_SPI_BUFFER_LEN];
uint32_t DmaTxSarReg, DmaRxDarReg;
// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
void example( void )
{
    osThreadDef_t tThreadDef;
#include "hal_wdt.h"
    Hal_Wdt_Stop();
    // create the thread for Main-app thread
    tThreadDef.name = "MainApp";
    tThreadDef.pthread = Main_AppThread;
    tThreadDef.tpriority = OS_TASK_PRIORITY_APP;        // osPriorityNormal
    tThreadDef.instances = 0;                           // reserved, it is no used
    tThreadDef.stacksize = OS_TASK_STACK_SIZE_APP;      // (512), unit: 4-byte, the size is 512*4 bytes
    g_tAppThread_1 = osThreadCreate(&tThreadDef, NULL);
    if (g_tAppThread_1 == NULL)
    {
        printf("To create the thread for Main_AppThread is fail.\n");
    }
}

#if TEST_DMA_EN
// return : 0=ok; 1=timeout
uint8_t Dma_StsPoll(E_DMA_CHANNEL eChannel)
{
    uint32_t u32T1, u32T2;
    uint32_t u32TimeoutTick;
    
    u32TimeoutTick = Hal_Tick_PerMilliSec()*DMA_POLL_TIMEOUT_MS;
    
    u32T1 = Hal_Tick_Diff(0);
    while (Hal_Dma_Check(eChannel))
    {
        u32T2 = Hal_Tick_Diff(u32T1);
        if (u32T2 > u32TimeoutTick)
        {
            printf("DMA channel %d timeout, Abort\n", eChannel);
            Hal_Dma_Abort(eChannel);
            return 1;
        }
    }
    return 0;
}


uint32_t Spi_Master_TrxDmaInit(void)
{
    E_DMA_CHANNEL eTxChannel = SPI_MASTER_TX_DMA_CHANNEL;
    E_DMA_CHANNEL eRxChannel = SPI_MASTER_RX_DMA_CHANNEL;
    
    S_SPI_MST_Reg_t *pSpi = NULL;
    E_SpiIdx_t eSpiIdx = TEST_SPI;
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    
    Hal_Dma_Init();   
    // config DMA
    S_DMA_Init_t tTxDma;
    memset(&tTxDma, 0, sizeof(S_DMA_Init_t));
    S_DMA_Init_t tRxDma;
    memset(&tRxDma, 0, sizeof(S_DMA_Init_t));

    // setup TX DMA configurations
    tTxDma.Sar = (uint32_t) g_u8aSpiTxPattern;
    tTxDma.Dar = (uint32_t) &(pSpi->DR[0]);
    tTxDma.eTtfc = DMA_TT_m2p_FC_Dma;
    tTxDma.eSrcTrWidth = DMA_TR_WIDTH_8_bits;
    tTxDma.eDstTrWidth = DMA_TR_WIDTH_8_bits;
    tTxDma.eSrcMsize = DMA_MSIZE_16;
    tTxDma.eDestMsize = DMA_MSIZE_16;
    tTxDma.eSinc = DMA_INC_Increment;
    tTxDma.eDinc = DMA_INC_No_Change;
    tTxDma.BlockTs = SEND_SPI_BUFFER_LEN;
    if (eSpiIdx == SPI_IDX_1)
        tTxDma.DestPer = APS_DMA_ITF_SPI1_TX;
    else if (eSpiIdx == SPI_IDX_3)
        tTxDma.DestPer = APS_DMA_ITF_SPI3_TX;
    
    // setup RX DMA configurations
    tRxDma.Sar = (uint32_t) &(pSpi->DR[0]);
    tRxDma.Dar = (uint32_t) g_u8aSpiRxBuffer;
    tRxDma.eTtfc = DMA_TT_p2m_FC_Dma;
    tRxDma.eSrcTrWidth = DMA_TR_WIDTH_8_bits;
    tRxDma.eDstTrWidth = DMA_TR_WIDTH_8_bits;
    tRxDma.eSrcMsize = DMA_MSIZE_16;
    tRxDma.eDestMsize = DMA_MSIZE_16;
    tRxDma.eSinc = DMA_INC_No_Change;
    tRxDma.eDinc = DMA_INC_Increment;
    tRxDma.BlockTs = SEND_SPI_BUFFER_LEN;
    if (eSpiIdx == SPI_IDX_1)
        tRxDma.SrcPer = APS_DMA_ITF_SPI1_RX;
    else if (eSpiIdx == SPI_IDX_3)
        tRxDma.SrcPer = APS_DMA_ITF_SPI3_RX;
    
    // wait for DMA Channel is idle
    if (Dma_StsPoll(eTxChannel))
        return 0;
    Hal_Dma_Config(eTxChannel, &tTxDma);
    
    // wait for DMA Channel is idle
    if (Dma_StsPoll(eRxChannel))
        return 0;
    Hal_Dma_Config(eRxChannel, &tRxDma);

    // Config SPI
    S_SPI_Config_t tCfg;
    memset(&tCfg, 0, sizeof(S_SPI_Config_t));
    tCfg.eSpi_frf = SPI_FRF_STD;
    tCfg.eDfs_32 = SPI_DFS_08_bit;
    tCfg.eTmod = SPI_TMOD_TRX;
    Hal_Spi_Master_Config(eSpiIdx, &tCfg);
    
    DmaTxSarReg = (uint32_t)&(g_pscaHalDmaCHx[eTxChannel]->SAR);
    DmaRxDarReg = (uint32_t)&(g_pscaHalDmaCHx[eRxChannel]->DAR);
    return 1;
}

void Spi_Communicate(void)
{
    E_DMA_CHANNEL eTxChannel = SPI_MASTER_TX_DMA_CHANNEL;
    E_DMA_CHANNEL eRxChannel = SPI_MASTER_RX_DMA_CHANNEL;
    
    S_SPI_MST_Reg_t *pSpi = NULL;
    E_SpiIdx_t eSpiIdx = TEST_SPI;
    
    reg_write(DmaTxSarReg, (uint32_t)&g_u8aSpiTxPattern);
    reg_write(DmaRxDarReg, (uint32_t)&g_u8aSpiRxBuffer);
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    // enable Transmit DMA
    Hal_Spi_Dma_Config(eSpiIdx, 1, 0, 16, 0);
        
    // Data (using DMA)
    // enable Receive DMA
    Hal_Spi_Dma_Config(eSpiIdx, 1, 1, 16, 15);
    Hal_Dma_Enable(eRxChannel); // RX DMA
    
    // disable slave
    Hal_Spi_Slave_Select_Enable(eSpiIdx, SPI_SLAVE_0, 0);
    Hal_Dma_Enable(eTxChannel); // TX DMA
    while (!(pSpi->TXFLR >= 16));
    Hal_Spi_Slave_Select_Enable(eSpiIdx, SPI_SLAVE_0, 1);
    
    Dma_StsPoll(eTxChannel);
    Dma_StsPoll(eRxChannel);
    while (_Hal_Spi_Poll_Busy(eSpiIdx));
    Hal_Spi_ResetFifo(eSpiIdx);
    Hal_Dma_Abort(eTxChannel);
    Hal_Dma_Abort(eRxChannel);
}
#else /* TEST_DMA_EN */
void Delay_us(uint32_t us)
{
    double dSysClkTicks = 1000000/SystemCoreClockGet();
    uint32_t u32UsTick;
    uint32_t u32Start;
    
    u32UsTick = (uint32_t)((us + 1) * dSysClkTicks + 1);
    
    u32Start = Hal_Tick_Diff(0);
    while (1)
    {
        if (Hal_Tick_Diff(u32Start) >= u32UsTick)
            break;
    }
}

void Spi_Communicate(void)
{
    uint32_t u32Idx;
    uint32_t u32Data=0;
    
    /* Start to TX/RX */
    for (u32Idx=0; u32Idx<SEND_SPI_BUFFER_LEN; u32Idx++)
    {
        Hal_Spi_Data_Send(TEST_SPI, g_u8aSpiTxPattern[u32Idx]);
        Hal_Spi_Data_Recv(TEST_SPI, &u32Data);
        g_u8aSpiRxBuffer[u32Idx] = u32Data;
        Delay_us(SPI_FIFO_MODE_DELAY_US);
    }
    
    /* Wait finish */
    while (_Hal_Spi_Poll_Busy(TEST_SPI));
    Hal_Spi_ResetFifo(SPI_IDX_2);
}
#endif /* TEST_DMA_EN */
void Main_AppThread(void *argu)
{
    uint32_t u32Idx;
    
    // Init SPIx
    if (Hal_Spi_Init_Ext(TEST_SPI, TEST_SPI_SLAVE, SPI_BAUDRATE, SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1) !=0)
    {
        printf("SPI_%d init fail\n", TEST_SPI);
        osThreadTerminate(NULL);
        return;
    }
    #if TEST_DMA_EN 
    if (!Spi_Master_TrxDmaInit())
    {
        tracer_drct_printf("Init DMA fail\n");
        osThreadTerminate(NULL);
        return;
    }
    #endif /* TEST_DMA_EN */

    printf("SPI_%d Running: %s mode\n", TEST_SPI, (TEST_DMA_EN)?"DMA":"FIFO");
    osDelay(500);
    
    while(1)
    {
        memset(g_u8aSpiRxBuffer, 0, sizeof(g_u8aSpiRxBuffer));
        
        /* Sync first */
        while (1)
        {
            uint32_t RxAck = 0;
            Hal_Spi_Data_Send(TEST_SPI, MASTER_TX_SYNC);
            Hal_Spi_Data_Recv(TEST_SPI, &RxAck);
            if (RxAck == SLAVE_TX_ACK)
            {
                tracer_drct_printf("Sync done\n");
                Hal_Spi_ResetFifo(TEST_SPI);
                break;
            }
            else
            {
                tracer_drct_printf("%02x\n", RxAck);
            }
            osDelay(1000);
        }
        osDelay(100);
        
        /* Start to TX/RX */
        Spi_Communicate();

        tracer_drct_printf("[Master TX data %s mode]:", (TEST_DMA_EN)?"DMA":"FIFO");
        for (u32Idx=0; u32Idx<SEND_SPI_BUFFER_LEN; u32Idx++)
        {
            if (u32Idx % SPI_BUFF_DUMP_PER_LINE == 0)
                tracer_drct_printf("\n");
            tracer_drct_printf("%02X ", g_u8aSpiTxPattern[u32Idx]);
        }
        tracer_drct_printf("\n[Master RX data %s mode]:", (TEST_DMA_EN)?"DMA":"FIFO");
        for (u32Idx=0; u32Idx<SEND_SPI_BUFFER_LEN; u32Idx++)
        {
            if (u32Idx % SPI_BUFF_DUMP_PER_LINE == 0)
                tracer_drct_printf("\n");
            tracer_drct_printf("%02X ", g_u8aSpiRxBuffer[u32Idx]);
        }
        tracer_drct_printf("\n\n");
        
        osDelay( 5000 );
    }
}
