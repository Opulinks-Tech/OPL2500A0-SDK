/* Share RAM */
/* SHM_START = 0x5091E8; */
SHM_START = 0x5091E8;
SHM_END   = 0x50C100;
SHM_LEN   = (SHM_END - SHM_START);
/* Rom pbuf memory */
APS_PART1_START = 0x409B64;
APS_PART1_END   = 0x40FB28;
APS_PART1_LEN   = (APS_PART1_END - APS_PART1_START);

MEMORY
{
    SHARE_RAM (rwx)      : ORIGIN = SHM_START, LENGTH = SHM_LEN
    APS_PART1_RAM (rwx)  : ORIGIN = APS_PART1_START, LENGTH = APS_PART1_LEN
}


SECTIONS
{
    . = SHM_START;
    __shm_text_start__ = ADDR(.shm_text);
    .shm_text ALIGN(4) : {
        *opl2500_patch_aps.a:boot_sequence_patch.o(.text .text*)
        *opl2500_patch_aps.a:freertos_cmsis_patch.o(.text .text*)
        *opl2500_patch_aps.a:diag_task_patch.o(.text .text*)
        *opl2500_patch_aps.a:ipc_patch.o(.text .text*)
        *opl2500_patch_aps.a:mw_fim_msq_patch.o(.text .text*)
        *opl2500_patch_aps.a:ps_func_map_patch.o(.text .text*)
        *opl2500_patch_aps.a:ps_patch.o(.text .text*)
        *opl2500_patch_aps.a:at_cmd_sys_patch.o(.text .text*)
        *opl2500_patch_aps.a:at_cmd_func_patch.o(.text .text*)
        
        *sys_init_patch.o(.text .text*)
        *irq_patch.o(.text .text*)
        *hal_system_patch.o(.text .text*)
        *hal_auxadc_patch.o(.text .text*)
        *hal_flash_patch.o(.text .text*)
        *hal_qspi_patch.o(.text .text*)
        *hal_sys_rcc_patch.o(.text .text*)
        *hal_pin_patch.o(.text .text*)
        *hal_vic_patch.o(.text .text*)
        *hal_uart_patch.o(.text .text*)
        *hal_dma_patch.o(.text .text*)
    } >SHARE_RAM
    . = ALIGN(4);
    __shm_text_end__ = .;

    __shm_rodata_start__ = ADDR(.shm_rodata);
    .shm_rodata ALIGN(4) : {
        *opl2500_patch_aps.a:boot_sequence_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:freertos_cmsis_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:diag_task_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ipc_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:mw_fim_msq_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ps_func_map_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ps_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:at_cmd_sys_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:at_cmd_func_patch.o(.rodata .rodata*)
        
        *wpa_supplicant.a:os_freertos.o(.rodata .rodata*)
        *wpa_supplicant.a:common.o(.rodata .rodata*)
        
        *sys_init_patch.o(.rodata .rodata*)
        *irq_patch.o(.rodata .rodata*)
        *hal_system_patch.o(.rodata .rodata*)
        *hal_auxadc_patch.o(.rodata .rodata*)
        *hal_flash_patch.o(.rodata .rodata*)
        *hal_qspi_patch.o(.rodata .rodata*)
        *hal_sys_rcc_patch.o(.rodata .rodata*)
        *hal_pin_patch.o(.rodata .rodata*)
        *hal_vic_patch.o(.rodata .rodata*)
        *hal_uart_patch.o(.rodata .rodata*)
        *hal_dma_patch.o(.rodata .rodata*)
    } >SHARE_RAM
    . = ALIGN(4);
    __shm_rodata_end__ = .;
    
    __shm_data_start__ = ADDR(.shm_data);
    .shm_data ALIGN(4) : { 
        *opl2500_patch_aps.a:boot_sequence_patch.o(.data .data*)
        *opl2500_patch_aps.a:freertos_cmsis_patch.o(.data .data*)
        *opl2500_patch_aps.a:diag_task_patch.o(.data .data*)
        *opl2500_patch_aps.a:ipc_patch.o(.data .data*)
        *opl2500_patch_aps.a:mw_fim_msq_patch.o(.data .data*)
        *opl2500_patch_aps.a:ps_func_map_patch.o(.data .data*)
        *opl2500_patch_aps.a:ps_patch.o(.data .data*)
        *opl2500_patch_aps.a:at_cmd_sys_patch.o(.data .data*)
        *opl2500_patch_aps.a:at_cmd_func_patch.o(.data .data*)
        
        *wpa_supplicant.a:os_freertos.o(.data .data*)
        *wpa_supplicant.a:common.o(.data .data*)
        
        *sys_init_patch.o(.data .data*)
        *irq_patch.o(.data .data*)
        *hal_system_patch.o(.data .data*)
        *hal_auxadc_patch.o(.data .data*)
        *hal_flash_patch.o(.data .data*)
        *hal_qspi_patch.o(.data .data*)
        *hal_sys_rcc_patch.o(.data .data*)
        *hal_pin_patch.o(.data .data*)
        *hal_vic_patch.o(.data .data*)
        *hal_uart_patch.o(.data .data*)
        *hal_dma_patch.o(.data .data*)
    } >SHARE_RAM
    . = ALIGN(4);
    __shm_data_end__ = .;
    
    __shm_bss_start__ = ADDR(.shm_bss);
    .shm_bss ALIGN(4) : { 
        *opl2500_patch_aps.a:boot_sequence_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:freertos_cmsis_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:diag_task_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:ipc_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:mw_fim_msq_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:ps_func_map_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:ps_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:at_cmd_sys_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:at_cmd_func_patch.o(.bss .bss* COMMON)
        
        *wpa_supplicant.a:os_freertos.o(.bss .bss* COMMON)
        /**wpa_supplicant.a:common.o(.bss .bss* COMMON)*/
        
        *sys_init_patch.o(.bss .bss* COMMON)
        *irq_patch.o(.bss .bss* COMMON)
        *hal_system_patch.o(.bss .bss* COMMON)
        *hal_auxadc_patch.o(.bss .bss* COMMON)
        *hal_flash_patch.o(.bss .bss* COMMON)
        *hal_qspi_patch.o(.bss .bss* COMMON)
        *hal_sys_rcc_patch.o(.bss .bss* COMMON)
        *hal_pin_patch.o(.bss .bss* COMMON)
        *hal_vic_patch.o(.bss .bss* COMMON)
        *hal_uart_patch.o(.bss .bss* COMMON)
        *hal_dma_patch.o(.bss .bss* COMMON)
    } >SHARE_RAM
    . = ALIGN(4);
    __shm_bss_end__ = .;
}

