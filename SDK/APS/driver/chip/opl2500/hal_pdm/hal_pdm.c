/******************************************************************************
 *  Copyright 2017 - 2018, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 ******************************************************************************
 * @file hal_pdm.c
 *
 * @brief API to control PDM
 *
 */

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>

#include "opulinks.h"
#include "hal_sys_rcc.h"
#include "hal_pdm.h"
#include "hal_vic.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define SYS_PDM_FIR_CFG_INITIAL_SRAM_Pos   (1U)
#define SYS_PDM_FIR_CFG_INITIAL_SRAM_Msk   (0x1 << SYS_PDM_FIR_CFG_INITIAL_SRAM_Pos)
#define SYS_PDM_FIR_CFG_INITIAL_SRAM       SYS_PDM_FIR_CFG_INITIAL_SRAM_Msk
#define SYS_PDM_FIR_CFG_BYP_Pos            (2U)
#define SYS_PDM_FIR_CFG_BYP_Msk            (0x3 << SYS_PDM_FIR_CFG_BYP_Pos)

/*
 *************************************************************************
 *						    Enums and Structures
 *************************************************************************
 */

/*
 *************************************************************************
 *						    Typedefs
 *************************************************************************
 */

/*
 *************************************************************************
 *                           Declarations of Private Functions
 *************************************************************************
 */



/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
E_PDM_DATA_MODE  g_eHalPdmDataMode;
T_PdmCallBack    g_tHalPdmCallBack;

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
E_DMA_CHANNEL    g_eHalPdmDmaChannel;
uint16_t         g_u16HalPdmDmaBlockSize;

/*
 *************************************************************************
 *                          Patch Definition
 *************************************************************************
 */
typedef void (*T_Hal_Pdm_ConfigNoBufferInt_fp)(const S_PDM_Conf_t *conf);
typedef void (*T_Hal_Pdm_ConfigDma_fp)(const S_PDM_Conf_t *conf, uint32_t sar, E_DMA_TR_WIDTH dmaTrWidth);

T_Hal_Pdm_ConfigNoBufferInt_fp Hal_Pdm_ConfigNoBufferInt;
T_Hal_Pdm_ConfigDma_fp         Hal_Pdm_ConfigDma;

T_Hal_Pdm_Init_fp              Hal_Pdm_Init;
T_Hal_Pdm_Config_fp            Hal_Pdm_Config;
T_Hal_Pdm_Enable_fp            Hal_Pdm_Enable;
T_Hal_Pdm_GetData_fp           Hal_Pdm_GetData;

/*
 *************************************************************************
 *                           Private Functions
 *************************************************************************
 */
