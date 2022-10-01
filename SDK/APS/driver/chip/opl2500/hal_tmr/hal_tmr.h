/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
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
*  hal_tmr.h
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of Timer functions
*
*  Author:
*  -------
*  Luke Liao
******************************************************************************/

#ifndef __HAL_TMR_H__
#define __HAL_TMR_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include "opulinks.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

typedef enum 
{
    TMR_IDX_0 = 0,
    TMR_IDX_1,
    TMR_IDX_NUM,
} E_TmrIdx_t;

typedef enum
{
    TMR_CLK_INT = 0,
    TMR_CLK_EXT = TMR_CTRL_EXT_CLOCK,
} E_TmrClkSel_t;


typedef struct
{
    E_TmrClkSel_t eClkSel;          /* Choose TIMER clock source is from internal (PCLK) or from external clock (XTAL/8) */
    uint32_t u32TmrClkFreq;         /* According to Clock selection, store current frequency */
    uint32_t u32TmrUsMax;           /* Maximum us */
} S_TmrCfg_t;

typedef void (*T_Tmr_CallBack)(E_TmrIdx_t eTmrIdx);
typedef uint32_t (*T_Hal_Tmr_Init)(E_TmrIdx_t eTmrIdx);
typedef uint32_t (*T_Hal_Tmr_Start)(E_TmrIdx_t eTmrIdx, uint32_t u32Us);
typedef uint32_t (*T_Hal_Tmr_Stop)(E_TmrIdx_t eTmrIdx);
typedef uint32_t (*T_Hal_Tmr_RemainUsGet)(E_TmrIdx_t eTmrIdx);
typedef uint32_t (*T_Hal_Tmr_IntClear)(E_TmrIdx_t eTmrIdx);
typedef uint32_t (*T_Hal_Tmr_CallBackFuncSet)(E_TmrIdx_t eTmrIdx, T_Tmr_CallBack tFunc);


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern T_Tmr_CallBack Tmr_CallBack[2];

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
extern T_Hal_Tmr_Init Hal_Tmr_Init;
extern T_Hal_Tmr_Start Hal_Tmr_Start;
extern T_Hal_Tmr_Stop Hal_Tmr_Stop;
extern T_Hal_Tmr_RemainUsGet Hal_Tmr_RemainUsGet;
extern T_Hal_Tmr_IntClear Hal_Tmr_IntClear;
extern T_Hal_Tmr_CallBackFuncSet Hal_Tmr_CallBackFuncSet;

uint32_t Hal_Tmr_Init_impl(E_TmrIdx_t eTmrIdx);
uint32_t Hal_Tmr_Start_impl(E_TmrIdx_t eTmrIdx, uint32_t u32Us);
uint32_t Hal_Tmr_Stop_impl(E_TmrIdx_t eTmrIdx);
uint32_t Hal_Tmr_RemainUsGet_impl(E_TmrIdx_t eTmrIdx);
uint32_t Hal_Tmr_IntClear_impl(E_TmrIdx_t eTmrIdx);
uint32_t Hal_Tmr_CallBackFuncSet_impl(E_TmrIdx_t eTmrIdx, T_Tmr_CallBack tFunc);


#endif
