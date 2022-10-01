/* *****************************************************************************
 *  Copyright 2017 - 2021, Opulinks Technology Ltd.
 *  ----------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 ******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  hal_pin.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the patch proto-types of system functions
*
*  Author:
*  -------
*  Jeff
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "hal_pin_def.h"
#include "hal_pin.h"
#include "hal_vic.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
// External
T_Hal_Pin_Config_Fp         Hal_Pin_Config          = Hal_Pin_Config_impl;
T_Hal_Pin_ConfigSet_Fp      Hal_Pin_ConfigSet       = Hal_Pin_ConfigSet_impl;
T_Hal_Pin_UpdatePsramCfg    Hal_Pin_UpdatePsramCfg  = Hal_Pin_UpdatePsramCfg_impl;


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype

/***********
C Functions
***********/
// Sec 8: C Functions


/*************************************************************************
* FUNCTION:
*  Hal_Pin_ConfigSet
*
* DESCRIPTION:
*   1. Wrapper for OPL1000
*   2. Only support GPIO input/output
*
* PARAMETERS
*   1. ubIoIdx   : [In] the index of IO
*   2. uwConfig  : [In] the type config of IO
*   3. ubDriving : [In] the driving of IO
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_ConfigSet_impl(uint8_t ubIoIdx, uint16_t uwConfig, uint8_t ubDriving)
{
    switch (uwConfig)
    {
        case PIN_TYPE_GPIO_INPUT:
            Hal_Gpio_Pinmux((E_GpioIdx_t)ubIoIdx, GPIO_INPUT, (E_GpioLevel_t)ubDriving);
            break;
        case PIN_TYPE_GPIO_OUTPUT_LOW:
            Hal_Gpio_Pinmux((E_GpioIdx_t)ubIoIdx, GPIO_OUTPUT, GPIO_LEVEL_LOW);
            break;
        case PIN_TYPE_GPIO_OUTPUT_HIGH:
            Hal_Gpio_Pinmux((E_GpioIdx_t)ubIoIdx, GPIO_OUTPUT, GPIO_LEVEL_HIGH);
            break;
        default:
            return HAL_PIN_FAIL;
    }
    
    return HAL_PIN_OK;
}


/*************************************************************************
* FUNCTION:
*  Hal_Pin_Config
*
* DESCRIPTION:
*   1. the configuration of PinMux
*
* PARAMETERS
*   1. u64PinCfg   : [In] Pin configuration value.
*
* RETURNS
*   1. HAL_PIN_OK   : success
*   2. HAL_PIN_FAIL : fail
*
*************************************************************************/
uint8_t Hal_Pin_Config_impl(uint64_t u64PinCfg)
{
    S_GPIO_PIN_CFG tPinCfg;
    volatile uint32_t *pu32GrpReg;
    uint32_t u32GrpBit, u32RegValue;
    
    tPinCfg.u64Value = u64PinCfg;
    if (tPinCfg.IsSipIo)
    {   /* SiP IO, for internal pad */
        REG_BB_WRITE(&PIN->PSRAM_CONTROL_1, tPinCfg.IoNum + PIN_PSRAM_CONTROL_1_PD_IE_PSRAM_39_44_Pos,      0);      /* Disable Input enable */
        /* Output */
        REG_BB_WRITE(&PIN->PSRAM_CONTROL_2, tPinCfg.IoNum + PIN_PSRAM_CONTROL_2_PD_I_INV_PSRAM_39_44_Pos,   tPinCfg.OutInverter);   /* Output inverter */
        REG_BB_WRITE(&PIN->PSRAM_CONTROL_2, tPinCfg.IoNum + PIN_PSRAM_CONTROL_2_PD_OEN_INV_PSRAM_39_44_Pos, 0);                     /* Disable OEN inverter */

        if (tPinCfg.OutEnable == PIN_FUNC_ENABLE)
        {   /* Output enable */
            REG_BB_WRITE(&PIN->PSRAM_CONTROL_2, tPinCfg.IoNum + PIN_PSRAM_CONTROL_2_GPO_PSRAM_39_44_Pos,    tPinCfg.OutLevel);          /* Output low level */
            REG_BB_WRITE(&PIN->PSRAM_CONTROL_1, tPinCfg.IoNum + PIN_PSRAM_CONTROL_1_PD_DS_PSRAM_39_44_Pos,  tPinCfg.OutIoDrv);          /* Configure driving strength */
            if (tPinCfg.IoType == PIN_IO_TYPE_PERIPH)
            {
                REG_BB_WRITE(&PIN->PSRAM_CONTROL_3, tPinCfg.IoNum + PIN_PSRAM_CONTROL_3_PDOC_PSRAM_39_44_Pos, PIN_PDOC_FROM_PERIPH);    /* Control by pinmux */
                REG_BB_WRITE(&PIN->PSRAM_CONTROL_3, tPinCfg.IoNum + PIN_PSRAM_CONTROL_3_PDOV_PSRAM_39_44_Pos, PIN_PDOV_FROM_PERIPH);    /* Control Output value from pinmux */
            }
            else
            {
                REG_BB_WRITE(&PIN->PSRAM_CONTROL_3, tPinCfg.IoNum + PIN_PSRAM_CONTROL_3_PDOC_PSRAM_39_44_Pos, PIN_PDOC_FROM_GPIO);      /* Control by DIR */
                REG_BB_WRITE(&PIN->PSRAM_CONTROL_3, tPinCfg.IoNum + PIN_PSRAM_CONTROL_3_PDOV_PSRAM_39_44_Pos, PIN_PDOV_FROM_GPO);       /* Control Output value from GPO */
                REG_BB_WRITE(&PIN->PSRAM_CONTROL_2, tPinCfg.IoNum + PIN_PSRAM_CONTROL_2_PD_DIR_PSRAM_39_44_Pos, PIN_DIR_OUTPUT_ENABLE); /* Enable output */
            }
        }
        else
        {   /* Output Disable */
            REG_BB_WRITE(&PIN->PSRAM_CONTROL_3, tPinCfg.IoNum + PIN_PSRAM_CONTROL_3_PDOC_PSRAM_39_44_Pos,   PIN_PDOC_FROM_GPIO);        /* Control by DIR */
            REG_BB_WRITE(&PIN->PSRAM_CONTROL_2, tPinCfg.IoNum + PIN_PSRAM_CONTROL_2_PD_DIR_PSRAM_39_44_Pos, PIN_DIR_OUTPUT_DISABLE);    /* Turn off output */
        }
        /* Input enable */
        REG_BB_WRITE(&PIN->PSRAM_CONTROL_1, tPinCfg.IoNum + PIN_PSRAM_CONTROL_1_PD_O_INV_PSRAM_39_44_Pos,   tPinCfg.InInverter);        /* Input inverter */
        REG_BB_WRITE(&PIN->PSRAM_CONTROL_1, tPinCfg.IoNum + PIN_PSRAM_CONTROL_1_PD_PE_PSRAM_39_44_Pos,      tPinCfg.IoPE);              /* IO pull resistor enable */
        REG_BB_WRITE(&PIN->PSRAM_CONTROL_1, tPinCfg.IoNum + PIN_PSRAM_CONTROL_1_PD_IE_PSRAM_39_44_Pos,      tPinCfg.InEnable);          /* Input enable */
        Hal_Gpio_SleepIoAutoCtrlSet((E_GpioIdx_t)(tPinCfg.IoNum + GPIO_SIP_START_IDX), SLEEP_OUTPUT_DISABLE);
    }
    else
    {   /* External IO pad */
        if (tPinCfg.IoNum != PIN_IO_NUM_DIS)
        {   /* IO number not disabled, setup input/output configuration */       
            REG_BB_WRITE(PIN->PD_IE,    tPinCfg.IoNum, 0);       /* Disable Input enable */
            
            /* Output */
            REG_BB_WRITE(&PIN->PD_I_INV, tPinCfg.IoNum, tPinCfg.OutInverter);   /* Output inverter */
            REG_BB_WRITE(&PIN->PD_OEN_INV, tPinCfg.IoNum, 0);                   /* Disable OEN inverter */

            if (tPinCfg.OutEnable == PIN_FUNC_ENABLE)
            {   /* Output enable */
                REG_BB_WRITE((tPinCfg.OutLevel == PIN_OUT_LEVEL_LOW)?&PIN->GPO_LOW:&PIN->GPO_HIGH, tPinCfg.IoNum, 1);       /* Output low level */
                REG_BB_WRITE(&PIN->PD_DS,      tPinCfg.IoNum, tPinCfg.OutIoDrv);                                            /* Configure driving strength */
                if (tPinCfg.IoType == PIN_IO_TYPE_PERIPH)
                {   /* From pin-mux table */
                    REG_BB_WRITE(&PIN->PDOC_MODE,  tPinCfg.IoNum, PIN_PDOC_FROM_PERIPH);    /* Controlled by pinmux */
                    REG_BB_WRITE(&PIN->PDOV_MODE,  tPinCfg.IoNum, PIN_PDOV_FROM_PERIPH);    /* Control Output value from pinmux */
                }
                else
                {   /* GPIO */
                    REG_BB_WRITE(&PIN->PDOC_MODE,  tPinCfg.IoNum, PIN_PDOC_FROM_GPIO);      /* Control by DIR */
                    REG_BB_WRITE(&PIN->PDOV_MODE,  tPinCfg.IoNum, PIN_PDOV_FROM_GPO);       /* Control Output value from  GPO */
                    REG_BB_WRITE(&PIN->PD_DIR,     tPinCfg.IoNum, PIN_DIR_OUTPUT_ENABLE);   /* Enable output */
                }
            }
            else
            {   /* Output Disable */
                REG_BB_WRITE(&PIN->PDOC_MODE,   tPinCfg.IoNum, PIN_PDOC_FROM_GPIO);         /* Control by DIR */
                REG_BB_WRITE(&PIN->PD_DIR,      tPinCfg.IoNum, PIN_DIR_OUTPUT_DISABLE);     /* Disable output */
            }
            
            /* Input enable */
            REG_BB_WRITE(PIN->PD_IE,    tPinCfg.IoNum, tPinCfg.InEnable);       /* Input enable */
            REG_BB_WRITE(PIN->PD_O_INV, tPinCfg.IoNum, tPinCfg.InInverter);     /* Input inverter */
            REG_BB_WRITE(PIN->PD_PE,    tPinCfg.IoNum, tPinCfg.IoPE);           /* IO pull resistor enable */
            Hal_Gpio_SleepIoAutoCtrlSet((E_GpioIdx_t)tPinCfg.IoNum, SLEEP_OUTPUT_DISABLE);
        }
    }
    
    if (tPinCfg.IoType == PIN_IO_TYPE_PERIPH)
    {
        if (tPinCfg.InGrpOfst)
        {   /* In group offset not zero. Setup peripheral input pinmux */
            pu32GrpReg = (volatile uint32_t *)PIN_REG_GRP_OFST_TO_REG(tPinCfg.InGrpOfst);
            u32GrpBit = PIN_REG_GRP_OFST_TO_BIT(tPinCfg.InGrpOfst);
            u32RegValue = *pu32GrpReg & (~ (PIN_REG_GRP_BITS_MASK << u32GrpBit));
            u32RegValue |= tPinCfg.InGrpValue << u32GrpBit;
            *pu32GrpReg = u32RegValue;
        }
        
        if (tPinCfg.OutGrpOfst)
        {   /* Out group offset not zero. Setup IO pad output pinmux */
            pu32GrpReg = (volatile uint32_t *)PIN_REG_GRP_OFST_TO_REG(tPinCfg.OutGrpOfst);
            u32GrpBit = PIN_REG_GRP_OFST_TO_BIT(tPinCfg.OutGrpOfst);
            u32RegValue = *pu32GrpReg & (~ (PIN_REG_GRP_BITS_MASK << u32GrpBit));
            u32RegValue |= tPinCfg.OutGrpValue << u32GrpBit;
            *pu32GrpReg = u32RegValue;
            
            if (tPinCfg.AutoCtrlEn)
            {   /* Auto control enabled */
                Hal_Gpio_SleepIoAutoCtrlSet((E_GpioIdx_t)(tPinCfg.IoNum + (tPinCfg.IsSipIo?GPIO_SIP_START_IDX:0)), (tPinCfg.AutoCtrlLvl)?SLEEP_OUTPUT_ENABLE_HIGH:SLEEP_OUTPUT_ENABLE_LOW);
            }
        }
    }
    
    return HAL_PIN_OK;
}




