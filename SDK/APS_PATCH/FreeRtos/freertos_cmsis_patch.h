/* *****************************************************************************
 *  Copyright 2017 - 2022, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2022
 *
 *******************************************************************************
 *
 *  @file freertos_cmsis_patch.h 
 * 
 *  @brief FreeRTOS patch
 *  
 *******************************************************************************/
#ifndef _FREERTOS_CMSIS_H_
#error "Please include freertos_cmsis.h. Don't directly include freertos_cmsis_patch.h"
#endif

#ifndef _FREERTOS_CMSIS_PATCH_H_
#define _FREERTOS_CMSIS_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

#define MAX_NUM_MEM_POOL        8

#define PHEAP_MAGIC_NUM    0x90CBD1C4
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef struct os_memory_def
{
    uint32_t ulBlockSize;
    uint32_t ulBlockNum;
} osMemoryDef_t;




typedef void* (*T_osPHeapAllocateFp)(uint32_t ulWantedSize);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void osHwPatch(void);
void osPatchInit(void);
void osPHeapPatchInit(void);
/**
 * @brief Update partition memory pool configuration
 * @warning Call after osHeapAssign. This function will checking the HEAP size enough or not
 *
 * @param psMemTable[in] The memory pool blocks definition. array number = MAX_NUM_MEM_POOL
 * @return Update status
 * @retval osOK Success
 * @retval osErrorOS fail
 */
osStatus osMemoryPoolUpdate(osMemoryDef_t *psMemTable);

/* Enable "malloc" to use PSRAM heap (PHeap)
 * When enable PHeap malloc mix mode. "malloc" will allocate buffer in the following order
 *   1. PMP (partiion memory pool, on RAM)
 *   2. VMP (variable memory pool, on RAM)
 *   3. PHeap (PSRAM heap, on external PSRAM)
 *
 * "mallocPHeap" can allocate buffer from PSRAM either mix mode enable or not 
 * "free" can support free from RAM or PSRAM
 */
void osMemoryPHeapMallocMixModeEn(uint32_t u32Enable);
void *mallocPHeap(unsigned int size);
void vPortPHeapRegionInit(uint8_t* pucAddr, uint32_t ulSize);

void vPortInfoPHeap( void );
void vPortBlockInfoPHeap(void);

#ifdef __cplusplus
}
#endif
#endif  /* _FREERTOS_CMSIS_PATCH_H_ */
