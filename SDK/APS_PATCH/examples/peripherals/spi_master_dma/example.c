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
#include "sys_cfg.h"
#include "sys_os_config.h"
#include "hal_spi.h"
#include "hal_dma.h"
#include "hal_gpio.h"
#include "hal_tick.h"
#include "hal_sys_rcc.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define TEST_SPI            SPI_IDX_3
#define TEST_SPI_SLAVE      SPI_SLAVE_0

#define SEND_SPI_BUFFER_LEN     2048
#define SPI_BUFFER_POOL_NUM     4   /* Power of 2 */
#define SPI_BUFF_DUMP_PER_LINE  32


#define SYNC_GPIO_IDX               GPIO_IDX_07
#define SYNC_GPIO_LEVEL             GPIO_LEVEL_LOW
#define SYNC_GPIO_INT_TRIG          VIC_TRIG_LEVEL_LOW

#define SPI_MASTER_TX_DMA_CHANNEL   DMA_Channel_0
#define SPI_MASTER_RX_DMA_CHANNEL   DMA_Channel_1


#define DMA_TIMEOUT_TICKS           (1 * SystemCoreClockGet())  // 1 sec

#define DEBUG_DUMP_DATA             (0 && SPI_MASTER_RX_ENABLE)
#define TEST_RX_SAMPLE_DELAY        0
/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
osThreadId g_tAppThread_1;
osSemaphoreId g_tAppSemaphore_Sync, g_tAppSemaphore_SpiDma;

// Sec 5: declaration of global function prototype
void Main_AppThread(void *argu);
uint8_t SpiDataCompare(uint8_t *pu8Data1, uint8_t *pu8Data2, uint32_t u32Size);
uint8_t Dma_StsPoll(E_DMA_CHANNEL eChannel);
uint32_t Spi_Master_TrxDmaInit(void);
void Spi_DmaRun(void);

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
#define g_u8aSpiTxPattern g_u8aSpiMasterPattern
#define g_u8aSpiRxPattern g_u8aSpiSlavePattern
extern uint8_t g_u8aSpiMasterPattern[2048];
extern uint8_t g_u8aSpiSlavePattern[2048];
#if SPI_MASTER_RX_ENABLE
uint8_t g_u8aSpiRxBufferIdx = 0;
uint8_t g_u8aSpiRxBuffer[SPI_BUFFER_POOL_NUM][SEND_SPI_BUFFER_LEN];
uint32_t DmaRxDarReg;
uint32_t g_SpiRxDone=0;
#define SPI_RX_BUFF_PREV  g_u8aSpiRxBuffer[((g_u8aSpiRxBufferIdx - 1) & (SPI_BUFFER_POOL_NUM - 1))]
#define SPI_RX_BUFF_CURR  g_u8aSpiRxBuffer[(g_u8aSpiRxBufferIdx & (SPI_BUFFER_POOL_NUM - 1))]
#endif /* SPI_MASTER_RX_ENABLE */
#if SPI_MASTER_TX_ENABLE
uint32_t DmaTxSarReg;
uint32_t g_SpiTxDone=0;
#endif /* SPI_MASTER_TX_ENABLE */
uint64_t g_DmaThrouthputTestCnt=0;
// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
void example( void )
{
    osThreadDef_t tThreadDef;
    osSemaphoreDef_t tSemDef = {0};

    g_tAppSemaphore_SpiDma = osSemaphoreCreate(&tSemDef, 1);
    osSemaphoreWait(g_tAppSemaphore_SpiDma, osWaitForever);
    g_tAppSemaphore_Sync = osSemaphoreCreate(&tSemDef, 1);
    osSemaphoreWait(g_tAppSemaphore_Sync, osWaitForever);
    
    
    // create the thread for Main-app thread
    tThreadDef.name = "MainApp";
    tThreadDef.pthread = Main_AppThread;
    tThreadDef.tpriority = OS_TASK_PRIORITY_APP;        // osPriorityNormal
    tThreadDef.instances = 0;                           // reserved, it is no used
    tThreadDef.stacksize = OS_TASK_STACK_SIZE_APP;      // (512), unit: 4-byte, the size is 512*4 bytes
    g_tAppThread_1 = osThreadCreate(&tThreadDef, NULL);
    if (g_tAppThread_1 == NULL)
    {
        tracer_drct_printf("To create the thread for Main_AppThread is fail.\n");
    }
}

