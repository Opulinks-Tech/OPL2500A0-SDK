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
 *  @file camera.h 
 * 
 *  @brief Camera API
 *  
 *******************************************************************************/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdbool.h>
#include "cmsis_os.h"
#include "image_sensor.h"
#include "hal_gpio.h"
#include "hal_sys_rcc.h"
#include "hal_dma.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define CAM_IF_DATA_IO_MAX_NUM  16
#define CAM_IF_DMA_RX_LEVEL     15
    
#define CAM_JPEG_QUALITY_MIN    1
#define CAM_JPEG_QUALITY_MAX    100
#define CAM_JPEG_QUALITY_RANGE  (CAM_JPEG_QUALITY_MAX - CAM_JPEG_QUALITY_MIN + 1)

#define CAMERA_CAPTURE_SKIP_CNTS    5


/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */



typedef enum 
{
    CAMERA_XVCLK_SRC_XTAL               = APS_CLK_CAM_XVCLK_SRC_XTAL         | APS_CLK_CAM_XVCLK_DIV_1,
    CAMERA_XVCLK_SRC_XTAL_Div2          = APS_CLK_CAM_XVCLK_SRC_XTAL         | APS_CLK_CAM_XVCLK_DIV_2,
    CAMERA_XVCLK_SRC_XTAL_Div3          = APS_CLK_CAM_XVCLK_SRC_XTAL         | APS_CLK_CAM_XVCLK_DIV_3,
    CAMERA_XVCLK_SRC_XTAL_Div4          = APS_CLK_CAM_XVCLK_SRC_XTAL         | APS_CLK_CAM_XVCLK_DIV_4,
    CAMERA_XVCLK_SRC_XTAL_X2            = APS_CLK_CAM_XVCLK_SRC_XTAL_X2      | APS_CLK_CAM_XVCLK_DIV_1,
    CAMERA_XVCLK_SRC_XTAL_Div1p5        = APS_CLK_CAM_XVCLK_SRC_XTAL_X2      | APS_CLK_CAM_XVCLK_DIV_3,
    CAMERA_XVCLK_SRC_80M                = APS_CLK_CAM_XVCLK_SRC_DECI_160M_BB | APS_CLK_CAM_XVCLK_DIV_2,
    CAMERA_XVCLK_SRC_53M                = APS_CLK_CAM_XVCLK_SRC_DECI_160M_BB | APS_CLK_CAM_XVCLK_DIV_3,
    CAMERA_XVCLK_SRC_40M                = APS_CLK_CAM_XVCLK_SRC_DECI_160M_BB | APS_CLK_CAM_XVCLK_DIV_4,
} E_CAMERA_XVCLK_SRC;

typedef enum
{
    CAMERA_RX_STATUS_WAITING = 0,
    CAMERA_RX_STATUS_OVERFLOW,       /* DMA finished before VSYNC */
    CAMERA_RX_STATUS_RX_DONE,
} E_CAMERA_RX_STATUS;

typedef enum
{
    CAMERA_INIT_STATUS_SUCCESS = 0,
    CAMERA_INIT_STATUS_ALREADY_INIT,
    CAMERA_INIT_STATUS_INSUF_FRAMES,    /* Init success but not enough memory to allocat request frames */
    CAMERA_INIT_STATUS_FAIL_LOCAL,      /* Init fail because local fail */
    CAMERA_INIT_STATUS_FAIL_SENSOR,     /* Init fail because setup sensor fail */
} E_CAMERA_INIT_STATUS;

typedef struct
{
    E_DMA_TR_WIDTH eDstTrWidth;
    E_DMA_TR_WIDTH eSrcTrWidth;
    E_DMA_MSIZE eDstMsize;
    E_DMA_MSIZE eSrcMsize;
    E_DMA_TT_FC eFlowCtrl;
    uint32_t u32TranBytes;
    
    uint32_t u32CamIfRxLevel;
} S_CAMERA_DMA_PARAM;


typedef struct
{
    E_IMGSENSOR_DEVICES eSensorDevice;
    
    E_CAMERA_XVCLK_SRC eXvclkSrc;
    E_DMA_CHANNEL eDmaChannel;
    
    /* Pin */
    E_GpioIdx_t ePinVsync;
    uint32_t u32eDataIoNum;
    E_GpioIdx_t ePinData[CAM_IF_DATA_IO_MAX_NUM];

    E_GpioIdx_t ePinCameraReset;
    uint32_t u32ResetTimingMs;
    E_GpioIdx_t ePinCameraPwdn;
    
    E_CAMERA_SYNC_POLARITY eVsyncPolarity;
    E_CAMERA_SYNC_POLARITY eHrefPolarity;
    
    /* Image */
    E_CAMERA_FRAMESIZE eFrameSize;          /* CIF/SVGA/UXGA... */
    E_CAMERA_PIXEL_FMT ePixelFormat;      /* RGB565/YUV422... */
    uint8_t u8JpegQuality;                  /* JPEG quality: 1~100. 1 is best quality */
    
    /* Frame */
    uint32_t u32FrameCnts;                  /* How many frames to be created */
    
} S_CAMERA_CFG;

typedef struct
{
    uint8_t *pu8Data;
    uint16_t u16Width;
    uint16_t u16Height;
    uint32_t u32FrmLen;
    E_CAMERA_PIXEL_FMT ePixelFormat; 
    E_CAMERA_RX_STATUS eRxStatus;
    uint32_t u32RxLen;
    uint32_t u32Tsmp;       /* os Tick */
    uint32_t u32Reserved[8];
} S_CAMERA_FRMB;


