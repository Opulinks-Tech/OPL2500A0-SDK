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
*  hal_wdt.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of Watchdog functions
*
*  Author:
*  -------
*  Luke Liao
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "opulinks.h"
#include "hal_system.h"
#include "hal_wdt.h"
#include "hal_vic.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/* KEY to unlock/lock the Watchdog registers */
#define WDT_UNLOCK_KEY          0x1ACCE551
#define WDT_LOCK_KEY            0x00000001


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
T_Wdt_CallBack Wdt_CallBack = NULL;

T_Hal_Wdt_Init              Hal_Wdt_Init            = Hal_Wdt_Init_impl;
T_Hal_Wdt_InitForInt        Hal_Wdt_InitForInt      = Hal_Wdt_InitForInt_impl;
T_Hal_Wdt_Start             Hal_Wdt_Start           = Hal_Wdt_Start_impl;
T_Hal_Wdt_Stop              Hal_Wdt_Stop            = Hal_Wdt_Stop_impl;
T_Hal_Wdt_Feed              Hal_Wdt_Feed            = Hal_Wdt_Feed_impl;
T_Hal_Wdt_Clear             Hal_Wdt_Clear           = Hal_Wdt_Clear_impl;
T_Hal_Wdt_UpdateClk         Hal_Wdt_UpdateClk       = Hal_Wdt_UpdateClk_impl;
T_Hal_Wdt_CallBackFuncSet   Hal_Wdt_CallBackFuncSet = Hal_Wdt_CallBackFuncSet_impl;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable
uint32_t g_u32WdtTimeoutMs = 0;  /* unit: ms */
// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Init
*
* DESCRIPTION:
* This function is mainly used to initialize the watchdog hardware.
* It is used to set the WDT to either reset or interrupt modes.
* CALLS
*
* PARAMETERS
*   1. u32Ms: milliseconds to trigger WDT timeout
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Init_impl(uint32_t u32Ms)
{
    // Enable module
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_WDT);
    // Reset module
    Hal_Sys_ApsModuleRst(APS_RST_WDT);

    g_u32WdtTimeoutMs = u32Ms;
    WDT->LOCK = WDT_UNLOCK_KEY;
    WDT->LOAD = (SystemCoreClockGet()/1000) * u32Ms;
    WDT->CTRL = (WDT_CTRL_INTEN | WDT_CTRL_RESEN); // Enable watchdog reset output
    WDT->LOCK = WDT_LOCK_KEY;

    Hal_Vic_IntInit(WDT_IRQn, VIC_TRIG_EDGE_RISING, IRQ_PRIORITY_WDT);
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_InitForInt
*
* DESCRIPTION:
* This function is mainly used to initialize the watchdog hardware.
* It is used to set the WDT to either reset or interrupt modes.
*
* CALLS
*
* PARAMETERS
*   1. u32Ms: milliseconds to trigger WDT timeout
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_InitForInt_impl(uint32_t u32Ms)
{
    // Enable module
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_WDT);
    // Reset module
    Hal_Sys_ApsModuleRst(APS_RST_WDT);

    g_u32WdtTimeoutMs = u32Ms;
    WDT->LOCK = WDT_UNLOCK_KEY;
    WDT->LOAD = (SystemCoreClockGet()/1000) * u32Ms;
    WDT->CTRL = WDT_CTRL_INTEN; // Enable watchdog interrupt mode
    WDT->LOCK = WDT_LOCK_KEY;

    Hal_Vic_IntInit(WDT_IRQn, VIC_TRIG_EDGE_RISING, IRQ_PRIORITY_WDT);
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Start
*
* DESCRIPTION:
* Enable the watchdog timer.
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Start_impl(void)
{
    WDT->LOCK  = WDT_UNLOCK_KEY;
    WDT->CTRL |= WDT_CTRL_INTEN;
    WDT->LOCK  = WDT_LOCK_KEY;
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Stop
*
* DESCRIPTION:
* Disable the watchdog timer.
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Stop_impl(void)
{
    WDT->LOCK = WDT_UNLOCK_KEY;
    WDT->CTRL &= ~(WDT_CTRL_RESEN | WDT_CTRL_INTEN);
    WDT->LOCK = WDT_LOCK_KEY;
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Feed
*
* DESCRIPTION:
* This function is used to feed the watchdog (restart the watchdog timer)
* and the count is immediately restarted from the new value.
* To avoid the WDT from expiring, this function should be called regularly.
*
* CALLS
*
* PARAMETERS
*   1. u32Ms: milliseconds to trigger WDT timeout
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Feed_impl(uint32_t u32Ms)
{
    g_u32WdtTimeoutMs = u32Ms;
    WDT->LOCK = WDT_UNLOCK_KEY;
    WDT->LOAD = (SystemCoreClockGet()/1000) * u32Ms;
    WDT->LOCK = WDT_LOCK_KEY;
}

/*************************************************************************
* FUNCTION:
*  Hal_Wdt_Clear
*
* DESCRIPTION:
* This function is used to clear watchdog interrupt & reload conter
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Wdt_Clear_impl(void)
{
    WDT->LOCK = WDT_UNLOCK_KEY;
    /* write any value to clear wdt interrupt & reload wdt counter */
    WDT->INTCLR = 0x94539453;
    WDT->LOCK = WDT_LOCK_KEY;
}


/**
 * @brief Hal_Wdt_UpdateClk
 *          Update WDT reload counter when HCLK changed
 */
void Hal_Wdt_UpdateClk_impl(void)
{
    if (g_u32WdtTimeoutMs > 0)
        Hal_Wdt_Feed(g_u32WdtTimeoutMs);
}

/**
 * @brief Hal_Wdt_CallBackFuncSet
 *          Set WDT callback fucntion
 */
uint32_t Hal_Wdt_CallBackFuncSet_impl(T_Wdt_CallBack tFunc)
{
    Wdt_CallBack = tFunc;
    return RESULT_SUCCESS;
};

