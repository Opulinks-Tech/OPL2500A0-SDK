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
*  main_patch.c
*
*  Project:
*  --------
*  OPL2500 Project - the main patch implement file
*
*  Description:
*  ------------
*  This implement file is include the main patch function and api.
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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mw_ota.h"
#include "mw_ota_boot.h"
#include "boot_sequence.h"
#include "hal_uart.h"
#include "hal_pin.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define SYS_UART_SCLK_FON_MSK                       (SYS_UART_SCLK_FON_UART0_CLK_FON_Msk | \
                                                     SYS_UART_SCLK_FON_UART1_CLK_FON_Msk | \
                                                     SYS_UART_SCLK_FON_APS_UART_CLK_FON_Msk | \
                                                     SYS_UART_SCLK_FON_MSQ_UART_CLK_FON_Msk)
#define SYS_UART_SCLK_FON_SET_VALUE                 (0)

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern unsigned int Image$$RW_IRAM1$$ZI$$Length;
extern char Image$$RW_IRAM1$$ZI$$Base[];


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype
void BA_main(void)  __attribute__((used, section("BA_MAIN")));

#ifdef OPL2500_EXT_FLASH
void BA_PinmuxUpdate(void);
#endif /* OPL2500_EXT_FLASH */

/***********
C Functions
***********/
// Sec 8: C Functions

/*************************************************************************
* FUNCTION:
*   BA_main
*
* DESCRIPTION:
*   the entry point of boot agent
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void BA_main(void)
{
    // for cold boot only
    if (0 != Boot_CheckWarmBoot())
        return;
    
    MwOta_PatchInit();
#ifdef OPL2500_EXT_FLASH
    BA_PinmuxUpdate();
    MwOta_ExtFlashInit(SPI_SLAVE_1, MW_OTA_EXT_IMAGE_ADDR_PATCH_START, MW_OTA_EXT_IMAGE_SIZE_PATCH);
#endif /* OPL2500_EXT_FLASH */
    
    Hal_Uart_PatchInit();
    SYS->UART_SCLK_FON = (SYS->UART_SCLK_FON & ~SYS_UART_SCLK_FON_MSK) | SYS_UART_SCLK_FON_SET_VALUE;

    // init bss section
    memset(Image$$RW_IRAM1$$ZI$$Base, 0, (unsigned int)&Image$$RW_IRAM1$$ZI$$Length);
    
    // OTA
    if (MW_OTA_OK == MwOta_Boot_Init())
    {
        // the UART action
        if (0 == MwOta_Boot_CheckUartBehavior())
        {
            // load the patch image
            MwOta_Boot_LoadPatchImage();
        }
    }
    
    // !!!
    // it is needed to rollback the SW patch
    // the memory space will be used for dynamical memory pool
    Boot_BeforeApplyPatch = Boot_BeforeApplyPatch_impl;
}

#ifdef OPL2500_EXT_FLASH
void BA_PinmuxUpdate(void)
{
    /* Update Pinmux for external flash here */
    Hal_Pin_Config(PIN_TYPE_SPI0_CS1_IO3  | PIN_DRVCRNT_IO3_4mA | PIN_INMODE_IO3_FLOATING);
    Hal_Pin_Config(PIN_TYPE_SPI0_IO2_IO10 | PIN_DRVCRNT_IO10_8mA | PIN_INMODE_IO10_PULL_UP);
    Hal_Pin_Config(PIN_TYPE_SPI0_IO3_IO11 | PIN_DRVCRNT_IO11_8mA | PIN_INMODE_IO11_PULL_UP);
    Hal_Pin_Config(PIN_TYPE_SPI0_CLK_IO13 | PIN_DRVCRNT_IO13_8mA | PIN_INMODE_IO13_FLOATING);
    Hal_Pin_Config(PIN_TYPE_SPI0_IO0_IO14 | PIN_DRVCRNT_IO14_8mA | PIN_INMODE_IO14_FLOATING);
    Hal_Pin_Config(PIN_TYPE_SPI0_IO1_IO15 | PIN_DRVCRNT_IO15_8mA | PIN_INMODE_IO15_FLOATING);
}
#endif /* OPL2500_EXT_FLASH */
