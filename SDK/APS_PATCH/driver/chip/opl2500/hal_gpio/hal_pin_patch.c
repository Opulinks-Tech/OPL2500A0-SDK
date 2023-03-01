/* *****************************************************************************
 *  Copyright 2017 - 2022, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2022
 *
 *******************************************************************************
 *
 *  @file hal_pin_patch.c
 * 
 *  @brief PIN function patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_pin.h"
#include "hal_pin_def_patch.h"


/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
uint8_t g_u8Gpio_TxEn  = 0xFF; // Note: GPIO_00 ~ 38
uint8_t g_u8Gpio_RxEn  = 0xFF; // Note: GPIO_00 ~ 38
uint8_t g_u8Gpio_LnaEn = 0xFF; // Note: GPIO_00 ~ 38

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 




 
 
 
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
uint8_t Hal_Pin_ConfigSet_patch(uint8_t ubIoIdx, uint16_t uwConfig, uint8_t ubDriving)
{
    // Prohibited Eext-PA pins change
    if( (ubIoIdx == g_u8Gpio_TxEn) | (ubIoIdx == g_u8Gpio_RxEn) | (ubIoIdx == g_u8Gpio_LnaEn) )
    {
        printf("Assert: GPIO_%d was used in ext-PA.\n", ubIoIdx);
        while(1);
    }else
        return Hal_Pin_ConfigSet_impl(ubIoIdx, uwConfig, ubDriving);
}

uint8_t Hal_Pin_Config_patch(uint64_t u64PinCfg)
{
    S_GPIO_PIN_CFG tPinCfg;
    tPinCfg.u64Value = u64PinCfg;

    // Prohibited Eext-PA pins change
    if( (tPinCfg.IoNum == g_u8Gpio_TxEn) | (tPinCfg.IoNum == g_u8Gpio_RxEn) | (tPinCfg.IoNum == g_u8Gpio_LnaEn) )
    {
        printf("Assert: GPIO_%d was used in ext-PA.\n", tPinCfg.IoNum);
        while(1);
    }else
        return Hal_Pin_Config_impl(u64PinCfg);
}

void Hal_SysDisableAllTxPeriphPinmux(void)
{
    PIN->PDI_SRC_IO[0] = (PIN_OUTGRP_VAL_IO0_PERIPH_LOW << 0  ) |
                         (PIN_OUTGRP_VAL_IO1_PERIPH_LOW << 4  ) |
                         (PIN_OUTGRP_VAL_IO2_PERIPH_LOW << 8  ) |
                         (PIN_OUTGRP_VAL_IO3_PERIPH_LOW << 12 ) |
                         (PIN_OUTGRP_VAL_IO4_PERIPH_LOW << 16 ) |
                         (PIN_OUTGRP_VAL_IO5_PERIPH_LOW << 20 ) |
                         (PIN_OUTGRP_VAL_IO6_PERIPH_LOW << 24 ) |
                         (PIN_OUTGRP_VAL_IO7_PERIPH_LOW << 28 );
    PIN->PDI_SRC_IO[1] = (PIN_OUTGRP_VAL_IO8_PERIPH_LOW  << 0  ) |
                         (PIN_OUTGRP_VAL_IO9_PERIPH_LOW  << 4  ) |
                         (PIN_OUTGRP_VAL_IO10_PERIPH_LOW << 8  ) |
                         (PIN_OUTGRP_VAL_IO11_PERIPH_LOW << 12 ) |
                         (PIN_OUTGRP_VAL_IO12_PERIPH_LOW << 16 ) |
                         (PIN_OUTGRP_VAL_IO13_PERIPH_LOW << 20 ) |
                         (PIN_OUTGRP_VAL_IO14_PERIPH_LOW << 24 ) |
                         (PIN_OUTGRP_VAL_IO15_PERIPH_LOW << 28 );
    PIN->PDI_SRC_IO[2] = (PIN_OUTGRP_VAL_IO16_PERIPH_LOW << 0 ) |
                         (PIN_OUTGRP_VAL_IO17_PERIPH_LOW << 4 ) |
                         (PIN_OUTGRP_VAL_IO18_PERIPH_LOW << 8 ) |
                         (PIN_OUTGRP_VAL_IO19_PERIPH_LOW << 12) |
                         (PIN_OUTGRP_VAL_IO20_PERIPH_LOW << 16) |
                         (PIN_OUTGRP_VAL_IO21_PERIPH_LOW << 20) |
                         (PIN_OUTGRP_VAL_IO22_PERIPH_LOW << 24) |
                         (PIN_OUTGRP_VAL_IO23_PERIPH_LOW << 28);

    PIN->PDI_SRC_IO[3] = (PIN_OUTGRP_VAL_IO24_PERIPH_LOW << 0 ) |
                         (PIN_OUTGRP_VAL_IO25_PERIPH_LOW << 4 ) |
                         (PIN_OUTGRP_VAL_IO26_PERIPH_LOW << 8 ) |
                         (PIN_OUTGRP_VAL_IO27_PERIPH_LOW << 12) |
                         (PIN_OUTGRP_VAL_IO28_PERIPH_LOW << 16) |
                         (PIN_OUTGRP_VAL_IO29_PERIPH_LOW << 20) |
                         (PIN_OUTGRP_VAL_IO30_PERIPH_LOW << 24) |
                         (PIN_OUTGRP_VAL_IO31_PERIPH_LOW << 28);

    PIN->PDI_SRC_IO[4] = (PIN_OUTGRP_VAL_IO32_PERIPH_LOW << 0 ) |
                         (PIN_OUTGRP_VAL_IO33_PERIPH_LOW << 4 ) |
                         (PIN_OUTGRP_VAL_IO34_PERIPH_LOW << 8 ) |
                         (PIN_OUTGRP_VAL_IO35_PERIPH_LOW << 12) |
                         (PIN_OUTGRP_VAL_IO36_PERIPH_LOW << 16) |
                         (PIN_OUTGRP_VAL_IO37_PERIPH_LOW << 20) |
                         (PIN_OUTGRP_VAL_IO38_PERIPH_LOW << 24) |
                         (PIN_OUTGRP_VAL_IO39_PERIPH_LOW << 28);

    PIN->PD_I_SEQ_SEL = (PIN_OUTGRP_VAL_IO40_PERIPH_LOW << PIN_PD_I_SEQ_SEL_PDI_SRC_IO40_Pos) |
                        (PIN_OUTGRP_VAL_IO41_PERIPH_LOW << PIN_PD_I_SEQ_SEL_PDI_SRC_IO41_Pos) |
                        (PIN_OUTGRP_VAL_IO42_PERIPH_LOW << PIN_PD_I_SEQ_SEL_PDI_SRC_IO42_Pos) |
                        (PIN_OUTGRP_VAL_IO43_PERIPH_LOW << PIN_PD_I_SEQ_SEL_PDI_SRC_IO43_Pos) |
                        (PIN_OUTGRP_VAL_IO44_PERIPH_LOW << PIN_PD_I_SEQ_SEL_PDI_SRC_IO44_Pos);
}

void Hal_ExtPa_Pin_Set(uint8_t u8Gpio_TxEn, uint8_t u8Gpio_RxEn, uint8_t u8Gpio_LnaEn)
{
    if( (u8Gpio_TxEn > GPIO_IDX_OUT_MAX) | (u8Gpio_RxEn > GPIO_IDX_OUT_MAX) | (u8Gpio_LnaEn > GPIO_IDX_OUT_MAX) )
    {
        g_u8Gpio_TxEn  = 0xFF;
        g_u8Gpio_RxEn  = 0xFF;
        g_u8Gpio_LnaEn = 0xFF;

        Hal_Pin_ConfigSet = Hal_Pin_ConfigSet_impl;
        Hal_Pin_Config    = Hal_Pin_Config_impl;
    }else{
        g_u8Gpio_TxEn  = u8Gpio_TxEn;
        g_u8Gpio_RxEn  = u8Gpio_RxEn;
        g_u8Gpio_LnaEn = u8Gpio_LnaEn;

        Hal_Pin_ConfigSet = Hal_Pin_ConfigSet_patch;
        Hal_Pin_Config    = Hal_Pin_Config_patch;
    }
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

