/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
 *  @file hal_pdm.h
 *  @brief API to control PDM
 ******************************************************************************/

#ifndef __HAL_PDM_H__
#define __HAL_PDM_H__

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
#include "hal_dma.h"

/*
 *************************************************************************
 *						    Definitions and Macros
 *************************************************************************
 */
#define PDM_STS_L_DATA(sts)             ((int16_t) (sts & 0xFFFF))
#define PDM_STS_R_DATA(sts)             ((int16_t) (sts >> 16))
#define PDM_GET_DONE_BUF()              ((int16_t *) (((S_DMA_Channel_t *) ((uint32_t) &DMA->SAR0 + 0x58 * g_eHalPdmDmaChannel))->DSTAT - g_u16HalPdmDmaBlockSize))
#define PDM_GET_DONE_SIZE()             (g_u16HalPdmDmaBlockSize)

/*
 *************************************************************************
 *						    Typedefs
 *************************************************************************
 */
typedef void (*T_PdmCallBack)(void);

/*
 *************************************************************************
 *						    Enums and Structures
 *************************************************************************
 */
typedef enum
{
    PDM_CHANNEL_STEREO = 0,
    PDM_CHANNEL_MONO_R = 1,
    PDM_CHANNEL_MONO_L = 2,

    PDM_CHANNEL_MAX    = 3
} E_PDM_CHANNEL;

typedef enum
{
    PDM_SAMPLE_RATE_64K = 0,
    PDM_SAMPLE_RATE_32K = 1,
    PDM_SAMPLE_RATE_16K = 2,
    PDM_SAMPLE_RATE_8K  = 3,

    PDM_SAMPLE_RATE_MAX = 4
} E_PDM_SAMPLE_RATE;

typedef enum
{
    PDM_DATA_MODE_NO_BUFFER         = 0,
    PDM_DATA_MODE_CONTINUOUS_BUFFER = 1,
    PDM_DATA_MODE_CIRCULAR_BUFFER   = 2,

    PDM_DATA_MODE_MAX               = 3
} E_PDM_DATA_MODE;

typedef struct
{
    E_PDM_CHANNEL     eChannel;
    E_PDM_SAMPLE_RATE eSampleRate;
    E_PDM_DATA_MODE   eDataMode;
    T_PdmCallBack     tCallback;
    E_DMA_CHANNEL     eDmaChannel;
    int16_t          *dmaDataBuffer;
    uint8_t          *dmaInnerBuffer;   /* 28Bytes * n */
    uint16_t          dmaBlockTs;       /* 0-4095 */
    uint8_t           dmaBlockNum;

} S_PDM_Conf_t;

/*
 *************************************************************************
 *							Public Variables
 *************************************************************************
 */
extern E_DMA_CHANNEL        g_eHalPdmDmaChannel;
extern uint16_t             g_u16HalPdmDmaBlockSize;

/*
 *************************************************************************
 *							Public Functions
 *************************************************************************
 */
void Hal_Pdm_Init_impl(void);
uint32_t Hal_Pdm_Config_impl(const S_PDM_Conf_t *);
uint32_t Hal_Pdm_Enable_impl(uint8_t enable);
uint32_t Hal_Pdm_GetData_impl(void);

void Hal_Pdm_Pre_Init(void);

/*
 *************************************************************************
 *							Patch Definition
 *************************************************************************
 */
typedef void (*T_Hal_Pdm_Init_fp)(void);
typedef uint32_t (*T_Hal_Pdm_Config_fp)(const S_PDM_Conf_t *);
typedef uint32_t (*T_Hal_Pdm_Enable_fp)(uint8_t enable);
typedef uint32_t (*T_Hal_Pdm_GetData_fp)(void);

extern T_Hal_Pdm_Init_fp    Hal_Pdm_Init;
extern T_Hal_Pdm_Config_fp  Hal_Pdm_Config;
extern T_Hal_Pdm_Enable_fp  Hal_Pdm_Enable;
extern T_Hal_Pdm_GetData_fp Hal_Pdm_GetData;

#endif
