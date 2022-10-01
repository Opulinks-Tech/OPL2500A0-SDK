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
*  os_partition_pool.h
*
*  Project:
*  --------
*  OPL2500 Project - the partition pool definition file
*
*  Description:
*  ------------
*  This include file is the partition pool definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _OS_PARTITION_POOL_H_
#define _OS_PARTITION_POOL_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file
#ifdef __PMP_ENABLE__


// Sec 1: Include File
#include "FreeRTOS.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
typedef struct PP_PCB_STRUCT
{
    void *pvStartAddr;
    uint32_t ulPoolSize;
    uint32_t ulPartitionSize;
    uint32_t ulBlockNum;
    uint32_t ulAllocatedCount;
    uint32_t ulMaxCount;
    struct PP_HEADER_STRUCT *pxAvailableList;
} PP_PCB_t;

typedef struct PP_HEADER_STRUCT
{
    uint32_t ulMagicNum;        // used to distinguish the fixed or the variable
    struct PP_HEADER_STRUCT *pxNextAvailables;
    PP_PCB_t *pxPartitionCtrlBlock;
    uint32_t ulWantedSize;
    uint32_t ulInstanceId;
} PP_HEADER_t;

#define PP_OVERHEAD     sizeof(PP_HEADER_t)
#define PP_MAGIC_NUM    0x54847124

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype

//BaseType_t xPartitionPoolCreate(PP_PCB_t *pxPCB, void *pvStartAddr, uint32_t ulPoolSize, uint32_t ulPartitionSize);
//void *pvPartitionPoolAllocate(PP_PCB_t *pxPCB, uint32_t ulWantedSize, uint32_t ulInstanceId);
//void vPartitionPoolDeallocate(void *pvMemPtr);
//void vPartitionPoolBlockInfo(PP_PCB_t *pxPCB);

typedef BaseType_t (*T_xPartitionPoolCreateFp)(PP_PCB_t *pxPCB, void *pvStartAddr, uint32_t ulPoolSize, uint32_t ulPartitionSize);
typedef void* (*T_pvPartitionPoolAllocateFp)(PP_PCB_t *pxPCB, uint32_t ulWantedSize, uint32_t ulInstanceId);
typedef void (*T_vPartitionPoolDeallocateFp)(void *pvMemPtr);
typedef void (*T_vPartitionPoolBlockInfoFp)(PP_PCB_t *pxPCB);
typedef void (*vPartitionPoolReachThresholdHook_fp)(void);
typedef void (*T_vPartitionPoolDetectTimerFunction)(void const *argument);
typedef void (*T_vPartitionPoolDetectStart)(uint32_t u32Threshold, uint32_t u32PeriodMs, vPartitionPoolReachThresholdHook_fp pFunc);
typedef void (*T_vPartitionPoolDetectStop)(void);

extern T_xPartitionPoolCreateFp xPartitionPoolCreate;
extern T_pvPartitionPoolAllocateFp pvPartitionPoolAllocate;
extern T_vPartitionPoolDeallocateFp vPartitionPoolDeallocate;
extern T_vPartitionPoolBlockInfoFp vPartitionPoolBlockInfo;
extern T_vPartitionPoolDetectTimerFunction vPartitionPoolDetectTimerFunction;
extern T_vPartitionPoolDetectStart vPartitionPoolDetectStart;
extern T_vPartitionPoolDetectStop vPartitionPoolDetectStop;

void vPartitionPoolDetectTimerFunction_impl(void const *argument);
void vPartitionPoolDetectStart_impl(uint32_t u32Threshold, uint32_t u32PeriodMs, vPartitionPoolReachThresholdHook_fp pFunc);
void vPartitionPoolDetectStop_impl(void);

BaseType_t xPartitionPoolCreate_impl(PP_PCB_t *pxPCB, void *pvStartAddr, uint32_t ulPoolSize, uint32_t ulPartitionSize);
void *pvPartitionPoolAllocate_impl(PP_PCB_t *pxPCB, uint32_t ulWantedSize, uint32_t ulInstanceId);
void vPartitionPoolDeallocate_impl(void *pvMemPtr);
void vPartitionPoolBlockInfo_impl(PP_PCB_t *pxPCB);


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype

#endif  // #ifdef __PMP_ENABLE__

#ifdef __cplusplus
}
#endif

#endif // _OS_PARTITION_POOL_H_
