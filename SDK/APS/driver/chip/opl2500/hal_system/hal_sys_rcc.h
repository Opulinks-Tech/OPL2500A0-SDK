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
 * @file hal_sys_rcc.h
 * 
 * @brief API to Reset and Clock Control module
 * 
 */
#ifndef __HAL_SYS_RCC_H__
#define __HAL_SYS_RCC_H__


/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
#include <stddef.h>
#include "opulinks.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/* Source clock definition */
#define CLK_SRC_FREQ_RC                     20000000UL      /* unit: Hz */
#define CLK_SRC_FREQ_XTAL                   20000000UL      /* Default XTAL frequency. When no 32k XTAL to detect, no configuration in flash, and UART ABR timeout */
#define CLK_SRC_FREQ_EXT_APS_CLK_DEF        20000000UL      /* Default external APS clock input. unit: Hz */
#define CLK_SRC_FREQ_160M_BB                160000000UL     /* From VCO 320M Hz. unit: Hz */
#define CLK_SRC_FREQ_150M_VAR               150000000UL     /* From VCO 2442M Hz. unit: Hz */
#define CLK_SRC_FREQ_D1_200M                200000000UL     /* unit: Hz */
#define CLK_SRC_FREQ_D1_180M                180000000UL     /* Security use. unit: Hz */
#define CLK_SRC_FREQ_32K_RC                 32768UL         /* Internal 32k RC unit: Hz */
#define CLK_SRC_FREQ_32K_XTAL               32768UL         /* External 32k XTAL unit: Hz */

#define APS_SET_SHELTER_SRC_TO_RC()         (AOS->APS_CLK_SEL &= ~AOS_APS_CLK_SEL_APS_SRC_2TO1_CLK_SEL)
#define APS_SET_SHELTER_SRC_TO_XTAL()       (AOS->APS_CLK_SEL |= AOS_APS_CLK_SEL_APS_SRC_2TO1_CLK_SEL)
#define SCRT_SET_SHELTER_SRC_TO_RC()        (AOS->SCRT_CLK_APS &= ~AOS_SCRT_CLK_APS_SCRT_SRC_2TO1_CLKSEL)
#define SCRT_SET_SHELTER_SRC_TO_XTAL()      (AOS->SCRT_CLK_APS |= AOS_SCRT_CLK_APS_SCRT_SRC_2TO1_CLKSEL)
#define MSQ_SET_SHELTER_SRC_TO_RC()         (AOS->MSQ_CLK_SEL &= ~AOS_MSQ_CLK_SEL_MSQ_SRC_2TO1_CLK_SEL)
#define MSQ_SET_SHELTER_SRC_TO_XTAL()       (AOS->MSQ_CLK_SEL |= AOS_MSQ_CLK_SEL_MSQ_SRC_2TO1_CLK_SEL)


/* ******************************* RC calibration ******************************* */
/* RC calibration, in RF bank
 * RF_RC_OSC_CAL_CTL [1:0] RG_FREQCOMP_CTL
 * RG_FREQCOMP_CTL [0]: Reset RC_CLK_CAL, low to reset (FREQ_COMP_RESET_N).  
 * RG_FREQCOMP_CTL [1]: Enable RC_CLK_CAL to count (CAL_START)
 */ 
#define RF_RC_OSC_CAL_CTL_FREQ_COMP_RST_N   (1 << 0)
#define RF_RC_OSC_CAL_CTL_CAL_START         (1 << 1)
/* RF->RC_OSC_CAL_CTL pattern length */
#define RC_CAL_PATTERN_LEN                  12
#define RC_CAL_OSC_CTUNE_LEN                4

#define RC_CAL_CEN_MIN                      0x420
#define RC_CAL_CEN_MAX                      0xBC0
 
 
#ifdef OPL2500_FPGA 
#undef CLK_SRC_FREQ_XTAL
#define CLK_SRC_FREQ_XTAL                   22000000UL      /* FPGA default XTAL frequency is 22MHz */
#endif /* OPL2500_FPGA */

#define CLK_US                              1000000
#define CLK_MS                              1000

/* ******************************* Setup function ******************************* */
/* Register bit definition */
#define REG_BDEF_VALID_POS                  0
#define REG_BDEF_TYPE_POS                   1
#define REG_BDEF_VALUE_POS                  2
#define REG_BDEF_T1_REG_OFST_POS            2
#define REG_BDEF_T1_BIT_POS0_POS            11
#define REG_BDEF_T1_BIT_POS1_POS            17
#define REG_BDEF_T1_BIT_POS2_POS            23
#define REG_BDEF_T1_BIT_POS3_POS            29
#define REG_BDEF_T2_REG_OFST0_POS           2
#define REG_BDEF_T2_BIT_OFST0_POS           11
#define REG_BDEF_T2_REG_OFST1_POS           16
#define REG_BDEF_T2_BIT_OFST1_POS           25

#define REG_BDEF_RESERVED_DEFINE            0
#define REG_BDEF_VALID_REG_BIT_DEF          1

#define REG_BDEF_TYPE_ONE_REG               0
#define REG_BDEF_TYPE_TWO_REG               1


#define REG_CFG_SP_VALUE(value)                                     ( (REG_BDEF_RESERVED_DEFINE) | ((value) << REG_BDEF_VALUE_POS) )
            
#define REG_CFG_T1_ONE_REG                                          ( (REG_BDEF_VALID_REG_BIT_DEF << REG_BDEF_VALID_POS) | (REG_BDEF_TYPE_ONE_REG << REG_BDEF_TYPE_POS) )
#define REG_CFG_T1_GET_REG_OFST(reg)                                ( offsetof(S_AOS_Reg_t, reg) << REG_BDEF_T1_REG_OFST_POS )
#define REG_CFG_T1_GET_BITPOS0(bit_pos)                             ( (bit_pos) << REG_BDEF_T1_BIT_POS0_POS )
#define REG_CFG_T1_GET_BITPOS1(bit_pos)                             ( (bit_pos) << REG_BDEF_T1_BIT_POS1_POS )
#define REG_CFG_T1_GET_BITPOS2(bit_pos)                             ( (bit_pos) << REG_BDEF_T1_BIT_POS2_POS )
#define REG_CFG_T1_GET_BITPOS3(bit_pos)                             ( (bit_pos) << REG_BDEF_T1_BIT_POS3_POS )
#define REG_BDEF_T1_BITPOS_RSV                                      (0x3F)
#define REG_CFG_T1_VALUE_POS1(reg, bit_pos0)                        ( REG_CFG_T1_ONE_REG | \
                                                                      REG_CFG_T1_GET_REG_OFST(reg) | \
                                                                      REG_CFG_T1_GET_BITPOS0(bit_pos0) | \
                                                                      REG_CFG_T1_GET_BITPOS1(REG_BDEF_T1_BITPOS_RSV) |\
                                                                      REG_CFG_T1_GET_BITPOS2(REG_BDEF_T1_BITPOS_RSV) )
#define REG_CFG_T1_VALUE_POS2(reg, bit_pos0, bit_pos1)              ( REG_CFG_T1_ONE_REG | \
                                                                      REG_CFG_T1_GET_REG_OFST(reg) | \
                                                                      REG_CFG_T1_GET_BITPOS0(bit_pos0) | \
                                                                      REG_CFG_T1_GET_BITPOS1(bit_pos1) |\
                                                                      REG_CFG_T1_GET_BITPOS2(REG_BDEF_T1_BITPOS_RSV) )
#define REG_CFG_T1_VALUE_POS3(reg, bit_pos0, bit_pos1, bit_pos2)    ( REG_CFG_T1_ONE_REG | \
                                                                      REG_CFG_T1_GET_REG_OFST(reg) | \
                                                                      REG_CFG_T1_GET_BITPOS0(bit_pos0) | \
                                                                      REG_CFG_T1_GET_BITPOS1(bit_pos1) |\
                                                                      REG_CFG_T1_GET_BITPOS2(bit_pos2) )

#define REG_CFG_T2_TWO_REG                                          ( (REG_BDEF_VALID_REG_BIT_DEF << REG_BDEF_VALID_POS) | (REG_BDEF_TYPE_TWO_REG << REG_BDEF_TYPE_POS) )
#define REG_CFG_T2_REG_BIT_0(reg, bit_pos)                          ( (offsetof(S_AOS_Reg_t, reg) << REG_BDEF_T2_REG_OFST0_POS) | ( (bit_pos) << REG_BDEF_T2_BIT_OFST0_POS ) )
#define REG_CFG_T2_REG_BIT_1(reg, bit_pos)                          ( (offsetof(S_AOS_Reg_t, reg) << REG_BDEF_T2_REG_OFST1_POS) | ( (bit_pos) << REG_BDEF_T2_BIT_OFST1_POS ) )
#define REG_CFG_T2_VALUE(reg0, bit_pos0, reg1, bit_pos1)            ( (int32_t)(REG_CFG_T2_TWO_REG | \
                                                                                REG_CFG_T2_REG_BIT_0(reg0, bit_pos0) | \
                                                                                REG_CFG_T2_REG_BIT_1(reg1, bit_pos1)) )

/* ******************************* Module reset ********************************* */
#define RCC_SWRST_STATUS_TIMEOUT                    30

/* ******************************* Module enable ******************************** */
                                
#define AOS_APS_CLK_SEL_ALL_APS                     (AOS_APS_CLK_SEL_I2C_PCLK_EN |          \
                                                     AOS_APS_CLK_SEL_TMR0_PCLK_EN |         \
                                                     AOS_APS_CLK_SEL_TMR1_PCLK_EN |         \
                                                     AOS_APS_CLK_SEL_WDT_PCLK_EN |          \
                                                     AOS_APS_CLK_SEL_UART0_PCLK_EN |        \
                                                     AOS_APS_CLK_SEL_UART1_PCLK_EN |        \
                                                     AOS_APS_CLK_SEL_APS_UART_PCLK_EN |     \
                                                     AOS_APS_CLK_SEL_OTP_PCLK_EN |          \
                                                     AOS_APS_CLK_SEL_APS_DMA_HCLK_EN |      \
                                                     AOS_APS_CLK_SEL_I2S_PCLK_EN |          \
                                                     AOS_APS_CLK_SEL_JTAG_HCLK_EN |         \
                                                     AOS_APS_CLK_SEL_APS_WDT_EN)

#define AOS_MSQ_CLK_SEL_ALL_MSQ                     (AOS_MSQ_CLK_SEL_MSQ_DMA_HCLK_EN |      \
                                                     AOS_MSQ_CLK_SEL_MAC_HCLK_EN |          \
                                                     AOS_MSQ_CLK_SEL_PHY_HCLK_EN |          \
                                                     AOS_MSQ_CLK_SEL_MSQ_WDT_HCLK_EN |      \
                                                     AOS_MSQ_CLK_SEL_MSQ_VIC_HCLK_EN |      \
                                                     AOS_MSQ_CLK_SEL_MSQ_PU_HCLK_EN |       \
                                                     AOS_MSQ_CLK_SEL_MSQ_WDT_EN |           \
                                                     AOS_MSQ_CLK_SEL_MSQ_UART_PCLK_EN |     \
                                                     AOS_MSQ_CLK_SEL_PU_EVENT_HCLK_EN)
  
