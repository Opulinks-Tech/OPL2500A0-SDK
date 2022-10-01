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
*  hal_auxadc.h
*
*  Project:
*  --------
*  OPL2500 Project - the AUXADC definition file
*
*  Description:
*  ------------
*  This include file is the AUXADC definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _HAL_AUXADC_H_
#define _HAL_AUXADC_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define HAL_AUX_OK                  0
#define HAL_AUX_FAIL                1

#define HAL_AUX_GPIO_NUM_MAX        16  // support 16 IOs : from 0 to 15
#define HAL_AUX_AVG_CNT_DEFAULT     30  // the average count of ADC value


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
// source type
typedef enum
{
    HAL_AUX_SRC_DCDC_IN = 0,
    HAL_AUX_SRC_VBAT,
    HAL_AUX_SRC_LDO_VCO,
    HAL_AUX_SRC_LDO_RF,
    HAL_AUX_SRC_TEMP_SEN,
    HAL_AUX_SRC_HPBG_REF,
    HAL_AUX_SRC_VSS,
    HAL_AUX_SRC_VDD_CORE,
    HAL_AUX_SRC_GPIO,
    
    HAL_AUX_SRC_MAX,
    HAL_AUX_SRC_NUM = HAL_AUX_SRC_MAX
} E_HalAux_Src_t;

// the calibration data format
typedef struct _sAuxadcCal
{
    uint16_t u16MiniVolt; // Ref. Voltage
    uint16_t u16RawData;  // Ref. ADC raw-data
}S_AuxadcCal_t;

typedef struct _sAuxadcCalTable
{
    uint32_t      u32Header;    // Reserved for future used
    S_AuxadcCal_t stIntSrc[ 2 ];
    
}S_AuxadcCalTable_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype
// Internal: Basic
typedef uint8_t (*T_Hal_Aux_SourceSelect_Fp)(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx);
typedef uint8_t (*T_Hal_Aux_AdcValueGet_Fp)(uint32_t *pulValue);
// Internal: Calibration
typedef uint32_t (*T_Hal_Aux_AdcCal_LoadCfg_Fp)( void );
typedef float (*T_Hal_Aux_AdcMiniVolt_Convert_Fp)(uint32_t u32RawData);

// APIs
typedef void (*T_Hal_Aux_Init_Fp)(void);
typedef void (*T_Hal_Aux_AdcCal_Init_Fp)( void );
typedef uint8_t(*T_Hal_Aux_AdcMiniVolt_Get_Fp)( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, float *pfVbat);
typedef uint8_t (*T_Hal_Aux_AdcConvValue_Get_Fp)( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t *pulValue);

typedef uint32_t (*T_Hal_Aux_Adc_AvgCnt_Get_Fp)( void );
typedef uint32_t (*T_Hal_Aux_Adc_AvgCnt_Set_Fp)(uint32_t u32AvgCnt);
typedef uint32_t (*T_Hal_Aux_Adc_PuEnDelay_Get_Fp)( void );
typedef uint32_t (*T_Hal_Aux_Adc_PuEnDelay_Set_Fp)(uint32_t u32DelayUs);
typedef uint8_t (*T_Hal_Aux_Adc_AlwaysOn_Get_Fp)( void );
typedef uint32_t (*T_Hal_Aux_Adc_AlwaysOn_Set_Fp)(uint8_t u8IsAlwaysOn);

typedef void (*T_Hal_Aux_LseRegressUpdate_Fp)(uint8_t u8Num, S_AuxadcCal_t *puStAdcCalPts);

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
extern S_AuxadcCalTable_t sAuxadcCalTable;

