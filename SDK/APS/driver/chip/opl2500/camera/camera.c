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
 *  @file camera.c
 * 
 *  @brief Camera API
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdlib.h>
#include <string.h>
#include "cmsis_os.h"
#include "camera.h"
#include "camera_if.h"
#include "image_sensor.h"
#include "hal_sys_rcc.h"
#include "hal_dma.h"
#include "hal_vic.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */



#define DMA_BLOCK_TS_MAX                    (DMA_CTL_H_BLOCK_TS_Msk >> DMA_CTL_H_BLOCK_TS_Pos)  /* Max block TS number */
#define DMA_BYTES_PER_BLOCK                 (DMA_BLOCK_TS_MAX * g_s32Camera_DmaParameter.u32TranBytes)
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
/* camera API */
T_Camera_Init               Camera_Init                 = Camera_Init_impl;
T_Camera_DeInit             Camera_DeInit               = Camera_DeInit_impl;
T_Camera_ResetDevice        Camera_ResetDevice          = Camera_ResetDevice_impl;
T_Camera_GetFrame           Camera_GetFrame             = Camera_GetFrame_impl;
T_Camera_ReturnFrameBuffer  Camera_ReturnFrameBuffer    = Camera_ReturnFrameBuffer_impl;

/* Local use */
T_Camera_VsyncTracking      _Camera_VsyncTracking       = _Camera_VsyncTracking_impl;
T_Camera_VsyncTrigCb        _Camera_VsyncTrigCb         = _Camera_VsyncTrigCb_impl;
T_Camera_FinishLastDataCopy _Camera_FinishLastDataCopy  = _Camera_FinishLastDataCopy_impl;
T_Camera_DmaInit            _Camera_DmaInit             = _Camera_DmaInit_impl;
T_Camera_DmaUpdate          _Camera_DmaUpdate           = _Camera_DmaUpdate_impl;
T_Camera_DmaIrqHandler      _Camera_DmaIrqHandler       = _Camera_DmaIrqHandler_impl;
T_Camera_SetupFrameBuffers  _Camera_SetupFrameBuffers   = _Camera_SetupFrameBuffers_impl;
T_Camera_DestroyFrame       _Camera_DestroyFrame        = _Camera_DestroyFrame_impl;
T_Camera_CalcFrameSize      _Camera_CalcFrameSize       = _Camera_CalcFrameSize_impl;
T_Camera_Malloc             _Camera_Malloc              = _Camera_Malloc_impl;
T_Camera_Free               _Camera_Free                = _Camera_Free_impl;

S_CAMERA_CTRL g_sCamera_Ctrl;


/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */

const E_VIC_INT_TRIG_TYPE g_eaSyncTrigType[CAMERA_POLARITY_NUM] = {
                VIC_TRIG_EDGE_FALLING,      /* CAMERA_POLARITY_POSITIVE. Trigger at falling edge. */
                VIC_TRIG_EDGE_RISING,       /* CAMERA_POLARITY_NEGATIVE. Trigger at rising edge. */
};

/* For patch use */
S_CAMERA_DMA_PARAM g_s32Camera_DmaParameter = {
    .eDstTrWidth = DMA_TR_WIDTH_32_bits,
    .eSrcTrWidth = DMA_TR_WIDTH_32_bits,
    .eDstMsize = DMA_MSIZE_16,
    .eSrcMsize = DMA_MSIZE_16,
    .eFlowCtrl = DMA_TT_p2m_FC_Dma,
    .u32TranBytes = 1 << DMA_TR_WIDTH_32_bits,
    .u32CamIfRxLevel = CAM_IF_DMA_RX_LEVEL,
};

uint32_t g_u32Camera_VsyncSkipCnt = 0;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

