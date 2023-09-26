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
#include "hal_cache.h"
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
void Hal_Qspi_Peri_Select_patch(E_SpiSlave_t eSlvIdx);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
uint32_t g_u32aHal_QspiRemapAddr[SPI_SLAVE_MAX] = {
    APS_XIP_MEM_BASE,
    APS_XIP_MEM_BASE,
    APS_XIP_MEM_BASE,
    APS_XIP_MEM_BASE
};

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 

E_SpiSlave_t eHal_QspiCurrCsIdx = SPI_SLAVE_MAX;/* Avoid ZI function */
E_SpiSlave_t eHal_QspiXipCsIdx = SPI_SLAVE_MAX; /* Avoid ZI function */



/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
void Hal_Qspi_PatchInit(void)
{
    Hal_Qspi_Init = Hal_Qspi_Init_patch;
    Hal_Qspi_Peri_Select = Hal_Qspi_Peri_Select_patch;
    eHal_QspiCurrCsIdx = SPI_SLAVE_0;
    eHal_QspiXipCsIdx = SPI_SLAVE_0;
}

void Hal_QSpi_UpdateRemap(E_SpiSlave_t eSlvIdx, uint32_t u32Addr)
{
    uint8_t u8XipEn;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return;
    
    g_u32aHal_QspiRemapAddr[eSlvIdx] = u32Addr;
    
    if (eHal_QspiCurrCsIdx != eSlvIdx)
        return;
    
    if (u32Addr == XIP->RAR)
        return;
    
    u8XipEn = BIT_GET(XIP->QCR, XIP_QCR_QEN_Pos);
    
    /* Disable QSPI before set Remap address */
    Hal_Qspi_Enable(0);
    XIP->RAR = u32Addr;
    if (u8XipEn)
        Hal_Qspi_Enable(1);
}

void Hal_QSpi_SetXipCs(E_SpiSlave_t eSlvIdx)
{
    eHal_QspiXipCsIdx = eSlvIdx;
}

void Hal_Qspi_RestoreXipCs(void)
{
    if (eHal_QspiCurrCsIdx != eHal_QspiXipCsIdx)
    {
        Hal_Qspi_Peri_Select(eHal_QspiXipCsIdx);
    }
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
    eHal_QspiCurrCsIdx = eSlvIdx;
    Hal_QSpi_UpdateRemap(eSlvIdx, g_u32aHal_QspiRemapAddr[eSlvIdx]); /* Update for warmboot */
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Peri_Select
*
* DESCRIPTION:
*   1. to setup peripheral chip select line
*
* CALLS
*
* PARAMETERS
*   1. eSlvIdx : Index of SPI slave.
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Qspi_Peri_Select_patch(E_SpiSlave_t eSlvIdx)
{
    // current peripheral chip select
    uint32_t u32Temp = (XIP->QCR & XIP_QCR_PCSL_Msk) >> XIP_QCR_PCSL_Pos;
    
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return;
    
    if ((u32Temp & (0x1 << eSlvIdx)) == 0) // current chip select is the same as specified
    {
        // do nothing
    }
    else // change peripheral chip select
    {
        Hal_Qpsi_Idle_Poll();
        u32Temp = XIP->QCR;
        u32Temp &= ~(XIP_QCR_PCSL_Msk);
        u32Temp |= ((0xF & ~(0x1 << eSlvIdx)) << XIP_QCR_PCSL_Pos);
        XIP->QCR = u32Temp;
        
        eHal_QspiCurrCsIdx = eSlvIdx;
    }
    
    Hal_Cache_Clear();
}
