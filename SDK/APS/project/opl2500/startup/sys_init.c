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
*  sys_init.c
*
*  Project:
*  --------
*  OPL2500 Project - the system initialize implement file
*
*  Description:
*  ------------
*  This implement file is include the system initialize function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "sys_init.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_dbg_uart.h"
#include "hal_uart.h"
#include "hal_spi.h"
#include "hal_qspi.h"
#include "hal_flash.h"
#include "hal_pwm.h"
#include "hal_auxadc.h"
#include "hal_wdt.h"
#include "mw_fim.h"
#include "at_cmd_common.h"
#include "ipc.h"
#include "msg.h"
#include "ps.h"
#include "boot_sequence.h"
#include "mw_fim_default_group01.h"
#include "mw_fim_msq.h"
#include "agent.h"
#include "mw_ota.h"
#include "network_config.h"
#include "sys_cfg.h"

#define __SVN_REVISION__
#define __DIAG_TASK__
//#define __AT_CMD_TASK__
//#define __WIFI_MAC_TASK__
//#define __LWIP_TASK__
#define __HW_CRYPTO_ENGINE__
//#define __WPA_SUPPLICANT__
#define __CONTROLLER_TASK__
//#define __BLE__

#if defined(__SVN_REVISION__)
#include "svn_rev.h"
#endif

#if defined(__DIAG_TASK__)
#include "diag_task.h"
#endif

#if defined(__AT_CMD_TASK__)
#include "at_cmd_task.h"
#endif

#if defined(__WIFI_MAC_TASK__)
#include "wifi_mac_task.h"
#endif

#if defined(__HW_CRYPTO_ENGINE__)
#include "scrt.h"
#endif

#if defined(__CONTROLLER_TASK__)
#include "controller_task.h"
#endif

#if defined(__BLE__)
#include "le_rtos_task.h"
#endif

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define WDT_TIMEOUT_MSECS    10000

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
T_Sys_PowerSetup_fp Sys_PowerSetup = Sys_PowerSetup_impl;
T_Sys_ClockSetup_fp Sys_ClockSetup = Sys_ClockSetup_impl;
T_Sys_UartInit_fp Sys_UartInit = Sys_UartInit_impl;
T_Sys_MiscModulesInit_fp Sys_MiscModulesInit = Sys_MiscModulesInit_impl;
T_Sys_DriverInit_fp Sys_DriverInit = Sys_DriverInit_impl;
T_Sys_WaitforMsqReady_fp Sys_WaitforMsqReady = Sys_WaitforMsqReady_impl;
T_Sys_ServiceInit_fp Sys_ServiceInit = Sys_ServiceInit_impl;
T_Sys_AppInit_fp Sys_AppInit  = Sys_AppInit_impl;
T_Sys_PostInit_fp Sys_PostInit = Sys_PostInit_impl;
T_Sys_StackOverflowHook_fp Sys_StackOverflowHook = Sys_StackOverflowHook_impl;
T_Sys_IdleHook_fp Sys_IdleHook = Sys_IdleHook_impl;


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype
__forceinline static void Sys_NotifyReadyToMsq(uint32_t indicator);


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   Sys_PowerSetup
*
* DESCRIPTION:
*   the initial for power driver
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_PowerSetup_impl(void)
{
    Hal_Sys_PowerDefaultSettings();
}

/*************************************************************************
* FUNCTION:
*   Sys_ClockSetup
*
* DESCRIPTION:
*   the initial for clock driver
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_ClockSetup_impl(void)
{
    if (!Boot_CheckWarmBoot())
    {
        Hal_Sys_UnusedModuleDis();

        /* Switch peripheral clock to XTAL, because RC will be turned off later */
        Hal_Sys_ScrtSrcSelect(ASP_CLK_SCRT_SRC_XTAL, APS_CLK_SCRT_DIV_1);
        Hal_Sys_Spi1SrcSelect(APS_CLK_SPI1_SRC_XTAL, APS_CLK_SPI1_DIV_1);
        Hal_Sys_Spi2SrcSelect(APS_CLK_SPI2_SRC_XTAL, APS_CLK_SPI2_DIV_1);
        Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_XTAL, APS_CLK_SPI3_DIV_1);

        Hal_Sys_StDivSelect(APS_CLK_ST_DIV_1);
    }
    else
    {
        Hal_Sys_WakeupClkResume();
        Sys_NotifyReadyToMsq(IPC_SPARE0_APS_WARMBOOT_CLK_READY_MSK);
    }
}