/*************************************************************************
* FUNCTION:
*  Hal_Pin_UpdatePsramCfg
*
* DESCRIPTION:
*   1. Update PSRAM config by IO43 chip select pin assignment
*
* PARAMETERS
*
* RETURNS
*
*************************************************************************/
void Hal_Pin_UpdatePsramCfg_impl(void)
{
    uint8_t u8IoPsramCsDef;
    uint32_t u32PsramKgdCfg;
    
    /* Get PSRAM Chip select IO43 definition */
    u8IoPsramCsDef = (PIN->PD_I_SEQ_SEL & PIN_PD_I_SEQ_SEL_PDI_SRC_IO43_Msk) >> PIN_PD_I_SEQ_SEL_PDI_SRC_IO43_Pos;
    
    switch (u8IoPsramCsDef)
    {
        case PIN_OUTGRP_VAL_IO43_SPI0_CS1:
            u32PsramKgdCfg = PSRAM_KGD_ENABLE_CS1;
            break;
        case PIN_OUTGRP_VAL_IO43_SPI0_CS2:
            u32PsramKgdCfg = PSRAM_KGD_ENABLE_CS2;
            break;
        case PIN_OUTGRP_VAL_IO43_SPI0_CS3:
            u32PsramKgdCfg = PSRAM_KGD_ENABLE_CS3;
            break;
        default:
            u32PsramKgdCfg = PSRAM_KGD_DISABLE;
    }
    
    PIN->PTS_INMUX_F = (PIN->PTS_INMUX_F & ~PSRAM_KGD_MSK) | u32PsramKgdCfg;
}

