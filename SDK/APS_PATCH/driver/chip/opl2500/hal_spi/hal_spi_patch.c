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
 *  @file hal_spi_patch.c
 * 
 *  @brief SPI patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_spi.h"

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

/*************************************************************************
* FUNCTION:
*  Hal_Spi_ReadStatus
*
* DESCRIPTION:
*   1. To Get SPI status
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
E_HAL_SPI_STATUS Hal_Spi_ReadStatus(E_SpiIdx_t eSpiIdx)
{
    uint32_t StatusReg ;
    if (eSpiIdx == SPI_IDX_1)
        StatusReg = SPI1->SR;
    else if (eSpiIdx == SPI_IDX_2)
        StatusReg = SPI2->SR;
    else if (eSpiIdx == SPI_IDX_3)
        StatusReg = SPI3->SR;
    else
        StatusReg = SPI_STATUS_INVALID;
    
    return (E_HAL_SPI_STATUS)StatusReg;
    
}



/*************************************************************************
* FUNCTION:
*  Hal_Spi_ResetFIFO
*
* DESCRIPTION:
*   1. Disable SPI to reset FIFO. 
*      If SPI is enable before disable, then enable SPI
*      If SPI is diabled, ignore this reset FIFO command.
*
* CALLS
*
* PARAMETERS
*   1. eSpiIdx  : Index of SPI. refer to E_SpiIdx_t
*
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Spi_ResetFifo(E_SpiIdx_t eSpiIdx)
{
    uint32_t ret = 0;
    _Hal_Spi_Poll_Busy(eSpiIdx);
    switch (eSpiIdx)
    {
        case SPI_IDX_1:
            if (SPI1->SSIENR)
            {
                SPI1->SSIENR = 0;
                __ISB();
                __DSB();
                SPI1->SSIENR = 1;
            }
            break;
        case SPI_IDX_2:
            if (SPI2->SSIENR)
            {
                SPI2->SSIENR = 0;
                __ISB();
                __DSB();
                SPI2->SSIENR = 1;
            }
            break;
        case SPI_IDX_3:
            if (SPI3->SSIENR)
            {
                SPI3->SSIENR = 0;
                __ISB();
                __DSB();
                SPI3->SSIENR = 1;
            }
            break;
        default:
            ret = 1;
            break;
    }

    return ret;
}

void Hal_Spi_IntHandler(E_SpiIdx_t eSpiIdx)
{
    if (eSpiIdx == SPI_IDX_1)
    {
        if (g_tHalSpi1CallBack)
            g_tHalSpi1CallBack();
    }
    else if (eSpiIdx == SPI_IDX_2)
    {
        if (g_tHalSpi2CallBack)
            g_tHalSpi2CallBack();
    }
    else if (eSpiIdx == SPI_IDX_3)
    {
        if (g_tHalSpi3CallBack)
            g_tHalSpi3CallBack();
    }
    else
        return;
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

