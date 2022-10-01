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
*  hal_tmr.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of Timer functions
*
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "opulinks.h"
#include "hal_sys_rcc.h"
#include "hal_tmr.h"
#include "hal_vic.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


#define TIMER_MAX_VALUE     0xFFFFFFFF


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable

T_Tmr_CallBack Tmr_CallBack[2] = {NULL, NULL};
S_TmrCfg_t g_Tmr_saTmrCfg[2] = {{TMR_CLK_EXT, 0, 0}, {TMR_CLK_EXT, 0, 0}};
S_TMR_Reg_t *g_Tmr_psaRegs[2] = {TMR0, TMR1};

T_Hal_Tmr_Init              Hal_Tmr_Init            = Hal_Tmr_Init_impl;
T_Hal_Tmr_Start             Hal_Tmr_Start           = Hal_Tmr_Start_impl;
T_Hal_Tmr_Stop              Hal_Tmr_Stop            = Hal_Tmr_Stop_impl;
T_Hal_Tmr_RemainUsGet       Hal_Tmr_RemainUsGet     = Hal_Tmr_RemainUsGet_impl;
T_Hal_Tmr_IntClear          Hal_Tmr_IntClear        = Hal_Tmr_IntClear_impl;
T_Hal_Tmr_CallBackFuncSet   Hal_Tmr_CallBackFuncSet = Hal_Tmr_CallBackFuncSet_impl;

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


/**
 * @brief Hal_Tmr_Init
 *        This function is mainly used to initialize the Timer0 or Timer1 hardware.
 * @param eTmrIdx[in] Timer index, TMR_IDX_0 or TMR_IDX_1
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Tmr_Init_impl(E_TmrIdx_t eTmrIdx)
{
    S_TMR_Reg_t *psTmr = (S_TMR_Reg_t *)eTmrIdx;
    S_TmrCfg_t *psTmrCfg;
    IRQn_Type eIrq = IRQn_MAX;
    uint32_t u32TmrClk;
    uint32_t u32IrqPriorty = 0xFF;
    uint64_t u64MaxUsBuff = 0;

    switch (eTmrIdx)
    {
        case TMR_IDX_0:
            Hal_Sys_ApsModuleRst(APS_RST_TIMER_0);
            Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_TIMER_0);

            eIrq = TMR0_IRQn;
            u32IrqPriorty = IRQ_PRIORITY_TMR0;
            psTmrCfg = &g_Tmr_saTmrCfg[0];
            if (psTmrCfg->eClkSel == TMR_CLK_INT)
                u32TmrClk = SystemCoreClockGet();
            else
                Hal_Sys_ApsClockGet(APS_CLK_GRP_TMR0_EXT, &u32TmrClk);
            break;
        case TMR_IDX_1:
            Hal_Sys_ApsModuleRst(APS_RST_TIMER_1);
            Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_TIMER_1);

            eIrq = TMR1_IRQn;
            u32IrqPriorty = IRQ_PRIORITY_TMR1;
            psTmrCfg = &g_Tmr_saTmrCfg[1];
            if (psTmrCfg->eClkSel == TMR_CLK_INT)
                u32TmrClk = SystemCoreClockGet();
            else
                Hal_Sys_ApsClockGet(APS_CLK_GRP_TMR1_EXT, &u32TmrClk);
            break;
        default:
            return RESULT_FAIL;
    }
    
    // Min:1 MHz for API
    if( u32TmrClk < CLK_US )
        return RESULT_FAIL;
    
    psTmrCfg->u32TmrClkFreq = u32TmrClk;
    // Truncate, avoid ticks over-flow
    u64MaxUsBuff = ( (uint64_t)TIMER_MAX_VALUE * CLK_US / psTmrCfg->u32TmrClkFreq );
    psTmrCfg->u32TmrUsMax = (uint32_t)u64MaxUsBuff;
    
    psTmr = g_Tmr_psaRegs[eTmrIdx];
    
    // Clean module parts
    psTmr->INTSTATUS = 1;

    Hal_Vic_IntInit(eIrq, VIC_TRIG_LEVEL_HIGH, u32IrqPriorty);
    
    return RESULT_SUCCESS;
}


/**
 * @brief Hal_Tmr_Start
 *        This function is mainly used to start the Timer0 or Timer1 hardware.
 * @param eTmrIdx[in] Timer index, TMR_IDX_0 or TMR_IDX_1
 * @param u32Us[in] timer reload value in us
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Tmr_Start_impl(E_TmrIdx_t eTmrIdx, uint32_t u32Us)
{
    S_TMR_Reg_t *psTmr;
    S_TmrCfg_t *psTmrCfg;
    uint32_t reload = 0;
    uint64_t u64MaxUsBuff = 0;

    switch (eTmrIdx)
    {
        case TMR_IDX_0:
            psTmrCfg = &g_Tmr_saTmrCfg[0];
            break;
        case TMR_IDX_1:
            psTmrCfg = &g_Tmr_saTmrCfg[1];
            break;
        default:
            return RESULT_FAIL;
    }
    psTmr = g_Tmr_psaRegs[eTmrIdx];
    
    // error handle
    if (u32Us == 0)
        return RESULT_FAIL;

    if (u32Us >= psTmrCfg->u32TmrUsMax)
    {
        reload = TIMER_MAX_VALUE;
    }
    else
    {
        u64MaxUsBuff = ( (uint64_t)u32Us * psTmrCfg->u32TmrClkFreq + CLK_US/2 ) / CLK_US;
        reload = (uint32_t)u64MaxUsBuff;
    }

    psTmr->CTRL = 0;
    psTmr->VALUE = 0;
    psTmr->LOAD = reload;
    psTmr->CTRL |= (TMR_CTRL_ENABLE | TMR_CTRL_INT_ENABLE | psTmrCfg->eClkSel);
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Tmr_Stop
 *        This function is mainly used to stop the Timer0 or Timer1 hardware.
 * @param eTmrIdx[in] Timer index, TMR_IDX_0 or TMR_IDX_1
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Tmr_Stop_impl(E_TmrIdx_t eTmrIdx)
{
    if (eTmrIdx >= TMR_IDX_NUM)
        return RESULT_FAIL;
    
    g_Tmr_psaRegs[eTmrIdx]->CTRL = 0;
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Tmr_RemainUsGet
 *        This function is mainly used to read the Timer0 or Timer1 remaining us.
 * @param eTmrIdx[in] Timer index, TMR_IDX_0 or TMR_IDX_1
 * @return Remaining us
 */
