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
 * @file hal_qspi.c
 * 
 * @brief API to control QSPI for APS
 * 
 */

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "opulinks.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_qspi.h"
#include "hal_tick.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define QSPI_BAUDR_MIN      0x2
#define QSPI_BAUDR_MAX      0x20 // 32

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
 *                          Public Variables
 *************************************************************************
 */
extern uint32_t u32SpiSpeed[SPI_IDX_MAX];
extern uint32_t g_u32FlashPollingStatusTimeoutMs;

uint32_t g_u32QspiTimeoutMs = QSPI_TIMEOUT_MS;

T_Qspi_Callback g_tHalQspiCallBack = NULL;

T_Hal_Qspi_Init_fp                      Hal_Qspi_Init                       = Hal_Qspi_Init_impl;
T_Hal_Qspi_Config_fp                    Hal_Qspi_Config                     = Hal_Qspi_Config_impl;
T_Hal_Qspi_Peri_Select_fp               Hal_Qspi_Peri_Select                = Hal_Qspi_Peri_Select_impl;
T_Hal_Qspi_Instruction_Setup_fp         Hal_Qspi_Instruction_Setup          = Hal_Qspi_Instruction_Setup_impl;
T_Hal_Qspi_Delay_Setup_fp               Hal_Qspi_Delay_Setup                = Hal_Qspi_Delay_Setup_impl;
T_Hal_Qspi_RxSampleDly_Set_fp           Hal_Qspi_RxSampleDly_Set            = Hal_Qspi_RxSampleDly_Set_impl;
T_Hal_Qspi_Command_Exec_fp              Hal_Qspi_Command_Exec               = Hal_Qspi_Command_Exec_impl;
T_Hal_Qspi_Dma_Access_fp                Hal_Qspi_Dma_Access                 = Hal_Qspi_Dma_Access_impl;
T_Hal_Qspi_Baudrate_Set_fp              Hal_Qspi_Baudrate_Set               = Hal_Qspi_Baudrate_Set_impl;
T_Hal_Qspi_Baudrate_Get_fp              Hal_Qspi_Baudrate_Get               = Hal_Qspi_Baudrate_Get_impl;
T_Hal_Qpsi_Idle_Poll_fp                 Hal_Qpsi_Idle_Poll                  = Hal_Qpsi_Idle_Poll_impl;
T_Hal_Qspi_Divider_Update_fp            Hal_Qspi_Divider_Update             = Hal_Qspi_Divider_Update_impl;
T_Hal_Qspi_Psram_Instruction_Setup_fp   Hal_Qspi_Psram_Instruction_Setup    = Hal_Qspi_Psram_Instruction_Setup_impl;
T_Hal_Qspi_Psram_RxSampleDly_Set_fp     Hal_Qspi_Psram_RxSampleDly_Set      = Hal_Qspi_Psram_RxSampleDly_Set_impl;
T_Hal_Qspi_Psram_Coexistence_Setup_fp   Hal_Qspi_Psram_Coexistence_Setup    = Hal_Qspi_Psram_Coexistence_Setup_impl;
T_Hal_Qspi_Enable_fp                    Hal_Qspi_Enable                     = Hal_Qspi_Enable_impl;

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */

