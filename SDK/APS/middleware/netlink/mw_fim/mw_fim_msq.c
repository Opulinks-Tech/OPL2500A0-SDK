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
 *  @file mw_fim_msq.c
 * 
 *  @brief Handle MSQ FIM data
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdlib.h>
#include <string.h>
#include "mw_fim_msq.h"
#include "mw_fim.h"
#include "mw_fim_default.h"
#include "mw_fim_default_group06.h"
#include "data_flow.h"
#include "cmsis_os.h"
#include "ps.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define MW_FIM_MSQ_FIM_IPC_QUEUE_NUM        1
#define MW_FIM_MSQ_FIM_WAIT_REPLY_MS        1000

#define MW_FIM_MSQ_GROUP_ID_MSK             MW_FIM_IDX_GP06_START

#ifdef OPL2500_FPGA
#include "ipc.h"
#include "hal_system.h"
#define IPC_SPARE0_FPGA_SYNC_MASK              (1UL << IPC_SPARE0_FPGA_0_BIT)
#endif /* OPL2500_FPGA */


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
T_MwFim_MsqFimInit              MwFim_MsqFimInit                = MwFim_MsqFimInit_impl;
T_MwFim_MsqFimLoadAll           MwFim_MsqFimLoadAll             = MwFim_MsqFimLoadAll_impl;
T_MwFim_MsqFimIpcEvtHandler     MwFim_MsqFimIpcEvtHandler       = MwFim_MsqFimIpcEvtHandler_impl;
T_MwFim_MsqFimWrite             MwFim_MsqFimWriteToMsq          = MwFim_MsqFimWriteToMsq_impl;
T_MwFim_MsqFimWrite             MwFim_MsqFimWriteToFlash        = MwFim_MsqFimWriteToFlash_impl;
T_MwFim_MsqFimRead              MwFim_MsqFimReadFromMsq         = MwFim_MsqFimReadFromMsq_impl;
T_MwFim_MsqFimRead              MwFim_MsqFimReadFromFlash       = MwFim_MsqFimReadFromFlash_impl;
T_MwFim_MsqFimStoreMsqToFlash   MwFim_MsqFimStoreMsqToFlash     = MwFim_MsqFimStoreMsqToFlash_impl;
T_MwFim_MsqFimApplyFlashToMsq   MwFim_MsqFimApplyFlashToMsq     = MwFim_MsqFimApplyFlashToMsq_impl;
T_MwFim_MsqFimDeleteFlash       MwFim_MsqFimDeleteFlash         = MwFim_MsqFimDeleteFlash_impl;

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
bool g_bMwFim_MsqFimInitDone = false;
bool g_bMwFim_MsqFimInitDoneReply = false;
osMessageQId g_tMwFim_MsqFimIpcReplyQueue = NULL;
osSemaphoreId g_tMwFim_MsqFimSem = NULL;
uint32_t g_u32MwFim_MsqFimWaitReplyMs = MW_FIM_MSQ_FIM_WAIT_REPLY_MS;

 
 
 
 

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
uint32_t MwFim_MsqFimInit_impl(void)
{
    osMessageQDef_t tQueueDef;
    osSemaphoreDef_t tSemDef;
    
    if (!g_tMwFim_MsqFimSem)
    {
        g_tMwFim_MsqFimSem = osSemaphoreCreate(&tSemDef, 1);
    }
    
    if (!g_tMwFim_MsqFimIpcReplyQueue)
    {
        tQueueDef.item_sz = sizeof(uint32_t);
        tQueueDef.queue_sz = MW_FIM_MSQ_FIM_IPC_QUEUE_NUM;
        g_tMwFim_MsqFimIpcReplyQueue = osMessageCreate(&tQueueDef, NULL);
    }
    
    if ((g_tMwFim_MsqFimSem == NULL) || (g_tMwFim_MsqFimIpcReplyQueue == NULL))
        return RESULT_FAIL;
    
    return RESULT_SUCCESS;
}


