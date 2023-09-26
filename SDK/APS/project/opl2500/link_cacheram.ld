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
        *opl2500_patch_aps.a:*.o(.text .text*)
    } >CACHE_RAM
    . = ALIGN(4);
    __cache_text_end__ = .;

    __cache_rodata_start__ = ADDR(.cache_rodata);
    .cache_rodata ALIGN(4) : { 
        *wpa_supplicant.a:*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:*.o(.rodata .rodata*)
    } >CACHE_RAM
    . = ALIGN(4);
    __cache_rodata_end__ = .;
    
    __cache_data_start__ = ADDR(.cache_data);
    .cache_data ALIGN(4) : { 
        *wpa_supplicant.a:*.o(.data .data*)
        *opl2500_patch_aps.a:*.o(.data .data*)
    } >CACHE_RAM
    . = ALIGN(4);
    __cache_data_end__ = .;
    /* Don't put BSS part here.
     * When XIP enabled, this ram will be selected to cache for XIP.
     * If BSS exists, it needs to be ZI in Sys init.
     * But XIP enable flag is based on application.
     * So don't allocat BSS part here */
}