/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  hal_auxadc.c
*
*  Project:
*  --------
*  OPL2500 Project - the AUXADC implement file
*
*  Description:
*  ------------
*  This implement file is include the AUXADC function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "hal_auxadc.h"
#include "auxadc_internal.h"
#include "hal_tick.h"
#include "cmsis_os.h"
#include "boot_sequence.h"
#include "sys_cfg.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
osSemaphoreId     g_taHalAux_SemaphoreId       = NULL;
uint8_t           g_ubHalAux_Init              = 0;
uint8_t           g_ubHalAux_Cal               = 0;
E_HalAux_Src_t    g_tHalAux_CurrentType        = HAL_AUX_SRC_GPIO;
uint8_t           g_ubHalAux_CurrentGpioIdx    = 0;
uint32_t          g_ulHalAux_AverageCount      = HAL_AUX_AVG_CNT_DEFAULT;
uint8_t           g_ubHalAux_AdcAlwaysOn       = 0;
uint32_t          g_ulHalAux_PuEnDelay_Us      = 0;

S_AuxadcCalTable_t sAuxadcCalTable;
float g_fSlope; // RawData/mv
float g_fOffset;

// Internal: Basic
T_Hal_Aux_SourceSelect_Fp        Hal_Aux_SourceSelect        = Hal_Aux_SourceSelect_ts;
T_Hal_Aux_AdcValueGet_Fp         Hal_Aux_AdcValueGet         = Hal_Aux_AdcValueGet_ts;
// Internal: Calibration
T_Hal_Aux_AdcCal_LoadCfg_Fp      Hal_Aux_AdcCal_LoadDef      = Hal_Aux_AdcCal_LoadDef_impl;
T_Hal_Aux_AdcCal_LoadCfg_Fp      Hal_Aux_AdcCal_LoadOtp      = Hal_Aux_AdcCal_LoadOtp_impl;
T_Hal_Aux_AdcCal_LoadCfg_Fp      Hal_Aux_AdcCal_LoadFlash    = Hal_Aux_AdcCal_LoadFlash_impl;
T_Hal_Aux_AdcMiniVolt_Convert_Fp Hal_Aux_AdcMiniVolt_Convert = Hal_Aux_AdcMiniVolt_Convert_impl;

// APIs
T_Hal_Aux_Init_Fp                Hal_Aux_Init                = Hal_Aux_Init_ts;
T_Hal_Aux_AdcCal_Init_Fp         Hal_Aux_AdcCal_Init         = Hal_Aux_AdcCal_Init_impl;
T_Hal_Aux_AdcMiniVolt_Get_Fp     Hal_Aux_AdcMiniVolt_Get     = Hal_Aux_AdcMiniVolt_Get_impl;
T_Hal_Aux_AdcConvValue_Get_Fp    Hal_Aux_AdcConvValue_Get    = Hal_Aux_AdcConvValue_Get_impl;

T_Hal_Aux_Adc_AvgCnt_Get_Fp      Hal_Aux_Adc_AvgCnt_Get      = Hal_Aux_Adc_AvgCnt_Get_impl;
T_Hal_Aux_Adc_AvgCnt_Set_Fp      Hal_Aux_Adc_AvgCnt_Set      = Hal_Aux_Adc_AvgCnt_Set_impl;
T_Hal_Aux_Adc_PuEnDelay_Get_Fp   Hal_Aux_Adc_PuEnDelay_Get   = Hal_Aux_Adc_PuEnDelay_Get_impl;
T_Hal_Aux_Adc_PuEnDelay_Set_Fp   Hal_Aux_Adc_PuEnDelay_Set   = Hal_Aux_Adc_PuEnDelay_Set_impl;
T_Hal_Aux_Adc_AlwaysOn_Get_Fp    Hal_Aux_Adc_AlwaysOn_Get    = Hal_Aux_Adc_AlwaysOn_Get_impl;
T_Hal_Aux_Adc_AlwaysOn_Set_Fp    Hal_Aux_Adc_AlwaysOn_Set    = Hal_Aux_Adc_AlwaysOn_Set_impl;

T_Hal_Aux_LseRegressUpdate_Fp    Hal_Aux_LseRegressUpdate    = Hal_Aux_LseRegressUpdate_impl;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   Hal_Aux_Init
*
* DESCRIPTION:
*   AUXADC init
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_Init_impl(void)
{
    osSemaphoreDef_t tSemaphoreDef;

    // before init
    g_ubHalAux_Init = 0;

    Aux_Init();

    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        // create the semaphore
        tSemaphoreDef.dummy = 0;                            // reserved, it is no used
        g_taHalAux_SemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
        if (g_taHalAux_SemaphoreId == NULL)
        {
            printf("To create the semaphore for Hal_Aux is fail.\n");
            return;
        }
    }

    // after init
    g_ubHalAux_Init = 1;
}