/*************************************************************************
* FUNCTION:
*   Sys_UartInit
*
* DESCRIPTION:
*   1. get the config of UART from FIM
*   2. init the UART modules
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_UartInit_impl(void)
{
    T_HalUartConfig tUartConfig;

    if (!Boot_CheckWarmBoot())
    {   /* Cold boot */
        /* Init UART0 */
        if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP01_UART_CFG, 0, MW_FIM_UART_CFG_SIZE, (uint8_t*)&tUartConfig))
        {
            // if fail, get the default value
            memcpy(&tUartConfig, &g_ctHal_Uart_DefaultConfig, MW_FIM_UART_CFG_SIZE);
        }

        Hal_Uart_Init(UART_IDX_0,
                      tUartConfig.ulBuadrate,
                      (E_UartDataBit_t)(tUartConfig.ubDataBit),
                      (E_UartParity_t)(tUartConfig.ubParity),
                      (E_UartStopBit_t)(tUartConfig.ubStopBit),
                      tUartConfig.ubFlowCtrl);
        /* Init UART1, AT UART */
        if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP01_UART_CFG, 1, MW_FIM_UART_CFG_SIZE, (uint8_t*)&tUartConfig))
        {
            // if fail, get the default value
            memcpy(&tUartConfig, &g_ctHal_Uart_DefaultConfig, MW_FIM_UART_CFG_SIZE);
        }
        Hal_Uart_Init(UART_IDX_1,
                      tUartConfig.ulBuadrate,
                      (E_UartDataBit_t)(tUartConfig.ubDataBit),
                      (E_UartParity_t)(tUartConfig.ubParity),
                      (E_UartStopBit_t)(tUartConfig.ubStopBit),
                      tUartConfig.ubFlowCtrl);

        // HCI and AT command
        uart1_mode_set_default();
    }
}

/*************************************************************************
* FUNCTION:
*   Sys_MiscModulesInit
*
* DESCRIPTION:
*   the initial for misc driver
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_MiscModulesInit_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

/*************************************************************************
* FUNCTION:
*   Sys_DriverInit
*
* DESCRIPTION:
*   the initial for driver
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_DriverInit_impl(void)
{
    uint32_t u32ClkFreq;
    
    sleep_cold_init();
    
    // Set power
    Sys_PowerSetup();

    // Set system clock
    Sys_ClockSetup();

    if (Boot_CheckWarmBoot())
    {   /* In cold boot, the SPI0 already setup at boot_sequence */
        // SPI_0 is QSPI
        Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI0, &u32ClkFreq);
        Hal_Qspi_Init(SPI_SLAVE_0, u32ClkFreq/4);

        ps_update_boot_gpio_int_status(Hal_Vic_GpioIntStatRead());
        Hal_Uart_WakeupResume(UART_IDX_DBG);
        Hal_Uart_WakeupResume(UART_IDX_0);
        Hal_Uart_WakeupResume(UART_IDX_1);
        Hal_Sys_WakeupResume();

        // power-saving module init
        ps_init();
    }
    else
    {
        // Init flash on SPI 0
        Hal_Flash_Init(SPI_IDX_0, SPI_SLAVE_0); // SPI_0 is for QSPI now
        //Hal_Flash_Opcode_Config(SPI_SLAVE_0, 1); /* Setup Quad mode */

        // FIM
        MwFim_Init(SPI_IDX_0, SPI_SLAVE_0);

        // power-saving module init
        ps_init();
        ipc_init(); /* IPC will call PS function, needs to init PS before IPC send message */

        /* Send MSQ tables to MSQ */
        MwFim_MsqFimLoadAll();
        /* [MSQ init start] ************************************************************* */

        // Set pin-mux
        Hal_SysPinMuxAppInit();

        // Init GPIO
        Hal_Gpio_VicInit();

        // Init DBG_UART
        Hal_Uart_Init(UART_IDX_DBG, 115200, DATA_BIT_8, PARITY_NONE, STOP_BIT_1, 0);
        Hal_Uart_RxCallBackFuncSet(UART_IDX_DBG, uartdbg_rx_int_handler);
        Hal_Uart_RxIntEn(UART_IDX_DBG, IRQ_PRIORITY_DBG_UART, ENABLE);

        // Init IPC
        Hal_Vic_IpcIntEn(IPC_IDX_0, IRQ_PRIORITY_IPC0, ENABLE);
        Hal_Vic_IpcIntEn(IPC_IDX_1, IRQ_PRIORITY_IPC1, ENABLE);
        Hal_Vic_IpcIntEn(IPC_IDX_2, IRQ_PRIORITY_IPC2, ENABLE);
        Hal_Vic_IpcIntEn(IPC_IDX_3, IRQ_PRIORITY_IPC3, ENABLE);
    }

    // Init PWM
    //Hal_Pwm_Init();

    // Init AUXADC
    //Hal_Aux_Init();

    //Watch Dog
    if (Hal_Sys_StrapModeRead() == STRAP_MODE_NORMAL)
    {
        Hal_Wdt_Init(WDT_TIMEOUT_MSECS);
    }

    // Wait for M0 initialization to be completed
    Sys_WaitforMsqReady();
    /* [MSQ init finished] *************************************************************
     * Avoid access flash when MSQ init. Because MSQ init will run RF calibration and cost higher current.
     */

    // Init UART0 / UART1
    Sys_UartInit(); /* Access FIM */


    // SPI_1 & 3 is SPI Master, SPI_2 is SPI Slave
    /* SPI1 (Master) */
    //Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI1, &u32ClkFreq);
    //Hal_Spi_Init(SPI_IDX_1, u32ClkFreq/4,
    //    SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1);
    /* SPI3 (Master) */
    //Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI3, &u32ClkFreq);
    //Hal_Spi_Init(SPI_IDX_3, u32ClkFreq/4,
    //    SPI_CLK_PLOAR_HIGH_ACT, SPI_CLK_PHASE_START, SPI_FMT_MOTOROLA, SPI_DFS_08_bit, 1);

    // Init flash on SPI 1/3
    //Hal_Flash_Init(SPI_IDX_1, SPI_SLAVE_0);
    //Hal_Flash_Init(SPI_IDX_3, SPI_SLAVE_0);


    // Other tasks' driver config
    // For APP use, put last in this function.
    Sys_MiscModulesInit();
}

