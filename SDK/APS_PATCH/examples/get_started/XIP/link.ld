APS_PATCH_START = LD_APS_PATCH_START;
APS_HEAP_START  = LD_APS_HEAP_START;
APS_RAM_END     = LD_APS_RAM_END;
APS_XIP_RANGE   = LD_APS_XIP_RANGE;
APS_XIP_START   = 0x01000000;

MEMORY
{
    APS_RAM (rwx) : ORIGIN = APS_PATCH_START, LENGTH = APS_RAM_END - APS_PATCH_START
    APS_XIP (rx)  : ORIGIN = APS_XIP_START, LENGTH = APS_XIP_RANGE
}

INCLUDE link_sub.ld

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
    __heap_size__ = APS_RAM_END - __heap_start__;
    __heap_start__ = ADDR(.heap );
    .heap ALIGN(4) : { . = . + __heap_size__; } >APS_RAM
    __heap_end__ = .;
}

SECTIONS
{
    . = APS_XIP_START;
    __xip_region_start__ = APS_XIP_START;
    .xip_region __xip_region_start__ : ALIGN (4) {
        *(XIP_REGION.text)
        . = ALIGN(4);
        *(XIP_REGION.rodata)
        . = ALIGN(4);
    } >APS_XIP
    __xip_region_length__ = SIZEOF(.xip_region);
    __xip_region_end__ = .;
}
