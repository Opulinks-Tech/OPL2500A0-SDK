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
*  hal_int.h
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
*  Luke Liao
******************************************************************************/

#ifndef __HAL_VIC_H__
#define __HAL_VIC_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>
#include "opulinks.h"
// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

#define VIC_TRIG_INV_EN_OFST        0
#define VIC_TRIG_TYPE_OFST          8

#define VIC_INT_TYPE1_IRQ_START                     16
#define VIC_INT_IDX_TO_TYPE_BIT_IDX(irq)            (((irq) * 2) & 0x1F)
#define VIC_SET_INT_TYPE_BITS(reg, bit, value)      ((reg) = ((reg) & ~(3UL << (bit))) | ((value) & 3) << (bit))


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef enum 
{
    IPC_IDX_0 = 0x00,
    IPC_IDX_1,
    IPC_IDX_2,
    IPC_IDX_3,
    IPC_IDX_MAX
} E_IpcIdx_t;

typedef enum 
{
    INT_TYPE_LEVEL        = 0x0,
    INT_TYPE_RISING_EDGE  = 0x1,
    INT_TYPE_FALLING_EDGE = 0x2,
    INT_TYPE_BOTH_EDGE    = 0x3
} E_ItrType_t;

typedef enum
{
    VIC_TRIG_LEVEL_HIGH     = (0 << VIC_TRIG_INV_EN_OFST) | (INT_TYPE_LEVEL         << VIC_TRIG_TYPE_OFST),
    VIC_TRIG_LEVEL_LOW      = (1 << VIC_TRIG_INV_EN_OFST) | (INT_TYPE_LEVEL         << VIC_TRIG_TYPE_OFST),
    VIC_TRIG_EDGE_RISING    = (0 << VIC_TRIG_INV_EN_OFST) | (INT_TYPE_RISING_EDGE   << VIC_TRIG_TYPE_OFST),
    VIC_TRIG_EDGE_FALLING   = (0 << VIC_TRIG_INV_EN_OFST) | (INT_TYPE_FALLING_EDGE  << VIC_TRIG_TYPE_OFST),
    VIC_TRIG_EDGE_BOTH      = (0 << VIC_TRIG_INV_EN_OFST) | (INT_TYPE_BOTH_EDGE     << VIC_TRIG_TYPE_OFST),
} E_VIC_INT_TRIG_TYPE;

typedef struct
{
    uint8_t u8InvEn;
    uint8_t u8Type;
} S_VIC_TRIG_CTRL;

/* Store NVIC configuration before entering sleep. APS has IRQn_MAX interrupts */
typedef struct
{
    uint32_t u32IrqEn;              /* Interrupt enable status. 
                                     * Apply to ISER[0] and ICER[0]
                                     * Set enable to ISER and disable to ICER */
    uint8_t u8aIrqPri[IRQn_MAX];    /* Interrupt priority.
                                     * Apply to IP[0] ~ IP[IRQn_MAX-1] */
} S_NVIC_STORAGE;

// VIC moudle
typedef void (*T_Hal_Vic_IntInit)(IRQn_Type eIrqIdx, E_VIC_INT_TRIG_TYPE eTrigType, uint32_t u32IrqPriority);
typedef void (*T_Hal_Vic_IntDeInit)(IRQn_Type eIrqIdx);
typedef void (*T_Hal_Vic_IntEn)(IRQn_Type eIrqIdx, uint8_t u8Enable);
typedef void (*T_Hal_Vic_IntClear)(IRQn_Type eIrqIdx);
typedef void (*T_Hal_Vic_IntMask)(IRQn_Type eIrqIdx, uint8_t u8Enable);
typedef void (*T_Hal_Vic_IntTypeSel)(IRQn_Type eIrqIdx, E_ItrType_t eType);
typedef void (*T_Hal_Vic_IntInv)(IRQn_Type eIrqIdx, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Vic_IntStatRead)(void);
typedef void (*T_Hal_Vic_SleepStore)(void);
typedef void (*T_Hal_Vic_WakeupResume)(void);

