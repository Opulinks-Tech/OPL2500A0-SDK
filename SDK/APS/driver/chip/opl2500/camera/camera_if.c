/* *****************************************************************************
 *  Copyright 2018 - 2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file camera_if.h 
 * 
 *  @brief Camera interface hardware driver
 *         Don't call these API directly.
 *
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "camera_if.h"
#include "hal_spi.h"
#include "hal_sys_rcc.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

#define CAM_IF_WAIT_FIFO_TIMEOUT        0x1000
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



T_CamIf_Init    CamIf_Init      = CamIf_Init_impl;
T_CamIf_DeInit  CamIf_DeInit    = CamIf_DeInit_impl;
T_CamIf_Start   CamIf_Start     = CamIf_Start_impl;
T_CamIf_Stop    CamIf_Stop      = CamIf_Stop_impl;

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

/**
 * @brief Init CamIf hardware
 *        It will init SPI3 and CAM_IF related regsiters.
 * @param u32DataIoNum [in] How many data io used in current interface.
 *                          Valid range is 1 to 16.
 *
 */
uint32_t CamIf_Init_impl(uint32_t u32DataIoNum, uint32_t u32DmaRxLvl)
{   
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_SPI_3);
    Hal_Sys_ApsModuleRst(APS_RST_SPI_3);
    
    /* Camera interface is from SPI 3 */
    CAM_SPI->SPI_OPLR0 |= SPI_MST_SPI_OPLR0_BYTE_REVERSE_EN;
    CAM_SPI->CTRLR0 = (CAM_SPI->CTRLR0 & ~SPI_MST_CTRLR0_DFS_32_Msk) | (SPI_DFS_32_bit << SPI_MST_CTRLR0_DFS_32_Pos);
    CAM_SPI->SSIENR = SPI_MST_SSIENR_SSI_EN;
    
    CAM_IF->CAM_MODE = CAM_REG_CAM_MODE_MASTER;
    CAM_IF->CAM_WIDTH = CAM_IF_DATAIO_NUM_TO_WIDTH(u32DataIoNum);
    
    Hal_Spi_Dma_Config(SPI_IDX_3, 0 ,1, 0, u32DmaRxLvl);
    
    return 0;
}

void CamIf_DeInit_impl(void)
{
    CAM_IF->CAM_ENABLE = 0;
    Hal_Sys_ApsModuleRst(APS_RST_SPI_3);
    Hal_Sys_ApsModuleClkEn(DISABLE, APS_CLK_SPI_3);
}

uint32_t CamIf_Start_impl(void)
{
    uint32_t u32Ret = 1;
    
    CAM_SPI->SSIENR = SPI_MST_SSIENR_SSI_EN;
    CAM_IF->CAM_ENABLE = CAM_CAM_ENABLE_CAM_ENABLE;

    return u32Ret;
}

void CamIf_Stop_impl(void)
{
    CAM_IF->CAM_ENABLE = 0;
    CAM_SPI->SSIENR = 0;
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

