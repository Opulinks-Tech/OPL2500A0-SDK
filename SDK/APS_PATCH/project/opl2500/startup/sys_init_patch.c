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
#include "hal_flash_internal.h"
#include "hal_psram.h"
#include "hal_cache.h"
#include "hal_vic.h"
#include "hal_uart.h"
#include "hal_i2c.h"
#include "hal_wdt.h"
#include "hal_auxadc.h"
#include "hal_pin.h"
#include "hal_gpio.h"
#include "hal_patch.h"
#include "ps.h"
#include "ipc.h"
#include "irq.h"
#include "mw_ota.h"
#include "mw_ota_boot.h"
#include "mw_ota_def.h"
#include "mw_fim.h"
#include "mw_fim_msq.h"
#include "lwip_jmptbl_patch.h"
#include "controller_task.h"
#include "controller_wifi.h"
#include "le_ctrl_patch.h"
#include "at_cmd_task.h"
#include "at_cmd_common.h"
#include "at_cmd_func_patch.h"
#include "mw_fim_default_group01_patch.h"
#include "mw_fim_default_group02_patch.h"
#include "wifi_mac_task.h"
#include "wifi_mac_data.h"
#include "freertos_cmsis.h"
#include "scrt.h"
#include "agent.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define WDT_TIMEOUT_MSECS    10000

uint32_t g_u32BcnLostAddr, g_BcnLostCnt, g_BcLostShowEn = 0;
uint32_t g_u32BcnRcvAddr;
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
typedef void (*tcpip_config_dhcp_arp_check_init_fp_t) (void);
extern tcpip_config_dhcp_arp_check_init_fp_t tcpip_config_dhcp_arp_check_init;
void LeRtosTaskCreat(void);
void LeHostPatchAssign(void);
void Sys_ClockSetup_patch(void);
void Sys_ServiceInit_patch(void);
void Sys_DriverInit_patch(void);
void Sys_UartInit_patch(void);
static void Sys_XipInit(void);
void Sys_OtaPatchInit(void);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
typedef struct
{
    E_XIP_MODE eXipMode;
    E_SpiSlave_t eSpiSlaveIndex;
    uint32_t u32XipFlashAddress;
} S_XIP_CFG;

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
S_XIP_CFG g_sSys_XipCfg = {
    .eXipMode = XIP_MODE_DISABLE,
    .eSpiSlaveIndex = SPI_SLAVE_0,
    .u32XipFlashAddress = 0,
};
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
    Sys_XipInit();

    /* HW patch for warm boot and cold boot */
    Boot_HwPatchInit();
    osHwPatch();


    /*
     *********************************
     * Only for cold boot            *
     *********************************
     */
    if (Boot_CheckWarmBoot())
        return;

    if ((uint32_t)Hal_Cache_ClockEnable == 0)
    {   /* Not initialized yet */
        /* patch function init */
        Hal_Qspi_PatchInit();
        Hal_Cache_Pre_Init();
        Hal_Flash_PatchInit();
    }

    Hal_Sys_Xtal32DetectStart();
    Boot_PrepareM0PatchOK_patch();    /* Move here, skip origin function */
    Hal_Sys_SpareRegWait(SPARE_0, IPC_SPARE0_PATCH_SYNC_MASK, 1);   /* Wait MSQ run PatchEntry */

    // init bss section
    memset(ZI_REGION_START, 0, ZI_REGION_LENGTH);
    memset(ZI_REGION_SHARERAM_START, 0, ZI_REGION_SHARERAM_LENGTH);
    memset(ZI_REGION_PART1RAM_START, 0, ZI_REGION_PART1RAM_LENGTH);


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
    Hal_Vic_PatchInit();
    Hal_Uart_PatchInit();
    Hal_Auxadc_PatchInit();
    Hal_Gpio_PatchInit();
    Hal_I2c_PatchInit();

    // 14. os
    osPatchInit();

    // 15. util api

    // 16. Power Saving
    ps_patch_init();

    // 17. ISR
    Irq_PatchInit();

    // 18. DIAG
    Diag_PatchInit();

    // 19. FIM
    MwFim_MsqPatchInit();
    MwFim_PatchInit();
    MwFim_Group01_patch();
    MwFim_Group02_patch();

    // 20. AUXADC

    // 21. Temperature

    // 22. Main

    // 23. Agent
    Agent_PatchInit();

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

    Diag_ShowRevision(NULL);

    diag_task_create();

    at_task_create();

    wifi_mac_task_create();

    nl_scrt_Init();

    controller_task_create();

    LeRtosTaskCreat();

    // Agent
    agent_init();

    // Load param from FIM for Tracer
    tracer_load();

    Sys_OtaPatchInit();

    // DHCP ARP check
    tcpip_config_dhcp_arp_check_init();

    // SYS config
    sys_cfg_init();

    Hal_Sys_Xtal32CalcEnd();
}

