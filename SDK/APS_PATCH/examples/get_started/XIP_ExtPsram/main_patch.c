/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  main_patch.c
*
*  Project:
*  --------
*  OPL2500 Project - the main patch implement file
*
*  Description:
*  ------------
*  This implement file is include the main patch function and api.
*
*  Author:
*  -------
*  FW team
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "sys_init.h"
#include "hal_system.h"
#include "mw_fim.h"
#include "cmsis_os.h"
#include "sys_os_config.h"
#include "hal_pin.h"
#include "hal_pin_def.h"
#include "hal_pin_config_project.h"
#include "hal_dbg_uart.h"
#include "hal_vic.h"
#include "boot_sequence.h"
#include "freertos_cmsis.h"
#include "at_cmd_common.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the number of elements in the message queue
#define APP_MESSAGE_Q_SIZE  16
#define DEMO_XIP_RODATA_LEN 128
#define DEMO_PHEAP_ALLOC_WINDOW_SIZE     16 /* Must be power of 2 (2^n) */


#define EN_MALLOC_AUTO_PHEAP        1           /* if enable this configuration,
                                                 *   malloc can automatically allocate memory from PSRAM.
                                                 *
                                                 * mallocPHeap can allocate memory from PSRAM either this mode enable or not */

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list
// the content of message queue
typedef struct
{
    uint32_t ulCount;
} S_MessageQ;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
PSRAM_DATA uint16_t g_u16aDummyPattern[128] = {
    0xE6F0, 0xF503, 0x70B6, 0x75C8, 0xA5FC, 0xDE95, 0xCAEF, 0x3335,
    0x0C6F, 0x7E18, 0xE040, 0x0EFA, 0x17ED, 0x45F3, 0x01BA, 0x7C82,
    0x7109, 0x653E, 0xFEAE, 0xD8D4, 0xB647, 0x8A27, 0x2A77, 0x41A8,
    0x2356, 0x8EA7, 0x90B4, 0x041E, 0xCCFD, 0x29A2, 0x6CA7, 0x7BDA,
    0x420B, 0xC507, 0xFD48, 0x5303, 0x3CAD, 0x9740, 0xC652, 0x5F5D,
    0xF020, 0xDB73, 0xBA6B, 0xB208, 0xEB7A, 0x2B77, 0x51AF, 0xEEF2,
    0x95C6, 0x0523, 0x0991, 0x949B, 0xD80D, 0xCC99, 0x474C, 0xA6DF,
    0x16D1, 0xEFCD, 0xF232, 0xEB5E, 0x9DC5, 0xA59E, 0xC364, 0xAC3E,
    0x657B, 0x96EC, 0x9462, 0xAF4C, 0x3922, 0x7086, 0x6CDD, 0x09C3,
    0xE344, 0x2F08, 0x96C0, 0x1988, 0xE51B, 0x0260, 0xFFCC, 0xB74B,
    0x6EF8, 0x7E5B, 0x4C40, 0xE826, 0xE1FE, 0xF45B, 0x20B7, 0x86D5,
    0xE047, 0x036E, 0x4A00, 0x1E33, 0xB823, 0xD8AF, 0xD735, 0x9C85,
    0xF325, 0x9F46, 0x00AA, 0x3012, 0x9D68, 0x365D, 0x4718, 0x3398,
    0x7966, 0x9C9F, 0xA5E8, 0x6B23, 0x63D5, 0xAC2F, 0xFB4C, 0xE6E8,
    0xD812, 0x9ED1, 0xDC85, 0xDA26, 0x65C1, 0x2BCA, 0x93B8, 0xCE3D,
    0xEEF2, 0xD3C2, 0xE1E2, 0xB52B, 0xA170, 0x8AE8, 0xF1C1, 0x6629,
};
// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
static E_PIN_MAIN_UART_MODE g_eAppMainUartMode = HAL_PIN_MAIN_UART_MODE_PATCH;
static osThreadId g_tAppThread_1;
static osThreadId g_tAppThread_2;

static osMessageQId g_tAppMessageQ;
static osPoolId g_tAppMemPoolId;