static inline void Hal_Pdm_WriteCoeff(uint8_t addr, uint16_t data)
{
    SYS->PDM_SRAM_FORCE = (data << SYS_PDM_SRAM_FORCE_PDM_FORCE_SRAM_DAT_Pos) | (addr << SYS_PDM_SRAM_FORCE_PDM_FORCE_SRAM_ADR_Pos) | SYS_PDM_SRAM_FORCE_PDM_FORCE_SRAM_ENA;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pdm_ConfigNoBufferInt
*
* DESCRIPTION:
*   1. Configure interrupt controller for NO_BUFFER mode
*
* CALLS
*
* PARAMETERS
*   1. conf : configuration of PDM
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Pdm_ConfigNoBufferInt_impl(const S_PDM_Conf_t *conf)
{
    // ISR
    g_tHalPdmCallBack = conf->tCallback;

    // VIC
    Hal_Vic_IntTypeSel(PDM_IRQn, INT_TYPE_LEVEL);
    Hal_Vic_IntMask(PDM_IRQn, 0);
    Hal_Vic_IntClear(PDM_IRQn);
    Hal_Vic_IntEn(PDM_IRQn, 1);

    // NVIC
    NVIC_ClearPendingIRQ(PDM_IRQn);
    NVIC_SetPriority(PDM_IRQn, 0);
    NVIC_EnableIRQ(PDM_IRQn);
}

/*************************************************************************
* FUNCTION:
*  Hal_Pdm_ConfigDma
*
* DESCRIPTION:
*   1. Configure DMA access mode for BUFFER mode
*
* CALLS
*
* PARAMETERS
*   1. conf : configuration of PDM
*   2. sar: DMA srouce address
*   3. dmaTrWidth: DMA transfer width
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Pdm_ConfigDma_impl(const S_PDM_Conf_t *conf, uint32_t sar, E_DMA_TR_WIDTH dmaTrWidth)
{
    // LLP
    int i;

    S_DMA_LinkListItem_t *tLli = (S_DMA_LinkListItem_t *) conf->dmaInnerBuffer;
    S_DMA_CTL_L_t *ptCtl_L = (S_DMA_CTL_L_t *) &(tLli[0].CTL_L);
    S_DMA_CTL_H_t *ptCtl_H = (S_DMA_CTL_H_t *) &(tLli[0].CTL_H);

    /* LLI preparation */
    memset(&tLli[0], 0, sizeof(S_DMA_LinkListItem_t) * conf->dmaBlockNum);
    tLli[0].SAR = sar;
    tLli[0].DAR = (uint32_t) conf->dmaDataBuffer;
    tLli[0].LLP = ((uint32_t)(&tLli[1])) & 0xFFFFFFFC;
    // Ctl_L
    ptCtl_L->CTL_L_INT_EN       = 1;
    ptCtl_L->CTL_L_SRC_TR_WIDTH = dmaTrWidth;
    ptCtl_L->CTL_L_DST_TR_WIDTH = dmaTrWidth;
    ptCtl_L->CTL_L_SINC         = DMA_INC_No_Change;
    ptCtl_L->CTL_L_DINC         = DMA_INC_Increment;
    ptCtl_L->CTL_L_TT_FC        = DMA_TT_p2m_FC_Dma;
    ptCtl_L->CTL_L_SRC_MSIZE    = DMA_MSIZE_1;
    ptCtl_L->CTL_L_DEST_MSIZE   = DMA_MSIZE_1;
    ptCtl_L->CTL_L_LLP_SRC_EN   = 1;
    ptCtl_L->CTL_L_LLP_DST_EN   = 1;
    // Ctl_H
    ptCtl_H->CTL_H_BLOCK_TS     = conf->dmaBlockTs;
    ptCtl_H->CTL_H_DONE         = 1;

    for (i = 1; i < conf->dmaBlockNum; i++)
    {
        memcpy(&tLli[i], &tLli[i-1], sizeof(S_DMA_LinkListItem_t));
        tLli[i].DAR = (uint32_t) (conf->dmaDataBuffer + (conf->dmaBlockTs << (dmaTrWidth - 1)) * i);
        tLli[i].LLP = ((uint32_t)(&tLli[i+1])) & 0xFFFFFFFC;
    }

    S_DMA_LinkListItem_t *lastLLP = &tLli[conf->dmaBlockNum - 1];
    if (conf->eDataMode == PDM_DATA_MODE_CONTINUOUS_BUFFER)
    {
        ((S_DMA_CTL_L_t *) (&lastLLP->CTL_L))->CTL_L_LLP_SRC_EN = 0;
        ((S_DMA_CTL_L_t *) (&lastLLP->CTL_L))->CTL_L_LLP_DST_EN = 0;
        lastLLP->LLP = 0;
    }
    else if (conf->eDataMode == PDM_DATA_MODE_CIRCULAR_BUFFER)
    {
        lastLLP->LLP = ((uint32_t) (&tLli[0])) & 0xFFFFFFFC;
    }

    // Init DMA
    S_DMA_Init_t tRxDma;
    memset(&tRxDma, 0, sizeof(tRxDma));
    tRxDma.SrcPer   = APS_DMA_ITF_PDM_RX;
    tRxDma.LlpSrcEn = 1;
    tRxDma.LlpDstEn = 1;
    tRxDma.Loc      = ((uint32_t) &tLli[0] >> 2);
    tRxDma.Dstatar  = (uint32_t) &DMA->DAR0 + 0x58 * g_eHalPdmDmaChannel;
    tRxDma.DsUpdEn  = 1;

    // Setup DMA interrupt
    Hal_Dma_CallBackSet(conf->eDmaChannel, conf->tCallback);
    Hal_Dma_IntMask(conf->eDmaChannel, DMA_BMP_BLK);
    Hal_Dma_IntEn(ENABLE, DMA_BMP_INT_CH0);
    Hal_Dma_Config(conf->eDmaChannel, &tRxDma);

    // Enable DMA
    Hal_Dma_Enable(conf->eDmaChannel);
}

