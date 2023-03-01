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
 *  @file hal_dma_patch.c
 * 
 *  @brief DMA patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_dma.h"
#include "hal_vic.h"
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



/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

/*************************************************************************
* FUNCTION:
*  Hal_Dma_IntEn
*
* DESCRIPTION:
*   1. Enable/Disable the DMA interrupts
*
* CALLS
*
* PARAMETERS
*   1. u8Enable : 1 = Enable
*                 0 = Disable
*   2. u8Channel
*
* RETURNS
*   0 : setting complete
*   1 : error
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Dma_IntEn(uint8_t u8Enable, E_DMA_CHANNEL eChannel)
{
    S_DMA_Channel_t *CHx = g_pscaHalDmaCHx[eChannel];
    
    /* Check channel is free and channel is valid */
    if (Hal_Dma_Check(eChannel) != 0)
        return 1;
    
    /* Write IER, enable interrupt */
    if(u8Enable == ENABLE)
    {
        Hal_Vic_IntInit(APS_DMA_IRQn, VIC_TRIG_LEVEL_HIGH, IRQ_PRIORITY_DMA);
        /* Module part: Enable module */
        CHx->CTL_L |= DMA_CTL_L_INT_EN;
    }
    else
    {
        /* Module part: Disable module */
        CHx->CTL_L &= ~DMA_CTL_L_INT_EN_Msk;
        Hal_Vic_IntDeInit(APS_DMA_IRQn);
    }
    return 0;
}
