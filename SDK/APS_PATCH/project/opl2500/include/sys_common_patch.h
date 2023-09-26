/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  sys_common_patch.h
*
*  Project:
*  --------
*  OPL2500 Project - the system common definition file
*
*  Description:
*  ------------
*  This include file is the system common definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/

#ifndef _SYS_COMMON_H_
#error "Please include sys_common.h. Don't directly include sys_common_patch.h"
#endif

#ifndef _SYS_COMMON_PATCH_H_
#define _SYS_COMMON_PATCH_H_


#define REFINE_TX_PROC          //defined: enable tx-one-copy
#define REFINE_RX_PROC      2   //0:original / 1:simplify-decryption / 2:simplify-decryption + rx-one-copy

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#if defined(GCC)
    #define __forceinline   inline
    #undef _ARMABI_PURE
    #define _ARMABI_PURE __attribute__((nothrow, const))
        
    #ifdef __cplusplus
        //namespace std {
        #define __CLIBNS ::std::
        //extern "C" {
    #else
      #define __CLIBNS
    #endif  /* __cplusplus */
    
    /* HEAP */
    extern unsigned int __heap_start__, __heap_size__;
    #define APS_HEAP_START      ((unsigned int)&__heap_start__)
    #define APS_HEAP_LENGTH     ((unsigned int)&__heap_size__)
    
    /* APS ZI */
    extern unsigned int __aps_bss_start__, __aps_bss_end__;
    #define ZI_REGION_START     (&__aps_bss_start__)
    #define ZI_REGION_LENGTH    ((unsigned int) &__aps_bss_end__ - (unsigned int) &__aps_bss_start__)
    /* Shm ZI */
    extern unsigned int __shm_bss_start__, __shm_bss_end__;
    #define ZI_REGION_SHARERAM_START     (&__shm_bss_start__)
    #define ZI_REGION_SHARERAM_LENGTH    ((unsigned int) &__shm_bss_end__ - (unsigned int) &__shm_bss_start__)
    /* part1 RAM */
    extern unsigned int __part1_bss_start__, __part1_bss_end__;
    #define ZI_REGION_PART1RAM_START     (&__part1_bss_start__)
    #define ZI_REGION_PART1RAM_LENGTH    ((unsigned int) &__part1_bss_end__ - (unsigned int) &__part1_bss_start__)

    /* XIP region */
    #define XIP_TEXT            __attribute__((section("XIP_REGION.text")))
    #define XIP_RODATA          __attribute__((section("XIP_REGION.rodata")))
    #define FORCE_RAM_TEXT      __attribute__((section("FORCE_RAM_REGION.text")))     /* XIP preparation functions must be on RAM. It shall not be on XIP memory */
    
    /* PSRAM region */
    #define PSRAM_DATA          __attribute__((section("PSRAM_REGION.data")))       /* PSRAM region for data usage */
    #define PSRAM_ZIDATA        __attribute__((section("PSRAM_REGION.bss")))        /* PSRAM region for data usage */
    
    /* PSRAM ZI RAM */
    extern unsigned int __xip_bss_start__, __xip_bss_size__;
    #define ZI_REGION_PSRAM_START       (&__xip_bss_start__)
    #define ZI_REGION_PSRAM_LENGTH      ((unsigned int) &__xip_bss_size__)
    
    extern unsigned int __pheap_start__, __pheap_size__;
    #define APS_PHEAP_START             ((unsigned int)&__pheap_start__)
    #define APS_PHEAP_LENGTH            ((unsigned int)&__pheap_size__)
    #define configTOTAL_PHEAP_SIZE  APS_HEAP_LENGTH
#else
    /* HEAP */
    #define APS_HEAP_START              ((unsigned int)Image$$RW_IRAMx_Heap$$ZI$$Base)
    #define APS_HEAP_LENGTH             ((unsigned int)&Image$$RW_IRAMx_Heap$$ZI$$Length)
    extern char Image$$RW_IRAMx_Heap$$ZI$$Base[];
    extern unsigned int Image$$RW_IRAMx_Heap$$ZI$$Length;
    
    /* APS ZI */
    extern unsigned int Image$$RW_IRAM1$$ZI$$Length;
    extern char Image$$RW_IRAM1$$ZI$$Base[];
    #define ZI_REGION_START             (Image$$RW_IRAM1$$ZI$$Base)
    #define ZI_REGION_LENGTH            ((unsigned int)&Image$$RW_IRAM1$$ZI$$Length)
    /* Shm ZI */
    extern unsigned int Image$$RW_IRAM2_ShareRam$$ZI$$Length;
    extern char Image$$RW_IRAM2_ShareRam$$ZI$$Base[];
    #define ZI_REGION_SHARERAM_START    (Image$$RW_IRAM2_ShareRam$$ZI$$Base)
    #define ZI_REGION_SHARERAM_LENGTH   ((unsigned int)&Image$$RW_IRAM2_ShareRam$$ZI$$Length)
    /* part1 RAM */
    extern unsigned int Image$$RW_IRAM3_PartMem$$ZI$$Length;
    extern char Image$$RW_IRAM3_PartMem$$ZI$$Base[];
    #define ZI_REGION_PART1RAM_START     (Image$$RW_IRAM3_PartMem$$ZI$$Base)
    #define ZI_REGION_PART1RAM_LENGTH    ((unsigned int)&Image$$RW_IRAM3_PartMem$$ZI$$Length)
    
    /* XIP region */
    #define XIP_TEXT            __attribute__((section("XIP_REGION")))
    #define XIP_RODATA          __attribute__((section("XIP_REGION")))
    #define FORCE_RAM_TEXT      __attribute__((section("FORCE_RAM_REGION")))     /* XIP preparation functions must be on RAM. It shall not be on XIP memory */
    
    extern unsigned int Image$$XIP_ROM$$Length;
    #define XIP_REGION_LENGTH    (((unsigned int)&Image$$XIP_ROM$$Length) + 20)
                                                                /* sizeof(PatchHeader) = 20 */
    
    
    /* PSRAM region */
    #define PSRAM_DATA          __attribute__((section("PSRAM_REGION")))         /* PSRAM region for data usage */
    #define PSRAM_ZIDATA        __attribute__((section("PSRAM_REGION")))         /* PSRAM region for data usage */
    
    /* Psram ZI */
    extern unsigned int Image$$XIP_RAM$$ZI$$Length;
    extern char Image$$XIP_RAM$$ZI$$Base[];
    #define ZI_REGION_PSRAM_START       (Image$$XIP_RAM$$ZI$$Base)
    #define ZI_REGION_PSRAM_LENGTH      ((unsigned int)&Image$$XIP_RAM$$ZI$$Length)
    
    #define APS_PHEAP_START             ((unsigned int)Image$$XIP_HEAP$$ZI$$Base)
    #define APS_PHEAP_LENGTH            ((unsigned int)&Image$$XIP_HEAP$$ZI$$Length)
    extern char Image$$XIP_HEAP$$ZI$$Base[];
    extern unsigned int Image$$XIP_HEAP$$ZI$$Length;
    #define configTOTAL_PHEAP_SIZE      APS_PHEAP_LENGTH
#endif



#ifndef OPL2500_XIP
/* If XIP not enabled, no specific section */
#undef XIP_TEXT
#undef XIP_RODATA
#define XIP_TEXT
#define XIP_RODATA
#endif /* OPL2500_XIP */
    
#ifndef  OPL2500_PSRAM 
#undef PSRAM_DATA   
#undef PSRAM_ZIDATA 
#endif /* OPL2500_PSRAM */



/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _SYS_COMMON_PATCH_H_