#define AOS_APS_DOMAIN_EN_ALL_APS                   (AOS_APS_DOMAIN_EN_PWM_CLK_EN |         \
                                                     AOS_APS_DOMAIN_EN_UART_ABR_CLK_EN |    \
                                                     AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN)

#define AOS_SCRT_CLK_APS_SEL_ALL_APS                (AOS_SCRT_CLK_APS_SCRT_GLB_CLK_APS_EN)

#define AOS_SCRT_CLK_MSQ_SEL_ALL_MSQ                (AOS_SCRT_CLK_MSQ_SCRT_GLB_CLK_MSQ_EN)

#define AOS_APS_STCLK_ALL_APS                       (AOS_APS_STCLK_APS_STCLK_SRC_EN)

                                    
#define AOS_SPI_CLK_SEL_ALL_APS                     (AOS_SPI_CLK_SEL_SPI0_HCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI0_PCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI1_PCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI2_PCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI3_PCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI0_REFCLK_EN |       \
                                                     AOS_SPI_CLK_SEL_SPI1_SCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI2_SCLK_EN |         \
                                                     AOS_SPI_CLK_SEL_SPI3_SCLK_EN)                                    
                                     
#define AOS_APS_DM_CLK_SEL_ALL_APS                  (AOS_APS_DM_CLK_SEL_I2S_SCLK_EN |           \
                                                     AOS_APS_DM_CLK_SEL_APSXIP_CACHE_HCLK_EN |  \
                                                     AOS_APS_DM_CLK_SEL_APSXIP_PREFT_HCLK_EN |  \
                                                     AOS_APS_DM_CLK_SEL_PDM_IO_CLK_EN |         \
                                                     AOS_APS_DM_CLK_SEL_PDM_CLK_DIV_EN |        \
                                                     AOS_APS_DM_CLK_SEL_PDM_CLK_EN)

#define AOS_CAM_XVCLK_CLK_APS                       (AOS_CAM_XVCLK_SEL_CAM_XVCLK_EN)

/* ******************************* Source clock ********************************* */
#define RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB_Pos     (11U)
#define RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB_Msk     (0x00000001U << RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB_Pos)
#define RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB         RF_CLKGEN_CTL0_CK_GATE_CTRL_XTAL_BB_Msk

#define RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB_Pos       (14U)
#define RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB_Msk       (0x00000001U << RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB_Pos)
#define RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB           RF_CLKGEN_CTL0_CK_GATE_CTRL_RC_BB_Msk

#define RF_PU_VAL_PU_RC_OSC_Pos                     (13U)
#define RF_PU_VAL_PU_RC_OSC_Msk                     (0x00000001U << RF_PU_VAL_PU_RC_OSC_Pos)
#define RF_PU_VAL_PU_RC_OSC                         RF_PU_VAL_PU_RC_OSC_Msk

#define RF_PU_VAL_PU_XTAL_Pos                       (14U)
#define RF_PU_VAL_PU_XTAL_Msk                       (0x00000001U << RF_PU_VAL_PU_XTAL_Pos)
#define RF_PU_VAL_PU_XTAL                           RF_PU_VAL_PU_XTAL_Msk

/* XTAL detection */
/* MSQ 32k XTAL setting */
#define MSQ_CLK_32k_XTAL_GAIN                       0xF
#define MSQ_CLK_32k_XTAL_CAP                        0
#define AOS_REG_OVCTL_FW                            (0 << AOS_RET_MUX_RET_MUX_Pos)

#define DET_XTAL_HIGH_XTAL_DET_MAX                  (28600000UL)    /* Maximum supported detected XTAL frequency: 26M + 10% */
#define DET_XTAL_PRE_DET_32K_CYCLE                  (4UL)           /* The wait 32K cycle count in pre-detection */
#define DET_XTAL_PRE_DET_32K_TIMEOUT                (DET_XTAL_PRE_DET_32K_CYCLE*(DET_XTAL_HIGH_XTAL_DET_MAX/CLK_SRC_FREQ_32K_XTAL))     /* Pre-detection 32K timeout cycles */
#define DET_XTAL_DET_POLLING_TIMEOUT                (DET_XTAL_PRE_DET_32K_TIMEOUT)

/* ******************************* System clock ********************************* */
/* The divider bit value to real divider value
 * e.g. Bit value = 1, the divider is clock divided by 2 */
#define CLK_DIVN_VALUE_OFST                         (1)

/* APS System clock */
#define APS_CLK_SYS_SWITCH_TO_TARGET()              (AOS->APS_CLK_SEL &= ~AOS_APS_CLK_SEL_APS_CLK_SRC_DYN_SEL_Msk)
#define APS_CLK_SYS_SWITCH_TO_SHELTER()             (AOS->APS_CLK_SEL |= 1 << AOS_APS_CLK_SEL_APS_CLK_SRC_DYN_SEL_Pos)
#define APS_CLK_SYS_DIVN_GET(value)                 ((((value) & AOS_APS_CLK_SEL_APS_CLK_DIV_SEL_Msk) >> AOS_APS_CLK_SEL_APS_CLK_DIV_SEL_Pos) + CLK_DIVN_VALUE_OFST)
#define APS_CLK_PCLK_DIVN_GET(value)                ((((value) & AOS_APS_CLK_SEL_APS_PCLK_DIV_SEL_Msk) >> AOS_APS_CLK_SEL_APS_PCLK_DIV_SEL_Pos) + CLK_DIVN_VALUE_OFST)

#define MSQ_CLK_SYS_SRC_SEL_MSK                     (AOS_MSQ_CLK_SEL_MSQ_SRC1_CLKSEL_Msk | AOS_MSQ_CLK_SEL_MSQ_SRC2_CLKSEL_Msk)
#define MSQ_CLK_SYS_SRC_SEL_POS                     (AOS_MSQ_CLK_SEL_MSQ_SRC1_CLKSEL_Pos)

/* ******************************* Peripherl clock ****************************** */
/* APS security clock */
#define APS_CLK_SCRT_SWITCH_TO_TARGET()             (AOS->SCRT_CLK_APS &= ~AOS_SCRT_CLK_APS_SCRT_CLK_SRC_DYNAMIC_SEL_Msk)
#define APS_CLK_SCRT_SWITCH_TO_SHELTER()            (AOS->SCRT_CLK_APS |= 1 << AOS_SCRT_CLK_APS_SCRT_CLK_SRC_DYNAMIC_SEL_Pos)
#define APS_CLK_SCRT_DIVN_GET(value)                ((((value) & AOS_SCRT_CLK_APS_SCRT_CLK_DIVN_SEL_Msk) >> AOS_SCRT_CLK_APS_SCRT_CLK_DIVN_SEL_Pos) + CLK_DIVN_VALUE_OFST)

///* APS SysTick clock */
//#define APS_CLK_ST_DIVN_GET(value)                  ( 8 >> ((value) >> AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos) )    /* 0: 8, 1:4, 2: 2, 3:1 */

/* APS Timer clock divider fixed at 8 */
#define APS_CLK_TMR_CLK_DIV                         8

/* APS SPI clock */
#define APS_CLK_SPI0_DIVN_GET(value)                ((((value) & AOS_SPI_CLK_SEL_SPI0_REFCLK_DIVN_Msk) >> AOS_SPI_CLK_SEL_SPI0_REFCLK_DIVN_Pos) + CLK_DIVN_VALUE_OFST)
#define APS_CLK_SPI1_DIVN_GET(value)                ((((value) & AOS_SPI_CLK_SEL_SPI1_SCLK_DIV_Msk) >> AOS_SPI_CLK_SEL_SPI1_SCLK_DIV_Pos) + CLK_DIVN_VALUE_OFST)
#define APS_CLK_SPI2_DIVN_GET(value)                ((((value) & AOS_SPI_CLK_SEL_SPI2_SCLK_DIV_Msk) >> AOS_SPI_CLK_SEL_SPI2_SCLK_DIV_Pos) + CLK_DIVN_VALUE_OFST)
#define APS_CLK_SPI3_DIVN_GET(value)                ((((value) & AOS_SPI_CLK_SEL_SPI3_SCLK_DIV_Msk) >> AOS_SPI_CLK_SEL_SPI3_SCLK_DIV_Pos) + CLK_DIVN_VALUE_OFST)

/* APS I2S clock */
#define APS_CLK_I2S_DIVN_GET(value)                 ((((value) & AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Msk) >> AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos) + CLK_DIVN_VALUE_OFST)

/* APS UART clock */
#define APS_CLK_UART0_SRC_MSK                       (AOS_APS_DOMAIN_EN_UART0_SRC_SEL_Msk | AOS_APS_DOMAIN_EN_UART0_SRC_DYNAMIC_SEL_Msk)
#define APS_CLK_UART0_SRC_POS                       (AOS_APS_DOMAIN_EN_UART0_SRC_SEL_Pos)
#define APS_CLK_UART1_SRC_MSK                       (AOS_APS_DOMAIN_EN_UART1_SRC_SEL_Msk | AOS_APS_DOMAIN_EN_UART1_SRC_DYNAMIC_SEL_Msk)
#define APS_CLK_UART1_SRC_POS                       (AOS_APS_DOMAIN_EN_UART1_SRC_SEL_Pos)
#define APS_CLK_APS_UART_SRC_MSK                    (AOS_APS_DOMAIN_EN_APS_UART_SRC_SEL_Msk | AOS_APS_DOMAIN_EN_APS_UART_SRC_DYNAMIC_SEL_Msk)
#define APS_CLK_APS_UART_SRC_POS                    (AOS_APS_DOMAIN_EN_APS_UART_SRC_SEL_Pos)
#define APS_CLK_UART_ABR_SRC_MSK                    (AOS_APS_DOMAIN_EN_UART_ABR_SRC_SEL_Msk | AOS_APS_DOMAIN_EN_UART_ABR_SRC_DYNAMIC_SEL_Msk)
#define APS_CLK_UART_ABR_SRC_POS                    (AOS_APS_DOMAIN_EN_UART_ABR_SRC_SEL_Pos)

/* APS PDM clock */
#define APS_CLK_PDM_DIVN_GET(value)                 ((((value) & AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_DIV_Msk) >> AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_DIV_Pos) + CLK_DIVN_VALUE_OFST) 
/* APS PDM IO clock */
#define APS_CLK_PDM_IO_DIVN_GET(value)              ((((value) & AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Msk) >> AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos) + CLK_DIVN_VALUE_OFST) 

/* APS EXT 2 IO clock */
#define APS_CLK_EXT2IO_DIVN_GET(value)              ( 2 << ((value) >> AOS_APS_DOMAIN_EN_EXT_2IO_CLK_DIV_Pos) )

/* CAM XV clock */
#define APS_CLK_CAM_XVCLK_DIVN_GET(value)           ((((value) & AOS_CAM_XVCLK_SEL_CAM_XVCLK_DIVN_Msk) >> AOS_CAM_XVCLK_SEL_CAM_XVCLK_DIVN_Pos) + CLK_DIVN_VALUE_OFST)

/* MSQ 32k XTAL setting */
#define MSQ_CLK_32k_XTAL_GAIN                       0xF
#define MSQ_CLK_32k_XTAL_CAP                        0

/* MSQ RTC clock source */
#define MSQ_RTC_CLK_SRC_LPO                         0
#define MSQ_RTC_CLK_SRC_32K_XTAL                    1

