/* Not supported when XIP enabled */

/* Cache RAM */
CACHE_START = 0x454000;
CACHE_END   = 0x45C000;
CACHE_LEN   = (CACHE_END - CACHE_START);

MEMORY
{
    CACHE_RAM(rwx)       : ORIGIN = CACHE_START, LENGTH = CACHE_LEN
}

SECTIONS
{
    . = CACHE_START;
    
    __cache_text_start__ = ADDR(.cache_text);
    .cache_text ALIGN(4) : {
        *wpa_supplicant.a:bss.o(.text .text*)
        *wpa_supplicant.a:wpa_ie.o(.text .text*)
        *wpa_supplicant.a:wpa_cli.o(.text .text*)
        *wpa_supplicant.a:wpa3_main.o(.text .text*)
        *wpa_supplicant.a:aes-omac1.o(.text .text*)
        *wpa_supplicant.a:sha1-prf.o(.text .text*)
        *wpa_supplicant.a:sha256-kdf.o(.text .text*)
        *wpa_supplicant.a:sha256-prf.o(.text .text*)
        *wpa_supplicant.a:driver_opl.o(.text .text*)
        *wpa_supplicant.a:pmksa_cache.o(.text .text*)
        *wpa_supplicant.a:wpas_glue.o(.text .text*)
        *wpa_supplicant.a:rc4.o(.text .text*)
        *wpa_supplicant.a:config.o(.text .text*)
        *opl2500_patch_aps.a:at_cmd_wifi.o(.text .text*)
        *opl2500_patch_aps.a:at_cmd_msg_ext.o(.text .text*)
        *opl2500_patch_aps.a:at_cmd_property.o(.text .text*)
        *opl2500_patch_aps.a:event_loop.o(.text .text*)
        /**opl2500_patch_aps.a:wifi_event_handler.o(.text .text*)*/
        *mw_fim_patch.o(.text .text*)
        *mw_fim_default_group02_patch.o(.text .text*)
    } >CACHE_RAM
    . = ALIGN(4);
    __cache_text_end__ = .;

    __cache_rodata_start__ = ADDR(.cache_rodata);
    .cache_rodata ALIGN(4) : { 
        *wpa_supplicant.a:bss.o(.rodata .rodata*)
        *wpa_supplicant.a:wpa_debug.o(.rodata .rodata*)
        *wpa_supplicant.a:wpa_common.o(.rodata .rodata*)
        *wpa_supplicant.a:wpabuf.o(.rodata .rodata*)
        *wpa_supplicant.a:wpa_ie.o(.rodata .rodata*)
        *wpa_supplicant.a:wpa_cli.o(.rodata .rodata*)
        *wpa_supplicant.a:wpa2_main.o(.rodata .rodata*)
        *wpa_supplicant.a:wpa3_main.o(.rodata .rodata*)
        *wpa_supplicant.a:aes-ccm.o(.rodata .rodata*)
        *wpa_supplicant.a:aes-omac1.o(.rodata .rodata*)
        *wpa_supplicant.a:sha1-prf.o(.rodata .rodata*)
        *wpa_supplicant.a:sha256-kdf.o(.rodata .rodata*)
        *wpa_supplicant.a:sha256-prf.o(.rodata .rodata*)
        *wpa_supplicant.a:driver_opl.o(.rodata .rodata*)
        *wpa_supplicant.a:pmksa_cache.o(.rodata .rodata*)
        *wpa_supplicant.a:wpas_glue.o(.rodata .rodata*)
        *wpa_supplicant.a:rc4.o(.rodata .rodata*)
        *wpa_supplicant.a:crypto_mbedtls-bignum.o(.rodata .rodata*)
        *wpa_supplicant.a:config.o(.rodata .rodata*)
        *wpa_supplicant.a:ccmp.o(.rodata .rodata*)
        *opl2500_patch_aps.a:at_cmd_msg_ext.o(.rodata .rodata*)
        *opl2500_patch_aps.a:at_cmd_property.o(.rodata .rodata*)
        *opl2500_patch_aps.a:at_cmd_app.o(.rodata .rodata*)
        *opl2500_patch_aps.a:at_cmd_wifi.o(.rodata .rodata*)
        *opl2500_patch_aps.a:event_loop.o(.rodata .rodata*)
        *opl2500_patch_aps.a:wifi_event_handler.o(.rodata .rodata*)
        
        *mw_fim_patch.o(.rodata .rodata*)
        *mw_fim_default_group02_patch.o(.rodata .rodata*)
    } >CACHE_RAM
    . = ALIGN(4);
    __cache_rodata_end__ = .;
    
    __cache_data_start__ = ADDR(.cache_data);
    .cache_data ALIGN(4) : { 
        *wpa_supplicant.a:bss.o(.data .data*)
        *wpa_supplicant.a:ieee802_11_common.o(.data .data*)
        *wpa_supplicant.a:sae.o(.data .data*)
        *wpa_supplicant.a:wpa_debug.o(.data .data*)
        *wpa_supplicant.a:wpa_common.o(.data .data*)
        *wpa_supplicant.a:wpabuf.o(.data .data*)
        *wpa_supplicant.a:wpa_ie.o(.data .data*)
        *wpa_supplicant.a:wpa_cli.o(.data .data*)
        *wpa_supplicant.a:wpa2_main.o(.data .data*)
        *wpa_supplicant.a:wpa3_main.o(.data .data*)
        *wpa_supplicant.a:aes-omac1.o(.data .data*)
        *wpa_supplicant.a:sha1-prf.o(.data .data*)
        *wpa_supplicant.a:sha256-kdf.o(.data .data*)
        *wpa_supplicant.a:sha256-prf.o(.data .data*)
        *wpa_supplicant.a:driver_opl.o(.data .data*)
        *wpa_supplicant.a:pmksa_cache.o(.data .data*)
        *wpa_supplicant.a:wpas_glue.o(.data .data*)
        *wpa_supplicant.a:rc4.o(.data .data*)
        *wpa_supplicant.a:crypto_mbedtls.o(.data .data*)
        *wpa_supplicant.a:crypto_mbedtls-bignum.o(.data .data*)
        *wpa_supplicant.a:dh_groups.o(.data .data*)
        *wpa_supplicant.a:wpa.o(.data .data*)
        *wpa_supplicant.a:config.o(.data .data*)
        *wpa_supplicant.a:ccmp.o(.data .data*)
        *wpa_supplicant.a:aes-ccm.o(.data .data*)
        *opl2500_patch_aps.a:at_cmd_msg_ext.o(.data .data*)
        *opl2500_patch_aps.a:at_cmd_property.o(.data .data*)
        *opl2500_patch_aps.a:at_cmd_wifi.o(.data .data*)
        *opl2500_patch_aps.a:at_cmd_app.o(.data .data*)
        *opl2500_patch_aps.a:event_loop.o(.data .data*)
        *opl2500_patch_aps.a:wifi_event_handler.o(.data .data*)
        *opl2500_patch_aps.a:wifi_api.o(.data .data*)
        
        *mw_fim_patch.o(.data .data*)
        *mw_fim_default_group02_patch.o(.data .data*)
    } >CACHE_RAM
    . = ALIGN(4);
    __cache_data_end__ = .;
    /* Don't put BSS part here.
     * When XIP enabled, this ram will be selected to cache for XIP.
     * If BSS exists, it needs to be ZI in Sys init.
     * But XIP enable flag is based on application.
     * So don't allocat BSS part here */
}