SECTIONS
{
    . = APS_PART1_START;
    
    __part1_text_start__ = ADDR(.part1_text);
    .part1_text ALIGN(4) : { 
        *opl2500_patch_aps.a:sys_cfg_patch.o(.text .text*)
        *opl2500_patch_aps.a:ble_host_patch_init.o(.text .text*)
        *opl2500_patch_aps.a:le_hci_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_pts_app_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_cmd_app_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_smp_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_ctrl_func_map_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_ctrl_hci_uart_tl_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_ctrl_data_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_ctrl_enc_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_ctrl_hci_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_ctrl_hci_handler_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_ctrl_fim_patch.o(.text .text*)
        *opl2500_patch_aps.a:le_ctrl_cmd_patch.o(.text .text*)
        *opl2500_patch_aps.a:wifi_mac_rx_data_patch.o(.text .text*)
        *opl2500_patch_aps.a:wifi_mac_tx_data_patch.o(.text .text*)
        *opl2500_patch_aps.a:wifi_mac_frame_patch.o(.text .text*)
        *opl2500_patch_aps.a:wifi_nvm_patch.o(.text .text*)
        *opl2500_patch_aps.a:controller_wifi_patch.o(.text .text*)
        *opl2500_patch_aps.a:ccmp_patch.o(.text .text*)
        *opl2500_patch_aps.a:tkip_patch.o(.text .text*)
        *opl2500_patch_aps.a:ieee80211_crypto_patch.o(.text .text*)
        /**wpa_supplicant.a:dh_groups.o(.text .text*)*/
        *wpa_supplicant.a:wpa_debug.o(.text .text*)
        
        
        *lwip_helper_patch.o(.text .text*)
        *lwip_jmptbl_patch.o(.text .text*)
        *lwip*memp_patch.o(.text .text*)
        *wlannetif_patch.o(.text .text*)
        *lwip*pbuf_patch.o(.text .text*)
 
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_text_end__ = .;

    __part1_rodata_start__ = ADDR(.part1_rodata);
    .part1_rodata ALIGN(4) : { 
        *opl2500_patch_aps.a:sys_cfg_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ble_host_patch_init.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_hci_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_pts_app_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_cmd_app_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_smp_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_ctrl_func_map_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_ctrl_hci_uart_tl_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_ctrl_data_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_ctrl_enc_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_ctrl_hci_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_ctrl_hci_handler_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_ctrl_fim_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:le_ctrl_cmd_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:wifi_mac_rx_data_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:wifi_mac_tx_data_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:wifi_mac_frame_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:wifi_nvm_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:controller_wifi_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ccmp_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:tkip_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ieee80211_crypto_patch.o(.rodata .rodata*)
        *wpa_supplicant.a:dh_groups.o(.rodata .rodata*)
        *wpa_supplicant.a:crypto_mbedtls.o(.rodata .rodata*)
        
        *lwip_helper_patch.o(.rodata .rodata*)
        *lwip_jmptbl_patch.o(.rodata .rodata*)
        *lwip*memp_patch.o(.rodata .rodata*)
        *wlannetif_patch.o(.rodata .rodata*)
        *lwip*pbuf_patch.o(.rodata .rodata*)
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_rodata_end__ = .;
    
    __part1_data_start__ = ADDR(.part1_data);
    .part1_data ALIGN(4) : { 
        *opl2500_patch_aps.a:sys_cfg_patch.o(.data .data*)
        *opl2500_patch_aps.a:ble_host_patch_init.o(.data .data*)
        *opl2500_patch_aps.a:le_hci_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_pts_app_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_cmd_app_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_smp_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_ctrl_func_map_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_ctrl_hci_uart_tl_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_ctrl_data_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_ctrl_enc_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_ctrl_hci_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_ctrl_hci_handler_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_ctrl_fim_patch.o(.data .data*)
        *opl2500_patch_aps.a:le_ctrl_cmd_patch.o(.data .data*)
        *opl2500_patch_aps.a:wifi_mac_rx_data_patch.o(.data .data*)
        *opl2500_patch_aps.a:wifi_mac_tx_data_patch.o(.data .data*)
        *opl2500_patch_aps.a:wifi_mac_frame_patch.o(.data .data*)
        *opl2500_patch_aps.a:wifi_nvm_patch.o(.data .data*)
        *opl2500_patch_aps.a:controller_wifi_patch.o(.data .data*)
        *opl2500_patch_aps.a:ccmp_patch.o(.data .data*)
        *opl2500_patch_aps.a:tkip_patch.o(.data .data*)
        *opl2500_patch_aps.a:ieee80211_crypto_patch.o(.data .data*)

        *lwip_helper_patch.o(.data .data*)
        *lwip_jmptbl_patch.o(.data .data*)
        *lwip*memp_patch.o(.data .data*)
        *wlannetif_patch.o(.data .data*)
        *lwip*pbuf_patch.o(.data .data*)
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_data_end__ = .;
    
    __part1_bss_start__ = ADDR(.part1_bss);
    .part1_bss ALIGN(4) : {
        *opl2500_patch_aps.a:sys_cfg_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:ble_host_patch_init.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_hci_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_pts_app_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_cmd_app_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_smp_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_ctrl_func_map_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_ctrl_hci_uart_tl_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_ctrl_data_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_ctrl_enc_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_ctrl_hci_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_ctrl_hci_handler_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_ctrl_fim_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:le_ctrl_cmd_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:wifi_mac_rx_data_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:wifi_mac_tx_data_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:wifi_mac_frame_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:wifi_nvm_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:controller_wifi_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:ccmp_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:tkip_patch.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:ieee80211_crypto_patch.o(.bss .bss* COMMON)
        *wpa_supplicant.a:aes-ccm.o(.bss .bss* COMMON)

        *lwip_helper_patch.o(.bss .bss* COMMON)
        *lwip_jmptbl_patch.o(.bss .bss* COMMON)
        *wlannetif_patch.o(.bss .bss* COMMON)
        *lwip*pbuf_patch.o(.bss .bss* COMMON)
        
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_bss_end__ = .;
}