uint8_t SpiDataCompare(uint8_t *pu8Data1, uint8_t *pu8Data2, uint32_t u32Size)
{
    while (u32Size--)
    {
        if (*pu8Data1++ != *pu8Data2++)
            return 0;
    }
    
    return 1;
}

void Spi_DmaFinished(void)
{
    g_DmaThrouthputTestCnt++;
    #if SPI_MASTER_RX_ENABLE
    g_u8aSpiRxBufferIdx++;
    g_SpiRxDone = 0;
    #endif /* SPI_MASTER_RX_ENABLE */
    #if SPI_MASTER_TX_ENABLE
    g_SpiTxDone = 0;
    #endif /* SPI_MASTER_TX_ENABLE */
    osSemaphoreRelease(g_tAppSemaphore_SpiDma);
}

void Spi_DmaCallback(void)
{
    Hal_Vic_IntClear(APS_DMA_IRQn);
    #if SPI_MASTER_TX_ENABLE
    if (DMA->STS_TFR & (1 << SPI_MASTER_TX_DMA_CHANNEL))
    {
        Hal_Dma_IntClear(SPI_MASTER_TX_DMA_CHANNEL, DMA_BMP_TFR);
        g_SpiTxDone = 1;
    }
    #endif /* SPI_MASTER_TX_ENABLE */
    
    #if SPI_MASTER_RX_ENABLE
    if (DMA->STS_TFR & (1 << SPI_MASTER_RX_DMA_CHANNEL))
    {
        Hal_Dma_IntClear(SPI_MASTER_RX_DMA_CHANNEL, DMA_BMP_TFR);
        g_SpiRxDone = 1;
    }
    #endif /* SPI_MASTER_RX_ENABLE */

    #if SPI_MASTER_RX_ENABLE && SPI_MASTER_TX_ENABLE
    if (g_SpiTxDone && g_SpiRxDone)
    {
        Spi_DmaFinished();
    }
    #elif SPI_MASTER_RX_ENABLE
    if (g_SpiRxDone)
    {
        Spi_DmaFinished();
    }
    #elif SPI_MASTER_TX_ENABLE
    if (g_SpiTxDone)
    {
        Spi_DmaFinished();
    }
    #endif /* SPI_MASTER_RX_ENABLE */
}
// return : 0=ok; 1=timeout
uint8_t Dma_StsPoll(E_DMA_CHANNEL eChannel)
{
    uint32_t u32T1, u32T2;
    
    u32T1 = Hal_Tick_Diff(0);
    while (Hal_Dma_Check(eChannel))
    {
        u32T2 = Hal_Tick_Diff(u32T1);
        if (u32T2 > DMA_TIMEOUT_TICKS)
        {
            tracer_drct_printf("!!!!! DMA channel %d timeout, Abort !!!!!\n", eChannel);
            Hal_Dma_Abort(eChannel);
            return 1;
        }
    }
    return 0;
}
uint32_t Spi_Master_TrxDmaInit(void)
{
    S_SPI_MST_Reg_t *pSpi = NULL;
    E_SpiIdx_t eSpiIdx = TEST_SPI;
    
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    
    Hal_Dma_Init();   
    
    #if SPI_MASTER_RX_ENABLE
    {
        E_DMA_CHANNEL eRxChannel = SPI_MASTER_RX_DMA_CHANNEL;
        S_DMA_Init_t tRxDma;
        memset(&tRxDma, 0, sizeof(S_DMA_Init_t));
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
        DmaRxDarReg = (uint32_t)&(g_pscaHalDmaCHx[eRxChannel]->DAR);
        // Check DMA Channel is idle
        if (Dma_StsPoll(eRxChannel))
            return 0;
        
        /* Setup interrupt */
        Hal_Dma_CallBackSet(eRxChannel, Spi_DmaCallback);
        Hal_Dma_IntMask(eRxChannel, (DMA_BMP_TFR));
        Hal_Dma_IntClear(eRxChannel, (DMA_BMP_TFR));
        Hal_Dma_IntEn(ENABLE, eRxChannel);
        Hal_Dma_Config(eRxChannel, &tRxDma);
    }
    #endif /* SPI_MASTER_RX_ENABLE */
    #if SPI_MASTER_TX_ENABLE
    {
        E_DMA_CHANNEL eTxChannel = SPI_MASTER_TX_DMA_CHANNEL;
        S_DMA_Init_t tTxDma;
        memset(&tTxDma, 0, sizeof(S_DMA_Init_t));
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
        // Check DMA Channel is idle
        if (Dma_StsPoll(eTxChannel))
          return 0;
        
        /* Setup interrupt */
        Hal_Dma_CallBackSet(eTxChannel, Spi_DmaCallback);
        Hal_Dma_IntMask(eTxChannel, (DMA_BMP_TFR));
        Hal_Dma_IntClear(eTxChannel, (DMA_BMP_TFR));
        Hal_Dma_IntEn(ENABLE, eTxChannel);
        Hal_Dma_Config(eTxChannel, &tTxDma);
        DmaTxSarReg = (uint32_t)&(g_pscaHalDmaCHx[eTxChannel]->SAR);    
}
    #endif /* SPI_MASTER_TX_ENABLE */

    return 1;
}

