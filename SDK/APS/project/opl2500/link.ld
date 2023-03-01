APS_PATCH_START = LD_APS_PATCH_START;
APS_HEAP_START  = LD_APS_HEAP_START;
APS_RAM_END     = LD_APS_RAM_END;


MEMORY
{
    APS_RAM (rwx)   : ORIGIN = APS_PATCH_START, LENGTH = APS_RAM_END - APS_PATCH_START
}


INCLUDE link_sub.ld
INCLUDE link_cacheram.ld

SECTIONS
{
    . = APS_PATCH_START;
    end = .;
    . = ALIGN(4);
    .aps_entry : { *(ENTRY_POINT) } >APS_RAM
    
    __aps_text_start__ = ADDR(.aps_text);
    .aps_text ALIGN(4) : { *(.text .text*) } >APS_RAM
    . = ALIGN(4);
    __aps_text_end__ = .;

    __aps_rodata_start__ = ADDR(.aps_rodata);
    .aps_rodata ALIGN(4) : { *(.rodata .rodata*) } >APS_RAM
    . = ALIGN(4);
    __aps_rodata_end__ = .;
    
    __aps_data_start__ = ADDR(.aps_data);
    .aps_data ALIGN(4) : { *(.data .data*) } >APS_RAM
    . = ALIGN(4);
    __aps_data_end__ = .;
    
    __aps_bss_start__ = ADDR(.aps_bss);
    .aps_bss ALIGN(4) : { *(.bss .bss* COMMON) } >APS_RAM
    . = ALIGN(4);
    __aps_bss_end__ = .;
    
    . = APS_HEAP_START;
    __heap_size__ = APS_RAM_END - __heap_start__;
    __heap_start__ = ADDR(.heap );
    .heap ALIGN(4) : { . = . + __heap_size__; } >APS_RAM
    __heap_end__ = .;
}