/*************************************************************************
* FUNCTION:
*   Sys_WaitforMsqReady
*
* DESCRIPTION:
*   wait for M0 initialization to be completed
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_WaitforMsqReady_impl(void)
{
    Hal_Sys_SpareRegWait(SPARE_0, IPC_SPARE0_MSQ_INIT_DONE_MSK, 1);
}


/*************************************************************************
* FUNCTION:
*   Sys_ServiceInit
*
* DESCRIPTION:
*   the initial for service
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_ServiceInit_impl(void)
{
    T_MwOtaLayoutInfo tLayout;

    Diag_ShowRevision(NULL);

#if defined(__DIAG_TASK__)
    diag_task_create();
#endif

#if defined(__AT_CMD_TASK__)
    at_task_create();
#endif

#if defined(__WIFI_MAC_TASK__)
    wifi_mac_task_create();
#endif

#if defined(__HW_CRYPTO_ENGINE__)
    nl_scrt_Init();
#endif

#if defined(__CONTROLLER_TASK__)
	controller_task_create();
#endif


#if defined(__BLE__)
    LeRtosTaskCreat();
#endif

    // Agent
    agent_init();

    // Load param from FIM for Tracer
    tracer_load();

    // OTA
    tLayout.ulaHeaderAddr[0] = MW_OTA_HEADER_ADDR_1;
    tLayout.ulaHeaderAddr[1] = MW_OTA_HEADER_ADDR_2;
    tLayout.ulaImageAddr[0] = MW_OTA_IMAGE_ADDR_1;
    tLayout.ulaImageAddr[1] = MW_OTA_IMAGE_ADDR_2;
    tLayout.ulImageSize = MW_OTA_IMAGE_SIZE;
    MwOta_Init(&tLayout, 0);

    // DHCP ARP check
    tcpip_config_dhcp_arp_check_init();

    // SYS config
    sys_cfg_init();
}

/*************************************************************************
* FUNCTION:
*   Sys_AppInit
*
* DESCRIPTION:
*   the initial for application
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_AppInit_impl(void)
{
    __DSB();
    __ISB();
    __DSB();
    __ISB();
}

/*************************************************************************
* FUNCTION:
*   Sys_PostInit
*
* DESCRIPTION:
*   the post initial for sys init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_PostInit_impl(void)
{
    /* Patch Function */
    sys_cfg_rf_init(NULL);
    sys_cfg_clk_init();
    tracer_init();
}

/*************************************************************************
* FUNCTION:
*   Sys_StackOverflowHook
*
* DESCRIPTION:
*   the hook function of stack overflow
*
* PARAMETERS
*   1. xTask      : [In] the pointer of task handle
*   2. pcTaskName : [In] the task name
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_StackOverflowHook_impl(TaskHandle_t xTask, char *pcTaskName)
{
	tracer_drct_printf("stack overflow: %x %s\r\n", (unsigned int)xTask, (portCHAR *)pcTaskName);

    while(1) {}
}

/*************************************************************************
* FUNCTION:
*   Sys_IdleHook
*
* DESCRIPTION:
*   the hook function of idle task
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Sys_IdleHook_impl(void)
{
    if (Hal_Sys_StrapModeRead() == STRAP_MODE_NORMAL)
    {
        //Clear watchdog interrupt & reload conter
        Hal_Wdt_Clear();
    }

	ps_sleep();
}


__forceinline static void Sys_NotifyReadyToMsq(uint32_t indicator)
{
	Hal_Sys_SpareRegWriteBit(SPARE_0, indicator, indicator);
}
