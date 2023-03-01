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
        namespace std {
        #define __CLIBNS ::std::
        extern "C" {
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
#else
    /* HEAP */
    #define APS_HEAP_START      ((unsigned int)Image$$RW_IRAMx_Heap$$ZI$$Base)
    #define APS_HEAP_LENGTH     ((unsigned int)&Image$$RW_IRAMx_Heap$$ZI$$Length)
    extern char Image$$RW_IRAMx_Heap$$ZI$$Base[];
    extern unsigned int Image$$RW_IRAMx_Heap$$ZI$$Length;
    
    /* APS ZI */
    extern unsigned int Image$$RW_IRAM1$$ZI$$Length;
    extern char Image$$RW_IRAM1$$ZI$$Base[];
    #define ZI_REGION_START     (Image$$RW_IRAM1$$ZI$$Base)
    #define ZI_REGION_LENGTH    ((unsigned int)&Image$$RW_IRAM1$$ZI$$Length)
    
    /* XIP region */
    #define XIP_TEXT            __attribute__((section("XIP_REGION")))
    #define XIP_RODATA          __attribute__((section("XIP_REGION")))
    
#endif



#ifndef OPL2500_XIP
/* If XIP not enabled, no specific section */
#undef XIP_TEXT
#undef XIP_RODATA
#define XIP_TEXT
#define XIP_RODATA
#endif /* OPL2500_XIP */
    
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
