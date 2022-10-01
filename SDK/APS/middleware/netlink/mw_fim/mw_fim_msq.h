/* *****************************************************************************
 *  Copyright 2017 - 2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file mw_fim_msq.h 
 * 
 *  @brief Handle MSQ FIM data
 *  
 *******************************************************************************/

#ifndef _MW_FIM_MSQ_H_
#define _MW_FIM_MSQ_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
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
typedef enum
{
    MW_FIM_MSQ_FIM_CMD_INIT = 0,
    MW_FIM_MSQ_FIM_CMD_INIT_DONE,
    MW_FIM_MSQ_FIM_CMD_APPLY,
    MW_FIM_MSQ_FIM_CMD_READ,
    MW_FIM_MSQ_FIM_CMD_NUM,
} E_MW_FIM_MSQ_FIM_CMD;

typedef enum
{
    MW_FIM_MSQ_FIM_STATUS_REQ = 0,
    MW_FIM_MSQ_FIM_STATUS_REPLY_SUCCESS,
    MW_FIM_MSQ_FIM_STATUS_REPLY_FAIL,
} E_MW_FIM_MSQ_FIM_STATUS;

typedef struct
{
    uint8_t u8Cmd;          /* E_MW_FIM_MSQ_FIM_CMD */
    uint8_t u8Version;      /* As MW_FIM_VER06 */
    uint32_t u32FileId;
    uint16_t u16RecId;
    uint16_t u16DataSize;
    void *pData;
    
    /* Will be updated by replier */
    E_MW_FIM_MSQ_FIM_STATUS eStatus;
} MW_FIM_MSQ_FIM;


typedef uint32_t (*T_MwFim_MsqFimInit)(void);
typedef void (*T_MwFim_MsqFimLoadAll)(void);
typedef void (*T_MwFim_MsqFimIpcEvtHandler)(void *pData);
typedef uint8_t (*T_MwFim_MsqFimWrite)(uint32_t u32FileId, uint16_t u16RecId, uint16_t u16Size, void *pData);
typedef uint8_t (*T_MwFim_MsqFimRead)(uint32_t u32FileId, uint16_t u16RecId, uint16_t u16Size, void *pData);
typedef uint8_t (*T_MwFim_MsqFimStoreMsqToFlash)(uint32_t u32FileId, uint16_t u16RecId);
typedef uint8_t (*T_MwFim_MsqFimApplyFlashToMsq)(uint32_t u32FileId, uint16_t u16RecId);
typedef uint8_t (*T_MwFim_MsqFimDeleteFlash)(uint32_t u32FileId, uint16_t u16RecId);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern T_MwFim_MsqFimInit               MwFim_MsqFimInit;
extern T_MwFim_MsqFimLoadAll            MwFim_MsqFimLoadAll;
extern T_MwFim_MsqFimIpcEvtHandler      MwFim_MsqFimIpcEvtHandler;
extern T_MwFim_MsqFimWrite              MwFim_MsqFimWriteToMsq;
extern T_MwFim_MsqFimWrite              MwFim_MsqFimWriteToFlash;
extern T_MwFim_MsqFimRead               MwFim_MsqFimReadFromMsq;
extern T_MwFim_MsqFimRead               MwFim_MsqFimReadFromFlash;
extern T_MwFim_MsqFimStoreMsqToFlash    MwFim_MsqFimStoreMsqToFlash;
extern T_MwFim_MsqFimApplyFlashToMsq    MwFim_MsqFimApplyFlashToMsq;
extern T_MwFim_MsqFimDeleteFlash        MwFim_MsqFimDeleteFlash;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
/**
 * @brief MwFim_MsqFimInit
 *        Init MSQ FIM needed semaphore and message queue
 * @return Init status
 * @retval RESULT_SUCCESS Success
 * @retval RESULT_FAIL Fail to create semaphore or message queue
 */
uint32_t MwFim_MsqFimInit_impl(void);

/**
 * @brief MwFim_MsqFimLoadAll
 *        Load all MSQ FIM data at driver init state
 *        Not supported in runnning state
 */
void MwFim_MsqFimLoadAll_impl(void);

/**
 * @brief MwFim_MsqFimIpcEvtHandler
 *        IPC event handler for FIM event
 * @param pData [in/out] The data received from IPC
 */
void MwFim_MsqFimIpcEvtHandler_impl(void *pData);