/**
 * @brief Camera_Init
 *        Init camera settings.                                                                                                 \n
 *        According the config setting, it will setup CAM_IF, DMA, SCCB(I2C), and allocate frame buffer.                        \n
 *        If the frame buffer length is over the DMA block maximum, it will allocate DMA linked list buffer for DMA control.    \n
 *        After init OPL2500 hardware, it will control external image sensor through SCCB interface.                            \n
 *        Then setup VSYNC edge trigger interrupt.
 *
 * @note  Call this function after OS scheduler started
 * 
 * @param psCmCfg [in] The camera configuration.                                                                                \n
 *                     In PINs setting, only ePinCameraReset and ePinCameraPwdn are referenced.                                 \n
 *                     This function will NOT handler other PINs.
 * @return The init status.
 * @retval CAMERA_INIT_STATUS_SUCCESS           Setup success and requested frame buffers are allocated success.
 * @retval CAMERA_INIT_STATUS_ALREADY_INIT      Skipped. The camera are already initialized.                                    \n
 *                                              Call Camera_DeInit and then this function to re-init again.
 * @retval CAMERA_INIT_STATUS_INSUF_FRAMES      Setup success, but the number of frame buffers is less than requested number.   \n
 * @retval CAMERA_INIT_STATUS_FAIL_LOCAL        Setup fail. Might be wrong setting, OS not started yet, or insufficient memory. \n
 *                                              All hardware and allocated memory will be de-init.
 * @retval CAMERA_INIT_STATUS_FAIL_SENSOR       Setup local hardwrare success, but configure sensor fail.
 *                                              All hardware and allocated memory will be de-init.
 */
E_CAMERA_INIT_STATUS Camera_Init_impl(S_CAMERA_CFG *psCmCfg)
{
    E_CAMERA_INIT_STATUS eRetFail = CAMERA_INIT_STATUS_FAIL_LOCAL;
    osMessageQDef_t sMsgDef = {psCmCfg->u32FrameCnts, sizeof( S_CAMERA_FRMB * ), NULL};
    uint32_t u32AllocFrmCnts = 0;
    uint32_t u32FrmLen;
    uint32_t u32DmaTransUnits;
    uint32_t u32DmaTransBlkNum;
    
    if (osKernelRunning() != 1)
        return CAMERA_INIT_STATUS_FAIL_SENSOR;
    if (g_sCamera_Ctrl.bInitDone)
        return CAMERA_INIT_STATUS_ALREADY_INIT;

    g_sCamera_Ctrl.sConfig = *psCmCfg;
    
    /* hardware prepare */
    Hal_Sys_CamXvClkSrcSelect((E_ApsClkCamXvClkSrc_t)(psCmCfg->eXvclkSrc & APS_CLK_CAM_XVCLK_SRC_MSK), 
                                (E_ApsClkCamXvCLKDivn_t)(psCmCfg->eXvclkSrc & APS_CLK_CAM_XVCLK_DIV_MSK));
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_XVCLK);
    if (CamIf_Init(psCmCfg->u32eDataIoNum, g_s32Camera_DmaParameter.u32CamIfRxLevel))
        goto fail;

    
    /* Memory preprare */
    if ((g_sCamera_Ctrl.xReadyFrmBQId = osMessageCreate(&sMsgDef, NULL)) == NULL)
        goto fail;
    if ((g_sCamera_Ctrl.xPrepFrmBQId = osMessageCreate(&sMsgDef, NULL)) == NULL)
        goto fail;
    if ((u32AllocFrmCnts = _Camera_SetupFrameBuffers(&u32FrmLen)) == 0)
        goto fail;
    
    /* Align to DMA trans width */
    u32DmaTransUnits = DIV_CEILING(u32FrmLen, g_s32Camera_DmaParameter.u32TranBytes);
    u32DmaTransBlkNum = DIV_CEILING(u32DmaTransUnits, DMA_BLOCK_TS_MAX);
    if (u32DmaTransBlkNum > 1)
    {   /* Allocate DMA LLP */
        uint32_t u32DmaLlpLength = u32DmaTransBlkNum*sizeof(S_DMA_LinkListItem_t);
        if ((g_sCamera_Ctrl.psDmaLlp = (S_DMA_LinkListItem_t *)malloc(u32DmaLlpLength)) == NULL)
            goto fail;
    }
    if (_Camera_DmaInit(g_sCamera_Ctrl.sConfig.eDmaChannel, u32DmaTransUnits, g_sCamera_Ctrl.psDmaLlp, u32DmaTransBlkNum))
        goto fail;
    
    
    /* Configure image sensor */
    eRetFail = CAMERA_INIT_STATUS_FAIL_SENSOR;
    if (ImgSensor_Init(&g_sCamera_Ctrl.sSensor, psCmCfg->eSensorDevice, psCmCfg->ePinCameraReset, psCmCfg->ePinCameraPwdn))
        goto fail;
    if (g_sCamera_Ctrl.sSensor.pfSensorSetup(&g_sCamera_Ctrl.sSensor, g_sCamera_Ctrl.sConfig.eFrameSize, g_sCamera_Ctrl.sConfig.ePixelFormat))
        goto fail;
    
    g_u32Camera_VsyncSkipCnt = g_sCamera_Ctrl.sSensor.u32VsyncSkipCnt;
    _Camera_VsyncTracking(ENABLE);
    g_sCamera_Ctrl.bRunning = false;
    g_sCamera_Ctrl.bInitDone = true;
    
    if (u32AllocFrmCnts < psCmCfg->u32FrameCnts)
        return CAMERA_INIT_STATUS_INSUF_FRAMES;
    return CAMERA_INIT_STATUS_SUCCESS;
    
