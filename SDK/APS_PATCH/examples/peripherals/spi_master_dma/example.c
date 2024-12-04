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
#include <stdlib.h>
#include "example.h"
#include "sys_os_config.h"
#include "sys_cfg.h"
#include "hal_spi.h"
#include "hal_dma.h"
#include "hal_tick.h"
#include "hal_pin.h"
#include "hal_system.h"
#include "at_cmd.h"
#include "at_cmd_task.h"
#include "at_cmd_common.h"
#include "at_cmd_data_process.h"



// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define TEST_SPI            SPI_IDX_3
#define TEST_SPI_SLAVE      SPI_SLAVE_0

#define SEND_SPI_BUFFER_LEN     2048    /* The maximum trans size */
#define SPI_BUFF_DUMP_PER_LINE  16

#define SYNC_GPIO_IDX               GPIO_IDX_07
#define SYNC_GPIO_INT_TRIG          VIC_TRIG_EDGE_FALLING


#define SPI_MASTER_TX_DMA_CHANNEL   DMA_Channel_0
#define SPI_MASTER_RX_DMA_CHANNEL   DMA_Channel_1

#define APP_MESSAGE_Q_SIZE          1
#define SPI_TRANS_BLOCK_SIZE        DMA_MSIZE_16


#define SPI_MAX_RX_SAMPLE_DELAY     4


#define SPI_SYNC_IO_JITTER_TIME_US  300     /* unit: us. When SyncIO falling edge IRQ triggered interval less than this value, ignore the interrupt */

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
osThreadId g_tAppThread_1;
osMessageQId g_tSpiDmaDoneMsgQ;
osSemaphoreId g_tSpiSyncSema, g_tCmdStartSema;


// Sec 5: declaration of global function prototype
void Main_AppThread(void *argu);
void Main_SpiDmaSetupCb(E_SpiIdx_t eSpiIdx, uint8_t *pu8TxData, uint8_t *pu8RxData, uint32_t u32Size);
void Main_SpiTrxDoneCb(E_SpiIdx_t eSpiIdx, E_HAL_SPI_DMA_STATUS_RESULT eResult);
bool Main_SpiTrxDoneResult(E_HAL_SPI_DMA_STATUS_RESULT eResult);

uint8_t Main_DataCompare(uint8_t *pu8Data1, uint8_t *pu8Data2, uint32_t u32Size);
int at_cmd_spi_start(char *buf, int len, int mode);
int at_cmd_spi_cfg(char *buf, int len, int mode);
int at_cmd_spi_dump(char *buf, int len, int mode);
/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
uint8_t g_u8aSpiRxBuffer[SEND_SPI_BUFFER_LEN] __attribute__((aligned(16)));
extern uint8_t g_u8aSpiMasterPattern[2048];
extern uint8_t g_u8aSpiSlavePattern[2048];
uint64_t g_DmaThrouthputTestCnt=0;
uint32_t g_SpiFailCnt,g_SpiTotalCnt;

uint32_t g_u32SpiCfg_IsChkData=0;
uint32_t g_u32SpiCfg_TransSize=SEND_SPI_BUFFER_LEN;
uint32_t g_u32SpiCfg_TransRound=1000;

uint32_t g_u32EnableDumpData = 0;   /* 0: off, 1: on, 2: on when error */

at_command_t gAtCmdTbl_SpiTest[] =
{
    { "at+spistart",            at_cmd_spi_start,           "Run SPI test"},
    { "at+spicfg",              at_cmd_spi_cfg,             "Configure SPI"},
    { "at+spidump",             at_cmd_spi_dump,            "Dump data"},
};

// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
void example( void )
{
    osThreadDef_t tThreadDef;
    osMessageQDef_t tMessageDef;
    osSemaphoreDef_t tSemDef = {0};
    
    g_tSpiSyncSema = osSemaphoreCreate(&tSemDef, 1);
    osSemaphoreWait(g_tSpiSyncSema, osWaitForever);

    // create the thread for Main-app thread
    tThreadDef.name = "MainApp";
    tThreadDef.pthread = Main_AppThread;
    tThreadDef.tpriority = OS_TASK_PRIORITY_APP;        // osPriorityNormal
    tThreadDef.instances = 0;                           // reserved, it is no used
    tThreadDef.stacksize = OS_TASK_STACK_SIZE_APP;      // (512), unit: 4-byte, the size is 512*4 bytes
    g_tAppThread_1 = osThreadCreate(&tThreadDef, NULL);
    if (g_tAppThread_1 == NULL)
    {
        msg_print_uart1("To create the thread for Main_AppThread is fail.\n");
    }
    
    g_tCmdStartSema = osSemaphoreCreate(&tSemDef, 1);
    osSemaphoreWait(g_tCmdStartSema, osWaitForever);
    
    // create the message queue for AppMessageQ
    tMessageDef.queue_sz = APP_MESSAGE_Q_SIZE;          // number of elements in the queue
    tMessageDef.item_sz = sizeof(E_HAL_SPI_DMA_STATUS_RESULT);           // size of an item
    tMessageDef.pool = NULL;                            // reserved, it is no used
    g_tSpiDmaDoneMsgQ = osMessageCreate(&tMessageDef, g_tAppThread_1);
    if (g_tSpiDmaDoneMsgQ == NULL)
    {
        msg_print_uart1("To create the message queue for g_tSpiDmaDoneMsgQ is fail.\n");
    }
    
    at_cmd_ext_tbl_reg( &gAtCmdTbl_SpiTest );
}