// Sec 7: declaration of static function prototype
void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT")));
void __Patch_EntryPoint(void) __attribute__((used));
static void Main_HeapPatchInit(void);
static void Main_PinMuxUpdate(void);
static void Main_MiscModulesInit(void);
static void Main_FlashLayoutUpdate(void);
static void Main_AppInit_patch(void);
static void Main_AtUartDbgUartSwitch(void);
static void Main_AppThread_1(void *argu);
static void Main_AppThread_2(void *argu);
static osStatus Main_AppMessageQSend(S_MessageQ *ptMsg);

/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   __Patch_EntryPoint
*
* DESCRIPTION:
*   the entry point of SW patch
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void __Patch_EntryPoint(void)
{
    /* PSRAM initialization */
    osPHeapPatchInit();
    osMemoryPHeapMallocMixModeEn(EN_MALLOC_AUTO_PHEAP);       /* Select to PHEAP mix mode or not */
    /* Assign PHEAP */
    vPortPHeapRegionInit((uint8_t* )APS_PHEAP_START, APS_PHEAP_LENGTH);
    
    Main_PinMuxUpdate(); /* Init SPI pin-mux first */
    Sys_XipSetup(XIP_MODE_EXT_PSRAM, SPI_SLAVE_1, 0);
    memset(ZI_REGION_PSRAM_START, 0, ZI_REGION_PSRAM_LENGTH);   /* Init PSRAM ZI region */
    
    // don't remove this code
    SysInit_EntryPoint();
    

    
#ifdef SWITCH_TO_32K_RC
    /* Not needs to setup, OPL2500 will auto detect 32k XTAL
     * When not found 32k XTAL, it will use 32k RC */
#endif 
    // update the pin mux
    Hal_SysPinMuxAppInit = Main_PinMuxUpdate;
    
    // update the flash layout
    MwFim_FlashLayoutUpdate = Main_FlashLayoutUpdate;
    
    /* APS_HEAP_START and APS_HEAP_LENGTH are from scatter/linker file
     * When needs to change HEAP size, please modify scatter/linker file.
     * Do NOT write the argument here */
    osHeapAssign(APS_HEAP_START, APS_HEAP_LENGTH);
    Main_HeapPatchInit();
    
    Sys_MiscModulesInit = Main_MiscModulesInit;
    
    // update the switch AT UART / dbg UART function
    at_cmd_switch_uart1_dbguart = Main_AtUartDbgUartSwitch;

    // application init
    Sys_AppInit = Main_AppInit_patch;
}

/*************************************************************************
* FUNCTION:
*   Main_HeadPatchInit
*
* DESCRIPTION:
*   Update HEAP setting here.
*   This function must be run after osHeapAssign. i.e. HEAP size updated first.
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_HeapPatchInit(void)
{
    osMemoryDef_t PartitionMemoryTable[MAX_NUM_MEM_POOL] = {
        /* {block_size, number}
         *     block_size: The memory block max allocation size.
         *     number: The number of the memory block of given block size.
         * The order of block size must be small to big.
         * When block_size or block_num is zero, it means end of table.
         *
         *{block_size, number} */
          {        32,     48},
          {        64,     32},
          {       128,     64},
          {       256,     28},
          {       512,     12},
          {         0,      0},
          {         0,      0},
          {         0,      0}
    };
    
    osMemoryPoolUpdate(PartitionMemoryTable);
}