fail:
    Camera_DeInit();
    return eRetFail;
}

void Camera_DeInit_impl(void)
{   
    osEvent sRxEvent;
    
    CamIf_DeInit();
    ImgSensor_DeInit(&g_sCamera_Ctrl.sSensor);
    _Camera_VsyncTracking(DISABLE);
    Hal_Sys_ApsModuleClkEn(DISABLE, APS_CLK_XVCLK);
    
    if (g_sCamera_Ctrl.bInitDone)
        Hal_Dma_Abort(g_sCamera_Ctrl.sConfig.eDmaChannel);
    
    if (g_sCamera_Ctrl.psFrmBCurr != NULL)
        _Camera_DestroyFrame(g_sCamera_Ctrl.psFrmBCurr);
    while (1)
    {   /* Remove frame buffer in queue */
        sRxEvent = osMessageGet(g_sCamera_Ctrl.xReadyFrmBQId, 0);
        if (sRxEvent.status == osEventMessage)
            _Camera_DestroyFrame(sRxEvent.value.p);
        else    /* Empty */
            break;
    }
    while (1)
    {   /* Remove frame buffer in queue */
        sRxEvent = osMessageGet(g_sCamera_Ctrl.xPrepFrmBQId, 0);
        if (sRxEvent.status == osEventMessage)
            _Camera_DestroyFrame(sRxEvent.value.p);
        else    /* Empty */
            break;
    }
    
    if(g_sCamera_Ctrl.xReadyFrmBQId != NULL)
        free(g_sCamera_Ctrl.xReadyFrmBQId);
    if(g_sCamera_Ctrl.xPrepFrmBQId != NULL)
        free(g_sCamera_Ctrl.xPrepFrmBQId);
    if (g_sCamera_Ctrl.psDmaLlp != NULL)
        free(g_sCamera_Ctrl.psDmaLlp);
    
    memset(&g_sCamera_Ctrl, 0, sizeof(g_sCamera_Ctrl));
}

void Camera_ResetDevice_impl(void)
{
    /* Stop first */
    _Camera_VsyncTracking(DISABLE);
    Hal_Dma_Abort(g_sCamera_Ctrl.sConfig.eDmaChannel);
    CamIf_Stop();
    
    /* TODO : handler exception */
    ImgSensor_Reset(&g_sCamera_Ctrl.sSensor);
    
    if (g_sCamera_Ctrl.bInitDone)
        g_sCamera_Ctrl.sSensor.pfSensorSetup(&g_sCamera_Ctrl.sSensor, g_sCamera_Ctrl.sConfig.eFrameSize, g_sCamera_Ctrl.sConfig.ePixelFormat);
}


S_CAMERA_FRMB *Camera_GetFrame_impl(uint32_t u32TimeoutMs)
{
    osEvent sRxEvent;
    
    if (!g_sCamera_Ctrl.bInitDone)
        return NULL;
    
    sRxEvent = osMessageGet(g_sCamera_Ctrl.xReadyFrmBQId, u32TimeoutMs);
    if(sRxEvent.status == osEventMessage)
    {
        return (S_CAMERA_FRMB *)sRxEvent.value.p;
    }

    return NULL;
}


