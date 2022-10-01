/******************************************************************************
*  Copyright 2017 - 2022, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2022
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  auxadc_internal.h
*
*  Project:
*  --------
*  OPL2500 Project - the AUXADC definition file
*
*  Description:
*  ------------
*  This include file is the AUXADC definition file (for FT use only)
*
*  Author:
*  -------
*  C.C. Wang
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _HAL_AUXADC_INTERNAL_H_
#define _HAL_AUXADC_INTERNAL_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define AUXADC_OK               0
#define AUXADC_NG               1

#define AUX_GPIO_NUM_MAX        16  // support 16 IOs : from 0 to 15

#define AUXADC_CFG_G0_INIT             (0x1 << 0)
#define AUXADC_CFG_G0_CAL_INIT         (0x1 << 1)
#define AUXADC_CFG_G0_SHOWLOG          (0x1 << 2)
#define AUXADC_CFG_G0_PWR_ALWAYS_ON    (0x1 << 3)

/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list
typedef struct
{
    uint8_t u8CurrSrc;
    uint8_t u8Config[1];
    uint16_t u16AvgCnt;
}S_AuxAdc_Cfg_t;

typedef enum
{
    AUX_SRC_DCDC_IN = 0,
    AUX_SRC_VBAT,
    AUX_SRC_LDO_VCO,
    AUX_SRC_LDO_RF,
    AUX_SRC_TEMP_SEN,
    AUX_SRC_HPBG_REF,
    AUX_SRC_VSS,
    AUX_SRC_VDD_CORE,
    AUX_SRC_GPIO,

    AUX_SRC_MAX,
    AUX_SRC_NUM = AUX_SRC_MAX
} E_Aux_Src_t;

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
// ADC
typedef void (*T_Aux_AdcEnable)( uint8_t u8IsEn );
typedef void (*T_Aux_AdcConfig)( uint8_t u8Var );
typedef void (*T_Aux_Init)( void );
// ADC single
typedef uint8_t (*T_Aux_SourceSelect)( E_Aux_Src_t tSrc, uint8_t ubGpioIdx );
typedef uint8_t (*T_Aux_AdcValueGet)( uint32_t *pulValue );

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
extern char *pAuxadcSrcName[ AUX_SRC_NUM ];
extern S_AuxAdc_Cfg_t stAuxAdcCfg;

// ADC
extern T_Aux_AdcEnable    Aux_AdcEnable;
extern T_Aux_AdcConfig    Aux_AdcGainSet;
extern T_Aux_AdcConfig    Aux_AdcRcSet;
extern T_Aux_AdcConfig    Aux_AdcRefSet;
extern T_Aux_Init         Aux_Init;
// ADC single
extern T_Aux_SourceSelect Aux_SourceSelect;
extern T_Aux_AdcValueGet  Aux_AdcValueGet;

// Sec 7: declaration of static function prototype

/***********
C Functions
***********/
// Sec 8: C Functions
// ADC
void Aux_AdcEnable_impl( uint8_t u8IsEn );
void Aux_AdcGainSet_impl( uint8_t u8Var );
void Aux_AdcRcSet_impl( uint8_t u8Var );
void Aux_AdcRefSet_impl( uint8_t u8Var );
void Aux_Init_impl(void);

// ADC single
uint8_t Aux_SourceSelect_impl(E_Aux_Src_t tSrc, uint8_t ubGpioIdx);
uint8_t Aux_AdcValueGet_impl(uint32_t *pulValue);

#ifdef __cplusplus
}
#endif

#endif // _HAL_AUXADC_INTERNAL_H_
