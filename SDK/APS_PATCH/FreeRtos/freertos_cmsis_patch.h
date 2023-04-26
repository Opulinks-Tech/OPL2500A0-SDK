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

#ifdef __cplusplus
}
#endif
#endif  /* _FREERTOS_CMSIS_PATCH_H_ */
