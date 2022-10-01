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

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "mw_fim_msq.h"
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
*                           Declarations of Private Functions
*************************************************************************
*/

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 
 
 
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Private Functions
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
E_RESULT_COMMON MwFim_MsqFimLoadTableAdd(T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad, uint32_t u32FileId, uint32_t u32RecId)
{
    uint32_t u32Id;
    
    if (ptFimLoad->u32ValidNum >= g_tMwFim_MsqFimParam.u32FimLoadFileMax)
        return RESULT_FAIL;

    for (u32Id=0; u32Id<ptFimLoad->u32ValidNum; u32Id++)
    {
        if ((ptFimLoad->taValidFiles[u32Id].u16FileId == u32FileId) &&
            (ptFimLoad->taValidFiles[u32Id].u16RecId == u32RecId))
        {
            break;
        }
    }
    if (u32Id == ptFimLoad->u32ValidNum)
    {   /* No the same file in loading table */
        ptFimLoad->taValidFiles[u32Id].u16FileId = u32FileId;
        ptFimLoad->taValidFiles[u32Id].u16RecId = u32RecId;
        ptFimLoad->u32ValidNum++;
    }
    else
    {
        /* Already in loading table */
    }
    return RESULT_SUCCESS;
}

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
E_RESULT_COMMON MwFim_MsqFimLoadTableDelete(T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad, uint32_t u32FileId, uint32_t u32RecId)
{
    uint32_t u32Id;
    
    if (ptFimLoad->u32ValidNum)
        return RESULT_FAIL;
    for (u32Id=0; u32Id<ptFimLoad->u32ValidNum; u32Id++)
    {
        if ((ptFimLoad->taValidFiles[u32Id].u16FileId == u32FileId) &&
            (ptFimLoad->taValidFiles[u32Id].u16RecId == u32RecId))
        {
            break;
        }
    }

    while (u32Id + 1 < ptFimLoad->u32ValidNum)
    {   /* Copy the remaining array */
        ptFimLoad->taValidFiles[u32Id].u16FileId = ptFimLoad->taValidFiles[u32Id+1].u16FileId;
        ptFimLoad->taValidFiles[u32Id].u16RecId = ptFimLoad->taValidFiles[u32Id+1].u16RecId;
        u32Id++;
    }
    ptFimLoad->u32ValidNum--;

    return RESULT_SUCCESS;
}
