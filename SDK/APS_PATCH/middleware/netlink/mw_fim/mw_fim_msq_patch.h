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
 *  @file mw_fim_msq_patch.h 
 * 
 *  @brief FIM MSQ patch
 *  
 *******************************************************************************/
#ifndef _MW_FIM_MSQ_H_
#error "Please include mw_fim_msq.h. Don't directly include mw_fim_msq_patch.h"
#endif

#ifndef _MW_FIM_MSQ_PATCH_H_
#define _MW_FIM_MSQ_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "mw_fim_default_group06.h"
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
/**
 * @brief MwFim_MsqFimLoadTableAdd
 *        Add file into MSQ FIM loading tables.
 *        This API will only update content in ptFimLoad, not to access flash.
 *
 * @param ptFimLoad [in/out] The loading buffer. Before calling this API, the buffer needs to be allocated and filled in valid data.
 *                           If adding success, the content in ptFimLoad will be updated.
 * @param u32FileId [in] Target file ID to be added into loading tables
 * @param u32RecId [in] Target recorde to be added into loading tables
 * @return The adding status
 * @retval RESULT_FAIL Loading table full, add fail
 * @retval RESULT_SUCCESS Add success
 */
E_RESULT_COMMON MwFim_MsqFimLoadTableAdd(T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad, uint32_t u32FileId, uint32_t u32RecId);
/**
 * @brief MwFim_MsqFimLoadTableDelete
 *        Delete file from MSQ FIM loading tables.
 *        This API will only update content in ptFimLoad, not to access flash. 
 *
 * @param ptFimLoad [in/out] The loading buffer. Before calling this API, the buffer needs to be allocated and filled in valid data.
 *                           If adding success, the content in ptFimLoad will be updated.
 * @param u32FileId [in] Target file ID to be added into loading tables
 * @param u32RecId [in] Target recorde to be added into loading tables
 * @return The delete status
 * @retval RESULT_FAIL Not found target record of file
 * @retval RESULT_SUCCESS Delete success
 */
E_RESULT_COMMON MwFim_MsqFimLoadTableDelete(T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad, uint32_t u32FileId, uint32_t u32RecId);

void MwFim_MsqPatchInit(void);

#ifdef __cplusplus
}
#endif
#endif  /* _MW_FIM_MSQ_PATCH_H_ */
