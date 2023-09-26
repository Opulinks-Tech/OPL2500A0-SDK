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
#define SEND_SPI_BUFFER_LEN     128
#define SPI_BUFF_DUMP_PER_LINE  16

#define MASTER_TX_SYNC              0xA5
#define SLAVE_TX_ACK                0x5A

#define SPI_MASTER_TX_DMA_CHANNEL   DMA_Channel_0
#define SPI_MASTER_RX_DMA_CHANNEL   DMA_Channel_1
#define SPI_BAUDRATE                10000
#define DMA_POLL_TIMEOUT_MS         (uint32_t)(3 * (1 + (SEND_SPI_BUFFER_LEN * (8*1000.0 / SPI_BAUDRATE))))
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
uint32_t Spi_Slave_TrxDmaInit(void);
void Spi_DmaRun(void);
#endif

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
uint8_t g_u8aSpiTxPattern[SEND_SPI_BUFFER_LEN] = {
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
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

uint32_t Spi_Slave_TrxDmaInit(void)
{
    E_DMA_CHANNEL eTxChannel = SPI_MASTER_TX_DMA_CHANNEL;
    E_DMA_CHANNEL eRxChannel = SPI_MASTER_RX_DMA_CHANNEL;
    
    
    Hal_Dma_Init();
    // config DMA
    S_DMA_Init_t tTxDma;
    memset(&tTxDma, 0, sizeof(S_DMA_Init_t));
    S_DMA_Init_t tRxDma;
    memset(&tRxDma, 0, sizeof(S_DMA_Init_t));
    
    // setup TX DMA configurations
    tTxDma.Sar = (uint32_t) &g_u8aSpiTxPattern[0];
    tTxDma.Dar = (uint32_t) &(SPI2->DR[0]);
    tTxDma.eTtfc = DMA_TT_m2p_FC_Dma;
    tTxDma.eSrcTrWidth = DMA_TR_WIDTH_8_bits;
    tTxDma.eDstTrWidth = DMA_TR_WIDTH_8_bits;
    tTxDma.eSrcMsize = DMA_MSIZE_16;
    tTxDma.eDestMsize = DMA_MSIZE_16;
    tTxDma.eSinc = DMA_INC_Increment;
    tTxDma.eDinc = DMA_INC_No_Change;
    tTxDma.BlockTs = SEND_SPI_BUFFER_LEN;
    tTxDma.DestPer = APS_DMA_ITF_SPI2_TX;
    
    // setup RX DMA configurations
    tRxDma.Sar = (uint32_t) &(SPI2->DR[0]);
    tRxDma.Dar = (uint32_t) &g_u8aSpiRxBuffer[0];
    tRxDma.eTtfc = DMA_TT_p2m_FC_Dma;
    tRxDma.eSrcTrWidth = DMA_TR_WIDTH_8_bits;
    tRxDma.eDstTrWidth = DMA_TR_WIDTH_8_bits;
    tRxDma.eSrcMsize = DMA_MSIZE_16;
    tRxDma.eDestMsize = DMA_MSIZE_16;
    tRxDma.eSinc = DMA_INC_No_Change;
    tRxDma.eDinc = DMA_INC_Increment;
    tRxDma.BlockTs = SEND_SPI_BUFFER_LEN;
    tRxDma.SrcPer = APS_DMA_ITF_SPI2_RX;
    
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
    tCfg.eDfs_32  = SPI_DFS_08_bit;
    tCfg.eTmod    = SPI_TMOD_TRX;    
    Hal_Spi_Slave_Config(SPI_IDX_2, &tCfg);
    
    DmaTxSarReg = (uint32_t)&(g_pscaHalDmaCHx[eTxChannel]->SAR);
    DmaRxDarReg = (uint32_t)&(g_pscaHalDmaCHx[eRxChannel]->DAR);
    return 1;
}
    
void Spi_Communicate(void)
{
    E_DMA_CHANNEL eTxChannel = SPI_MASTER_TX_DMA_CHANNEL;
    E_DMA_CHANNEL eRxChannel = SPI_MASTER_RX_DMA_CHANNEL;
    
    reg_write(DmaTxSarReg, (uint32_t)&g_u8aSpiTxPattern);
    reg_write(DmaRxDarReg, (uint32_t)&g_u8aSpiRxBuffer);
    
    // enable Receive DMA
    Hal_Spi_Slave_Select_Enable(SPI_IDX_2, SPI_SLAVE_0, 1);
    Hal_Spi_Dma_Config(SPI_IDX_2, 0, 1, 0, 15);
    Hal_Dma_Enable(eRxChannel); // RX DMA
    
    // enable Transmit DMA
    Hal_Spi_Dma_Config(SPI_IDX_2, 1, 1, 16, 15);
    Hal_Dma_Enable(eTxChannel); // TX DMA
    
    Dma_StsPoll(eRxChannel);
    Dma_StsPoll(eTxChannel);
    while (_Hal_Spi_Poll_Busy(SPI_IDX_2));
    Hal_Spi_ResetFifo(SPI_IDX_2);
    Hal_Dma_Abort(eRxChannel);
    Hal_Dma_Abort(eTxChannel);
}
#else /* TEST_DMA_EN */
void Spi_Communicate(void)
{
    uint32_t u32Idx;
    uint32_t u32Data;
    uint32_t u32Cnt;
    
    for (u32Cnt=0; u32Cnt<HAL_SPI_FIFO_LENGTH; u32Cnt++)
    {   /* Put reply data to FIFO first */
        Hal_Spi_Data_Send(SPI_IDX_2, g_u8aSpiTxPattern[u32Cnt]);
    }
    
    for (u32Idx=0; u32Idx<SEND_SPI_BUFFER_LEN;)
    {
        if (Hal_Spi_Data_Recv(SPI_IDX_2, &u32Data) == 0)
        {   /* Received success */
            g_u8aSpiRxBuffer[u32Idx++] = u32Data;
            if (u32Cnt < SEND_SPI_BUFFER_LEN)
            {
                Hal_Spi_Data_Send(SPI_IDX_2, g_u8aSpiTxPattern[u32Cnt++]);
            }
        }
    }

    /* Wait finish */
    while (_Hal_Spi_Poll_Busy(SPI_IDX_2));
    Hal_Spi_ResetFifo(SPI_IDX_2);
}
#endif /* TEST_DMA_EN */
void Main_AppThread(void *argu)
{
    uint32_t u32Idx;
    
    // Init SPIx
    if (Hal_Spi_Init(SPI_IDX_2, 0, SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1) !=0)
    {   /* baud rate is ignored when initialized as slave mode */
        printf("SPI_2 init slave fail\n");
        osThreadTerminate(NULL);
        return;
    }
    #if TEST_DMA_EN
    if (!Spi_Slave_TrxDmaInit())
    {
        tracer_drct_printf("Init DMA fail\n");
        osThreadTerminate(NULL);
        return;
    }
    #endif /* TEST_DMA_EN */

    printf("SPI_2 Running: %s mode\n", (TEST_DMA_EN)?"DMA":"FIFO");
    osDelay(100);
    
    while(1)
    {
        memset(g_u8aSpiRxBuffer, 0, sizeof(g_u8aSpiRxBuffer));
        
        Hal_Spi_Data_Send(SPI_IDX_2, SLAVE_TX_ACK);
        while (1)
        {   /* Sync first */
            uint32_t RxSync = 0;
            if (Hal_Spi_Data_Recv(SPI_IDX_2, &RxSync) == 0)
            {
                if (RxSync == MASTER_TX_SYNC)
                {
                    tracer_drct_printf("Sync done\n");
                    break;
                }
                else
                {
                    tracer_drct_printf("%02x\n", RxSync);
                    Hal_Spi_Data_Send(SPI_IDX_2, SLAVE_TX_ACK);
                }
            }
            osDelay(50);
        }
        Hal_Spi_ResetFifo(SPI_IDX_2);
        osDelay(50);
        
        /* Start to TX/RX */
        Spi_Communicate();

        tracer_drct_printf("[Slave RX data %s mode]:", (TEST_DMA_EN)?"DMA":"FIFO");
        for (u32Idx=0; u32Idx<SEND_SPI_BUFFER_LEN; u32Idx++)
        {
            if (u32Idx % SPI_BUFF_DUMP_PER_LINE == 0)
                tracer_drct_printf("\n");
            tracer_drct_printf("%02X ", g_u8aSpiRxBuffer[u32Idx]);
        }
        tracer_drct_printf("\n[Slave TX data %s mode]:", (TEST_DMA_EN)?"DMA":"FIFO");
        for (u32Idx=0; u32Idx<SEND_SPI_BUFFER_LEN; u32Idx++)
        {
            if (u32Idx % SPI_BUFF_DUMP_PER_LINE == 0)
                tracer_drct_printf("\n");
            tracer_drct_printf("%02X ", g_u8aSpiTxPattern[u32Idx]);
        }
        tracer_drct_printf("\n\n");
        
        osDelay( 4000 );
    }
}
