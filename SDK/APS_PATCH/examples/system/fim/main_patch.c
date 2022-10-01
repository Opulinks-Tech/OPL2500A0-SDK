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
#include <stdlib.h>
#include <stdint.h>
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

#include "mw_fim_default_group07_extension.h"
#include "mw_fim_default_group11_extension.h"
#include "mw_fim_default_group12_extension.h"

//#include "hal_wdt.h"
// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the number of elements in the message queue
#define APP_MESSAGE_Q_SIZE  16

/*
    Please refer following hyper link to under memory allocation.
    https://github.com/Opulinks-Tech/OPL1000-SDK/blob/master/Doc/zh_CN/OPL1000-Flash-User-Guide.pdf
    Chapter 3 Flash Layout
*/
#define MW_FIM_ZONE1_BASE_ADDR_Ex  0x00090000 /* Physical Memory allocation */
#define MW_FIM_ZONE1_BLOCK_SIZE_Ex 0x1000
#define MW_FIM_ZONE1_BLOCK_NUM_Ex  3          /* 2 Group BLOCK + SWAP BLOCK = 3  BLOCK */

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, unio, enum, linked list
// the content of message queue
typedef struct
{
    uint32_t ulCount;
} S_MessageQ;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
static osThreadId g_tAppThread_1;
static osThreadId g_tAppThread_2;
static osMessageQId g_tAppMessageQ;
static E_PIN_MAIN_UART_MODE g_eAppMainUartMode = PIN_MAIN_UART_MODE_AT;

// Sec 7: declaration of static function prototype
void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT"), used));
static void Main_PinMuxUpdate(void);
static void Main_MiscModulesInit(void);
static void Main_AppInit_patch(void);
static void Main_FlashLayoutUpdate(void);
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
    // don't remove this code
    SysInit_EntryPoint();

#ifdef SWITCH_TO_32K_RC
    // Uncomment this function when the device is without 32k XTAL.
    Sys_SwitchTo32kRC();
#endif 
    // update the pin mux
    Hal_SysPinMuxAppInit = Main_PinMuxUpdate;
    
    // update the flash layout
    MwFim_FlashLayoutUpdate = Main_FlashLayoutUpdate;
    
    osHeapAssign(APS_HEAP_START, APS_HEAP_LENGTH);
    //Sys_SetUnsuedSramEndBound(0x43E000);
    
    Sys_MiscModulesInit = Main_MiscModulesInit;
    
    // update the switch AT UART / dbg UART function
    at_cmd_switch_uart1_dbguart = Main_AtUartDbgUartSwitch;

    // application init
    Sys_AppInit = Main_AppInit_patch;
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
static void Main_PinMuxUpdate(void)
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
    T_MwFimZoneInfo taMwFimZoneInfoTable_Extension =
    {
        MW_FIM_ZONE1_BASE_ADDR_Ex,
        MW_FIM_ZONE1_BLOCK_SIZE_Ex,
        MW_FIM_ZONE1_BLOCK_NUM_Ex,
        g_ubaMwFimVersionTable[1]
    };

    // add the new zone 01
    MwFim_ZoneInfoUpdate(1, &taMwFimZoneInfoTable_Extension);
    
    // update the group information
    MwFim_GroupInfoUpdate(0, 7, (T_MwFimFileInfo *)g_taMwFimGroupTable07_Extension);
    MwFim_GroupVersionUpdate(0, 7, MW_FIM_VER07_Extension);
    MwFim_GroupInfoUpdate(1, 1, (T_MwFimFileInfo *)g_taMwFimGroupTable11_Extension);
    MwFim_GroupVersionUpdate(1, 1, MW_FIM_VER11_Extension);
    MwFim_GroupInfoUpdate(1, 2, (T_MwFimFileInfo *)g_taMwFimGroupTable12_Extension);
    MwFim_GroupVersionUpdate(1, 2, MW_FIM_VER12_Extension);
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
static void Main_AppInit_patch(void)
{
    osThreadDef_t tThreadDef;
    osMessageQDef_t tMessageDef;
    
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
static void Main_AppThread_1(void *argu)
{
    S_MessageQ tMsg;
    uint32_t ulCount = 0;
    
    while (1)
    {
        osDelay(1000);      // delay 1000ms (1sec)
        
        // send the message into AppMessageQ
        ulCount++;
        tMsg.ulCount = ulCount;
        Main_AppMessageQSend(&tMsg);
    }
}

/*
    This is a example which the default value doesn't initialize.
*/
void Zone_00_Group_07_Sample_Mode(S_MessageQ *ptMsgPool)
{
    sample_mode_t sample;

    /* Group 6 Sample Mode */
    if (MW_FIM_OK != MwFim_FileRead(MW_FIM_GP07_SAMPLE_MODE, 0, MW_FIM_SAMPLE_MODE_SIZE, (uint8_t *)&sample))
    {
            // if fail, give the default value
            printf("\r\nGroup 07 Read Mode Value ERROR\r\n");
            sample.mode = 0;
    } 
    else
    {
            printf("\r\nGroup 07 Read Mode = %d from FIM\r\n",sample.mode);
    }
    
    sample.mode = ptMsgPool->ulCount;
    if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_GP07_SAMPLE_MODE, 0, MW_FIM_SAMPLE_MODE_SIZE, (uint8_t *)&sample))
    {
            printf("\r\nGroup 07 Write Mode Value ERROR\r\n");
    } 
    else
    {
            printf("\r\nGroup 07 Write Mode = %d into FIM\r\n",sample.mode);
    }
}

