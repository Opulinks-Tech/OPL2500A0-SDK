/******************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *******************************************************************************
 * @file hal_pin.h
 * 
 * @brief API to control GPIO
 * 
 */


#ifndef __HAL_PIN_H__
#define __HAL_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
//#include "hal_vic.h"
#include "hal_gpio.h"
#include "hal_pin_def.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define HAL_PIN_IO_COUNT    40

#define HAL_PIN_OK          1
#define HAL_PIN_FAIL        0

    
    
#define PINMUX_TYPE_INPUT       0
#define PINMUX_TYPE_OUTPUT      1
#define PINMUX_TYPE_DISABLE     2



#define PTS_PINMUX_REG_F                     5
#define PTS_PINMUX_REG_PSRAM                 PTS_PINMUX_REG_F

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

/* Wrapper for OPL1000 */
typedef enum {
    PIN_TYPE_GPIO_INPUT,
    PIN_TYPE_GPIO_OUTPUT_LOW,       // the default output is low
    PIN_TYPE_GPIO_OUTPUT_HIGH,      // the default output is high
}E_GPIO_INOUT_MODE;
/* Wrapper for OPL1000 */
typedef enum
{
    PIN_DRIVING_FLOAT   = GPIO_LEVEL_FLOATING,      // floating
    
    PIN_DRIVING_LOW     = GPIO_LEVEL_LOW,           // pull low
    PIN_DRIVING_HIGH    = GPIO_LEVEL_HIGH           // pull high
    
} E_PIN_DRIVING;


typedef enum {
    PIN_DIR_OUTPUT_ENABLE           = 0,    /* DIR: Active low                              */
    PIN_DIR_OUTPUT_DISABLE          = 1,    /* DIR: Active low                              */
}E_PIN_DIR_OEN;


typedef enum {
    PIN_PDOC_FROM_GPIO              = 0,    /* Control by PD_DIR                            */
    PIN_PDOC_FROM_PERIPH            = 1,    /* Control by pinmux from peripherals           */
}E_PIN_PDOC_MODE;


typedef enum {
    PIN_PDOV_FROM_GPO               = 0,    /* Value from GPO                               */
    PIN_PDOV_FROM_PERIPH            = 1,    /* Value from pinmux (peripherals)              */
}E_PIN_PDOV_MODE;


/* SiP PSRAM configuration */
typedef enum {
    PSRAM_KGD_DISABLE               = 0 << PIN_PTS_INMUX_F_PSRAM_KGD_MOD_Pos,
    PSRAM_KGD_ENABLE_CS1            = (1 << PIN_PTS_INMUX_F_PSRAM_KGD_MOD_Pos) | (0 << PIN_PTS_INMUX_F_PSRAM_CS_SEL_Pos),
    PSRAM_KGD_ENABLE_CS2            = (1 << PIN_PTS_INMUX_F_PSRAM_KGD_MOD_Pos) | (1 << PIN_PTS_INMUX_F_PSRAM_CS_SEL_Pos),
    PSRAM_KGD_ENABLE_CS3            = (1 << PIN_PTS_INMUX_F_PSRAM_KGD_MOD_Pos) | (2 << PIN_PTS_INMUX_F_PSRAM_CS_SEL_Pos),
    PSRAM_KGD_MSK                   = PIN_PTS_INMUX_F_PSRAM_KGD_MOD_Msk | PIN_PTS_INMUX_F_PSRAM_CS_SEL_Msk,
} E_SIP_PSRAM_CFG;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable

// Sec 5: declaration of global function prototype
// External
typedef uint8_t (*T_Hal_Pin_Config_Fp)(uint64_t u64PinCfg);
typedef uint8_t (*T_Hal_Pin_ConfigSet_Fp)(uint8_t ubIoIdx, uint16_t uwConfig, uint8_t ubDriving);
typedef void (*T_Hal_Pin_UpdatePsramCfg)(void);
extern T_Hal_Pin_Config_Fp Hal_Pin_Config;
extern T_Hal_Pin_ConfigSet_Fp Hal_Pin_ConfigSet;
extern T_Hal_Pin_UpdatePsramCfg Hal_Pin_UpdatePsramCfg;
uint8_t Hal_Pin_ConfigSet_impl(uint8_t ubIoIdx, uint16_t uwConfig, uint8_t ubDriving);
uint8_t Hal_Pin_Config_impl(uint64_t u64PinCfg);
void Hal_Pin_UpdatePsramCfg_impl(void);
// Internal


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype
#ifdef __cplusplus
}
#endif
#include "hal_pin_patch.h"
#endif  // end of __HAL_PIN_H__