/* ******************************* Clock modulator ****************************** */
#define APS_MMFACTOR_MAX                            0x10
#define APS_MMFACTOR_DEFAULT                        16
#define APS_MMFACTOR_DIVISOR                        16


/*
 *************************************************************************
 *						    Enums and Structures
 *************************************************************************
 */

/** Store clock frequency. Unit: Hz */
typedef struct {
    /* MMD Factor */
    uint32_t u32MMFactor;
    /* Common clock */
    uint32_t u32ExtApsClk;
    uint32_t u32XtalClk;
    uint32_t u32SysClk;         /* After MMD */
    uint32_t u32SysSrcClk;      /* Before MMD */
    uint32_t u32Pclk;
    /* Peripheral clock */
    uint32_t u32PwmClk;
    uint32_t u32ScrtClk;
    uint32_t u32Spi0Clk;
    uint32_t u32Spi1Clk;
    uint32_t u32Spi2Clk;
    uint32_t u32Spi3Clk;
    uint32_t u32I2sClk;
    uint32_t u32Uart0Clk;
    uint32_t u32Uart1Clk;
    uint32_t u32ApsUartClk;
    uint32_t u32UartAbrClk;
    uint32_t u32PdmIoClk;
    uint32_t u32PdmClk;
    uint32_t u32Ext2IoClk;
    uint32_t u32Tmr0ExtClk;         /* XTAL/8 */
    uint32_t u32Tmr1ExtClk;         /* XTAL/8 */
    uint32_t u32StClk;
    uint32_t u32CamXvClk;
}S_APS_CLK_FREQ;

#pragma push
#pragma anon_unions
typedef struct
{
    union {
        struct {
            uint32_t u32ValidDef:1;     /* 0: Not for Reg/Bit definition setup. 1: Using following format to setup. */
            uint32_t u32SetType:1;      /* 0: Multi bits in one register. 1: Two bits in two registers */
            uint32_t Value:30;          /* When not valid reg/bit definition setup. Handle the value by previous layer */
        };
        struct {
            /* Type 0: multi-bits in one register */
            uint32_t u32ValidDefT1:1;   /* 0: Not for Reg/Bit definition setup. 1: Using following format to setup. */
            uint32_t u32SetTypeT1:1;    /* 0: Multi bits in one register. 1: Two bits in two registers */
            uint32_t RegOfst:9;         /* Register offset: 0x134, 0x138, 0x140, 0x194, or 0x1A4 */
            uint32_t BitPos0:6;         /* Bit pos #0. 0x3F as reserved */
            uint32_t BitPos1:6;         /* Bit pos #1. 0x3F as reserved */
            uint32_t BitPos2:6;         /* Bit pos #2. 0x3F as reserved */
            uint32_t ReservedT1:3;      /* Reserved for MSB */
        };
        struct {
            /* Type 1: Two bits in two registers. For two register or the bit range over type 1 */
            uint32_t u32ValidDefT2:1;   /* 0: Not for Reg/Bit definition setup. 1: Using following format to setup. */
            uint32_t u32SetTypeT2:1;    /* 0: Multi bits in one register. 1: Two bits in two registers */
            uint32_t RegOfst0:9;        /* First register offset: 0x134, 0x138, 0x140, 0x194, or 0x1A4 */
            uint32_t BitOfst0:5;        /* First Bit position */
            uint32_t RegOfst1:9;        /* Second register offset: 0x134, 0x138, 0x140, 0x194, or 0x1A4 */
            uint32_t BitOfst1:5;        /* Second Bit position */
            uint32_t ReservedT2:2;      /* Reserved for MSB */
        };
    };
} S_REG_BIT_DEF;
#pragma pop


/* ******************************* Module reset ********************************* */
typedef enum
{
    RESET_BY_APS_WDT            = AOS_RSTS_RST_APS_WDT,
    RESET_BY_MSQ_WDT            = AOS_RSTS_RST_MSQ_WDT,
    RESET_BY_SWRST              = AOS_RSTS_RST_SWRST,
    RESET_BY_CPOR               = AOS_RSTS_RST_CPOR,
    RESET_BY_SPOR               = AOS_RSTS_RST_SPOR,
    RESET_BY_PAD                = AOS_RSTS_RST_PAD,
    RESET_STS_WARM              = RESET_BY_CPOR,
} E_RstSrc_t;

typedef enum 
{   
    APS_RST_I2C                 = AOS_APS_SWRST_I2C_RST,
    APS_RST_I2S                 = AOS_APS_SWRST_I2S_RST,
    APS_RST_PWM                 = AOS_APS_SWRST_PWM_RST,
    APS_RST_TIMER_0             = AOS_APS_SWRST_TMR0_RST,
    APS_RST_TIMER_1             = AOS_APS_SWRST_TMR1_RST,
    APS_RST_WDT                 = AOS_APS_SWRST_WDT_RST,
    APS_RST_VIC                 = AOS_APS_SWRST_VIC_RST,
    APS_RST_SPI_0               = AOS_APS_SWRST_SPI0_RST,
    APS_RST_SPI_1               = AOS_APS_SWRST_SPI1_RST,
    APS_RST_SPI_2               = AOS_APS_SWRST_SPI2_RST,
    APS_RST_UART_0              = AOS_APS_SWRST_UART0_RST,
    APS_RST_UART_1              = AOS_APS_SWRST_UART1_RST,
    APS_RST_DBG_UART            = AOS_APS_SWRST_APS_UART_RST,
    APS_RST_OTP                 = AOS_APS_SWRST_OTP_RST,
    APS_RST_DMA                 = AOS_APS_SWRST_APS_DMA_RST,
    APS_RST_JTAG                = AOS_APS_SWRST_APS_JTAG_RST,
    APS_RST_SCRT                = AOS_APS_SWRST_SCRT_RST,
    APS_RST_TAP                 = AOS_APS_SWRST_TAP_RST,
    APS_RST_UART_ABR            = AOS_APS_SWRST_UART_ABR_SWRST_EN,
    APS_RST_SPI_3               = AOS_APS_SWRST_SPI3_SWRST_EN,
    APS_RST_XIP_CACHE           = AOS_APS_SWRST_APSXIP_CACHE_SWRST_EN,
    APS_RST_XIP_PREFETCH        = AOS_APS_SWRST_APSXIP_PREFT_SWRST_EN,
} E_ApsRstModule_t;

typedef enum 
{
    MSQ_RST_DMA                 = AOS_MSQ_SWRST_MSQ_DMA_RST,
    MSQ_RST_MAC                 = AOS_MSQ_SWRST_MAC_RST,
    MSQ_RST_WDT                 = AOS_MSQ_SWRST_MSQ_WDT_RST,
    MSQ_RST_VIC                 = AOS_MSQ_SWRST_MSQ_VIC_RST,
    MSQ_RST_PHY                 = AOS_MSQ_SWRST_PHY_RST,
    MSQ_RST_CORE_M0             = AOS_MSQ_SWRST_MSQ_CPU_RST,
    MSQ_RST_RFPHY               = AOS_MSQ_SWRST_RFPHY_RST,
    MSQ_RST_DBG_UART            = AOS_MSQ_SWRST_MSQ_UART_RST,
    MSQ_RST_PROG_PU             = AOS_MSQ_SWRST_PROG_PU_RST,
    MSQ_RST_MMD_CH              = AOS_MSQ_SWRST_MMD_CH_RST,
    MSQ_RST_MMD_BB              = AOS_MSQ_SWRST_MMD_BB_RST,
    MSQ_RST_PU_EVENT            = AOS_MSQ_SWRST_PU_EVENT_RST,
    MSQ_RST_PDM                 = AOS_MSQ_SWRST_PDM_RST,
    MSQ_RST_PIN                 = AOS_MSQ_SWRST_PIN_RST,
    MSQ_RST_MMD_DFM             = AOS_MSQ_SWRST_MMD_DFM_SWRST_EN,
} E_MsqRstModule_t;


/* ******************************* Module enable ******************************** */
typedef enum
{
    AOS_CLK_SET_APS_ALL         = 1,
    AOS_CLK_SET_MSQ_ALL         = 2,
} E_AOS_CLK_SET;

typedef enum 
{
    /* pclk domain */
    APS_CLK_I2C                 = REG_CFG_T1_VALUE_POS1(APS_CLK_SEL, AOS_APS_CLK_SEL_I2C_PCLK_EN_Pos),
    APS_CLK_TIMER_0             = REG_CFG_T1_VALUE_POS1(APS_CLK_SEL, AOS_APS_CLK_SEL_TMR0_PCLK_EN_Pos),
    APS_CLK_TIMER_1             = REG_CFG_T1_VALUE_POS1(APS_CLK_SEL, AOS_APS_CLK_SEL_TMR1_PCLK_EN_Pos),
    APS_CLK_WDT                 = REG_CFG_T1_VALUE_POS2(APS_CLK_SEL, AOS_APS_CLK_SEL_WDT_PCLK_EN_Pos, AOS_APS_CLK_SEL_APS_WDT_EN_Pos),
    APS_CLK_VIC                 = REG_CFG_T1_VALUE_POS1(APS_CLK_SEL, AOS_APS_CLK_SEL_VIC_PCLK_EN_Pos),
    APS_CLK_SPI_0               = REG_CFG_T1_VALUE_POS3(SPI_CLK_SEL, AOS_SPI_CLK_SEL_SPI0_HCLK_EN_Pos, AOS_SPI_CLK_SEL_SPI0_PCLK_EN_Pos, AOS_SPI_CLK_SEL_SPI0_REFCLK_EN_Pos),
    APS_CLK_SPI_1               = REG_CFG_T1_VALUE_POS2(SPI_CLK_SEL, AOS_SPI_CLK_SEL_SPI1_PCLK_EN_Pos, AOS_SPI_CLK_SEL_SPI1_SCLK_EN_Pos),
    APS_CLK_SPI_2               = REG_CFG_T1_VALUE_POS2(SPI_CLK_SEL, AOS_SPI_CLK_SEL_SPI2_PCLK_EN_Pos, AOS_SPI_CLK_SEL_SPI2_SCLK_EN_Pos),
    APS_CLK_SPI_3               = REG_CFG_T1_VALUE_POS2(SPI_CLK_SEL, AOS_SPI_CLK_SEL_SPI3_PCLK_EN_Pos, AOS_SPI_CLK_SEL_SPI3_SCLK_EN_Pos),
    APS_CLK_UART_0              = REG_CFG_T1_VALUE_POS1(APS_CLK_SEL, AOS_APS_CLK_SEL_UART0_PCLK_EN_Pos),
    APS_CLK_UART_1              = REG_CFG_T1_VALUE_POS1(APS_CLK_SEL, AOS_APS_CLK_SEL_UART1_PCLK_EN_Pos),
    APS_CLK_DBG_UART            = REG_CFG_T1_VALUE_POS1(APS_CLK_SEL, AOS_APS_CLK_SEL_APS_UART_PCLK_EN_Pos),
    APS_CLK_UART_ABR            = REG_CFG_T1_VALUE_POS1(APS_DOMAIN_EN, AOS_APS_DOMAIN_EN_UART_ABR_CLK_EN_Pos),
    APS_CLK_OTP                 = REG_CFG_T1_VALUE_POS1(APS_CLK_SEL, AOS_APS_CLK_SEL_OTP_PCLK_EN_Pos),
    APS_CLK_I2S                 = REG_CFG_T2_VALUE(APS_CLK_SEL, AOS_APS_CLK_SEL_I2S_PCLK_EN_Pos, APS_DM_CLK_SEL, AOS_APS_DM_CLK_SEL_I2S_SCLK_EN_Pos),
    /* hclk domain */
    APS_CLK_DMA                 = REG_CFG_T1_VALUE_POS1(APS_CLK_SEL, AOS_APS_CLK_SEL_APS_DMA_HCLK_EN_Pos),
    APS_CLK_SCRT                = REG_CFG_T1_VALUE_POS1(SCRT_CLK_APS, AOS_SCRT_CLK_APS_SCRT_GLB_CLK_APS_EN_Pos),
    APS_CLK_JTAG                = REG_CFG_T1_VALUE_POS1(APS_CLK_SEL, AOS_APS_CLK_SEL_JTAG_HCLK_EN_Pos),
    APS_CLK_XIP_CACHE           = REG_CFG_T1_VALUE_POS1(APS_DM_CLK_SEL, AOS_APS_DM_CLK_SEL_APSXIP_CACHE_HCLK_EN_Pos),
    APS_CLK_XIP_PREFETCH        = REG_CFG_T1_VALUE_POS1(APS_DM_CLK_SEL, AOS_APS_DM_CLK_SEL_APSXIP_PREFT_HCLK_EN_Pos),
	APS_CLK_PDM_IO              = REG_CFG_T1_VALUE_POS1(APS_DM_CLK_SEL, AOS_APS_DM_CLK_SEL_PDM_IO_CLK_EN_Pos),
	APS_CLK_PDM                 = REG_CFG_T1_VALUE_POS1(APS_DM_CLK_SEL, AOS_APS_DM_CLK_SEL_PDM_CLK_EN_Pos),
    /* stand-alone clock  */
    APS_CLK_PWM                 = REG_CFG_T1_VALUE_POS1(APS_DOMAIN_EN, AOS_APS_DOMAIN_EN_PWM_CLK_EN_Pos),
    APS_CLK_XVCLK               = REG_CFG_T1_VALUE_POS1(CAM_XVCLK_SEL, AOS_CAM_XVCLK_SEL_CAM_XVCLK_EN_Pos),
    /* all */
    APS_CLK_ALL                 = REG_CFG_SP_VALUE(AOS_CLK_SET_APS_ALL),
} E_ApsClkEnModule_t;

