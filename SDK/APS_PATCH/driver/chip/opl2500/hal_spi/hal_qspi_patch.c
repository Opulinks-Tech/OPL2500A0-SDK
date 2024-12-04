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
#include "boot_sequence.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define QSPI_WAIT_IDLE_TIMEOUT_MS  2

#define QSPI_FLASH_DEVICE_SIZE_8MBIT        0
#define QSPI_FLASH_DEVICE_SIZE_64MBIT       1

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
void Hal_Qspi_Command_Exec_patch(S_QSPI_Command_t *ptCmd);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
uint32_t g_u32Hal_QspiRemapAddr = 0xFF000000;
uint32_t g_u32XipDsr = 0x101002;
float g_fQspiStigWaitTimeoutMs = 0.05;  //50us
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
void Hal_Qspi_PatchInit(void)
{
    Hal_Qspi_Init = Hal_Qspi_Init_patch;
    Hal_Qspi_Command_Exec = Hal_Qspi_Command_Exec_patch;
}

void Hal_QSpi_UpdateRemap(uint32_t u32Addr)
{
    g_u32Hal_QspiRemapAddr = u32Addr;
    
    if (u32Addr == XIP->RAR)
        return;
    
    /* Disable QSPI before set Remap address */
    __disable_irq();
    Hal_Qspi_Enable(0);
    XIP->RAR = u32Addr;
    Hal_Qspi_Enable(1);
    __enable_irq();
}




/*************************************************************************
* FUNCTION:
*  Hal_Qspi_UpdateDeviceSize
*
* DESCRIPTION:
*  1. Update device mapping size by chip select
*
* CALLS
*
* PARAMETERS
*   1. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t.
*   2. eSize: The target device size
*
* RETURNS
*   0: setting complete
*   1: error
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Qspi_UpdateDeviceSize(E_SpiSlave_t eSlvIdx, E_QSPI_DEVICE_SIZE eSize)
{
    uint32_t ret = 1;

    __disable_irq();
    Hal_Qspi_Enable(0);
    switch (eSlvIdx)
    {
        case SPI_SLAVE_1:
            XIP->DSR = (XIP->DSR & ~XIP_DSR_SD1_Msk) | (eSize << XIP_DSR_SD1_Pos);
            ret = 0;
            break;
        case SPI_SLAVE_2:
            XIP->DSR = (XIP->DSR & ~XIP_DSR_SD2_Msk) | (eSize << XIP_DSR_SD2_Pos);
            ret = 0;
            break;
        case SPI_SLAVE_3:
            XIP->DSR = (XIP->DSR & ~XIP_DSR_SD3_Msk) | (eSize << XIP_DSR_SD3_Pos);
            ret = 0;
            break;
        default:
            break;
    }
    Hal_Qspi_Enable(1);
    __enable_irq();
    
    g_u32XipDsr = XIP->DSR;
    return ret;
}


uint32_t Hal_Qspi_WaitPollingState(void)
{
    uint32_t u32Ret = 0;
    
    while (1)
    {   /* Wait until polling state finished */
        if (XIP->PFSR & XIP_PFSR_VLD)
        {   /* Tag one if ever in polling state */
            u32Ret = 1;
        }
        else
        {
            break;
        }
    }
    return u32Ret;
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
    __disable_irq();
    Hal_Qspi_Init_impl(eSlvIdx, u32Sclk);
    XIP->QCR |= XIP_QCR_ADEN;
    Hal_QSpi_UpdateRemap(g_u32Hal_QspiRemapAddr);
    
    if (XIP->DSR != g_u32XipDsr)
    {
        Hal_Qspi_Enable(0);
        XIP->DSR = g_u32XipDsr;
        Hal_Qspi_Enable(1);
    }
    
    /* Rollback this HW feature.
     * It might casue some side effect */
    //XIP->AHB_DEC_ACCESS = 1;    /* Fix wXwY and wXrY issue. */
    
    __enable_irq();
}


/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Command_Exec
*
* DESCRIPTION:
*   1. to execute Flash Comamnd
*
* CALLS
*
* PARAMETERS
*   1. ptCmd : pointer of Flash command configurations
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Qspi_Command_Exec_patch(S_QSPI_Command_t *ptCmd)
{   
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    uint32_t u32Temp = 0;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_fQspiStigWaitTimeoutMs;

    if (ptCmd->ModeBitEn)
    {
        XIP->MBR = ptCmd->ModeBit;
    }
    
    if (ptCmd->CmdAddrEn)
    {
        XIP->FCAR = ptCmd->CmdAddr;
    }
    
    if (ptCmd->WrDataEn)
    {
        XIP->FCWDRL = ptCmd->WrDataLow;
        XIP->FCWDRU = ptCmd->WrDataUpper;
    }

    // execute command
    u32Temp = XIP->FCR;
    u32Temp &= ~( XIP_FCR_NDB_Msk |
                  XIP_FCR_NWDB_Msk |
                  XIP_FCR_WE_Msk |
                  XIP_FCR_NAB_Msk |
                  XIP_FCR_MBEN_Msk |
                  XIP_FCR_CAEN_Msk |
                  XIP_FCR_NRDB_Msk |
                  XIP_FCR_RDEN_Msk |
                  XIP_FCR_CMDOP_Msk |
                  XIP_FCR_EXCMD_Msk );
    u32Temp |= ( (ptCmd->DummyByteNum << XIP_FCR_NDB_Pos) |
                 (ptCmd->eWrDataByteNum << XIP_FCR_NWDB_Pos) |
                 (ptCmd->WrDataEn << XIP_FCR_WE_Pos) |
                 (ptCmd->eAddrByteNum << XIP_FCR_NAB_Pos) |
                 (ptCmd->ModeBitEn << XIP_FCR_MBEN_Pos) |
                 (ptCmd->CmdAddrEn << XIP_FCR_CAEN_Pos) |
                 (ptCmd->eRdDataByteNum << XIP_FCR_NRDB_Pos) |
                 (ptCmd->RdDataEn << XIP_FCR_RDEN_Pos) |
                 (ptCmd->Opcode << XIP_FCR_CMDOP_Pos) |
                 (1 << XIP_FCR_EXCMD_Pos) );
                 
    __disable_irq();
    XIP->FCR = u32Temp;
    
    u32TickStart = Hal_Tick_Diff(0);
    // poll for command execution progress
    while ( (XIP->FCR & XIP_FCR_EIP_Msk) ||
            (!(XIP->QCR & XIP_QCR_IDLE_Msk)) ) // Note: need to poll for QSPI IDLE status, otherwise may encounter erase fail
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks)
            break;
    }
    __enable_irq();

    // Output read data
    if (ptCmd->RdDataEn)
    {
        ptCmd->RdDataLow = XIP->FCRDRL;
        ptCmd->RdDataUpper = XIP->FCRDRU;
    }
}