void Camera_ReturnFrameBuffer_impl(S_CAMERA_FRMB *psFrmB)
{
    if (osMessagePut(g_sCamera_Ctrl.xPrepFrmBQId, (uint32_t)psFrmB, 0) == osErrorOS)
        g_sCamera_Ctrl.u32PrpQOverflowCnt++;
    
    /* Re-start VSYNC */
    if (g_sCamera_Ctrl.bInitDone && !g_sCamera_Ctrl.bRunning)
        _Camera_VsyncTracking(ENABLE);
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

uint32_t _Camera_VsyncTracking_impl(E_FUNC_ST eFunc)
{
    if (eFunc == ENABLE)
        Hal_Gpio_IntInit(g_sCamera_Ctrl.sConfig.ePinVsync, g_eaSyncTrigType[g_sCamera_Ctrl.sConfig.eVsyncPolarity], _Camera_VsyncTrigCb);
    else
        Hal_Gpio_IntDeInit(g_sCamera_Ctrl.sConfig.ePinVsync);
    return 0;
}


uint32_t _Camera_FinishLastDataCopy_impl(E_DMA_CHANNEL eDmaChannel)
{
    uint32_t u32Dar;
    uint32_t u32FifoLevel;;
    uint32_t *pu32Dst;
    S_DMA_Channel_t *psDmaCHx = g_pscaHalDmaCHx[eDmaChannel];
    
    /* Wait SPI Rx Fifo lower than threshold */
    CAM_IF->CAM_ENABLE = 0;
    while (CAM_SPI->RXFLR > CAM_SPI->DMARDLR);
    /* Wait DMA copy done */
    psDmaCHx->CFG_L |= DMA_CFG_L_CH_SUSP;
    while (!(psDmaCHx->CFG_L & DMA_CFG_L_FIFO_EMPTY));
    
    /* Stop DMA and start to copy by CPU */
    Hal_Dma_Abort(eDmaChannel);
    u32Dar = psDmaCHx->DAR;
    u32FifoLevel = CAM_SPI->RXFLR;
    pu32Dst = (uint32_t *)u32Dar;
    for (uint32_t u32Idx=0; u32Idx<u32FifoLevel; u32Idx++)
    {
        *pu32Dst++ = CAM_SPI->DR[0];
    }
    return (uint32_t)pu32Dst;
}

void _Camera_VsyncTrigCb_impl(E_GpioIdx_t eIdx)
{
    osEvent sPrepFrmEvent;
    S_CAMERA_FRMB *psFrmB = g_sCamera_Ctrl.psFrmBCurr;
    
    if (g_u32Camera_VsyncSkipCnt)
    {   /* Only run following after SkipCnt == 0 */
        if (--g_u32Camera_VsyncSkipCnt)
            return;
    }
    
    if (g_sCamera_Ctrl.bRunning == true)
    {   /* Handle received picture */
        uint32_t u32Dar;
        if (psFrmB == NULL)
        {
            tracer_drct_printf("!!!Wrong frame\n");
            while (1);
        }
        psFrmB->u32Tsmp = osKernelSysTick();
        psFrmB->u32Reserved[1] = DWT->CYCCNT;
        /* Wait SPI RX FIFO empty */
        u32Dar = _Camera_FinishLastDataCopy(g_sCamera_Ctrl.sConfig.eDmaChannel);
        psFrmB->u32Reserved[2] = DWT->CYCCNT;
        psFrmB->u32Reserved[0] = u32Dar;
        psFrmB->eRxStatus = CAMERA_RX_STATUS_RX_DONE;
        psFrmB->u32RxLen = u32Dar - (uint32_t)psFrmB->pu8Data;
        
        /* Sending frame to message queue */
        if (osMessagePut(g_sCamera_Ctrl.xReadyFrmBQId, (uint32_t)psFrmB, 0) == osErrorOS)
            g_sCamera_Ctrl.u32RdyQOverflowCnt++;
    }
    
    CamIf_Stop();
    
    /* Get next frame */
    sPrepFrmEvent = osMessageGet(g_sCamera_Ctrl.xPrepFrmBQId, 0);
    if(sPrepFrmEvent.status == osEventMessage)
    {   /* Get new prepare frame success */
        g_sCamera_Ctrl.bRunning = true;
        g_sCamera_Ctrl.psFrmBCurr = (S_CAMERA_FRMB *)sPrepFrmEvent.value.p;
        _Camera_DmaUpdate(g_sCamera_Ctrl.sConfig.eDmaChannel, (uint32_t)g_sCamera_Ctrl.psFrmBCurr->pu8Data, g_sCamera_Ctrl.psDmaLlp);
        CamIf_Start();
    }
    else
    {   /* No prepare frame */
        g_sCamera_Ctrl.psFrmBCurr = NULL;
        _Camera_VsyncTracking(DISABLE);
        g_sCamera_Ctrl.bRunning = false;
    }
    
    
}



/**
 * @brief DMA initialization for camera
 * @param psCamIfCfg [in] Camera interface configuration
 * @return Init status
 * @retval 1: Fail
 * @retval 0: Success
 */
uint32_t _Camera_DmaInit_impl(E_DMA_CHANNEL eDmaChannel, uint32_t u32DmaTransUnits, S_DMA_LinkListItem_t *psDmaLlp, uint32_t u32DmaTransBlkNum)
{   
    uint32_t u32Ret = 1;
    S_DMA_Init_t tCamDmaCfg;
   
    memset(&tCamDmaCfg, 0, sizeof(S_DMA_Init_t));
    
    /* Checking DMA length needs LLP or not */
    if (u32DmaTransBlkNum > 1)
    {   /* Need linked list commands */
        S_DMA_CTL_L_t *ptCtl_L = &psDmaLlp->CTL_L;
        S_DMA_CTL_H_t *ptCtl_H = &psDmaLlp->CTL_H;
        
        if (psDmaLlp == NULL)
            return 1;  /* Not provided */
        
        memset(psDmaLlp, 0, u32DmaTransBlkNum*sizeof(S_DMA_LinkListItem_t));
        
        tCamDmaCfg.LlpDstEn = 1;
        tCamDmaCfg.LlpSrcEn = 1;
        tCamDmaCfg.Loc = ((uint32_t)psDmaLlp >> 2);
        tCamDmaCfg.Done = 0;
        
        psDmaLlp->SAR = (uint32_t)&CAM_SPI->DR;
        psDmaLlp->LLP = (uint32_t)(psDmaLlp+1) & 0xFFFFFFFC;
        
        ptCtl_L->CTL_L_INT_EN       = 1;
        ptCtl_L->CTL_L_DST_TR_WIDTH = g_s32Camera_DmaParameter.eDstTrWidth;
        ptCtl_L->CTL_L_SRC_TR_WIDTH = g_s32Camera_DmaParameter.eSrcTrWidth;
        ptCtl_L->CTL_L_DINC         = DMA_INC_Increment;
        ptCtl_L->CTL_L_SINC         = DMA_INC_No_Change;
        ptCtl_L->CTL_L_DEST_MSIZE   = g_s32Camera_DmaParameter.eDstMsize;
        ptCtl_L->CTL_L_SRC_MSIZE    = g_s32Camera_DmaParameter.eSrcMsize;
        ptCtl_L->CTL_L_TT_FC        = g_s32Camera_DmaParameter.eFlowCtrl;
        ptCtl_L->CTL_L_LLP_DST_EN   = 1;
        ptCtl_L->CTL_L_LLP_SRC_EN   = 1;
        // Ctl_H
        ptCtl_H->CTL_H_BLOCK_TS     = DMA_BLOCK_TS_MAX;
        ptCtl_H->CTL_H_DONE         = 0;
        
        for (uint32_t i = 1; i<u32DmaTransBlkNum; i++)
        {
            psDmaLlp[i] = psDmaLlp[i - 1];
            psDmaLlp[i].LLP = (uint32_t)(&psDmaLlp[i+1]) & 0xFFFFFFFC; // point to next LLI_Block
        }
        
        /* Last part */
        psDmaLlp[u32DmaTransBlkNum - 1].LLP = 0;
        ptCtl_L = (S_DMA_CTL_L_t *) &(psDmaLlp[u32DmaTransBlkNum - 1].CTL_L);
        ptCtl_H = (S_DMA_CTL_H_t *) &(psDmaLlp[u32DmaTransBlkNum - 1].CTL_H);
        ptCtl_L->CTL_L_LLP_DST_EN   = 0;
        ptCtl_L->CTL_L_LLP_SRC_EN   = 0;
        ptCtl_H->CTL_H_BLOCK_TS     = u32DmaTransUnits % DMA_BLOCK_TS_MAX; 
        if (!ptCtl_H->CTL_H_BLOCK_TS)
            ptCtl_H->CTL_H_BLOCK_TS = DMA_BLOCK_TS_MAX;
    }
    else
    {
        /* SAR */
        tCamDmaCfg.Sar = (uint32_t)&CAM_SPI->DR;    
        /* LLP = 0 */
        tCamDmaCfg.LlpSrcEn = 0;
        tCamDmaCfg.LlpDstEn = 0;
    }
    

    /* CTL_L */
    tCamDmaCfg.eTtfc = g_s32Camera_DmaParameter.eFlowCtrl;
    tCamDmaCfg.eDinc = DMA_INC_Increment;
    tCamDmaCfg.eSinc = DMA_INC_No_Change;
    tCamDmaCfg.eDstTrWidth = g_s32Camera_DmaParameter.eDstTrWidth;
    tCamDmaCfg.eSrcTrWidth = g_s32Camera_DmaParameter.eSrcTrWidth;
    
    tCamDmaCfg.BlockTs = u32DmaTransUnits;
    tCamDmaCfg.eDestMsize = g_s32Camera_DmaParameter.eDstMsize;
    tCamDmaCfg.eSrcMsize = g_s32Camera_DmaParameter.eSrcMsize;
    /* CTL_H */
    tCamDmaCfg.Done = 0;
    
    /* CFG_H */
    tCamDmaCfg.SrcPer = APS_DMA_ITF_SPI3_RX;

    /* Setup interrupt */
    Hal_Dma_CallBackSet(eDmaChannel, _Camera_DmaIrqHandler);
    Hal_Dma_IntMask(eDmaChannel, (DMA_BMP_TFR|DMA_BMP_BLK|DMA_BMP_SRC_TRAN|DMA_BMP_DST_TRAN|DMA_BMP_ERR));
    Hal_Dma_IntClear(eDmaChannel, (DMA_BMP_TFR|DMA_BMP_BLK|DMA_BMP_SRC_TRAN|DMA_BMP_DST_TRAN|DMA_BMP_ERR));
    Hal_Dma_IntEn(ENABLE, eDmaChannel);
    Hal_Dma_Config(eDmaChannel, &tCamDmaCfg);
    
    u32Ret = 0;

    return u32Ret;
}


uint32_t _Camera_DmaUpdate_impl(E_DMA_CHANNEL eDmaChannel, uint32_t u32TargetAddr, S_DMA_LinkListItem_t *psDmaLlp)
{
    uint32_t u32Ret = 1;
    
    if (eDmaChannel >= DMA_Channel_Max)
        return 1;
    
    /* Update DMA */
    if (psDmaLlp)
    {   /* With DMA linked list */
        g_pscaHalDmaCHx[eDmaChannel]->LLP = (uint32_t)psDmaLlp;
        g_pscaHalDmaCHx[eDmaChannel]->CTL_L |= DMA_CTL_L_LLP_DST_EN | DMA_CTL_L_LLP_SRC_EN;
        while (psDmaLlp->CTL_L.CTL_L_LLP_DST_EN)
        {
            psDmaLlp->DAR = u32TargetAddr;
            psDmaLlp->CTL_H.CTL_H_DONE = 0;
            u32TargetAddr += DMA_BYTES_PER_BLOCK;
            psDmaLlp++;
        }
        /* Last */
        psDmaLlp->DAR = u32TargetAddr;
        psDmaLlp->CTL_H.CTL_H_DONE = 0;
    }
    else
    {   /* Not use LLP */
        g_pscaHalDmaCHx[eDmaChannel]->DAR = u32TargetAddr;
        g_pscaHalDmaCHx[eDmaChannel]->CTL_H &= ~DMA_CTL_H_DONE_Msk;
    }
                                                            
    Hal_Dma_Enable(eDmaChannel);
    u32Ret = 0;
    
    return u32Ret;
}


uint32_t _Camera_SetupFrameBuffers_impl(uint32_t *pu32FrmLen)
{
    uint32_t u32AllocFrmCnts = 0;
    S_CAMERA_FRMB *psFrmCurr = NULL;

    if (g_sCamera_Ctrl.sConfig.eFrameSize >= CAMERA_FRAMESIZE_NUM)
        return 0;
    
    do {
        if ((psFrmCurr = (S_CAMERA_FRMB *)malloc(sizeof(S_CAMERA_FRMB))) == NULL)
            break;
        memset(psFrmCurr, 0, sizeof(S_CAMERA_FRMB));
        _Camera_CalcFrameSize(psFrmCurr, g_sCamera_Ctrl.sConfig.eFrameSize, g_sCamera_Ctrl.sConfig.ePixelFormat);
        *pu32FrmLen = psFrmCurr->u32FrmLen;
        if ((psFrmCurr->pu8Data = _Camera_Malloc(psFrmCurr->u32FrmLen)) == NULL)
        {   /* Allocate data buffer fail */  
            free(psFrmCurr);
            break;
        }
                
        if (osMessagePut(g_sCamera_Ctrl.xPrepFrmBQId, (uint32_t)psFrmCurr, 0) == osErrorOS)
        {
            g_sCamera_Ctrl.u32PrpQOverflowCnt++;
            break;
        }
    } while (++u32AllocFrmCnts < g_sCamera_Ctrl.sConfig.u32FrameCnts);

    return u32AllocFrmCnts;
}

void _Camera_DestroyFrame_impl(S_CAMERA_FRMB *psFrmB)
{   
    if (psFrmB == NULL)
        return;
    
    if (psFrmB->pu8Data != NULL)
        _Camera_Free(psFrmB->pu8Data);
    free(psFrmB);
}



/**
 * @brief _Camera_CalcFrameSize
 *        According FRAME_SIZE and PIXEL_FORMAT, to calculate the frame length, byte per pixel, and compressed ratio for jpeg
 * @param psFrmB [out] The field will be updated
 * @param eFrameSize [in] The frame size configuration
 * @param ePixelFormat [in] The pixel format
 * @return The calculated result
 * @retval 1 Wrong config, calculated fail
 * @retval 0 Success
 */
uint32_t _Camera_CalcFrameSize_impl(S_CAMERA_FRMB *psFrmB, E_CAMERA_FRAMESIZE eFrameSize, E_CAMERA_PIXEL_FMT ePixelFormat)
{
    uint32_t u32Ret = 1;
    uint32_t u32Pixels;
    uint32_t u32Ratio = 1;
    uint32_t u32FrmBpp;
    
    if (eFrameSize >= CAMERA_FRAMESIZE_NUM)
        goto done;
    psFrmB->u16Width = g_saCamera_FrameSize[eFrameSize].u16Width;
    psFrmB->u16Height = g_saCamera_FrameSize[eFrameSize].u16Height;
    psFrmB->ePixelFormat = ePixelFormat;
        
    /* TODO: according pixel format to find correct frame size */
    u32Pixels = psFrmB->u16Width * psFrmB->u16Height;
    u32FrmBpp = 2;
    
    switch (ePixelFormat)
    {   
        case CAMERA_PIXEL_FMT_JPEG:
            u32Ratio = 10;
            break;
        default:
            u32Ratio = 1;
    }
    psFrmB->u32FrmLen = u32Pixels * u32FrmBpp / u32Ratio;
    
     
    u32Ret = 0;
done:
    return u32Ret;
}



void _Camera_DmaIrqHandler_impl(void)
{
    if (DMA->STS_TFR & (1 << g_sCamera_Ctrl.sConfig.eDmaChannel))
    {   /* RX overflow */
        uint32_t u32Dar;
        S_CAMERA_FRMB *psFrmB = g_sCamera_Ctrl.psFrmBCurr;
        
        CamIf_Stop();
        Hal_Dma_Abort(g_sCamera_Ctrl.sConfig.eDmaChannel);
        _Camera_VsyncTracking(DISABLE);
        g_sCamera_Ctrl.u32DmaOverflowCnt++;
        
        u32Dar = g_pscaHalDmaCHx[g_sCamera_Ctrl.sConfig.eDmaChannel]->DAR;
        psFrmB->u32Reserved[0] = u32Dar;
        psFrmB->u32RxLen = u32Dar - (uint32_t)psFrmB->pu8Data;
        psFrmB->u32Tsmp = osKernelSysTick();
        if (psFrmB->u32RxLen == psFrmB->u32FrmLen)
            psFrmB->eRxStatus = CAMERA_RX_STATUS_RX_DONE;
        else
            psFrmB->eRxStatus = CAMERA_RX_STATUS_OVERFLOW;
        
        if (osMessagePut(g_sCamera_Ctrl.xReadyFrmBQId, (uint32_t)psFrmB, 0) == osErrorOS)
            g_sCamera_Ctrl.u32RdyQOverflowCnt++;
    }
     
    Hal_Dma_IntClear(g_sCamera_Ctrl.sConfig.eDmaChannel, (DMA_BMP_TFR|DMA_BMP_BLK|DMA_BMP_SRC_TRAN|DMA_BMP_DST_TRAN|DMA_BMP_ERR));
    Hal_Vic_IntClear(APS_DMA_IRQn);
   
}

/* To be patched for PSRAM malloc support */
void *_Camera_Malloc_impl(uint32_t u32Length)
{
    return NULL;
}

/* To be patched for PSRAM malloc support */
void _Camera_Free_impl(void *pvBuffer)
{
    
}
