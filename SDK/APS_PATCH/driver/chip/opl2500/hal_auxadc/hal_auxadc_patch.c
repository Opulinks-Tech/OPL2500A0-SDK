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
#include "auxadc_internal.h"
#include "rf_reg_lite.h"

#include "hal_auxadc.h"
#include "cmsis_os.h"
#include "hal_flash.h"

#include "mw_fim_default_group01_patch.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define AUXADC_FLASH_START_ADDR            0x8E000

// RF 0x7C
#define CK_GATE_AUXADC_POS    (3)

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

T_AdcGpioOffset stAdcGpioOffset;

// Sec 5: declaration of global function prototype
uint8_t Hal_Aux_AdcValueGet_patch(uint32_t *pulValue);
void Hal_Aux_Init_patch(void);
void Hal_Aux_Init_ts_patch(void);
void Hal_Aux_AdcCal_Init_patch( void );
uint8_t Hal_Aux_SourceSelect_patch(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx);
float Hal_Aux_AdcMiniVolt_Convert_patch(uint32_t u32RawData);

// For auxadc_internal.c
void Aux_AdcEnable_patch( uint8_t u8IsEn );

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
void Hal_Auxadc_PatchInit(void)
{
// ---- ---- [ For auxadc_internal.c ] ---- ----
    // ADC
    Aux_AdcEnable  = Aux_AdcEnable_patch;
    // Aux_AdcGainSet = Aux_AdcGainSet_impl;
    // Aux_AdcRcSet   = Aux_AdcRcSet_impl;
    // Aux_AdcRefSet  = Aux_AdcRefSet_impl;
    // Aux_Init       = Aux_Init_impl;
    // ADC single
    // Aux_SourceSelect = Aux_SourceSelect_impl;
    // Aux_AdcValueGet  = Aux_AdcValueGet_impl;

// ---- ---- [ For hal_auxadc.c ] ---- ----
    // Internal: Basic
    Hal_Aux_SourceSelect        = Hal_Aux_SourceSelect_patch; // TS or patch
    Hal_Aux_AdcValueGet         = Hal_Aux_AdcValueGet_patch; // TS or patch
    // Internal: Calibration
    // Hal_Aux_AdcCal_LoadDef      = Hal_Aux_AdcCal_LoadDef_impl;
    Hal_Aux_AdcCal_LoadOtp      = Hal_Aux_AdcCal_LoadOtp_patch;
    Hal_Aux_AdcCal_LoadFlash    = Hal_Aux_AdcCal_LoadFlash_patch;
    Hal_Aux_AdcMiniVolt_Convert = Hal_Aux_AdcMiniVolt_Convert_patch;

    // APIs
    Hal_Aux_Init                = Hal_Aux_Init_patch; // TS_patch or patch
    Hal_Aux_AdcCal_Init         = Hal_Aux_AdcCal_Init_patch;
    // Hal_Aux_AdcMiniVolt_Get     = Hal_Aux_AdcMiniVolt_Get_impl;
    // Hal_Aux_AdcConvValue_Get    = Hal_Aux_AdcConvValue_Get_impl;

    // Hal_Aux_Adc_AvgCnt_Get      = Hal_Aux_Adc_AvgCnt_Get_impl;
    // Hal_Aux_Adc_AvgCnt_Set      = Hal_Aux_Adc_AvgCnt_Set_impl;
    // Hal_Aux_Adc_PuEnDelay_Get   = Hal_Aux_Adc_PuEnDelay_Get_impl;
    // Hal_Aux_Adc_PuEnDelay_Set   = Hal_Aux_Adc_PuEnDelay_Set_impl;
    // Hal_Aux_Adc_AlwaysOn_Get    = Hal_Aux_Adc_AlwaysOn_Get_impl;
    // Hal_Aux_Adc_AlwaysOn_Set    = Hal_Aux_Adc_AlwaysOn_Set_impl;

    // Hal_Aux_LseRegressUpdate    = Hal_Aux_LseRegressUpdate_impl;

    g_ulHalAux_AverageCount      = 1;
}