/*************************************************************************
* FUNCTION:
*   Main_PinMuxUpdate
*
* DESCRIPTION:
*   update the pinmux setting
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
FORCE_RAM_TEXT static void Main_PinMuxUpdate(void)
{
    Hal_SysDisableAllTxPeriphPinmux();
    Hal_SysDisableAllRxPin();

    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_0);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_1);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_2);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_3);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_4);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_5);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_6);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_7);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_8);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_9);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_10);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_11);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_12);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_13);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_14);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_15);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_16);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_17);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_18);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_19);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_20);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_21);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_22);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_23);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_24);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_25);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_26);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_27);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_28);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_29);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_30);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_31);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_32);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_33);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_34);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_35);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_36);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_37);

    /* SiP IO */
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_SIP_39);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_SIP_40);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_SIP_41);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_SIP_42);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_SIP_43);
    Hal_Pin_Config(HAL_PIN_TYPE_PATCH_IO_SIP_44);
    
    Hal_Pin_UpdatePsramCfg();
}
/*************************************************************************
* FUNCTION:
*   Main_FlashLayoutUpdate
*
* DESCRIPTION:
*   update the flash layout
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_FlashLayoutUpdate(void)
{
    // update here
}

/*************************************************************************
* FUNCTION:
*   Main_MiscModulesInit
*
* DESCRIPTION:
*   the initial of driver part for cold and warm boot
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_MiscModulesInit(void)
{
    //Hal_Wdt_Stop();   //disable watchdog here.
}

/*************************************************************************
* FUNCTION:
*   Main_AtUartDbgUartSwitch
*
* DESCRIPTION:
*   switch the UART1 and dbg UART
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_AtUartDbgUartSwitch(void)
{
    if (g_eAppMainUartMode == PIN_MAIN_UART_MODE_AT)
    {
        Hal_Pin_Config(PIN_TYPE_APS_UART_TXD_IO0 | PIN_INMODE_IO0_FLOATING);
        Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_IO2 | PIN_INMODE_IO2_PULL_UP);

        Hal_Pin_Config(PIN_TYPE_UART1_TXD_IO5 | PIN_INMODE_IO5_FLOATING);
        Hal_Pin_Config(PIN_TYPE_UART1_RXD_IO1 | PIN_INMODE_IO1_PULL_UP);
    }
    else
    {
        Hal_Pin_Config(PIN_TYPE_UART1_TXD_IO0 | PIN_INMODE_IO0_FLOATING);
        Hal_Pin_Config(PIN_TYPE_UART1_RXD_IO2 | PIN_INMODE_IO2_PULL_UP);
        
        Hal_Pin_Config(PIN_TYPE_APS_UART_TXD_IO5 | PIN_INMODE_IO5_FLOATING);
        Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_IO1 | PIN_INMODE_IO1_PULL_UP);
    }
    
    g_eAppMainUartMode = (E_PIN_MAIN_UART_MODE)!g_eAppMainUartMode;
}

/*************************************************************************
* FUNCTION:
*   Main_AppInit_patch
*
* DESCRIPTION:
*   the initial of application
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
XIP_TEXT static void Main_AppInit_patch(void)
{
    osThreadDef_t tThreadDef;
    osMessageQDef_t tMessageDef;
    osPoolDef_t tMemPoolDef;
        
    // create the thread for AppThread_1
    tThreadDef.name = "App_1";
    tThreadDef.pthread = Main_AppThread_1;
    tThreadDef.tpriority = OS_TASK_PRIORITY_APP;        // osPriorityNormal
    tThreadDef.instances = 0;                           // reserved, it is no used
    tThreadDef.stacksize = OS_TASK_STACK_SIZE_APP;      // (512), unit: 4-byte, the size is 512*4 bytes
    g_tAppThread_1 = osThreadCreate(&tThreadDef, NULL);
    if (g_tAppThread_1 == NULL)
    {
        printf("To create the thread for AppThread_1 is fail.\n");
    }
    
    // create the thread for AppThread_2
    tThreadDef.name = "App_2";
    tThreadDef.pthread = Main_AppThread_2;
    tThreadDef.tpriority = OS_TASK_PRIORITY_APP;        // osPriorityNormal
    tThreadDef.instances = 0;                           // reserved, it is no used
    tThreadDef.stacksize = OS_TASK_STACK_SIZE_APP;      // (512), unit: 4-byte, the size is 512*4 bytes
    g_tAppThread_2 = osThreadCreate(&tThreadDef, NULL);
    if (g_tAppThread_2 == NULL)
    {
        printf("To create the thread for AppThread_2 is fail.\n");
    }
    
    // create the message queue for AppMessageQ
    tMessageDef.queue_sz = APP_MESSAGE_Q_SIZE;          // number of elements in the queue
    tMessageDef.item_sz = sizeof(S_MessageQ);           // size of an item
    tMessageDef.pool = NULL;                            // reserved, it is no used
    g_tAppMessageQ = osMessageCreate(&tMessageDef, g_tAppThread_2);
    if (g_tAppMessageQ == NULL)
    {
        printf("To create the message queue for AppMessageQ is fail.\n");
    }
    
    // create the memory pool for AppMessageQ
    tMemPoolDef.pool_sz = APP_MESSAGE_Q_SIZE;           // number of items (elements) in the pool
    tMemPoolDef.item_sz = sizeof(S_MessageQ);           // size of an item
    tMemPoolDef.pool = NULL;                            // reserved, it is no used
    g_tAppMemPoolId = osPoolCreate(&tMemPoolDef);
    if (g_tAppMemPoolId == NULL)
    {
        printf("To create the memory pool for AppMessageQ is fail.\n");
    }
}

/*************************************************************************
* FUNCTION:
*   Main_AppThread_1
*
* DESCRIPTION:
*   the application thread 1
*
* PARAMETERS
*   1. argu     : [In] the input argument
*
* RETURNS
*   none
*
*************************************************************************/
XIP_TEXT static void Main_AppThread_1(void *argu)
{
    S_MessageQ tMsg;
    uint32_t ulCount = 0;
    uint16_t u16Length;
    
    while (1)
    {
        osDelay(1000);      // delay 1000ms (1sec)
        
        // send the message into AppMessageQ
        u16Length = g_u16aDummyPattern[ulCount++ & (DEMO_XIP_RODATA_LEN-1)];
        tMsg.ulCount = u16Length;
        Main_AppMessageQSend(&tMsg);
    }
}