/**
 * @brief XIP setup
 *        Please call this function at __Patch_EntryPoint and before SysInit_EntryPoint
 *
 * @warning -------------------------------------------------------------------------------------------------
 *          | XIP limitation:                                                                               |
 *          | 1. When flash erasing and polling busy, the XIP will be delayed until flash process finished. |
 *          |    Including FLASH API and FIM API                                                            |
 *          | 2. QSPI DMA is not available                                                                  |
 *          |    Hal_Qspi_Dma_Access is invalid                                                             |
 *          -------------------------------------------------------------------------------------------------
 *
 * @param eXipMode [in] To setup the XIP mode.
 *                      XIP_MODE_DISABLE: There is no XIP content in flash.
 *                      XIP_MODE_OTA_BUNDLE: The XIP content is within OTA image. Slave must be CS0.
 *                      XIP_MODE_STAND_ALONE: The XIP content is NOT within OTA image. Slave must be CS0.
 *                      XIP_MODE_EXT_FLASH: The full external flash are XIP content. Slave must be CS1~CS3.
 *                      XIP_MODE_EXT_PSRAM: The full external PSRAM are XIP content, and support write to PSRAM. Slave must be CS1 ~ CS3.
 * @param eSlvIdx [in] Assign XIP mode slave index. Default is CS0.
 *                     SPI_SLAVE_0 for SPI0_CS0. For XIP_MODE_OTA_BUNDLE and XIP_MODE_STAND_ALONE mode.
 *                     SPI_SLAVE_1 for SPI0_CS1. For XIP_MODE_EXT_FLASH and XIP_MODE_EXT_FLASH mode.
 *                     SPI_SLAVE_2 for SPI0_CS2. For XIP_MODE_EXT_FLASH and XIP_MODE_EXT_FLASH mode.
 *                     SPI_SLAVE_3 for SPI0_CS3. For XIP_MODE_EXT_FLASH and XIP_MODE_EXT_FLASH mode.
 *                     Ingored when XIP mode is XIP_MODE_DISABLE.
 * @param u32FlashAddress [in] Only valid when XIP_MODE_STAND_ALONE mode.
 *                             It is ignored in other modes.
 * @return Setup status
 * @retval RESULT_SUCCESS: configuration is valid
 * @retval RESULT_FAIL: configuration is invalid
 */
E_RESULT_COMMON Sys_XipSetup(E_XIP_MODE eXipMode, E_SpiSlave_t eSlvIdx, uint32_t u32FlashAddress)
{
    uint32_t u32Addr = 0;
    if (Boot_CheckWarmBoot())
        goto done;

    if (eSlvIdx >= SPI_SLAVE_MAX)
        goto error;

    switch (eXipMode)
    {
        case XIP_MODE_DISABLE:
            g_sSys_XipCfg.eXipMode = XIP_MODE_DISABLE;
            Hal_Sys_ApsModuleClkEn(DISABLE, APS_CLK_XIP_CACHE);
            Hal_Sys_ApsModuleClkEn(DISABLE, APS_CLK_XIP_PREFETCH);
            goto done;
        case XIP_MODE_OTA_BUNDLE:
            if (eSlvIdx != SPI_SLAVE_0)
                goto error;
            break;
        case XIP_MODE_STAND_ALONE:
            u32Addr = u32FlashAddress;
            if (eSlvIdx != SPI_SLAVE_0)
                goto error;
            break;
        case XIP_MODE_EXT_FLASH:
            if (eSlvIdx == SPI_SLAVE_0)
                goto error;
            break;
        case XIP_MODE_EXT_PSRAM:
            if (eSlvIdx == SPI_SLAVE_0)
                goto error;
            break;
        default:
            goto error;
    }

    g_sSys_XipCfg.eXipMode = eXipMode;
    g_sSys_XipCfg.eSpiSlaveIndex = eSlvIdx;
    g_sSys_XipCfg.u32XipFlashAddress = u32Addr;

done:

    return RESULT_SUCCESS;

error:
    g_sSys_XipCfg.eXipMode = XIP_MODE_DISABLE;
    return RESULT_FAIL;
}