void Spi_Communicate(void)
{
    E_SpiIdx_t eSpiIdx = TEST_SPI;
    S_SPI_MST_Reg_t *pSpi = NULL;
    if (eSpiIdx == SPI_IDX_1)
        pSpi = SPI1;
    else if (eSpiIdx == SPI_IDX_3)
        pSpi = SPI3;
    uint8_t DmaRxEn = 0, DmaRxLvl = 0;
    
    #if SPI_MASTER_RX_ENABLE
    {
        E_DMA_CHANNEL eRxChannel = SPI_MASTER_RX_DMA_CHANNEL;
        
        DmaRxEn = 1;
        DmaRxLvl = 15;
        reg_write(DmaRxDarReg, (uint32_t)SPI_RX_BUFF_CURR);
        
        // Data (using DMA)
        // enable Receive DMA
        Hal_Spi_Dma_Config(eSpiIdx, 0, DmaRxEn, 0, DmaRxLvl);
        Hal_Dma_Enable(eRxChannel); // RX DMA
    }
    #endif /* SPI_MASTER_RX_ENABLE */
    #if SPI_MASTER_TX_ENABLE
    {
        E_DMA_CHANNEL eTxChannel = SPI_MASTER_TX_DMA_CHANNEL;
        reg_write(DmaTxSarReg, (uint32_t)g_u8aSpiTxPattern);
        // enable Transmit DMA
        Hal_Spi_Dma_Config(eSpiIdx, 1, DmaRxEn, 16, DmaRxLvl);
        
        // disable slave
        Hal_Spi_Slave_Select_Enable(eSpiIdx, SPI_SLAVE_0, 0);
        Hal_Dma_Enable(eTxChannel); // TX DMA
        while (!(pSpi->TXFLR >= 16));
        Hal_Spi_Slave_Select_Enable(eSpiIdx, SPI_SLAVE_0, 1);
    }
    #endif /* SPI_MASTER_TX_ENABLE */
}
void Spi_SyncGpioCallback(E_GpioIdx_t eIdx)
{
    if (eIdx == SYNC_GPIO_IDX)
    {
        osSemaphoreRelease(g_tAppSemaphore_Sync);
        Hal_Gpio_IntDeInit(SYNC_GPIO_IDX);
    }
}
#if TEST_RX_SAMPLE_DELAY
void Spi_TestRxSampleDelayLoop(void)
{
    uint32_t u32Delay = 0;
    uint32_t u32DelayMax = 4;
    tracer_drct_printf("Rx sample delay test, baud rate = %d\n", Hal_Spi_BaudRateGet(TEST_SPI));
    while (1)
    {
        _Hal_Spi_Poll_Busy(TEST_SPI);
        Hal_Spi_RxSampleDly_Set(TEST_SPI, u32Delay);
        #define MASTER_TX_SYNC 0x5A
        #if SPI_MASTER_TX_ENABLE
        Hal_Spi_Data_Send(TEST_SPI, MASTER_TX_SYNC);
        #endif /* SPI_MASTER_TX_ENABLE */
        #if SPI_MASTER_RX_ENABLE
        uint32_t RxAck = 0;
        Hal_Spi_Data_Recv(TEST_SPI, &RxAck);
        tracer_drct_printf("[Master@%d]T:%02X, R:%02X\n", u32Delay, MASTER_TX_SYNC, RxAck);
        #endif /* SPI_MASTER_RX_ENABLE */
        if (++u32Delay > u32DelayMax)
        {
            u32Delay = 0;
        }
        osDelay(200);
    }
}
#endif /* TEST_RX_SAMPLE_DELAY */
void Spi_Sync(void)
{
    Hal_Gpio_IntInit(SYNC_GPIO_IDX, SYNC_GPIO_INT_TRIG, Spi_SyncGpioCallback);
    osSemaphoreWait(g_tAppSemaphore_Sync, osWaitForever);
}
void Dump_SpiRxData(uint8_t *pu8aTxData, uint8_t *pu8RxData, uint32_t u32Size)
{
    uint32_t u32Idx;
    
    if (pu8aTxData)
    {
        tracer_drct_printf("[Master TX data DMA mode]:");
        for (u32Idx=0; u32Idx<u32Size; u32Idx++)
        {
            if (u32Idx % SPI_BUFF_DUMP_PER_LINE == 0)
                tracer_drct_printf("\n");
            tracer_drct_printf("%02X ", pu8aTxData[u32Idx]);
        }
    }
    if (pu8RxData)
    {
        tracer_drct_printf("\n[Master RX data DMA mode]:");
        for (u32Idx=0; u32Idx<u32Size; u32Idx++)
        {
            if (u32Idx % SPI_BUFF_DUMP_PER_LINE == 0)
                tracer_drct_printf("\n");
            tracer_drct_printf("%02X ", pu8RxData[u32Idx]);
        }
    }
    tracer_drct_printf("\n\n");
}

