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
#include <string.h>
#include "boot_sequence.h"
#include "sys_init.h"
#include "hal_system.h"
#include "mw_fim.h"
#include "mw_fim_default_group01.h"
#include "hal_pin.h"
#include "freertos_cmsis.h"
#include "at_cmd_task.h"
#include "at_cmd_common.h"
#include "at_cmd_tcpip.h"
#include "at_cmd_wifi.h"
#include "at_cmd_app.h"
#include "at_cmd_property.h"
#include "at_cmd_msg_ext.h"

// #define BOARD_EVB                ( 1) /* Not support in this example */
// #define BOARD_EVB_EXT_3_PINS     ( 2) /* Not support in this example */
// #define BOARD_EVB_EXT_4_PINS     ( 3) /* Not support in this example */
// #define BOARD_2500S_MODULE       (12) /* Not support in this example */
#define BOARD_2500P_MODULE_V2    (22)
#define BOARD_2500P_MODULE_V3    (23)

#define BOARD_CURR               BOARD_2500P_MODULE_V3 /* <-- Config here */

#if( BOARD_CURR == BOARD_2500P_MODULE_V3 )
    #include "hal_pin_config_project_2500p_v3.h"
#elif( BOARD_CURR == BOARD_2500P_MODULE_V2 )
    #include "hal_pin_config_project_2500p_v2.h"
#else
    #error Not supported !!!
#endif

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list
// the content of message queue


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable

// Sec 7: declaration of static function prototype
void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT")));
void __Patch_EntryPoint(void) __attribute__((used));
static void Main_HeapPatchInit(void);
static void Main_PinMuxUpdate(void);
static void Main_MiscModulesInit(void);
static void Main_FlashLayoutUpdate(void);
static void Main_AppInit_patch(void);
static void at_cmd_switch_uart1_dbguart_patch(void);

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

    // update driver-level config
    Sys_MiscModulesInit = Main_MiscModulesInit;

    // update the switch AT UART / dbg UART function
    at_cmd_switch_uart1_dbguart = at_cmd_switch_uart1_dbguart_patch;

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

    at_io01_uart_mode_set(HAL_PIN_MAIN_UART_MODE_PATCH);
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
    /*
     * Two steps to active ext-Pa mode:
     *
     * Step 1) Config " hal_pin_config_project_XXX.h "
     *         1-1) Assigned three pins according to schematic:
     *                  [ TX_EN ]
     *                  [ RX_EN ]
     *                  [ LNA_EN ]
     *              The three pins are MUST assinged to PIN_TYPE_GPIO_OUT_LOW.
     *         1-2) (Optional) ("0xFF" for 3-pins control case)
                    Assigned the pin : PwrCtrl according to schematic.
     *                  [ Pwr Ctrl ]
     *              The pin is MUST assinged to PIN_TYPE_GPIO_OUT_LOW.
     *
     * Step 2) Set Hal_ExtPa_Pin_Set(), default value was disable.
     *
     */
    if (!Boot_CheckWarmBoot())
    {
        #if( BOARD_CURR == BOARD_2500P_MODULE_V3 )
            Hal_ExtPa_Pin_Set( 4, 6, 3, 5);
        #elif( BOARD_CURR == BOARD_2500P_MODULE_V2 )
            Hal_ExtPa_Pin_Set( 4, 6, 18, 0xFF);
        #endif

        // Force Wifi pwr to ext-PA level
        uint8_t u8Temp = 0;
        MwFim_FileRead(MW_FIM_IDX_GP01_RF_CFG, 0, MW_FIM_RF_CFG_SIZE, &u8Temp);
        if(u8Temp < 0xE0)
        {
            u8Temp = 0xE0;
            MwFim_FileWrite(MW_FIM_IDX_GP01_RF_CFG, 0, MW_FIM_RF_CFG_SIZE, &u8Temp);
        }
    }

    //Hal_Wdt_Stop();   //disable watchdog here.
}

/*************************************************************************
* FUNCTION:
*   at_cmd_switch_uart1_dbguart_patch
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
extern E_PIN_MAIN_UART_MODE g_eAt_MainUartMode;
static void at_cmd_switch_uart1_dbguart_patch(void)
{
    osSemaphoreWait(g_tSwitchuartSem, osWaitForever);

    /* Set UART RX to high first in order not to receive wrong data */
    Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_HIGH);
    Hal_Pin_Config(PIN_TYPE_UART1_RXD_HIGH);

    if(g_eAt_MainUartMode == PIN_MAIN_UART_MODE_AT)
    {   /* Switch 0, 2 to debug UART */
        Hal_Pin_Config(PIN_TYPE_APS_UART_TXD_IO0);
        Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_IO2|PIN_INMODE_IO2_PULL_UP);

        Hal_Pin_Config(PIN_TYPE_UART1_TXD_IO22);
        Hal_Pin_Config(PIN_TYPE_UART1_RXD_IO1|PIN_INMODE_IO1_PULL_UP);
    }
    else
    {   /* Switch 0, 2 to AT UART */
        Hal_Pin_Config(PIN_TYPE_APS_UART_TXD_IO22);
        Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_IO1|PIN_INMODE_IO1_PULL_UP);

        Hal_Pin_Config(PIN_TYPE_UART1_TXD_IO0);
        Hal_Pin_Config(PIN_TYPE_UART1_RXD_IO2|PIN_INMODE_IO2_PULL_UP);
    }

    g_eAt_MainUartMode = (E_PIN_MAIN_UART_MODE)!g_eAt_MainUartMode;
    osSemaphoreRelease(g_tSwitchuartSem);
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
    at_cmd_wifi_func_init();
    at_cmd_app_func_preinit();
    at_cmd_property_func_init();
    at_msg_ext_init();

    // add the application initialization from here
    printf("AppInit\n");
}