static void Sys_XipInit(void)
{
    uint32_t Spi0Clk = 0;
    S_MW_OTA_BOOT_STS *psOtaBootSts = (S_MW_OTA_BOOT_STS *)SYSTEM_SPARE_REG_OTA_STATUS;

    if (!Boot_CheckWarmBoot())
    {
        /* patch function init */
        Hal_Qspi_PatchInit();
        Hal_Cache_Pre_Init();
        Hal_Flash_PatchInit();
    }
    if (g_sSys_XipCfg.eXipMode == XIP_MODE_DISABLE)
    {
        Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI0, &Spi0Clk);
        Hal_Qspi_Init(SPI_SLAVE_0, Spi0Clk/4);
        Hal_Flash_ReleasePowerDown(SPI_IDX_0, SPI_SLAVE_0, 0);
        Hal_Flash_Init_Internal(SPI_IDX_0, SPI_SLAVE_0);
        Hal_Flash_InternalFlashWpSetup();
        Hal_Cache_Enable(0);
        return;
    }

    Hal_Sys_Spi0SrcSelect(APS_CLK_SPI0_SRC_XTAL, APS_CLK_SPI0_DIV_1);
    Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI0, &Spi0Clk);
    Hal_Qspi_Init(g_sSys_XipCfg.eSpiSlaveIndex, Spi0Clk/4);

    switch (g_sSys_XipCfg.eXipMode)
    {
        case XIP_MODE_OTA_BUNDLE:
            Hal_Flash_ReleasePowerDown(SPI_IDX_0, g_sSys_XipCfg.eSpiSlaveIndex, 0);
            Hal_Flash_Init_Internal(SPI_IDX_0, g_sSys_XipCfg.eSpiSlaveIndex);
            if (!Boot_CheckWarmBoot())
            {
                uint32_t u32Addr, OtaImgAddr;
                OtaImgAddr = (psOtaBootSts->Signature == MW_OTA_BOOT_STATUS_SIGNATURE)?psOtaBootSts->CurrOtaImagAddr:0;
                if (Hal_Patch_ScanXipPartition(SPI_SLAVE_0, OtaImgAddr, &u32Addr))
                {
                    g_sSys_XipCfg.u32XipFlashAddress = u32Addr;
                }
            }
            Hal_QSpi_UpdateRemap(g_sSys_XipCfg.u32XipFlashAddress);
            break;
        case XIP_MODE_STAND_ALONE:
            Hal_Flash_ReleasePowerDown(SPI_IDX_0, g_sSys_XipCfg.eSpiSlaveIndex, 0);
            Hal_Flash_Init_Internal(SPI_IDX_0, g_sSys_XipCfg.eSpiSlaveIndex);
            Hal_QSpi_UpdateRemap(g_sSys_XipCfg.u32XipFlashAddress);
            break;
        case XIP_MODE_EXT_FLASH:
            Hal_Flash_ReleasePowerDown(SPI_IDX_0, g_sSys_XipCfg.eSpiSlaveIndex, 0);
            if (Hal_Flash_Init_Internal(SPI_IDX_0, g_sSys_XipCfg.eSpiSlaveIndex) == 0)
            {
                uint32_t u32OtaExtImgAddr = 0;
                if (psOtaBootSts->Signature == MW_OTA_BOOT_STATUS_SIGNATURE)
                {
                    if (psOtaBootSts->OtaExtImgEnable)
                        u32OtaExtImgAddr = psOtaBootSts->CurrOtaExtImgAddr;
                    else
                        tracer_drct_printf("Init XIP flash, Boot Agent not support external flash\n");
                }
                else
                {   /* No Boot agent */
                    u32OtaExtImgAddr = MW_OTA_EXT_IMAGE_ADDR_PATCH_START;
                }

                if (!Boot_CheckWarmBoot())
                {
                    uint32_t u32Addr;
                    if (Hal_Patch_ScanXipPartition(g_sSys_XipCfg.eSpiSlaveIndex, u32OtaExtImgAddr, &u32Addr))
                    {
                        g_sSys_XipCfg.u32XipFlashAddress = u32Addr;
                    }
                }
                Hal_QSpi_UpdateRemap(g_sSys_XipCfg.u32XipFlashAddress);
            }
            else
            {
                tracer_drct_printf("Init XIP on external flash fail\n");
            }
            break;
        case XIP_MODE_EXT_PSRAM:
            if (Hal_Psram_Init(SPI_IDX_0, g_sSys_XipCfg.eSpiSlaveIndex, 0, 1) == 0)
            {
                uint32_t u32OtaImgAddr = 0;
                if (psOtaBootSts->Signature == MW_OTA_BOOT_STATUS_SIGNATURE)
                {
                    u32OtaImgAddr = psOtaBootSts->CurrOtaImagAddr;
                }
                if (!Boot_CheckWarmBoot())
                {
                    uint32_t u32Addr, u32Size;
                    u32Size = Hal_Patch_ScanXipPartition(g_sSys_XipCfg.eSpiSlaveIndex, u32OtaImgAddr, &u32Addr);
                    if (u32Size)
                    {
                        Hal_Patch_CopyXipContent(u32Addr, APS_XIP_MEM_RW_BASE, u32Size);
                    }
                }
            }
            else
            {
                tracer_drct_printf("Init XIP PSRAM fail\n");
            }
            break;
        default: /* Invalid */
            break;
    }
    /* Enable Cache when XIP enabled */
    Hal_Cache_ClockEnable();
    Hal_Cache_Enable(1);
    Hal_Cache_PipelineEnable(0);
    Hal_Cache_PrefetchBypass(1);
}

