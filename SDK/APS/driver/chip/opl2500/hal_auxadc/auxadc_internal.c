/******************************************************************************
*  Copyright 2017 - 2022, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
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
*  auxadc_internal.c
*
*  Project:
*  --------
*  OPL2500 Project - the AUXADC implement file
*
*  Description:
*  ------------
*  This implement file is include the AUXADC function (for FT use only)
*
*  Author:
*  -------
*  C.C. Wang
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include "auxadc_internal.h"
#include "rf_reg_lite.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define ADC_GAIN    9
#define ADC_RC      1
#define ADC_REF     0

/* 
 * Here for check RF-bank, use for FPGA
 * Three cases cound happen:
 *    1) Real chip     => RF exist
 *    2) FPGA          => not exist
 *    3) FPGA+RF-board => RF exist
 */
#define RF_EXT_BOARD    0
#define RF_REG_EXIST    !( defined( OPL2500_FPGA ) && !RF_EXT_BOARD )

#if( RF_EXT_BOARD )
    #define ADC_AOS    ((S_AOS_Reg_t *) (AOS_BASE+0x0F000000) )
#else
    #define ADC_AOS    AOS
#endif

// Regs not in opl2500_msq.h
// AOS 0xA8
#define AOS_HPBG_TEST_REF_EN         (0x1 << 6)
#define AOS_TEST_PAD_SRC_POS         8
#define AOS_TEST_PAD_SRC_MSK         (0x7 << AOS_TEST_PAD_SRC_POS)


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, union, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
S_AuxAdc_Cfg_t stAuxAdcCfg;

char *pAuxadcSrcName[ AUX_SRC_NUM ] =
{
    "DCDC_IN",  // AUX_SRC_DCDC_IN,
    "VBAT",     // AUX_SRC_VBAT,
    "LDO_VCO",  // AUX_SRC_LDO_VCO,
    "LDO_RF",   // AUX_SRC_LDO_RF,
    "TEMP",     // AUX_SRC_TEMP_SEN,
    "HPBG",     // AUX_SRC_HPBG_REF,
    "VSS",      // AUX_SRC_VSS,
    "VDD_CORE", // AUX_SRC_VDD_CORE,
    "GPIO",     // AUX_SRC_GPIO,
};

// Sec 5: declaration of global function prototype
// ADC
T_Aux_AdcEnable    Aux_AdcEnable  = Aux_AdcEnable_impl;
T_Aux_AdcConfig    Aux_AdcGainSet = Aux_AdcGainSet_impl;
T_Aux_AdcConfig    Aux_AdcRcSet   = Aux_AdcRcSet_impl;
T_Aux_AdcConfig    Aux_AdcRefSet  = Aux_AdcRefSet_impl;
T_Aux_Init         Aux_Init       = Aux_Init_impl;
// ADC single
T_Aux_SourceSelect Aux_SourceSelect = Aux_SourceSelect_impl;
T_Aux_AdcValueGet  Aux_AdcValueGet  = Aux_AdcValueGet_impl;

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
void Aux_AdcEnable_impl( uint8_t u8IsEn )
{
#if( RF_REG_EXIST )
    if( u8IsEn )
    {
        // Rf_Auxadc_En_Update( u8IsEn );
        RF->CLKGEN_CTL0 |= CK_GATE_AUXADC_EN;

        // Enable Pu and clock
        RF->AUX_ADC_CK_GEN_CTL |= RG_PU_AUXADC;

        // Enable AUXADC
        ADC_AOS->ADC_CTL |= AOS_ADC_CTL_AUXADC_EN;
    }else{
        if( !(stAuxAdcCfg.u8Config[0] & AUXADC_CFG_G0_PWR_ALWAYS_ON) )
        {
            // Rf_Auxadc_En_Update( u8IsEn );
            RF->CLKGEN_CTL0 &= ~CK_GATE_AUXADC_EN;

            // Disable AUXADC
            ADC_AOS->ADC_CTL &= ~AOS_ADC_CTL_AUXADC_EN;

            // Disable Pu and clock
            RF->AUX_ADC_CK_GEN_CTL &= ~RG_PU_AUXADC;
        }
    }
#endif
}

/*************************************************************************
* FUNCTION:
*   Aux_AdcGainSet
*
* DESCRIPTION:
*   Set gain of ADC
*
* PARAMETERS
*   u8Var : 
*
* RETURNS
*   none
*
*************************************************************************/
void Aux_AdcGainSet_impl( uint8_t u8Var )
{
#if( RF_REG_EXIST )
    uint32_t u32Temp;

    // Adjust AUXADC performance: Single
    u32Temp = RF->AUXADC_CTRL0;
    u32Temp &= ~RF_AUXADC_CTRL0_AUX_GAIN_TUNE_Msk;
    u32Temp |= u8Var << RF_AUXADC_CTRL0_AUX_GAIN_TUNE_Pos;
    RF->AUXADC_CTRL0 = u32Temp;
#endif
}