/*
 *************************************************************************
 *                           Public Functions
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
void Hal_Qspi_Init_impl(E_SpiSlave_t eSlvIdx, uint32_t u32Sclk)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return;
    
    uint32_t u32Spi0ClkFreq = 0;
    uint32_t u32QspiSclk = 0;
    S_QSPI_Delay_t tDelay = 
    {
        .Cssot  = QSPI_CFG_CSSOT_NS,
        .Cseot  = QSPI_CFG_CSEOT_NS,
        .Csdads = QSPI_CFG_CSDADS_NS,
        .Csda   = QSPI_CFG_CSDA_NS,
    };
    
    Hal_Sys_ApsModuleRst(APS_RST_SPI_0);
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_SPI_0);
    
    // get reference clock of QSPI
    Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI0, &u32Spi0ClkFreq);
    // set QSPI baudrate
    Hal_Qspi_Baudrate_Set(u32Sclk);
    u32QspiSclk = Hal_Qspi_Baudrate_Get();
    // set Delay
    Hal_Qspi_Delay_Setup(u32Spi0ClkFreq, u32QspiSclk, &tDelay);

    // Setup QSPI configuration
    S_QSPI_Config_t tCfg;
    memset(&tCfg, 0, sizeof(S_QSPI_Config_t));
    tCfg.DacEn = 1;
    tCfg.eInstrType = QSPI_SIO_Mode;
    tCfg.AddrRemapEn = 1;
    tCfg.Peri_CS = ~(0x1 << eSlvIdx);
    tCfg.RemapAddr = QSPI_REMAP_ADDR;
    Hal_Qspi_Config(&tCfg);
    
    // Interrupt
    Hal_Vic_IntInit(SPI0_IRQn, VIC_TRIG_LEVEL_HIGH, IRQ_PRIORITY_SPI0);
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Config
*
* DESCRIPTION:
*   1. to configure QSPI module
*
* CALLS
*
* PARAMETERS
*   1. ptInit : pointer of configurations
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Qspi_Config_impl(S_QSPI_Config_t *ptInit)
{
    uint32_t u32Temp = 0;
    
    u32Temp = XIP->QCR;
    u32Temp &= ~( XIP_QCR_CPOL_Msk |
                  XIP_QCR_CLPH_Msk |
                  XIP_QCR_DACEN_Msk |
                  XIP_QCR_LIPEN_Msk |
                  XIP_QCR_PSDEC_Msk |
                  XIP_QCR_PCSL_Msk |
                  XIP_QCR_AAREN_Msk |
                  XIP_QCR_ADEN_Msk );
    u32Temp |= ( (ptInit->ClkPolar << XIP_QCR_CPOL_Pos) |
                 (ptInit->ClkPhase << XIP_QCR_CLPH_Pos) |
                 (ptInit->DacEn << XIP_QCR_DACEN_Pos) |
                 (ptInit->LegacyEn << XIP_QCR_LIPEN_Pos) |
                 (ptInit->PeriSelDecode << XIP_QCR_PSDEC_Pos) |
                 (ptInit->Peri_CS << XIP_QCR_PCSL_Pos) |
                 (ptInit->AddrRemapEn << XIP_QCR_AAREN_Pos) |
                 (ptInit->DecoderEn << XIP_QCR_ADEN_Pos) );
    XIP->QCR = u32Temp;
    
    u32Temp = XIP->DRIR;
    u32Temp &= ~(XIP_DRIR_ITYPE_Msk);
    u32Temp |= (ptInit->eInstrType << XIP_DRIR_ITYPE_Pos);
    XIP->DRIR = u32Temp;
    
    u32Temp = XIP->DSR;
    u32Temp &= ~( XIP_DSR_SD0_Msk |
                  XIP_DSR_SD1_Msk |
                  XIP_DSR_SD2_Msk |
                  XIP_DSR_SD3_Msk );
    u32Temp |= ( (ptInit->SizeCS0 << XIP_DSR_SD0_Pos) |
                 (ptInit->SizeCS1 << XIP_DSR_SD1_Pos) |
                 (ptInit->SizeCS2 << XIP_DSR_SD2_Pos) |
                 (ptInit->SizeCS3 << XIP_DSR_SD3_Pos) );
                 
    XIP->RAR = ptInit->RemapAddr;
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
void Hal_Qspi_Peri_Select_impl(E_SpiSlave_t eSlvIdx)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return;
    
    // current peripheral chip select
    uint32_t u32Temp = (XIP->QCR & XIP_QCR_PCSL_Msk) >> XIP_QCR_PCSL_Pos;
    
    if ((u32Temp & (0x1 << eSlvIdx)) == 0) // current chip select is the same as specified
    {
        // do nothing
    }
    else // change peripheral chip select
    {
        u32Temp = XIP->QCR;
        u32Temp &= ~(XIP_QCR_PCSL_Msk);
        u32Temp |= ((0xF & ~(0x1 << eSlvIdx)) << XIP_QCR_PCSL_Pos);
        XIP->QCR = u32Temp;
    }
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Instruction_Setup
*
* DESCRIPTION:
*   1. to setup Instruction of Device for QSPI module 
*
* CALLS
*
* PARAMETERS
*   1. ptInst : pointer of configurations
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Qspi_Instruction_Setup_impl(S_QSPI_Instruction_t *ptInst)
{
    uint32_t u32Temp = 0;
    
    u32Temp = XIP->DRIR;
    u32Temp &= ~( XIP_DRIR_ROP_Msk |
                  XIP_DRIR_DDRBEN_Msk |
                  XIP_DRIR_ATT_Msk |
                  XIP_DRIR_DTT_Msk |
                  XIP_DRIR_MBEN_Msk |
                  XIP_DRIR_DCC_Msk );
    u32Temp |= ( (ptInst->RdOpCode << XIP_DRIR_ROP_Pos) |
                 (ptInst->DDR_BitEn << XIP_DRIR_DDRBEN_Pos) |
                 (ptInst->eRdAddrXferType << XIP_DRIR_ATT_Pos) |
                 (ptInst->eRdDataXferType << XIP_DRIR_DTT_Pos) |
                 (ptInst->ModeBitEn << XIP_DRIR_MBEN_Pos) |
                 (ptInst->RdDummy << XIP_DRIR_DCC_Pos) );
    XIP->DRIR = u32Temp;
    
    u32Temp = XIP->DWIR;
    u32Temp &= ~( XIP_DWIR_WOP_Msk |
                  XIP_DWIR_WDIS_Msk |
                  XIP_DWIR_ATT_Msk |
                  XIP_DWIR_DTT_Msk |
                  XIP_DWIR_DCC_Msk );
    u32Temp |= ( (ptInst->WrOpCode << XIP_DWIR_WOP_Pos) |
                 (ptInst->WEL_Dis << XIP_DWIR_WDIS_Pos) |
                 (ptInst->eWrAddrXferType << XIP_DWIR_ATT_Pos) |
                 (ptInst->eWrDataXferType << XIP_DWIR_DTT_Pos) |
                 (ptInst->WrDummy << XIP_DWIR_DCC_Pos) );
    XIP->DWIR = u32Temp;
    
    u32Temp = XIP->MBR;
    u32Temp &= ~(XIP_MBR_MBITS_Msk);
    u32Temp |= (ptInst->ModeBit << XIP_MBR_MBITS_Pos);
    XIP->MBR = u32Temp;
    
    u32Temp = XIP->WCCR;
    u32Temp &= ~( XIP_WCCR_OPCODE_Msk | 
                  XIP_WCCR_PLBI_Msk |
                  XIP_WCCR_PLPOL_Msk |
                  XIP_WCCR_DISPL_Msk |
                  XIP_WCCR_PLCNT_Msk |
                  XIP_WCCR_PLREPDLY_Msk );
    u32Temp |= ( (ptInst->PollOpCode << XIP_WCCR_OPCODE_Pos) |
                 (ptInst->PollBitIdx << XIP_WCCR_PLBI_Pos) |
                 (ptInst->PollPolarity << XIP_WCCR_PLPOL_Pos) |
                 (ptInst->Poll_Dis << XIP_WCCR_DISPL_Pos) |
                 (ptInst->PollCount << XIP_WCCR_PLCNT_Pos) |
                 (ptInst->PollRepDelay << XIP_WCCR_PLREPDLY_Pos) );
    XIP->WCCR = u32Temp;
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Delay_Setup
*
* DESCRIPTION:
*   1. to setup delay to meet characteristics of device for QSPI module, only can be called after Hal_Qspi_Baudrate_Set
*
* CALLS
*
* PARAMETERS
*   1. u32RefClk : reference clock (Hz)
*   2. u32Sclk : system clock of QSPI (Hz)
*   3. ptDelay : pointer of Delay timing values
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Qspi_Delay_Setup_impl(uint32_t u32RefClk, uint32_t u32Sclk, S_QSPI_Delay_t *ptDelay)
{
    uint16_t u8Cssot  = ptDelay->Cssot;
    uint16_t u8Cseot  = ptDelay->Cseot;
    uint16_t u8Csdads = ptDelay->Csdads;
    uint16_t u8Csda   = ptDelay->Csda;
    
    uint32_t u32RefClk_ns = (1000000000) / u32RefClk; // how many ns per tick
    uint32_t u32Sclk_ns   = (1000000000) / u32Sclk; // how many ns per sclk
    
    uint8_t u8Cssot_tick  = 0;
    uint8_t u8Cseot_tick  = 1;
    uint8_t u8Csdads_tick = 0;
    uint8_t u8Csda_tick   = 0;
    
    // If CSSOT=X, chip select will be asserted half a sclk_out period before the first rising edge of sclk_out + X*ref_clks.
    // default = 0.5*sclk_out
    if (u8Cssot > (u32Sclk_ns >> 1))
    {
        u8Cssot_tick = ((u8Cssot - (u32Sclk_ns >> 1)) + u32RefClk_ns - 1) / u32RefClk_ns;
    }
    
    // If CSEOT=X, then chip selected will de-assert X*ref_clks after the last falling edge of sclk_out.
    // default = no delay
    u8Cseot_tick = ((u8Cseot + u32RefClk_ns - 1) / u32RefClk_ns) + 1;
    
    // The minimum delay (CSDADS=0) is: 1*sclk_out + 3*ref_clks.
    // If CSDADS=X, then the delay will be: 1*sclk_out + 3*ref_clks + X*ref_clks
    // default = 1*sclk_out + 3*ref_clks
    if (u8Csdads > (u32Sclk_ns + 3 * u32RefClk_ns))
    {
        u8Csdads_tick = ((u8Csdads - (u32Sclk_ns + 3 * u32RefClk_ns)) + u32RefClk_ns - 1) / u32RefClk_ns;
    }
    
    // The minimum delay for chip select to be de-asserted (CSDA=0) is:  1*sclk_out + 1*ref_clk to ensure the chip select is never re-asserted within an sclk_out period.
    // If CSDA=X, then the chip select de-assert time will be: 1*sclk_out + 1*ref_clk + X*ref_clks.
    // default = 1*sclk_out + 1*ref_clk
	if (u8Csda > (u32Sclk_ns + u32RefClk_ns))
    {
        u8Csda_tick = ((u8Csda - (u32Sclk_ns + u32RefClk_ns)) + u32RefClk_ns - 1) / u32RefClk_ns;    
    }
    
    XIP->QDDR = ( (u8Cssot_tick << XIP_QDDR_CSSOT_Pos) |
                  (u8Cseot_tick << XIP_QDDR_CSEOT_Pos) |
                  (u8Csdads_tick << XIP_QDDR_CSDADS_Pos) |
                  (u8Csda_tick << XIP_QDDR_CSDA_Pos) );
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_RxSampleDly_Set
*
* DESCRIPTION:
*   1. to set the delay of read data capturing logic by the specified number of ref_clk cycles
*
* CALLS
*
* PARAMETERS
*   1. u8DlyNum : delay number of ref_clk cycles (0 to 15)
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Qspi_RxSampleDly_Set_impl(uint8_t u8DlyNum)
{
    uint32_t u32Temp = 0;
    
    // set the delay number of ref_clk cycles for read data capture logic
    u32Temp = XIP->RDCR & ~(XIP_RDCR_DRDCL_Msk);
    u32Temp |= (u8DlyNum << XIP_RDCR_DRDCL_Pos);

    XIP->RDCR = u32Temp;
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
void Hal_Qspi_Command_Exec_impl(S_QSPI_Command_t *ptCmd)
{   
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
    uint32_t u32Temp = 0;
    
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32QspiTimeoutMs;

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
    XIP->FCR = u32Temp;
    
    u32TickStart = Hal_Tick_Diff(0);
    // poll for command execution progress
    while ( (XIP->FCR & XIP_FCR_EIP_Msk) ||
            (!(XIP->QCR & XIP_QCR_IDLE_Msk)) ) // Note: need to poll for QSPI IDLE status, otherwise may encounter erase fail
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return;
    }

    // Output read data
    if (ptCmd->RdDataEn)
    {
        ptCmd->RdDataLow = XIP->FCRDRL;
        ptCmd->RdDataUpper = XIP->FCRDRU;
    }
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Dma_Access
*
* DESCRIPTION:
*   1. Read/Write data from/to QSPI by DMA
*
* CALLS
*
* PARAMETERS
*   1. eSlvIdx       : Index of SPI slave. refer to E_SpiSlave_t
*   2. eChannel      : DMA channel.
*   3. u32SrcAddr    : Source address.
*   4. u32DstAddr    : Destination address.
*   5. u32Size       : Size of DMA transfer.
* 
* RETURNS
*   0 : success
*   1 : error
* 
* GLOBALS AFFECTED
*************************************************************************/
uint32_t Hal_Qspi_Dma_Access_impl(E_SpiSlave_t eSlvIdx, E_DMA_CHANNEL eChannel, uint32_t u32SrcAddr, uint32_t u32DstAddr, uint32_t u32Size)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (eChannel >= DMA_Channel_Max)
        return 1;
    
    // u32SrcAddr must be flash_ahb/memory address space
    
    if ( !(IS_XIP_ADDR_SPACE(u32SrcAddr) || IS_MEM_ADDR_SPACE(u32SrcAddr)) )
        return 1;
    
    // u32DstAddr must be flash_ahb/memory address space
    if ( !(IS_XIP_ADDR_SPACE(u32SrcAddr) || IS_MEM_ADDR_SPACE(u32SrcAddr)) )
        return 1;
    
    if (u32Size == 0)
        return 1;
    
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;

    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32QspiTimeoutMs; 
    // TODO: confirm the timeout value, original = 500ms , now only 10 ms
    
    // Check peripheral chip select
    Hal_Qspi_Peri_Select(eSlvIdx);
    
    // config DMA
    S_DMA_Init_t tDma;
    memset(&tDma, 0, sizeof(S_DMA_Init_t));
    
    if (u32Size > MAX_DMA_XFER_NUM)
    {
        int i = 0;
        uint32_t u32LliNum = u32Size / MAX_DMA_XFER_NUM;
        uint32_t u32RemainByte = u32Size % MAX_DMA_XFER_NUM;
        u32LliNum = (u32RemainByte > 0) ? (u32LliNum + 1) : u32LliNum;
        
        // LLI preparation
        S_DMA_LinkListItem_t tLli[u32LliNum];
        memset(&tLli[0], 0, sizeof(S_DMA_LinkListItem_t) * u32LliNum);

        S_DMA_CTL_L_t *ptCtl_L = (S_DMA_CTL_L_t *) &(tLli[0].CTL_L);
        S_DMA_CTL_H_t *ptCtl_H = (S_DMA_CTL_H_t *) &(tLli[0].CTL_H);
        tLli[0].SAR = u32SrcAddr;
        tLli[0].DAR = u32DstAddr;
        tLli[0].LLP = ((uint32_t)(&tLli[1])) & 0xFFFFFFFC;
        // Ctl_L
        ptCtl_L->CTL_L_INT_EN = 1;
        ptCtl_L->CTL_L_DST_TR_WIDTH = DMA_TR_WIDTH_8_bits;
        ptCtl_L->CTL_L_SRC_TR_WIDTH = DMA_TR_WIDTH_8_bits;
        ptCtl_L->CTL_L_DINC = DMA_INC_Increment;
        ptCtl_L->CTL_L_SINC = DMA_INC_Increment;
        ptCtl_L->CTL_L_TT_FC = DMA_TT_m2m_FC_Dma;
        ptCtl_L->CTL_L_LLP_DST_EN = 1;
        ptCtl_L->CTL_L_LLP_SRC_EN = 1;
        // Ctl_H
        ptCtl_H->CTL_H_BLOCK_TS = MAX_DMA_XFER_NUM;
        ptCtl_H->CTL_H_DONE = 1;

        for (i = 1; i < u32LliNum; i++)
        {
            if (i != u32LliNum - 1)
            {
                memcpy(&tLli[i], &tLli[i-1], sizeof(S_DMA_LinkListItem_t));
                tLli[i].SAR = u32SrcAddr + (i * MAX_DMA_XFER_NUM);
                tLli[i].DAR = u32DstAddr + (i * MAX_DMA_XFER_NUM);
                tLli[i].LLP = ((uint32_t)(&tLli[i+1])) & 0xFFFFFFFC;
            }
            else
            {
                memcpy(&tLli[i], &tLli[i-1], sizeof(S_DMA_LinkListItem_t));
                tLli[i].SAR = u32SrcAddr + (i * MAX_DMA_XFER_NUM);
                tLli[i].DAR = u32DstAddr + (i * MAX_DMA_XFER_NUM);
                tLli[i].LLP = 0x0;
                ptCtl_L = (S_DMA_CTL_L_t *) &(tLli[i].CTL_L);
                ptCtl_L->CTL_L_LLP_DST_EN = 0;
                ptCtl_L->CTL_L_LLP_SRC_EN = 0;
                ptCtl_H = (S_DMA_CTL_H_t *) &(tLli[i].CTL_H);
                ptCtl_H->CTL_H_BLOCK_TS = u32RemainByte;
            }
        }
        // Other CFGs would be reprogrammed by LLI
        tDma.LlpDstEn = 1;
        tDma.LlpSrcEn = 1;
        tDma.Loc = ((uint32_t) &tLli[0] >> 2);
    }
    else
    {
        // Setup DMA configurations
        tDma.Sar = u32SrcAddr;
        tDma.Dar = u32DstAddr;
        tDma.eDstTrWidth = DMA_TR_WIDTH_8_bits;
        tDma.eSrcTrWidth = DMA_TR_WIDTH_8_bits;
        tDma.eTtfc = DMA_TT_m2m_FC_Dma;
        tDma.BlockTs = u32Size;
    }
    
    // wait for DMA Channel is idle
    u32TickStart = Hal_Tick_Diff(0);
    
    while (Hal_Dma_Check(eChannel))
    {
        u32TickDiff = Hal_Tick_Diff(u32TickStart);
        if (u32TickDiff > u32TimeoutTicks) return 1;
    }
    Hal_Dma_Config(eChannel, &tDma);
    
    // Enable channel
    Hal_Dma_Enable(eChannel);
    
    return 0;
}


