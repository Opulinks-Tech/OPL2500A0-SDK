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
        *opl2500_patch_aps.a:diag_*.o(.text .text*)
        *opl2500_patch_aps.a:sys_*.o(.text .text*)
        *opl2500_patch_aps.a:freertos_*.o(.text .text*)
        *opl2500_patch_aps.a:mw_fim_msq_patch.o(.text .text*)
    } >SHARE_RAM
    . = ALIGN(4);
    __shm_text_end__ = .;

    __shm_rodata_start__ = ADDR(.shm_rodata);
    .shm_rodata ALIGN(4) : {
        *opl2500_patch_aps.a:freertos_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:at_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ccmp_patch.o(.rodata .rodata*)
        *wpa_supplicant.a:*.o(.rodata .rodata*)
        
    } >SHARE_RAM
    . = ALIGN(4);
    __shm_rodata_end__ = .;
    
    __shm_data_start__ = ADDR(.shm_data);
    .shm_data ALIGN(4) : { 
        *wpa_supplicant.a:*.o(.data .data*)
    } >SHARE_RAM
    . = ALIGN(4);
    __shm_data_end__ = .;
    
    __shm_bss_start__ = ADDR(.shm_bss);
    .shm_bss ALIGN(4) : {
        *wpa_supplicant.a:*.o(.bss .bss* COMMON)
    } >SHARE_RAM
    . = ALIGN(4);
    __shm_bss_end__ = .;
}

SECTIONS
{
    . = APS_PART1_START;
    
    __part1_text_start__ = ADDR(.part1_text);
    .part1_text ALIGN(4) : { 
        /* --> Must put on RAM */
        *(FORCE_RAM_REGION.text)
        *aps_patch/driver/*.o(.text .text*)
        *aps_patch/project/opl2500/*.o(.text .text*)
        *aps_patch/middleware/netlink/mw_ota/*.o(.text .text*)
        *aps_patch/middleware/netlink/mw_fim/*.o(.text .text*)
        *opl2500_patch_aps.a:boot_*.o(.text .text*)
        *opl2500_patch_aps.a:hal_*.o(.text .text*)
        *opl2500_patch_aps.a:ps_*.o(.text .text*)
        *opl2500_patch_aps.a:ipc_*.o(.text .text*)
        *opl2500_patch_aps.a:heap_*.o(.text .text*)
        *opl2500_patch_aps.a:freertos_cmsis_patch.o(.text .text*)
        /* <--- Must put on RAM */
        
        *(.glue_7)
        *(.glue_7t)
        *(.vfp11_veneer)
        *(.v4_bx)
        *(.iplt)
        *(.igot.plt)
        
        *libgcc.a:*.o(.text .text*)
        *opl2500_patch_aps.a:heap_4_psram.o(.text .text*)
        *opl2500_patch_aps.a:sys_cfg_patch.o(.text .text*)
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_text_end__ = .;

    __part1_rodata_start__ = ADDR(.part1_rodata);
    .part1_rodata ALIGN(4) : { 
        /* --> Must put on RAM */
        *(.rel.dyn)
        *(.rel.iplt)
        *(.rodata.str*)
        *aps_patch/driver/*.o(.rodata .rodata*)
        *aps_patch/project/opl2500/*.o(.rodata .rodata*)
        *aps_patch/middleware/netlink/mw_ota/*.o(.rodata .rodata*)
        *aps_patch/middleware/netlink/mw_fim/*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:boot_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:hal_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ps_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:ipc_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:heap_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:freertos_cmsis_patch.o(.rodata .rodata*)
        /* <--- Must put on RAM */

        *opl2500_patch_aps.a:agent_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:heap_4_psram.o(.rodata .rodata*)
        *opl2500_patch_aps.a:sys_cfg_patch.o(.rodata .rodata*)
        *opl2500_patch_aps.a:*le_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:*wifi_*.o(.rodata .rodata*)
        *opl2500_patch_aps.a:diag_*.o(.rodata .rodata*)
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_rodata_end__ = .;
    
    __part1_data_start__ = ADDR(.part1_data);
    .part1_data ALIGN(4) : { 
        /* --> Must put on RAM */
        *aps_patch/driver/*.o(.data .data*)
        *aps_patch/project/opl2500/*.o(.data .data*)
        *aps_patch/middleware/netlink/mw_ota/*.o(.data .data*)
        *opl2500_patch_aps.a:boot_*.o(.data .data*)
        *opl2500_patch_aps.a:hal_*.o(.data .data*)
        *opl2500_patch_aps.a:ps_*.o(.data .data*)
        *opl2500_patch_aps.a:ipc_*.o(.data .data*)
        *opl2500_patch_aps.a:heap_*.o(.data .data*)
        *opl2500_patch_aps.a:freertos_cmsis_patch.o(.data .data*)
        /* <--- Must put on RAM */
        
        *aps_patch/middleware/netlink/mw_fim/*.o(.data .data*)
        *opl2500_patch_aps.a:*.o(.data .data*)
        *arm-none-eabi*.a:*.o(.data .data*)
        *aps_patch/middleware/*.o(.data .data*)
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_data_end__ = .;
    
    __part1_bss_start__ = ADDR(.part1_bss);
    .part1_bss ALIGN(4) : {
        /* --> Must put on RAM */
        *aps_patch/driver/*.o(.bss .bss* COMMON)
        *aps_patch/project/opl2500/*.o(.bss .bss* COMMON)
        *aps_patch/middleware/netlink/mw_ota/*.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:boot_*.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:hal_*.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:ps_*.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:ipc_*.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:heap_*.o(.bss .bss* COMMON)
        *opl2500_patch_aps.a:freertos_cmsis_patch.o(.bss .bss* COMMON)
        /* <--- Must put on RAM */
        
        *aps_patch/middleware/netlink/mw_fim/*.o(.bss .bss* COMMON)
        *arm-none-eabi*.a:*.o(.bss .bss* COMMON)
         EXCLUDE_FILE (*memp_patch.o) *aps_patch/middleware/*.o(.bss .bss* COMMON)        
    } >APS_PART1_RAM
    . = ALIGN(4);
    __part1_bss_end__ = .;
}
