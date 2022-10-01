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
*  hal_vic.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of vic functions
*
*  Author:
*  -------
*  FW Team
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <string.h>
#include "opulinks.h"
#include "hal_vic.h"
#include "hal_gpio.h"
#include "core_cm4.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
S_NVIC_STORAGE g_Hal_VicStorage = {0,};

// VIC moudle
T_Hal_Vic_IntInit       Hal_Vic_IntInit         = Hal_Vic_IntInit_impl;
T_Hal_Vic_IntDeInit     Hal_Vic_IntDeInit       = Hal_Vic_IntDeInit_impl;
T_Hal_Vic_IntEn         Hal_Vic_IntEn           = Hal_Vic_IntEn_impl;
T_Hal_Vic_IntClear      Hal_Vic_IntClear        = Hal_Vic_IntClear_impl;
T_Hal_Vic_IntMask       Hal_Vic_IntMask         = Hal_Vic_IntMask_impl;
T_Hal_Vic_IntTypeSel    Hal_Vic_IntTypeSel      = Hal_Vic_IntTypeSel_impl;
T_Hal_Vic_IntInv        Hal_Vic_IntInv          = Hal_Vic_IntInv_impl;
T_Hal_Vic_IntStatRead   Hal_Vic_IntStatRead     = Hal_Vic_IntStatRead_impl;
T_Hal_Vic_SleepStore    Hal_Vic_SleepStore      = Hal_Vic_SleepStore_impl;
T_Hal_Vic_WakeupResume  Hal_Vic_WakeupResume    = Hal_Vic_WakeupResume_impl;

// IPC module
T_Hal_Vic_IpcIntTrig    Hal_Vic_IpcIntTrig      = Hal_Vic_IpcIntTrig_impl;
T_Hal_Vic_IpcIntEn      Hal_Vic_IpcIntEn        = Hal_Vic_IpcIntEn_impl;
T_Hal_Vic_IpcIntClear   Hal_Vic_IpcIntClear     = Hal_Vic_IpcIntClear_impl;


// MmFactor relative
T_Hal_Vic_MmFactorResumeByIpcIntEn        Hal_Vic_MmFactorResumeByIpcIntEn          = Hal_Vic_MmFactorResumeByIpcIntEn_impl;
T_Hal_Vic_MmFactorResumeByIpcIntStatRead  Hal_Vic_MmFactorResumeByIpcIntStatRead    = Hal_Vic_MmFactorResumeByIpcIntStatRead_impl;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable
const IRQn_Type g_ecaVicIpcIrqs[IRQn_MAX] = {IPC0_IRQn, IPC1_IRQn, IPC2_IRQn, IPC3_IRQn};

// Sec 7: declaration of static function prototype

/***********
C Functions
***********/
// Sec 8: C Functions

/*********** VIC interrupt **************************/

/**
 * @brief Hal_Vic_IntInit
 *        VIC Interrupt initialization, including NVIC setup
 * 
 * @param eIrqIdx [in] Interrupt number, ::IRQn_Type
 * @param eTrigType [in] Trig Type of interrupt, ::E_VIC_INT_TRIG_TYPE                      \n
 *                       ::VIC_TRIG_LEVEL_HIGH   - High level trigger interrupt             \n
 *                       ::VIC_TRIG_LEVEL_LOW    - Low level trigger interrupt              \n
 *                       ::VIC_TRIG_EDGE_RISING  - Rising edge trigger intterupt            \n
 *                       ::VIC_TRIG_EDGE_FALLING - Falling edge trigger interrupt           \n
 *                       ::VIC_TRIG_EDGE_BOTH    - Rising or falling edge trigger interrupt \n
 * @param u32IrqPriority [in] NVIC priority
 * @return None
 */
void Hal_Vic_IntInit_impl(IRQn_Type eIrqIdx, E_VIC_INT_TRIG_TYPE eTrigType, uint32_t u32IrqPriority)
{
    S_VIC_TRIG_CTRL *psTrigCtrl = (S_VIC_TRIG_CTRL *)&eTrigType;
    uint32_t u32TypeBitIdx;

    /* Masked interrupt first */
    BIT_SET(VIC->CTRL5_INT_MSK, eIrqIdx, 1);
    
    /* Inverter */
    BIT_SET(VIC->CTRL0_INT_INV, eIrqIdx, psTrigCtrl->u8InvEn);
    
    /* Disable */
    BIT_SET(VIC->CTRL1_INT_DIS, eIrqIdx, 0);
    
    /* Interrupt Type */
    u32TypeBitIdx = VIC_INT_IDX_TO_TYPE_BIT_IDX(eIrqIdx);
    if (eIrqIdx >= VIC_INT_TYPE1_IRQ_START)
        VIC_SET_INT_TYPE_BITS(VIC->CTRL3_INT_TYPE1, u32TypeBitIdx, psTrigCtrl->u8Type);
    else
        VIC_SET_INT_TYPE_BITS(VIC->CTRL2_INT_TYPE0, u32TypeBitIdx, psTrigCtrl->u8Type);
    
    /* Clear first time */
    VIC->CTRL4_INT_CLR = 1 << eIrqIdx;
    
    /* Mask enable */
    BIT_SET(VIC->CTRL5_INT_MSK, eIrqIdx, 0);
    
    /* Clear NVIC */
    NVIC_ClearPendingIRQ(eIrqIdx);
    
    /* Set prority */
    NVIC_SetPriority(eIrqIdx, u32IrqPriority);
    
    /* Enable/Disable NVIC */
    NVIC_EnableIRQ(eIrqIdx);
}