// Internal: Basic
extern T_Hal_Aux_SourceSelect_Fp        Hal_Aux_SourceSelect;
extern T_Hal_Aux_AdcValueGet_Fp         Hal_Aux_AdcValueGet;
// Internal: Calibration
extern T_Hal_Aux_AdcCal_LoadCfg_Fp      Hal_Aux_AdcCal_LoadDef;
extern T_Hal_Aux_AdcCal_LoadCfg_Fp      Hal_Aux_AdcCal_LoadOtp;
extern T_Hal_Aux_AdcCal_LoadCfg_Fp      Hal_Aux_AdcCal_LoadFlash;
extern T_Hal_Aux_AdcMiniVolt_Convert_Fp Hal_Aux_AdcMiniVolt_Convert;

// APIs
extern T_Hal_Aux_Init_Fp                Hal_Aux_Init;
extern T_Hal_Aux_AdcCal_Init_Fp         Hal_Aux_AdcCal_Init;
extern T_Hal_Aux_AdcMiniVolt_Get_Fp     Hal_Aux_AdcMiniVolt_Get;
extern T_Hal_Aux_AdcConvValue_Get_Fp    Hal_Aux_AdcConvValue_Get;

extern T_Hal_Aux_Adc_AvgCnt_Get_Fp      Hal_Aux_Adc_AvgCnt_Get;
extern T_Hal_Aux_Adc_AvgCnt_Set_Fp      Hal_Aux_Adc_AvgCnt_Set;
extern T_Hal_Aux_Adc_PuEnDelay_Get_Fp   Hal_Aux_Adc_PuEnDelay_Get;
extern T_Hal_Aux_Adc_PuEnDelay_Set_Fp   Hal_Aux_Adc_PuEnDelay_Set;
extern T_Hal_Aux_Adc_AlwaysOn_Get_Fp    Hal_Aux_Adc_AlwaysOn_Get;
extern T_Hal_Aux_Adc_AlwaysOn_Set_Fp    Hal_Aux_Adc_AlwaysOn_Set;

extern T_Hal_Aux_LseRegressUpdate_Fp    Hal_Aux_LseRegressUpdate;

// Sec 7: declaration of static function prototype
// Internal: Basic
uint8_t Hal_Aux_SourceSelect_impl(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx);
    uint8_t Hal_Aux_SourceSelect_ts(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx);
uint8_t Hal_Aux_AdcValueGet_impl(uint32_t *pulValue);
    uint8_t Hal_Aux_AdcValueGet_ts(uint32_t *pulValue);
// Internal: Calibration
uint32_t Hal_Aux_AdcCal_LoadDef_impl( void );
uint32_t Hal_Aux_AdcCal_LoadOtp_impl( void );
uint32_t Hal_Aux_AdcCal_LoadFlash_impl( void );
float Hal_Aux_AdcMiniVolt_Convert_impl(uint32_t u32RawData);

// APIs
void Hal_Aux_Init_impl(void);
    void Hal_Aux_Init_ts(void);
void Hal_Aux_AdcCal_Init_impl( void );
uint8_t Hal_Aux_AdcMiniVolt_Get_impl( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, float *pfVbat);
uint8_t Hal_Aux_AdcConvValue_Get_impl( E_HalAux_Src_t tSrc, uint8_t ubGpioIdx, uint32_t *pulValue);

uint32_t Hal_Aux_Adc_AvgCnt_Get_impl( void );
uint32_t Hal_Aux_Adc_AvgCnt_Set_impl(uint32_t u32AvgCnt);
uint32_t Hal_Aux_Adc_PuEnDelay_Get_impl( void );
uint32_t Hal_Aux_Adc_PuEnDelay_Set_impl(uint32_t u32DelayUs);
uint8_t Hal_Aux_Adc_AlwaysOn_Get_impl( void );
uint32_t Hal_Aux_Adc_AlwaysOn_Set_impl(uint8_t u8IsAlwaysOn);

void Hal_Aux_LseRegressUpdate_impl(uint8_t u8Num, S_AuxadcCal_t *puStAdcCalPts);

#include "hal_auxadc_patch.h"

#ifdef __cplusplus
}
#endif

#include "hal_auxadc_patch.h"

#endif // _HAL_AUXADC_H_