/*************************************************************************
* FUNCTION:
*   Aux_AdcRcSet
*
* DESCRIPTION:
*   Set Rc of ADC
*
* PARAMETERS
*   u8Var : 
*
* RETURNS
*   none
*
*************************************************************************/
void Aux_AdcRcSet_impl( uint8_t u8Var )
{
#if( RF_REG_EXIST )
    uint32_t u32Temp;

    // Adjust AUXADC performance: Single
    u32Temp = RF->AUXADC_CTRL0;
    u32Temp &= ~RF_AUXADC_CTRL0_AUX_RC_TUNE_Msk;
    u32Temp |= u8Var << RF_AUXADC_CTRL0_AUX_RC_TUNE_Pos;
    RF->AUXADC_CTRL0 = u32Temp;
#endif
}

/*************************************************************************
* FUNCTION:
*   Aux_AdcRefSet_impl
*
* DESCRIPTION:
*   Set Ref of ADC
*
* PARAMETERS
*   u8Var : 
*
* RETURNS
*   none
*
*************************************************************************/
void Aux_AdcRefSet_impl( uint8_t u8Var )
{
#if( RF_REG_EXIST )
    uint32_t u32Temp;

    // Adjust AUXADC performance: Single
    u32Temp = RF->AUXADC_CTRL0;
    u32Temp &= ~RF_AUXADC_CTRL0_AUX_REF_ITUNE_Msk;
    u32Temp |= u8Var << RF_AUXADC_CTRL0_AUX_REF_ITUNE_Pos;
    RF->AUXADC_CTRL0 = u32Temp;
#endif
}

/*************************************************************************
* FUNCTION:
*   Aux_Init
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
void Aux_Init_impl(void)
{
#if( RF_REG_EXIST )
    uint32_t tmp;

    // Disable AUXADC
    ADC_AOS->ADC_CTL &= ~AOS_ADC_CTL_AUXADC_EN;

    // AUXADC and HPBG bias
    tmp = ADC_AOS->ADC_CTL;
    tmp &= ~( AOS_ADC_CTL_AUX_REF_VTUNE_Msk | AOS_ADC_CTL_ADC_IBIAS_Msk );
    tmp |= ( (0x10 << AOS_ADC_CTL_AUX_REF_VTUNE_Pos) | (0x10 << AOS_ADC_CTL_ADC_IBIAS_Pos));
    ADC_AOS->ADC_CTL = tmp;
    
    // Disable all the internal sources
    RF->AUXADC_CTRL1 &= ~RF_AUXADC_CTRL1_TEST_MUX_VCO_LDO_EN; // LDO_VCO
    ADC_AOS->HPBG_CTL &= ~AOS_HPBG_CTL_TEMP_SEN_EN;               // TEMP_SEN
    ADC_AOS->PMU_CTRL &= ~AOS_HPBG_TEST_REF_EN;                   // HPBG
#endif

    // Turn off AUXADC
    Aux_AdcEnable( 0 );

    // Adjust AUXADC performance
    Aux_AdcGainSet( ADC_GAIN );
    Aux_AdcRcSet( ADC_RC );
    Aux_AdcRefSet( ADC_REF );

    // Set default input to AUXADC
    Aux_SourceSelect( AUX_SRC_TEMP_SEN, 0 );

    // Init config
    stAuxAdcCfg.u8CurrSrc   = AUX_SRC_MAX;
    stAuxAdcCfg.u8Config[0] = 0;
    stAuxAdcCfg.u16AvgCnt       = 30;
}

/*************************************************************************
* FUNCTION:
*   Aux_SourceSelect
*
* DESCRIPTION:
*   select the source of AUXADC
*
* PARAMETERS
*   1. tSrc      : [In] the source type of AUXADC
*   2. ubGpioIdx : [In] the index of GPIO
*
* RETURNS
*   1. AUXADC_OK : success
*   2. AUXADC_NG : fail
*
*************************************************************************/
uint8_t Aux_SourceSelect_impl(E_Aux_Src_t tSrc, uint8_t ubGpioIdx)
{
    // check the source type
    if (tSrc >= AUX_SRC_MAX)
        return AUXADC_NG;

    // check the index of GPIO
    if (tSrc == AUX_SRC_GPIO)
    {
        if (ubGpioIdx >= AUX_GPIO_NUM_MAX)
            return AUXADC_NG;
    }

#if( RF_REG_EXIST )
    uint32_t tmp;

    // Select GPIO_01 or internal MUX
    if (tSrc == AUX_SRC_GPIO)
    {
        // Switch MUX to GPIO
        RF->AUXADC_CTRL0 &= ~RF_AUXADC_CTRL0_AUX_TEMP_SENS_EN;
    }
    else
    {
        // Select internal sources
        tmp = ADC_AOS->PMU_CTRL;
        tmp &= ~AOS_TEST_PAD_SRC_MSK;
        tmp |= (tSrc << AOS_TEST_PAD_SRC_POS);
        ADC_AOS->PMU_CTRL = tmp;

        // Switch MUX to internal sources
        RF->AUXADC_CTRL0 |= RF_AUXADC_CTRL0_AUX_TEMP_SENS_EN;

        // Fix to channel 01
        ubGpioIdx = 1;
    }

    // Select the IO channel
    tmp = RF->AUXADC_IN_SEL;
    tmp &= ~AUX_IN_SEL_MSK;
    tmp |= (ubGpioIdx << AUX_IN_SEL_POS);
    RF->AUXADC_IN_SEL = tmp;
#endif

    stAuxAdcCfg.u8CurrSrc = tSrc;
    return AUXADC_OK;
}

