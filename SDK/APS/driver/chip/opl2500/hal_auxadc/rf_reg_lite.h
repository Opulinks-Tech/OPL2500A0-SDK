/******************************************************************************
*  Copyright 2017 - 2021, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2021
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  rf_reg.h
*
*  Project:
*  --------
*  NL2X00 series
*
*  Description:
*  ------------
*  This include file defines the registers of RF .
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

#ifndef __RF_REG_H__
#define __RF_REG_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// 0x48
#define CK_GATE_AUXADC_EN          (0x1 << 3)

// 0x5C
#define AUX_IN_PIN_01_SEL_POS              6
#define AUX_IN_PIN_01_SEL_MSK              (0x1 << AUX_IN_PIN_01_SEL_POS)
#define AUX_IN_PIN_01_SEL_FROM_GPIO        (0 << AUX_IN_PIN_01_SEL_POS)
#define AUX_IN_PIN_01_SEL_FROM_TEST_MUX    (1 << AUX_IN_PIN_01_SEL_POS) 

// 0x64
#define AUX_IN_SEL_POS              0
#define AUX_IN_SEL_MSK              (0xF << AUX_IN_SEL_POS)

// 0xCC
#define RC_CONVSTB_EN         (0x1 << 0)
#define RG_PU_AUXADC          (0x1 << 12)

// 0xD0
#define AUX_ADC_ECL_OUT_POS     0
#define AUX_ADC_ECL_OUT_MSK     (0x3FF << AUX_ADC_ECL_OUT_POS)
#define AUX_EOCB_FINISH         (0x1 << 16)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

#endif