typedef enum 
{
    /* pclk domain */
    MSQ_CLK_DBG_UART            = AOS_MSQ_CLK_SEL_MSQ_UART_PCLK_EN,
    /* hclk domain */
    MSQ_CLK_DMA                 = AOS_MSQ_CLK_SEL_MSQ_DMA_HCLK_EN,
    MSQ_CLK_MAC                 = AOS_MSQ_CLK_SEL_MAC_HCLK_EN,
    MSQ_CLK_PHY                 = AOS_MSQ_CLK_SEL_PHY_HCLK_EN,
    MSQ_CLK_WDT                 = AOS_MSQ_CLK_SEL_MSQ_WDT_HCLK_EN | AOS_MSQ_CLK_SEL_MSQ_WDT_EN,
    MSQ_CLK_VIC                 = AOS_MSQ_CLK_SEL_MSQ_VIC_HCLK_EN,
    MSQ_CLK_PU                  = AOS_MSQ_CLK_SEL_MSQ_PU_HCLK_EN,
    MSQ_CLK_PU_EVT              = (int32_t)AOS_MSQ_CLK_SEL_PU_EVENT_HCLK_EN,

    /* all */
    MSQ_CLK_ALL                 = (int32_t)(AOS_MSQ_CLK_SEL_MSQ_DMA_HCLK_EN |  \
                                     AOS_MSQ_CLK_SEL_MAC_HCLK_EN |      \
                                     AOS_MSQ_CLK_SEL_PHY_HCLK_EN |      \
                                     AOS_MSQ_CLK_SEL_MSQ_WDT_HCLK_EN |  \
                                     AOS_MSQ_CLK_SEL_MSQ_VIC_HCLK_EN |  \
                                     AOS_MSQ_CLK_SEL_MSQ_PU_HCLK_EN |   \
                                     AOS_MSQ_CLK_SEL_MSQ_WDT_EN |       \
                                     AOS_MSQ_CLK_SEL_MSQ_UART_PCLK_EN | \
                                     AOS_MSQ_CLK_SEL_PU_EVENT_HCLK_EN)
} E_MsqClkEnModule_t;


/* ******************************* Source clock ********************************* */
typedef enum 
{
    RC_LOW_BY_SEQ               = (0 << AOS_OSC_CTL_LPO_ON_Pos),
    RC_LOW_OFF                  = (1 << AOS_OSC_CTL_LPO_ON_Pos) | (0 << AOS_OSC_CTL_32K_LPO_EN_Pos),
    RC_LOW_ON                   = (1 << AOS_OSC_CTL_LPO_ON_Pos) | (1 << AOS_OSC_CTL_32K_LPO_EN_Pos),
    RC_LOW_MSK                  = (AOS_OSC_CTL_LPO_ON_Msk | AOS_OSC_CTL_32K_LPO_EN_Msk),
}E_ClkSrcRcLow_t;

typedef enum 
{
    XTAL_LOW_BY_SEQ             = (0 << AOS_OSC_CTL_XTAL_ON_Pos),
    XTAL_LOW_OFF                = (1 << AOS_OSC_CTL_XTAL_ON_Pos) | (0 << AOS_OSC_CTL_32K_XTAL_EN_Pos),
    XTAL_LOW_ON                 = (1 << AOS_OSC_CTL_XTAL_ON_Pos) | (1 << AOS_OSC_CTL_32K_XTAL_EN_Pos) | (0 << AOS_OSC_CTL_32K_XTAL_BYPASS_Pos),
    XTAL_LOW_BYPS_ON            = (1 << AOS_OSC_CTL_XTAL_ON_Pos) | (1 << AOS_OSC_CTL_32K_XTAL_EN_Pos) | (1 << AOS_OSC_CTL_32K_XTAL_BYPASS_Pos),
    XTAL_LOW_MSK                = (AOS_OSC_CTL_XTAL_ON_Msk | AOS_OSC_CTL_32K_XTAL_EN_Msk | AOS_OSC_CTL_32K_XTAL_BYPASS_Msk),
}E_ClkSrcXtalLow_t;

typedef enum
{
    RC_HIGH_OFF                 = 0,
    RC_HIGH_ON
}E_ClkSrcRcHigh;

typedef enum
{
    XTAL_HIGH_OFF               = 0,
    XTAL_HIGH_ON
}E_ClkSrcXtalHigh;

typedef enum
{
    FROM_RC_LOW_PWM             = 0,
    FROM_RC_LOW_RTC,
}E_ModuleFromRcLow_t;

typedef enum
{
    FROM_XTAL_LOW_RTC           = 0,
}E_ModuleFromXtalLow_t;

typedef enum
{
    FROM_RC_HIGH_APS_SYSCLK     = 0,         /* Selected */
    FROM_RC_HIGH_SCRT,                       /* Selected */
    FROM_RC_HIGH_SPI0,                       /* Selected */
    FROM_RC_HIGH_SPI1,                       /* Selected */
    FROM_RC_HIGH_SPI2,                       /* Selected */
    FROM_RC_HIGH_SPI3,                       /* Selected */
    FROM_RC_HIGH_EXT2IO,                     /* Selected */
    FROM_RC_HIGH_MSQ_SYSCLK,                 /* Selected */
}E_ModuleFromRcHigh_t;

/** Module from XTAL clock 
 *  Including APS_CLK_SYS_SRC_150_VAR and APS_CLK_SYS_SRC_DECI_160M_BB */
typedef enum
{
    FROM_XTAL_HIGH_APS_SYSCLK     = 0,      /* Selected */
    FROM_XTAL_HIGH_APS_SCRT,                /* Selected */
    FROM_XTAL_HIGH_APS_SPI0,                /* Selected */
    FROM_XTAL_HIGH_APS_SPI1,                /* Selected */
    FROM_XTAL_HIGH_APS_SPI2,                /* Selected */
    FROM_XTAL_HIGH_APS_SPI3,                /* Selected */
    FROM_XTAL_HIGH_APS_I2S,                 /* From XTAL */
    FROM_XTAL_HIGH_APS_UART0,               /* From XTAL */
    FROM_XTAL_HIGH_APS_UART1,               /* From XTAL */
    FROM_XTAL_HIGH_APS_DBG_UART,            /* From XTAL */
    FROM_XTAL_HIGH_APS_UART_ABR,            /* From XTAL */
    FROM_XTAL_HIGH_APS_PDM,                 /* From XTAL */
    FROM_XTAL_HIGH_APS_EXT2IO,              /* Selected */
    FROM_XTAL_HIGH_APS_ST,                  /* From XTAL */
    FROM_XTAL_HIGH_APS_TMR0,                /* From XTAL */
    FROM_XTAL_HIGH_APS_TMR1,                /* From XTAL */
    FROM_XTAL_HIGH_MSQ_SYSCLK,              /* Selected */
    FROM_XTAL_HIGH_MSQ_DBG_UART,            /* From XTAL */
    FROM_XTAL_HIGH_CAM_XVCLK,               /* From XTAL */
}E_ModuleFromXtalHigh_t;


typedef enum
{
    DET_XTAL_STS_SUCCESS = 0,               /* Detect high XTAL success */
    DET_XTAL_STS_FAIL_PRE_DET_TIMEOUT,      /* Pre-detection not done */
    DET_XTAL_STS_FAIL_NO_32K_XTAL,          /* Pre-detection timeout, no 32K XTAL */
    DET_XTAL_STS_FAIL_DET_TIMOEUT,          /* Pre-deteciton done. XTAL detect timeout */
    DET_XTAL_STS_FAIL_XTAL_DISTORTION,      /* Pre-detection done, with 32K XTAL. High XTAL distortion */
}E_DetectXtalStatus;



/* ******************************* System clock ********************************* */
/** APS system clock source selection */
typedef enum 
{
    APS_CLK_SYS_SRC_RC                  = 0, 
    APS_CLK_SYS_SRC_XTAL                = 1, 
    APS_CLK_SYS_SRC_XTAL_X2             = 2, 
    APS_CLK_SYS_SRC_150_VAR             = 3,
    APS_CLK_SYS_SRC_DECI_160M_BB        = 4,
    APS_CLK_SYS_SRC_EXTERNAL            = 5
} E_ApsClkSysSrc_t;

/** APS system clock divider */
typedef enum 
{
    APS_CLK_SYS_DIV_1                   = ((0 << AOS_APS_CLK_SEL_APS_CLK_DIV_SEL_Pos) | (0 << AOS_APS_CLK_SEL_APS_CLK_DIVN_UNGATED_Pos)),
    APS_CLK_SYS_DIV_2                   = ((1 << AOS_APS_CLK_SEL_APS_CLK_DIV_SEL_Pos) | (1 << AOS_APS_CLK_SEL_APS_CLK_DIVN_UNGATED_Pos)),
    APS_CLK_SYS_DIV_MSK                 = (AOS_APS_CLK_SEL_APS_CLK_DIV_SEL_Msk | AOS_APS_CLK_SEL_APS_CLK_DIVN_UNGATED_Msk),
} E_ApsClkSrcDivn_t;

