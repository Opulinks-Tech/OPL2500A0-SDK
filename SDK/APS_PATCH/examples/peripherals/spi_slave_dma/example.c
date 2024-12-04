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
#define TEST_SPI                SPI_IDX_2
#define SEND_SPI_BUFFER_LEN     2048
#define SPI_BUFF_DUMP_PER_LINE  16

#define SPI_CS_PIN                  GPIO_IDX_25

#define SYNC_GPIO_IDX               GPIO_IDX_09
#define SYNC_GPIO_LEVEL             GPIO_LEVEL_LOW
#define SYNC_GPIO_LEVEL_INV         GPIO_LEVEL_HIGH

#define SPI_MASTER_TX_DMA_CHANNEL   DMA_Channel_0
#define SPI_MASTER_RX_DMA_CHANNEL   DMA_Channel_1

#define APP_MESSAGE_Q_SIZE          1
#define SPI_TRANS_BLOCK_SIZE        DMA_MSIZE_16
#define DEBUG_DUMP_DATA             0


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
osThreadId g_tAppThread_1;
osMessageQId g_tAppMessageQ;


// Sec 5: declaration of global function prototype
void Main_AppThread(void *argu);
void Main_SpiDmaSetupCb(E_SpiIdx_t eSpiIdx, uint8_t *pu8TxData, uint8_t *pu8RxData, uint32_t u32Size);
void Main_SpiTrxDoneCb(E_SpiIdx_t eSpiIdx, E_HAL_SPI_DMA_STATUS_RESULT eResult);
void Main_SpiTrxDoneResult(E_HAL_SPI_DMA_STATUS_RESULT eResult);
uint8_t Main_DataCompare(uint8_t *pu8Data1, uint8_t *pu8Data2, uint32_t u32Size);

int at_cmd_spi_result(char *buf, int len, int mode);
int at_cmd_spi_check(char *buf, int len, int mode);
int at_cmd_spi_dump(char *buf, int len, int mode);
/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
uint8_t g_u8aSpiRxBuffer[SEND_SPI_BUFFER_LEN] __attribute__((aligned(16)));
extern uint8_t g_u8aSpiMasterPattern[2048];
extern uint8_t g_u8aSpiSlavePattern[2048];

uint32_t g_SpiFailCnt,g_SpiTotalCnt;
uint32_t g_EnableSpiDataCheck=1;
uint32_t g_u32EnableDumpData = 0;   /* 0: off, 1: on, 2: on when error */


at_command_t gAtCmdTbl_SpiTest[] =
{
    { "at+spiresult",           at_cmd_spi_result,          "Show result or clean result"},
    { "at+spicheck",            at_cmd_spi_check,           "To enable or disable data checking"},
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
    
    // create the message queue for AppMessageQ
    tMessageDef.queue_sz = APP_MESSAGE_Q_SIZE;          // number of elements in the queue
    tMessageDef.item_sz = sizeof(E_HAL_SPI_DMA_STATUS_RESULT);           // size of an item
    tMessageDef.pool = NULL;                            // reserved, it is no used
    g_tAppMessageQ = osMessageCreate(&tMessageDef, g_tAppThread_1);
    if (g_tAppMessageQ == NULL)
    {
        printf("To create the message queue for AppMessageQ is fail.\n");
    }
    
    at_cmd_ext_tbl_reg( &gAtCmdTbl_SpiTest );
}