void MwFim_MsqFimLoadAll_impl(void)
{
    T_MwFimFileInfo *ptFileInfo = (T_MwFimFileInfo *)g_taMwFimGroupTable06;
    MW_FIM_MSQ_FIM tMsqFimCmdTx, *ptMsqFimCmdRx;
    uint32_t u32FileId, u32Addr, u32Vid;
    uint16_t u16RecId;
    T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad;
    
    if (g_bMwFim_MsqFimInitDone == true)
        return;
    
    ptFimLoad = (T_MW_FIM_MSQ_FIM_LOAD *)malloc(g_tMwFim_MsqFimParam.u32FimLoadTableSize);
    if (ptFimLoad == NULL)
        goto load_done;
    MwFim_MsqFimLoadRead(ptFimLoad);
    
    for (u32Vid = 0; u32Vid < ptFimLoad->u32ValidNum; u32Vid++)
    {
        u32FileId = ptFimLoad->taValidFiles[u32Vid].u16FileId | MW_FIM_MSQ_GROUP_ID_MSK;
        u16RecId = ptFimLoad->taValidFiles[u32Vid].u16RecId;
        
        while (ptFileInfo->ulFileId != 0xFFFFFFFF)
        {
            if (ptFileInfo->ulFileId == u32FileId)
                break;
            else
                ptFileInfo++;
        }
        if (ptFileInfo->ulFileId == 0xFFFFFFFF) /* Search finished */
            goto load_done;
        if (u16RecId > ptFileInfo->uwRecordMax)
            continue;
        
        u32Addr = ptFileInfo->pulDataAddr[u16RecId];
        if (u32Addr == 0xFFFFFFFF)  /* Not exist in flash */
            continue;
        
        tMsqFimCmdTx.pData = (uint8_t *)malloc(ptFileInfo->uwDataSize);
        if (!tMsqFimCmdTx.pData)
            goto load_done;
        if (Hal_Flash_AddrRead_Ext(g_MwFim_FlashCtrlCfg.eSpiIdx, g_MwFim_FlashCtrlCfg.eSlvIdx, u32Addr, 0, ptFileInfo->uwDataSize, tMsqFimCmdTx.pData) != RESULT_SUCCESS)
        {
            free(tMsqFimCmdTx.pData);
            goto load_done;
        }
        
        tMsqFimCmdTx.u8Cmd = MW_FIM_MSQ_FIM_CMD_INIT;
        tMsqFimCmdTx.u8Version = MW_FIM_VER06;
        tMsqFimCmdTx.u32FileId = ptFileInfo->ulFileId;
        tMsqFimCmdTx.u16RecId = u16RecId;
        tMsqFimCmdTx.u16DataSize = ptFileInfo->uwDataSize;
        tMsqFimCmdTx.eStatus = MW_FIM_MSQ_FIM_STATUS_REQ;
        IPC_CMD_SEND(FIM_CMD_EVT, &tMsqFimCmdTx, sizeof(MW_FIM_MSQ_FIM));
    }
    
load_done:
    memset(&tMsqFimCmdTx, 0, sizeof(tMsqFimCmdTx));
    tMsqFimCmdTx.u8Cmd = MW_FIM_MSQ_FIM_CMD_INIT_DONE;
    tMsqFimCmdTx.u8Version = MW_FIM_VER06;
    tMsqFimCmdTx.eStatus = MW_FIM_MSQ_FIM_STATUS_REQ;
    IPC_CMD_SEND(FIM_CMD_EVT, &tMsqFimCmdTx, sizeof(MW_FIM_MSQ_FIM));
    
    while (!g_bMwFim_MsqFimInitDoneReply)
    {   /* Wait MSQ read done */
        ptMsqFimCmdRx = IPC_EVT_READ();
        ps_update_processing_time(proc_type_aps_rsp);
        if (ptMsqFimCmdRx != NULL)
        {   
            MwFim_MsqFimIpcEvtHandler(ptMsqFimCmdRx);
            IPC_EVT_READ_DONE();
        }
    }
    
    if (ptFimLoad)
        free(ptFimLoad);
    
    g_bMwFim_MsqFimInitDone = true;

#ifdef OPL2500_FPGA
    Hal_Sys_SpareRegWait(SPARE_0, IPC_SPARE0_FPGA_SYNC_MASK, 1);
#endif /* OPL2500_FPGA */
}


