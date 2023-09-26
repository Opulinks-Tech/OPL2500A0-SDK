APS_PATCH_START = LD_APS_PATCH_START;
APS_HEAP_START  = LD_APS_HEAP_START;
APS_RAM_END     = LD_APS_RAM_END;
APS_XIP_SIZE    = LD_APS_XIP_SIZE;
APS_XIP_START   = 0x01000000;

PATCH_HEADER_LEN = 20;
APS_XIP_REGION_SIZE = APS_XIP_SIZE - PATCH_HEADER_LEN;

MEMORY
{
    APS_RAM (rwx) : ORIGIN = APS_PATCH_START, LENGTH = APS_RAM_END - APS_PATCH_START
    APS_XIP (rx)  : ORIGIN = APS_XIP_START, LENGTH = APS_XIP_REGION_SIZE
}

INCLUDE link_sub.ld

SECTIONS
{
    . = APS_XIP_START;
    __xip_text_start__ = APS_XIP_START;
    .xip_text __xip_text_start__ : ALIGN (4) {
        *opl2500_patch_aps.a:*.o(.text .text*)
        *wpa_supplicant.a:*.o(.text .text*)
        *arm-none-eabi*.a:*.o(.text .text*)
        *(XIP_REGION.text)
        . = ALIGN(4);
    } >APS_XIP
    __xip_text_end__ = .;
    __xip_text_size__ = SIZEOF(.xip_text);
    
    
    __xip_rodata_start__ = ADDR(.xip_rodata);
    .xip_rodata ALIGN(4) : {
        *opl2500_patch_aps.a:*.o(.rodata .rodata*)
        *wpa_supplicant.a:*.o(.rodata .rodata*)
        *(XIP_REGION.rodata)
        . = ALIGN(4);
    } >APS_XIP
    __xip_rodata_end__ = .;
    __xip_rodata_size__ = SIZEOF(.xip_rodata);
}

SECTIONS
{
    . = APS_PATCH_START;
    end = .;
    . = ALIGN(4);
    .aps_entry : { *(ENTRY_POINT) } >APS_RAM
    
    __aps_text_start__ = ADDR(.aps_text);
    .aps_text ALIGN(4) : { *(.text) *(.text*) } >APS_RAM
    . = ALIGN(4);
    __aps_text_end__ = .;

    __aps_rodata_start__ = ADDR(.aps_rodata);
    .aps_rodata ALIGN(4) : { *(.rodata) *(.rodata*) } >APS_RAM
    . = ALIGN(4);
    __aps_rodata_end__ = .;
    
    __aps_data_start__ = ADDR(.aps_data);
    .aps_data ALIGN(4) : { *(.data) *(.data*) } >APS_RAM
    . = ALIGN(4);
    __aps_data_end__ = .;
    
    __aps_bss_start__ = ADDR(.aps_bss);
    .aps_bss ALIGN(4) : { *(.bss) *(.bss*) *(COMMON) } >APS_RAM
    . = ALIGN(4);
    __aps_bss_end__ = .;
    
    . = APS_HEAP_START;
    . = ALIGN(16);
    __heap_start__ = .;
    __heap_end__ = APS_RAM_END;
    __heap_size__ = APS_RAM_END - __heap_start__;
    .heap ALIGN(16) : { 
        . = .;
        ASSERT((__heap_start__  >= __aps_bss_end__ ), "Error: Heap and APS Bss section are overlapped");
        ASSERT((__heap_start__ < APS_RAM_END), "Error: APS_HEAP_START is over than APS_RAM_END");
        ASSERT((APS_RAM_END <= 0x454000), "Error: APS_RAM_END definition is invalid, APS Ram end cannot be over 0x454000");
    } >APS_RAM
}
