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
*  TW FW
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "hal_auxadc.h"
#include "cmsis_os.h"
#include "hal_flash.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define AUXADC_FLASH_START_ADDR            0x8F000

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern osSemaphoreId     g_taHalAux_SemaphoreId;
extern uint8_t           g_ubHalAux_Init;
extern uint8_t           g_ubHalAux_Cal;
extern E_HalAux_Src_t    g_tHalAux_CurrentType;
extern uint8_t           g_ubHalAux_CurrentGpioIdx;
extern uint32_t          g_ulHalAux_AverageCount;
extern uint8_t           g_ubHalAux_AdcAlwaysOn;
extern uint32_t          g_ulHalAux_PuEnDelay_Us;

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
*   Hal_Aux_AdcCal_LoadFlash
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
uint32_t Hal_Aux_AdcCal_LoadFlash_patch( void )
{
    uint32_t u32Res = 0;
    uint32_t u32Header = 0;
    
    u32Res = Hal_Flash_AddrRead(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable.u32Header), (uint8_t *)&u32Header);
    if( u32Header == 0xFFFFFFFF)
        return HAL_AUX_FAIL;

    u32Res = Hal_Flash_AddrRead(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable), (uint8_t *)&sAuxadcCalTable);

    if( u32Res != 0 )
    {
        // Error happen
        return HAL_AUX_FAIL;
    }else{
        // No error, updated slope and offset
        Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);
        return HAL_AUX_OK;
    }
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_EraseFlash
*
* DESCRIPTION:
*   Erase calibration data to flash
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_EraseFlash( void )
{
    uint32_t u32Res = 0;
    
    u32Res = Hal_Flash_4KSectorAddrErase( SPI_IDX_0, AUXADC_FLASH_START_ADDR);
    if( u32Res != 0)
        return HAL_AUX_FAIL;
    else
        return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcCal_StoreFlash
*
* DESCRIPTION:
*   Store calibration data to flash
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcCal_StoreFlash( void )
{
    uint32_t u32Res = 0;
    
    u32Res = Hal_Aux_AdcCal_EraseFlash();
    if( u32Res != HAL_AUX_OK)
        return HAL_AUX_FAIL;

    u32Res = Hal_Flash_AddrProgram(SPI_IDX_0, AUXADC_FLASH_START_ADDR, 0, sizeof(sAuxadcCalTable), (uint8_t *)&sAuxadcCalTable);
    if( u32Res != 0)
        return HAL_AUX_FAIL;
    else
        return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcGpioInCal
*
* DESCRIPTION:
*   Cal auxadc value of given GPIO
*
* PARAMETERS
*   1. u8GpioIdx   : [In] the given GPIO
*   2. u16MiniVolt : [In] the input mVolt
*   3. u8PtsIdx    : [In] the index of calibration pts
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcGpioInCal(uint8_t u8GpioIdx, uint16_t u16MiniVolt, uint8_t u8PtsIdx)
{
    uint32_t u32Temp = 0;
    uint32_t u32Res = 0;

    // Check input valid
    if(u8GpioIdx >= HAL_AUX_GPIO_NUM_MAX)
        return HAL_AUX_FAIL;
    if(u8PtsIdx >= 2)
        return HAL_AUX_FAIL;

    // Get ADC raw-data
    Hal_Aux_SourceSelect( HAL_AUX_SRC_GPIO, u8GpioIdx);
    u32Res = Hal_Aux_AdcValueGet( &u32Temp );
    if(u32Res == HAL_AUX_FAIL)
        return HAL_AUX_FAIL;

    // Update table
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16MiniVolt = u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16RawData = u32Temp;
    Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);
    sAuxadcCalTable.u32Header = 0x1 | u8PtsIdx;

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcVbatInCal
*
* DESCRIPTION:
*   Cal auxadc value of given Vbat
*
* PARAMETERS
*   1. u16MiniVolt : [In] the input mVolt
*   2. u8PtsIdx    : [In] the index of calibration pts
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint32_t Hal_Aux_AdcVbatInCal(uint16_t u16MiniVolt, uint8_t u8PtsIdx)
{
    uint32_t u32Temp = 0;
    uint32_t u32Res = 0;

    // Check input valid
    if(u8PtsIdx >= 2)
        return HAL_AUX_FAIL;

    // Get ADC raw-data
    Hal_Aux_SourceSelect( HAL_AUX_SRC_VBAT, 0);
    u32Res = Hal_Aux_AdcValueGet( &u32Temp );
    if(u32Res == HAL_AUX_FAIL)
        return HAL_AUX_FAIL;

    // Update table
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16MiniVolt = u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ u8PtsIdx ].u16RawData = u32Temp;
    Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);
    sAuxadcCalTable.u32Header = 0x2 | u8PtsIdx;

    return HAL_AUX_OK;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_VbatCalibration
*
* DESCRIPTION:
*   do the calibration of VBAT
*
* PARAMETERS
*   1. fVbat : [In] the voltage of VBAT
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_VbatCalibration(float fVbat)
{
    uint32_t u32Temp;
    uint8_t ubRet = HAL_AUX_FAIL;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    // VSS
    Hal_Aux_SourceSelect(HAL_AUX_SRC_VSS, 0);
    if( HAL_AUX_OK != Hal_Aux_AdcValueGet(&u32Temp) )
        goto done;
    sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt = 0;
    sAuxadcCalTable.stIntSrc[ 0 ].u16RawData = u32Temp;

    // VBat
    Hal_Aux_SourceSelect(HAL_AUX_SRC_VBAT, 0);
    if( HAL_AUX_OK != Hal_Aux_AdcValueGet(&u32Temp) )
        goto done;
    sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt = (uint16_t)fVbat;
    sAuxadcCalTable.stIntSrc[ 1 ].u16RawData = u32Temp;

    // Update table
    Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);

    // Store data
    sAuxadcCalTable.u32Header = 0x31;
    if( HAL_AUX_OK != Hal_Aux_AdcCal_StoreFlash() )
        goto done;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_IoVoltageCalibration
*
* DESCRIPTION:
*   do the calibration of the IO voltage
*
* PARAMETERS
*   1. ubGpioIdx : [In] the index of GPIO
*   2. fVoltage  : [In] the IO voltage value
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_IoVoltageCalibration(uint8_t ubGpioIdx, float fVoltage)
{
    uint32_t u32Temp;
    uint8_t ubRet = HAL_AUX_FAIL;

    // wait the semaphore
    osSemaphoreWait(g_taHalAux_SemaphoreId, osWaitForever);

    // VSS
    Hal_Aux_SourceSelect(HAL_AUX_SRC_VSS, 0);
    if( HAL_AUX_OK != Hal_Aux_AdcValueGet(&u32Temp) )
        goto done;
    sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt = 0;
    sAuxadcCalTable.stIntSrc[ 0 ].u16RawData = u32Temp;

    // GPIO
    Hal_Aux_SourceSelect(HAL_AUX_SRC_GPIO, ubGpioIdx);
    if( HAL_AUX_OK != Hal_Aux_AdcValueGet(&u32Temp) )
        goto done;
    sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt = (uint16_t)fVoltage;
    sAuxadcCalTable.stIntSrc[ 1 ].u16RawData = u32Temp;

    // Update table
    Hal_Aux_LseRegressUpdate(2, sAuxadcCalTable.stIntSrc);

    // Store data
    // Store data
    sAuxadcCalTable.u32Header = 0x41;
    if( HAL_AUX_OK != Hal_Aux_AdcCal_StoreFlash() )
        goto done;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}