void MwFim_MsqFimIpcEvtHandler_impl(void *pData)
{
    MW_FIM_MSQ_FIM *ptCmd = (MW_FIM_MSQ_FIM *)pData;
    if (ptCmd == NULL)
        return;
    
    switch (ptCmd->u8Cmd)
    {
        case MW_FIM_MSQ_FIM_CMD_INIT:
            free(ptCmd->pData);
            break;
        case MW_FIM_MSQ_FIM_CMD_INIT_DONE:
            g_bMwFim_MsqFimInitDoneReply = true;
            break;
        case MW_FIM_MSQ_FIM_CMD_READ:
            osMessagePut(g_tMwFim_MsqFimIpcReplyQueue, (uint32_t)ptCmd->eStatus, 0);
            break;
        case MW_FIM_MSQ_FIM_CMD_APPLY:
            osMessagePut(g_tMwFim_MsqFimIpcReplyQueue, (uint32_t)ptCmd->eStatus, 0);
            break;
        default:
            break;
    }
}


uint8_t MwFim_MsqFimWriteToMsq_impl(uint32_t u32FileId, uint16_t u16RecId, uint16_t u16Size, void *pData)
{
    osEvent tEvent;
    uint8_t u8Ret = MW_FIM_FAIL;
    MW_FIM_MSQ_FIM tMsqFimCmdTx;

    if (pData == NULL)
        return MW_FIM_FAIL;

    osSemaphoreWait(g_tMwFim_MsqFimSem, osWaitForever);
    
    tMsqFimCmdTx.u8Cmd = MW_FIM_MSQ_FIM_CMD_APPLY;
    tMsqFimCmdTx.u8Version = MW_FIM_VER06;
    tMsqFimCmdTx.u32FileId = u32FileId;
    tMsqFimCmdTx.u16RecId = u16RecId;
    tMsqFimCmdTx.u16DataSize = u16Size;
    tMsqFimCmdTx.pData = pData;
    tMsqFimCmdTx.eStatus = MW_FIM_MSQ_FIM_STATUS_REQ;
    if (IPC_CMD_SEND(FIM_CMD_EVT, &tMsqFimCmdTx, sizeof(MW_FIM_MSQ_FIM)) != 0)
    {
        goto done;
    }

    /* Wait MSQ replies */
    tEvent = osMessageGet(g_tMwFim_MsqFimIpcReplyQueue, g_u32MwFim_MsqFimWaitReplyMs);
    if (tEvent.status == osEventMessage) 
    {
        if (tEvent.value.v == MW_FIM_MSQ_FIM_STATUS_REPLY_SUCCESS)
            u8Ret = MW_FIM_OK;
    }

done:
    osSemaphoreRelease(g_tMwFim_MsqFimSem);
    return u8Ret;
}

uint8_t MwFim_MsqFimWriteToFlash_impl(uint32_t u32FileId, uint16_t u16RecId, uint16_t u16Size, void *pData)
{
    uint8_t u8Ret;

    if (pData == NULL)
        return MW_FIM_FAIL;

    osSemaphoreWait(g_tMwFim_MsqFimSem, osWaitForever);
    
    u8Ret = MwFim_FileWrite(u32FileId, u16RecId, u16Size, pData);
    
    osSemaphoreRelease(g_tMwFim_MsqFimSem);
    return u8Ret;
}