/** APS PCLK divier */
typedef enum 
{
    APS_CLK_PCLK_DIV_1                  = ((0 << AOS_APS_CLK_SEL_APS_PCLK_DIV_SEL_Pos) | (0 << AOS_APS_CLK_SEL_APS_PCLK_DIVN_UNGATED_Pos)),
    APS_CLK_PCLK_DIV_2                  = ((1 << AOS_APS_CLK_SEL_APS_PCLK_DIV_SEL_Pos) | (1 << AOS_APS_CLK_SEL_APS_PCLK_DIVN_UNGATED_Pos)),
    APS_CLK_PCLK_DIV_MSK                = (AOS_APS_CLK_SEL_APS_PCLK_DIV_SEL_Msk | AOS_APS_CLK_SEL_APS_PCLK_DIVN_UNGATED_Msk),
} E_ApsClkPclkDivn_t;

/* APS clock group. For update clock frequency use */
typedef enum
{
    APS_CLK_GRP_XTAL                    = offsetof(S_APS_CLK_FREQ, u32XtalClk),
    APS_CLK_GRP_SYS                     = offsetof(S_APS_CLK_FREQ, u32SysClk),              /* The clock after MMD, and cannot be updated directly */
    APS_CLK_GRP_SYS_SRC                 = offsetof(S_APS_CLK_FREQ, u32SysSrcClk),
    APS_CLK_GRP_PCLK                    = offsetof(S_APS_CLK_FREQ, u32Pclk),
    APS_CLK_GRP_PWM                     = offsetof(S_APS_CLK_FREQ, u32PwmClk),
    APS_CLK_GRP_SCRT                    = offsetof(S_APS_CLK_FREQ, u32ScrtClk),
    APS_CLK_GRP_SPI0                    = offsetof(S_APS_CLK_FREQ, u32Spi0Clk),
    APS_CLK_GRP_SPI1                    = offsetof(S_APS_CLK_FREQ, u32Spi1Clk),
    APS_CLK_GRP_SPI2                    = offsetof(S_APS_CLK_FREQ, u32Spi2Clk),
    APS_CLK_GRP_SPI3                    = offsetof(S_APS_CLK_FREQ, u32Spi3Clk),
    APS_CLK_GRP_I2S                     = offsetof(S_APS_CLK_FREQ, u32I2sClk),
    APS_CLK_GRP_UART0                   = offsetof(S_APS_CLK_FREQ, u32Uart0Clk),
    APS_CLK_GRP_UART1                   = offsetof(S_APS_CLK_FREQ, u32Uart1Clk),
    APS_CLK_GRP_APS_UART                = offsetof(S_APS_CLK_FREQ, u32ApsUartClk),
    APS_CLK_GRP_UART_ABR                = offsetof(S_APS_CLK_FREQ, u32UartAbrClk),
    APS_CLK_GRP_PDM_IO                  = offsetof(S_APS_CLK_FREQ, u32PdmIoClk),
    APS_CLK_GRP_PDM                     = offsetof(S_APS_CLK_FREQ, u32PdmClk),
    APS_CLK_GRP_EXT_2IO                 = offsetof(S_APS_CLK_FREQ, u32Ext2IoClk),
    APS_CLK_GRP_TMR0_EXT                = offsetof(S_APS_CLK_FREQ, u32Tmr0ExtClk),
    APS_CLK_GRP_TMR1_EXT                = offsetof(S_APS_CLK_FREQ, u32Tmr1ExtClk),
    APS_CLK_GRP_ST                      = offsetof(S_APS_CLK_FREQ, u32StClk),
    APS_CLK_GRP_CAM_XVCLK               = offsetof(S_APS_CLK_FREQ, u32CamXvClk),
} E_ApsClkGrp_t;


/** Shelter source, RC or XTAL */
typedef enum
{
    CLKTREE_SHELTER_SRC_RC = 0,
    CLKTREE_SHELTER_SRC_XTAL,
} E_ClkTreeShelterSrc_t;


/* ******************************* Peripheral clock ***************************** */
/** PWM clock source selection */
typedef enum 
{
    APS_CLK_PWM_SRC_32k_CLK             = 0,        /* Same as RTC clock source. LPO(low frequency RC) or 32k XTAL */
    APS_CLK_PWM_SRC_XTAL_HIGH           = 1
} E_ApsClkPwmSrc_t;

/** APS security clock source selection */
typedef enum 
{
    ASP_CLK_SCRT_SRC_RC                 = 0, 
    ASP_CLK_SCRT_SRC_XTAL               = 1, 
    ASP_CLK_SCRT_SRC_XTAL_X2            = 2, 
    ASP_CLK_SCRT_SRC_D1_180M            = 3,
    ASP_CLK_SCRT_SRC_DECI_160M_BB       = 4,
    ASP_CLK_SCRT_SRC_EXTERNAL           = 5
} E_ApsClkScrtSrc_t;

/** APS security clock divier */
typedef enum 
{
    APS_CLK_SCRT_DIV_1                  = ((0 << AOS_SCRT_CLK_APS_SCRT_CLK_DIVN_SEL_Pos) | (0 << AOS_SCRT_CLK_APS_APSSCRT_CLK_DIVN_EN_Pos)),
    APS_CLK_SCRT_DIV_2                  = ((1 << AOS_SCRT_CLK_APS_SCRT_CLK_DIVN_SEL_Pos) | (1 << AOS_SCRT_CLK_APS_APSSCRT_CLK_DIVN_EN_Pos)),
    APS_CLK_SCRT_DIV_3                  = ((2 << AOS_SCRT_CLK_APS_SCRT_CLK_DIVN_SEL_Pos) | (1 << AOS_SCRT_CLK_APS_APSSCRT_CLK_DIVN_EN_Pos)),
    APS_CLK_SCRT_DIV_4                  = ((3 << AOS_SCRT_CLK_APS_SCRT_CLK_DIVN_SEL_Pos) | (1 << AOS_SCRT_CLK_APS_APSSCRT_CLK_DIVN_EN_Pos)),
    APS_CLK_SCRT_DIV_MSK                = (AOS_SCRT_CLK_APS_SCRT_CLK_DIVN_SEL_Msk | AOS_SCRT_CLK_APS_APSSCRT_CLK_DIVN_EN_Msk),
} E_ApsClkScrtDivn_t;


///** APS Systick divider. Systick source: XTAL */
//typedef enum
//{
//    APS_CLK_ST_DIV_1                    = (3 << AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos),
//    APS_CLK_ST_DIV_2                    = (2 << AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos),
//    APS_CLK_ST_DIV_4                    = (1 << AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos),
//    APS_CLK_ST_DIV_8                    = (0 << AOS_APS_STCLK_APS_STCLK_SRC_SEL_Pos),
//} E_ApsClkStDivn_t;

/* APS SPI0 clock source selection */
typedef enum
{
    APS_CLK_SPI0_SRC_RC                 = 0,
    APS_CLK_SPI0_SRC_XTAL               = 1,
    APS_CLK_SPI0_SRC_150M_VAR           = 2, /* Not recommand */
    APS_CLK_SPI0_SRC_DECI_160M_BB       = 3,
    APS_CLK_SPI0_SRC_D1_200M            = 4,
    APS_CLK_SPI0_SRC_EXTERNAL           = 5
}E_ApsClkSpi0Src_t;

/** APS SPI0 clock divider */
typedef enum
{
    APS_CLK_SPI0_DIV_1                  = (0 << AOS_SPI_CLK_SEL_SPI0_REFCLK_DIVN_Pos),
    APS_CLK_SPI0_DIV_2                  = (1 << AOS_SPI_CLK_SEL_SPI0_REFCLK_DIVN_Pos),
    APS_CLK_SPI0_DIV_3                  = (2 << AOS_SPI_CLK_SEL_SPI0_REFCLK_DIVN_Pos),
    APS_CLK_SPI0_DIV_4                  = (3 << AOS_SPI_CLK_SEL_SPI0_REFCLK_DIVN_Pos),
} E_ApsClkSpi0Divn_t;

/** APS SPI1 clock source selection */
typedef enum
{
    APS_CLK_SPI1_SRC_RC                 = 0,
    APS_CLK_SPI1_SRC_XTAL               = 1,
    APS_CLK_SPI1_SRC_150M_VAR           = 2, /* Not recommand */
    APS_CLK_SPI1_SRC_DECI_160M_BB       = 3,
    APS_CLK_SPI1_SRC_EXTERNAL           = 4
}E_ApsClkSpi1Src_t;

/** APS SPI1 clock divider */
typedef enum
{
    APS_CLK_SPI1_DIV_1                  = (0 << AOS_SPI_CLK_SEL_SPI1_SCLK_DIV_Pos),
    APS_CLK_SPI1_DIV_2                  = (1 << AOS_SPI_CLK_SEL_SPI1_SCLK_DIV_Pos),
    APS_CLK_SPI1_DIV_3                  = (2 << AOS_SPI_CLK_SEL_SPI1_SCLK_DIV_Pos),
    APS_CLK_SPI1_DIV_4                  = (3 << AOS_SPI_CLK_SEL_SPI1_SCLK_DIV_Pos),
} E_ApsClkSpi1Divn_t;

/** APS SPI2 clock source selection */
typedef enum
{
    APS_CLK_SPI2_SRC_RC                 = 0,
    APS_CLK_SPI2_SRC_XTAL               = 1,
    APS_CLK_SPI2_SRC_150M_VAR           = 2, /* Not recommand */
    APS_CLK_SPI2_SRC_DECI_160M_BB       = 3,
    APS_CLK_SPI2_SRC_EXTERNAL           = 4
}E_ApsClkSpi2Src_t;

/** APS SPI2 clock divider */
typedef enum
{
    APS_CLK_SPI2_DIV_1                  = (0 << AOS_SPI_CLK_SEL_SPI2_SCLK_DIV_Pos),
    APS_CLK_SPI2_DIV_2                  = (1 << AOS_SPI_CLK_SEL_SPI2_SCLK_DIV_Pos),
    APS_CLK_SPI2_DIV_3                  = (2 << AOS_SPI_CLK_SEL_SPI2_SCLK_DIV_Pos),
    APS_CLK_SPI2_DIV_4                  = (3 << AOS_SPI_CLK_SEL_SPI2_SCLK_DIV_Pos),
} E_ApsClkSpi2Divn_t;

/** APS SPI3 clock source selection */
typedef enum
{
    APS_CLK_SPI3_SRC_RC                 = 0,
    APS_CLK_SPI3_SRC_XTAL               = 1,
    APS_CLK_SPI3_SRC_150M_VAR           = 2, /* Not recommand */
    APS_CLK_SPI3_SRC_DECI_160M_BB       = 3,
    APS_CLK_SPI3_SRC_EXTERNAL           = 4
}E_ApsClkSpi3Src_t;

/** APS SPI3 clock divider */
typedef enum
{
    APS_CLK_SPI3_DIV_1                  = (0 << AOS_SPI_CLK_SEL_SPI3_SCLK_DIV_Pos),
    APS_CLK_SPI3_DIV_2                  = (1 << AOS_SPI_CLK_SEL_SPI3_SCLK_DIV_Pos),
    APS_CLK_SPI3_DIV_3                  = (2 << AOS_SPI_CLK_SEL_SPI3_SCLK_DIV_Pos),
    APS_CLK_SPI3_DIV_4                  = (3 << AOS_SPI_CLK_SEL_SPI3_SCLK_DIV_Pos),
} E_ApsClkSpi3Divn_t;