/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Baudrate_Set
*
* DESCRIPTION:
*  1. to set baud rate of QSPI
*
* CALLS
*
* PARAMETERS
*   1. u32Baud : desired clock of QSPI
*
* RETURNS
*   0: setting complete
*   1: error
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Qspi_Baudrate_Set_impl(uint32_t u32Baud)
{
    uint32_t u32Temp = 0;
    uint32_t u32RefClk = 0;
    uint32_t u32Div = 0;
    uint32_t u32DivRegVal = 0;
    
    if (u32Baud == 0)
        return 1;
    
    Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI0, &u32RefClk);
    
    // Find closest value
    u32Div = (u32RefClk + u32Baud - 1) / u32Baud;
    
    if (u32Div < QSPI_BAUDR_MIN)
        u32Div = QSPI_BAUDR_MIN;
    else if (u32Div > QSPI_BAUDR_MAX)
        u32Div = QSPI_BAUDR_MAX;
    
    // Mapping to register value
    // +-----+--+--+--+---+--+--+
    // | reg | 0| 1| 2|...|14|15|  reg = (div/2 - 1)
    // +-----+--+--+--+---+--+--+
    // | div | 2| 4| 6|...|30|32|
    // +-----+--+--+--+---+--+--+
    u32DivRegVal = (u32Div >> 1) - 1;   // (u32Div/2) - 1
    
    // Store baud rate
    u32SpiSpeed[SPI_IDX_0] = u32Baud;
    
    // Set baud rate & Recover QSPI
    u32Temp = XIP->QCR;
    u32Temp &= ~(XIP_QCR_BAUD_Msk);
	u32Temp |= (u32DivRegVal << XIP_QCR_BAUD_Pos);
    XIP->QCR = u32Temp;
    
    return 0;
}