/**
 * @brief MwFim_MsqFimWriteToMsq
 *        Prepared MSQ FIM, it will apply to MSQ
 * @param u32FileId [in] FIM data file ID
 * @param u16RecId [in] FIM data record ID
 * @param u16Size [in] FIM data size
 * @param pData [in] The FIM data to be written to flash and MSQ
 * @return Apply status
 * @retval MW_FIM_FAIL Fail
 * @retval MW_FIM_OK Success
 */
uint8_t MwFim_MsqFimWriteToMsq_impl(uint32_t u32FileId, uint16_t u16RecId, uint16_t u16Size, void *pData);

/**
 * @brief MwFim_MsqFimWriteToFlash
 *        Prepared MSQ FIM data, it will write to flash
 * @param u32FileId [in] FIM data file ID
 * @param u16RecId [in] FIM data record ID
 * @param u16Size [in] FIM data size
 * @param pData [in] The FIM data to be written to flash and MSQ
 * @return Apply status
 * @retval MW_FIM_FAIL Fail
 * @retval MW_FIM_OK Success
 */
uint8_t MwFim_MsqFimWriteToFlash_impl(uint32_t u32FileId, uint16_t u16RecId, uint16_t u16Size, void *pData);

/**
 * @brief MwFim_MsqFimReadFromMsq
 *        Read MSQ FIM data through IPC
 *        The data is MSQ running data, maybe not same as in flash
 *
 * @param u32FileId [in] The ID defined in ::E_MwFimIdxGroup06
 * @param u16RecId [in] The record ID of target data
 * @param u16Size [in] The FIM data size, if not matched, the read command will be ignored
 * @param pData [out] The buffer must be allocated before calling this function.
 * @return The FIM result
 * @retval MW_FIM_OK    Read success from MSQ
 * @retval MW_FIM_FAIL  Read fail
 */
uint8_t MwFim_MsqFimReadFromMsq_impl(uint32_t u32FileId, uint16_t u16RecId, uint16_t u16Size, void *pData);

/**
 * @brief MwFim_MsqFimReadFromFlash
 *        Read MSQ FIM data from flash
 *        The data is in flash, might not be the same as in MSQ
 *
 * @param u32FileId [in] The ID defined in ::E_MwFimIdxGroup06
 * @param u16RecId [in] The record ID of target data
 * @param u16Size [in] The FIM data size, if not matched, the read command will be ignored
 * @param pData [out] The buffer must be allocated before calling this function.
 * @return The FIM result
 * @retval MW_FIM_OK    Read success from MSQ
 * @retval MW_FIM_FAIL  Read fail
 */
uint8_t MwFim_MsqFimReadFromFlash_impl(uint32_t u32FileId, uint16_t u16RecId, uint16_t u16Size, void *pData);

/**
 * @brief MwFim_MsqFimStoreMsqToFlash
 *        Read MSQ FIM data and write it to flash.
 *        In function, it will malloc a buffer and read MSQ FIM data.
 *        If read success, write the data to flash, and free buffer in the end.
 * @param u32FileId [in] The ID defined in ::E_MwFimIdxGroup06
 * @param u16RecId [in] The record ID of target data
 * @return Read MSQ and write flash result
 * @retval MW_FIM_OK    Read and write success
 * @retval MW_FIM_FAIL  Fail
 */
uint8_t MwFim_MsqFimStoreMsqToFlash_impl(uint32_t u32FileId, uint16_t u16RecId);

/**
 * @brief MwFim_MsqFimApplyFlashToMsq
 *        Read Flash FIM data and apply it to MSQ
 *        In function, it will malloc a buffer and read flash FIM data.
 *        If read success, write the data to MSQ, and free buffer in the end.
 * @param u32FileId [in] The ID defined in ::E_MwFimIdxGroup06
 * @param u16RecId [in] The record ID of target data
 * @return Read flash and write MSQ result
 * @retval MW_FIM_OK    Read and write success
 * @retval MW_FIM_FAIL  Fail
 */
uint8_t MwFim_MsqFimApplyFlashToMsq_impl(uint32_t u32FileId, uint16_t u16RecId);

/**
 * @brief MwFim_MsqFimDeleteFlash
 *        Delete MSQ FIM data in flash
 *        No effect to MSQ
 * @param u32FileId [in] The ID defined in ::E_MwFimIdxGroup06
 * @param u16RecId [in] The record ID of target data
 * @return The FIM result
 * @retval MW_FIM_OK    Delete success from MSQ
 * @retval MW_FIM_FAIL  Delete fail
 */
uint8_t MwFim_MsqFimDeleteFlash_impl(uint32_t u32FileId, uint16_t u16RecId);


#ifdef __cplusplus
}
#endif
#include "mw_fim_msq_patch.h"
#endif  /* _MW_FIM_MSQ_H_ */