/*************************************************************************
* FUNCTION:
*   Main_AppThread_2
*
* DESCRIPTION:
*   the application thread 2
*
* PARAMETERS
*   1. argu     : [In] the input argument
*
* RETURNS
*   none
*
*************************************************************************/
XIP_TEXT static void Main_AppThread_2(void *argu)
{
    osEvent tEvent;
    S_MessageQ *ptMsgPool;
    void *paBuf[DEMO_PHEAP_ALLOC_WINDOW_SIZE]={0,};
    uint32_t count=0, id;
    
    while (1)
    {
        // receive the message from AppMessageQ
        tEvent = osMessageGet(g_tAppMessageQ, osWaitForever);
        if (tEvent.status != osEventMessage)
        {
            printf("To receive the message from AppMessageQ is fail.\n");
            continue;
        }
        
        // get the content of message
        ptMsgPool = (S_MessageQ *)tEvent.value.p;
        printf("XIP PSRAM running,");
        
        id = count++ & (DEMO_PHEAP_ALLOC_WINDOW_SIZE - 1);
        if (paBuf[id] != NULL)
        {
            printf("free %p, ", paBuf[id]);
            free(paBuf[id]);
            paBuf[id] = NULL;
        }
        // output the contect of message
        printf("receive data = %d\n", ptMsgPool->ulCount);
        #if EN_MALLOC_AUTO_PHEAP
        paBuf[id] = malloc(ptMsgPool->ulCount);
        #else /* EN_MALLOC_AUTO_PHEAP */
        paBuf[id] = mallocPHeap(ptMsgPool->ulCount);
        #endif /* EN_MALLOC_AUTO_PHEAP */
        printf("  Allocated buffer address: %p\n", paBuf[id]);
        
        // free the memory pool
        osPoolFree(g_tAppMemPoolId, ptMsgPool);
    }
}

/*************************************************************************
* FUNCTION:
*   Main_AppMessageQSend
*
* DESCRIPTION:
*   send the message into AppMessageQ
*
* PARAMETERS
*   1. ptMsg    : [In] the pointer of message content
*
* RETURNS
*   osOK        : successful
*   osErrorOS   : fail
*
*************************************************************************/
XIP_TEXT static osStatus Main_AppMessageQSend(S_MessageQ *ptMsg)
{
    osStatus tRet = osErrorOS;
    S_MessageQ *ptMsgPool;
    
    // allocate the memory pool
    ptMsgPool = (S_MessageQ *)osPoolCAlloc(g_tAppMemPoolId);
    if (ptMsgPool == NULL)
    {
        printf("To allocate the memory pool for AppMessageQ is fail.\n");
        goto done;
    }
    
    // copy the message content
    memcpy(ptMsgPool, ptMsg, sizeof(S_MessageQ));
    
    // send the message
    if (osOK != osMessagePut(g_tAppMessageQ, (uint32_t)ptMsgPool, osWaitForever))
    {
        printf("To send the message for AppMessageQ is fail.\n");
        
        // free the memory pool
        osPoolFree(g_tAppMemPoolId, ptMsgPool);
        goto done;
    }
    
    tRet = osOK;

done:
    return tRet;
}