/*************************************************************************
* FUNCTION:
*   Hal_Qspi_Baudrate_Get
*
* DESCRIPTION:
*  1. to get QSPI working buad rate
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   QSPI baud rate
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Qspi_Baudrate_Get_impl(void)
{
    uint32_t u32RefClk = 0;
    uint32_t u32Div = 0;
    uint32_t u32DivRegVal = 0;
    
    u32DivRegVal = ((XIP->QCR & XIP_QCR_BAUD_Msk) >> XIP_QCR_BAUD_Pos);
    u32Div = ((u32DivRegVal + 1) << 1);         // (u32DivRegVal + 1) * 2
    
    Hal_Sys_ApsClockGet(APS_CLK_GRP_SPI0, &u32RefClk);
    
    return u32RefClk / u32Div;
}


/*************************************************************************
* FUNCTION:
*   Hal_Qpsi_Idle_Poll
*
* DESCRIPTION:
*  1. to check if QSPI is busy
*
* CALLS
*
* PARAMETERS
*   None
*
* RETURNS
*   0: QSPI is IDLE
*   1: QSPI is not IDLE, timeout happened
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Qpsi_Idle_Poll_impl(void)
{
    uint32_t u32TimeoutTicks = 0;
    uint32_t u32TickStart = 0;
    uint32_t u32TickDiff = 0;
	
    u32TimeoutTicks = Hal_Tick_PerMilliSec() * g_u32QspiTimeoutMs;
    u32TickStart = Hal_Tick_Diff(0);
    
	// poll for QSPI IDLE status
	while (!(XIP->QCR & XIP_QCR_IDLE_Msk))
	{
		u32TickDiff = Hal_Tick_Diff(u32TickStart);
		if (u32TickDiff > u32TimeoutTicks) return 1;
	}
	
	return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Divider_Update
*
* DESCRIPTION:
*   1. Update QSPI divider
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
void Hal_Qspi_Divider_Update_impl(void)
{
    Hal_Qspi_Baudrate_Set(u32SpiSpeed[SPI_IDX_0]);
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Psram_Instruction_Setup
*
* DESCRIPTION:
*   1. to setup Instruction of Psram Device for QSPI module
*   (for Flash & Psram Coexistence case)
*
* CALLS
*
* PARAMETERS
*   1. ptInst : pointer of configurations
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Qspi_Psram_Instruction_Setup_impl(S_QSPI_Psram_Instruction_t *ptInst)
{
    uint32_t u32Temp = 0;
    
    u32Temp = XIP->PSRAM_DRIR;
    u32Temp &= ~( XIP_PSRAM_DRIR_ROP_Msk |
                  XIP_PSRAM_DRIR_DDRBEN_Msk |
                  XIP_PSRAM_DRIR_ATT_Msk |
                  XIP_PSRAM_DRIR_DTT_Msk |
                  XIP_PSRAM_DRIR_MBEN_Msk |
                  XIP_PSRAM_DRIR_DCC_Msk );
    u32Temp |= ( (ptInst->PsramRdOpCode << XIP_PSRAM_DRIR_ROP_Pos) |
                 (ptInst->PsramDDR_BitEn << XIP_PSRAM_DRIR_DDRBEN_Pos) |
                 (ptInst->ePsramRdAddrXferType << XIP_PSRAM_DRIR_ATT_Pos) |
                 (ptInst->ePsramRdDataXferType << XIP_PSRAM_DRIR_DTT_Pos) |
                 (ptInst->PsramModeBitEn << XIP_PSRAM_DRIR_MBEN_Pos) |
                 (ptInst->PsramRdDummy << XIP_PSRAM_DRIR_DCC_Pos) );
    XIP->PSRAM_DRIR = u32Temp;
    
    u32Temp = XIP->PSRAM_DWIR;
    u32Temp &= ~( XIP_PSRAM_DWIR_WOP_Msk |
                  XIP_PSRAM_DWIR_WDIS_Msk |
                  XIP_PSRAM_DWIR_ATT_Msk |
                  XIP_PSRAM_DWIR_DTT_Msk |
                  XIP_PSRAM_DWIR_DCC_Msk );
    u32Temp |= ( (ptInst->PsramWrOpCode << XIP_PSRAM_DWIR_WOP_Pos) |
                 (ptInst->PsramWEL_Dis << XIP_PSRAM_DWIR_WDIS_Pos) |
                 (ptInst->ePsramWrAddrXferType << XIP_PSRAM_DWIR_ATT_Pos) |
                 (ptInst->ePsramWrDataXferType << XIP_PSRAM_DWIR_DTT_Pos) |
                 (ptInst->PsramWrDummy << XIP_PSRAM_DWIR_DCC_Pos) );
    XIP->PSRAM_DWIR = u32Temp;
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Psram_RxSampleDly_Set
*
* DESCRIPTION:
*   1. to set the delay of read data capturing logic for Psram device by the specified number of ref_clk cycles
*   (for Flash & Psram Coexistence case)
*
* CALLS
*
* PARAMETERS
*   1. u8DlyNum : delay number of ref_clk cycles (0 to 15)
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Qspi_Psram_RxSampleDly_Set_impl(uint8_t u8DlyNum)
{
    uint32_t u32Temp = 0;

    // set the delay number ofr ref_clk cycles for read data capture logic of PSRAM device
    u32Temp = XIP->RDCR & ~(XIP_RDCR_PDRDCL_Msk);
    u32Temp |= (u8DlyNum << XIP_RDCR_PDRDCL_Pos);
    
    XIP->RDCR = u32Temp;
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Psram_Coexistence_Setup
*
* DESCRIPTION:
*  1. to configure QSPI for Flash & Psram Coexistence
* (for Flash & Psram Coexistence case)
*
* CALLS
*
* PARAMETERS
*   1. u8Enable : 1 = Enable
*                 0 = Disable
*   2. eSlvIdx : Index of SPI slave. refer to E_SpiSlave_t.
*
* RETURNS
*   0: setting complete
*   1: error
*
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_Qspi_Psram_Coexistence_Setup_impl(uint8_t u8Enable, E_SpiSlave_t eSlvIdx)
{
    if (eSlvIdx >= SPI_SLAVE_MAX)
        return 1;
    
    if (u8Enable)
    {
        // TODO: refine it 
        
        // AHB_Decode_Enable => auto-select slave# by address
        XIP->QCR |= XIP_QCR_ADEN;
        
        // 0x3000_70BC = 0x2, 4'b0010 => slave#1 is PSRAM
        XIP->FLASH_OR_PSRAM = (1 << eSlvIdx); // 0x2; 
        
        // 0x3000_70C0 = 0x1, this is the switch of the newly modification, if =0, every modifcation is masked
        XIP->AHB_DEC_ACCESS = 0x1;
    }
    else
    {
        XIP->QCR &= ~(XIP_QCR_ADEN_Msk);
        XIP->FLASH_OR_PSRAM = 0;
        XIP->AHB_DEC_ACCESS = 0;
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_Qspi_Enable
*
* DESCRIPTION:
*   1. Enable/Disable QSPI
*
* CALLS
*
* PARAMETERS
*   1. u8Enable : 1 = Enable
*                 0 = Disable
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_Qspi_Enable_impl(uint8_t u8Enable)
{
    if (u8Enable)
	{
        XIP->QCR = XIP->QCR | XIP_QCR_QEN_Msk;
	}
    else
	{
		// poll for QSPI IDLE status
		Hal_Qpsi_Idle_Poll();
		
        XIP->QCR = XIP->QCR & ~(XIP_QCR_QEN_Msk);
	}
}

/*
 *************************************************************************
 *                           Private Functions
 *************************************************************************
 */