uint8_t MwFim_MsqFimReadFromMsq_impl(uint32_t u32FileId, uint16_t u16RecId, uint16_t u16Size, void *pData)
{
    osEvent tEvent;
    uint8_t u8Ret = MW_FIM_FAIL;
    MW_FIM_MSQ_FIM tMsqFimCmdTx;
    
    if (pData == NULL)
        return MW_FIM_FAIL;
    
    osSemaphoreWait(g_tMwFim_MsqFimSem, osWaitForever);
    
    /* Send read request */
    tMsqFimCmdTx.u8Cmd = MW_FIM_MSQ_FIM_CMD_READ;
    tMsqFimCmdTx.u8Version = MW_FIM_VER06;
    tMsqFimCmdTx.u32FileId = u32FileId;
    tMsqFimCmdTx.u16RecId = u16RecId;
    tMsqFimCmdTx.u16DataSize = u16Size;
    tMsqFimCmdTx.pData = pData;
    tMsqFimCmdTx.eStatus = MW_FIM_MSQ_FIM_STATUS_REQ;
    if (IPC_CMD_SEND(FIM_CMD_EVT, &tMsqFimCmdTx, sizeof(MW_FIM_MSQ_FIM)) < 0)
        goto done;
    
    /* Wait MSQ replies */
    tEvent = osMessageGet(g_tMwFim_MsqFimIpcReplyQueue, g_u32MwFim_MsqFimWaitReplyMs);
    if ((tEvent.status == osEventMessage) && (tEvent.value.v == MW_FIM_MSQ_FIM_STATUS_REPLY_SUCCESS))
    {
        /* Read from MSQ success */
        u8Ret = MW_FIM_OK;
    }

done:
    osSemaphoreRelease(g_tMwFim_MsqFimSem);
    return u8Ret;
}

uint8_t MwFim_MsqFimReadFromFlash_impl(uint32_t u32FileId, uint16_t u16RecId, uint16_t u16Size, void *pData)
{
    uint8_t u8Ret;

    if (pData == NULL)
        return MW_FIM_FAIL;

    osSemaphoreWait(g_tMwFim_MsqFimSem, osWaitForever);
    
    u8Ret = MwFim_FileRead(u32FileId, u16RecId, u16Size, pData);
    
    osSemaphoreRelease(g_tMwFim_MsqFimSem);
    return u8Ret;
}



uint8_t MwFim_MsqFimStoreMsqToFlash_impl(uint32_t u32FileId, uint16_t u16RecId)
{
    uint8_t u8Ret = MW_FIM_FAIL;
    uint16_t u16Size;
    void *pData = NULL;
    
    u16Size = MwFim_FileSizeGet(u32FileId);
    if (!u16Size)
        goto done;
    
    pData = malloc(u16Size);
    if (!pData)
        goto done;
    
    u8Ret = MwFim_MsqFimReadFromMsq(u32FileId, u16RecId, u16Size, pData);
    if (u8Ret == MW_FIM_FAIL)
        goto done;

    u8Ret = MwFim_FileWrite(u32FileId, u16RecId, u16Size, pData);
    
done:
    if (pData)
        free(pData);
    return u8Ret;
}


uint8_t MwFim_MsqFimApplyFlashToMsq_impl(uint32_t u32FileId, uint16_t u16RecId)
{
    uint8_t u8Ret = MW_FIM_FAIL;
    uint16_t u16Size;
    void *pData = NULL;
    
    u16Size = MwFim_FileSizeGet(u32FileId);
    if (!u16Size)
        goto done;
    
    pData = malloc(u16Size);
    if (!pData)
        goto done;
    
    u8Ret = MwFim_MsqFimReadFromFlash(u32FileId, u16RecId, u16Size, pData);
    if (u8Ret == MW_FIM_FAIL)
        goto done;
    
    u8Ret = MwFim_MsqFimWriteToMsq(u32FileId, u16RecId, u16Size, pData);
    
done:
    if (pData)
        free(pData);
    return u8Ret;
}

uint8_t MwFim_MsqFimDeleteFlash_impl(uint32_t u32FileId, uint16_t u16RecId)
{
    uint8_t u8Ret;

    osSemaphoreWait(g_tMwFim_MsqFimSem, osWaitForever);
    
    u8Ret = MwFim_FileDelete(u32FileId, u16RecId);
    
    osSemaphoreRelease(g_tMwFim_MsqFimSem);
    return u8Ret;
}
