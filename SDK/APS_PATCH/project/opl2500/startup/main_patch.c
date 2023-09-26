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
#include "opulinks.h"
#include <stdio.h>

#include "sys_init.h"
#include "hal_system.h"
#include "at_cmd_common.h"
#include "at_cmd_tcpip.h"
#include "at_cmd_wifi.h"
#include "at_cmd_app.h"
#include "at_cmd_property.h"
#include "at_cmd_msg_ext.h"
#include "at_cmd_task.h"
#include "freertos_cmsis.h"
#include "hal_pin.h"

#define BOARD_EVB                ( 1)
// #define BOARD_EVB_EXT_3_PINS     ( 2) /* Not support in SDK */
// #define BOARD_EVB_EXT_4_PINS     ( 3) /* Not support in SDK */
#define BOARD_2500S_MODULE       (12)
// #define BOARD_2500P_MODULE_V2    (22) /* Support in ext-pa example */
// #define BOARD_2500P_MODULE_V3    (23) /* Support in ext-pa example */

#ifdef OPL2500_FPGA
    #define BOARD_CURR               BOARD_EVB
#else
    #define BOARD_CURR               BOARD_2500S_MODULE /* <-- Config here */
#endif

#if( BOARD_CURR == BOARD_2500S_MODULE )
    #include "hal_pin_config_project_2500s.h"
#elif( BOARD_CURR == BOARD_EVB )
    #include "hal_pin_config_patch.h" /* For EVB or FPGA */
#else
    #error Not supported !!!
#endif



static void Main_HeapPatchInit(void);
static void Main_AppInit_patch(void);
static void Main_PinMuxUpdate(void);
static void at_cmd_switch_uart1_dbguart_patch(void);

void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT")));
void __Patch_EntryPoint(void)
{
    Sys_XipSetup(XIP_MODE_DISABLE, SPI_SLAVE_0, 0);
    
    /* Don't remove SysInit_EntryPoint */
    SysInit_EntryPoint();

    // update the pin mux
    Hal_SysPinMuxAppInit = Main_PinMuxUpdate;

    /* APS_HEAP_START and APS_HEAP_LENGTH are from scatter/linker file
     * When needs to change HEAP size, please modify scatter/linker file.
     * Do NOT write the argument here */
    osHeapAssign(APS_HEAP_START, APS_HEAP_LENGTH);
    Main_HeapPatchInit();

#if( BOARD_CURR >= BOARD_2500S_MODULE )
    // update the switch AT UART / dbg UART function
    at_cmd_switch_uart1_dbguart = at_cmd_switch_uart1_dbguart_patch;
#endif

    // apply the sw patch from here
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
