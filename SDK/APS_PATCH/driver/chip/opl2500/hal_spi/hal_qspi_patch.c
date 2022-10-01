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
 *  @file hal_qspi_patch.c
 * 
 *  @brief QSPI API patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_qspi.h"
#include "hal_tick.h"
#include "opl2500.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define QSPI_WAIT_IDLE_TIMEOUT_MS  2
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
void Hal_Qspi_Init_patch(E_SpiSlave_t eSlvIdx, uint32_t u32Sclk);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 


uint32_t u32Hal_QspiRemapAddr = QSPI_REMAP_ADDR;



/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void Hal_Qspi_PatchInit(void)
{
    Hal_Qspi_Init = Hal_Qspi_Init_patch;
}

void Hal_QSpi_UpdateRemap(uint32_t u32Addr)
{
    uint8_t u8XipEn;
    
    if (u32Addr == XIP->RAR)
        return;
    
    u8XipEn = BIT_GET(XIP->QCR, XIP_QCR_QEN_Pos);
    
    /* Disable QSPI before set Remap address */
    Hal_Qspi_Enable(0);
    XIP->RAR = u32Addr;
    if (u8XipEn)
        Hal_Qspi_Enable(1);
    
    u32Hal_QspiRemapAddr = XIP->RAR;
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Init
*
* DESCRIPTION:
*   1. to initialize the QSPI hardware
*
* CALLS
*
* PARAMETERS
*   1. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t.
*   2. u32Sclk : desired system clock of QSPI (Hz)
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Qspi_Init_patch(E_SpiSlave_t eSlvIdx, uint32_t u32Sclk)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return;
    Hal_Qspi_Init_impl(eSlvIdx, u32Sclk);
    Hal_QSpi_UpdateRemap(u32Hal_QspiRemapAddr); /* Update for warmboot */
}
