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
 */
#ifndef _FREERTOS_CMSIS_H_
#define _FREERTOS_CMSIS_H_


#ifdef  __cplusplus
extern "C"
{
#endif

#include "portmacro.h"
#include "cmsis_os.h"

typedef UBaseType_t (*T_convertFreeRtosPriorityFp)(osPriority priority);
typedef osPriority (*T_convertCmsisPriorityFp)(UBaseType_t fpriority);
typedef int (*T_inHandlerModeFp)(void);
typedef int (*T_osHeapAssignFp)(uint32_t u32HeapStartAddr, uint32_t u32HeapSize);
    
extern T_convertFreeRtosPriorityFp convertFreeRtosPriority;
extern T_convertCmsisPriorityFp convertCmsisPriority;
extern T_inHandlerModeFp inHandlerMode;
extern T_osHeapAssignFp osHeapAssign;

    
int osHeapAssign_impl(uint32_t u32HeapStartAddr, uint32_t u32HeapSize);

#ifdef  __cplusplus
}
#endif

#endif  /* _FREERTOS_CMSIS_H_ */