/**
 * @brief Hal_Vic_IntDeInit
 *        VIC Interrupt de-initialization, including NVIC disable
 * 
 * @param eIrqIdx [in] Interrupt number, ::IRQn_Type
 * @return None
 */
void Hal_Vic_IntDeInit_impl(IRQn_Type eIrqIdx)
{
    uint32_t u32TypeBitIdx;
    
    /* Disable NVIC */
    NVIC_DisableIRQ(eIrqIdx);
    
    /* Clear NVIC */
    NVIC_ClearPendingIRQ(eIrqIdx);
    
    /* Masked interrupt first */
    BIT_SET(VIC->CTRL5_INT_MSK, eIrqIdx, 1);
    
    /* Interrupt Type */
    u32TypeBitIdx = VIC_INT_IDX_TO_TYPE_BIT_IDX(eIrqIdx);
    if (eIrqIdx >= VIC_INT_TYPE1_IRQ_START)
        VIC_SET_INT_TYPE_BITS(VIC->CTRL3_INT_TYPE1, u32TypeBitIdx, 0);
    else
        VIC_SET_INT_TYPE_BITS(VIC->CTRL2_INT_TYPE0, u32TypeBitIdx, 0);
    
    /* Disable */
    BIT_SET(VIC->CTRL1_INT_DIS, eIrqIdx, 1);
    
    /* Inverter */
    BIT_SET(VIC->CTRL0_INT_INV, eIrqIdx, 0);
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_IntEn
*
* DESCRIPTION:
*   1. Enable or disable interrupt signal
* CALLS
*
* PARAMETERS
*   1. eInterrupt: Index of interrupt. refert to IRQn_Type
*   2. u8Enable  : 1 for Enable/0 for Disable
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_IntEn_impl(IRQn_Type eIrqIdx, uint8_t u8Enable)
{
    if (eIrqIdx >= IRQn_MAX)
        return;
    
    // Note: regster function is "disable"
    BIT_SET(VIC->CTRL1_INT_DIS, eIrqIdx, (u8Enable == ENABLE)?0:1);
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_IntClear
*
* DESCRIPTION:
*   1. Clear interrupt
* CALLS
*
* PARAMETERS
*   1. eInterrupt: Index of interrupt. refert to IRQn_Type
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_IntClear_impl(IRQn_Type eIrqIdx)
{
    if (eIrqIdx >= IRQn_MAX)
        return;
    
    VIC->CTRL4_INT_CLR = (1 << eIrqIdx);
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_IntMask
*
* DESCRIPTION:
*   1. Mask interrupt
* CALLS
*
* PARAMETERS
*   1. eInterrupt: Index of interrupt. refert to IRQn_Type
*   2. u8Enable  : 1 for Enable/0 for Disable
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_IntMask_impl(IRQn_Type eIrqIdx, uint8_t u8Enable)
{
    if (eIrqIdx >= IRQn_MAX)
        return;
    
    BIT_SET(VIC->CTRL5_INT_MSK, eIrqIdx, (u8Enable == ENABLE)?1:0);
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_IntTypeSel
*
* DESCRIPTION:
*   1. Select interrupt type
* CALLS
*
* PARAMETERS
*   1. eInterrupt: Index of interrupt. refert to IRQn_Type
*   2. eType     : Type of interrupt. refert to E_ItrType_t
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_IntTypeSel_impl(IRQn_Type eIrqIdx, E_ItrType_t eType)
{
    uint32_t u32TypeBitIdx;

    if (eIrqIdx >= IRQn_MAX)
        return;

    u32TypeBitIdx = VIC_INT_IDX_TO_TYPE_BIT_IDX(eIrqIdx);
    if (eIrqIdx >= VIC_INT_TYPE1_IRQ_START)
        VIC_SET_INT_TYPE_BITS(VIC->CTRL3_INT_TYPE1, u32TypeBitIdx, eType);
    else
        VIC_SET_INT_TYPE_BITS(VIC->CTRL2_INT_TYPE0, u32TypeBitIdx, eType);
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_IntInv
*
* DESCRIPTION:
*   1. Invert interrupt signal
* CALLS
*
* PARAMETERS
*   1. eIrqIdx: Index of interrupt. refert to IRQn_Type
*   2. u8Enable  : 1 for Enable/0 for Disable
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_IntInv_impl(IRQn_Type eIrqIdx, uint8_t u8Enable)
{
    if (eIrqIdx >= IRQn_MAX)
        return;
    
    BIT_SET(VIC->CTRL0_INT_INV, eIrqIdx, (u8Enable == ENABLE)?1:0);
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_IntStatRead
*
* DESCRIPTION:
*   1. Return interrupt status
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   interrupt status, each bit index refer to IRQn_Type
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Vic_IntStatRead_impl(void)
{
    return VIC->CTRL6_INT_STS;
}


/*************************************************************************
* FUNCTION:
*  Hal_Vic_SleepStore
*
* DESCRIPTION:
*   1. Store NVIC setting before entering sleep
* CALLS
*
* PARAMETERS
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_SleepStore_impl(void)
{
    g_Hal_VicStorage.u32IrqEn = NVIC->ISER[0];
    memcpy(g_Hal_VicStorage.u8aIrqPri, (void *)NVIC->IP, sizeof(g_Hal_VicStorage.u8aIrqPri));
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_WakeupResume
*
* DESCRIPTION:
*   1. When wakeup, resume NVIC setting
* CALLS
*
* PARAMETERS
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_WakeupResume_impl(void)
{
    NVIC->ISER[0] = g_Hal_VicStorage.u32IrqEn;
    NVIC->ICER[0] = ~g_Hal_VicStorage.u32IrqEn;
    memcpy((void *)NVIC->IP, g_Hal_VicStorage.u8aIrqPri, sizeof(g_Hal_VicStorage.u8aIrqPri));
}


/*********** IPC interrupt **************************/
/*************************************************************************
* FUNCTION:
*  Hal_Vic_IpcIntEn
*
* DESCRIPTION:
*   1. Enable IPC interrupt
* CALLS
*
* PARAMETERS
*   1. eIpc    : Index of IPC. refert to E_IpcIdx_t
*   2. u32Priority: The IPC interrupt priority
*   3. u8Enable: ENABLE/DISABLE
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_IpcIntEn_impl(E_IpcIdx_t eIpc, uint32_t u32Priority, uint8_t u8Enable)
{
    if (eIpc >= IPC_IDX_MAX)
        return;
    
    if (u8Enable == ENABLE)
        Hal_Vic_IntInit(g_ecaVicIpcIrqs[eIpc], VIC_TRIG_LEVEL_HIGH, u32Priority);
    else
        Hal_Vic_IntDeInit(g_ecaVicIpcIrqs[eIpc]);
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_IpcIntTrig
*
* DESCRIPTION:
*   1. Trigger the IPC interrupt
* CALLS
*
* PARAMETERS
*   1. eIpc    : Index of IPC. refert to E_IpcIdx_t
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_IpcIntTrig_impl(E_IpcIdx_t eIpc)
{
    if (eIpc >= IPC_IDX_MAX)
        return;
    
    VIC->IPC_INT = (1 << eIpc);
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_IpcIntClear
*
* DESCRIPTION:
*   1. Clear the IPC interrupt
* CALLS
*
* PARAMETERS
*   1. eIpc    : Index of IPC. refert to E_IpcIdx_t
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_IpcIntClear_impl(E_IpcIdx_t eIpc)
{
    if (eIpc >= IPC_IDX_MAX)
        return;
 
    // Clear VIC part
    Hal_Vic_IntClear(g_ecaVicIpcIrqs[eIpc]);
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_IpcIntOtherCoreStatRead
*
* DESCRIPTION:
*   1. Get the IPC interrupt status of the other core
* CALLS
*
* PARAMETERS
*   None 
*
* RETURNS
*   IPC interrupt status.  bit index refert to E_IpcIdx_t
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Vic_IpcIntOtherCoreStatRead_impl(void)
{
    return (VIC->IPC_INT & VIC_IPC_INT_IPC_STATUS_Msk) >> VIC_IPC_INT_IPC_STATUS_Pos;
}



/*************************************************************************
* FUNCTION:
*  Hal_Vic_MmFactorResumeByIpcIntEn
*
* DESCRIPTION:
*   1. ipc interrupt notify clock modulator at full speed enable
*      Used with Hal_Sys_ApsClkMmFactorSet
* CALLS
*
* PARAMETERS
*   1. eIpc       : Index of IPC. refert to E_IpcIdx_t
*   2. u8Enable : 1 for enable/ 0 for disable
*
* RETURNS
*   None
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Vic_MmFactorResumeByIpcIntEn_impl(E_IpcIdx_t eIpc, uint8_t u8Enable)
{
    if (eIpc >= IPC_IDX_MAX)
        return;
    
    BIT_SET(VIC->CTRLF_IINT_MMF, eIpc, (u8Enable == ENABLE)?1:0);
}

/*************************************************************************
* FUNCTION:
*  Hal_Vic_MmFactorResumeByIpcIntStatRead
*
* DESCRIPTION:
*   1. Which ipc interrupt will notify clock modulator at full speed
*      Used with Hal_Sys_ApsClkMmFactorSet
* CALLS
*
* PARAMETERS
*  None
* RETURNS
*  Index of IPC. refert to E_IpcIdx_t
* GLOBALS AFFECTED
* 
*************************************************************************/

uint32_t Hal_Vic_MmFactorResumeByIpcIntStatRead_impl(void)
{
    return VIC->CTRLF_IINT_MMF;
}
