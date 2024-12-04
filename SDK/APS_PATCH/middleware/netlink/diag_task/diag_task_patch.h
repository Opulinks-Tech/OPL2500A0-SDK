/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 *
 *  @file diag_task_patch.h 
 * 
 *  @brief Patch for Diag task API
 *  
 ******************************************************************************/
#ifndef __DIAG_TASK_H__
#error "Please include diag_task.h. Don't directly include diag_task_patch.h"
#endif
#ifndef _DIAG_TASK_PATCH_H_
#define _DIAG_TASK_PATCH_H_

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


/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
#ifdef WIFI_TPUT
// sizeof(T_WifiStats) <= IPC_WIFI_TPUT_INFO_SIZE
typedef struct
{
    uint32_t u32TxUdfl;
    uint32_t u32RxOvfl;
    uint32_t u32ApsRxFull;
    uint32_t u32TxNoResp;
} T_WifiStats;
#endif


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
void Diag_PatchInit(void);


#ifdef __cplusplus
}
#endif
#endif  /* _DIAG_TASK_PATCH_H_ */