typedef struct
{
    S_CAMERA_CFG sConfig;
    S_IMG_SENSOR sSensor;
    S_DMA_LinkListItem_t *psDmaLlp;
    S_CAMERA_FRMB *psFrmBCurr;
    bool bInitDone;
    bool bRunning;
    
    /* OS */
    osMessageQId xReadyFrmBQId;
    osMessageQId xPrepFrmBQId;
    
    /* Status */
    uint32_t u32DmaOverflowCnt;
    uint32_t u32RdyQOverflowCnt;
    uint32_t u32PrpQOverflowCnt;
} S_CAMERA_CTRL;

/* camera API */
typedef E_CAMERA_INIT_STATUS (*T_Camera_Init)(S_CAMERA_CFG *psCmCfg);
typedef void (*T_Camera_DeInit)(void);
typedef void (*T_Camera_ResetDevice)(void);
typedef S_CAMERA_FRMB *(*T_Camera_GetFrame)(uint32_t u32TimeoutMs);
typedef void (*T_Camera_ReturnFrameBuffer)(S_CAMERA_FRMB *psFrmB);

/* Local use */
typedef uint32_t (*T_Camera_VsyncTracking)(E_FUNC_ST eFunc);
typedef void (*T_Camera_VsyncTrigCb)(E_GpioIdx_t eIdx);
typedef uint32_t (*T_Camera_FinishLastDataCopy)(E_DMA_CHANNEL eDmaChannel);
typedef uint32_t (*T_Camera_DmaInit)(E_DMA_CHANNEL eDmaChannel, uint32_t u32DmaTransUnits, S_DMA_LinkListItem_t *psDmaLlp, uint32_t u32DmaTransBlkNum);
typedef uint32_t (*T_Camera_DmaUpdate)(E_DMA_CHANNEL eDmaChannel, uint32_t u32TargetAddr, S_DMA_LinkListItem_t *psDmaLlp);
typedef void (*T_Camera_DmaIrqHandler)(void);
typedef uint32_t (*T_Camera_SetupFrameBuffers)(uint32_t *pu32FrmLen);
typedef void (*T_Camera_DestroyFrame)(S_CAMERA_FRMB *psFrmB);
typedef uint32_t (*T_Camera_CalcFrameSize)(S_CAMERA_FRMB *psFrmB, E_CAMERA_FRAMESIZE eFrameSize, E_CAMERA_PIXEL_FMT ePixelFormat);

typedef void *(*T_Camera_Malloc)(uint32_t u32Length);
typedef void (*T_Camera_Free)(void *pvBuffer);



/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
/* camera API */
extern T_Camera_Init                Camera_Init;
extern T_Camera_DeInit              Camera_DeInit;
extern T_Camera_ResetDevice         Camera_ResetDevice;
extern T_Camera_GetFrame            Camera_GetFrame;
extern T_Camera_ReturnFrameBuffer   Camera_ReturnFrameBuffer;

/* Local use */
extern T_Camera_VsyncTracking       _Camera_VsyncTracking;
extern T_Camera_VsyncTrigCb         _Camera_VsyncTrigCb;
extern T_Camera_FinishLastDataCopy  _Camera_FinishLastDataCopy;
extern T_Camera_DmaInit             _Camera_DmaInit;
extern T_Camera_DmaUpdate           _Camera_DmaUpdate;
extern T_Camera_DmaIrqHandler       _Camera_DmaIrqHandler;
extern T_Camera_SetupFrameBuffers   _Camera_SetupFrameBuffers;
extern T_Camera_DestroyFrame        _Camera_DestroyFrame;
extern T_Camera_CalcFrameSize       _Camera_CalcFrameSize;
extern T_Camera_Malloc              _Camera_Malloc;
extern T_Camera_Free                _Camera_Free;


extern uint32_t u32DmaStatus[];
/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
E_CAMERA_INIT_STATUS Camera_Init_impl(S_CAMERA_CFG *psCmCfg);
void Camera_DeInit_impl(void);
void Camera_ResetDevice_impl(void);
S_CAMERA_FRMB *Camera_GetFrame_impl(uint32_t u32TimeoutMs);
void Camera_ReturnFrameBuffer_impl(S_CAMERA_FRMB *psFrmB);

/* Local use */
uint32_t _Camera_SetupFrameBuffers_impl(uint32_t *pu32FrmLen);
uint32_t _Camera_VsyncTracking_impl(E_FUNC_ST eFunc);
void _Camera_VsyncTrigCb_impl(E_GpioIdx_t eIdx);
uint32_t _Camera_FinishLastDataCopy_impl(E_DMA_CHANNEL eDmaChannel);
uint32_t _Camera_DmaInit_impl(E_DMA_CHANNEL eDmaChannel, uint32_t u32DmaTransUnits, S_DMA_LinkListItem_t *psDmaLlp, uint32_t u32DmaTransBlkNum);
uint32_t _Camera_DmaUpdate_impl(E_DMA_CHANNEL eDmaChannel, uint32_t u32TargetAddr, S_DMA_LinkListItem_t *psDmaLlp);
void _Camera_DestroyFrame_impl(S_CAMERA_FRMB *psFrmB);
uint32_t _Camera_CalcFrameSize_impl(S_CAMERA_FRMB *psFrmB, E_CAMERA_FRAMESIZE eFrameSize, E_CAMERA_PIXEL_FMT ePixelFormat);
void _Camera_DmaIrqHandler_impl(void);


/* Needs to integrate PSRAM heap */
void *_Camera_Malloc_impl(uint32_t u32Length);
void _Camera_Free_impl(void *pvBuffer);

#ifdef __cplusplus
}
#endif
#endif  /* _CAMERA_H_ */
