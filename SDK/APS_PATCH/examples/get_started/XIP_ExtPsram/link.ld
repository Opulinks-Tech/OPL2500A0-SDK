APS_PATCH_START = LD_APS_PATCH_START;
APS_HEAP_START  = LD_APS_HEAP_START;
APS_RAM_END     = LD_APS_RAM_END;
APS_XIP_SIZE    = LD_APS_XIP_SIZE;
APS_XIP_START   = 0x01000000;
APS_XIP_RW_START = 0x03000000;
APS_XIP_END_RWBASE = (APS_XIP_RW_START + APS_XIP_SIZE);
/* Share RAM */
SHM_START = 0x508168;
SHM_END   = 0x50B080;
SHM_LEN   = (SHM_END - SHM_START);
/* Rom pbuf memory */
APS_PART1_START = 0x409B64;
APS_PART1_END   = 0x40FB28;
APS_PART1_LEN   = (APS_PART1_END - APS_PART1_START);


MEMORY
{
    APS_RAM (rwx)        : ORIGIN = APS_PATCH_START, LENGTH = APS_RAM_END - APS_PATCH_START
    SHARE_RAM (rwx)      : ORIGIN = SHM_START, LENGTH = SHM_LEN
    APS_PART1_RAM (rwx)  : ORIGIN = APS_PART1_START, LENGTH = APS_PART1_LEN
    APS_XIP (rx)         : ORIGIN = APS_XIP_START, LENGTH = APS_XIP_SIZE
}


SECTIONS
{
    . = SHM_START;
    /* Put most text and rodata to XIP */
    __shm_data_start__ = ADDR(.shm_data);
    .shm_data ALIGN(4) : { 
        *opl2500_sdk_aps_sub.a:*.o(.data .data*)
        *opl2500_patch_aps.a:*.o(.data .data*)
        *wpa_supplicant.a:*.o(.data .data*)
        *main_patch.o(.data .data*)
    } >SHARE_RAM
    . = ALIGN(4);
    __shm_data_end__ = .;
    
    __shm_bss_start__ = ADDR(.shm_bss);
    .shm_bss ALIGN(4) : {
        EXCLUDE_FILE(memp_patch.o) *opl2500_sdk_aps_sub.a:*.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:*.o(.bss .bss* COMMON)
        *wpa_supplicant.a:*.o(.bss .bss* COMMON)
        *main_patch.o(.bss .bss* COMMON)
    } >SHARE_RAM
    . = ALIGN(4);
    __shm_bss_end__ = .;
}