void Hal_Aux_Init_ts(void)
{
    osSemaphoreDef_t tSemaphoreDef;

    // before init
    g_ubHalAux_Init = 0;

    // Set default source
    Hal_Aux_SourceSelect( HAL_AUX_SRC_GPIO, 0);

    // cold boot
    if (0 == Boot_CheckWarmBoot())
    {
        // create the semaphore
        tSemaphoreDef.dummy = 0;                            // reserved, it is no used
        g_taHalAux_SemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
        if (g_taHalAux_SemaphoreId == NULL)
        {
            printf("To create the semaphore for Hal_Aux is fail.\n");
            return;
        }
    }

    // after init
    g_ubHalAux_Init = 1;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_SourceSelect
*
* DESCRIPTION:
*   select the source of AUXADC
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_SourceSelect_impl(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx)
{
    return Aux_SourceSelect( (E_Aux_Src_t)tSrc, ubGpioIdx);
}

uint8_t Hal_Aux_SourceSelect_ts(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx)
{
    uint8_t ubRet = HAL_AUX_FAIL;

    // check the source type
    if (tSrc >= HAL_AUX_SRC_MAX)
        goto done;

    // check the index of GPIO
    if (tSrc == HAL_AUX_SRC_GPIO)
    {
        if (ubGpioIdx >= HAL_AUX_GPIO_NUM_MAX)
            goto done;
    }

    g_tHalAux_CurrentType = tSrc;
    g_ubHalAux_CurrentGpioIdx = ubGpioIdx;

    ubRet = HAL_AUX_OK;

done:
    return ubRet;
}
/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcValueGet
*
* DESCRIPTION:
*   get the ADC value from AUXADC
*
* PARAMETERS
*   1. pulValue : [Out] the ADC value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcValueGet_impl(uint32_t *pulValue)
{
    uint8_t ubRet = HAL_AUX_FAIL;
    uint32_t ulCurrentTick = 0;
    uint32_t ulDiffTick = 0;
    uint32_t ulDelayTicks = ( Hal_Tick_PerMilliSec() / 1000 ) * g_ulHalAux_PuEnDelay_Us;

    Aux_AdcEnable(1);

    // Check delay time
    ulDiffTick = Hal_Tick_Diff( ulCurrentTick );
    if( ulDiffTick < ulDelayTicks )
    {
        osDelay( (ulDelayTicks - ulDiffTick) / Hal_Tick_PerMilliSec() );
        do
        {
            ulDiffTick = Hal_Tick_Diff( ulCurrentTick );
        }while( ulDiffTick < ulDelayTicks );
    }

    ubRet =  Aux_AdcValueGet( pulValue );

    Aux_AdcEnable(0);

    return ubRet;
}

uint8_t Hal_Aux_AdcValueGet_ts(uint32_t *pulValue)
{
    uint8_t ubRet = HAL_AUX_FAIL;
    T_AuxAdcReq tReq = {0};
    T_AuxAdcRsp tRsp = {0};

    tReq.u8Type          = SYS_CFG_AUXADC_VAL_GET;
    tReq.u8Src           = g_tHalAux_CurrentType;
    tReq.u8GpioIdx       = g_ubHalAux_CurrentGpioIdx;
    tReq.u8AdcAlwaysOn   = g_ubHalAux_AdcAlwaysOn;

    tReq.u32RepeatCnt    = g_ulHalAux_AverageCount;
    tReq.u32PuEnDelay_Us = g_ulHalAux_PuEnDelay_Us;

    if(sys_cfg_auxadc_req(&tReq, &tRsp))
    {
        goto done;
    }

    *pulValue = tRsp.u32Value;

    ubRet = HAL_AUX_OK;

done:
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_Init
*
* DESCRIPTION:
*   init data for convert raw-data to mini-volt
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_Aux_AdcCal_Init_impl( void )
{
    if(g_ubHalAux_Cal)
        return;

    if( Hal_Aux_AdcCal_LoadFlash() == HAL_AUX_OK )
    {
        // Load from flash
    }
    else if( Hal_Aux_AdcCal_LoadOtp() == HAL_AUX_OK )
    {
        // Load from OTP
    }
    else
    {
        // Load from internal sources
        Hal_Aux_AdcCal_LoadDef();
    }

    g_ubHalAux_Cal = 1;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_LoadDef
*
* DESCRIPTION:
*   Init default setting from internal sources
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_LoadDef_impl( void )
{
    sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt = 0;
    sAuxadcCalTable.stIntSrc[ 0 ].u16RawData  = 0;

    sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt = 0;
    sAuxadcCalTable.stIntSrc[ 1 ].u16RawData  = 0;

    g_fSlope = 0.3; // RawData/mv
    g_fOffset = 64;

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_LoadOtp
*
* DESCRIPTION:
*   Load default setting form OTP
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_LoadOtp_impl( void )
{
    return HAL_AUX_FAIL;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_LoadTable
*
* DESCRIPTION:
*   Load calibration data from flash
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_LoadFlash_impl( void )
{
    return HAL_AUX_FAIL;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcMiniVolt_Convert
*
* DESCRIPTION:
*   Convert given AUXADC raw-data to mVolt
*
* PARAMETERS
*   1. u32RawData: [In] the given ADC rawData 
*
* RETURNS
*   1. float : mVolt
*
*************************************************************************/
float Hal_Aux_AdcMiniVolt_Convert_impl(uint32_t u32RawData)
{
    return ((float)u32RawData - g_fOffset)/g_fSlope;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcMiniVolt_Get
*
* DESCRIPTION:
*   Got AUXADC voltage in mVolt
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. pfVbat    : [Out] the mVolt of the source
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcMiniVolt_Get_impl( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, float *pfVbat)
{
    uint32_t u32AdcRaw = 0;
    uint8_t ubRet = HAL_AUX_FAIL;

    // check init
    if(g_ubHalAux_Init != 1)
        return ubRet;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    if( HAL_AUX_OK != Hal_Aux_SourceSelect( (E_HalAux_Src_t)tSrc, ubGpioIdx) )
        goto done;

    if( HAL_AUX_OK != Hal_Aux_AdcValueGet( &u32AdcRaw ) )
        goto done;

    // Convert RawData to mVlot
    *pfVbat = (float)Hal_Aux_AdcMiniVolt_Convert( u32AdcRaw )/(float)1000;
    if(*pfVbat < 0)
    {
        *pfVbat = 0;
    }

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcConvValue_Get
*
* DESCRIPTION:
*   Got the converted ADC value (map 0x000~0x3FF to 0 ~ 3000 mV)
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*   3. pulValue  : [Out] the converted ADC value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcConvValue_Get_impl( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t *pulValue)
{
    float fAdcVol = 0;

    if( HAL_AUX_OK != Hal_Aux_AdcMiniVolt_Get(tSrc, ubGpioIdx, &fAdcVol) )
        return HAL_AUX_FAIL;

    // mVlot convert to ideal ADC code
    *pulValue = (uint32_t)( (fAdcVol * 1023) + 3/2 ) / 3;
    if( *pulValue >0x3FF)
    {
        *pulValue = 0x3FF;
    }

    return HAL_AUX_OK;
}

/* Config Average times*/ 
uint32_t Hal_Aux_Adc_AvgCnt_Get_impl( void )
{
    return g_ulHalAux_AverageCount;
}

/* Config Average times*/ 
uint32_t Hal_Aux_Adc_AvgCnt_Set_impl(uint32_t u32AvgCnt)
{
    if( u32AvgCnt == 0 )
        return HAL_AUX_FAIL;

    g_ulHalAux_AverageCount = u32AvgCnt;
    return HAL_AUX_OK;
}

uint32_t Hal_Aux_Adc_PuEnDelay_Get_impl( void )
{
    return g_ulHalAux_PuEnDelay_Us;
}

/* Config PuEn delay. This value caculate from Application Note*/
uint32_t Hal_Aux_Adc_PuEnDelay_Set_impl(uint32_t u32DelayUs)
{
    g_ulHalAux_PuEnDelay_Us = u32DelayUs;
    return HAL_AUX_OK;
}

uint8_t Hal_Aux_Adc_AlwaysOn_Get_impl( void )
{
    return g_ubHalAux_AdcAlwaysOn;
}

uint32_t Hal_Aux_Adc_AlwaysOn_Set_impl(uint8_t u8IsAlwaysOn)
{
    g_ubHalAux_AdcAlwaysOn = u8IsAlwaysOn;
    return HAL_AUX_OK;
}

/* Regression function for AUXADC */
void Hal_Aux_LseRegressUpdate_impl(uint8_t u8Num, S_AuxadcCal_t *puStAdcCalPts)
{
    uint8_t u8Idx = 0;
    float fAvgMiniVol = 0;
    float fAvgAdcCode = 0;
    float fTemp_diff_x = 0, fTemp_diff_y = 0;
    float fTemp_den = 0, fTemp_num = 0;

    // LSE regression
    // Step 1: compute AVG
    for(u8Idx = 0; u8Idx<u8Num ; u8Idx++)
    {
        fAvgMiniVol += (float)puStAdcCalPts[ u8Idx ].u16MiniVolt;
        fAvgAdcCode += (float)puStAdcCalPts[ u8Idx ].u16RawData;
    }
    fAvgMiniVol /= u8Num;
    fAvgAdcCode /= u8Num;

    // Step 2: compute slove and offset
    for(u8Idx = 0; u8Idx<u8Num ; u8Idx++)
    {
        fTemp_diff_x = (float)puStAdcCalPts[ u8Idx ].u16MiniVolt - fAvgMiniVol;
        fTemp_diff_y = (float)puStAdcCalPts[ u8Idx ].u16RawData  - fAvgAdcCode;
        
        fTemp_den += fTemp_diff_x * fTemp_diff_y;
        fTemp_num += fTemp_diff_x * fTemp_diff_x;
    }
    g_fSlope  = fTemp_den/fTemp_num; // RawData/mv
    g_fOffset = fAvgAdcCode - g_fSlope*fAvgMiniVol;
}