void Sys_DriverInit_patch(void)
{
    uint32_t u32Spare0 = 0;

    sleep_cold_init();

    // Set power
    Sys_PowerSetup();

    // Set system clock
    Sys_ClockSetup();

    if (Boot_CheckWarmBoot())
    {
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
        g_u32BcnLostAddr = SYSTEM_SPARE_REG_BCN_DBG1;
        g_u32BcnRcvAddr  = SYSTEM_SPARE_REG_BCN_DBG2;
        Hal_Sys_Xtal32DetectEnd();
        // Init flash on SPI 0
        //Hal_Flash_Init(SPI_IDX_0, SPI_SLAVE_0); // SPI_0 is for QSPI now
        //Hal_Flash_Opcode_Config(SPI_SLAVE_0, 1); /* Setup Quad mode */
        Hal_Flash_InitSemaphore(SPI_IDX_0);

        // FIM
        MwFim_Init(SPI_IDX_0, SPI_SLAVE_0);

        // power-saving module init
        ps_init();
        ipc_init(); /* IPC will call PS function, needs to init PS before IPC send message */

        /* Send MSQ tables to MSQ */
        MwFim_MsqFimLoadAll();
        /* [MSQ init start] ************************************************************* */

        Hal_Sys_Xtal32CalcStart();
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
        printf("\n\n\nXtalFail\n\n\n");
        msg_print_uart1("\n\n\nXtalFail\n\n\n");
    }


    if (Boot_CheckWarmBoot() && g_BcLostShowEn)
    {
        uint32_t Cnt = reg_read(g_u32BcnLostAddr);
        uint32_t Rcv = reg_read(g_u32BcnRcvAddr);
        /*if (Cnt > g_BcnLostCnt)
        {
            g_BcnLostCnt = Cnt;
            at_output("BcnLost:%u\n", Cnt);
        }*/
        if (Cnt != 255 && Rcv)
        {
            at_output("BcnLost:%2d,%d\n", Cnt, Rcv);
            reg_write(g_u32BcnLostAddr, 255);
            reg_write(g_u32BcnRcvAddr, 0);
        }
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

void Sys_OtaPatchInit(void)
{
    T_MwOtaLayoutInfo tLayout;
    // OTA
    tLayout.ulaHeaderAddr[0] = MW_OTA_HEADER_ADDR_PATCH_1;
    tLayout.ulaHeaderAddr[1] = MW_OTA_HEADER_ADDR_PATCH_2;
    tLayout.ulaImageAddr[0] = MW_OTA_IMAGE_ADDR_PATCH_1;
    tLayout.ulaImageAddr[1] = MW_OTA_IMAGE_ADDR_PATCH_2;
    tLayout.ulImageSize = MW_OTA_IMAGE_SIZE_PATCH;
    MwOta_Init(&tLayout, 0);
}