/*
 *************************************************************************
 *                           Public Functions
 *************************************************************************
 */

/*************************************************************************
* FUNCTION:
*  Hal_Pdm_Init
*
* DESCRIPTION:
*   1. Init PDM
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Pdm_Init_impl()
{
    // Select PDM clock source
    Hal_Sys_PdmSrcSelect(APS_CLK_PDM_SRC_DECI_160M_BB_DIV_4, APS_CLK_PDM_IO_SRC_DECI_160M_BB, APS_CLK_PDM_IO_DIV_100);

    // FIXME: HW change reset value
    SYS->PDM_CIC &= ~(SYS_PDM_CIC_PDM_CIC_GAIN0_Msk | SYS_PDM_CIC_PDM_CIC_GAIN1_Msk);

    // Config FIR coefficient
    SYS->PDM_FIR_CFG |= SYS_PDM_FIR_CFG_INITIAL_SRAM;
    SYS->PDM_FIR_CFG &= ~SYS_PDM_FIR_CFG_INITIAL_SRAM_Msk;

    Hal_Pdm_WriteCoeff(0x80, 0x0000);
    Hal_Pdm_WriteCoeff(0x81, 0x0000);
    Hal_Pdm_WriteCoeff(0x82, 0xFFFF);
    Hal_Pdm_WriteCoeff(0x83, 0xFFFF);
    Hal_Pdm_WriteCoeff(0x84, 0xFFFF);
    Hal_Pdm_WriteCoeff(0x85, 0x0000);
    Hal_Pdm_WriteCoeff(0x86, 0x0004);
    Hal_Pdm_WriteCoeff(0x87, 0x000B);
    Hal_Pdm_WriteCoeff(0x88, 0x0011);
    Hal_Pdm_WriteCoeff(0x89, 0x000F);
    Hal_Pdm_WriteCoeff(0x8A, 0xFFFF);
    Hal_Pdm_WriteCoeff(0x8B, 0xFFDB);
    Hal_Pdm_WriteCoeff(0x8C, 0xFFAC);
    Hal_Pdm_WriteCoeff(0x8D, 0xFF88);
    Hal_Pdm_WriteCoeff(0x8E, 0xFF93);
    Hal_Pdm_WriteCoeff(0x8F, 0xFFF0);
    Hal_Pdm_WriteCoeff(0x90, 0x00A5);
    Hal_Pdm_WriteCoeff(0x91, 0x0185);
    Hal_Pdm_WriteCoeff(0x92, 0x022B);
    Hal_Pdm_WriteCoeff(0x93, 0x0210);
    Hal_Pdm_WriteCoeff(0x94, 0x00C1);
    Hal_Pdm_WriteCoeff(0x95, 0xFE2A);
    Hal_Pdm_WriteCoeff(0x96, 0xFAD0);
    Hal_Pdm_WriteCoeff(0x97, 0xF7DD);
    Hal_Pdm_WriteCoeff(0x98, 0xF6DF);
    Hal_Pdm_WriteCoeff(0x99, 0xF94C);
    Hal_Pdm_WriteCoeff(0x9A, 0xFFE8);
    Hal_Pdm_WriteCoeff(0x9B, 0x0A4F);
    Hal_Pdm_WriteCoeff(0x9C, 0x16D3);
    Hal_Pdm_WriteCoeff(0x9D, 0x22D7);
    Hal_Pdm_WriteCoeff(0x9E, 0x2B84);
    Hal_Pdm_WriteCoeff(0x9F, 0x2EAE);
    Hal_Pdm_WriteCoeff(0xA0, 0x2B84);
    Hal_Pdm_WriteCoeff(0xA1, 0x22D7);
    Hal_Pdm_WriteCoeff(0xA2, 0x16D3);
    Hal_Pdm_WriteCoeff(0xA3, 0x0A4F);
    Hal_Pdm_WriteCoeff(0xA4, 0xFFE8);
    Hal_Pdm_WriteCoeff(0xA5, 0xF94C);
    Hal_Pdm_WriteCoeff(0xA6, 0xF6DF);
    Hal_Pdm_WriteCoeff(0xA7, 0xF7DD);
    Hal_Pdm_WriteCoeff(0xA8, 0xFAD0);
    Hal_Pdm_WriteCoeff(0xA9, 0xFE2A);
    Hal_Pdm_WriteCoeff(0xAA, 0x00C1);
    Hal_Pdm_WriteCoeff(0xAB, 0x0210);
    Hal_Pdm_WriteCoeff(0xAC, 0x022B);
    Hal_Pdm_WriteCoeff(0xAD, 0x0185);
    Hal_Pdm_WriteCoeff(0xAE, 0x00A5);
    Hal_Pdm_WriteCoeff(0xAF, 0xFFF0);
    Hal_Pdm_WriteCoeff(0xB0, 0xFF93);
    Hal_Pdm_WriteCoeff(0xB1, 0xFF88);
    Hal_Pdm_WriteCoeff(0xB2, 0xFFAC);
    Hal_Pdm_WriteCoeff(0xB3, 0xFFDB);
    Hal_Pdm_WriteCoeff(0xB4, 0xFFFF);
    Hal_Pdm_WriteCoeff(0xB5, 0x000F);
    Hal_Pdm_WriteCoeff(0xB6, 0x0011);
    Hal_Pdm_WriteCoeff(0xB7, 0x000B);
    Hal_Pdm_WriteCoeff(0xB8, 0x0004);
    Hal_Pdm_WriteCoeff(0xB9, 0x0000);
    Hal_Pdm_WriteCoeff(0xBA, 0xFFFF);
    Hal_Pdm_WriteCoeff(0xBB, 0xFFFF);
    Hal_Pdm_WriteCoeff(0xBC, 0xFFFF);
    Hal_Pdm_WriteCoeff(0xBD, 0x0000);
    Hal_Pdm_WriteCoeff(0xBE, 0x0000);
    Hal_Pdm_WriteCoeff(0xBF, 0x0000);

    SYS->PDM_SRAM_FORCE &= ~SYS_PDM_SRAM_FORCE_PDM_FORCE_SRAM_ENA;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pdm_Config
*
* DESCRIPTION:
*   1. Configure PDM channel, sample rate, data access mode
*
* CALLS
*
* PARAMETERS
*   1. conf : configuration of PDM
*
* RETURNS
*   0 : success
*   1 : error
*
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Pdm_Config_impl(const S_PDM_Conf_t *conf)
{
    uint32_t reg = SYS->PDM_FIR_CFG;

    // TODO: Error handling
//    if (Hal_Dma_Check(conf->eDmaChannel))
//    {
//        tracer_cli(LOG_HIGH_LEVEL, "DMA channel not available\n");
//        return 1;
//    }

    // Configure channel bypass
    reg = (reg & ~SYS_PDM_FIR_CFG_BYP_Msk) | (conf->eChannel << SYS_PDM_FIR_CFG_BYP_Pos);

    // Configure sample rate division
    reg = (reg & ~SYS_PDM_FIR_CFG_PDM_FIR_VALID_DIV_Msk) | (conf->eSampleRate << SYS_PDM_FIR_CFG_PDM_FIR_VALID_DIV_Pos);

    SYS->PDM_FIR_CFG = reg;

    // Configure data access mode
    g_eHalPdmDataMode = conf->eDataMode;

    switch (g_eHalPdmDataMode)
    {
        case PDM_DATA_MODE_NO_BUFFER:
        {
            Hal_Pdm_ConfigNoBufferInt(conf);
            break;
        }
        case PDM_DATA_MODE_CONTINUOUS_BUFFER:
        case PDM_DATA_MODE_CIRCULAR_BUFFER:
        {
            uint32_t sar;
            E_DMA_TR_WIDTH dmaTrWidth;

            SYS->PDM_SRAM_WR_DLY |= SYS_PDM_SRAM_WR_DLY_PDM_DMA_MOD;

            g_eHalPdmDmaChannel = conf->eDmaChannel;

            switch (conf->eChannel)
            {
                case PDM_CHANNEL_STEREO:
                    sar = (uint32_t) &SYS->PDM_STS0;
                    dmaTrWidth = DMA_TR_WIDTH_32_bits;
                    g_u16HalPdmDmaBlockSize = 4 * conf->dmaBlockTs;
                    break;
                case PDM_CHANNEL_MONO_R:
                    sar = (uint32_t) &SYS->PDM_STS0 + 2;
                    dmaTrWidth = DMA_TR_WIDTH_16_bits;
                    g_u16HalPdmDmaBlockSize = 2 * conf->dmaBlockTs;
                    break;
                case PDM_CHANNEL_MONO_L:
                    sar = (uint32_t) &SYS->PDM_STS0;
                    dmaTrWidth = DMA_TR_WIDTH_16_bits;
                    g_u16HalPdmDmaBlockSize = 2 * conf->dmaBlockTs;
                    break;
            }

            Hal_Pdm_ConfigDma(conf, sar, dmaTrWidth);

            break;
        }
    }

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pdm_Enable
*
* DESCRIPTION:
*   1. Enable or disable PDM recording
*
* CALLS
*
* PARAMETERS
*   1. enable : [1]enable; [0]disable
*
* RETURNS
*   0 : success
*   1 : error
*
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Pdm_Enable_impl(uint8_t enable)
{
    if (enable)
    {
        // Enable PDM_CLK, PDM_IO_CLK
        Hal_Sys_ApsModuleClkEn(1, APS_CLK_PDM);
        Hal_Sys_ApsModuleClkEn(1, APS_CLK_PDM_IO);
    }

    // Configure PDM enable/disable
    SYS->PDM_CFG0 = (SYS->PDM_CFG0 & ~SYS_PDM_CFG0_PDM_ENABLE_Msk) | (enable << SYS_PDM_CFG0_PDM_ENABLE_Pos);

    if (!enable)
    {
        // Disable PDM_IO_CLK, PDM_CLK
        Hal_Sys_ApsModuleClkEn(0, APS_CLK_PDM_IO);
        Hal_Sys_ApsModuleClkEn(0, APS_CLK_PDM);

        switch (g_eHalPdmDataMode)
        {
            case PDM_DATA_MODE_NO_BUFFER:
            {
                NVIC_DisableIRQ(PDM_IRQn);
                break;
            }
            case PDM_DATA_MODE_CONTINUOUS_BUFFER:
            case PDM_DATA_MODE_CIRCULAR_BUFFER:
            {
                Hal_Dma_Abort(g_eHalPdmDmaChannel);
                break;
            }
        }
    }

    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pdm_GetData
*
* DESCRIPTION:
*   1. Get sample data from PDM module
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   [15:0]  Left channel data
*   [31:16] Right channel data
*
* GLOBALS AFFECTED
*
*************************************************************************/
uint32_t Hal_Pdm_GetData_impl()
{
    return SYS->PDM_STS0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Pdm_Pre_Init
*
* DESCRIPTION:
*   1. Inital of functuin pointers
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void Hal_Pdm_Pre_Init()
{
    Hal_Pdm_Init              = Hal_Pdm_Init_impl;
    Hal_Pdm_Config            = Hal_Pdm_Config_impl;
    Hal_Pdm_Enable            = Hal_Pdm_Enable_impl;
    Hal_Pdm_GetData           = Hal_Pdm_GetData_impl;

    Hal_Pdm_ConfigNoBufferInt = Hal_Pdm_ConfigNoBufferInt_impl;
    Hal_Pdm_ConfigDma         = Hal_Pdm_ConfigDma_impl;
}