/*************************************************************************
* FUNCTION:
*   Aux_AdcEnable
*
* DESCRIPTION:
*   Enable/Disable AUXADC pu, clock and updated RF tables
*
* PARAMETERS
*   u8IsEn: Enable/Disable
*
* RETURNS
*   none
*
*************************************************************************/
void Aux_AdcEnable_patch( uint8_t u8IsEn )
{
    if( u8IsEn )
    {
        // Rf_Auxadc_En_Update( u8IsEn );
        RF->CK_GATE_CTRL = (0x1 << CK_GATE_AUXADC_POS);

        // Enable Pu and clock
        RF->AUX_ADC_CK_GEN_CTL |= RG_PU_AUXADC;

        // Enable AUXADC
        AOS->ADC_CTL |= AOS_ADC_CTL_AUXADC_EN;
    }else{
        if( !g_ubHalAux_AdcAlwaysOn )
        {
            // Rf_Auxadc_En_Update( u8IsEn );
            RF->CK_GATE_CTRL = ( 0x1 << (CK_GATE_AUXADC_POS+16) );

            // Disable AUXADC
            AOS->ADC_CTL &= ~AOS_ADC_CTL_AUXADC_EN;

            // Disable Pu and clock
            RF->AUX_ADC_CK_GEN_CTL &= ~RG_PU_AUXADC;
        }
    }
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
void Hal_Aux_AdcCal_Init_patch( void )
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

    Hal_Aux_AdcGpioOffset_Load();

    g_ubHalAux_Cal = 1;
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
#define FT_TO_CURR_OFFSET    100
uint32_t Hal_Aux_AdcCal_LoadOtp_patch( void )
{
    uint8_t u8Idx = 0;
    uint32_t u32Otp_temp, u32ApsClk;
    S_AuxadcCal_t stOtpRef[5];

    // Check OTP clock
    u32ApsClk = AOS->APS_CLK_SEL;
    AOS->APS_CLK_SEL |= AOS_APS_CLK_SEL_OTP_PCLK_EN;
    for(u8Idx = 0; u8Idx<5; u8Idx++)
    {
        u32Otp_temp = reg_read(0x30002140 + 4*u8Idx);

        stOtpRef[ u8Idx ].u16MiniVolt = (u8Idx + 1)*500 - FT_TO_CURR_OFFSET;
        stOtpRef[ u8Idx ].u16RawData = u32Otp_temp;
    }
    AOS->APS_CLK_SEL = u32ApsClk;

    if(  stOtpRef[ 0 ].u16RawData ==  stOtpRef[ 4 ].u16RawData )
        return HAL_AUX_FAIL;

    sAuxadcCalTable.stIntSrc[ 0 ].u16MiniVolt = stOtpRef[ 0 ].u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ 0 ].u16RawData  = stOtpRef[ 0 ].u16RawData;
    
    sAuxadcCalTable.stIntSrc[ 1 ].u16MiniVolt = stOtpRef[ 4 ].u16MiniVolt;
    sAuxadcCalTable.stIntSrc[ 1 ].u16RawData  = stOtpRef[ 4 ].u16RawData;
    
    // Updated slope and offset
    Hal_Aux_LseRegressUpdate(5, stOtpRef);

    return HAL_AUX_OK;
}

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
    if( (u32Header & MAGIC_CODE_MSK) != MAGIC_CODE_AUXADC )
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
uint8_t Hal_Aux_AdcValueGet_patch(uint32_t *pulValue)
{
    stAuxAdcCfg.u16AvgCnt = g_ulHalAux_AverageCount;
    return Hal_Aux_AdcValueGet_impl(pulValue);
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
float Hal_Aux_AdcMiniVolt_Convert_patch(uint32_t u32RawData)
{
    if(g_tHalAux_CurrentType == HAL_AUX_SRC_GPIO)
        return Hal_Aux_AdcMiniVolt_Convert_impl( u32RawData ) + stAdcGpioOffset.i16GpioOffset[ g_ubHalAux_CurrentGpioIdx ];
    else
        return Hal_Aux_AdcMiniVolt_Convert_impl( u32RawData );
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcGpioOffset_Load
*
* DESCRIPTION:
*   Load ADC offset data
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcGpioOffset_Load(void)
{
    uint8_t ubRet = MwFim_FileRead(MW_FIM_IDX_GP01_ADC_OFFSET, 0, MW_FIM_ADC_OFFSET_SIZE, (uint8_t *)&stAdcGpioOffset);
    if( ubRet == MW_FIM_OK)
        return HAL_AUX_OK;
    else
        return HAL_AUX_FAIL;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcGpioOffset_Store
*
* DESCRIPTION:
*   Store ADC offset data
*
* PARAMETERS
*   none
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcGpioOffset_Store(void)
{
    uint8_t ubRet = MwFim_FileWrite(MW_FIM_IDX_GP01_ADC_OFFSET, 0, MW_FIM_ADC_OFFSET_SIZE, (uint8_t *)&stAdcGpioOffset);
    if( ubRet == MW_FIM_OK)
        return HAL_AUX_OK;
    else
        return HAL_AUX_FAIL;
}

/*************************************************************************
* FUNCTION:
*   Hal_Aux_AdcGpioOffset_Set
*
* DESCRIPTION:
*   Set ADC offset data
*
* PARAMETERS
*   1. ubGpioIdx   : [In] the index of GPIO
*   2. i16MiniVolt : [In] the input mVolt
*
* RETURNS
*   1. HAL_AUX_OK   : success
*   2. HAL_AUX_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Aux_AdcGpioOffset_Set(uint8_t ubGpioIdx, int16_t i16MiniVolt)
{
    if( ubGpioIdx >= HAL_AUX_GPIO_NUM_MAX)
        return HAL_AUX_FAIL;

    stAdcGpioOffset.i16GpioOffset[ ubGpioIdx ] = i16MiniVolt;
    return HAL_AUX_OK;
}

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
void Hal_Aux_Init_patch(void)
{
    Aux_Init();

    // create the semaphore
    if(g_taHalAux_SemaphoreId == NULL)
    {
        osSemaphoreDef_t tSemaphoreDef;
        tSemaphoreDef.dummy = 0;
        g_taHalAux_SemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
        if (g_taHalAux_SemaphoreId == NULL)
        {
            printf("To create the semaphore for Hal_Aux is fail.\n");
            return;
        }
    }

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
uint8_t Hal_Aux_SourceSelect_patch(E_HalAux_Src_t tSrc, uint8_t ubGpioIdx)
{
    g_tHalAux_CurrentType = tSrc;
    g_ubHalAux_CurrentGpioIdx = ubGpioIdx;

    return Hal_Aux_SourceSelect_impl(tSrc, ubGpioIdx);
}

/*
void Hal_Aux_Init_ts_patch(void)
{
    // Set default source
    Hal_Aux_SourceSelect( HAL_AUX_SRC_GPIO, 0);

    // create the semaphore
    if(g_taHalAux_SemaphoreId == NULL)
    {
        osSemaphoreDef_t tSemaphoreDef;
        tSemaphoreDef.dummy = 0;
        g_taHalAux_SemaphoreId = osSemaphoreCreate(&tSemaphoreDef, 1);
        if (g_taHalAux_SemaphoreId == NULL)
        {
            printf("To create the semaphore for Hal_Aux is fail.\n");
            return;
        }
    }

    g_ubHalAux_Init = 1;
}
*/

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
    sAuxadcCalTable.u32Header = MAGIC_CODE_AUXADC | ADC_CAL_SRC_MAJOR_FLASH | ADC_CAL_SRC_MINOR_GPIO_2P | u8PtsIdx;

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
    sAuxadcCalTable.u32Header = MAGIC_CODE_AUXADC | ADC_CAL_SRC_MAJOR_FLASH | ADC_CAL_SRC_MINOR_VBAT_2P | u8PtsIdx;

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
    sAuxadcCalTable.u32Header = MAGIC_CODE_AUXADC | ADC_CAL_SRC_MAJOR_FLASH | ADC_CAL_SRC_MINOR_VBAT_AND_GND | 0x1;
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
    sAuxadcCalTable.u32Header = MAGIC_CODE_AUXADC | ADC_CAL_SRC_MAJOR_FLASH | ADC_CAL_SRC_MINOR_GPIO_AND_GND | 0x1;
    if( HAL_AUX_OK != Hal_Aux_AdcCal_StoreFlash() )
        goto done;

    ubRet = HAL_AUX_OK;

done:
    // release the semaphore
    osSemaphoreRelease(g_taHalAux_SemaphoreId);
    return ubRet;
}