/** APS I2S clock source selection */
typedef enum
{
    APS_CLK_I2S_SRC_XTAL                = 0,
    APS_CLK_I2S_SRC_RC                  = 1,
    APS_CLK_I2S_SRC_DECI_160M_BB        = 2,
}E_ApsClkI2sSrc_t;

/** APS I2S clock divider */
typedef enum
{
    APS_CLK_I2S_DIV_1                   = ( 0 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_2                   = ( 1 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_3                   = ( 2 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_4                   = ( 3 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_5                   = ( 4 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_6                   = ( 5 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_7                   = ( 6 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_8                   = ( 7 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_9                   = ( 8 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_10                  = ( 9 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_11                  = (10 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_12                  = (11 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_13                  = (12 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_14                  = (13 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_15                  = (14 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_16                  = (15 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_17                  = (16 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_18                  = (17 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_19                  = (18 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_20                  = (19 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_21                  = (20 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_22                  = (21 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_23                  = (22 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_24                  = (23 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_25                  = (24 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_26                  = (25 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_27                  = (26 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_28                  = (27 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_29                  = (28 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_30                  = (29 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_31                  = (30 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_32                  = (31 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_33                  = (32 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_34                  = (33 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_35                  = (34 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_36                  = (35 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_37                  = (36 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_38                  = (37 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_39                  = (38 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_40                  = (39 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_41                  = (40 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_52                  = (51 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_53                  = (52 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_54                  = (53 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_55                  = (54 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_56                  = (55 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_57                  = (56 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_58                  = (57 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_59                  = (58 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_60                  = (59 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_61                  = (60 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_62                  = (61 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_63                  = (62 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
    APS_CLK_I2S_DIV_64                  = (63 << AOS_APS_DM_CLK_SEL_I2S_SCLK_SRC_DIV_Pos),
} E_ApsClkI2sDivn_t;



/** APS UART0 clock source selection */
typedef enum
{
    APS_CLK_UART0_SRC_XTAL              = 0,
    APS_CLK_UART0_SRC_XTAL_X2           = 1,
    APS_CLK_UART0_SRC_DECI_160M_BB      = 2,
}E_ApsClkUart0Src_t;


/** APS UART1 clock source selection */
typedef enum
{
    APS_CLK_UART1_SRC_XTAL              = 0,
    APS_CLK_UART1_SRC_XTAL_X2           = 1,
    APS_CLK_UART1_SRC_DECI_160M_BB      = 2,
}E_ApsClkUart1Src_t;


/** APS Debug UART clock source selection */
typedef enum
{
    APS_CLK_DBG_UART_SRC_XTAL           = 0,
    APS_CLK_DBG_UART_SRC_XTAL_X2        = 1,
    APS_CLK_DBG_UART_SRC_DECI_160M_BB   = 2,
}E_ApsClkDbgUartSrc_t;


/** APS UART ABR clock source selection */
typedef enum
{
    APS_CLK_UART_ABR_SRC_XTAL           = 0,
    APS_CLK_UART_ABR_SRC_XTAL_X2        = 1,
    APS_CLK_UART_ABR_SRC_DECI_160M_BB   = 2,
}E_ApsClkUartAbrSrc_t;



/** APS PDM clock source and divider selection */
typedef enum
{
    APS_CLK_PDM_SRC_DECI_160M_BB_DIV_1  = (0 << AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_SEL_Pos) | (0 << AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_DIV_Pos) | (1 << AOS_APS_DM_CLK_SEL_PDM_CLK_DIV_EN_Pos),
    APS_CLK_PDM_SRC_DECI_160M_BB_DIV_2  = (0 << AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_SEL_Pos) | (1 << AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_DIV_Pos) | (1 << AOS_APS_DM_CLK_SEL_PDM_CLK_DIV_EN_Pos),
    APS_CLK_PDM_SRC_DECI_160M_BB_DIV_3  = (0 << AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_SEL_Pos) | (2 << AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_DIV_Pos) | (1 << AOS_APS_DM_CLK_SEL_PDM_CLK_DIV_EN_Pos),
    APS_CLK_PDM_SRC_DECI_160M_BB_DIV_4  = (0 << AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_SEL_Pos) | (3 << AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_DIV_Pos) | (1 << AOS_APS_DM_CLK_SEL_PDM_CLK_DIV_EN_Pos),
    APS_CLK_PDM_SRC_XTAL                = (1 << AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_SEL_Pos),
    APS_CLK_PDM_SRC_MSK                 = AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_SEL_Msk | AOS_APS_DM_CLK_SEL_PDM_CLK_SRC_DIV_Msk | AOS_APS_DM_CLK_SEL_PDM_CLK_DIV_EN_Msk,
}E_ApsClkPdmSrc_t;


/** APS PDM IO clock source selection */
typedef enum
{
    APS_CLK_PDM_IO_SRC_DECI_160M_BB     = 0,
    APS_CLK_PDM_IO_SRC_XTAL             = 1,
}E_ApsClkPdmIoSrc_t;

/** APS PDM IO clock divider */
typedef enum
{
    APS_CLK_PDM_IO_DIV_1                = (  0 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_2                = (  1 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_3                = (  2 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_4                = (  3 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_5                = (  4 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_6                = (  5 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_7                = (  6 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_8                = (  7 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_9                = (  8 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_10               = (  9 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_11               = ( 10 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_12               = ( 11 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_13               = ( 12 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_14               = ( 13 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_15               = ( 14 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_16               = ( 15 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_17               = ( 16 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_18               = ( 17 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_19               = ( 18 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_20               = ( 19 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_21               = ( 20 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_22               = ( 21 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_23               = ( 22 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_24               = ( 23 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_25               = ( 24 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_26               = ( 25 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_27               = ( 26 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_28               = ( 27 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_29               = ( 28 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_30               = ( 29 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_31               = ( 30 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_32               = ( 31 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_33               = ( 32 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_34               = ( 33 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_35               = ( 34 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_36               = ( 35 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_37               = ( 36 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_38               = ( 37 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_39               = ( 38 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_40               = ( 39 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_41               = ( 40 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_42               = ( 41 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_43               = ( 42 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_44               = ( 43 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_45               = ( 44 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_46               = ( 45 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_47               = ( 46 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_48               = ( 47 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_49               = ( 48 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_50               = ( 49 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_51               = ( 50 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_52               = ( 51 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_53               = ( 52 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_54               = ( 53 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_55               = ( 54 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_56               = ( 55 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_57               = ( 56 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_58               = ( 57 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_59               = ( 58 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_60               = ( 59 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_61               = ( 60 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_62               = ( 61 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_63               = ( 62 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_64               = ( 63 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_65               = ( 64 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_66               = ( 65 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_67               = ( 66 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_68               = ( 67 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_69               = ( 68 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_70               = ( 69 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_71               = ( 70 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_72               = ( 71 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_73               = ( 72 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_74               = ( 73 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_75               = ( 74 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_76               = ( 75 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_77               = ( 76 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_78               = ( 77 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_79               = ( 78 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_80               = ( 79 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_81               = ( 80 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_82               = ( 81 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_83               = ( 82 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_84               = ( 83 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_85               = ( 84 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_86               = ( 85 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_87               = ( 86 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_88               = ( 87 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_89               = ( 88 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_90               = ( 89 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_91               = ( 90 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_92               = ( 91 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_93               = ( 92 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_94               = ( 93 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_95               = ( 94 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_96               = ( 95 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_97               = ( 96 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_98               = ( 97 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_99               = ( 98 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_100              = ( 99 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_101              = (100 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_102              = (101 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_103              = (102 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_104              = (103 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_105              = (104 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_106              = (105 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_107              = (106 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_108              = (107 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_109              = (108 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_110              = (109 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_111              = (110 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_112              = (111 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_113              = (112 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_114              = (113 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_115              = (114 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_116              = (115 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_117              = (116 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_118              = (117 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_119              = (118 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_120              = (119 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_121              = (120 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_122              = (121 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_123              = (122 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_124              = (123 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_125              = (124 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_126              = (125 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_127              = (126 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_128              = (127 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_129              = (128 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_130              = (129 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_131              = (130 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_132              = (131 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_133              = (132 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_134              = (133 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_135              = (134 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_136              = (135 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_137              = (136 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_138              = (137 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_139              = (138 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_140              = (139 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_141              = (140 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_142              = (141 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_143              = (142 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_144              = (143 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_145              = (144 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_146              = (145 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_147              = (146 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_148              = (147 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_149              = (148 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_150              = (149 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_151              = (150 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_152              = (151 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_153              = (152 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_154              = (153 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_155              = (154 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_156              = (155 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_157              = (156 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_158              = (157 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_159              = (158 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_160              = (159 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_161              = (160 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_162              = (161 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_163              = (162 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_164              = (163 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_165              = (164 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_166              = (165 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_167              = (166 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_168              = (167 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_169              = (168 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_170              = (169 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_171              = (170 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_172              = (171 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_173              = (172 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_174              = (173 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_175              = (174 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_176              = (175 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_177              = (176 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_178              = (177 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_179              = (178 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_180              = (179 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_181              = (180 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_182              = (181 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_183              = (182 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_184              = (183 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_185              = (184 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_186              = (185 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_187              = (186 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_188              = (187 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_189              = (188 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_190              = (189 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_191              = (190 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_192              = (191 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_193              = (192 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_194              = (193 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_195              = (194 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_196              = (195 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_197              = (196 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_198              = (197 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_199              = (198 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_200              = (199 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_201              = (200 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_202              = (201 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_203              = (202 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_204              = (203 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_205              = (204 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_206              = (205 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_207              = (206 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_208              = (207 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_209              = (208 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_210              = (209 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_211              = (210 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_212              = (211 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_213              = (212 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_214              = (213 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_215              = (214 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_216              = (215 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_217              = (216 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_218              = (217 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_219              = (218 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_220              = (219 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_221              = (220 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_222              = (221 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_223              = (222 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_224              = (223 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_225              = (224 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_226              = (225 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_227              = (226 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_228              = (227 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_229              = (228 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_230              = (229 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_231              = (230 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_232              = (231 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_233              = (232 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_234              = (233 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_235              = (234 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_236              = (235 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_237              = (236 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_238              = (237 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_239              = (238 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_240              = (239 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_241              = (240 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_242              = (241 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_243              = (242 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_244              = (243 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_245              = (244 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_246              = (245 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_247              = (246 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_248              = (247 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_249              = (248 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_250              = (249 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_251              = (250 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_252              = (251 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_253              = (252 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_254              = (253 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_255              = (254 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
    APS_CLK_PDM_IO_DIV_256              = (255 << AOS_APS_DM_CLK_SEL_PDM_IO_CLK_SRC_DIV_Pos),
} E_ApsClkPdmIoDivn_t;

/** APS EXT to IO Clock source selection */
typedef enum
{
    APS_CLK_EXT_2_IO_SRC_XTAL           = 0,
    APS_CLK_EXT_2_IO_SRC_RC             = 1,
}E_ApsClkExt2IoSrc_t;

/** APS EXT to IO clock divider */
typedef enum
{
    APS_CLK_EXT_2_IO_DIV_1              = (0 << AOS_APS_DOMAIN_EN_EXT_2IO_CLK_DIV_Pos),
    APS_CLK_EXT_2_IO_DIV_2              = (1 << AOS_APS_DOMAIN_EN_EXT_2IO_CLK_DIV_Pos),
    APS_CLK_EXT_2_IO_DIV_4              = (2 << AOS_APS_DOMAIN_EN_EXT_2IO_CLK_DIV_Pos),
    APS_CLK_EXT_2_IO_DIV_8              = (3 << AOS_APS_DOMAIN_EN_EXT_2IO_CLK_DIV_Pos),
} E_ApsClkExt2IoDivn_t;

/** APS EXT to IO clock enable source */
typedef enum
{
    APS_CLK_EXT_2_IO_EN_SW_ON           = (int32_t)((0 << AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN_HW_Pos) | (1UL << AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN_Pos)),
    APS_CLK_EXT_2_IO_EN_SW_OFF          = (int32_t)((0 << AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN_HW_Pos) | (0 << AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN_Pos)),
    APS_CLK_EXT_2_IO_EN_PAD             = (int32_t)((1 << AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN_HW_Pos)),
    APS_CLK_EXT_2_IO_EN_MSK             = (int32_t)(AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN_HW_Msk | AOS_APS_DOMAIN_EN_EXT_2IO_CLK_EN_Msk),
}E_ApsClkExt2IoEn_t;


/** CAM XVCLK clock source selection */
typedef enum
{
    APS_CLK_CAM_XVCLK_SRC_XTAL          = 0,
    APS_CLK_CAM_XVCLK_SRC_XTAL_X2       = 1,
    APS_CLK_CAM_XVCLK_SRC_150M_VAR      = 2, /* Not recommand */
    APS_CLK_CAM_XVCLK_SRC_DECI_160M_BB  = 3,
    APS_CLK_CAM_XVCLK_SRC_EXTERNAL      = 4,    
    APS_CLK_CAM_XVCLK_SRC_MSK           = AOS_CAM_XVCLK_SEL_CAM_XVCLKSEL_Msk,
}E_ApsClkCamXvClkSrc_t;

/** APS XVCLK clock divider */
typedef enum
{
    APS_CLK_CAM_XVCLK_DIV_1             = (0 << AOS_CAM_XVCLK_SEL_CAM_XVCLK_DIVN_Pos),
    APS_CLK_CAM_XVCLK_DIV_2             = (1 << AOS_CAM_XVCLK_SEL_CAM_XVCLK_DIVN_Pos),
    APS_CLK_CAM_XVCLK_DIV_3             = (2 << AOS_CAM_XVCLK_SEL_CAM_XVCLK_DIVN_Pos),
    APS_CLK_CAM_XVCLK_DIV_4             = (3 << AOS_CAM_XVCLK_SEL_CAM_XVCLK_DIVN_Pos),
    APS_CLK_CAM_XVCLK_DIV_MSK           = AOS_CAM_XVCLK_SEL_CAM_XVCLK_DIVN_Msk,
} E_ApsClkCamXvCLKDivn_t;

/** MSQ system clock source selection */
typedef enum
{
    MSQ_CLK_SYS_SRC_RC                  = 0,
    MSQ_CLK_SYS_SRC_XTAL                = 1, 
    MSQ_CLK_SYS_SRC_XTAL_X2             = 2,
    MSQ_CLK_SYS_SRC_DECI_160M           = 3,
    MSQ_CLK_SYS_SRC_EXTERNAL            = 7,
    MSQ_CLK_SYS_SRC_MAX,
} E_MsqClkSysSrc_t;  


/** RTC clock selection */
typedef enum 
{
    CLK_LPO_CTRL_BY_SEQ         = (0 << AOS_OSC_CTL_LPO_ON_Pos) | (1 << AOS_OSC_CTL_32K_LPO_EN_Pos), /* LPO as default value */
    CLK_LPO_CTRL_OFF            = (1 << AOS_OSC_CTL_LPO_ON_Pos) | (0 << AOS_OSC_CTL_32K_LPO_EN_Pos),
    CLK_LPO_CTRL_ON             = (1 << AOS_OSC_CTL_LPO_ON_Pos) | (1 << AOS_OSC_CTL_32K_LPO_EN_Pos),
    CLK_LPO_CTRL_MSK            = (AOS_OSC_CTL_LPO_ON_Msk | AOS_OSC_CTL_32K_LPO_EN_Msk),
}E_ClkSrcLpo_t;

typedef enum 
{
    XTAL_32k_BY_SEQ             = (0 << AOS_OSC_CTL_XTAL_ON_Pos),
    XTAL_32k_OFF                = (1 << AOS_OSC_CTL_XTAL_ON_Pos) | (0 << AOS_OSC_CTL_32K_XTAL_EN_Pos),
    XTAL_32k_ON                 = (1 << AOS_OSC_CTL_XTAL_ON_Pos) | (1 << AOS_OSC_CTL_32K_XTAL_EN_Pos) | (0 << AOS_OSC_CTL_32K_XTAL_BYPASS_Pos),
    XTAL_32k_BYPS_ON            = (1 << AOS_OSC_CTL_XTAL_ON_Pos) | (1 << AOS_OSC_CTL_32K_XTAL_EN_Pos) | (1 << AOS_OSC_CTL_32K_XTAL_BYPASS_Pos),
    XTAL_32k_MSK                = (AOS_OSC_CTL_XTAL_ON_Msk | AOS_OSC_CTL_32K_XTAL_EN_Msk | AOS_OSC_CTL_32K_XTAL_BYPASS_Msk),
}E_ClkSrcXtal32k_t;

/** RTC clock selection */
typedef enum
{
    MSQ_CLK_RTC_OFF             = (0 << AOS_OSC_SEL_RTC_EN_Pos),
    MSQ_CLK_RTC_ON_LPO          = (1 << AOS_OSC_SEL_RTC_EN_Pos) | (MSQ_RTC_CLK_SRC_LPO         << AOS_OSC_SEL_32K_OSC_SEL_Pos),
    MSQ_CLK_RTC_ON_32K_XTAL     = (1 << AOS_OSC_SEL_RTC_EN_Pos) | (MSQ_RTC_CLK_SRC_32K_XTAL    << AOS_OSC_SEL_32K_OSC_SEL_Pos),
    MSQ_CLK_RTC_MSK             = AOS_OSC_SEL_RTC_EN_Msk | AOS_OSC_SEL_32K_OSC_SEL_Msk,
}E_MsqClkRtc_t;

typedef struct
{
    E_MsqClkRtc_t       eRtcSrc;        /* On/Off RTC and RTC source from LPO or 32k XTAL */
    E_ClkSrcLpo_t       eLpoCtrl;       /* On/Off LPO */
    E_ClkSrcXtal32k_t   eXtal32kCtrl;   /* On/Off 32k XTAL or bypass */
    uint8_t             u8XtalGm;       /* Set GM for 32k XTAL */
    uint8_t             u8XtalCap;      /* Set CAP for 32k XTAL */
} S_RTC_CLK_CFG;


/*
 *************************************************************************
 *						    Typedefs
 *************************************************************************
 */

/* Setup function */
typedef void (*T_Hal_Sys_ClkInit)(uint32_t u32XtalFreq);
typedef uint32_t (*T_Hal_Sys_RegBitDefSetup)(E_FLAG_STS eFlag, uint32_t u32Def);
    
/* Module reset */
typedef uint32_t (*T_Hal_Sys_SwResetAll)(void);
typedef uint32_t (*T_Hal_Sys_ApsModuleRst)(E_ApsRstModule_t eModule);
typedef uint32_t (*T_Hal_Sys_MsqModuleRst)(E_MsqRstModule_t eModule);

/* Module enable */
typedef uint32_t (*T_Hal_Sys_ApsModuleClkEn)(uint8_t u8Enable, E_ApsClkEnModule_t eModule);
typedef uint32_t (*T_Hal_Sys_MsqModuleClkEn)(uint8_t u8Enable, E_MsqClkEnModule_t eModule);
typedef void (*T_Hal_Sys_UnusedModuleDis)(void);

/* Source clock */
typedef uint32_t (*T_Hal_Sys_RcHighEn)(E_ClkSrcRcHigh eEn);
typedef uint32_t (*T_Hal_Sys_XtalHighEn)(E_ClkSrcXtalHigh eEn);
typedef uint32_t (*T_Hal_Sys_FromRcHighCheck)(void);
typedef uint32_t (*T_Hal_Sys_FromXtalHighCheck)(void);
typedef uint32_t (*T_Hal_Sys_RcCal)(void);
typedef int32_t (*T_Hal_Sys_RcCalCompare)(uint32_t u32Ctune, uint32_t u32Cen, uint32_t u32WaitXtalTicks);
typedef void (*T_Hal_Sys_CheckXtalReady)(void);

/* System Clock */
typedef void (*T_Hal_Sys_SleepClkSetup)(void);
typedef void (*T_Hal_Sys_WakeupClkResume)(void);
typedef uint32_t (*T_Hal_Sys_ApsClkTreeSetup)(E_ApsClkSysSrc_t eClkTreeSrc, E_ApsClkSrcDivn_t eClkTreeSrcDiv, E_ApsClkPclkDivn_t eClkTreePclkDiv);
typedef void (*T_Hal_Sys_ClkChangeApply)(void);
typedef uint32_t (*T_Hal_Sys_ApsClockUpdate)(E_ApsClkGrp_t eApsClkGrp, uint32_t u32ClkFreq);
typedef uint32_t (*T_Hal_Sys_ApsClockGet)(E_ApsClkGrp_t eApsClkGrp, uint32_t *pu32ClkFreq);
typedef void (*T_Hal_Sys_SwitchShelterSet)(E_ClkTreeShelterSrc_t eSrc);


/* Peripheral clock */
typedef uint32_t (*T_Hal_Sys_PwmSrcSelect)(E_ApsClkPwmSrc_t ePwmSrc);
typedef uint32_t (*T_Hal_Sys_ScrtSrcSelect)(E_ApsClkScrtSrc_t eSrc, E_ApsClkScrtDivn_t eDiv);
//typedef uint32_t (*T_Hal_Sys_StDivSelect)(E_ApsClkStDivn_t eDiv); /* Not supported */
typedef uint32_t (*T_Hal_Sys_Spi0SrcSelect)(E_ApsClkSpi0Src_t eSrc, E_ApsClkSpi0Divn_t eDiv);
typedef uint32_t (*T_Hal_Sys_Spi1SrcSelect)(E_ApsClkSpi1Src_t eSrc, E_ApsClkSpi1Divn_t eDiv);
typedef uint32_t (*T_Hal_Sys_Spi2SrcSelect)(E_ApsClkSpi2Src_t eSrc, E_ApsClkSpi2Divn_t eDiv);
typedef uint32_t (*T_Hal_Sys_Spi3SrcSelect)(E_ApsClkSpi3Src_t eSrc, E_ApsClkSpi3Divn_t eDiv);
typedef uint32_t (*T_Hal_Sys_I2sSrcSelect)(E_ApsClkI2sSrc_t eSrc, E_ApsClkI2sDivn_t eDiv);
typedef uint32_t (*T_Hal_Sys_Uart0SrcSelect)(E_ApsClkUart0Src_t eSrc);
typedef uint32_t (*T_Hal_Sys_Uart1SrcSelect)(E_ApsClkUart1Src_t eSrc);
typedef uint32_t (*T_Hal_Sys_ApsUartSrcSelect)(E_ApsClkDbgUartSrc_t eSrc);
typedef uint32_t (*T_Hal_Sys_UartAbrSrcSelect)(E_ApsClkUartAbrSrc_t eSrc);
typedef uint32_t (*T_Hal_Sys_PdmSrcSelect)(E_ApsClkPdmSrc_t ePdmSrc, E_ApsClkPdmIoSrc_t ePdmIoSrc, E_ApsClkPdmIoDivn_t ePdmIoDiv);
typedef uint32_t (*T_Hal_Sys_Ext2IoSetup)(E_ApsClkExt2IoSrc_t eSrc, E_ApsClkExt2IoDivn_t eDiv, E_ApsClkExt2IoEn_t eEn);
typedef uint32_t (*T_Hal_Sys_CamXvClkSrcSelect)(E_ApsClkCamXvClkSrc_t eSrc, E_ApsClkCamXvCLKDivn_t eDiv);
typedef uint32_t (*T_Hal_Sys_RtcClkCfg)(S_RTC_CLK_CFG *psCfg);

/* Clock modulator */
typedef uint32_t (*T_Hal_Sys_ApsClkMmFactorSet)(uint32_t u32Factor);
typedef uint32_t (*T_Hal_Sys_ApsClkMmFactorGet)(void);



/*
 *************************************************************************
 *							Public Variables
 *************************************************************************
 */

/* Setup function */
extern T_Hal_Sys_ClkInit            Hal_Sys_ClkInit;
extern T_Hal_Sys_RegBitDefSetup     Hal_Sys_RegBitDefSetup;
    
/* Module reset */
extern T_Hal_Sys_SwResetAll         Hal_Sys_SwResetAll;
extern T_Hal_Sys_ApsModuleRst       Hal_Sys_ApsModuleRst;
extern T_Hal_Sys_MsqModuleRst       Hal_Sys_MsqModuleRst;

/* Module enable */
extern T_Hal_Sys_ApsModuleClkEn     Hal_Sys_ApsModuleClkEn;
extern T_Hal_Sys_MsqModuleClkEn     Hal_Sys_MsqModuleClkEn;
extern T_Hal_Sys_UnusedModuleDis    Hal_Sys_UnusedModuleDis;

/* Source clock */
extern T_Hal_Sys_RcHighEn           Hal_Sys_RcHighEn;
extern T_Hal_Sys_XtalHighEn         Hal_Sys_XtalHighEn;
extern T_Hal_Sys_FromRcHighCheck    Hal_Sys_FromRcHighCheck;
extern T_Hal_Sys_FromXtalHighCheck  Hal_Sys_FromXtalHighCheck;
extern T_Hal_Sys_RcCal              Hal_Sys_RcCal;
extern T_Hal_Sys_RcCalCompare       Hal_Sys_RcCalCompare;
extern T_Hal_Sys_CheckXtalReady     Hal_Sys_CheckXtalReady;

/* System Clock */
extern T_Hal_Sys_SleepClkSetup      Hal_Sys_SleepClkSetup;
extern T_Hal_Sys_WakeupClkResume    Hal_Sys_WakeupClkResume;
extern T_Hal_Sys_ApsClkTreeSetup    Hal_Sys_ApsClkTreeSetup;
extern T_Hal_Sys_ClkChangeApply     Hal_Sys_ApsClkChangeApply;
extern T_Hal_Sys_ApsClockUpdate     Hal_Sys_ApsClockUpdate;
extern T_Hal_Sys_ApsClockGet        Hal_Sys_ApsClockGet;
extern T_Hal_Sys_SwitchShelterSet   Hal_Sys_SwitchShelterSet;
    
/* Peripheral clock */
extern T_Hal_Sys_PwmSrcSelect       Hal_Sys_PwmSrcSelect;
extern T_Hal_Sys_ScrtSrcSelect      Hal_Sys_ScrtSrcSelect;
//extern T_Hal_Sys_StDivSelect        Hal_Sys_StDivSelect;
extern T_Hal_Sys_Spi0SrcSelect      Hal_Sys_Spi0SrcSelect;
extern T_Hal_Sys_Spi1SrcSelect      Hal_Sys_Spi1SrcSelect;
extern T_Hal_Sys_Spi2SrcSelect      Hal_Sys_Spi2SrcSelect;
extern T_Hal_Sys_Spi3SrcSelect      Hal_Sys_Spi3SrcSelect;
extern T_Hal_Sys_I2sSrcSelect       Hal_Sys_I2sSrcSelect;
extern T_Hal_Sys_Uart0SrcSelect     Hal_Sys_Uart0SrcSelect;
extern T_Hal_Sys_Uart1SrcSelect     Hal_Sys_Uart1SrcSelect;
extern T_Hal_Sys_ApsUartSrcSelect   Hal_Sys_ApsUartSrcSelect;
extern T_Hal_Sys_UartAbrSrcSelect   Hal_Sys_UartAbrSrcSelect;
extern T_Hal_Sys_PdmSrcSelect       Hal_Sys_PdmSrcSelect;
extern T_Hal_Sys_Ext2IoSetup        Hal_Sys_Ext2IoSetup;
extern T_Hal_Sys_CamXvClkSrcSelect  Hal_Sys_CamXvClkSrcSelect;
extern T_Hal_Sys_RtcClkCfg          Hal_Sys_RtcClkCfg;

/* Clock modulator */
extern T_Hal_Sys_ApsClkMmFactorSet  Hal_Sys_ApsClkMmFactorSet;
extern T_Hal_Sys_ApsClkMmFactorGet  Hal_Sys_ApsClkMmFactorGet;

/*
 *************************************************************************
 *							Public Functions
 *************************************************************************
 */

/* Setup function */
void Hal_Sys_ClkInit_impl(uint32_t u32XtalFreq);
uint32_t Hal_Sys_RegBitDefSetup_impl(E_FLAG_STS eFlag, uint32_t u32Def);

/* Module reset */
uint32_t Hal_Sys_SwResetAll_impl(void);
uint32_t Hal_Sys_ApsModuleRst_impl(E_ApsRstModule_t eModule);
uint32_t Hal_Sys_MsqModuleRst_impl(E_MsqRstModule_t eModule);

/* Module enable */
uint32_t Hal_Sys_ApsModuleClkEn_impl(uint8_t u8Enable, E_ApsClkEnModule_t eModule);
uint32_t Hal_Sys_MsqModuleClkEn_impl(uint8_t u8Enable, E_MsqClkEnModule_t eModule);
void Hal_Sys_UnusedModuleDis_impl(void);

/* Source clock */
uint32_t Hal_Sys_RcHighEn_impl(E_ClkSrcRcHigh eEn);
uint32_t Hal_Sys_XtalHighEn_impl(E_ClkSrcXtalHigh eEn);
uint32_t Hal_Sys_FromRcHighCheck_impl(void);
uint32_t Hal_Sys_FromXtalHighCheck_impl(void);
uint32_t Hal_Sys_RcCal_impl(void);
int32_t Hal_Sys_RcCalCompare_impl(uint32_t u32Ctune, uint32_t u32Cen, uint32_t u32WaitXtalTicks);
void Hal_Sys_CheckXtalReady_impl(void);

void Hal_Sys_32kXtalSetup(void);
void Hal_Sys_DetXtalStart(void);
E_DetectXtalStatus Hal_Sys_DetXtalEnd(uint32_t *pu32XtalFreq);

/* System Clock */
void Hal_Sys_SleepClkSetup_impl(void);
void Hal_Sys_WakeupClkResume_impl(void);
uint32_t Hal_Sys_ApsClkTreeSetup_impl(E_ApsClkSysSrc_t eSrc, E_ApsClkSrcDivn_t eSysDiv, E_ApsClkPclkDivn_t ePclkDiv);
void Hal_Sys_ApsClkChangeApply_impl(void);
uint32_t Hal_Sys_ApsClockUpdate_impl(E_ApsClkGrp_t eApsClkGrp, uint32_t u32ClkFreq);
uint32_t Hal_Sys_ApsClockGet_impl(E_ApsClkGrp_t eApsClkGrp, uint32_t *pu32ClkFreq);
void Hal_Sys_SwitchShelterSet_impl(E_ClkTreeShelterSrc_t eSrc);

/* Peripheral clock */
uint32_t Hal_Sys_PwmSrcSelect_impl(E_ApsClkPwmSrc_t eSrc);
uint32_t Hal_Sys_ScrtSrcSelect_impl(E_ApsClkScrtSrc_t eSrc, E_ApsClkScrtDivn_t eDiv);
//uint32_t Hal_Sys_StDivSelect_impl(E_ApsClkStDivn_t eDiv);
uint32_t Hal_Sys_Spi0SrcSelect_impl(E_ApsClkSpi0Src_t eSrc, E_ApsClkSpi0Divn_t eDiv);
uint32_t Hal_Sys_Spi1SrcSelect_impl(E_ApsClkSpi1Src_t eSrc, E_ApsClkSpi1Divn_t eDiv);
uint32_t Hal_Sys_Spi2SrcSelect_impl(E_ApsClkSpi2Src_t eSrc, E_ApsClkSpi2Divn_t eDiv);
uint32_t Hal_Sys_Spi3SrcSelect_impl(E_ApsClkSpi3Src_t eSrc, E_ApsClkSpi3Divn_t eDiv);
uint32_t Hal_Sys_I2sSrcSelect_impl(E_ApsClkI2sSrc_t eSrc, E_ApsClkI2sDivn_t eDiv);
uint32_t Hal_Sys_Uart0SrcSelect_impl(E_ApsClkUart0Src_t eSrc);
uint32_t Hal_Sys_Uart1SrcSelect_impl(E_ApsClkUart1Src_t eSrc);
uint32_t Hal_Sys_ApsUartSrcSelect_impl(E_ApsClkDbgUartSrc_t eSrc);
uint32_t Hal_Sys_UartAbrSrcSelect_impl(E_ApsClkUartAbrSrc_t eSrc);
uint32_t Hal_Sys_PdmSrcSelect_impl(E_ApsClkPdmSrc_t ePdmSrc, E_ApsClkPdmIoSrc_t ePdmIoSrc, E_ApsClkPdmIoDivn_t ePdmIoDiv);
uint32_t Hal_Sys_Ext2IoSetup_impl(E_ApsClkExt2IoSrc_t eSrc, E_ApsClkExt2IoDivn_t eDiv, E_ApsClkExt2IoEn_t eEn);
uint32_t Hal_Sys_CamXvClkSrcSelect_impl(E_ApsClkCamXvClkSrc_t eSrc, E_ApsClkCamXvCLKDivn_t eDiv);
uint32_t Hal_Sys_RtcClkCfg_impl(S_RTC_CLK_CFG *psCfg);

/* APS clock modulator */
uint32_t Hal_Sys_ApsClkMmFactorSet_impl(uint32_t u32Factor);
uint32_t Hal_Sys_ApsClkMmFactorGet_impl(void);


void Hal_RCC_PreInit(void);

/* bootROM stage used, no patchable */ 
uint32_t Hal_Sys_ResetStatusRead(void);
uint32_t Hal_Sys_ResetStatusClear(uint32_t u32Sources);

#include "hal_sys_rcc_patch.h"
#endif  /* __HAL_SYS_RCC_H__ */