int at_cmd_spi_start(char *buf, int len, int mode)
{
    int ret = -1;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    switch (mode)
    {
        case AT_CMD_MODE_SET:
        {
            /* at+spidma=trans_round,data_check */
            if (argc <= 2)
                break;
            
            g_u32SpiCfg_TransRound = strtoul(argv[1], NULL, 10);
            g_u32SpiCfg_IsChkData = strtoul(argv[2], NULL, 10)?1:0;
            
            
            msg_print_uart1("SPI master test. Trans size:%d bytes, rounds: %d. Data checking:%d\n", g_u32SpiCfg_TransSize, g_u32SpiCfg_TransRound, g_u32SpiCfg_IsChkData);
            msg_print_uart1("\r\nOK\r\n");
            ret = 0;
            osSemaphoreRelease(g_tCmdStartSema);
        }
        default:
            break;
    }
    
    if (ret != 0)
        msg_print_uart1("\r\nERROR\r\n");
    return ret;
}

int at_cmd_spi_cfg(char *buf, int len, int mode)
{
    int ret = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    switch (mode)
    {
        case AT_CMD_MODE_SET:
        {
            uint32_t u32SpiClk;
            uint32_t u32RxSampleDelay;
            uint32_t u32RefClk, u32SpiRate;
            /* at+spicfg=spi_rate,rx_sample_delay */
            
            /*
             * SPI source | Sys clock | SPI divider | SPI rate
             * -----------+-----------+-------------+---------
             *  160div1   |  160div2  |     4       |  40
             *  160div1   |  160div2  |     6       |  26
             *  160div1   |  160div2  |     8       |  20
             *  160div1   |  160div2  |     10      |  16
             *  160div1   |  160div2  |     12      |  13
             *  116div1   |  116div1  |     4       |  29
             */
            
            if (argc <= 2)
                break;
            u32SpiClk = strtoul(argv[1], NULL, 10);
            
            switch (u32SpiClk)
            {
                case 40:
                    sys_cfg_clk_set((T_SysCfgClkIdx)SYS_CFG_CLK_DECI_080_MHZ);
                    Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_DECI_160M_BB, APS_CLK_SPI3_DIV_1);
                    Hal_Spi_BaudRateSet(TEST_SPI, 40000000);
                    break;
                case 26:
                    sys_cfg_clk_set((T_SysCfgClkIdx)SYS_CFG_CLK_DECI_080_MHZ);
                    Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_DECI_160M_BB, APS_CLK_SPI3_DIV_1);
                    Hal_Spi_BaudRateSet(TEST_SPI, 26000000);
                    break;
                case 16:
                    sys_cfg_clk_set((T_SysCfgClkIdx)SYS_CFG_CLK_DECI_080_MHZ);
                    Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_DECI_160M_BB, APS_CLK_SPI3_DIV_1);
                    Hal_Spi_BaudRateSet(TEST_SPI, 16000000);
                    break;
                case 13:
                    sys_cfg_clk_set((T_SysCfgClkIdx)SYS_CFG_CLK_DECI_080_MHZ);
                    Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_DECI_160M_BB, APS_CLK_SPI3_DIV_1);
                    Hal_Spi_BaudRateSet(TEST_SPI, 13333333);
                    break;
                case 29:
                    sys_cfg_clk_set((T_SysCfgClkIdx)SYS_CFG_CLK_VAR_116_MHZ);
                    Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_150M_VAR, APS_CLK_SPI3_DIV_1);
                    Hal_Spi_BaudRateSet(TEST_SPI, 29000000);
                    break;
                default: //default 20M
                    sys_cfg_clk_set((T_SysCfgClkIdx)SYS_CFG_CLK_DECI_080_MHZ);
                    Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_DECI_160M_BB, APS_CLK_SPI3_DIV_1);
                    Hal_Spi_BaudRateSet(TEST_SPI, 20000000);
                    break;
            }
            
            msg_print_uart1("SPI master config.\n");
            Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI3, &u32RefClk);
            u32SpiRate = Hal_Spi_BaudRateGet(TEST_SPI);
            msg_print_uart1("  SPI clock = %d, rate = %d\n", u32RefClk, u32SpiRate);
            
            u32RxSampleDelay = strtoul(argv[2], NULL, 10);
            if (u32RxSampleDelay > SPI_MAX_RX_SAMPLE_DELAY)
                u32RxSampleDelay = SPI_MAX_RX_SAMPLE_DELAY;
            Hal_Spi_RxSampleDly_Set(TEST_SPI, u32RxSampleDelay);
            msg_print_uart1("  RX sample delay = %d\n", u32RxSampleDelay);
        }
        default:
            break;
    }
    msg_print_uart1("\r\nOK\r\n");
    
    return ret;
}