void Main_AppThread(void *argu)
{
    #if EN_DATA_CHECKING
    uint32_t FailCnt=0, TotalCnt=0;
    #endif /* EN_DATA_CHECKING */
    #if DEBUG_DUMP_DATA
    uint32_t u32Cnt = 0;
    #endif /* DEBUG_DUMP_DATA */
    #if !STRESS_TEST_MODE
    uint32_t u32Startms, u32Endms;
    #endif /* STRESS_TEST_MODE */
    
    sys_cfg_clk_set(SYS_CFG_CLK_DECI_080_MHZ);
    
    // Init SPIx
    if (Hal_Spi_Init_Ext(TEST_SPI, TEST_SPI_SLAVE, SPI_BAUDRATE, SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1) !=0)
    {
        // Config SPI
        S_SPI_Config_t tCfg;
        memset(&tCfg, 0, sizeof(S_SPI_Config_t));
        tCfg.eSpi_frf = SPI_FRF_STD;
        tCfg.eDfs_32 = SPI_DFS_08_bit;
        #if SPI_MASTER_RX_ENABLE && SPI_MASTER_TX_ENABLE
        tCfg.eTmod = SPI_TMOD_TRX;
        #elif SPI_MASTER_RX_ENABLE
        tCfg.eTmod = SPI_TMOD_RX_ONLY;
        #elif SPI_MASTER_TX_ENABLE
        tCfg.eTmod = SPI_TMOD_TX_ONLY;
        #endif
        Hal_Spi_Master_Config(TEST_SPI, &tCfg);
        tracer_drct_printf("SPI_%d init fail\n", TEST_SPI);
        osThreadTerminate(NULL);
        return;
    }
    //Hal_Sys_Spi1SrcSelect(APS_CLK_SPI1_SRC_DECI_160M_BB, APS_CLK_SPI1_DIV_1);
    Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_DECI_160M_BB, APS_CLK_SPI3_DIV_3);
    
    Hal_Spi_RxSampleDly_Set(TEST_SPI, SPI_RX_SAMPLE_DELAY);
    
    #if TEST_RX_SAMPLE_DELAY
    Spi_TestRxSampleDelayLoop();
    #endif /* TEST_RX_SAMPLE_DELAY */
    
    if (!Spi_Master_TrxDmaInit())
    {
        tracer_drct_printf("Init DMA fail\n");
        osThreadTerminate(NULL);
        return;
    }

    tracer_drct_printf("SPI_%d Running: DMA mode\n", TEST_SPI);
    #if SPI_MASTER_RX_ENABLE
    memset(g_u8aSpiRxBuffer, 0, sizeof(g_u8aSpiRxBuffer));
    #endif /* SPI_MASTER_RX_ENABLE */
    
    
    /* Sync first */
    Spi_Sync();
    /* Start to TX/RX */
    Spi_Communicate();
    #if !STRESS_TEST_MODE
    u32Startms = osKernelSysTick();
    #endif /* STRESS_TEST_MODE */
    
    while(1)
    {
        osSemaphoreWait(g_tAppSemaphore_SpiDma, osWaitForever);
        #if !STRESS_TEST_MODE
        u32Endms = osKernelSysTick();
        #endif /* STRESS_TEST_MODE */
        
        #if EN_DATA_CHECKING
        {
            uint8_t result;
            TotalCnt++;
            result = SpiDataCompare(SPI_RX_BUFF_PREV, g_u8aSpiRxPattern, SEND_SPI_BUFFER_LEN);
            if (!result)
                FailCnt++;
        }
        #endif /* EN_DATA_CHECKING */
        #if DEBUG_DUMP_DATA
        tracer_drct_printf("Master [%d]\n", u32Cnt++);
        Dump_SpiRxData(g_u8aSpiTxPattern, NULL, SEND_SPI_BUFFER_LEN);
        #endif /* DEBUG_DUMP_DATA */
        
        #if STRESS_TEST_MODE
        if ((g_DmaThrouthputTestCnt % STRESS_TEST_SHORT_REPORT_NUM) == 0)
        {
            tracer_drct_printf("[Maste] CPU clock = %.2f MHz\n", SystemCoreClockGet()/1000000.0);
            tracer_drct_printf("SPI rate = %d bps\n", Hal_Spi_BaudRateGet(TEST_SPI));
            tracer_drct_printf("TRX buffer %d bytes per block, block counts = %d\n", SEND_SPI_BUFFER_LEN, g_DmaThrouthputTestCnt);
            #if EN_DATA_CHECKING
            tracer_drct_printf("RX blocks Fail/Total: %d/%d\n\n", FailCnt, TotalCnt);
            #else
            tracer_drct_printf("\n");
            #endif /* EN_DATA_CHECKING */
        }
        #else /* STRESS_TEST_MODE */
        if (g_DmaThrouthputTestCnt == CALC_BLK_CNT)
        {
            double elapsed_time = (double)(u32Endms - u32Startms)/1000;
            double rate = (double)SEND_SPI_BUFFER_LEN * CALC_BLK_CNT / elapsed_time;
            uint32_t spi_rate = Hal_Spi_BaudRateGet(TEST_SPI);
            tracer_drct_printf("Master test done, elpased time : %lf seconds\n", elapsed_time);
            tracer_drct_printf("SPI rate = %d bps\n", spi_rate);
            tracer_drct_printf("           %.2lf kbps\n", spi_rate/1024.0);
            tracer_drct_printf("           %.4lf Mbps\n", spi_rate/1048576.0);
            tracer_drct_printf("Block size   = %d\n", SEND_SPI_BUFFER_LEN);
            tracer_drct_printf("Block counts = %d\n", CALC_BLK_CNT);
            tracer_drct_printf("Throughput = %.2lf bps\n", rate*8);
            tracer_drct_printf("             %.2lf kbps\n", rate*8/1024.0);
            tracer_drct_printf("             %.4lf Mbps\n", rate*8/1048576.0);
            #if EN_DATA_CHECKING
            tracer_drct_printf("\nRX blocks Fail/Total: %d/%d\n", FailCnt, TotalCnt);
            #endif /* EN_DATA_CHECKING */
            tracer_drct_printf("\n");
            break;
        }
        #endif /* STRESS_TEST_MODE */
        
        Spi_Sync();
        Spi_Communicate();
        #if SPI_MASTER_RX_ENABLE
        memset(SPI_RX_BUFF_PREV, 0, sizeof(SPI_RX_BUFF_PREV));
        #endif /* SPI_MASTER_RX_ENABLE */
    }
    #if !STRESS_TEST_MODE
    osThreadTerminate(NULL);
    #endif
}