/*************************************************************************
* FUNCTION:
*   Aux_AdcValueGet
*
* DESCRIPTION:
*   get the ADC value from AUXADC
*
* PARAMETERS
*   1. pulValue : [Out] the ADC value
*
* RETURNS
*   1. AUXADC_OK : success
*   2. AUXADC_NG : fail
*
*************************************************************************/
uint8_t Aux_AdcValueGet_impl(uint32_t *pulValue)
{
    uint8_t u8Ret = AUXADC_OK;

#if( RF_REG_EXIST )
    // Enable the internal path
    if (stAuxAdcCfg.u8CurrSrc == AUX_SRC_LDO_VCO)
        RF->AUXADC_CTRL1 |= RF_AUXADC_CTRL1_TEST_MUX_VCO_LDO_EN;
    else if (stAuxAdcCfg.u8CurrSrc == AUX_SRC_TEMP_SEN)
        ADC_AOS->HPBG_CTL |= AOS_HPBG_CTL_TEMP_SEN_EN;
    else if (stAuxAdcCfg.u8CurrSrc == AUX_SRC_HPBG_REF)
        ADC_AOS->PMU_CTRL |= AOS_HPBG_TEST_REF_EN;

    // Repeat got ADC value
    uint32_t u32RepeatIdx = 0;
    uint32_t u32Temp = 0;
    for( u32RepeatIdx = 0; u32RepeatIdx < stAuxAdcCfg.u16AvgCnt; u32RepeatIdx++ )
    {
        // Trigger
        RF->AUX_ADC_CK_GEN_CTL &= ~RC_CONVSTB_EN;
        RF->AUX_ADC_CK_GEN_CTL |= RC_CONVSTB_EN;

        // get the ADC value
        uint32_t u32WaitCnt = 0;
        while (RF->AUX_ADC_ECL & AUX_EOCB_FINISH)
        {
            if(u32WaitCnt >= 0xFF)
            {
                u8Ret =  AUXADC_NG;
                goto ADC_GOT_VALUE_DONE;
            }
            u32WaitCnt++;
        }
        u32Temp += RF->AUX_ADC_ECL & AUX_ADC_ECL_OUT_MSK;
    }
    *pulValue = DIV_ROUND( u32Temp, stAuxAdcCfg.u16AvgCnt );

ADC_GOT_VALUE_DONE:
    if( stAuxAdcCfg.u8Config[0] & AUXADC_CFG_G0_SHOWLOG )
    {
        printf("Repeat_Idx = %d, Accu = 0x%08X\n\r", u32RepeatIdx, u32Temp);
    }

    // Idle (non-trigger)
    RF->AUX_ADC_CK_GEN_CTL &= ~RC_CONVSTB_EN;

    if( !(stAuxAdcCfg.u8Config[0] & AUXADC_CFG_G0_PWR_ALWAYS_ON) )
    {
        // Disable the internal path
        if (stAuxAdcCfg.u8CurrSrc == AUX_SRC_LDO_VCO)
            RF->AUXADC_CTRL1 &= ~RF_AUXADC_CTRL1_TEST_MUX_VCO_LDO_EN;
        else if (stAuxAdcCfg.u8CurrSrc == AUX_SRC_TEMP_SEN)
            ADC_AOS->HPBG_CTL &= ~AOS_HPBG_CTL_TEMP_SEN_EN;
        else if (stAuxAdcCfg.u8CurrSrc == AUX_SRC_HPBG_REF)
            ADC_AOS->PMU_CTRL &= ~AOS_HPBG_TEST_REF_EN;
    }
#else
    // Fake value
    *pulValue = stAuxAdcCfg.u8CurrSrc << 4;
#endif
    return u8Ret;
}
