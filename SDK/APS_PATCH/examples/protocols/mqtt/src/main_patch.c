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
#include <stdint.h>
#include <string.h>
#include "sys_init.h"
#include "hal_system.h"
#include "mw_fim.h"
#include "cmsis_os.h"
#include "sys_os_config.h"
#include "hal_pin.h"
#include "hal_pin_def.h"
#include "hal_dbg_uart.h"
#include "hal_vic.h"
#include "boot_sequence.h"
#include "freertos_cmsis.h"
#include "at_cmd_common.h"

#ifdef __BLEWIFI_TRANSPARENT__
#include "at_cmd_app_patch.h"
#include "hal_pin_config_project_transparent.h"
#else
#include "hal_pin_config_project.h"
#endif

#include "blewifi_app.h"
#include "blewifi_configuration.h"
#include "mw_fim_default.h"
#include "mw_fim_default_group11_project.h"
#include "mw_fim_default_group12_project.h"

//#include "hal_wdt.h"
// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
static E_PIN_MAIN_UART_MODE g_eAppMainUartMode = PIN_MAIN_UART_MODE_AT;

// Sec 7: declaration of static function prototype
void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT"), used));
static void Main_PinMuxUpdate(void);
static void Main_MiscModulesInit(void);
static void Main_AppInit_patch(void);
static void Main_FlashLayoutUpdate(void);
static void Main_AtUartDbgUartSwitch(void);


#ifdef __BLEWIFI_TRANSPARENT__
static int Main_BleWifiInit(void);
#endif

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
#ifdef __BLEWIFI_TRANSPARENT__
    at_blewifi_init_adpt = Main_BleWifiInit;
#endif
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
    g_taMwFimZoneInfoTable[1].ulBaseAddr = 0x00090000;
    g_taMwFimZoneInfoTable[1].ulBlockNum = 9;

    MwFim_GroupInfoUpdate(1, 1, (T_MwFimFileInfo *)g_taMwFimGroupTable11_project);
    MwFim_GroupVersionUpdate(1, 1, MW_FIM_VER11_PROJECT);

    MwFim_GroupInfoUpdate(1, 2, (T_MwFimFileInfo *)g_taMwFimGroupTable12_project);
    MwFim_GroupVersionUpdate(1, 2, MW_FIM_VER12_PROJECT);
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
    // add the application initialization from here
    printf("AppInit\n");

#ifdef __BLEWIFI_TRANSPARENT__
    // the blewifi init will be triggered by AT Cmd
#else
      BleWifiAppInit();
#endif
}

/*************************************************************************
* FUNCTION:
*   Main_BleWifiInit
*
* DESCRIPTION:
*   the initial of application by AT Cmd
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
#ifdef __BLEWIFI_TRANSPARENT__
static int Main_BleWifiInit(void)
{
      BleWifiAppInit();
    
    return 0;
}
#endif