int at_cmd_spi_dump(char *buf, int len, int mode)
{
    int ret = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    switch (mode)
    {
        case AT_CMD_MODE_SET:
        {
            /* at+spidump=on/off */
            if (!strcasecmp(argv[1], "on"))
            {
                g_u32EnableDumpData = 1;
            }
            else if (!strcasecmp(argv[1], "off"))
            {
                g_u32EnableDumpData = 0;
            }
            else if (!strcasecmp(argv[1], "error"))
            {
                g_u32EnableDumpData = 2;
            }
            
            msg_print_uart1("  Enable dump data = %d\n", g_u32EnableDumpData);
        }
        default:
            break;
    }
    msg_print_uart1("\r\nOK\r\n");
    
    return ret;
}


void Spi_SyncGpioCallback(E_GpioIdx_t eIdx)
{
    static uint32_t u32LastTimeMs = 0;
    static uint32_t u32LastTimeTick = 0;
    uint32_t u32CurrTimeMs = osKernelSysTick();
    uint32_t u32CurrTimeTick = Hal_Tick_Diff(0);
    uint32_t u32TickJitterTick = (uint64_t)Hal_Tick_PerMilliSec() * SPI_SYNC_IO_JITTER_TIME_US / 1000;
    
    if (u32CurrTimeMs == u32LastTimeMs)
    {   /* Ignore too close IO interrupt. Only handle the interrupt when interval over SPI_SYNC_IO_JITTER_TIME_US */
        if (u32CurrTimeTick - u32LastTimeTick < u32TickJitterTick)
        {
            return;
        }
    }
    u32LastTimeMs = u32CurrTimeMs;
    u32LastTimeTick = u32CurrTimeTick;
    osSemaphoreRelease(g_tSpiSyncSema);
}