SECTIONS
{
    . = APS_PART1_START;
    /* Put most text and rodata to XIP */
    __part1_text_start__ = ADDR(.part1_text);
    .part1_text ALIGN(4) : { 
        /* --> Must put on RAM */
        *(FORCE_RAM_REGION.text)
        *opl2500_sdk_aps_sub.a:hal*.o(.text .text*)
        *opl2500_sdk_aps_sub.a:sys_init_patch.o(.text .text*)
        *opl2500_sdk_aps_sub.a:irq_patch.o(.text .text*)
        *opl2500_sdk_aps_sub.a:mw_fim*.o(.text .text*)
        *opl2500_sdk_aps_sub.a:mw_ota*.o(.text .text*)
        
        *opl2500_patch_aps.a:boot_*.o(.text .text*)
        *opl2500_patch_aps.a:hal_*.o(.text .text*)
        *opl2500_patch_aps.a:ps_*.o(.text .text*)
        *opl2500_patch_aps.a:ipc_*.o(.text .text*)
        *opl2500_patch_aps.a:heap_*.o(.text .text*)
        *opl2500_patch_aps.a:freertos_cmsis_patch.o(.text .text*)
        *main_patch.o(.text .text*)*/
        /* <--- Must put on RAM */
        /*
        *(.glue_7)
        *(.glue_7t)
        *(.vfp11_veneer)
        *(.v4_bx)
        *(.iplt)
        *(.igot.plt)
        
        *libgcc.a:*.o(.text .text*)
        *opl2500_patch_aps.a:heap_4_psram.o(.text .text*)
        *opl2500_patch_aps.a:sys_cfg_patch.o(.text .text*)*/
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_text_end__ = .;

    __part1_rodata_start__ = ADDR(.part1_rodata);
    .part1_rodata ALIGN(4) : { 
        /* --> Must put on RAM */
        *(.rel.dyn)
        *(.rel.iplt)
        *(.rodata.str*)
        *opl2500_sdk_aps_sub.a:hal*.o(.rodata .rodata*)
        *opl2500_sdk_aps_sub.a:sys_init_patch.o(.rodata .rodata*)
        *opl2500_sdk_aps_sub.a:irq_patch.o(.rodata .rodata*)
        *opl2500_sdk_aps_sub.a:mw_ota*.o(.rodata .rodata*)
        *opl2500_sdk_aps_sub.a:mw_fim*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:boot_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:hal_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ps_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ipc_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:heap_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:freertos_cmsis_patch.o(.rodata .rodata*)
        /* <--- Must put on RAM */

    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_rodata_end__ = .;
    
    __part1_data_start__ = ADDR(.part1_data);
    .part1_data ALIGN(4) : { 
        *arm-none-eabi*.a:*.o(.data .data*)
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_data_end__ = .;
    
    __part1_bss_start__ = ADDR(.part1_bss);
    .part1_bss ALIGN(4) : {
        *arm-none-eabi*.a:*.o(.bss .bss* COMMON)
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_bss_end__ = .;
}


SECTIONS
{

    . = APS_PATCH_START;
    end = .;
    . = ALIGN(4);
    
    .aps_entry ALIGN(4) : { *(ENTRY_POINT) } >APS_RAM
    
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


SECTIONS
{
    . = APS_XIP_START;
    __xip_text_start__ = APS_XIP_START;
    .xip_text __xip_text_start__ : ALIGN (4) {
        *(XIP_REGION.text)
        *.o(.text .text*)
        
        . = ALIGN(4);
    } >APS_XIP
    __xip_text_end__ = .;
    __xip_text_size__ = SIZEOF(.xip_text);
    
        
    __xip_rodata_start__ = ADDR(.xip_rodata);
    .xip_rodata ALIGN(4) : {
        *(XIP_REGION.rodata)
        *.o(.rodata .rodata*)
        . = ALIGN(4);
    } >APS_XIP
    __xip_rodata_end__ = .;
    __xip_rodata_size__ = SIZEOF(.xip_rodata);
    
    __xip_data_start__ = APS_XIP_RW_START + __xip_text_size__ + __xip_rodata_size__;
    .xip_data __xip_data_start__ : ALIGN (4) {
        *opl2500_patch_aps.a:*.o(.data .data*)
        *wpa_supplicant.a:*.o(.data .data*)
        *arm-none-eabi*.a:*.o(.data .data*)
        *(PSRAM_REGION.data)
        . = ALIGN(4);
    } AT >APS_XIP
    __xip_data_end__ = .;
    __xip_data_size__ = SIZEOF(.xip_data);
    
    __xip_bss_start__ = ADDR(.xip_bss);
    .xip_bss ALIGN(4) : {
        *opl2500_patch_aps.a:*.o(.bss .bss* COMMON)
        *wpa_supplicant.a:*.o(.bss .bss* COMMON)
        *(PSRAM_REGION.bss)
        . = ALIGN(4);
    } AT >APS_XIP
    __xip_bss_end__ = .;
    __xip_bss_size__ = SIZEOF(.xip_bss);
    
    . = ALIGN(16);
    __pheap_start__ = .;
    __pheap_size__ = APS_XIP_END_RWBASE - __pheap_start__;
    .pheap ALIGN(16) : { . = . + __pheap_size__; }AT >APS_XIP
    __pheap_end__ = .;
    
}