// IPC module
typedef void (*T_Hal_Vic_IpcIntTrig)(E_IpcIdx_t eIpc);
typedef void (*T_Hal_Vic_IpcIntEn)(E_IpcIdx_t eIpc, uint32_t u32Priority, uint8_t u8Enable);
typedef void (*T_Hal_Vic_IpcIntClear)(E_IpcIdx_t eIpc);

// MmFactor relative
typedef void (*T_Hal_Vic_MmFactorResumeByIpcIntEn)(E_IpcIdx_t eIpc, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Vic_MmFactorResumeByIpcIntStatRead)(void);

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
// VIC moudle
extern T_Hal_Vic_IntInit        Hal_Vic_IntInit;
extern T_Hal_Vic_IntDeInit      Hal_Vic_IntDeInit;
extern T_Hal_Vic_IntEn          Hal_Vic_IntEn;
extern T_Hal_Vic_IntClear       Hal_Vic_IntClear;
extern T_Hal_Vic_IntMask        Hal_Vic_IntMask;
extern T_Hal_Vic_IntTypeSel     Hal_Vic_IntTypeSel;
extern T_Hal_Vic_IntInv         Hal_Vic_IntInv;
extern T_Hal_Vic_IntStatRead    Hal_Vic_IntStatRead;
extern T_Hal_Vic_SleepStore     Hal_Vic_SleepStore;
extern T_Hal_Vic_WakeupResume   Hal_Vic_WakeupResume;

// IPC module
extern T_Hal_Vic_IpcIntTrig  Hal_Vic_IpcIntTrig;
extern T_Hal_Vic_IpcIntEn    Hal_Vic_IpcIntEn;
extern T_Hal_Vic_IpcIntClear Hal_Vic_IpcIntClear;

// MmFactor relative
extern T_Hal_Vic_MmFactorResumeByIpcIntEn        Hal_Vic_MmFactorResumeByIpcIntEn;
extern T_Hal_Vic_MmFactorResumeByIpcIntStatRead  Hal_Vic_MmFactorResumeByIpcIntStatRead;



// VIC moudle
void Hal_Vic_IntInit_impl(IRQn_Type eIrqIdx, E_VIC_INT_TRIG_TYPE eTrigType, uint32_t u32IrqPriority);
void Hal_Vic_IntDeInit_impl(IRQn_Type eIrqIdx);
void Hal_Vic_IntEn_impl(IRQn_Type eIrqIdx, uint8_t u8Enable);
void Hal_Vic_IntClear_impl(IRQn_Type eIrqIdx);
void Hal_Vic_IntMask_impl(IRQn_Type eIrqIdx, uint8_t u8Enable);
void Hal_Vic_IntTypeSel_impl(IRQn_Type eIrqIdx, E_ItrType_t eType);
void Hal_Vic_IntInv_impl(IRQn_Type eIrqIdx, uint8_t u8Enable);
uint32_t Hal_Vic_IntStatRead_impl(void);
void Hal_Vic_SleepStore_impl(void);
void Hal_Vic_WakeupResume_impl(void);

// IPC module
void Hal_Vic_IpcIntEn_impl(E_IpcIdx_t eIpc, uint32_t u32Priority, uint8_t u8Enable);
void Hal_Vic_IpcIntTrig_impl(E_IpcIdx_t eIpc);
void Hal_Vic_IpcIntClear_impl(E_IpcIdx_t eIpc);
uint32_t Hal_Vic_IpcIntOtherCoreStatRead_impl(void);


// MmFactor relative
void Hal_Vic_MmFactorResumeByIpcIntEn_impl(E_IpcIdx_t eIpc, uint8_t u8Enable);
uint32_t Hal_Vic_MmFactorResumeByIpcIntStatRead_impl(void);

#include "hal_vic_patch.h"
#endif