void Main_AppThread(void *argu)
{
    uint32_t ret;
    uint32_t u32Startms=0, u32Endms;

    sys_cfg_clk_set((T_SysCfgClkIdx)SYS_CFG_CLK_DECI_080_MHZ);
    // Init SPIx
    if (Hal_Spi_Init_Ext(TEST_SPI, TEST_SPI_SLAVE, SPI_BAUDRATE, SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1) !=0)
    {
        msg_print_uart1("SPI_%d init fail\n", TEST_SPI);
        osThreadTerminate(NULL);
        return;
    }
    Hal_Spi_RxSampleDly_Set(TEST_SPI, SPI_RX_SAMPLE_DELAY);
    Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_DECI_160M_BB, APS_CLK_SPI3_DIV_1);
    
    uint32_t u32RefClk;
    Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI3, &u32RefClk);
    msg_print_uart1("SPI clock = %d\n", u32RefClk);
    
    Hal_Spi_DmaInit(TEST_SPI, SPI_SLAVE_0, SPI_MASTER_TX_DMA_CHANNEL, SPI_MASTER_RX_DMA_CHANNEL, Main_SpiDmaSetupCb, Main_SpiTrxDoneCb);

    msg_print_uart1("SPI_%d Running: DMA mode\n", TEST_SPI);
    osDelay(100);
    memset(g_u8aSpiRxBuffer, 0, sizeof(g_u8aSpiRxBuffer));
    Hal_Gpio_IntInit(SYNC_GPIO_IDX, SYNC_GPIO_INT_TRIG, Spi_SyncGpioCallback); //Edge detect SyncIO
    
    
    while (1)
    {
        osEvent rxEvent;
        
        osSemaphoreWait(g_tCmdStartSema, osWaitForever);
        bool bStop = false;
        
        g_DmaThrouthputTestCnt = 0;
        u32Startms = 0;
        g_SpiFailCnt = 0;
        g_SpiTotalCnt = 0;
        
        while (g_DmaThrouthputTestCnt < g_u32SpiCfg_TransRound)
        {
            if (g_DmaThrouthputTestCnt)
            {
                osSemaphoreWait(g_tSpiSyncSema, osWaitForever);
            }
            else
            {   /* First transaction assumed ready */
                msg_print_uart1("Start SPI test\n");
                u32Startms = osKernelSysTick();
            }
            /* Start to TX/RX */
            ret = Hal_Spi_TransmitReceive_Dma(TEST_SPI, g_u8aSpiMasterPattern, g_u8aSpiRxBuffer, g_u32SpiCfg_TransSize, SPI_TRANS_BLOCK_SIZE);
            if (ret)
            {
                msg_print_uart1("TRX DMA fail\n");
                continue;
            }
            
            rxEvent = osMessageGet(g_tSpiDmaDoneMsgQ, osWaitForever);
            if(rxEvent.status == osEventMessage)
            {
                bStop = Main_SpiTrxDoneResult((E_HAL_SPI_DMA_STATUS_RESULT)rxEvent.value.v);
            }
            
            if (bStop || (g_DmaThrouthputTestCnt >= g_u32SpiCfg_TransRound))
            {
                double elapsed_time, rate;
                _Hal_Spi_Poll_Busy(TEST_SPI);
                u32Endms = osKernelSysTick();
                elapsed_time = (double)(u32Endms - u32Startms)/1000;
                rate = (double)g_u32SpiCfg_TransSize * g_DmaThrouthputTestCnt / elapsed_time;
                
                uint32_t spi_rate = Hal_Spi_BaudRateGet(TEST_SPI);
                msg_print_uart1("Master test done, elpased time : %lf seconds\n", elapsed_time);
                msg_print_uart1("SPI rate = %.4lf Mbps\n", spi_rate/1000000.0);
                msg_print_uart1("Trans size = %d\n", g_u32SpiCfg_TransSize);
                msg_print_uart1("Trans run = %llu\n", g_DmaThrouthputTestCnt);
                msg_print_uart1("Throughput = %.4lf Mbps\n", rate*8/1000000.0);
                if (g_u32SpiCfg_IsChkData)
                {
                    msg_print_uart1("\nRX blocks Fail/Total: %d/%d\n", g_SpiFailCnt, g_SpiTotalCnt);
                }
                else
                {
                    msg_print_uart1("Final data checking result: ");
                    if (Main_DataCompare(g_u8aSpiRxBuffer, g_u8aSpiSlavePattern, g_u32SpiCfg_TransSize))
                    {
                        msg_print_uart1("Matched\n");
                    }
                    else
                    {
                        msg_print_uart1("Mismatched\n");
                    }
                }
                break;
            }
        }
    }
}
uint8_t Main_DataCompare(uint8_t *pu8Data1, uint8_t *pu8Data2, uint32_t u32Size)
{
    while (u32Size--)
    {
        if (*pu8Data1++ != *pu8Data2++)
            return 0;
    }
    
    return 1;
}

void Main_SpiDmaSetupCb(E_SpiIdx_t eSpiIdx, uint8_t *pu8TxData, uint8_t *pu8RxData, uint32_t u32Size)
{
    //Do nothing in master
}

void Main_SpiTrxDoneCb(E_SpiIdx_t eSpiIdx, E_HAL_SPI_DMA_STATUS_RESULT eResult)
{
    osMessagePut(g_tSpiDmaDoneMsgQ, (uint32_t)eResult, 0);
}

bool Main_SpiTrxDoneResult(E_HAL_SPI_DMA_STATUS_RESULT eResult)
{
    bool bDump = false;
    g_DmaThrouthputTestCnt++;
    if (g_u32EnableDumpData == 1)
    {
        bDump = true;
    }
    
    if (g_u32SpiCfg_IsChkData)
    {
        uint8_t result;
        g_SpiTotalCnt++;
        result = Main_DataCompare(g_u8aSpiRxBuffer, g_u8aSpiSlavePattern, g_u32SpiCfg_TransSize);
        if (!result)
        {
            g_SpiFailCnt++;
            if (g_u32EnableDumpData == 2)
            {
                bDump = true;
            }
        }
    }
    
    if (bDump == true)
    {
        uint32_t u32Idx;
        uint8_t *pRxData = g_u8aSpiRxBuffer;
        uint32_t u32Size = g_u32SpiCfg_TransSize;

        msg_print_uart1("\n[Master RX data DMA mode]:");
        for (u32Idx=0; u32Idx<u32Size; u32Idx++)
        {
            if (u32Idx % SPI_BUFF_DUMP_PER_LINE == 0)
                msg_print_uart1("\n");
            msg_print_uart1("%02X ", pRxData[u32Idx]);
        }
        msg_print_uart1("\n\n");
    }
    
    return bDump;
}

