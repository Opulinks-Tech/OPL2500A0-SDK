/* *****************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 * @file sys_init_patch.c
 * 
 * @brief Patch for Sys init patch
 *  
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "sys_init.h"
#include "sys_cfg.h"
#include "boot_sequence.h"
#include "diag_task.h"
#include "hal_system.h"
#include "hal_sys_rcc.h"
#include "hal_uart.h"
#include "hal_qspi.h"
#include "hal_flash.h"
#include "hal_cache.h"
#include "hal_vic.h"
#include "hal_uart.h"
#include "hal_wdt.h"
#include "ps.h"
#include "ipc.h"
#include "irq.h"
#include "mw_ota.h"
#include "mw_fim.h"
#include "mw_fim_msq.h"
#include "lwip_jmptbl_patch.h"
#include "controller_wifi.h"
#include "le_ctrl_patch.h"
#include "at_cmd_common.h"
#include "at_cmd_func_patch.h"
#include "mw_fim_default_group01.h"
#include "mw_fim_default_group02_patch.h"
#include "wifi_mac_data.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define WDT_TIMEOUT_MSECS    10000
 

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
void LeHostPatchAssign(void);
void Sys_ClockSetup_patch(void);
void Sys_ServiceInit_patch(void);
void Sys_DriverInit_patch(void);
void Sys_UartInit_patch(void);
void Sys_XipInit(void);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
E_FUNC_ST g_eSys_XipEnable = DISABLE;
/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

__forceinline static void Sys_NotifyReadyToMsq(uint32_t indicator)
{
    Hal_Sys_SpareRegWriteBit(SPARE_0, indicator, indicator);
}

/**
 * @bried Init patch function when cold boot
 */