/*
    This is a example which the default value initialize.
*/
void Zone_00_Group_07_Sample_Data_Index_0(S_MessageQ *ptMsgPool)
{
    sample_data_t sample;

    /* Group 6 Sample Data */
    if (MW_FIM_OK != MwFim_FileRead(MW_FIM_GP07_SAMPLE_DATA, 0, MW_FIM_SAMPLE_DATA_CFG_SIZE, (uint8_t *)&sample))
    {
            // if fail, give the default value
            printf("\r\nGroup 07 Read Number Value ERROR\r\n");
            memcpy(&sample, &g_MwFimDefaultSampleCfg, MW_FIM_SAMPLE_DATA_CFG_SIZE);
    } 
    else
    {
            printf("\r\nGroup 07 Read Number = %d from FIM\r\n",sample.number);
    }
    
    sample.number = ptMsgPool->ulCount;
    if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_GP07_SAMPLE_DATA, 0, MW_FIM_SAMPLE_DATA_CFG_SIZE, (uint8_t *)&sample))
    {
            printf("\r\nGroup 07 Write Number Value ERROR\r\n");
    } 
    else
    {
            printf("\r\nGroup 07 Write Number = %d into FIM\r\n",sample.number);
    }
}

/*
    This is a example which read/write on the different index.
*/
void Zone_00_Group_07_Sample_Data_Index_1(S_MessageQ *ptMsgPool)
{
    sample_data_t sample;

    /* Group 6 Sample Data */
    if (MW_FIM_OK != MwFim_FileRead(MW_FIM_GP07_SAMPLE_DATA, 1, MW_FIM_SAMPLE_DATA_CFG_SIZE, (uint8_t *)&sample))
    {
            // if fail, give the default value
            printf("\r\nGroup 07 Read Number Value ERROR\r\n");
            memcpy(&sample, &g_MwFimDefaultSampleCfg, MW_FIM_SAMPLE_DATA_CFG_SIZE);
    } 
    else
    {
            printf("\r\nGroup 07 Read Number = %d from FIM\r\n",sample.number);
    }
    
    sample.number = ptMsgPool->ulCount;
    if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_GP07_SAMPLE_DATA, 1, MW_FIM_SAMPLE_DATA_CFG_SIZE, (uint8_t *)&sample))
    {
            printf("\r\nGroup 07 Write Number Value ERROR\r\n");
    } 
    else
    {
            printf("\r\nGroup 07 Write Number = %d into FIM\r\n",sample.number);
    }
}

/* 
    This is a example which create on the different Group and Zone.
*/
void Zone_01_Group_02_Sensor_Data_Index_6(S_MessageQ *ptMsgPool)
{
    sensor_data_t tmp;

    /* Group 12 Sensor Data */
    if (MW_FIM_OK != MwFim_FileRead(MW_FIM_GP12_SENSOR_DATA, 6, MW_FIM_SENSOR_DATA_CFG_SIZE, (uint8_t *)&tmp))
    {
            // if fail, give the default value
            printf("\r\nGroup 12 Read Number Value ERROR\r\n");
            memcpy(&tmp, &g_MwFimDefaultSensorDataNumber, MW_FIM_SENSOR_DATA_CFG_SIZE);
    } 
    else
    {
            printf("\r\nGroup 12 Read Number = %d from FIM\r\n",tmp.number);
    }

    tmp.number = ptMsgPool->ulCount;

    if (MW_FIM_OK != MwFim_FileWrite(MW_FIM_GP12_SENSOR_DATA, 6, MW_FIM_SENSOR_DATA_CFG_SIZE, (uint8_t *)&tmp))
    {
            printf("\r\nGroup 12 Write Number Value ERROR\r\n");
    } 
    else
    {
            printf("\r\nGroup 12 Writ Number = %d into FIM\r\n",tmp.number);
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
static void Main_AppThread_2(void *argu)
{
    osEvent tEvent;
    S_MessageQ *ptMsgPool;

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
        
        Zone_00_Group_07_Sample_Mode(ptMsgPool);
        Zone_00_Group_07_Sample_Data_Index_0(ptMsgPool); 
        Zone_00_Group_07_Sample_Data_Index_1(ptMsgPool);
        Zone_01_Group_02_Sensor_Data_Index_6(ptMsgPool);

        free(ptMsgPool);
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
static osStatus Main_AppMessageQSend(S_MessageQ *ptMsg)
{
    osStatus tRet = osErrorOS;
    S_MessageQ *ptMsgPool;
    
    ptMsgPool = (S_MessageQ *)malloc(1);

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
        
        free(ptMsgPool);
        goto done;
    }
    
    tRet = osOK;

done:
    return tRet;
}