uint32_t Hal_Tmr_RemainUsGet_impl(E_TmrIdx_t eTmrIdx)
{
    S_TmrCfg_t *psTmrCfg;
    S_TMR_Reg_t *psTmr;
    uint64_t u64MaxUsBuff = 0;
    
    switch (eTmrIdx)
    {
        case TMR_IDX_0:
            psTmrCfg = &g_Tmr_saTmrCfg[0];
            break;
        case TMR_IDX_1:
            psTmrCfg = &g_Tmr_saTmrCfg[1];
            break;
        default:
            return 0;
    }
    psTmr = g_Tmr_psaRegs[eTmrIdx];
    
    u64MaxUsBuff = ( (uint64_t)psTmr->VALUE * psTmrCfg->u32TmrClkFreq + CLK_US/2 ) / CLK_US;
    return (uint64_t)u64MaxUsBuff;
}

/**
 * @brief Hal_Tmr_IntClear
 *        This function is mainly used to clear the Timer0 or Timer1 interrupt.
 * @param eTmrIdx[in] Timer index, TMR_IDX_0 or TMR_IDX_1
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Tmr_IntClear_impl(E_TmrIdx_t eTmrIdx)
{
    if (eTmrIdx >= TMR_IDX_NUM)
        return RESULT_FAIL;
    
    g_Tmr_psaRegs[eTmrIdx]->INTSTATUS = 1;
    return RESULT_SUCCESS;
}

/*************************************************************************
* FUNCTION:
*  Hal_Tmr_CallBackFuncSet
*
* DESCRIPTION:
*   1. Setup timer callback function
*
* CALLS
*
* PARAMETERS
*   1. u32Timer: timer number 0 or 1
*   2. tFunc: Callback function
*
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/

/**
 * @brief Hal_Tmr_Start
 *        This function is mainly used to start the Timer0 or Timer1 hardware.
 * @param eTmrIdx[in] Timer index, TMR_IDX_0 or TMR_IDX_1
 * @param u32Us[in] timer reload value in us
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Tmr_CallBackFuncSet_impl(E_TmrIdx_t eTmrIdx, T_Tmr_CallBack tFunc)
{
    switch (eTmrIdx)
    {
        case TMR_IDX_0:
            Tmr_CallBack[0] = tFunc;
            break;
        case TMR_IDX_1:
            Tmr_CallBack[1] = tFunc;
            break;
        default:
            return RESULT_FAIL;
    }
    return RESULT_SUCCESS;
}

