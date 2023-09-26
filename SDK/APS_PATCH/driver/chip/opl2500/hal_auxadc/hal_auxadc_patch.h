/******************************************************************************
*  Copyright 2017 - 2022, Opulinks Technology Ltd.
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
*  TW FW
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _HAL_AUXADC_PATCH_H_
#define _HAL_AUXADC_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// [31:16] MAGIC NUM
#define MAGIC_CODE_MSK                    (0xFFFF0000)
#define MAGIC_CODE_AUXADC                 (0x5A18 << 16)
// [15:12] MAJOR SRC
#define ADC_CAL_SRC_MAJOR_MSK             (0xF << 12)
#define ADC_CAL_SRC_MAJOR_DEF             (0 << 12)
#define ADC_CAL_SRC_MAJOR_OTP             (1 << 12)
#define ADC_CAL_SRC_MAJOR_FLASH           (2 << 12)
// [11:08] MINOR SRC
#define ADC_CAL_SRC_MINOR_MSK             (0xF << 8)
#define ADC_CAL_SRC_MINOR_GPIO_2P         (1 << 8)
#define ADC_CAL_SRC_MINOR_VBAT_2P         (2 << 8)
#define ADC_CAL_SRC_MINOR_GPIO_AND_GND    (3 << 8)
#define ADC_CAL_SRC_MINOR_VBAT_AND_GND    (4 << 8)


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype
void Hal_Auxadc_PatchInit(void);

// Calibration data
uint32_t Hal_Aux_AdcCal_LoadOtp_patch( void );
uint32_t Hal_Aux_AdcCal_LoadFlash_patch( void );
uint32_t Hal_Aux_AdcCal_EraseFlash( void );
uint32_t Hal_Aux_AdcCal_StoreFlash( void );

// 2-pt calibration method
uint32_t Hal_Aux_AdcGpioInCal(uint8_t u8GpioIdx, uint16_t u16MiniVolt, uint8_t u8PtsIdx);
uint32_t Hal_Aux_AdcVbatInCal(uint16_t u16MiniVolt, uint8_t u8PtsIdx);
// 1-pt calibration method
uint8_t Hal_Aux_VbatCalibration(float fVbat);
uint8_t Hal_Aux_IoVoltageCalibration(uint8_t ubGpioIdx, float fVoltage);

#ifdef __cplusplus
}
#endif

#endif // _HAL_AUXADC_PATCH_H_