void SysInit_EntryPoint(void)
{
    // Only for cold boot
    if (Boot_CheckWarmBoot())
        return;
    
    Boot_PrepareM0PatchOK_patch();    /* Move here, skip origin function */
    Hal_Sys_SpareRegWait(SPARE_0, IPC_SPARE0_PATCH_SYNC_MASK, 1);   /* Wait MSQ run PatchEntry */
    
    // init bss section
    memset(ZI_REGION_START, 0, ZI_REGION_LENGTH);
    #if defined(GCC)
    memset(ZI_REGION_SHARERAM_START, 0, ZI_REGION_SHARERAM_LENGTH);
    memset(ZI_REGION_PART1RAM_START, 0, ZI_REGION_PART1RAM_LENGTH);
    #endif
    
    // 0. Tracer

    // 1. hal patch

    // 2. boot sequence

    // 3. sys init
    Sys_ClockSetup  = Sys_ClockSetup_patch;
    Sys_ServiceInit = Sys_ServiceInit_patch;
    Sys_DriverInit = Sys_DriverInit_patch;
    Sys_UartInit = Sys_UartInit_patch;
    
    // 4. IPC
    Ipc_PatchInit();

    // 5. Control task
    
    // 6. Wifi
    ctrl_wifi_init_patch();
    wifi_mac_rx_data_init_patch();
    wifi_mac_tx_data_init_patch();
    ieee80211_crypto_init_patch();
    
    // 7. le_ctrl
    le_ctrl_pre_patch_init();
    
    // 8. le_host
    LeHostPatchAssign();
    
    // 9. LwIP
    lwip_module_interface_init_patch();

    // 10. WPAS

    // 11. AT
    at_func_init_patch();
    
    // 12. SCRT

    // 13. HAL driver API
    Hal_Sys_RccPatchInit();
    Hal_Sys_PatchInit();
    Hal_Sys_PatchVerInit(IPC_SW_VER_INFO_START);    /* make sure if IPC address updated */
    Hal_Qspi_PatchInit();
    Hal_Cache_Pre_Init();
    Hal_FlashPatchInit();
    Hal_Vic_PatchInit();
    Hal_Uart_PatchInit();
    
    // 14. os

    // 15. util api

    // 16. Power Saving
    ps_patch_init();

    // 17. ISR
    Irq_PatchInit();

    // 18. DIAG
    Diag_PatchInit();
    
    // 19. FIM
    MwFim_PatchInit();
    MwFim_Group02_patch();

    // 20. AUXADC

    // 21. Temperature

    // 22. Main

    // 23. Agent
    
    // 24. OTA
    
    // 25. System Common
    
    // 26. SYS config
    sys_cfg_patch_init();

    // 27. User App Task
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */
void Sys_ClockSetup_patch(void)
{
    if (!Boot_CheckWarmBoot())
    {
        Hal_Sys_ApsClkTreeSetup(APS_CLK_SYS_SRC_XTAL, APS_CLK_SYS_DIV_1, APS_CLK_PCLK_DIV_1);
        Hal_Sys_StclkSetup(ENABLE, APS_CLK_ST_DIV_8);
        Hal_Sys_UnusedModuleDis();

        /* Switch peripheral clock to XTAL, because RC will be turned off later */
        Hal_Sys_ScrtSrcSelect(ASP_CLK_SCRT_SRC_XTAL, APS_CLK_SCRT_DIV_1);
        Hal_Sys_Spi1SrcSelect(APS_CLK_SPI1_SRC_XTAL, APS_CLK_SPI1_DIV_1);
        Hal_Sys_Spi2SrcSelect(APS_CLK_SPI2_SRC_XTAL, APS_CLK_SPI2_DIV_1);
        Hal_Sys_Spi3SrcSelect(APS_CLK_SPI3_SRC_XTAL, APS_CLK_SPI3_DIV_1);

    }
    else
    {
        Sys_WaitforMsqReady();
        Hal_Sys_WakeupClkResume();
        ps_psp_wkup_clk_ready();
        Sys_NotifyReadyToMsq(IPC_SPARE0_APS_WARMBOOT_CLK_READY_MSK);
    }
}

void Sys_ServiceInit_patch(void)
{
    Sys_ServiceInit_impl();
    Sys_XipInit();
}


void Sys_XipEnable(E_FUNC_ST eFunc)
{
    g_eSys_XipEnable = eFunc;
}

void Sys_XipInit(void)
{
    uint32_t u32Addr;
    
    if (g_eSys_XipEnable == DISABLE)
        return;
    
    if(Hal_Flash_Check(SPI_IDX_0) != SUPPORTED_FLASH)
        return;
    
    if (MwOta_BootAddrGet(&u32Addr) == MW_OTA_FAIL)
        u32Addr = 0;
    
    if (Hal_FlashQspiXipInit(SPI_SLAVE_0, u32Addr) == RESULT_SUCCESS)
    {   /* Enable Cache when XIP enabled */
        Hal_Cache_ClockEnable();
        Hal_Cache_Enable(1);
        Hal_Cache_PipelineEnable(0);
        Hal_Cache_PrefetchBypass(0);
    }
}

void Sys_DriverInit_patch(void)
{
    uint32_t u32ClkFreq;
    uint32_t u32Spare0 = 0;
    
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
        Hal_Flash_ReleasePowerDown(SPI_IDX_0, SPI_SLAVE_0, 0);

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
    
    if (!Boot_CheckWarmBoot())
    {
        // Wait for M0 initialization to be completed
        Sys_WaitforMsqReady();
    }
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

    Hal_Sys_SpareRegRead(SPARE_0, &u32Spare0);
    if (u32Spare0 & IPC_SPARE0_WARMBOOT_XTAL_NOT_RDY_MASK)
    {
        printf("XtalFail\n");
        msg_print_uart1("XtalFail\n");
        while (1);
        /* Wait WDT trigger */
    }
    // Other tasks' driver config
    // For APP use, put last in this function.
    Sys_MiscModulesInit();
}

void Sys_UartInit_patch(void)
{
    T_HalUartConfig tUartConfig;

    if (!Boot_CheckWarmBoot())
    {   /* Cold boot */
//        /* Init UART0 */
//        if (MW_FIM_OK != MwFim_FileRead(MW_FIM_IDX_GP01_UART_CFG, 0, MW_FIM_UART_CFG_SIZE, (uint8_t*)&tUartConfig))
//        {
//            // if fail, get the default value
//            memcpy(&tUartConfig, &g_ctHal_Uart_DefaultConfig, MW_FIM_UART_CFG_SIZE);
//        }

//        Hal_Uart_Init(UART_IDX_0,
//                      tUartConfig.ulBuadrate,
//                      (E_UartDataBit_t)(tUartConfig.ubDataBit),
//                      (E_UartParity_t)(tUartConfig.ubParity),
//                      (E_UartStopBit_t)(tUartConfig.ubStopBit),
//                      tUartConfig.ubFlowCtrl);
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