int at_cmd_spi_result(char *buf, int len, int mode)
{
    int ret = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    /* at+spiresult=clean
     * at+spiresult?
     */
    switch (mode)
    {
        case AT_CMD_MODE_SET:
        {
            if (!strcasecmp(argv[1], "clean"))
            {
                g_SpiTotalCnt = 0;
                g_SpiFailCnt = 0;
            }
        }
        break;
        case AT_CMD_MODE_READ:
        case AT_CMD_MODE_EXECUTION:
        {
            if (g_EnableSpiDataCheck)
            {
                msg_print_uart1("SPI slave total received: %u\n"
                                "              fail count: %u\n", g_SpiTotalCnt, g_SpiFailCnt);
            }
            else
            {
                msg_print_uart1("SPI slave total received: %u\n", g_SpiTotalCnt);
                msg_print_uart1("Final data checking result: ");
                if (Main_DataCompare(g_u8aSpiRxBuffer, g_u8aSpiMasterPattern, SEND_SPI_BUFFER_LEN))
                {
                    msg_print_uart1("Matched\n");
                }
                else
                {
                    msg_print_uart1("Mismatched\n");
                }
            }
            
        }
        break;
        default:
            break;
    }
    msg_print_uart1("\r\nOK\r\n");
    
    return ret;
}
int at_cmd_spi_check(char *buf, int len, int mode)
{
    int ret = 0;
    int argc = 0;
    char *argv[AT_MAX_CMD_ARGS] = {0};

    at_cmd_buf_to_argc_argv(buf, &argc, argv, AT_MAX_CMD_ARGS);

    /* at+spicheck=1/0
     * at+spicheck?
     */
    switch (mode)
    {
        case AT_CMD_MODE_SET:
        {
            g_EnableSpiDataCheck = strtoul(argv[1], NULL, 10)?1:0;
            g_SpiTotalCnt = 0;
            g_SpiFailCnt = 0;
            msg_print_uart1("Result cleaned\n");
        }
        //break; Show setting
        case AT_CMD_MODE_READ:
        case AT_CMD_MODE_EXECUTION:
        {
            msg_print_uart1("Data checking: %d\n", g_EnableSpiDataCheck);
        }
        break;
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

void Main_AppThread(void *argu)
{
    uint32_t ret;

    sys_cfg_clk_set((T_SysCfgClkIdx)SYS_CFG_CLK_DECI_080_MHZ);

    // Init SPIx
    if (Hal_Spi_Init(TEST_SPI, 0, SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1) !=0)
    {   /* baud rate is ignored when initialized as slave mode */
        printf("SPI_2 init slave fail\n");
        osThreadTerminate(NULL);
        return;
    }
    Hal_Sys_Spi2SrcSelect(APS_CLK_SPI2_SRC_DECI_160M_BB, APS_CLK_SPI2_DIV_1);
    Hal_Spi_DmaInit(TEST_SPI, SPI_SLAVE_0, SPI_MASTER_TX_DMA_CHANNEL, SPI_MASTER_RX_DMA_CHANNEL, Main_SpiDmaSetupCb, Main_SpiTrxDoneCb);

    msg_print_uart1("SPI_%d Running: DMA mode\n", TEST_SPI);
    osDelay(100);
    memset(g_u8aSpiRxBuffer, 0, sizeof(g_u8aSpiRxBuffer));
    
    while(1)
    {
        osEvent rxEvent;
        /* Start to TX/RX */
        ret = Hal_Spi_TransmitReceive_Dma(TEST_SPI, g_u8aSpiSlavePattern, g_u8aSpiRxBuffer, SEND_SPI_BUFFER_LEN, SPI_TRANS_BLOCK_SIZE);
        if (ret)
        {
            msg_print_uart1("TRX DMA fail\n");
            continue;
        }

        rxEvent = osMessageGet(g_tAppMessageQ, osWaitForever);
        if(rxEvent.status == osEventMessage)
        {
            Main_SpiTrxDoneResult((E_HAL_SPI_DMA_STATUS_RESULT)rxEvent.value.v);
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
    /* Pull SyncIO to low when DMA setup done */
    Hal_Gpio_Output(SYNC_GPIO_IDX, SYNC_GPIO_LEVEL);
}

void Main_SpiTrxDoneCb(E_SpiIdx_t eSpiIdx, E_HAL_SPI_DMA_STATUS_RESULT eResult)
{
    Hal_Gpio_Output(SYNC_GPIO_IDX, SYNC_GPIO_LEVEL_INV);
    osMessagePut(g_tAppMessageQ, (uint32_t)eResult, 0);
}

void Main_SpiTrxDoneResult(E_HAL_SPI_DMA_STATUS_RESULT eResult)
{
    bool bDump = false;
    if (g_u32EnableDumpData == 1)
    {
        bDump = true;
    }

    g_SpiTotalCnt++;
    if (g_EnableSpiDataCheck)
    {
        uint8_t result;
        result = Main_DataCompare(g_u8aSpiRxBuffer, g_u8aSpiMasterPattern, SEND_SPI_BUFFER_LEN);
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
        uint32_t u32Size = SEND_SPI_BUFFER_LEN;
        
        msg_print_uart1("[Slave RX data DMA mode]:");
        for (u32Idx=0; u32Idx<u32Size; u32Idx++)
        {
            if (u32Idx % SPI_BUFF_DUMP_PER_LINE == 0)
                msg_print_uart1("\n");
            msg_print_uart1("%02X ", pRxData[u32Idx]);
        }
        msg_print_uart1("\n\n");
    }
}

