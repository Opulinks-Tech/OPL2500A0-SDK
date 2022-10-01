/* *****************************************************************************
 *  Copyright 2017 - 2021, Opulinks Technology Ltd.
 *  ----------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 ******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  hal_pin_def.h
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the patch proto-types of system functions
*  Include sys-reg and AOS domains.
*
*  Author:
*  -------
*  Jeff
******************************************************************************/

#ifndef __HAL_PIN_DEF_H__
#define __HAL_PIN_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

#define PIN_REG_GRP_OFST_REG        8
#define PIN_REG_GRP_OFST_BIT        4
    
#define PIN_REG_GRP_OFST_TO_REG(ofst)           ((ofst)/PIN_REG_GRP_OFST_REG + PIN_BASE)
#define PIN_REG_GRP_OFST_TO_BIT(ofst)           (((ofst)%PIN_REG_GRP_OFST_REG) * PIN_REG_GRP_OFST_BIT)
#define PIN_REG_GRP_BITS_MASK                   0xFU


/* ************************** PIN CFG bit definition ***************************************** */

/* PIN CFG bit definition */
#define PIN_IO_TYPE_BIT                                 0
#define PIN_IS_SIP_IO_BIT                               2
#define PIN_IO_NUM_BIT                                  3
#define PIN_IN_ENABLE_BIT                               9
#define PIN_INGRP_OFST_BIT                              10
#define PIN_INGRP_VAL_BIT                               21
#define PIN_IN_INV_BIT                                  25
#define PIN_IO_PE_BIT                                   26
#define PIN_OUT_ENABLE_BIT                              27
#define PIN_OUTGRP_OFST_BIT                             28
#define PIN_OUTGRP_VAL_BIT                              39
#define PIN_OUT_INV_BIT                                 43
#define PIN_OEN_INV_BIT                                 44
#define PIN_OUT_LEVEL_BIT                               45
#define PIN_OUT_IO_DRV_BIT                              46
#define PIN_AUTO_CTRL_EN_BIT                            47
#define PIN_AUTO_CTRL_LVL_BIT                           48
#define PIN_OUT_DRV_CURR_BIT                            46
#define PIN_INPUT_MODE_BIT                              26

/* S_GPIO_PIN_CFG [1:0] IoType*/
#define PIN_IO_TYPE_PERIPH                              0ULL  /* Configure as peripheral IO */
#define PIN_IO_TYPE_GPIO_IN                             1ULL  /* Configure as GPIO input    */
#define PIN_IO_TYPE_GPIO_OUT                            2ULL  /* Configure as GPIO output   */
#define PIN_IO_TYPE_DISABLE                             3ULL  /* Disable this IO pin        */
#define PIN_IO_TYPE_PERIPH_BITSVAL                      (PIN_IO_TYPE_PERIPH   << PIN_IO_TYPE_BIT)
#define PIN_IO_TYPE_GPIO_IN_BITSVAL                     (PIN_IO_TYPE_GPIO_IN  << PIN_IO_TYPE_BIT)
#define PIN_IO_TYPE_GPIO_OUT_BITSVAL                    (PIN_IO_TYPE_GPIO_OUT << PIN_IO_TYPE_BIT)
#define PIN_IO_TYPE_DISABLE_BITSVAL                     (PIN_IO_TYPE_DISABLE  << PIN_IO_TYPE_BIT)

/* S_GPIO_PIN_CFG [2] IsSipIo*/
#define PIN_IS_SIP_IO_EXTERNAL                          0ULL  /* IO Bond to external for package 64 pin    */
#define PIN_IS_SIP_IO_INTERNAL                          1ULL  /* Not bond to external, used for SiP PSRAM  */
#define PIN_IS_SIP_IO_EXTERNAL_BITSVAL                  (PIN_IS_SIP_IO_EXTERNAL << PIN_IS_SIP_IO_BIT)
#define PIN_IS_SIP_IO_INTERNAL_BITSVAL                  (PIN_IS_SIP_IO_INTERNAL << PIN_IS_SIP_IO_BIT)

/* S_GPIO_PIN_CFG [2] IsSipIo.  [8:3] IoNum. */
#define PIN_IO_NUM_IO0                                  0ULL      /* Offset[0],  Register 0, Bit 0  */
#define PIN_IO_NUM_IO1                                  1ULL      /* Offset[1],  Register 0, Bit 1  */
#define PIN_IO_NUM_IO2                                  2ULL      /* Offset[2],  Register 0, Bit 2  */
#define PIN_IO_NUM_IO3                                  3ULL      /* Offset[3],  Register 0, Bit 3  */
#define PIN_IO_NUM_IO4                                  4ULL      /* Offset[4],  Register 0, Bit 4  */
#define PIN_IO_NUM_IO5                                  5ULL      /* Offset[5],  Register 0, Bit 5  */
#define PIN_IO_NUM_IO6                                  6ULL      /* Offset[6],  Register 0, Bit 6  */
#define PIN_IO_NUM_IO7                                  7ULL      /* Offset[7],  Register 0, Bit 7  */
#define PIN_IO_NUM_IO8                                  8ULL      /* Offset[8],  Register 0, Bit 8  */
#define PIN_IO_NUM_IO9                                  9ULL      /* Offset[9],  Register 0, Bit 9  */
#define PIN_IO_NUM_IO10                                 10ULL     /* Offset[10], Register 0, Bit 10 */
#define PIN_IO_NUM_IO11                                 11ULL     /* Offset[11], Register 0, Bit 11 */
#define PIN_IO_NUM_IO12                                 12ULL     /* Offset[12], Register 0, Bit 12 */
#define PIN_IO_NUM_IO13                                 13ULL     /* Offset[13], Register 0, Bit 13 */
#define PIN_IO_NUM_IO14                                 14ULL     /* Offset[14], Register 0, Bit 14 */
#define PIN_IO_NUM_IO15                                 15ULL     /* Offset[15], Register 0, Bit 15 */
#define PIN_IO_NUM_IO16                                 16ULL     /* Offset[16], Register 0, Bit 16 */
#define PIN_IO_NUM_IO17                                 17ULL     /* Offset[17], Register 0, Bit 17 */
#define PIN_IO_NUM_IO18                                 18ULL     /* Offset[18], Register 0, Bit 18 */
#define PIN_IO_NUM_IO19                                 19ULL     /* Offset[19], Register 0, Bit 19 */
#define PIN_IO_NUM_IO20                                 20ULL     /* Offset[20], Register 0, Bit 20 */
#define PIN_IO_NUM_IO21                                 21ULL     /* Offset[21], Register 0, Bit 21 */
#define PIN_IO_NUM_IO22                                 22ULL     /* Offset[22], Register 0, Bit 22 */
#define PIN_IO_NUM_IO23                                 23ULL     /* Offset[23], Register 0, Bit 23 */
#define PIN_IO_NUM_IO24                                 24ULL     /* Offset[24], Register 0, Bit 24 */
#define PIN_IO_NUM_IO25                                 25ULL     /* Offset[25], Register 0, Bit 25 */
#define PIN_IO_NUM_IO26                                 26ULL     /* Offset[26], Register 0, Bit 26 */
#define PIN_IO_NUM_IO27                                 27ULL     /* Offset[27], Register 0, Bit 27 */
#define PIN_IO_NUM_IO28                                 28ULL     /* Offset[28], Register 0, Bit 28 */
#define PIN_IO_NUM_IO29                                 29ULL     /* Offset[29], Register 0, Bit 29 */
#define PIN_IO_NUM_IO30                                 30ULL     /* Offset[30], Register 0, Bit 30 */
#define PIN_IO_NUM_IO31                                 31ULL     /* Offset[31], Register 0, Bit 31 */
#define PIN_IO_NUM_IO32                                 32ULL     /* Offset[32], Register 1, Bit 0  */
#define PIN_IO_NUM_IO33                                 33ULL     /* Offset[33], Register 1, Bit 1  */
#define PIN_IO_NUM_IO34                                 34ULL     /* Offset[34], Register 1, Bit 2  */
#define PIN_IO_NUM_IO35                                 35ULL     /* Offset[35], Register 1, Bit 3  */
#define PIN_IO_NUM_IO36                                 36ULL     /* Offset[36], Register 1, Bit 4  */
#define PIN_IO_NUM_IO37                                 37ULL     /* Offset[37], Register 1, Bit 5  */
#define PIN_IO_NUM_IO38                                 38ULL     /* Offset[38], Register 1, Bit 6  */
#define PIN_IO_NUM_IO39                                 0ULL      /* Offset[0],  Register X, Bit 0  */
#define PIN_IO_NUM_IO40                                 1ULL      /* Offset[1],  Register X, Bit 1  */
#define PIN_IO_NUM_IO41                                 2ULL      /* Offset[2],  Register X, Bit 2  */
#define PIN_IO_NUM_IO42                                 3ULL      /* Offset[3],  Register X, Bit 3  */
#define PIN_IO_NUM_IO43                                 4ULL      /* Offset[4],  Register X, Bit 4  */
#define PIN_IO_NUM_IO44                                 5ULL      /* Offset[5],  Register X, Bit 5  */
#define PIN_IO_NUM_DIS                                  63ULL     /* Disabled                       */

#define PIN_IO_NUM_IO0_BITSVAL                          ((PIN_IO_NUM_IO0   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO1_BITSVAL                          ((PIN_IO_NUM_IO1   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO2_BITSVAL                          ((PIN_IO_NUM_IO2   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO3_BITSVAL                          ((PIN_IO_NUM_IO3   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO4_BITSVAL                          ((PIN_IO_NUM_IO4   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO5_BITSVAL                          ((PIN_IO_NUM_IO5   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO6_BITSVAL                          ((PIN_IO_NUM_IO6   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO7_BITSVAL                          ((PIN_IO_NUM_IO7   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO8_BITSVAL                          ((PIN_IO_NUM_IO8   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO9_BITSVAL                          ((PIN_IO_NUM_IO9   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO10_BITSVAL                         ((PIN_IO_NUM_IO10  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO11_BITSVAL                         ((PIN_IO_NUM_IO11  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO12_BITSVAL                         ((PIN_IO_NUM_IO12  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO13_BITSVAL                         ((PIN_IO_NUM_IO13  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO14_BITSVAL                         ((PIN_IO_NUM_IO14  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO15_BITSVAL                         ((PIN_IO_NUM_IO15  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO16_BITSVAL                         ((PIN_IO_NUM_IO16  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO17_BITSVAL                         ((PIN_IO_NUM_IO17  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO18_BITSVAL                         ((PIN_IO_NUM_IO18  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO19_BITSVAL                         ((PIN_IO_NUM_IO19  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO20_BITSVAL                         ((PIN_IO_NUM_IO20  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO21_BITSVAL                         ((PIN_IO_NUM_IO21  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO22_BITSVAL                         ((PIN_IO_NUM_IO22  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO23_BITSVAL                         ((PIN_IO_NUM_IO23  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO24_BITSVAL                         ((PIN_IO_NUM_IO24  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO25_BITSVAL                         ((PIN_IO_NUM_IO25  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO26_BITSVAL                         ((PIN_IO_NUM_IO26  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO27_BITSVAL                         ((PIN_IO_NUM_IO27  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO28_BITSVAL                         ((PIN_IO_NUM_IO28  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO29_BITSVAL                         ((PIN_IO_NUM_IO29  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO30_BITSVAL                         ((PIN_IO_NUM_IO30  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO31_BITSVAL                         ((PIN_IO_NUM_IO31  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO32_BITSVAL                         ((PIN_IO_NUM_IO32  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO33_BITSVAL                         ((PIN_IO_NUM_IO33  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO34_BITSVAL                         ((PIN_IO_NUM_IO34  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO35_BITSVAL                         ((PIN_IO_NUM_IO35  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO36_BITSVAL                         ((PIN_IO_NUM_IO36  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO37_BITSVAL                         ((PIN_IO_NUM_IO37  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO38_BITSVAL                         ((PIN_IO_NUM_IO38  << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO39_BITSVAL                         ((PIN_IO_NUM_IO39  << PIN_IO_NUM_BIT)| (1ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO40_BITSVAL                         ((PIN_IO_NUM_IO40  << PIN_IO_NUM_BIT)| (1ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO41_BITSVAL                         ((PIN_IO_NUM_IO41  << PIN_IO_NUM_BIT)| (1ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO42_BITSVAL                         ((PIN_IO_NUM_IO42  << PIN_IO_NUM_BIT)| (1ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO43_BITSVAL                         ((PIN_IO_NUM_IO43  << PIN_IO_NUM_BIT)| (1ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_IO44_BITSVAL                         ((PIN_IO_NUM_IO44  << PIN_IO_NUM_BIT)| (1ULL << PIN_IS_SIP_IO_BIT))
#define PIN_IO_NUM_DIS_BITSVAL                          ((PIN_IO_NUM_DIS   << PIN_IO_NUM_BIT)| (0ULL << PIN_IS_SIP_IO_BIT))

/* S_GPIO_PIN_CFG [9] InEnable.  [25] InInverter.  [26] IoPE.  [27] OutEnable.  [43] OutInverter.  [44] OutEnInverter.  [47] AutoCtrlEn.  [48] AutoCtrlLvl.  */
#define PIN_FUNC_DISABLE                                0ULL
#define PIN_FUNC_ENABLE                                 1ULL
#define PIN_IN_ENABLE_DISABLE_BITSVAL                   (PIN_FUNC_DISABLE << PIN_IN_ENABLE_BIT   )
#define PIN_IN_ENABLE_ENABLE_BITSVAL                    (PIN_FUNC_ENABLE  << PIN_IN_ENABLE_BIT   )
#define PIN_OUT_ENABLE_DISABLE_BITSVAL                  (PIN_FUNC_DISABLE << PIN_OUT_ENABLE_BIT  )
#define PIN_OUT_ENABLE_ENABLE_BITSVAL                   (PIN_FUNC_ENABLE  << PIN_OUT_ENABLE_BIT  )
#define PIN_AUTO_CTRL_EN_DISABLE_BITSVAL                (PIN_FUNC_DISABLE << PIN_AUTO_CTRL_EN_BIT)
#define PIN_AUTO_CTRL_EN_ENABLE_BITSVAL                 (PIN_FUNC_ENABLE  << PIN_AUTO_CTRL_EN_BIT)

/* S_GPIO_PIN_CFG [20:10] InGrpOfst.  */
#define PIN_INGRP_OFST_UART0_CTS_N                      1152ULL   /* Input group offset. Reg offset = 0x90, Bit offset=0  */
#define PIN_INGRP_OFST_UART0_RXD                        1153ULL   /* Input group offset. Reg offset = 0x90, Bit offset=4  */
#define PIN_INGRP_OFST_UART1_CTS_N                      1154ULL   /* Input group offset. Reg offset = 0x90, Bit offset=8  */
#define PIN_INGRP_OFST_UART1_RXD                        1155ULL   /* Input group offset. Reg offset = 0x90, Bit offset=12 */
#define PIN_INGRP_OFST_APS_UART_CTS_N                   1156ULL   /* Input group offset. Reg offset = 0x90, Bit offset=16 */
#define PIN_INGRP_OFST_APS_UART_RXD                     1157ULL   /* Input group offset. Reg offset = 0x90, Bit offset=20 */
#define PIN_INGRP_OFST_MSQ_UART_CTS_N                   1158ULL   /* Input group offset. Reg offset = 0x90, Bit offset=24 */
#define PIN_INGRP_OFST_MSQ_UART_RXD                     1159ULL   /* Input group offset. Reg offset = 0x90, Bit offset=28 */
#define PIN_INGRP_OFST_I2C_SCL_IN                       1312ULL   /* Input group offset. Reg offset = 0xA4, Bit offset=0  */
#define PIN_INGRP_OFST_I2C_SDA_IN                       1313ULL   /* Input group offset. Reg offset = 0xA4, Bit offset=4  */
#define PIN_INGRP_OFST_TDI                              1184ULL   /* Input group offset. Reg offset = 0x94, Bit offset=0  */
#define PIN_INGRP_OFST_TCK                              1185ULL   /* Input group offset. Reg offset = 0x94, Bit offset=4  */
#define PIN_INGRP_OFST_TMS                              1186ULL   /* Input group offset. Reg offset = 0x94, Bit offset=8  */
#define PIN_INGRP_OFST_APS_SWDI                         832ULL    /* Input group offset. Reg offset = 0x68, Bit offset=0  */
#define PIN_INGRP_OFST_APS_SWCLK                        833ULL    /* Input group offset. Reg offset = 0x68, Bit offset=4  */
#define PIN_INGRP_OFST_MSQ_SWDI                         834ULL    /* Input group offset. Reg offset = 0x68, Bit offset=8  */
#define PIN_INGRP_OFST_MSQ_SWCLK                        835ULL    /* Input group offset. Reg offset = 0x68, Bit offset=12 */
#define PIN_INGRP_OFST_SPI0_RXD0                        1216ULL   /* Input group offset. Reg offset = 0x98, Bit offset=0  */
#define PIN_INGRP_OFST_SPI0_RXD1                        1217ULL   /* Input group offset. Reg offset = 0x98, Bit offset=4  */
#define PIN_INGRP_OFST_SPI0_RXD2                        1218ULL   /* Input group offset. Reg offset = 0x98, Bit offset=8  */
#define PIN_INGRP_OFST_SPI0_RXD3                        1219ULL   /* Input group offset. Reg offset = 0x98, Bit offset=12 */
#define PIN_INGRP_OFST_SPI1_RXD0                        1220ULL   /* Input group offset. Reg offset = 0x98, Bit offset=16 */
#define PIN_INGRP_OFST_SPI1_RXD1                        1221ULL   /* Input group offset. Reg offset = 0x98, Bit offset=20 */
#define PIN_INGRP_OFST_SPI1_RXD2                        1222ULL   /* Input group offset. Reg offset = 0x98, Bit offset=24 */
#define PIN_INGRP_OFST_SPI1_RXD3                        1223ULL   /* Input group offset. Reg offset = 0x98, Bit offset=28 */
#define PIN_INGRP_OFST_SPI2_CLK                         1250ULL   /* Input group offset. Reg offset = 0x9C, Bit offset=8  */
#define PIN_INGRP_OFST_SPI2_CS_IN                       1249ULL   /* Input group offset. Reg offset = 0x9C, Bit offset=4  */
#define PIN_INGRP_OFST_SPI2_RXD                         1248ULL   /* Input group offset. Reg offset = 0x9C, Bit offset=0  */
#define PIN_INGRP_OFST_SPI3_RXD0                        1251ULL   /* Input group offset. Reg offset = 0x9C, Bit offset=12 */
#define PIN_INGRP_OFST_SPI3_RXD1                        1252ULL   /* Input group offset. Reg offset = 0x9C, Bit offset=16 */
#define PIN_INGRP_OFST_SPI3_RXD2                        1253ULL   /* Input group offset. Reg offset = 0x9C, Bit offset=20 */
#define PIN_INGRP_OFST_SPI3_RXD3                        1254ULL   /* Input group offset. Reg offset = 0x9C, Bit offset=24 */
#define PIN_INGRP_OFST_OTP_OPM_MODE                     1255ULL   /* Input group offset. Reg offset = 0x9C, Bit offset=28 */
#define PIN_INGRP_OFST_I2S_SDI0                         1187ULL   /* Input group offset. Reg offset = 0x94, Bit offset=12 */
#define PIN_INGRP_OFST_PDM_RXD                          1280ULL   /* Input group offset. Reg offset = 0xA0, Bit offset=0  */
#define PIN_INGRP_OFST_EXT_2IO_CLK_EN                   1286ULL   /* Input group offset. Reg offset = 0xA0, Bit offset=24 */
#define PIN_INGRP_OFST_CAM_PCLK                         1760ULL   /* Input group offset. Reg offset = 0xDC, Bit offset=0  */
#define PIN_INGRP_OFST_CAM_HREF                         1761ULL   /* Input group offset. Reg offset = 0xDC, Bit offset=4  */
#define PIN_INGRP_OFST_CAM_VSYNC_IN                     1762ULL   /* Input group offset. Reg offset = 0xDC, Bit offset=8  */
#define PIN_INGRP_OFST_CAM_HSYNC_IN                     1763ULL   /* Input group offset. Reg offset = 0xDC, Bit offset=12 */
#define PIN_INGRP_OFST_CAM_DATA0                        1696ULL   /* Input group offset. Reg offset = 0xD4, Bit offset=0  */
#define PIN_INGRP_OFST_CAM_DATA1                        1697ULL   /* Input group offset. Reg offset = 0xD4, Bit offset=4  */
#define PIN_INGRP_OFST_CAM_DATA2                        1698ULL   /* Input group offset. Reg offset = 0xD4, Bit offset=8  */
#define PIN_INGRP_OFST_CAM_DATA3                        1699ULL   /* Input group offset. Reg offset = 0xD4, Bit offset=12 */
#define PIN_INGRP_OFST_CAM_DATA4                        1700ULL   /* Input group offset. Reg offset = 0xD4, Bit offset=16 */
#define PIN_INGRP_OFST_CAM_DATA5                        1701ULL   /* Input group offset. Reg offset = 0xD4, Bit offset=20 */
#define PIN_INGRP_OFST_CAM_DATA6                        1702ULL   /* Input group offset. Reg offset = 0xD4, Bit offset=24 */
#define PIN_INGRP_OFST_CAM_DATA7                        1703ULL   /* Input group offset. Reg offset = 0xD4, Bit offset=28 */
#define PIN_INGRP_OFST_CAM_DATA8                        1728ULL   /* Input group offset. Reg offset = 0xD8, Bit offset=0  */
#define PIN_INGRP_OFST_CAM_DATA9                        1729ULL   /* Input group offset. Reg offset = 0xD8, Bit offset=4  */
#define PIN_INGRP_OFST_CAM_DATA10                       1730ULL   /* Input group offset. Reg offset = 0xD8, Bit offset=8  */
#define PIN_INGRP_OFST_CAM_DATA11                       1731ULL   /* Input group offset. Reg offset = 0xD8, Bit offset=12 */
#define PIN_INGRP_OFST_CAM_DATA12                       1732ULL   /* Input group offset. Reg offset = 0xD8, Bit offset=16 */
#define PIN_INGRP_OFST_CAM_DATA13                       1733ULL   /* Input group offset. Reg offset = 0xD8, Bit offset=20 */
#define PIN_INGRP_OFST_CAM_DATA14                       1734ULL   /* Input group offset. Reg offset = 0xD8, Bit offset=24 */
#define PIN_INGRP_OFST_CAM_DATA15                       1735ULL   /* Input group offset. Reg offset = 0xD8, Bit offset=28 */
#define PIN_INGRP_OFST_PSRAM_RXD0                       1188ULL   /* Input group offset. Reg offset = 0x94, Bit offset=16 */
#define PIN_INGRP_OFST_PSRAM_RXD1                       1189ULL   /* Input group offset. Reg offset = 0x94, Bit offset=20 */
#define PIN_INGRP_OFST_PSRAM_RXD2                       1190ULL   /* Input group offset. Reg offset = 0x94, Bit offset=24 */
#define PIN_INGRP_OFST_PSRAM_RXD3                       1191ULL   /* Input group offset. Reg offset = 0x94, Bit offset=28 */
#define PIN_INGRP_OFST_SPI1_CS_IN_N                     1284ULL   /* Input group offset. Reg offset = 0xA0, Bit offset=16 */
#define PIN_INGRP_OFST_SPI3_CS_IN_N                     1285ULL   /* Input group offset. Reg offset = 0xA0, Bit offset=20 */
#define PIN_INGRP_OFST_CLK_EXT_APS                      1281ULL   /* Input group offset. Reg offset = 0xA0, Bit offset=4  */
#define PIN_INGRP_OFST_CLK_EXT_MSQ                      1282ULL   /* Input group offset. Reg offset = 0xA0, Bit offset=8  */

#define PIN_INGRP_OFST_UART0_CTS_N_BITSVAL              (PIN_INGRP_OFST_UART0_CTS_N    << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_UART0_RXD_BITSVAL                (PIN_INGRP_OFST_UART0_RXD      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_UART1_CTS_N_BITSVAL              (PIN_INGRP_OFST_UART1_CTS_N    << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_UART1_RXD_BITSVAL                (PIN_INGRP_OFST_UART1_RXD      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL           (PIN_INGRP_OFST_APS_UART_CTS_N << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_APS_UART_RXD_BITSVAL             (PIN_INGRP_OFST_APS_UART_RXD   << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL           (PIN_INGRP_OFST_MSQ_UART_CTS_N << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL             (PIN_INGRP_OFST_MSQ_UART_RXD   << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL               (PIN_INGRP_OFST_I2C_SCL_IN     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL               (PIN_INGRP_OFST_I2C_SDA_IN     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_TDI_BITSVAL                      (PIN_INGRP_OFST_TDI            << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_TCK_BITSVAL                      (PIN_INGRP_OFST_TCK            << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_TMS_BITSVAL                      (PIN_INGRP_OFST_TMS            << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_APS_SWDI_BITSVAL                 (PIN_INGRP_OFST_APS_SWDI       << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_APS_SWCLK_BITSVAL                (PIN_INGRP_OFST_APS_SWCLK      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_MSQ_SWDI_BITSVAL                 (PIN_INGRP_OFST_MSQ_SWDI       << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL                (PIN_INGRP_OFST_MSQ_SWCLK      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI0_RXD0_BITSVAL                (PIN_INGRP_OFST_SPI0_RXD0      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI0_RXD1_BITSVAL                (PIN_INGRP_OFST_SPI0_RXD1      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI0_RXD2_BITSVAL                (PIN_INGRP_OFST_SPI0_RXD2      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI0_RXD3_BITSVAL                (PIN_INGRP_OFST_SPI0_RXD3      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI1_RXD0_BITSVAL                (PIN_INGRP_OFST_SPI1_RXD0      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI1_RXD1_BITSVAL                (PIN_INGRP_OFST_SPI1_RXD1      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI1_RXD2_BITSVAL                (PIN_INGRP_OFST_SPI1_RXD2      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI1_RXD3_BITSVAL                (PIN_INGRP_OFST_SPI1_RXD3      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI2_CLK_BITSVAL                 (PIN_INGRP_OFST_SPI2_CLK       << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI2_CS_IN_BITSVAL               (PIN_INGRP_OFST_SPI2_CS_IN     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI2_RXD_BITSVAL                 (PIN_INGRP_OFST_SPI2_RXD       << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI3_RXD0_BITSVAL                (PIN_INGRP_OFST_SPI3_RXD0      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI3_RXD1_BITSVAL                (PIN_INGRP_OFST_SPI3_RXD1      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI3_RXD2_BITSVAL                (PIN_INGRP_OFST_SPI3_RXD2      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI3_RXD3_BITSVAL                (PIN_INGRP_OFST_SPI3_RXD3      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_OTP_OPM_MODE_BITSVAL             (PIN_INGRP_OFST_OTP_OPM_MODE   << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_I2S_SDI0_BITSVAL                 (PIN_INGRP_OFST_I2S_SDI0       << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_PDM_RXD_BITSVAL                  (PIN_INGRP_OFST_PDM_RXD        << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_EXT_2IO_CLK_EN_BITSVAL           (PIN_INGRP_OFST_EXT_2IO_CLK_EN << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_PCLK_BITSVAL                 (PIN_INGRP_OFST_CAM_PCLK       << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_HREF_BITSVAL                 (PIN_INGRP_OFST_CAM_HREF       << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_VSYNC_IN_BITSVAL             (PIN_INGRP_OFST_CAM_VSYNC_IN   << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_HSYNC_IN_BITSVAL             (PIN_INGRP_OFST_CAM_HSYNC_IN   << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA0_BITSVAL                (PIN_INGRP_OFST_CAM_DATA0      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA1_BITSVAL                (PIN_INGRP_OFST_CAM_DATA1      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA2_BITSVAL                (PIN_INGRP_OFST_CAM_DATA2      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA3_BITSVAL                (PIN_INGRP_OFST_CAM_DATA3      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA4_BITSVAL                (PIN_INGRP_OFST_CAM_DATA4      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA5_BITSVAL                (PIN_INGRP_OFST_CAM_DATA5      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA6_BITSVAL                (PIN_INGRP_OFST_CAM_DATA6      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA7_BITSVAL                (PIN_INGRP_OFST_CAM_DATA7      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA8_BITSVAL                (PIN_INGRP_OFST_CAM_DATA8      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA9_BITSVAL                (PIN_INGRP_OFST_CAM_DATA9      << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA10_BITSVAL               (PIN_INGRP_OFST_CAM_DATA10     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA11_BITSVAL               (PIN_INGRP_OFST_CAM_DATA11     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA12_BITSVAL               (PIN_INGRP_OFST_CAM_DATA12     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA13_BITSVAL               (PIN_INGRP_OFST_CAM_DATA13     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA14_BITSVAL               (PIN_INGRP_OFST_CAM_DATA14     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CAM_DATA15_BITSVAL               (PIN_INGRP_OFST_CAM_DATA15     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_PSRAM_RXD0_BITSVAL               (PIN_INGRP_OFST_PSRAM_RXD0     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_PSRAM_RXD1_BITSVAL               (PIN_INGRP_OFST_PSRAM_RXD1     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_PSRAM_RXD2_BITSVAL               (PIN_INGRP_OFST_PSRAM_RXD2     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_PSRAM_RXD3_BITSVAL               (PIN_INGRP_OFST_PSRAM_RXD3     << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI1_CS_IN_N_BITSVAL             (PIN_INGRP_OFST_SPI1_CS_IN_N   << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_SPI3_CS_IN_N_BITSVAL             (PIN_INGRP_OFST_SPI3_CS_IN_N   << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL              (PIN_INGRP_OFST_CLK_EXT_APS    << PIN_INGRP_OFST_BIT)
#define PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL              (PIN_INGRP_OFST_CLK_EXT_MSQ    << PIN_INGRP_OFST_BIT)


/* S_GPIO_PIN_CFG [24:21] InGrpValue. UART0_CTS_N   */
#define PIN_INGRP_VAL_UART0_CTS_N_IO4                   0ULL
#define PIN_INGRP_VAL_UART0_CTS_N_IO9                   1ULL
#define PIN_INGRP_VAL_UART0_CTS_N_IO12                  2ULL
#define PIN_INGRP_VAL_UART0_CTS_N_IO15                  3ULL
#define PIN_INGRP_VAL_UART0_CTS_N_IO25                  4ULL
#define PIN_INGRP_VAL_UART0_CTS_N_IO27                  5ULL
#define PIN_INGRP_VAL_UART0_CTS_N_LOW                   6ULL
#define PIN_INGRP_VAL_UART0_CTS_N_HIGH                  7ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. UART0_RXD     */
#define PIN_INGRP_VAL_UART0_RXD_IO1                     0ULL
#define PIN_INGRP_VAL_UART0_RXD_IO2                     1ULL
#define PIN_INGRP_VAL_UART0_RXD_IO8                     2ULL
#define PIN_INGRP_VAL_UART0_RXD_IO11                    3ULL
#define PIN_INGRP_VAL_UART0_RXD_IO17                    4ULL
#define PIN_INGRP_VAL_UART0_RXD_IO18                    5ULL
#define PIN_INGRP_VAL_UART0_RXD_IO20                    6ULL
#define PIN_INGRP_VAL_UART0_RXD_IO29                    7ULL
#define PIN_INGRP_VAL_UART0_RXD_IO30                    8ULL
#define PIN_INGRP_VAL_UART0_RXD_IO33                    9ULL
#define PIN_INGRP_VAL_UART0_RXD_HIGH                    10ULL
#define PIN_INGRP_VAL_UART0_RXD_LOW                     11ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. UART1_CTS_N   */
#define PIN_INGRP_VAL_UART1_CTS_N_IO4                   0ULL
#define PIN_INGRP_VAL_UART1_CTS_N_IO9                   1ULL
#define PIN_INGRP_VAL_UART1_CTS_N_IO12                  2ULL
#define PIN_INGRP_VAL_UART1_CTS_N_IO14                  3ULL
#define PIN_INGRP_VAL_UART1_CTS_N_IO25                  4ULL
#define PIN_INGRP_VAL_UART1_CTS_N_IO27                  5ULL
#define PIN_INGRP_VAL_UART1_CTS_N_IO29                  6ULL
#define PIN_INGRP_VAL_UART1_CTS_N_IO33                  7ULL
#define PIN_INGRP_VAL_UART1_CTS_N_IO38                  8ULL
#define PIN_INGRP_VAL_UART1_CTS_N_LOW                   9ULL
#define PIN_INGRP_VAL_UART1_CTS_N_HIGH                  10ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. UART1_RXD     */
#define PIN_INGRP_VAL_UART1_RXD_IO1                     0ULL
#define PIN_INGRP_VAL_UART1_RXD_IO2                     1ULL
#define PIN_INGRP_VAL_UART1_RXD_IO8                     2ULL
#define PIN_INGRP_VAL_UART1_RXD_IO10                    3ULL
#define PIN_INGRP_VAL_UART1_RXD_IO16                    4ULL
#define PIN_INGRP_VAL_UART1_RXD_IO19                    5ULL
#define PIN_INGRP_VAL_UART1_RXD_IO21                    6ULL
#define PIN_INGRP_VAL_UART1_RXD_IO28                    7ULL
#define PIN_INGRP_VAL_UART1_RXD_IO32                    8ULL
#define PIN_INGRP_VAL_UART1_RXD_IO37                    9ULL
#define PIN_INGRP_VAL_UART1_RXD_HIGH                    10ULL
#define PIN_INGRP_VAL_UART1_RXD_LOW                     11ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. APS_UART_CTS_N*/
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO4                0ULL
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO9                1ULL
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO10               2ULL
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO17               3ULL
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO27               4ULL
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO28               5ULL
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO33               6ULL
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO35               7ULL
#define PIN_INGRP_VAL_APS_UART_CTS_N_LOW                8ULL
#define PIN_INGRP_VAL_APS_UART_CTS_N_HIGH               9ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. APS_UART_RXD  */
#define PIN_INGRP_VAL_APS_UART_RXD_IO1                  0ULL
#define PIN_INGRP_VAL_APS_UART_RXD_IO2                  1ULL
#define PIN_INGRP_VAL_APS_UART_RXD_IO8                  2ULL
#define PIN_INGRP_VAL_APS_UART_RXD_IO11                 3ULL
#define PIN_INGRP_VAL_APS_UART_RXD_IO15                 4ULL
#define PIN_INGRP_VAL_APS_UART_RXD_IO18                 5ULL
#define PIN_INGRP_VAL_APS_UART_RXD_IO20                 6ULL
#define PIN_INGRP_VAL_APS_UART_RXD_IO25                 7ULL
#define PIN_INGRP_VAL_APS_UART_RXD_IO30                 8ULL
#define PIN_INGRP_VAL_APS_UART_RXD_IO32                 9ULL
#define PIN_INGRP_VAL_APS_UART_RXD_HIGH                 10ULL
#define PIN_INGRP_VAL_APS_UART_RXD_LOW                  11ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. MSQ_UART_CTS_N*/
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO4                0ULL
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO9                1ULL
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO14               2ULL
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO18               3ULL
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO22               4ULL
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO27               5ULL
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO29               6ULL
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO38               7ULL
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_LOW                8ULL
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_HIGH               9ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. MSQ_UART_RXD  */
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO2                  0ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO8                  1ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO10                 2ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO16                 3ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO19                 4ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO20                 5ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO25                 6ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO28                 7ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO32                 8ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO33                 9ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO36                 10ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_HIGH                 11ULL
#define PIN_INGRP_VAL_MSQ_UART_RXD_LOW                  12ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. I2C_SCL_IN    */
#define PIN_INGRP_VAL_I2C_SCL_IN_IO0                    0ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO1                    1ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO4                    2ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO8                    3ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO10                   4ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO14                   5ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO16                   6ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO18                   7ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO20                   8ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO23                   9ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO25                   10ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO27                   11ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO31                   12ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_IO32                   13ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_HIGH                   14ULL
#define PIN_INGRP_VAL_I2C_SCL_IN_LOW                    15ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. I2C_SDA_IN    */
#define PIN_INGRP_VAL_I2C_SDA_IN_IO2                    0ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO5                    1ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO9                    2ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO11                   3ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO15                   4ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO17                   5ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO19                   6ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO21                   7ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO24                   8ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO26                   9ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO29                   10ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO30                   11ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_IO33                   12ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_HIGH                   13ULL
#define PIN_INGRP_VAL_I2C_SDA_IN_LOW                    14ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. TDI           */
#define PIN_INGRP_VAL_TDI_HIGH                          0ULL
#define PIN_INGRP_VAL_TDI_IO1                           1ULL
#define PIN_INGRP_VAL_TDI_IO4                           2ULL
#define PIN_INGRP_VAL_TDI_IO8                           3ULL
#define PIN_INGRP_VAL_TDI_IO10                          4ULL
#define PIN_INGRP_VAL_TDI_IO14                          5ULL
#define PIN_INGRP_VAL_TDI_IO18                          6ULL
#define PIN_INGRP_VAL_TDI_IO23                          7ULL
#define PIN_INGRP_VAL_TDI_IO25                          8ULL
#define PIN_INGRP_VAL_TDI_IO27                          9ULL
#define PIN_INGRP_VAL_TDI_IO32                          10ULL
#define PIN_INGRP_VAL_TDI_LOW                           11ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. TCK           */
#define PIN_INGRP_VAL_TCK_LOW                           0ULL
#define PIN_INGRP_VAL_TCK_IO2                           1ULL
#define PIN_INGRP_VAL_TCK_IO5                           2ULL
#define PIN_INGRP_VAL_TCK_IO13                          3ULL
#define PIN_INGRP_VAL_TCK_IO15                          4ULL
#define PIN_INGRP_VAL_TCK_IO19                          5ULL
#define PIN_INGRP_VAL_TCK_IO21                          6ULL
#define PIN_INGRP_VAL_TCK_IO22                          7ULL
#define PIN_INGRP_VAL_TCK_IO28                          8ULL
#define PIN_INGRP_VAL_TCK_IO34                          9ULL
#define PIN_INGRP_VAL_TCK_IO35                          10ULL
#define PIN_INGRP_VAL_TCK_IO37                          11ULL
#define PIN_INGRP_VAL_TCK_HIGH                          12ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. TMS           */
#define PIN_INGRP_VAL_TMS_HIGH                          0ULL
#define PIN_INGRP_VAL_TMS_IO0                           1ULL
#define PIN_INGRP_VAL_TMS_IO7                           2ULL
#define PIN_INGRP_VAL_TMS_IO12                          3ULL
#define PIN_INGRP_VAL_TMS_IO16                          4ULL
#define PIN_INGRP_VAL_TMS_IO26                          5ULL
#define PIN_INGRP_VAL_TMS_IO29                          6ULL
#define PIN_INGRP_VAL_TMS_IO30                          7ULL
#define PIN_INGRP_VAL_TMS_LOW                           8ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. APS_SWDI      */
#define PIN_INGRP_VAL_APS_SWDI_IO0                      0ULL
#define PIN_INGRP_VAL_APS_SWDI_IO1                      1ULL
#define PIN_INGRP_VAL_APS_SWDI_IO4                      2ULL
#define PIN_INGRP_VAL_APS_SWDI_IO7                      3ULL
#define PIN_INGRP_VAL_APS_SWDI_IO15                     4ULL
#define PIN_INGRP_VAL_APS_SWDI_IO16                     5ULL
#define PIN_INGRP_VAL_APS_SWDI_IO18                     6ULL
#define PIN_INGRP_VAL_APS_SWDI_IO25                     7ULL
#define PIN_INGRP_VAL_APS_SWDI_IO27                     8ULL
#define PIN_INGRP_VAL_APS_SWDI_IO29                     9ULL
#define PIN_INGRP_VAL_APS_SWDI_IO31                     10ULL
#define PIN_INGRP_VAL_APS_SWDI_IO33                     11ULL
#define PIN_INGRP_VAL_APS_SWDI_IO35                     12ULL
#define PIN_INGRP_VAL_APS_SWDI_LOW                      13ULL
#define PIN_INGRP_VAL_APS_SWDI_HIGH                     14ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. APS_SWCLK     */
#define PIN_INGRP_VAL_APS_SWCLK_IO2                     0ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO6                     1ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO8                     2ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO14                    3ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO17                    4ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO19                    5ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO20                    6ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO26                    7ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO28                    8ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO30                    9ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO32                    10ULL
#define PIN_INGRP_VAL_APS_SWCLK_IO34                    11ULL
#define PIN_INGRP_VAL_APS_SWCLK_LOW                     12ULL
#define PIN_INGRP_VAL_APS_SWCLK_HIGH                    13ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. MSQ_SWDI      */
#define PIN_INGRP_VAL_MSQ_SWDI_IO2                      0ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO6                      1ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO8                      2ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO14                     3ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO17                     4ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO19                     5ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO20                     6ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO22                     7ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO24                     8ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO26                     9ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO28                     10ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO30                     11ULL
#define PIN_INGRP_VAL_MSQ_SWDI_IO32                     12ULL
#define PIN_INGRP_VAL_MSQ_SWDI_LOW                      13ULL
#define PIN_INGRP_VAL_MSQ_SWDI_HIGH                     14ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. MSQ_SWCLK     */
#define PIN_INGRP_VAL_MSQ_SWCLK_IO0                     0ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO1                     1ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO4                     2ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO7                     3ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO15                    4ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO16                    5ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO18                    6ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO23                    7ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO25                    8ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO27                    9ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO29                    10ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO31                    11ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_IO33                    12ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_LOW                     13ULL
#define PIN_INGRP_VAL_MSQ_SWCLK_HIGH                    14ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI0_RXD0     */
#define PIN_INGRP_VAL_SPI0_RXD0_IO8                     0ULL
#define PIN_INGRP_VAL_SPI0_RXD0_IO14                    1ULL
#define PIN_INGRP_VAL_SPI0_RXD0_HIGH                    2ULL
#define PIN_INGRP_VAL_SPI0_RXD0_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI0_RXD1     */
#define PIN_INGRP_VAL_SPI0_RXD1_IO7                     0ULL
#define PIN_INGRP_VAL_SPI0_RXD1_IO15                    1ULL
#define PIN_INGRP_VAL_SPI0_RXD1_HIGH                    2ULL
#define PIN_INGRP_VAL_SPI0_RXD1_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI0_RXD2     */
#define PIN_INGRP_VAL_SPI0_RXD2_IO5                     0ULL
#define PIN_INGRP_VAL_SPI0_RXD2_IO10                    1ULL
#define PIN_INGRP_VAL_SPI0_RXD2_HIGH                    2ULL
#define PIN_INGRP_VAL_SPI0_RXD2_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI0_RXD3     */
#define PIN_INGRP_VAL_SPI0_RXD3_IO4                     0ULL
#define PIN_INGRP_VAL_SPI0_RXD3_IO11                    1ULL
#define PIN_INGRP_VAL_SPI0_RXD3_HIGH                    2ULL
#define PIN_INGRP_VAL_SPI0_RXD3_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI1_RXD0     */
#define PIN_INGRP_VAL_SPI1_RXD0_IO14                    0ULL
#define PIN_INGRP_VAL_SPI1_RXD0_IO8                     1ULL
#define PIN_INGRP_VAL_SPI1_RXD0_HIGH                    2ULL
#define PIN_INGRP_VAL_SPI1_RXD0_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI1_RXD1     */
#define PIN_INGRP_VAL_SPI1_RXD1_IO15                    0ULL
#define PIN_INGRP_VAL_SPI1_RXD1_IO7                     1ULL
#define PIN_INGRP_VAL_SPI1_RXD1_HIGH                    2ULL
#define PIN_INGRP_VAL_SPI1_RXD1_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI1_RXD2     */
#define PIN_INGRP_VAL_SPI1_RXD2_IO10                    0ULL
#define PIN_INGRP_VAL_SPI1_RXD2_IO5                     1ULL
#define PIN_INGRP_VAL_SPI1_RXD2_HIGH                    2ULL
#define PIN_INGRP_VAL_SPI1_RXD2_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI1_RXD3     */
#define PIN_INGRP_VAL_SPI1_RXD3_IO11                    0ULL
#define PIN_INGRP_VAL_SPI1_RXD3_IO4                     1ULL
#define PIN_INGRP_VAL_SPI1_RXD3_HIGH                    2ULL
#define PIN_INGRP_VAL_SPI1_RXD3_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI2_CLK      */
#define PIN_INGRP_VAL_SPI2_CLK_IO1                      0ULL
#define PIN_INGRP_VAL_SPI2_CLK_IO24                     1ULL
#define PIN_INGRP_VAL_SPI2_CLK_IO32                     2ULL
#define PIN_INGRP_VAL_SPI2_CLK_HIGH                     3ULL
#define PIN_INGRP_VAL_SPI2_CLK_LOW                      4ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI2_CS_IN    */
#define PIN_INGRP_VAL_SPI2_CS_IN_IO22                   0ULL
#define PIN_INGRP_VAL_SPI2_CS_IN_IO25                   1ULL
#define PIN_INGRP_VAL_SPI2_CS_IN_IO33                   2ULL
#define PIN_INGRP_VAL_SPI2_CS_IN_HIGH                   3ULL
#define PIN_INGRP_VAL_SPI2_CS_IN_LOW                    4ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI2_RXD      */
#define PIN_INGRP_VAL_SPI2_RXD_IO26                     0ULL
#define PIN_INGRP_VAL_SPI2_RXD_IO28                     1ULL
#define PIN_INGRP_VAL_SPI2_RXD_IO29                     2ULL
#define PIN_INGRP_VAL_SPI2_RXD_IO32                     3ULL
#define PIN_INGRP_VAL_SPI2_RXD_IO33                     4ULL
#define PIN_INGRP_VAL_SPI2_RXD_IO36                     5ULL
#define PIN_INGRP_VAL_SPI2_RXD_HIGH                     6ULL
#define PIN_INGRP_VAL_SPI2_RXD_LOW                      7ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI3_RXD0     */
#define PIN_INGRP_VAL_SPI3_RXD0_IO5                     0ULL
#define PIN_INGRP_VAL_SPI3_RXD0_IO17                    1ULL
#define PIN_INGRP_VAL_SPI3_RXD0_IO27                    2ULL
#define PIN_INGRP_VAL_SPI3_RXD0_IO29                    3ULL
#define PIN_INGRP_VAL_SPI3_RXD0_IO35                    4ULL
#define PIN_INGRP_VAL_SPI3_RXD0_IO36                    5ULL
#define PIN_INGRP_VAL_SPI3_RXD0_HIGH                    6ULL
#define PIN_INGRP_VAL_SPI3_RXD0_LOW                     7ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI3_RXD1     */
#define PIN_INGRP_VAL_SPI3_RXD1_IO4                     0ULL
#define PIN_INGRP_VAL_SPI3_RXD1_IO16                    1ULL
#define PIN_INGRP_VAL_SPI3_RXD1_IO29                    2ULL
#define PIN_INGRP_VAL_SPI3_RXD1_IO33                    3ULL
#define PIN_INGRP_VAL_SPI3_RXD1_IO34                    4ULL
#define PIN_INGRP_VAL_SPI3_RXD1_IO36                    5ULL
#define PIN_INGRP_VAL_SPI3_RXD1_HIGH                    6ULL
#define PIN_INGRP_VAL_SPI3_RXD1_LOW                     7ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI3_RXD2     */
#define PIN_INGRP_VAL_SPI3_RXD2_IO2                     0ULL
#define PIN_INGRP_VAL_SPI3_RXD2_IO23                    1ULL
#define PIN_INGRP_VAL_SPI3_RXD2_IO28                    2ULL
#define PIN_INGRP_VAL_SPI3_RXD2_IO32                    3ULL
#define PIN_INGRP_VAL_SPI3_RXD2_HIGH                    4ULL
#define PIN_INGRP_VAL_SPI3_RXD2_LOW                     5ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. SPI3_RXD3     */
#define PIN_INGRP_VAL_SPI3_RXD3_IO0                     0ULL
#define PIN_INGRP_VAL_SPI3_RXD3_IO1                     1ULL
#define PIN_INGRP_VAL_SPI3_RXD3_IO31                    2ULL
#define PIN_INGRP_VAL_SPI3_RXD3_HIGH                    3ULL
#define PIN_INGRP_VAL_SPI3_RXD3_LOW                     4ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. OTP_OPM_MODE  */
#define PIN_INGRP_VAL_OTP_OPM_MODE_IO11                 0ULL
#define PIN_INGRP_VAL_OTP_OPM_MODE_IO31                 1ULL
#define PIN_INGRP_VAL_OTP_OPM_MODE_IO33                 2ULL
#define PIN_INGRP_VAL_OTP_OPM_MODE_IO37                 3ULL
#define PIN_INGRP_VAL_OTP_OPM_MODE_HIGH                 4ULL
#define PIN_INGRP_VAL_OTP_OPM_MODE_LOW                  5ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. I2S_SDI0      */
#define PIN_INGRP_VAL_I2S_SDI0_IO4                      0ULL
#define PIN_INGRP_VAL_I2S_SDI0_IO8                      1ULL
#define PIN_INGRP_VAL_I2S_SDI0_IO12                     2ULL
#define PIN_INGRP_VAL_I2S_SDI0_IO17                     3ULL
#define PIN_INGRP_VAL_I2S_SDI0_IO20                     4ULL
#define PIN_INGRP_VAL_I2S_SDI0_IO22                     5ULL
#define PIN_INGRP_VAL_I2S_SDI0_IO26                     6ULL
#define PIN_INGRP_VAL_I2S_SDI0_IO29                     7ULL
#define PIN_INGRP_VAL_I2S_SDI0_IO30                     8ULL
#define PIN_INGRP_VAL_I2S_SDI0_IO36                     9ULL
#define PIN_INGRP_VAL_I2S_SDI0_HIGH                     10ULL
#define PIN_INGRP_VAL_I2S_SDI0_LOW                      11ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. PDM_RXD       */
#define PIN_INGRP_VAL_PDM_RXD_IO7                       0ULL
#define PIN_INGRP_VAL_PDM_RXD_IO11                      1ULL
#define PIN_INGRP_VAL_PDM_RXD_IO13                      2ULL
#define PIN_INGRP_VAL_PDM_RXD_IO25                      3ULL
#define PIN_INGRP_VAL_PDM_RXD_IO26                      4ULL
#define PIN_INGRP_VAL_PDM_RXD_IO35                      5ULL
#define PIN_INGRP_VAL_PDM_RXD_IO36                      6ULL
#define PIN_INGRP_VAL_PDM_RXD_HIGH                      7ULL
#define PIN_INGRP_VAL_PDM_RXD_LOW                       8ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. EXT_2IO_CLK_EN*/
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO4                0ULL
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO22               1ULL
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO23               2ULL
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO35               3ULL
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO36               4ULL
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_LOW                5ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_PCLK      */
#define PIN_INGRP_VAL_CAM_PCLK_IO1                      0ULL
#define PIN_INGRP_VAL_CAM_PCLK_IO15                     1ULL
#define PIN_INGRP_VAL_CAM_PCLK_IO31                     2ULL
#define PIN_INGRP_VAL_CAM_PCLK_HIGH                     3ULL
#define PIN_INGRP_VAL_CAM_PCLK_LOW                      4ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_HREF      */
#define PIN_INGRP_VAL_CAM_HREF_IO11                     0ULL
#define PIN_INGRP_VAL_CAM_HREF_IO21                     1ULL
#define PIN_INGRP_VAL_CAM_HREF_HIGH                     2ULL
#define PIN_INGRP_VAL_CAM_HREF_LOW                      3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_VSYNC_IN  */
#define PIN_INGRP_VAL_CAM_VSYNC_IN_IO12                 0ULL
#define PIN_INGRP_VAL_CAM_VSYNC_IN_IO26                 1ULL
#define PIN_INGRP_VAL_CAM_VSYNC_IN_HIGH                 2ULL
#define PIN_INGRP_VAL_CAM_VSYNC_IN_LOW                  3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_HSYNC_IN  */
#define PIN_INGRP_VAL_CAM_HSYNC_IN_IO15                 0ULL
#define PIN_INGRP_VAL_CAM_HSYNC_IN_IO24                 1ULL
#define PIN_INGRP_VAL_CAM_HSYNC_IN_HIGH                 2ULL
#define PIN_INGRP_VAL_CAM_HSYNC_IN_LOW                  3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA0     */
#define PIN_INGRP_VAL_CAM_DATA0_IO3                     0ULL
#define PIN_INGRP_VAL_CAM_DATA0_HIGH                    1ULL
#define PIN_INGRP_VAL_CAM_DATA0_LOW                     2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA1     */
#define PIN_INGRP_VAL_CAM_DATA1_IO4                     0ULL
#define PIN_INGRP_VAL_CAM_DATA1_HIGH                    1ULL
#define PIN_INGRP_VAL_CAM_DATA1_LOW                     2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA2     */
#define PIN_INGRP_VAL_CAM_DATA2_IO5                     0ULL
#define PIN_INGRP_VAL_CAM_DATA2_IO32                    1ULL
#define PIN_INGRP_VAL_CAM_DATA2_HIGH                    2ULL
#define PIN_INGRP_VAL_CAM_DATA2_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA3     */
#define PIN_INGRP_VAL_CAM_DATA3_IO6                     0ULL
#define PIN_INGRP_VAL_CAM_DATA3_HIGH                    1ULL
#define PIN_INGRP_VAL_CAM_DATA3_LOW                     2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA4     */
#define PIN_INGRP_VAL_CAM_DATA4_IO7                     0ULL
#define PIN_INGRP_VAL_CAM_DATA4_HIGH                    1ULL
#define PIN_INGRP_VAL_CAM_DATA4_LOW                     2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA5     */
#define PIN_INGRP_VAL_CAM_DATA5_IO8                     0ULL
#define PIN_INGRP_VAL_CAM_DATA5_IO33                    1ULL
#define PIN_INGRP_VAL_CAM_DATA5_HIGH                    2ULL
#define PIN_INGRP_VAL_CAM_DATA5_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA6     */
#define PIN_INGRP_VAL_CAM_DATA6_IO9                     0ULL
#define PIN_INGRP_VAL_CAM_DATA6_HIGH                    1ULL
#define PIN_INGRP_VAL_CAM_DATA6_LOW                     2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA7     */
#define PIN_INGRP_VAL_CAM_DATA7_IO10                    0ULL
#define PIN_INGRP_VAL_CAM_DATA7_IO20                    1ULL
#define PIN_INGRP_VAL_CAM_DATA7_HIGH                    2ULL
#define PIN_INGRP_VAL_CAM_DATA7_LOW                     3ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA8     */
#define PIN_INGRP_VAL_CAM_DATA8_IO27                    0ULL
#define PIN_INGRP_VAL_CAM_DATA8_HIGH                    1ULL
#define PIN_INGRP_VAL_CAM_DATA8_LOW                     2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA9     */
#define PIN_INGRP_VAL_CAM_DATA9_IO28                    0ULL
#define PIN_INGRP_VAL_CAM_DATA9_HIGH                    1ULL
#define PIN_INGRP_VAL_CAM_DATA9_LOW                     2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA10    */
#define PIN_INGRP_VAL_CAM_DATA10_IO29                   0ULL
#define PIN_INGRP_VAL_CAM_DATA10_HIGH                   1ULL
#define PIN_INGRP_VAL_CAM_DATA10_LOW                    2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA11    */
#define PIN_INGRP_VAL_CAM_DATA11_IO30                   0ULL
#define PIN_INGRP_VAL_CAM_DATA11_HIGH                   1ULL
#define PIN_INGRP_VAL_CAM_DATA11_LOW                    2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA12    */
#define PIN_INGRP_VAL_CAM_DATA12_IO34                   0ULL
#define PIN_INGRP_VAL_CAM_DATA12_HIGH                   1ULL
#define PIN_INGRP_VAL_CAM_DATA12_LOW                    2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA13    */
#define PIN_INGRP_VAL_CAM_DATA13_IO35                   0ULL
#define PIN_INGRP_VAL_CAM_DATA13_HIGH                   1ULL
#define PIN_INGRP_VAL_CAM_DATA13_LOW                    2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA14    */
#define PIN_INGRP_VAL_CAM_DATA14_IO36                   0ULL
#define PIN_INGRP_VAL_CAM_DATA14_HIGH                   1ULL
#define PIN_INGRP_VAL_CAM_DATA14_LOW                    2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CAM_DATA15    */
#define PIN_INGRP_VAL_CAM_DATA15_IO37                   0ULL
#define PIN_INGRP_VAL_CAM_DATA15_HIGH                   1ULL
#define PIN_INGRP_VAL_CAM_DATA15_LOW                    2ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. PSRAM_RXD0    */
#define PIN_INGRP_VAL_PSRAM_RXD0_IO39                   0ULL
#define PIN_INGRP_VAL_PSRAM_RXD0_HIGH                   3ULL
#define PIN_INGRP_VAL_PSRAM_RXD0_LOW                    4ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. PSRAM_RXD1    */
#define PIN_INGRP_VAL_PSRAM_RXD1_IO40                   0ULL
#define PIN_INGRP_VAL_PSRAM_RXD1_HIGH                   3ULL
#define PIN_INGRP_VAL_PSRAM_RXD1_LOW                    4ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. PSRAM_RXD2    */
#define PIN_INGRP_VAL_PSRAM_RXD2_IO41                   0ULL
#define PIN_INGRP_VAL_PSRAM_RXD2_HIGH                   3ULL
#define PIN_INGRP_VAL_PSRAM_RXD2_LOW                    4ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. PSRAM_RXD3    */
#define PIN_INGRP_VAL_PSRAM_RXD3_IO42                   0ULL
#define PIN_INGRP_VAL_PSRAM_RXD3_HIGH                   3ULL
#define PIN_INGRP_VAL_PSRAM_RXD3_LOW                    4ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CLK_EXT_APS   */
#define PIN_INGRP_VAL_CLK_EXT_APS_LOW                   0ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO4                   1ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO5                   2ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO6                   3ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO7                   4ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO8                   5ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO9                   6ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO10                  7ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO11                  8ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO18                  9ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO19                  10ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO34                  11ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO35                  12ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_IO38                  13ULL
#define PIN_INGRP_VAL_CLK_EXT_APS_HIGH                  14ULL

/* S_GPIO_PIN_CFG [24:21] InGrpValue. CLK_EXT_MSQ   */
#define PIN_INGRP_VAL_CLK_EXT_MSQ_LOW                   0ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO4                   1ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO5                   2ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO6                   3ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO7                   4ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO8                   5ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO9                   6ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO10                  7ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO11                  8ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO18                  9ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO19                  10ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO34                  11ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO35                  12ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO38                  13ULL
#define PIN_INGRP_VAL_CLK_EXT_MSQ_HIGH                  14ULL

#define PIN_INGRP_VAL_UART0_CTS_N_IO4_BITSVAL           (PIN_INGRP_VAL_UART0_CTS_N_IO4       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_CTS_N_IO9_BITSVAL           (PIN_INGRP_VAL_UART0_CTS_N_IO9       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_CTS_N_IO12_BITSVAL          (PIN_INGRP_VAL_UART0_CTS_N_IO12      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_CTS_N_IO15_BITSVAL          (PIN_INGRP_VAL_UART0_CTS_N_IO15      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_CTS_N_IO25_BITSVAL          (PIN_INGRP_VAL_UART0_CTS_N_IO25      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_CTS_N_IO27_BITSVAL          (PIN_INGRP_VAL_UART0_CTS_N_IO27      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_CTS_N_LOW_BITSVAL           (PIN_INGRP_VAL_UART0_CTS_N_LOW       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_CTS_N_HIGH_BITSVAL          (PIN_INGRP_VAL_UART0_CTS_N_HIGH      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_IO1_BITSVAL             (PIN_INGRP_VAL_UART0_RXD_IO1         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_IO2_BITSVAL             (PIN_INGRP_VAL_UART0_RXD_IO2         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_IO8_BITSVAL             (PIN_INGRP_VAL_UART0_RXD_IO8         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_IO11_BITSVAL            (PIN_INGRP_VAL_UART0_RXD_IO11        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_IO17_BITSVAL            (PIN_INGRP_VAL_UART0_RXD_IO17        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_IO18_BITSVAL            (PIN_INGRP_VAL_UART0_RXD_IO18        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_IO20_BITSVAL            (PIN_INGRP_VAL_UART0_RXD_IO20        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_IO29_BITSVAL            (PIN_INGRP_VAL_UART0_RXD_IO29        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_IO30_BITSVAL            (PIN_INGRP_VAL_UART0_RXD_IO30        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_IO33_BITSVAL            (PIN_INGRP_VAL_UART0_RXD_IO33        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_HIGH_BITSVAL            (PIN_INGRP_VAL_UART0_RXD_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART0_RXD_LOW_BITSVAL             (PIN_INGRP_VAL_UART0_RXD_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_IO4_BITSVAL           (PIN_INGRP_VAL_UART1_CTS_N_IO4       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_IO9_BITSVAL           (PIN_INGRP_VAL_UART1_CTS_N_IO9       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_IO12_BITSVAL          (PIN_INGRP_VAL_UART1_CTS_N_IO12      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_IO14_BITSVAL          (PIN_INGRP_VAL_UART1_CTS_N_IO14      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_IO25_BITSVAL          (PIN_INGRP_VAL_UART1_CTS_N_IO25      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_IO27_BITSVAL          (PIN_INGRP_VAL_UART1_CTS_N_IO27      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_IO29_BITSVAL          (PIN_INGRP_VAL_UART1_CTS_N_IO29      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_IO33_BITSVAL          (PIN_INGRP_VAL_UART1_CTS_N_IO33      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_IO38_BITSVAL          (PIN_INGRP_VAL_UART1_CTS_N_IO38      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_LOW_BITSVAL           (PIN_INGRP_VAL_UART1_CTS_N_LOW       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_CTS_N_HIGH_BITSVAL          (PIN_INGRP_VAL_UART1_CTS_N_HIGH      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_IO1_BITSVAL             (PIN_INGRP_VAL_UART1_RXD_IO1         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_IO2_BITSVAL             (PIN_INGRP_VAL_UART1_RXD_IO2         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_IO8_BITSVAL             (PIN_INGRP_VAL_UART1_RXD_IO8         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_IO10_BITSVAL            (PIN_INGRP_VAL_UART1_RXD_IO10        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_IO16_BITSVAL            (PIN_INGRP_VAL_UART1_RXD_IO16        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_IO19_BITSVAL            (PIN_INGRP_VAL_UART1_RXD_IO19        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_IO21_BITSVAL            (PIN_INGRP_VAL_UART1_RXD_IO21        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_IO28_BITSVAL            (PIN_INGRP_VAL_UART1_RXD_IO28        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_IO32_BITSVAL            (PIN_INGRP_VAL_UART1_RXD_IO32        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_IO37_BITSVAL            (PIN_INGRP_VAL_UART1_RXD_IO37        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_HIGH_BITSVAL            (PIN_INGRP_VAL_UART1_RXD_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_UART1_RXD_LOW_BITSVAL             (PIN_INGRP_VAL_UART1_RXD_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO4_BITSVAL        (PIN_INGRP_VAL_APS_UART_CTS_N_IO4    << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO9_BITSVAL        (PIN_INGRP_VAL_APS_UART_CTS_N_IO9    << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO10_BITSVAL       (PIN_INGRP_VAL_APS_UART_CTS_N_IO10   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO17_BITSVAL       (PIN_INGRP_VAL_APS_UART_CTS_N_IO17   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO27_BITSVAL       (PIN_INGRP_VAL_APS_UART_CTS_N_IO27   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO28_BITSVAL       (PIN_INGRP_VAL_APS_UART_CTS_N_IO28   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO33_BITSVAL       (PIN_INGRP_VAL_APS_UART_CTS_N_IO33   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_CTS_N_IO35_BITSVAL       (PIN_INGRP_VAL_APS_UART_CTS_N_IO35   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_CTS_N_LOW_BITSVAL        (PIN_INGRP_VAL_APS_UART_CTS_N_LOW    << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_CTS_N_HIGH_BITSVAL       (PIN_INGRP_VAL_APS_UART_CTS_N_HIGH   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_IO1_BITSVAL          (PIN_INGRP_VAL_APS_UART_RXD_IO1      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_IO2_BITSVAL          (PIN_INGRP_VAL_APS_UART_RXD_IO2      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_IO8_BITSVAL          (PIN_INGRP_VAL_APS_UART_RXD_IO8      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_IO11_BITSVAL         (PIN_INGRP_VAL_APS_UART_RXD_IO11     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_IO15_BITSVAL         (PIN_INGRP_VAL_APS_UART_RXD_IO15     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_IO18_BITSVAL         (PIN_INGRP_VAL_APS_UART_RXD_IO18     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_IO20_BITSVAL         (PIN_INGRP_VAL_APS_UART_RXD_IO20     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_IO25_BITSVAL         (PIN_INGRP_VAL_APS_UART_RXD_IO25     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_IO30_BITSVAL         (PIN_INGRP_VAL_APS_UART_RXD_IO30     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_IO32_BITSVAL         (PIN_INGRP_VAL_APS_UART_RXD_IO32     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_HIGH_BITSVAL         (PIN_INGRP_VAL_APS_UART_RXD_HIGH     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_UART_RXD_LOW_BITSVAL          (PIN_INGRP_VAL_APS_UART_RXD_LOW      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO4_BITSVAL        (PIN_INGRP_VAL_MSQ_UART_CTS_N_IO4    << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO9_BITSVAL        (PIN_INGRP_VAL_MSQ_UART_CTS_N_IO9    << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO14_BITSVAL       (PIN_INGRP_VAL_MSQ_UART_CTS_N_IO14   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO18_BITSVAL       (PIN_INGRP_VAL_MSQ_UART_CTS_N_IO18   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO22_BITSVAL       (PIN_INGRP_VAL_MSQ_UART_CTS_N_IO22   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO27_BITSVAL       (PIN_INGRP_VAL_MSQ_UART_CTS_N_IO27   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO29_BITSVAL       (PIN_INGRP_VAL_MSQ_UART_CTS_N_IO29   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_IO38_BITSVAL       (PIN_INGRP_VAL_MSQ_UART_CTS_N_IO38   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_LOW_BITSVAL        (PIN_INGRP_VAL_MSQ_UART_CTS_N_LOW    << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_CTS_N_HIGH_BITSVAL       (PIN_INGRP_VAL_MSQ_UART_CTS_N_HIGH   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO2_BITSVAL          (PIN_INGRP_VAL_MSQ_UART_RXD_IO2      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO8_BITSVAL          (PIN_INGRP_VAL_MSQ_UART_RXD_IO8      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO10_BITSVAL         (PIN_INGRP_VAL_MSQ_UART_RXD_IO10     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO16_BITSVAL         (PIN_INGRP_VAL_MSQ_UART_RXD_IO16     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO19_BITSVAL         (PIN_INGRP_VAL_MSQ_UART_RXD_IO19     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO20_BITSVAL         (PIN_INGRP_VAL_MSQ_UART_RXD_IO20     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO25_BITSVAL         (PIN_INGRP_VAL_MSQ_UART_RXD_IO25     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO28_BITSVAL         (PIN_INGRP_VAL_MSQ_UART_RXD_IO28     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO32_BITSVAL         (PIN_INGRP_VAL_MSQ_UART_RXD_IO32     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO33_BITSVAL         (PIN_INGRP_VAL_MSQ_UART_RXD_IO33     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_IO36_BITSVAL         (PIN_INGRP_VAL_MSQ_UART_RXD_IO36     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_HIGH_BITSVAL         (PIN_INGRP_VAL_MSQ_UART_RXD_HIGH     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_UART_RXD_LOW_BITSVAL          (PIN_INGRP_VAL_MSQ_UART_RXD_LOW      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO0_BITSVAL            (PIN_INGRP_VAL_I2C_SCL_IN_IO0        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO1_BITSVAL            (PIN_INGRP_VAL_I2C_SCL_IN_IO1        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO4_BITSVAL            (PIN_INGRP_VAL_I2C_SCL_IN_IO4        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO8_BITSVAL            (PIN_INGRP_VAL_I2C_SCL_IN_IO8        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO10_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_IO10       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO14_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_IO14       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO16_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_IO16       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO18_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_IO18       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO20_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_IO20       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO23_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_IO23       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO25_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_IO25       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO27_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_IO27       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO31_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_IO31       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_IO32_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_IO32       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_HIGH_BITSVAL           (PIN_INGRP_VAL_I2C_SCL_IN_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SCL_IN_LOW_BITSVAL            (PIN_INGRP_VAL_I2C_SCL_IN_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO2_BITSVAL            (PIN_INGRP_VAL_I2C_SDA_IN_IO2        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO5_BITSVAL            (PIN_INGRP_VAL_I2C_SDA_IN_IO5        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO9_BITSVAL            (PIN_INGRP_VAL_I2C_SDA_IN_IO9        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO11_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_IO11       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO15_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_IO15       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO17_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_IO17       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO19_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_IO19       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO21_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_IO21       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO24_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_IO24       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO26_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_IO26       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO29_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_IO29       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO30_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_IO30       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_IO33_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_IO33       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_HIGH_BITSVAL           (PIN_INGRP_VAL_I2C_SDA_IN_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2C_SDA_IN_LOW_BITSVAL            (PIN_INGRP_VAL_I2C_SDA_IN_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_HIGH_BITSVAL                  (PIN_INGRP_VAL_TDI_HIGH              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_IO1_BITSVAL                   (PIN_INGRP_VAL_TDI_IO1               << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_IO4_BITSVAL                   (PIN_INGRP_VAL_TDI_IO4               << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_IO8_BITSVAL                   (PIN_INGRP_VAL_TDI_IO8               << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_IO10_BITSVAL                  (PIN_INGRP_VAL_TDI_IO10              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_IO14_BITSVAL                  (PIN_INGRP_VAL_TDI_IO14              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_IO18_BITSVAL                  (PIN_INGRP_VAL_TDI_IO18              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_IO23_BITSVAL                  (PIN_INGRP_VAL_TDI_IO23              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_IO25_BITSVAL                  (PIN_INGRP_VAL_TDI_IO25              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_IO27_BITSVAL                  (PIN_INGRP_VAL_TDI_IO27              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_IO32_BITSVAL                  (PIN_INGRP_VAL_TDI_IO32              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TDI_LOW_BITSVAL                   (PIN_INGRP_VAL_TDI_LOW               << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_LOW_BITSVAL                   (PIN_INGRP_VAL_TCK_LOW               << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO2_BITSVAL                   (PIN_INGRP_VAL_TCK_IO2               << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO5_BITSVAL                   (PIN_INGRP_VAL_TCK_IO5               << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO13_BITSVAL                  (PIN_INGRP_VAL_TCK_IO13              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO15_BITSVAL                  (PIN_INGRP_VAL_TCK_IO15              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO19_BITSVAL                  (PIN_INGRP_VAL_TCK_IO19              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO21_BITSVAL                  (PIN_INGRP_VAL_TCK_IO21              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO22_BITSVAL                  (PIN_INGRP_VAL_TCK_IO22              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO28_BITSVAL                  (PIN_INGRP_VAL_TCK_IO28              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO34_BITSVAL                  (PIN_INGRP_VAL_TCK_IO34              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO35_BITSVAL                  (PIN_INGRP_VAL_TCK_IO35              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_IO37_BITSVAL                  (PIN_INGRP_VAL_TCK_IO37              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TCK_HIGH_BITSVAL                  (PIN_INGRP_VAL_TCK_HIGH              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TMS_HIGH_BITSVAL                  (PIN_INGRP_VAL_TMS_HIGH              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TMS_IO0_BITSVAL                   (PIN_INGRP_VAL_TMS_IO0               << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TMS_IO7_BITSVAL                   (PIN_INGRP_VAL_TMS_IO7               << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TMS_IO12_BITSVAL                  (PIN_INGRP_VAL_TMS_IO12              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TMS_IO16_BITSVAL                  (PIN_INGRP_VAL_TMS_IO16              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TMS_IO26_BITSVAL                  (PIN_INGRP_VAL_TMS_IO26              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TMS_IO29_BITSVAL                  (PIN_INGRP_VAL_TMS_IO29              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TMS_IO30_BITSVAL                  (PIN_INGRP_VAL_TMS_IO30              << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_TMS_LOW_BITSVAL                   (PIN_INGRP_VAL_TMS_LOW               << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO0_BITSVAL              (PIN_INGRP_VAL_APS_SWDI_IO0          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO1_BITSVAL              (PIN_INGRP_VAL_APS_SWDI_IO1          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO4_BITSVAL              (PIN_INGRP_VAL_APS_SWDI_IO4          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO7_BITSVAL              (PIN_INGRP_VAL_APS_SWDI_IO7          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO15_BITSVAL             (PIN_INGRP_VAL_APS_SWDI_IO15         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO16_BITSVAL             (PIN_INGRP_VAL_APS_SWDI_IO16         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO18_BITSVAL             (PIN_INGRP_VAL_APS_SWDI_IO18         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO25_BITSVAL             (PIN_INGRP_VAL_APS_SWDI_IO25         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO27_BITSVAL             (PIN_INGRP_VAL_APS_SWDI_IO27         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO29_BITSVAL             (PIN_INGRP_VAL_APS_SWDI_IO29         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO31_BITSVAL             (PIN_INGRP_VAL_APS_SWDI_IO31         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO33_BITSVAL             (PIN_INGRP_VAL_APS_SWDI_IO33         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_IO35_BITSVAL             (PIN_INGRP_VAL_APS_SWDI_IO35         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_LOW_BITSVAL              (PIN_INGRP_VAL_APS_SWDI_LOW          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWDI_HIGH_BITSVAL             (PIN_INGRP_VAL_APS_SWDI_HIGH         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO2_BITSVAL             (PIN_INGRP_VAL_APS_SWCLK_IO2         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO6_BITSVAL             (PIN_INGRP_VAL_APS_SWCLK_IO6         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO8_BITSVAL             (PIN_INGRP_VAL_APS_SWCLK_IO8         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO14_BITSVAL            (PIN_INGRP_VAL_APS_SWCLK_IO14        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO17_BITSVAL            (PIN_INGRP_VAL_APS_SWCLK_IO17        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO19_BITSVAL            (PIN_INGRP_VAL_APS_SWCLK_IO19        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO20_BITSVAL            (PIN_INGRP_VAL_APS_SWCLK_IO20        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO26_BITSVAL            (PIN_INGRP_VAL_APS_SWCLK_IO26        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO28_BITSVAL            (PIN_INGRP_VAL_APS_SWCLK_IO28        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO30_BITSVAL            (PIN_INGRP_VAL_APS_SWCLK_IO30        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO32_BITSVAL            (PIN_INGRP_VAL_APS_SWCLK_IO32        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_IO34_BITSVAL            (PIN_INGRP_VAL_APS_SWCLK_IO34        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_LOW_BITSVAL             (PIN_INGRP_VAL_APS_SWCLK_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_APS_SWCLK_HIGH_BITSVAL            (PIN_INGRP_VAL_APS_SWCLK_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO2_BITSVAL              (PIN_INGRP_VAL_MSQ_SWDI_IO2          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO6_BITSVAL              (PIN_INGRP_VAL_MSQ_SWDI_IO6          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO8_BITSVAL              (PIN_INGRP_VAL_MSQ_SWDI_IO8          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO14_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_IO14         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO17_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_IO17         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO19_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_IO19         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO20_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_IO20         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO22_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_IO22         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO24_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_IO24         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO26_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_IO26         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO28_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_IO28         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO30_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_IO30         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_IO32_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_IO32         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_LOW_BITSVAL              (PIN_INGRP_VAL_MSQ_SWDI_LOW          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWDI_HIGH_BITSVAL             (PIN_INGRP_VAL_MSQ_SWDI_HIGH         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO0_BITSVAL             (PIN_INGRP_VAL_MSQ_SWCLK_IO0         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO1_BITSVAL             (PIN_INGRP_VAL_MSQ_SWCLK_IO1         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO4_BITSVAL             (PIN_INGRP_VAL_MSQ_SWCLK_IO4         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO7_BITSVAL             (PIN_INGRP_VAL_MSQ_SWCLK_IO7         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO15_BITSVAL            (PIN_INGRP_VAL_MSQ_SWCLK_IO15        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO16_BITSVAL            (PIN_INGRP_VAL_MSQ_SWCLK_IO16        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO18_BITSVAL            (PIN_INGRP_VAL_MSQ_SWCLK_IO18        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO23_BITSVAL            (PIN_INGRP_VAL_MSQ_SWCLK_IO23        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO25_BITSVAL            (PIN_INGRP_VAL_MSQ_SWCLK_IO25        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO27_BITSVAL            (PIN_INGRP_VAL_MSQ_SWCLK_IO27        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO29_BITSVAL            (PIN_INGRP_VAL_MSQ_SWCLK_IO29        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO31_BITSVAL            (PIN_INGRP_VAL_MSQ_SWCLK_IO31        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_IO33_BITSVAL            (PIN_INGRP_VAL_MSQ_SWCLK_IO33        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_LOW_BITSVAL             (PIN_INGRP_VAL_MSQ_SWCLK_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_MSQ_SWCLK_HIGH_BITSVAL            (PIN_INGRP_VAL_MSQ_SWCLK_HIGH        << PIN_INGRP_VAL_BIT)

#define PIN_INGRP_VAL_SPI0_RXD0_IO8_BITSVAL             (PIN_INGRP_VAL_SPI0_RXD0_IO8         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD0_IO14_BITSVAL            (PIN_INGRP_VAL_SPI0_RXD0_IO14        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD0_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI0_RXD0_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD0_LOW_BITSVAL             (PIN_INGRP_VAL_SPI0_RXD0_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD1_IO7_BITSVAL             (PIN_INGRP_VAL_SPI0_RXD1_IO7         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD1_IO15_BITSVAL            (PIN_INGRP_VAL_SPI0_RXD1_IO15        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD1_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI0_RXD1_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD1_LOW_BITSVAL             (PIN_INGRP_VAL_SPI0_RXD1_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD2_IO5_BITSVAL             (PIN_INGRP_VAL_SPI0_RXD2_IO5         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD2_IO10_BITSVAL            (PIN_INGRP_VAL_SPI0_RXD2_IO10        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD2_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI0_RXD2_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD2_LOW_BITSVAL             (PIN_INGRP_VAL_SPI0_RXD2_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD3_IO4_BITSVAL             (PIN_INGRP_VAL_SPI0_RXD3_IO4         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD3_IO11_BITSVAL            (PIN_INGRP_VAL_SPI0_RXD3_IO11        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD3_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI0_RXD3_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI0_RXD3_LOW_BITSVAL             (PIN_INGRP_VAL_SPI0_RXD3_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD0_IO14_BITSVAL            (PIN_INGRP_VAL_SPI1_RXD0_IO14        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD0_IO8_BITSVAL             (PIN_INGRP_VAL_SPI1_RXD0_IO8         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD0_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI1_RXD0_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD0_LOW_BITSVAL             (PIN_INGRP_VAL_SPI1_RXD0_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD1_IO15_BITSVAL            (PIN_INGRP_VAL_SPI1_RXD1_IO15        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD1_IO7_BITSVAL             (PIN_INGRP_VAL_SPI1_RXD1_IO7         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD1_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI1_RXD1_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD1_LOW_BITSVAL             (PIN_INGRP_VAL_SPI1_RXD1_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD2_IO10_BITSVAL            (PIN_INGRP_VAL_SPI1_RXD2_IO10        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD2_IO5_BITSVAL             (PIN_INGRP_VAL_SPI1_RXD2_IO5         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD2_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI1_RXD2_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD2_LOW_BITSVAL             (PIN_INGRP_VAL_SPI1_RXD2_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD3_IO11_BITSVAL            (PIN_INGRP_VAL_SPI1_RXD3_IO11        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD3_IO4_BITSVAL             (PIN_INGRP_VAL_SPI1_RXD3_IO4         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD3_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI1_RXD3_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI1_RXD3_LOW_BITSVAL             (PIN_INGRP_VAL_SPI1_RXD3_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_CLK_IO1_BITSVAL              (PIN_INGRP_VAL_SPI2_CLK_IO1          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_CLK_IO24_BITSVAL             (PIN_INGRP_VAL_SPI2_CLK_IO24         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_CLK_IO32_BITSVAL             (PIN_INGRP_VAL_SPI2_CLK_IO32         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_CLK_HIGH_BITSVAL             (PIN_INGRP_VAL_SPI2_CLK_HIGH         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_CLK_LOW_BITSVAL              (PIN_INGRP_VAL_SPI2_CLK_LOW          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_CS_IN_IO22_BITSVAL           (PIN_INGRP_VAL_SPI2_CS_IN_IO22       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_CS_IN_IO25_BITSVAL           (PIN_INGRP_VAL_SPI2_CS_IN_IO25       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_CS_IN_IO33_BITSVAL           (PIN_INGRP_VAL_SPI2_CS_IN_IO33       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_CS_IN_HIGH_BITSVAL           (PIN_INGRP_VAL_SPI2_CS_IN_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_CS_IN_LOW_BITSVAL            (PIN_INGRP_VAL_SPI2_CS_IN_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_RXD_IO26_BITSVAL             (PIN_INGRP_VAL_SPI2_RXD_IO26         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_RXD_IO28_BITSVAL             (PIN_INGRP_VAL_SPI2_RXD_IO28         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_RXD_IO29_BITSVAL             (PIN_INGRP_VAL_SPI2_RXD_IO29         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_RXD_IO32_BITSVAL             (PIN_INGRP_VAL_SPI2_RXD_IO32         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_RXD_IO33_BITSVAL             (PIN_INGRP_VAL_SPI2_RXD_IO33         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_RXD_IO36_BITSVAL             (PIN_INGRP_VAL_SPI2_RXD_IO36         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_RXD_HIGH_BITSVAL             (PIN_INGRP_VAL_SPI2_RXD_HIGH         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI2_RXD_LOW_BITSVAL              (PIN_INGRP_VAL_SPI2_RXD_LOW          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD0_IO5_BITSVAL             (PIN_INGRP_VAL_SPI3_RXD0_IO5         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD0_IO17_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD0_IO17        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD0_IO27_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD0_IO27        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD0_IO29_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD0_IO29        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD0_IO35_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD0_IO35        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD0_IO36_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD0_IO36        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD0_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD0_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD0_LOW_BITSVAL             (PIN_INGRP_VAL_SPI3_RXD0_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD1_IO4_BITSVAL             (PIN_INGRP_VAL_SPI3_RXD1_IO4         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD1_IO16_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD1_IO16        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD1_IO29_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD1_IO29        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD1_IO33_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD1_IO33        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD1_IO34_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD1_IO34        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD1_IO36_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD1_IO36        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD1_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD1_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD1_LOW_BITSVAL             (PIN_INGRP_VAL_SPI3_RXD1_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD2_IO2_BITSVAL             (PIN_INGRP_VAL_SPI3_RXD2_IO2         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD2_IO23_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD2_IO23        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD2_IO28_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD2_IO28        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD2_IO32_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD2_IO32        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD2_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD2_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD2_LOW_BITSVAL             (PIN_INGRP_VAL_SPI3_RXD2_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD3_IO0_BITSVAL             (PIN_INGRP_VAL_SPI3_RXD3_IO0         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD3_IO1_BITSVAL             (PIN_INGRP_VAL_SPI3_RXD3_IO1         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD3_IO31_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD3_IO31        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD3_HIGH_BITSVAL            (PIN_INGRP_VAL_SPI3_RXD3_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_SPI3_RXD3_LOW_BITSVAL             (PIN_INGRP_VAL_SPI3_RXD3_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_OTP_OPM_MODE_IO11_BITSVAL         (PIN_INGRP_VAL_OTP_OPM_MODE_IO11     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_OTP_OPM_MODE_IO31_BITSVAL         (PIN_INGRP_VAL_OTP_OPM_MODE_IO31     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_OTP_OPM_MODE_IO33_BITSVAL         (PIN_INGRP_VAL_OTP_OPM_MODE_IO33     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_OTP_OPM_MODE_IO37_BITSVAL         (PIN_INGRP_VAL_OTP_OPM_MODE_IO37     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_OTP_OPM_MODE_HIGH_BITSVAL         (PIN_INGRP_VAL_OTP_OPM_MODE_HIGH     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_OTP_OPM_MODE_LOW_BITSVAL          (PIN_INGRP_VAL_OTP_OPM_MODE_LOW      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_IO4_BITSVAL              (PIN_INGRP_VAL_I2S_SDI0_IO4          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_IO8_BITSVAL              (PIN_INGRP_VAL_I2S_SDI0_IO8          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_IO12_BITSVAL             (PIN_INGRP_VAL_I2S_SDI0_IO12         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_IO17_BITSVAL             (PIN_INGRP_VAL_I2S_SDI0_IO17         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_IO20_BITSVAL             (PIN_INGRP_VAL_I2S_SDI0_IO20         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_IO22_BITSVAL             (PIN_INGRP_VAL_I2S_SDI0_IO22         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_IO26_BITSVAL             (PIN_INGRP_VAL_I2S_SDI0_IO26         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_IO29_BITSVAL             (PIN_INGRP_VAL_I2S_SDI0_IO29         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_IO30_BITSVAL             (PIN_INGRP_VAL_I2S_SDI0_IO30         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_IO36_BITSVAL             (PIN_INGRP_VAL_I2S_SDI0_IO36         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_HIGH_BITSVAL             (PIN_INGRP_VAL_I2S_SDI0_HIGH         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_I2S_SDI0_LOW_BITSVAL              (PIN_INGRP_VAL_I2S_SDI0_LOW          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PDM_RXD_IO7_BITSVAL               (PIN_INGRP_VAL_PDM_RXD_IO7           << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PDM_RXD_IO11_BITSVAL              (PIN_INGRP_VAL_PDM_RXD_IO11          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PDM_RXD_IO13_BITSVAL              (PIN_INGRP_VAL_PDM_RXD_IO13          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PDM_RXD_IO25_BITSVAL              (PIN_INGRP_VAL_PDM_RXD_IO25          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PDM_RXD_IO26_BITSVAL              (PIN_INGRP_VAL_PDM_RXD_IO26          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PDM_RXD_IO35_BITSVAL              (PIN_INGRP_VAL_PDM_RXD_IO35          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PDM_RXD_IO36_BITSVAL              (PIN_INGRP_VAL_PDM_RXD_IO36          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PDM_RXD_HIGH_BITSVAL              (PIN_INGRP_VAL_PDM_RXD_HIGH          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PDM_RXD_LOW_BITSVAL               (PIN_INGRP_VAL_PDM_RXD_LOW           << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO4_BITSVAL        (PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO4    << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO22_BITSVAL       (PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO22   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO23_BITSVAL       (PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO23   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO35_BITSVAL       (PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO35   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO36_BITSVAL       (PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO36   << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_EXT_2IO_CLK_EN_LOW_BITSVAL        (PIN_INGRP_VAL_EXT_2IO_CLK_EN_LOW    << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_PCLK_IO1_BITSVAL              (PIN_INGRP_VAL_CAM_PCLK_IO1          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_PCLK_IO15_BITSVAL             (PIN_INGRP_VAL_CAM_PCLK_IO15         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_PCLK_IO31_BITSVAL             (PIN_INGRP_VAL_CAM_PCLK_IO31         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_PCLK_HIGH_BITSVAL             (PIN_INGRP_VAL_CAM_PCLK_HIGH         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_PCLK_LOW_BITSVAL              (PIN_INGRP_VAL_CAM_PCLK_LOW          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_HREF_IO11_BITSVAL             (PIN_INGRP_VAL_CAM_HREF_IO11         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_HREF_IO21_BITSVAL             (PIN_INGRP_VAL_CAM_HREF_IO21         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_HREF_HIGH_BITSVAL             (PIN_INGRP_VAL_CAM_HREF_HIGH         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_HREF_LOW_BITSVAL              (PIN_INGRP_VAL_CAM_HREF_LOW          << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_VSYNC_IN_IO12_BITSVAL         (PIN_INGRP_VAL_CAM_VSYNC_IN_IO12     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_VSYNC_IN_IO26_BITSVAL         (PIN_INGRP_VAL_CAM_VSYNC_IN_IO26     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_VSYNC_IN_HIGH_BITSVAL         (PIN_INGRP_VAL_CAM_VSYNC_IN_HIGH     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_VSYNC_IN_LOW_BITSVAL          (PIN_INGRP_VAL_CAM_VSYNC_IN_LOW      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_HSYNC_IN_IO15_BITSVAL         (PIN_INGRP_VAL_CAM_HSYNC_IN_IO15     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_HSYNC_IN_IO24_BITSVAL         (PIN_INGRP_VAL_CAM_HSYNC_IN_IO24     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_HSYNC_IN_HIGH_BITSVAL         (PIN_INGRP_VAL_CAM_HSYNC_IN_HIGH     << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_HSYNC_IN_LOW_BITSVAL          (PIN_INGRP_VAL_CAM_HSYNC_IN_LOW      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA0_IO3_BITSVAL             (PIN_INGRP_VAL_CAM_DATA0_IO3         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA0_HIGH_BITSVAL            (PIN_INGRP_VAL_CAM_DATA0_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA0_LOW_BITSVAL             (PIN_INGRP_VAL_CAM_DATA0_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA1_IO4_BITSVAL             (PIN_INGRP_VAL_CAM_DATA1_IO4         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA1_HIGH_BITSVAL            (PIN_INGRP_VAL_CAM_DATA1_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA1_LOW_BITSVAL             (PIN_INGRP_VAL_CAM_DATA1_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA2_IO5_BITSVAL             (PIN_INGRP_VAL_CAM_DATA2_IO5         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA2_IO32_BITSVAL            (PIN_INGRP_VAL_CAM_DATA2_IO32        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA2_HIGH_BITSVAL            (PIN_INGRP_VAL_CAM_DATA2_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA2_LOW_BITSVAL             (PIN_INGRP_VAL_CAM_DATA2_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA3_IO6_BITSVAL             (PIN_INGRP_VAL_CAM_DATA3_IO6         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA3_HIGH_BITSVAL            (PIN_INGRP_VAL_CAM_DATA3_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA3_LOW_BITSVAL             (PIN_INGRP_VAL_CAM_DATA3_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA4_IO7_BITSVAL             (PIN_INGRP_VAL_CAM_DATA4_IO7         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA4_HIGH_BITSVAL            (PIN_INGRP_VAL_CAM_DATA4_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA4_LOW_BITSVAL             (PIN_INGRP_VAL_CAM_DATA4_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA5_IO8_BITSVAL             (PIN_INGRP_VAL_CAM_DATA5_IO8         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA5_IO33_BITSVAL            (PIN_INGRP_VAL_CAM_DATA5_IO33        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA5_HIGH_BITSVAL            (PIN_INGRP_VAL_CAM_DATA5_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA5_LOW_BITSVAL             (PIN_INGRP_VAL_CAM_DATA5_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA6_IO9_BITSVAL             (PIN_INGRP_VAL_CAM_DATA6_IO9         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA6_HIGH_BITSVAL            (PIN_INGRP_VAL_CAM_DATA6_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA6_LOW_BITSVAL             (PIN_INGRP_VAL_CAM_DATA6_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA7_IO10_BITSVAL            (PIN_INGRP_VAL_CAM_DATA7_IO10        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA7_IO20_BITSVAL            (PIN_INGRP_VAL_CAM_DATA7_IO20        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA7_HIGH_BITSVAL            (PIN_INGRP_VAL_CAM_DATA7_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA7_LOW_BITSVAL             (PIN_INGRP_VAL_CAM_DATA7_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA8_IO27_BITSVAL            (PIN_INGRP_VAL_CAM_DATA8_IO27        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA8_HIGH_BITSVAL            (PIN_INGRP_VAL_CAM_DATA8_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA8_LOW_BITSVAL             (PIN_INGRP_VAL_CAM_DATA8_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA9_IO28_BITSVAL            (PIN_INGRP_VAL_CAM_DATA9_IO28        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA9_HIGH_BITSVAL            (PIN_INGRP_VAL_CAM_DATA9_HIGH        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA9_LOW_BITSVAL             (PIN_INGRP_VAL_CAM_DATA9_LOW         << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA10_IO29_BITSVAL           (PIN_INGRP_VAL_CAM_DATA10_IO29       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA10_HIGH_BITSVAL           (PIN_INGRP_VAL_CAM_DATA10_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA10_LOW_BITSVAL            (PIN_INGRP_VAL_CAM_DATA10_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA11_IO30_BITSVAL           (PIN_INGRP_VAL_CAM_DATA11_IO30       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA11_HIGH_BITSVAL           (PIN_INGRP_VAL_CAM_DATA11_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA11_LOW_BITSVAL            (PIN_INGRP_VAL_CAM_DATA11_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA12_IO34_BITSVAL           (PIN_INGRP_VAL_CAM_DATA12_IO34       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA12_HIGH_BITSVAL           (PIN_INGRP_VAL_CAM_DATA12_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA12_LOW_BITSVAL            (PIN_INGRP_VAL_CAM_DATA12_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA13_IO35_BITSVAL           (PIN_INGRP_VAL_CAM_DATA13_IO35       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA13_HIGH_BITSVAL           (PIN_INGRP_VAL_CAM_DATA13_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA13_LOW_BITSVAL            (PIN_INGRP_VAL_CAM_DATA13_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA14_IO36_BITSVAL           (PIN_INGRP_VAL_CAM_DATA14_IO36       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA14_HIGH_BITSVAL           (PIN_INGRP_VAL_CAM_DATA14_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA14_LOW_BITSVAL            (PIN_INGRP_VAL_CAM_DATA14_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA15_IO37_BITSVAL           (PIN_INGRP_VAL_CAM_DATA15_IO37       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA15_HIGH_BITSVAL           (PIN_INGRP_VAL_CAM_DATA15_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CAM_DATA15_LOW_BITSVAL            (PIN_INGRP_VAL_CAM_DATA15_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD0_IO39_BITSVAL           (PIN_INGRP_VAL_PSRAM_RXD0_IO39       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD0_HIGH_BITSVAL           (PIN_INGRP_VAL_PSRAM_RXD0_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD0_LOW_BITSVAL            (PIN_INGRP_VAL_PSRAM_RXD0_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD1_IO40_BITSVAL           (PIN_INGRP_VAL_PSRAM_RXD1_IO40       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD1_HIGH_BITSVAL           (PIN_INGRP_VAL_PSRAM_RXD1_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD1_LOW_BITSVAL            (PIN_INGRP_VAL_PSRAM_RXD1_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD2_IO41_BITSVAL           (PIN_INGRP_VAL_PSRAM_RXD2_IO41       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD2_HIGH_BITSVAL           (PIN_INGRP_VAL_PSRAM_RXD2_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD2_LOW_BITSVAL            (PIN_INGRP_VAL_PSRAM_RXD2_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD3_IO42_BITSVAL           (PIN_INGRP_VAL_PSRAM_RXD3_IO42       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD3_HIGH_BITSVAL           (PIN_INGRP_VAL_PSRAM_RXD3_HIGH       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_PSRAM_RXD3_LOW_BITSVAL            (PIN_INGRP_VAL_PSRAM_RXD3_LOW        << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_LOW_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_APS_LOW       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO4_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_APS_IO4       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO5_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_APS_IO5       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO6_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_APS_IO6       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO7_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_APS_IO7       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO8_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_APS_IO8       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO9_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_APS_IO9       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO10_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_APS_IO10      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO11_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_APS_IO11      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO18_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_APS_IO18      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO19_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_APS_IO19      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO34_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_APS_IO34      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO35_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_APS_IO35      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_IO38_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_APS_IO38      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_APS_HIGH_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_APS_HIGH      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_LOW_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_MSQ_LOW       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO4_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_MSQ_IO4       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO5_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_MSQ_IO5       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO6_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_MSQ_IO6       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO7_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_MSQ_IO7       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO8_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_MSQ_IO8       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO9_BITSVAL           (PIN_INGRP_VAL_CLK_EXT_MSQ_IO9       << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO10_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_MSQ_IO10      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO11_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_MSQ_IO11      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO18_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_MSQ_IO18      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO19_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_MSQ_IO19      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO34_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_MSQ_IO34      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO35_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_MSQ_IO35      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_IO38_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_MSQ_IO38      << PIN_INGRP_VAL_BIT)
#define PIN_INGRP_VAL_CLK_EXT_MSQ_HIGH_BITSVAL          (PIN_INGRP_VAL_CLK_EXT_MSQ_HIGH      << PIN_INGRP_VAL_BIT)

/* S_GPIO_PIN_CFG [38:28] OutGrpOfst.  */
#define PIN_OUTGRP_OFST_IO0                             864ULL      /* Output group offset. RegOffset=0x6C, Bit offset=0  */
#define PIN_OUTGRP_OFST_IO1                             865ULL      /* Output group offset. RegOffset=0x6C, Bit offset=4  */
#define PIN_OUTGRP_OFST_IO2                             866ULL      /* Output group offset. RegOffset=0x6C, Bit offset=8  */
#define PIN_OUTGRP_OFST_IO3                             867ULL      /* Output group offset. RegOffset=0x6C, Bit offset=12 */
#define PIN_OUTGRP_OFST_IO4                             868ULL      /* Output group offset. RegOffset=0x6C, Bit offset=16 */
#define PIN_OUTGRP_OFST_IO5                             869ULL      /* Output group offset. RegOffset=0x6C, Bit offset=20 */
#define PIN_OUTGRP_OFST_IO6                             870ULL      /* Output group offset. RegOffset=0x6C, Bit offset=24 */
#define PIN_OUTGRP_OFST_IO7                             871ULL      /* Output group offset. RegOffset=0x6C, Bit offset=28 */
#define PIN_OUTGRP_OFST_IO8                             896ULL      /* Output group offset. RegOffset=0x70, Bit offset=0  */
#define PIN_OUTGRP_OFST_IO9                             897ULL      /* Output group offset. RegOffset=0x70, Bit offset=4  */
#define PIN_OUTGRP_OFST_IO10                            898ULL      /* Output group offset. RegOffset=0x70, Bit offset=8  */
#define PIN_OUTGRP_OFST_IO11                            899ULL      /* Output group offset. RegOffset=0x70, Bit offset=12 */
#define PIN_OUTGRP_OFST_IO12                            900ULL      /* Output group offset. RegOffset=0x70, Bit offset=16 */
#define PIN_OUTGRP_OFST_IO13                            901ULL      /* Output group offset. RegOffset=0x70, Bit offset=20 */
#define PIN_OUTGRP_OFST_IO14                            902ULL      /* Output group offset. RegOffset=0x70, Bit offset=24 */
#define PIN_OUTGRP_OFST_IO15                            903ULL      /* Output group offset. RegOffset=0x70, Bit offset=28 */
#define PIN_OUTGRP_OFST_IO16                            928ULL      /* Output group offset. RegOffset=0x74, Bit offset=0  */
#define PIN_OUTGRP_OFST_IO17                            929ULL      /* Output group offset. RegOffset=0x74, Bit offset=4  */
#define PIN_OUTGRP_OFST_IO18                            930ULL      /* Output group offset. RegOffset=0x74, Bit offset=8  */
#define PIN_OUTGRP_OFST_IO19                            931ULL      /* Output group offset. RegOffset=0x74, Bit offset=12 */
#define PIN_OUTGRP_OFST_IO20                            932ULL      /* Output group offset. RegOffset=0x74, Bit offset=16 */
#define PIN_OUTGRP_OFST_IO21                            933ULL      /* Output group offset. RegOffset=0x74, Bit offset=20 */
#define PIN_OUTGRP_OFST_IO22                            934ULL      /* Output group offset. RegOffset=0x74, Bit offset=24 */
#define PIN_OUTGRP_OFST_IO23                            935ULL      /* Output group offset. RegOffset=0x74, Bit offset=28 */
#define PIN_OUTGRP_OFST_IO24                            960ULL      /* Output group offset. RegOffset=0x78, Bit offset=0  */
#define PIN_OUTGRP_OFST_IO25                            961ULL      /* Output group offset. RegOffset=0x78, Bit offset=4  */
#define PIN_OUTGRP_OFST_IO26                            962ULL      /* Output group offset. RegOffset=0x78, Bit offset=8  */
#define PIN_OUTGRP_OFST_IO27                            963ULL      /* Output group offset. RegOffset=0x78, Bit offset=12 */
#define PIN_OUTGRP_OFST_IO28                            964ULL      /* Output group offset. RegOffset=0x78, Bit offset=16 */
#define PIN_OUTGRP_OFST_IO29                            965ULL      /* Output group offset. RegOffset=0x78, Bit offset=20 */
#define PIN_OUTGRP_OFST_IO30                            966ULL      /* Output group offset. RegOffset=0x78, Bit offset=24 */
#define PIN_OUTGRP_OFST_IO31                            967ULL      /* Output group offset. RegOffset=0x78, Bit offset=28 */
#define PIN_OUTGRP_OFST_IO32                            992ULL      /* Output group offset. RegOffset=0x7C, Bit offset=0  */
#define PIN_OUTGRP_OFST_IO33                            993ULL      /* Output group offset. RegOffset=0x7C, Bit offset=4  */
#define PIN_OUTGRP_OFST_IO34                            994ULL      /* Output group offset. RegOffset=0x7C, Bit offset=8  */
#define PIN_OUTGRP_OFST_IO35                            995ULL      /* Output group offset. RegOffset=0x7C, Bit offset=12 */
#define PIN_OUTGRP_OFST_IO36                            996ULL      /* Output group offset. RegOffset=0x7C, Bit offset=16 */
#define PIN_OUTGRP_OFST_IO37                            997ULL      /* Output group offset. RegOffset=0x7C, Bit offset=20 */
#define PIN_OUTGRP_OFST_IO38                            998ULL      /* Output group offset. RegOffset=0x7C, Bit offset=24 */
#define PIN_OUTGRP_OFST_IO39                            999ULL      /* Output group offset. RegOffset=0x7C, Bit offset=28 */
#define PIN_OUTGRP_OFST_IO40                            1025ULL     /* Output group offset. RegOffset=0x80, Bit offset=4  */
#define PIN_OUTGRP_OFST_IO41                            1026ULL     /* Output group offset. RegOffset=0x80, Bit offset=8  */
#define PIN_OUTGRP_OFST_IO42                            1027ULL     /* Output group offset. RegOffset=0x80, Bit offset=12 */
#define PIN_OUTGRP_OFST_IO43                            1028ULL     /* Output group offset. RegOffset=0x80, Bit offset=16 */
#define PIN_OUTGRP_OFST_IO44                            1029ULL     /* Output group offset. RegOffset=0x80, Bit offset=20 */

#define PIN_OUTGRP_OFST_IO0_BITSVAL                     (PIN_OUTGRP_OFST_IO0  << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO1_BITSVAL                     (PIN_OUTGRP_OFST_IO1  << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO2_BITSVAL                     (PIN_OUTGRP_OFST_IO2  << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO3_BITSVAL                     (PIN_OUTGRP_OFST_IO3  << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO4_BITSVAL                     (PIN_OUTGRP_OFST_IO4  << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO5_BITSVAL                     (PIN_OUTGRP_OFST_IO5  << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO6_BITSVAL                     (PIN_OUTGRP_OFST_IO6  << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO7_BITSVAL                     (PIN_OUTGRP_OFST_IO7  << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO8_BITSVAL                     (PIN_OUTGRP_OFST_IO8  << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO9_BITSVAL                     (PIN_OUTGRP_OFST_IO9  << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO10_BITSVAL                    (PIN_OUTGRP_OFST_IO10 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO11_BITSVAL                    (PIN_OUTGRP_OFST_IO11 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO12_BITSVAL                    (PIN_OUTGRP_OFST_IO12 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO13_BITSVAL                    (PIN_OUTGRP_OFST_IO13 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO14_BITSVAL                    (PIN_OUTGRP_OFST_IO14 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO15_BITSVAL                    (PIN_OUTGRP_OFST_IO15 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO16_BITSVAL                    (PIN_OUTGRP_OFST_IO16 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO17_BITSVAL                    (PIN_OUTGRP_OFST_IO17 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO18_BITSVAL                    (PIN_OUTGRP_OFST_IO18 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO19_BITSVAL                    (PIN_OUTGRP_OFST_IO19 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO20_BITSVAL                    (PIN_OUTGRP_OFST_IO20 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO21_BITSVAL                    (PIN_OUTGRP_OFST_IO21 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO22_BITSVAL                    (PIN_OUTGRP_OFST_IO22 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO23_BITSVAL                    (PIN_OUTGRP_OFST_IO23 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO24_BITSVAL                    (PIN_OUTGRP_OFST_IO24 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO25_BITSVAL                    (PIN_OUTGRP_OFST_IO25 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO26_BITSVAL                    (PIN_OUTGRP_OFST_IO26 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO27_BITSVAL                    (PIN_OUTGRP_OFST_IO27 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO28_BITSVAL                    (PIN_OUTGRP_OFST_IO28 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO29_BITSVAL                    (PIN_OUTGRP_OFST_IO29 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO30_BITSVAL                    (PIN_OUTGRP_OFST_IO30 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO31_BITSVAL                    (PIN_OUTGRP_OFST_IO31 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO32_BITSVAL                    (PIN_OUTGRP_OFST_IO32 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO33_BITSVAL                    (PIN_OUTGRP_OFST_IO33 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO34_BITSVAL                    (PIN_OUTGRP_OFST_IO34 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO35_BITSVAL                    (PIN_OUTGRP_OFST_IO35 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO36_BITSVAL                    (PIN_OUTGRP_OFST_IO36 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO37_BITSVAL                    (PIN_OUTGRP_OFST_IO37 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO38_BITSVAL                    (PIN_OUTGRP_OFST_IO38 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO39_BITSVAL                    (PIN_OUTGRP_OFST_IO39 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO40_BITSVAL                    (PIN_OUTGRP_OFST_IO40 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO41_BITSVAL                    (PIN_OUTGRP_OFST_IO41 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO42_BITSVAL                    (PIN_OUTGRP_OFST_IO42 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO43_BITSVAL                    (PIN_OUTGRP_OFST_IO43 << PIN_OUTGRP_OFST_BIT)
#define PIN_OUTGRP_OFST_IO44_BITSVAL                    (PIN_OUTGRP_OFST_IO44 << PIN_OUTGRP_OFST_BIT)


/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO0           */
#define PIN_OUTGRP_VAL_IO0_SPI0_CS3                     0ULL
#define PIN_OUTGRP_VAL_IO0_SPI3_TXD3                    1ULL
#define PIN_OUTGRP_VAL_IO0_I2C_SCL_OUT                  2ULL
#define PIN_OUTGRP_VAL_IO0_UART0_TXD                    3ULL
#define PIN_OUTGRP_VAL_IO0_UART1_TXD                    4ULL
#define PIN_OUTGRP_VAL_IO0_DBG0                         6ULL
#define PIN_OUTGRP_VAL_IO0_APS_SWDO                     8ULL
#define PIN_OUTGRP_VAL_IO0_APS_UART_TXD                 10ULL
#define PIN_OUTGRP_VAL_IO0_MSQ_UART_TXD                 11ULL
#define PIN_OUTGRP_VAL_IO0_SPI1_CS3                     12ULL
#define PIN_OUTGRP_VAL_IO0_I2S_SCLK                     13ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO1           */
#define PIN_OUTGRP_VAL_IO1_SPI3_TXD3                    1ULL
#define PIN_OUTGRP_VAL_IO1_I2C_SCL_OUT                  2ULL
#define PIN_OUTGRP_VAL_IO1_DBG1                         6ULL
#define PIN_OUTGRP_VAL_IO1_APS_SWDO                     8ULL
#define PIN_OUTGRP_VAL_IO1_MSQ_UART_TXD                 11ULL
#define PIN_OUTGRP_VAL_IO1_PDM_CLK                      12ULL
#define PIN_OUTGRP_VAL_IO1_I2S_SCLK                     13ULL
#define PIN_OUTGRP_VAL_IO1_EXT_2IO_CLK                  14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO2           */
#define PIN_OUTGRP_VAL_IO2_SPI0_CS0                     0ULL
#define PIN_OUTGRP_VAL_IO2_SPI3_TXD2                    1ULL
#define PIN_OUTGRP_VAL_IO2_I2C_SDA_OUT                  2ULL
#define PIN_OUTGRP_VAL_IO2_DBG2                         6ULL
#define PIN_OUTGRP_VAL_IO2_MSQ_SWDO                     9ULL
#define PIN_OUTGRP_VAL_IO2_SPI1_CS0                     12ULL
#define PIN_OUTGRP_VAL_IO2_I2S_WS_OUT                   13ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO3           */
#define PIN_OUTGRP_VAL_IO3_SPI0_CS1                     0ULL
#define PIN_OUTGRP_VAL_IO3_SPI3_CS1                     1ULL
#define PIN_OUTGRP_VAL_IO3_UART0_RTS_N                  3ULL
#define PIN_OUTGRP_VAL_IO3_UART1_RTS_N                  4ULL
#define PIN_OUTGRP_VAL_IO3_DBG3                         6ULL
#define PIN_OUTGRP_VAL_IO3_TDO                          7ULL
#define PIN_OUTGRP_VAL_IO3_APS_UART_RTS_N               10ULL
#define PIN_OUTGRP_VAL_IO3_MSQ_UART_RTS_N               11ULL
#define PIN_OUTGRP_VAL_IO3_SPI1_CS1                     12ULL
#define PIN_OUTGRP_VAL_IO3_I2S_SDO0                     13ULL
#define PIN_OUTGRP_VAL_IO3_EXT_2IO_CLK                  14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO4           */
#define PIN_OUTGRP_VAL_IO4_SPI0_TXD3                    0ULL
#define PIN_OUTGRP_VAL_IO4_SPI3_TXD1                    1ULL
#define PIN_OUTGRP_VAL_IO4_I2C_SCL_OUT                  2ULL
#define PIN_OUTGRP_VAL_IO4_DBG4                         6ULL
#define PIN_OUTGRP_VAL_IO4_APS_SWDO                     8ULL
#define PIN_OUTGRP_VAL_IO4_SPI1_TXD3                    12ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO5           */
#define PIN_OUTGRP_VAL_IO5_SPI0_TXD2                    0ULL
#define PIN_OUTGRP_VAL_IO5_SPI3_TXD0                    1ULL
#define PIN_OUTGRP_VAL_IO5_I2C_SDA_OUT                  2ULL
#define PIN_OUTGRP_VAL_IO5_UART0_TXD                    3ULL
#define PIN_OUTGRP_VAL_IO5_UART1_TXD                    4ULL
#define PIN_OUTGRP_VAL_IO5_DBG5                         6ULL
#define PIN_OUTGRP_VAL_IO5_APS_UART_TXD                 10ULL
#define PIN_OUTGRP_VAL_IO5_MSQ_UART_TXD                 11ULL
#define PIN_OUTGRP_VAL_IO5_SPI1_TXD2                    12ULL
#define PIN_OUTGRP_VAL_IO5_I2S_SCLK                     13ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO6           */
#define PIN_OUTGRP_VAL_IO6_SPI0_CLK                     0ULL
#define PIN_OUTGRP_VAL_IO6_PDM_CLK                      1ULL
#define PIN_OUTGRP_VAL_IO6_UART0_RTS_N                  3ULL
#define PIN_OUTGRP_VAL_IO6_UART1_RTS_N                  4ULL
#define PIN_OUTGRP_VAL_IO6_DBG6                         6ULL
#define PIN_OUTGRP_VAL_IO6_TDO                          7ULL
#define PIN_OUTGRP_VAL_IO6_MSQ_SWDO                     9ULL
#define PIN_OUTGRP_VAL_IO6_APS_UART_RTS_N               10ULL
#define PIN_OUTGRP_VAL_IO6_MSQ_UART_RTS_N               11ULL
#define PIN_OUTGRP_VAL_IO6_SPI1_CLK                     12ULL
#define PIN_OUTGRP_VAL_IO6_I2S_WS_OUT                   13ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO7           */
#define PIN_OUTGRP_VAL_IO7_SPI0_TXD1                    0ULL
#define PIN_OUTGRP_VAL_IO7_UART0_TXD                    3ULL
#define PIN_OUTGRP_VAL_IO7_UART1_TXD                    4ULL
#define PIN_OUTGRP_VAL_IO7_DBG7                         6ULL
#define PIN_OUTGRP_VAL_IO7_APS_SWDO                     8ULL
#define PIN_OUTGRP_VAL_IO7_APS_UART_TXD                 10ULL
#define PIN_OUTGRP_VAL_IO7_MSQ_UART_TXD                 11ULL
#define PIN_OUTGRP_VAL_IO7_SPI1_TXD1                    12ULL
#define PIN_OUTGRP_VAL_IO7_I2S_SDO0                     13ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO8           */
#define PIN_OUTGRP_VAL_IO8_SPI0_TXD0                    0ULL
#define PIN_OUTGRP_VAL_IO8_SPI3_CS0                     1ULL
#define PIN_OUTGRP_VAL_IO8_I2C_SCL_OUT                  2ULL
#define PIN_OUTGRP_VAL_IO8_DBG8                         6ULL
#define PIN_OUTGRP_VAL_IO8_MSQ_SWDO                     9ULL
#define PIN_OUTGRP_VAL_IO8_SPI1_TXD0                    12ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO9           */
#define PIN_OUTGRP_VAL_IO9_SPI0_CS2                     0ULL
#define PIN_OUTGRP_VAL_IO9_SPI3_CLK                     1ULL
#define PIN_OUTGRP_VAL_IO9_I2C_SDA_OUT                  2ULL
#define PIN_OUTGRP_VAL_IO9_DBG9                         6ULL
#define PIN_OUTGRP_VAL_IO9_TDO                          7ULL
#define PIN_OUTGRP_VAL_IO9_SPI1_CS2                     12ULL
#define PIN_OUTGRP_VAL_IO9_I2S_SDO0                     13ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO10          */
#define PIN_OUTGRP_VAL_IO10_SPI1_TXD2                   0ULL
#define PIN_OUTGRP_VAL_IO10_OTP_PROGRAM_PERIOD          1ULL
#define PIN_OUTGRP_VAL_IO10_I2C_SCL_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO10_UART0_TXD                   3ULL
#define PIN_OUTGRP_VAL_IO10_DBG10                       6ULL
#define PIN_OUTGRP_VAL_IO10_PDM_CLK                     8ULL
#define PIN_OUTGRP_VAL_IO10_SPI0_TXD2                   12ULL
#define PIN_OUTGRP_VAL_IO10_I2S_SCLK                    13ULL
#define PIN_OUTGRP_VAL_IO10_EXT_2IO_CLK                 14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO11          */
#define PIN_OUTGRP_VAL_IO11_SPI1_TXD3                   0ULL
#define PIN_OUTGRP_VAL_IO11_I2C_SDA_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO11_UART1_TXD                   4ULL
#define PIN_OUTGRP_VAL_IO11_DBG11                       6ULL
#define PIN_OUTGRP_VAL_IO11_TDO                         7ULL
#define PIN_OUTGRP_VAL_IO11_MSQ_UART_RTS_N              11ULL
#define PIN_OUTGRP_VAL_IO11_SPI0_TXD3                   12ULL
#define PIN_OUTGRP_VAL_IO11_I2S_WS_OUT                  13ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO12          */
#define PIN_OUTGRP_VAL_IO12_SPI1_CS0                    0ULL
#define PIN_OUTGRP_VAL_IO12_PDM_CLK                     1ULL
#define PIN_OUTGRP_VAL_IO12_DBG12                       6ULL
#define PIN_OUTGRP_VAL_IO12_APS_UART_TXD                10ULL
#define PIN_OUTGRP_VAL_IO12_MSQ_UART_TXD                11ULL
#define PIN_OUTGRP_VAL_IO12_SPI0_CS0                    12ULL
#define PIN_OUTGRP_VAL_IO12_PU_EPA_OUT0                 14ULL
#define PIN_OUTGRP_VAL_IO12_CAM_VSYNC_OUT               15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO13          */
#define PIN_OUTGRP_VAL_IO13_SPI1_CLK                    0ULL
#define PIN_OUTGRP_VAL_IO13_UART0_RTS_N                 3ULL
#define PIN_OUTGRP_VAL_IO13_UART1_RTS_N                 4ULL
#define PIN_OUTGRP_VAL_IO13_DBG13                       6ULL
#define PIN_OUTGRP_VAL_IO13_APS_UART_RTS_N              10ULL
#define PIN_OUTGRP_VAL_IO13_MSQ_UART_TXD                11ULL
#define PIN_OUTGRP_VAL_IO13_SPI0_CLK                    12ULL
#define PIN_OUTGRP_VAL_IO13_I2S_SDO0                    13ULL
#define PIN_OUTGRP_VAL_IO13_PU_EPA_OUT1                 14ULL
#define PIN_OUTGRP_VAL_IO13_CAM_XVCLK                   15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO14          */
#define PIN_OUTGRP_VAL_IO14_SPI1_TXD0                   0ULL
#define PIN_OUTGRP_VAL_IO14_PWM5                        1ULL
#define PIN_OUTGRP_VAL_IO14_I2C_SCL_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO14_UART0_RTS_N                 3ULL
#define PIN_OUTGRP_VAL_IO14_DBG14                       6ULL
#define PIN_OUTGRP_VAL_IO14_MSQ_SWDO                    9ULL
#define PIN_OUTGRP_VAL_IO14_APS_UART_TXD                10ULL
#define PIN_OUTGRP_VAL_IO14_SPI0_TXD0                   12ULL
#define PIN_OUTGRP_VAL_IO14_I2S_SCLK                    13ULL
#define PIN_OUTGRP_VAL_IO14_PU_EPA_OUT2                 14ULL
#define PIN_OUTGRP_VAL_IO14_CAM_SHUTTER                 15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO15          */
#define PIN_OUTGRP_VAL_IO15_SPI1_TXD1                   0ULL
#define PIN_OUTGRP_VAL_IO15_PWM4                        1ULL
#define PIN_OUTGRP_VAL_IO15_I2C_SDA_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO15_UART1_RTS_N                 4ULL
#define PIN_OUTGRP_VAL_IO15_DBG15                       6ULL
#define PIN_OUTGRP_VAL_IO15_APS_SWDO                    8ULL
#define PIN_OUTGRP_VAL_IO15_MSQ_UART_TXD                11ULL
#define PIN_OUTGRP_VAL_IO15_SPI0_TXD1                   12ULL
#define PIN_OUTGRP_VAL_IO15_SPI3_CS3                    13ULL
#define PIN_OUTGRP_VAL_IO15_PU_EPA_OUT3                 14ULL
#define PIN_OUTGRP_VAL_IO15_CAM_HSYNC_OUT               15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO16          */
#define PIN_OUTGRP_VAL_IO16_PWM3                        1ULL
#define PIN_OUTGRP_VAL_IO16_I2C_SCL_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO16_UART0_TXD                   3ULL
#define PIN_OUTGRP_VAL_IO16_DBG16                       6ULL
#define PIN_OUTGRP_VAL_IO16_APS_SWDO                    8ULL
#define PIN_OUTGRP_VAL_IO16_APS_UART_RTS_N              10ULL
#define PIN_OUTGRP_VAL_IO16_SPI3_TXD1                   12ULL
#define PIN_OUTGRP_VAL_IO16_I2S_SDO0                    13ULL
#define PIN_OUTGRP_VAL_IO16_PU_EPA_OUT0                 14ULL
#define PIN_OUTGRP_VAL_IO16_CAM_HSYNC_OUT               15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO17          */
#define PIN_OUTGRP_VAL_IO17_SPI1_CS3                    0ULL
#define PIN_OUTGRP_VAL_IO17_PWM2                        1ULL
#define PIN_OUTGRP_VAL_IO17_I2C_SDA_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO17_UART1_TXD                   4ULL
#define PIN_OUTGRP_VAL_IO17_DBG17                       6ULL
#define PIN_OUTGRP_VAL_IO17_TDO                         7ULL
#define PIN_OUTGRP_VAL_IO17_MSQ_SWDO                    9ULL
#define PIN_OUTGRP_VAL_IO17_MSQ_UART_RTS_N              11ULL
#define PIN_OUTGRP_VAL_IO17_SPI3_TXD0                   12ULL
#define PIN_OUTGRP_VAL_IO17_PU_EPA_OUT1                 14ULL
#define PIN_OUTGRP_VAL_IO17_CAM_VSYNC_OUT               15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO18          */
#define PIN_OUTGRP_VAL_IO18_SPI1_CS1                    0ULL
#define PIN_OUTGRP_VAL_IO18_PWM1                        1ULL
#define PIN_OUTGRP_VAL_IO18_I2C_SCL_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO18_UART1_TXD                   4ULL
#define PIN_OUTGRP_VAL_IO18_DBG18                       6ULL
#define PIN_OUTGRP_VAL_IO18_APS_SWDO                    8ULL
#define PIN_OUTGRP_VAL_IO18_SPI3_CLK                    12ULL
#define PIN_OUTGRP_VAL_IO18_I2S_SCLK                    13ULL
#define PIN_OUTGRP_VAL_IO18_PU_EPA_OUT0                 14ULL
#define PIN_OUTGRP_VAL_IO18_CAM_FREX                    15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO19          */
#define PIN_OUTGRP_VAL_IO19_SPI1_CS2                    0ULL
#define PIN_OUTGRP_VAL_IO19_PWM0                        1ULL
#define PIN_OUTGRP_VAL_IO19_I2C_SDA_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO19_UART0_TXD                   3ULL
#define PIN_OUTGRP_VAL_IO19_SPI0_CS1                    5ULL
#define PIN_OUTGRP_VAL_IO19_DBG19                       6ULL
#define PIN_OUTGRP_VAL_IO19_MSQ_SWDO                    9ULL
#define PIN_OUTGRP_VAL_IO19_APS_UART_TXD                10ULL
#define PIN_OUTGRP_VAL_IO19_SPI3_CS0                    12ULL
#define PIN_OUTGRP_VAL_IO19_I2S_WS_OUT                  13ULL
#define PIN_OUTGRP_VAL_IO19_PU_EPA_OUT1                 14ULL
#define PIN_OUTGRP_VAL_IO19_CAM_EXPST                   15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO20          */
#define PIN_OUTGRP_VAL_IO20_SPI0_CS2                    0ULL
#define PIN_OUTGRP_VAL_IO20_PWM0                        1ULL
#define PIN_OUTGRP_VAL_IO20_I2C_SCL_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO20_UART1_TXD                   4ULL
#define PIN_OUTGRP_VAL_IO20_SPI0_CLK                    5ULL
#define PIN_OUTGRP_VAL_IO20_DBG20                       6ULL
#define PIN_OUTGRP_VAL_IO20_TDO                         7ULL
#define PIN_OUTGRP_VAL_IO20_MSQ_SWDO                    9ULL
#define PIN_OUTGRP_VAL_IO20_SPI3_CLK                    12ULL
#define PIN_OUTGRP_VAL_IO20_PU_EPA_OUT2                 14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO21          */
#define PIN_OUTGRP_VAL_IO21_SPI0_CS3                    0ULL
#define PIN_OUTGRP_VAL_IO21_PWM1                        1ULL
#define PIN_OUTGRP_VAL_IO21_I2C_SDA_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO21_UART0_TXD                   3ULL
#define PIN_OUTGRP_VAL_IO21_SPI0_CS0                    5ULL
#define PIN_OUTGRP_VAL_IO21_DBG21                       6ULL
#define PIN_OUTGRP_VAL_IO21_APS_UART_RTS_N              10ULL
#define PIN_OUTGRP_VAL_IO21_MSQ_UART_TXD                11ULL
#define PIN_OUTGRP_VAL_IO21_SPI3_CS0                    12ULL
#define PIN_OUTGRP_VAL_IO21_I2S_SDO0                    13ULL
#define PIN_OUTGRP_VAL_IO21_PU_EPA_OUT3                 14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO22          */
#define PIN_OUTGRP_VAL_IO22_PWM2                        1ULL
#define PIN_OUTGRP_VAL_IO22_UART0_TXD                   3ULL
#define PIN_OUTGRP_VAL_IO22_UART1_TXD                   4ULL
#define PIN_OUTGRP_VAL_IO22_SPI3_CLK                    5ULL
#define PIN_OUTGRP_VAL_IO22_DBG22                       6ULL
#define PIN_OUTGRP_VAL_IO22_MSQ_SWDO                    9ULL
#define PIN_OUTGRP_VAL_IO22_APS_UART_TXD                10ULL
#define PIN_OUTGRP_VAL_IO22_PDM_CLK                     12ULL
#define PIN_OUTGRP_VAL_IO22_CAM_XVCLK                   15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO23          */
#define PIN_OUTGRP_VAL_IO23_SPI1_CS3                    0ULL
#define PIN_OUTGRP_VAL_IO23_PWM3                        1ULL
#define PIN_OUTGRP_VAL_IO23_I2C_SCL_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO23_UART0_RTS_N                 3ULL
#define PIN_OUTGRP_VAL_IO23_UART1_TXD                   4ULL
#define PIN_OUTGRP_VAL_IO23_SPI0_CS2                    5ULL
#define PIN_OUTGRP_VAL_IO23_DBG23                       6ULL
#define PIN_OUTGRP_VAL_IO23_APS_UART_TXD                10ULL
#define PIN_OUTGRP_VAL_IO23_MSQ_UART_RTS_N              11ULL
#define PIN_OUTGRP_VAL_IO23_SPI3_TXD2                   12ULL
#define PIN_OUTGRP_VAL_IO23_I2S_WS_OUT                  13ULL
#define PIN_OUTGRP_VAL_IO23_CAM_SHUTTER                 15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO24          */
#define PIN_OUTGRP_VAL_IO24_PWM4                        1ULL
#define PIN_OUTGRP_VAL_IO24_I2C_SDA_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO24_UART1_RTS_N                 4ULL
#define PIN_OUTGRP_VAL_IO24_PDM_CLK                     5ULL
#define PIN_OUTGRP_VAL_IO24_DBG24                       6ULL
#define PIN_OUTGRP_VAL_IO24_TDO                         7ULL
#define PIN_OUTGRP_VAL_IO24_CLK_DS_AUX_78K125           8ULL
#define PIN_OUTGRP_VAL_IO24_MSQ_SWDO                    9ULL
#define PIN_OUTGRP_VAL_IO24_APS_UART_TXD                10ULL
#define PIN_OUTGRP_VAL_IO24_MSQ_UART_TXD                11ULL
#define PIN_OUTGRP_VAL_IO24_SPI3_CLK                    12ULL
#define PIN_OUTGRP_VAL_IO24_I2S_SCLK                    13ULL
#define PIN_OUTGRP_VAL_IO24_CAM_HSYNC_OUT               15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO25          */
#define PIN_OUTGRP_VAL_IO25_PWM5                        1ULL
#define PIN_OUTGRP_VAL_IO25_I2C_SCL_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO25_DBG25                       6ULL
#define PIN_OUTGRP_VAL_IO25_APS_SWDO                    8ULL
#define PIN_OUTGRP_VAL_IO25_SPI3_CS0                    12ULL
#define PIN_OUTGRP_VAL_IO25_I2S_WS_OUT                  13ULL
#define PIN_OUTGRP_VAL_IO25_PU_EPA_OUT0                 14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO26          */
#define PIN_OUTGRP_VAL_IO26_SPI2_TXD                    0ULL
#define PIN_OUTGRP_VAL_IO26_PWM0                        1ULL
#define PIN_OUTGRP_VAL_IO26_I2C_SDA_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO26_UART0_RTS_N                 3ULL
#define PIN_OUTGRP_VAL_IO26_UART1_RTS_N                 4ULL
#define PIN_OUTGRP_VAL_IO26_SPI3_CS0                    5ULL
#define PIN_OUTGRP_VAL_IO26_DBG26                       6ULL
#define PIN_OUTGRP_VAL_IO26_MSQ_SWDO                    9ULL
#define PIN_OUTGRP_VAL_IO26_APS_UART_RTS_N              10ULL
#define PIN_OUTGRP_VAL_IO26_MSQ_UART_TXD                11ULL
#define PIN_OUTGRP_VAL_IO26_PU_EPA_OUT0                 14ULL
#define PIN_OUTGRP_VAL_IO26_CAM_VSYNC_OUT               15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO27          */
#define PIN_OUTGRP_VAL_IO27_SPI0_CS2                    0ULL
#define PIN_OUTGRP_VAL_IO27_PWM1                        1ULL
#define PIN_OUTGRP_VAL_IO27_I2C_SCL_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO27_SPI3_CS2                    5ULL
#define PIN_OUTGRP_VAL_IO27_DBG27                       6ULL
#define PIN_OUTGRP_VAL_IO27_APS_SWDO                    8ULL
#define PIN_OUTGRP_VAL_IO27_SPI3_TXD0                   12ULL
#define PIN_OUTGRP_VAL_IO27_I2S_SCLK                    13ULL
#define PIN_OUTGRP_VAL_IO27_PU_EPA_OUT1                 14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO28          */
#define PIN_OUTGRP_VAL_IO28_SPI2_TXD                    0ULL
#define PIN_OUTGRP_VAL_IO28_PWM2                        1ULL
#define PIN_OUTGRP_VAL_IO28_DT_DS_AUX                   2ULL
#define PIN_OUTGRP_VAL_IO28_UART0_TXD                   3ULL
#define PIN_OUTGRP_VAL_IO28_SPI3_CLK                    5ULL
#define PIN_OUTGRP_VAL_IO28_DBG28                       6ULL
#define PIN_OUTGRP_VAL_IO28_MSQ_SWDO                    9ULL
#define PIN_OUTGRP_VAL_IO28_SPI3_TXD2                   12ULL
#define PIN_OUTGRP_VAL_IO28_I2S_SDO0                    13ULL
#define PIN_OUTGRP_VAL_IO28_EXT_2IO_CLK                 14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO29          */
#define PIN_OUTGRP_VAL_IO29_SPI2_TXD                    0ULL
#define PIN_OUTGRP_VAL_IO29_PDM_CLK                     1ULL
#define PIN_OUTGRP_VAL_IO29_I2C_SDA_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO29_SPI3_TXD0                   5ULL
#define PIN_OUTGRP_VAL_IO29_DBG29                       6ULL
#define PIN_OUTGRP_VAL_IO29_APS_SWDO                    8ULL
#define PIN_OUTGRP_VAL_IO29_APS_UART_RTS_N              10ULL
#define PIN_OUTGRP_VAL_IO29_SPI3_TXD1                   12ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO30          */
#define PIN_OUTGRP_VAL_IO30_SPI1_CS1                    0ULL
#define PIN_OUTGRP_VAL_IO30_OTP_PROGRAM_PERIOD          1ULL
#define PIN_OUTGRP_VAL_IO30_I2C_SDA_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO30_UART1_RTS_N                 4ULL
#define PIN_OUTGRP_VAL_IO30_SPI0_CS3                    5ULL
#define PIN_OUTGRP_VAL_IO30_DBG30                       6ULL
#define PIN_OUTGRP_VAL_IO30_MSQ_SWDO                    9ULL
#define PIN_OUTGRP_VAL_IO30_MSQ_UART_RTS_N              11ULL
#define PIN_OUTGRP_VAL_IO30_SPI3_CS1                    12ULL
#define PIN_OUTGRP_VAL_IO30_PU_EPA_OUT2                 14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO31          */
#define PIN_OUTGRP_VAL_IO31_I2C_SCL_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO31_UART0_TXD                   3ULL
#define PIN_OUTGRP_VAL_IO31_UART1_TXD                   4ULL
#define PIN_OUTGRP_VAL_IO31_DBG31                       6ULL
#define PIN_OUTGRP_VAL_IO31_TDO                         7ULL
#define PIN_OUTGRP_VAL_IO31_APS_SWDO                    8ULL
#define PIN_OUTGRP_VAL_IO31_APS_UART_TXD                10ULL
#define PIN_OUTGRP_VAL_IO31_MSQ_UART_RTS_N              11ULL
#define PIN_OUTGRP_VAL_IO31_SPI3_TXD3                   12ULL
#define PIN_OUTGRP_VAL_IO31_I2S_SDO0                    13ULL
#define PIN_OUTGRP_VAL_IO31_PU_EPA_OUT3                 14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO32          */
#define PIN_OUTGRP_VAL_IO32_SPI2_TXD                    0ULL
#define PIN_OUTGRP_VAL_IO32_OTP_PROGRAM_PERIOD          1ULL
#define PIN_OUTGRP_VAL_IO32_I2C_SCL_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO32_UART0_TXD                   3ULL
#define PIN_OUTGRP_VAL_IO32_SPI3_TXD2                   5ULL
#define PIN_OUTGRP_VAL_IO32_DBG0                        6ULL
#define PIN_OUTGRP_VAL_IO32_MSQ_SWDO                    9ULL
#define PIN_OUTGRP_VAL_IO32_I2S_WS_OUT                  13ULL
#define PIN_OUTGRP_VAL_IO32_PU_EPA_OUT1                 14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO33          */
#define PIN_OUTGRP_VAL_IO33_SPI2_TXD                    0ULL
#define PIN_OUTGRP_VAL_IO33_I2C_SDA_OUT                 2ULL
#define PIN_OUTGRP_VAL_IO33_SPI3_TXD1                   5ULL
#define PIN_OUTGRP_VAL_IO33_DBG1                        6ULL
#define PIN_OUTGRP_VAL_IO33_TDO                         7ULL
#define PIN_OUTGRP_VAL_IO33_APS_SWDO                    8ULL
#define PIN_OUTGRP_VAL_IO33_I2S_SCLK                    13ULL
#define PIN_OUTGRP_VAL_IO33_PU_EPA_OUT2                 14ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO34          */
#define PIN_OUTGRP_VAL_IO34_UART0_RTS_N                 3ULL
#define PIN_OUTGRP_VAL_IO34_UART1_RTS_N                 4ULL
#define PIN_OUTGRP_VAL_IO34_SPI3_CS3                    5ULL
#define PIN_OUTGRP_VAL_IO34_DBG2                        6ULL
#define PIN_OUTGRP_VAL_IO34_APS_UART_TXD                10ULL
#define PIN_OUTGRP_VAL_IO34_MSQ_UART_TXD                11ULL
#define PIN_OUTGRP_VAL_IO34_SPI3_TXD1                   12ULL
#define PIN_OUTGRP_VAL_IO34_I2S_WS_OUT                  13ULL
#define PIN_OUTGRP_VAL_IO34_CAM_HSYNC_OUT               15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO35          */
#define PIN_OUTGRP_VAL_IO35_OTP_PROGRAM_PERIOD          1ULL
#define PIN_OUTGRP_VAL_IO35_UART0_TXD                   3ULL
#define PIN_OUTGRP_VAL_IO35_UART1_RTS_N                 4ULL
#define PIN_OUTGRP_VAL_IO35_SPI3_TXD0                   5ULL
#define PIN_OUTGRP_VAL_IO35_DBG3                        6ULL
#define PIN_OUTGRP_VAL_IO35_APS_SWDO                    8ULL
#define PIN_OUTGRP_VAL_IO35_MSQ_UART_RTS_N              11ULL
#define PIN_OUTGRP_VAL_IO35_I2S_SDO0                    13ULL
#define PIN_OUTGRP_VAL_IO35_CAM_VSYNC_OUT               15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO36          */
#define PIN_OUTGRP_VAL_IO36_SPI2_TXD                    0ULL
#define PIN_OUTGRP_VAL_IO36_CLK_DS_AUX_78K125           2ULL
#define PIN_OUTGRP_VAL_IO36_UART0_TXD                   3ULL
#define PIN_OUTGRP_VAL_IO36_UART1_TXD                   4ULL
#define PIN_OUTGRP_VAL_IO36_SPI3_TXD1                   5ULL
#define PIN_OUTGRP_VAL_IO36_DBG4                        6ULL
#define PIN_OUTGRP_VAL_IO36_TDO                         7ULL
#define PIN_OUTGRP_VAL_IO36_APS_UART_RTS_N              10ULL
#define PIN_OUTGRP_VAL_IO36_SPI3_TXD0                   12ULL
#define PIN_OUTGRP_VAL_IO36_CAM_FREX                    15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO37          */
#define PIN_OUTGRP_VAL_IO37_SPI1_CS1                    0ULL
#define PIN_OUTGRP_VAL_IO37_UART0_RTS_N                 3ULL
#define PIN_OUTGRP_VAL_IO37_DBG5                        6ULL
#define PIN_OUTGRP_VAL_IO37_APS_UART_RTS_N              10ULL
#define PIN_OUTGRP_VAL_IO37_MSQ_UART_RTS_N              11ULL
#define PIN_OUTGRP_VAL_IO37_SPI3_CS2                    12ULL
#define PIN_OUTGRP_VAL_IO37_I2S_SCLK                    13ULL
#define PIN_OUTGRP_VAL_IO37_PU_EPA_OUT3                 14ULL
#define PIN_OUTGRP_VAL_IO37_CAM_EXPST                   15ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO38          */
#define PIN_OUTGRP_VAL_IO38_SPI0_CS3                    0ULL
#define PIN_OUTGRP_VAL_IO38_SPI3_CLK                    1ULL
#define PIN_OUTGRP_VAL_IO38_SPI3_CS3                    2ULL
#define PIN_OUTGRP_VAL_IO38_UART0_RTS_N                 3ULL
#define PIN_OUTGRP_VAL_IO38_DBG6                        6ULL
#define PIN_OUTGRP_VAL_IO38_APS_UART_RTS_N              10ULL
#define PIN_OUTGRP_VAL_IO38_I2S_SDO0                    13ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO39          */
#define PIN_OUTGRP_VAL_IO39_SPI0_TXD0                   0ULL
#define PIN_OUTGRP_VAL_IO39_DBG12                       6ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO40          */
#define PIN_OUTGRP_VAL_IO40_SPI0_TXD1                   0ULL
#define PIN_OUTGRP_VAL_IO40_DBG12                       6ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO41          */
#define PIN_OUTGRP_VAL_IO41_SPI0_TXD2                   0ULL
#define PIN_OUTGRP_VAL_IO41_DBG12                       6ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO42          */
#define PIN_OUTGRP_VAL_IO42_SPI0_TXD3                   0ULL
#define PIN_OUTGRP_VAL_IO42_DBG12                       6ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO43          */
#define PIN_OUTGRP_VAL_IO43_SPI0_CS1                    0ULL
#define PIN_OUTGRP_VAL_IO43_SPI0_CS2                    1ULL
#define PIN_OUTGRP_VAL_IO43_SPI0_CS3                    2ULL
#define PIN_OUTGRP_VAL_IO43_UART1_RTS_N                 4ULL
#define PIN_OUTGRP_VAL_IO43_DBG15                       6ULL

/* S_GPIO_PIN_CFG [42:39] OutGrpValue. IO44          */
#define PIN_OUTGRP_VAL_IO44_SPI0_CLK                    0ULL
#define PIN_OUTGRP_VAL_IO44_UART1_TXD                   4ULL
#define PIN_OUTGRP_VAL_IO44_DBG11                       6ULL
#define PIN_OUTGRP_VAL_IO44_TDO                         7ULL

#define PIN_OUTGRP_VAL_IO0_SPI0_CS3_BITSVAL             (PIN_OUTGRP_VAL_IO0_SPI0_CS3            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO0_SPI3_TXD3_BITSVAL            (PIN_OUTGRP_VAL_IO0_SPI3_TXD3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO0_I2C_SCL_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO0_I2C_SCL_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO0_UART0_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO0_UART0_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO0_UART1_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO0_UART1_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO0_DBG0_BITSVAL                 (PIN_OUTGRP_VAL_IO0_DBG0                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO0_APS_SWDO_BITSVAL             (PIN_OUTGRP_VAL_IO0_APS_SWDO            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO0_APS_UART_TXD_BITSVAL         (PIN_OUTGRP_VAL_IO0_APS_UART_TXD        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO0_MSQ_UART_TXD_BITSVAL         (PIN_OUTGRP_VAL_IO0_MSQ_UART_TXD        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO0_SPI1_CS3_BITSVAL             (PIN_OUTGRP_VAL_IO0_SPI1_CS3            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO0_I2S_SCLK_BITSVAL             (PIN_OUTGRP_VAL_IO0_I2S_SCLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO1_SPI3_TXD3_BITSVAL            (PIN_OUTGRP_VAL_IO1_SPI3_TXD3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO1_I2C_SCL_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO1_I2C_SCL_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO1_DBG1_BITSVAL                 (PIN_OUTGRP_VAL_IO1_DBG1                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO1_APS_SWDO_BITSVAL             (PIN_OUTGRP_VAL_IO1_APS_SWDO            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO1_MSQ_UART_TXD_BITSVAL         (PIN_OUTGRP_VAL_IO1_MSQ_UART_TXD        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO1_PDM_CLK_BITSVAL              (PIN_OUTGRP_VAL_IO1_PDM_CLK             << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO1_I2S_SCLK_BITSVAL             (PIN_OUTGRP_VAL_IO1_I2S_SCLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO1_EXT_2IO_CLK_BITSVAL          (PIN_OUTGRP_VAL_IO1_EXT_2IO_CLK         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO2_SPI0_CS0_BITSVAL             (PIN_OUTGRP_VAL_IO2_SPI0_CS0            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO2_SPI3_TXD2_BITSVAL            (PIN_OUTGRP_VAL_IO2_SPI3_TXD2           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO2_I2C_SDA_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO2_I2C_SDA_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO2_DBG2_BITSVAL                 (PIN_OUTGRP_VAL_IO2_DBG2                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO2_MSQ_SWDO_BITSVAL             (PIN_OUTGRP_VAL_IO2_MSQ_SWDO            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO2_SPI1_CS0_BITSVAL             (PIN_OUTGRP_VAL_IO2_SPI1_CS0            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO2_I2S_WS_OUT_BITSVAL           (PIN_OUTGRP_VAL_IO2_I2S_WS_OUT          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_SPI0_CS1_BITSVAL             (PIN_OUTGRP_VAL_IO3_SPI0_CS1            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_SPI3_CS1_BITSVAL             (PIN_OUTGRP_VAL_IO3_SPI3_CS1            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_UART0_RTS_N_BITSVAL          (PIN_OUTGRP_VAL_IO3_UART0_RTS_N         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_UART1_RTS_N_BITSVAL          (PIN_OUTGRP_VAL_IO3_UART1_RTS_N         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_DBG3_BITSVAL                 (PIN_OUTGRP_VAL_IO3_DBG3                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_TDO_BITSVAL                  (PIN_OUTGRP_VAL_IO3_TDO                 << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_APS_UART_RTS_N_BITSVAL       (PIN_OUTGRP_VAL_IO3_APS_UART_RTS_N      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_MSQ_UART_RTS_N_BITSVAL       (PIN_OUTGRP_VAL_IO3_MSQ_UART_RTS_N      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_SPI1_CS1_BITSVAL             (PIN_OUTGRP_VAL_IO3_SPI1_CS1            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_I2S_SDO0_BITSVAL             (PIN_OUTGRP_VAL_IO3_I2S_SDO0            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO3_EXT_2IO_CLK_BITSVAL          (PIN_OUTGRP_VAL_IO3_EXT_2IO_CLK         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO4_SPI0_TXD3_BITSVAL            (PIN_OUTGRP_VAL_IO4_SPI0_TXD3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO4_SPI3_TXD1_BITSVAL            (PIN_OUTGRP_VAL_IO4_SPI3_TXD1           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO4_I2C_SCL_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO4_I2C_SCL_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO4_DBG4_BITSVAL                 (PIN_OUTGRP_VAL_IO4_DBG4                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO4_APS_SWDO_BITSVAL             (PIN_OUTGRP_VAL_IO4_APS_SWDO            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO4_SPI1_TXD3_BITSVAL            (PIN_OUTGRP_VAL_IO4_SPI1_TXD3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO5_SPI0_TXD2_BITSVAL            (PIN_OUTGRP_VAL_IO5_SPI0_TXD2           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO5_SPI3_TXD0_BITSVAL            (PIN_OUTGRP_VAL_IO5_SPI3_TXD0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO5_I2C_SDA_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO5_I2C_SDA_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO5_UART0_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO5_UART0_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO5_UART1_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO5_UART1_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO5_DBG5_BITSVAL                 (PIN_OUTGRP_VAL_IO5_DBG5                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO5_APS_UART_TXD_BITSVAL         (PIN_OUTGRP_VAL_IO5_APS_UART_TXD        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO5_MSQ_UART_TXD_BITSVAL         (PIN_OUTGRP_VAL_IO5_MSQ_UART_TXD        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO5_SPI1_TXD2_BITSVAL            (PIN_OUTGRP_VAL_IO5_SPI1_TXD2           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO5_I2S_SCLK_BITSVAL             (PIN_OUTGRP_VAL_IO5_I2S_SCLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_SPI0_CLK_BITSVAL             (PIN_OUTGRP_VAL_IO6_SPI0_CLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_PDM_CLK_BITSVAL              (PIN_OUTGRP_VAL_IO6_PDM_CLK             << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_UART0_RTS_N_BITSVAL          (PIN_OUTGRP_VAL_IO6_UART0_RTS_N         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_UART1_RTS_N_BITSVAL          (PIN_OUTGRP_VAL_IO6_UART1_RTS_N         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_DBG6_BITSVAL                 (PIN_OUTGRP_VAL_IO6_DBG6                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_TDO_BITSVAL                  (PIN_OUTGRP_VAL_IO6_TDO                 << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_MSQ_SWDO_BITSVAL             (PIN_OUTGRP_VAL_IO6_MSQ_SWDO            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_APS_UART_RTS_N_BITSVAL       (PIN_OUTGRP_VAL_IO6_APS_UART_RTS_N      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_MSQ_UART_RTS_N_BITSVAL       (PIN_OUTGRP_VAL_IO6_MSQ_UART_RTS_N      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_SPI1_CLK_BITSVAL             (PIN_OUTGRP_VAL_IO6_SPI1_CLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO6_I2S_WS_OUT_BITSVAL           (PIN_OUTGRP_VAL_IO6_I2S_WS_OUT          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO7_SPI0_TXD1_BITSVAL            (PIN_OUTGRP_VAL_IO7_SPI0_TXD1           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO7_UART0_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO7_UART0_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO7_UART1_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO7_UART1_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO7_DBG7_BITSVAL                 (PIN_OUTGRP_VAL_IO7_DBG7                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO7_APS_SWDO_BITSVAL             (PIN_OUTGRP_VAL_IO7_APS_SWDO            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO7_APS_UART_TXD_BITSVAL         (PIN_OUTGRP_VAL_IO7_APS_UART_TXD        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO7_MSQ_UART_TXD_BITSVAL         (PIN_OUTGRP_VAL_IO7_MSQ_UART_TXD        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO7_SPI1_TXD1_BITSVAL            (PIN_OUTGRP_VAL_IO7_SPI1_TXD1           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO7_I2S_SDO0_BITSVAL             (PIN_OUTGRP_VAL_IO7_I2S_SDO0            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO8_SPI0_TXD0_BITSVAL            (PIN_OUTGRP_VAL_IO8_SPI0_TXD0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO8_SPI3_CS0_BITSVAL             (PIN_OUTGRP_VAL_IO8_SPI3_CS0            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO8_I2C_SCL_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO8_I2C_SCL_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO8_DBG8_BITSVAL                 (PIN_OUTGRP_VAL_IO8_DBG8                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO8_MSQ_SWDO_BITSVAL             (PIN_OUTGRP_VAL_IO8_MSQ_SWDO            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO8_SPI1_TXD0_BITSVAL            (PIN_OUTGRP_VAL_IO8_SPI1_TXD0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO9_SPI0_CS2_BITSVAL             (PIN_OUTGRP_VAL_IO9_SPI0_CS2            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO9_SPI3_CLK_BITSVAL             (PIN_OUTGRP_VAL_IO9_SPI3_CLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO9_I2C_SDA_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO9_I2C_SDA_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO9_DBG9_BITSVAL                 (PIN_OUTGRP_VAL_IO9_DBG9                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO9_TDO_BITSVAL                  (PIN_OUTGRP_VAL_IO9_TDO                 << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO9_SPI1_CS2_BITSVAL             (PIN_OUTGRP_VAL_IO9_SPI1_CS2            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO9_I2S_SDO0_BITSVAL             (PIN_OUTGRP_VAL_IO9_I2S_SDO0            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO10_SPI1_TXD2_BITSVAL           (PIN_OUTGRP_VAL_IO10_SPI1_TXD2          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO10_OTP_PROGRAM_PERIOD_BITSVAL  (PIN_OUTGRP_VAL_IO10_OTP_PROGRAM_PERIOD << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO10_I2C_SCL_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO10_I2C_SCL_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO10_UART0_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO10_UART0_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO10_DBG10_BITSVAL               (PIN_OUTGRP_VAL_IO10_DBG10              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO10_PDM_CLK_BITSVAL             (PIN_OUTGRP_VAL_IO10_PDM_CLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO10_SPI0_TXD2_BITSVAL           (PIN_OUTGRP_VAL_IO10_SPI0_TXD2          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO10_I2S_SCLK_BITSVAL            (PIN_OUTGRP_VAL_IO10_I2S_SCLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO10_EXT_2IO_CLK_BITSVAL         (PIN_OUTGRP_VAL_IO10_EXT_2IO_CLK        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO11_SPI1_TXD3_BITSVAL           (PIN_OUTGRP_VAL_IO11_SPI1_TXD3          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO11_I2C_SDA_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO11_I2C_SDA_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO11_UART1_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO11_UART1_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO11_DBG11_BITSVAL               (PIN_OUTGRP_VAL_IO11_DBG11              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO11_TDO_BITSVAL                 (PIN_OUTGRP_VAL_IO11_TDO                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO11_MSQ_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO11_MSQ_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO11_SPI0_TXD3_BITSVAL           (PIN_OUTGRP_VAL_IO11_SPI0_TXD3          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO11_I2S_WS_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO11_I2S_WS_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO12_SPI1_CS0_BITSVAL            (PIN_OUTGRP_VAL_IO12_SPI1_CS0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO12_PDM_CLK_BITSVAL             (PIN_OUTGRP_VAL_IO12_PDM_CLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO12_DBG12_BITSVAL               (PIN_OUTGRP_VAL_IO12_DBG12              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO12_APS_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO12_APS_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO12_MSQ_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO12_MSQ_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO12_SPI0_CS0_BITSVAL            (PIN_OUTGRP_VAL_IO12_SPI0_CS0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO12_PU_EPA_OUT0_BITSVAL         (PIN_OUTGRP_VAL_IO12_PU_EPA_OUT0        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO12_CAM_VSYNC_OUT_BITSVAL       (PIN_OUTGRP_VAL_IO12_CAM_VSYNC_OUT      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO13_SPI1_CLK_BITSVAL            (PIN_OUTGRP_VAL_IO13_SPI1_CLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO13_UART0_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO13_UART0_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO13_UART1_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO13_UART1_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO13_DBG13_BITSVAL               (PIN_OUTGRP_VAL_IO13_DBG13              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO13_APS_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO13_APS_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO13_MSQ_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO13_MSQ_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO13_SPI0_CLK_BITSVAL            (PIN_OUTGRP_VAL_IO13_SPI0_CLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO13_I2S_SDO0_BITSVAL            (PIN_OUTGRP_VAL_IO13_I2S_SDO0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO13_PU_EPA_OUT1_BITSVAL         (PIN_OUTGRP_VAL_IO13_PU_EPA_OUT1        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO13_CAM_XVCLK_BITSVAL           (PIN_OUTGRP_VAL_IO13_CAM_XVCLK          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_SPI1_TXD0_BITSVAL           (PIN_OUTGRP_VAL_IO14_SPI1_TXD0          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_PWM5_BITSVAL                (PIN_OUTGRP_VAL_IO14_PWM5               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_I2C_SCL_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO14_I2C_SCL_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_UART0_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO14_UART0_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_DBG14_BITSVAL               (PIN_OUTGRP_VAL_IO14_DBG14              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_MSQ_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO14_MSQ_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_APS_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO14_APS_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_SPI0_TXD0_BITSVAL           (PIN_OUTGRP_VAL_IO14_SPI0_TXD0          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_I2S_SCLK_BITSVAL            (PIN_OUTGRP_VAL_IO14_I2S_SCLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_PU_EPA_OUT2_BITSVAL         (PIN_OUTGRP_VAL_IO14_PU_EPA_OUT2        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO14_CAM_SHUTTER_BITSVAL         (PIN_OUTGRP_VAL_IO14_CAM_SHUTTER        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_SPI1_TXD1_BITSVAL           (PIN_OUTGRP_VAL_IO15_SPI1_TXD1          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_PWM4_BITSVAL                (PIN_OUTGRP_VAL_IO15_PWM4               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_I2C_SDA_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO15_I2C_SDA_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_UART1_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO15_UART1_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_DBG15_BITSVAL               (PIN_OUTGRP_VAL_IO15_DBG15              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_APS_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO15_APS_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_MSQ_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO15_MSQ_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_SPI0_TXD1_BITSVAL           (PIN_OUTGRP_VAL_IO15_SPI0_TXD1          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_SPI3_CS3_BITSVAL            (PIN_OUTGRP_VAL_IO15_SPI3_CS3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_PU_EPA_OUT3_BITSVAL         (PIN_OUTGRP_VAL_IO15_PU_EPA_OUT3        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO15_CAM_HSYNC_OUT_BITSVAL       (PIN_OUTGRP_VAL_IO15_CAM_HSYNC_OUT      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO16_PWM3_BITSVAL                (PIN_OUTGRP_VAL_IO16_PWM3               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO16_I2C_SCL_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO16_I2C_SCL_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO16_UART0_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO16_UART0_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO16_DBG16_BITSVAL               (PIN_OUTGRP_VAL_IO16_DBG16              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO16_APS_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO16_APS_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO16_APS_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO16_APS_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO16_SPI3_TXD1_BITSVAL           (PIN_OUTGRP_VAL_IO16_SPI3_TXD1          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO16_I2S_SDO0_BITSVAL            (PIN_OUTGRP_VAL_IO16_I2S_SDO0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO16_PU_EPA_OUT0_BITSVAL         (PIN_OUTGRP_VAL_IO16_PU_EPA_OUT0        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO16_CAM_HSYNC_OUT_BITSVAL       (PIN_OUTGRP_VAL_IO16_CAM_HSYNC_OUT      << PIN_OUTGRP_VAL_BIT)

#define PIN_OUTGRP_VAL_IO17_SPI1_CS3_BITSVAL            (PIN_OUTGRP_VAL_IO17_SPI1_CS3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO17_PWM2_BITSVAL                (PIN_OUTGRP_VAL_IO17_PWM2               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO17_I2C_SDA_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO17_I2C_SDA_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO17_UART1_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO17_UART1_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO17_DBG17_BITSVAL               (PIN_OUTGRP_VAL_IO17_DBG17              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO17_TDO_BITSVAL                 (PIN_OUTGRP_VAL_IO17_TDO                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO17_MSQ_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO17_MSQ_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO17_MSQ_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO17_MSQ_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO17_SPI3_TXD0_BITSVAL           (PIN_OUTGRP_VAL_IO17_SPI3_TXD0          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO17_PU_EPA_OUT1_BITSVAL         (PIN_OUTGRP_VAL_IO17_PU_EPA_OUT1        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO17_CAM_VSYNC_OUT_BITSVAL       (PIN_OUTGRP_VAL_IO17_CAM_VSYNC_OUT      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO18_SPI1_CS1_BITSVAL            (PIN_OUTGRP_VAL_IO18_SPI1_CS1           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO18_PWM1_BITSVAL                (PIN_OUTGRP_VAL_IO18_PWM1               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO18_I2C_SCL_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO18_I2C_SCL_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO18_UART1_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO18_UART1_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO18_DBG18_BITSVAL               (PIN_OUTGRP_VAL_IO18_DBG18              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO18_APS_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO18_APS_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO18_SPI3_CLK_BITSVAL            (PIN_OUTGRP_VAL_IO18_SPI3_CLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO18_I2S_SCLK_BITSVAL            (PIN_OUTGRP_VAL_IO18_I2S_SCLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO18_PU_EPA_OUT0_BITSVAL         (PIN_OUTGRP_VAL_IO18_PU_EPA_OUT0        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO18_CAM_FREX_BITSVAL            (PIN_OUTGRP_VAL_IO18_CAM_FREX           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_SPI1_CS2_BITSVAL            (PIN_OUTGRP_VAL_IO19_SPI1_CS2           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_PWM0_BITSVAL                (PIN_OUTGRP_VAL_IO19_PWM0               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_I2C_SDA_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO19_I2C_SDA_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_UART0_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO19_UART0_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_SPI0_CS1_BITSVAL            (PIN_OUTGRP_VAL_IO19_SPI0_CS1           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_DBG19_BITSVAL               (PIN_OUTGRP_VAL_IO19_DBG19              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_MSQ_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO19_MSQ_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_APS_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO19_APS_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_SPI3_CS0_BITSVAL            (PIN_OUTGRP_VAL_IO19_SPI3_CS0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_I2S_WS_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO19_I2S_WS_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_PU_EPA_OUT1_BITSVAL         (PIN_OUTGRP_VAL_IO19_PU_EPA_OUT1        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO19_CAM_EXPST_BITSVAL           (PIN_OUTGRP_VAL_IO19_CAM_EXPST          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO20_SPI0_CS2_BITSVAL            (PIN_OUTGRP_VAL_IO20_SPI0_CS2           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO20_PWM0_BITSVAL                (PIN_OUTGRP_VAL_IO20_PWM0               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO20_I2C_SCL_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO20_I2C_SCL_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO20_UART1_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO20_UART1_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO20_SPI0_CLK_BITSVAL            (PIN_OUTGRP_VAL_IO20_SPI0_CLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO20_DBG20_BITSVAL               (PIN_OUTGRP_VAL_IO20_DBG20              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO20_TDO_BITSVAL                 (PIN_OUTGRP_VAL_IO20_TDO                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO20_MSQ_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO20_MSQ_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO20_SPI3_CLK_BITSVAL            (PIN_OUTGRP_VAL_IO20_SPI3_CLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO20_PU_EPA_OUT2_BITSVAL         (PIN_OUTGRP_VAL_IO20_PU_EPA_OUT2        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_SPI0_CS3_BITSVAL            (PIN_OUTGRP_VAL_IO21_SPI0_CS3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_PWM1_BITSVAL                (PIN_OUTGRP_VAL_IO21_PWM1               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_I2C_SDA_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO21_I2C_SDA_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_UART0_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO21_UART0_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_SPI0_CS0_BITSVAL            (PIN_OUTGRP_VAL_IO21_SPI0_CS0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_DBG21_BITSVAL               (PIN_OUTGRP_VAL_IO21_DBG21              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_APS_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO21_APS_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_MSQ_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO21_MSQ_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_SPI3_CS0_BITSVAL            (PIN_OUTGRP_VAL_IO21_SPI3_CS0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_I2S_SDO0_BITSVAL            (PIN_OUTGRP_VAL_IO21_I2S_SDO0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO21_PU_EPA_OUT3_BITSVAL         (PIN_OUTGRP_VAL_IO21_PU_EPA_OUT3        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO22_PWM2_BITSVAL                (PIN_OUTGRP_VAL_IO22_PWM2               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO22_UART0_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO22_UART0_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO22_UART1_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO22_UART1_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO22_SPI3_CLK_BITSVAL            (PIN_OUTGRP_VAL_IO22_SPI3_CLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO22_DBG22_BITSVAL               (PIN_OUTGRP_VAL_IO22_DBG22              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO22_MSQ_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO22_MSQ_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO22_APS_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO22_APS_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO22_PDM_CLK_BITSVAL             (PIN_OUTGRP_VAL_IO22_PDM_CLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO22_CAM_XVCLK_BITSVAL           (PIN_OUTGRP_VAL_IO22_CAM_XVCLK          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_SPI1_CS3_BITSVAL            (PIN_OUTGRP_VAL_IO23_SPI1_CS3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_PWM3_BITSVAL                (PIN_OUTGRP_VAL_IO23_PWM3               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_I2C_SCL_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO23_I2C_SCL_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_UART0_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO23_UART0_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_UART1_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO23_UART1_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_SPI0_CS2_BITSVAL            (PIN_OUTGRP_VAL_IO23_SPI0_CS2           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_DBG23_BITSVAL               (PIN_OUTGRP_VAL_IO23_DBG23              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_APS_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO23_APS_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_MSQ_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO23_MSQ_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_SPI3_TXD2_BITSVAL           (PIN_OUTGRP_VAL_IO23_SPI3_TXD2          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_I2S_WS_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO23_I2S_WS_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO23_CAM_SHUTTER_BITSVAL         (PIN_OUTGRP_VAL_IO23_CAM_SHUTTER        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_PWM4_BITSVAL                (PIN_OUTGRP_VAL_IO24_PWM4               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_I2C_SDA_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO24_I2C_SDA_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_UART1_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO24_UART1_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_PDM_CLK_BITSVAL             (PIN_OUTGRP_VAL_IO24_PDM_CLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_DBG24_BITSVAL               (PIN_OUTGRP_VAL_IO24_DBG24              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_TDO_BITSVAL                 (PIN_OUTGRP_VAL_IO24_TDO                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_CLK_DS_AUX_78K125_BITSVAL   (PIN_OUTGRP_VAL_IO24_CLK_DS_AUX_78K125  << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_MSQ_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO24_MSQ_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_APS_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO24_APS_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_MSQ_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO24_MSQ_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_SPI3_CLK_BITSVAL            (PIN_OUTGRP_VAL_IO24_SPI3_CLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_I2S_SCLK_BITSVAL            (PIN_OUTGRP_VAL_IO24_I2S_SCLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO24_CAM_HSYNC_OUT_BITSVAL       (PIN_OUTGRP_VAL_IO24_CAM_HSYNC_OUT      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO25_PWM5_BITSVAL                (PIN_OUTGRP_VAL_IO25_PWM5               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO25_I2C_SCL_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO25_I2C_SCL_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO25_DBG25_BITSVAL               (PIN_OUTGRP_VAL_IO25_DBG25              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO25_APS_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO25_APS_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO25_SPI3_CS0_BITSVAL            (PIN_OUTGRP_VAL_IO25_SPI3_CS0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO25_I2S_WS_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO25_I2S_WS_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO25_PU_EPA_OUT0_BITSVAL         (PIN_OUTGRP_VAL_IO25_PU_EPA_OUT0        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_SPI2_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO26_SPI2_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_PWM0_BITSVAL                (PIN_OUTGRP_VAL_IO26_PWM0               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_I2C_SDA_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO26_I2C_SDA_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_UART0_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO26_UART0_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_UART1_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO26_UART1_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_SPI3_CS0_BITSVAL            (PIN_OUTGRP_VAL_IO26_SPI3_CS0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_DBG26_BITSVAL               (PIN_OUTGRP_VAL_IO26_DBG26              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_MSQ_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO26_MSQ_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_APS_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO26_APS_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_MSQ_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO26_MSQ_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_PU_EPA_OUT0_BITSVAL         (PIN_OUTGRP_VAL_IO26_PU_EPA_OUT0        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO26_CAM_VSYNC_OUT_BITSVAL       (PIN_OUTGRP_VAL_IO26_CAM_VSYNC_OUT      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO27_SPI0_CS2_BITSVAL            (PIN_OUTGRP_VAL_IO27_SPI0_CS2           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO27_PWM1_BITSVAL                (PIN_OUTGRP_VAL_IO27_PWM1               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO27_I2C_SCL_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO27_I2C_SCL_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO27_SPI3_CS2_BITSVAL            (PIN_OUTGRP_VAL_IO27_SPI3_CS2           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO27_DBG27_BITSVAL               (PIN_OUTGRP_VAL_IO27_DBG27              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO27_APS_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO27_APS_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO27_SPI3_TXD0_BITSVAL           (PIN_OUTGRP_VAL_IO27_SPI3_TXD0          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO27_I2S_SCLK_BITSVAL            (PIN_OUTGRP_VAL_IO27_I2S_SCLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO27_PU_EPA_OUT1_BITSVAL         (PIN_OUTGRP_VAL_IO27_PU_EPA_OUT1        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO28_SPI2_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO28_SPI2_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO28_PWM2_BITSVAL                (PIN_OUTGRP_VAL_IO28_PWM2               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO28_DT_DS_AUX_BITSVAL           (PIN_OUTGRP_VAL_IO28_DT_DS_AUX          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO28_UART0_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO28_UART0_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO28_SPI3_CLK_BITSVAL            (PIN_OUTGRP_VAL_IO28_SPI3_CLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO28_DBG28_BITSVAL               (PIN_OUTGRP_VAL_IO28_DBG28              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO28_MSQ_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO28_MSQ_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO28_SPI3_TXD2_BITSVAL           (PIN_OUTGRP_VAL_IO28_SPI3_TXD2          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO28_I2S_SDO0_BITSVAL            (PIN_OUTGRP_VAL_IO28_I2S_SDO0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO28_EXT_2IO_CLK_BITSVAL         (PIN_OUTGRP_VAL_IO28_EXT_2IO_CLK        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO29_SPI2_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO29_SPI2_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO29_PDM_CLK_BITSVAL             (PIN_OUTGRP_VAL_IO29_PDM_CLK            << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO29_I2C_SDA_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO29_I2C_SDA_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO29_SPI3_TXD0_BITSVAL           (PIN_OUTGRP_VAL_IO29_SPI3_TXD0          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO29_DBG29_BITSVAL               (PIN_OUTGRP_VAL_IO29_DBG29              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO29_APS_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO29_APS_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO29_APS_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO29_APS_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO29_SPI3_TXD1_BITSVAL           (PIN_OUTGRP_VAL_IO29_SPI3_TXD1          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO30_SPI1_CS1_BITSVAL            (PIN_OUTGRP_VAL_IO30_SPI1_CS1           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO30_OTP_PROGRAM_PERIOD_BITSVAL  (PIN_OUTGRP_VAL_IO30_OTP_PROGRAM_PERIOD << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO30_I2C_SDA_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO30_I2C_SDA_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO30_UART1_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO30_UART1_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO30_SPI0_CS3_BITSVAL            (PIN_OUTGRP_VAL_IO30_SPI0_CS3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO30_DBG30_BITSVAL               (PIN_OUTGRP_VAL_IO30_DBG30              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO30_MSQ_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO30_MSQ_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO30_MSQ_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO30_MSQ_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO30_SPI3_CS1_BITSVAL            (PIN_OUTGRP_VAL_IO30_SPI3_CS1           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO30_PU_EPA_OUT2_BITSVAL         (PIN_OUTGRP_VAL_IO30_PU_EPA_OUT2        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_I2C_SCL_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO31_I2C_SCL_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_UART0_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO31_UART0_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_UART1_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO31_UART1_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_DBG31_BITSVAL               (PIN_OUTGRP_VAL_IO31_DBG31              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_TDO_BITSVAL                 (PIN_OUTGRP_VAL_IO31_TDO                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_APS_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO31_APS_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_APS_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO31_APS_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_MSQ_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO31_MSQ_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_SPI3_TXD3_BITSVAL           (PIN_OUTGRP_VAL_IO31_SPI3_TXD3          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_I2S_SDO0_BITSVAL            (PIN_OUTGRP_VAL_IO31_I2S_SDO0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO31_PU_EPA_OUT3_BITSVAL         (PIN_OUTGRP_VAL_IO31_PU_EPA_OUT3        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO32_SPI2_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO32_SPI2_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO32_OTP_PROGRAM_PERIOD_BITSVAL  (PIN_OUTGRP_VAL_IO32_OTP_PROGRAM_PERIOD << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO32_I2C_SCL_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO32_I2C_SCL_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO32_UART0_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO32_UART0_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO32_SPI3_TXD2_BITSVAL           (PIN_OUTGRP_VAL_IO32_SPI3_TXD2          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO32_DBG0_BITSVAL                (PIN_OUTGRP_VAL_IO32_DBG0               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO32_MSQ_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO32_MSQ_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO32_I2S_WS_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO32_I2S_WS_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO32_PU_EPA_OUT1_BITSVAL         (PIN_OUTGRP_VAL_IO32_PU_EPA_OUT1        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO33_SPI2_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO33_SPI2_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO33_I2C_SDA_OUT_BITSVAL         (PIN_OUTGRP_VAL_IO33_I2C_SDA_OUT        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO33_SPI3_TXD1_BITSVAL           (PIN_OUTGRP_VAL_IO33_SPI3_TXD1          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO33_DBG1_BITSVAL                (PIN_OUTGRP_VAL_IO33_DBG1               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO33_TDO_BITSVAL                 (PIN_OUTGRP_VAL_IO33_TDO                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO33_APS_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO33_APS_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO33_I2S_SCLK_BITSVAL            (PIN_OUTGRP_VAL_IO33_I2S_SCLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO33_PU_EPA_OUT2_BITSVAL         (PIN_OUTGRP_VAL_IO33_PU_EPA_OUT2        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO34_UART0_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO34_UART0_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO34_UART1_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO34_UART1_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO34_SPI3_CS3_BITSVAL            (PIN_OUTGRP_VAL_IO34_SPI3_CS3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO34_DBG2_BITSVAL                (PIN_OUTGRP_VAL_IO34_DBG2               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO34_APS_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO34_APS_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO34_MSQ_UART_TXD_BITSVAL        (PIN_OUTGRP_VAL_IO34_MSQ_UART_TXD       << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO34_SPI3_TXD1_BITSVAL           (PIN_OUTGRP_VAL_IO34_SPI3_TXD1          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO34_I2S_WS_OUT_BITSVAL          (PIN_OUTGRP_VAL_IO34_I2S_WS_OUT         << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO34_CAM_HSYNC_OUT_BITSVAL       (PIN_OUTGRP_VAL_IO34_CAM_HSYNC_OUT      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO35_OTP_PROGRAM_PERIOD_BITSVAL  (PIN_OUTGRP_VAL_IO35_OTP_PROGRAM_PERIOD << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO35_UART0_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO35_UART0_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO35_UART1_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO35_UART1_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO35_SPI3_TXD0_BITSVAL           (PIN_OUTGRP_VAL_IO35_SPI3_TXD0          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO35_DBG3_BITSVAL                (PIN_OUTGRP_VAL_IO35_DBG3               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO35_APS_SWDO_BITSVAL            (PIN_OUTGRP_VAL_IO35_APS_SWDO           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO35_MSQ_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO35_MSQ_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO35_I2S_SDO0_BITSVAL            (PIN_OUTGRP_VAL_IO35_I2S_SDO0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO35_CAM_VSYNC_OUT_BITSVAL       (PIN_OUTGRP_VAL_IO35_CAM_VSYNC_OUT      << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO36_SPI2_TXD_BITSVAL            (PIN_OUTGRP_VAL_IO36_SPI2_TXD           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO36_CLK_DS_AUX_78K125_BITSVAL   (PIN_OUTGRP_VAL_IO36_CLK_DS_AUX_78K125  << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO36_UART0_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO36_UART0_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO36_UART1_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO36_UART1_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO36_SPI3_TXD1_BITSVAL           (PIN_OUTGRP_VAL_IO36_SPI3_TXD1          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO36_DBG4_BITSVAL                (PIN_OUTGRP_VAL_IO36_DBG4               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO36_TDO_BITSVAL                 (PIN_OUTGRP_VAL_IO36_TDO                << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO36_APS_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO36_APS_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO36_SPI3_TXD0_BITSVAL           (PIN_OUTGRP_VAL_IO36_SPI3_TXD0          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO36_CAM_FREX_BITSVAL            (PIN_OUTGRP_VAL_IO36_CAM_FREX           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO37_SPI1_CS1_BITSVAL            (PIN_OUTGRP_VAL_IO37_SPI1_CS1           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO37_UART0_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO37_UART0_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO37_DBG5_BITSVAL                (PIN_OUTGRP_VAL_IO37_DBG5               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO37_APS_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO37_APS_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO37_MSQ_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO37_MSQ_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO37_SPI3_CS2_BITSVAL            (PIN_OUTGRP_VAL_IO37_SPI3_CS2           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO37_I2S_SCLK_BITSVAL            (PIN_OUTGRP_VAL_IO37_I2S_SCLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO37_PU_EPA_OUT3_BITSVAL         (PIN_OUTGRP_VAL_IO37_PU_EPA_OUT3        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO37_CAM_EXPST_BITSVAL           (PIN_OUTGRP_VAL_IO37_CAM_EXPST          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO38_SPI0_CS3_BITSVAL            (PIN_OUTGRP_VAL_IO38_SPI0_CS3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO38_SPI3_CLK_BITSVAL            (PIN_OUTGRP_VAL_IO38_SPI3_CLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO38_SPI3_CS3_BITSVAL            (PIN_OUTGRP_VAL_IO38_SPI3_CS3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO38_UART0_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO38_UART0_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO38_DBG6_BITSVAL                (PIN_OUTGRP_VAL_IO38_DBG6               << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO38_APS_UART_RTS_N_BITSVAL      (PIN_OUTGRP_VAL_IO38_APS_UART_RTS_N     << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO38_I2S_SDO0_BITSVAL            (PIN_OUTGRP_VAL_IO38_I2S_SDO0           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO39_SPI0_TXD0_BITSVAL           (PIN_OUTGRP_VAL_IO39_SPI0_TXD0          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO39_DBG12_BITSVAL               (PIN_OUTGRP_VAL_IO39_DBG12              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO40_SPI0_TXD1_BITSVAL           (PIN_OUTGRP_VAL_IO40_SPI0_TXD1          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO40_DBG12_BITSVAL               (PIN_OUTGRP_VAL_IO40_DBG12              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO41_SPI0_TXD2_BITSVAL           (PIN_OUTGRP_VAL_IO41_SPI0_TXD2          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO41_DBG12_BITSVAL               (PIN_OUTGRP_VAL_IO41_DBG12              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO42_SPI0_TXD3_BITSVAL           (PIN_OUTGRP_VAL_IO42_SPI0_TXD3          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO42_DBG12_BITSVAL               (PIN_OUTGRP_VAL_IO42_DBG12              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO43_SPI0_CS1_BITSVAL            (PIN_OUTGRP_VAL_IO43_SPI0_CS1           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO43_SPI0_CS2_BITSVAL            (PIN_OUTGRP_VAL_IO43_SPI0_CS2           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO43_SPI0_CS3_BITSVAL            (PIN_OUTGRP_VAL_IO43_SPI0_CS3           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO43_UART1_RTS_N_BITSVAL         (PIN_OUTGRP_VAL_IO43_UART1_RTS_N        << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO43_DBG15_BITSVAL               (PIN_OUTGRP_VAL_IO43_DBG15              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO44_SPI0_CLK_BITSVAL            (PIN_OUTGRP_VAL_IO44_SPI0_CLK           << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO44_UART1_TXD_BITSVAL           (PIN_OUTGRP_VAL_IO44_UART1_TXD          << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO44_DBG11_BITSVAL               (PIN_OUTGRP_VAL_IO44_DBG11              << PIN_OUTGRP_VAL_BIT)
#define PIN_OUTGRP_VAL_IO44_TDO_BITSVAL                 (PIN_OUTGRP_VAL_IO44_TDO                << PIN_OUTGRP_VAL_BIT)

#define PIN_INPUT_FLOATING                              0ULL    /*Input floating. Pull resistor not enabled. */
#define PIN_INPUT_IO0_UP                                1ULL    /*IO0 pull UP                                */
#define PIN_INPUT_IO1_UP                                1ULL    /*IO1 pull UP                                */
#define PIN_INPUT_IO2_UP                                1ULL    /*IO2 pull UP                                */
#define PIN_INPUT_IO3_DOWN                              1ULL    /*IO3 pull DOWN                              */
#define PIN_INPUT_IO4_UP                                1ULL    /*IO4 pull UP                                */
#define PIN_INPUT_IO5_UP                                1ULL    /*IO5 pull UP                                */
#define PIN_INPUT_IO6_DOWN                              1ULL    /*IO6 pull DOWN                              */
#define PIN_INPUT_IO7_UP                                1ULL    /*IO7 pull UP                                */
#define PIN_INPUT_IO8_UP                                1ULL    /*IO8 pull UP                                */
#define PIN_INPUT_IO9_UP                                1ULL    /*IO9 pull UP                                */
#define PIN_INPUT_IO10_UP                               1ULL    /*IO10 pull UP                               */
#define PIN_INPUT_IO11_UP                               1ULL    /*IO11 pull UP                               */
#define PIN_INPUT_IO12_UP                               1ULL    /*IO12 pull UP                               */
#define PIN_INPUT_IO13_DOWN                             1ULL    /*IO13 pull DOWN                             */
#define PIN_INPUT_IO14_UP                               1ULL    /*IO14 pull UP                               */
#define PIN_INPUT_IO15_UP                               1ULL    /*IO15 pull UP                               */
#define PIN_INPUT_IO16_DOWN                             1ULL    /*IO16 pull DOWN                             */
#define PIN_INPUT_IO17_DOWN                             1ULL    /*IO17 pull DOWN                             */
#define PIN_INPUT_IO18_UP                               1ULL    /*IO18 pull UP                               */
#define PIN_INPUT_IO19_UP                               1ULL    /*IO19 pull UP                               */
#define PIN_INPUT_IO20_DOWN                             1ULL    /*IO20 pull DOWN                             */
#define PIN_INPUT_IO21_DOWN                             1ULL    /*IO21 pull DOWN                             */
#define PIN_INPUT_IO22_UP                               1ULL    /*IO22 pull UP                               */
#define PIN_INPUT_IO23_UP                               1ULL    /*IO23 pull UP                               */
#define PIN_INPUT_IO24_UP                               1ULL    /*IO24 pull UP                               */
#define PIN_INPUT_IO25_UP                               1ULL    /*IO25 pull UP                               */
#define PIN_INPUT_IO26_UP                               1ULL    /*IO26 pull UP                               */
#define PIN_INPUT_IO27_UP                               1ULL    /*IO27 pull UP                               */
#define PIN_INPUT_IO28_UP                               1ULL    /*IO28 pull UP                               */
#define PIN_INPUT_IO29_UP                               1ULL    /*IO29 pull UP                               */
#define PIN_INPUT_IO30_UP                               1ULL    /*IO30 pull UP                               */
#define PIN_INPUT_IO31_UP                               1ULL    /*IO31 pull UP                               */
#define PIN_INPUT_IO32_UP                               1ULL    /*IO32 pull UP                               */
#define PIN_INPUT_IO33_UP                               1ULL    /*IO33 pull UP                               */
#define PIN_INPUT_IO34_UP                               1ULL    /*IO34 pull UP                               */
#define PIN_INPUT_IO35_DOWN                             1ULL    /*IO35 pull DOWN                             */
#define PIN_INPUT_IO36_UP                               1ULL    /*IO36 pull UP                               */
#define PIN_INPUT_IO37_UP                               1ULL    /*IO37 pull UP                               */
#define PIN_INPUT_IO38_UP                               1ULL    /*IO38 pull UP                               */
#define PIN_INPUT_IO39_UP                               1ULL    /*IO39 pull UP                               */
#define PIN_INPUT_IO40_UP                               1ULL    /*IO40 pull UP                               */
#define PIN_INPUT_IO41_UP                               1ULL    /*IO41 pull UP                               */
#define PIN_INPUT_IO42_UP                               1ULL    /*IO42 pull UP                               */
#define PIN_INPUT_IO43_UP                               1ULL    /*IO43 pull UP                               */
#define PIN_INPUT_IO44_UP                               1ULL    /*IO44 pull UP                               */

/* S_GPIO_PIN_CFG [45] OutLevel.  */
#define PIN_OUT_LEVEL_LOW                               0ULL
#define PIN_OUT_LEVEL_HIGH                              1ULL
#define PIN_OUT_LEVEL_LOW_BITSVAL                       (PIN_FUNC_DISABLE << PIN_OUT_LEVEL_BIT)
#define PIN_OUT_LEVEL_HIGH_BITSVAL                      (PIN_FUNC_ENABLE  << PIN_OUT_LEVEL_BIT)
#define PIN_AUTO_CTRL_LVL_LOW_BITSVAL                   (PIN_FUNC_DISABLE << PIN_AUTO_CTRL_LVL_BIT)
#define PIN_AUTO_CTRL_LVL_HIGH_BITSVAL                  (PIN_FUNC_ENABLE  << PIN_AUTO_CTRL_LVL_BIT)

/* S_GPIO_PIN_CFG [46] OutDrvCurr.  */
#define PIN_OUT_IO0_DRV_2mA                             0ULL
#define PIN_OUT_IO0_DRV_4mA                             1ULL
#define PIN_OUT_IO1_DRV_2mA                             0ULL
#define PIN_OUT_IO1_DRV_4mA                             1ULL
#define PIN_OUT_IO2_DRV_2mA                             0ULL
#define PIN_OUT_IO2_DRV_4mA                             1ULL
#define PIN_OUT_IO3_DRV_2mA                             0ULL
#define PIN_OUT_IO3_DRV_4mA                             1ULL
#define PIN_OUT_IO4_DRV_2mA                             0ULL
#define PIN_OUT_IO4_DRV_4mA                             1ULL
#define PIN_OUT_IO5_DRV_2mA                             0ULL
#define PIN_OUT_IO5_DRV_4mA                             1ULL
#define PIN_OUT_IO6_DRV_2mA                             0ULL
#define PIN_OUT_IO6_DRV_4mA                             1ULL
#define PIN_OUT_IO7_DRV_2mA                             0ULL
#define PIN_OUT_IO7_DRV_4mA                             1ULL
#define PIN_OUT_IO8_DRV_2mA                             0ULL
#define PIN_OUT_IO8_DRV_4mA                             1ULL
#define PIN_OUT_IO9_DRV_2mA                             0ULL
#define PIN_OUT_IO9_DRV_4mA                             1ULL
#define PIN_OUT_IO10_DRV_4mA                            0ULL
#define PIN_OUT_IO10_DRV_8mA                            1ULL
#define PIN_OUT_IO11_DRV_4mA                            0ULL
#define PIN_OUT_IO11_DRV_8mA                            1ULL
#define PIN_OUT_IO12_DRV_4mA                            0ULL
#define PIN_OUT_IO12_DRV_8mA                            1ULL
#define PIN_OUT_IO13_DRV_4mA                            0ULL
#define PIN_OUT_IO13_DRV_8mA                            1ULL
#define PIN_OUT_IO14_DRV_4mA                            0ULL
#define PIN_OUT_IO14_DRV_8mA                            1ULL
#define PIN_OUT_IO15_DRV_4mA                            0ULL
#define PIN_OUT_IO15_DRV_8mA                            1ULL
#define PIN_OUT_IO16_DRV_12mA                           0ULL
#define PIN_OUT_IO16_DRV_16mA                           1ULL
#define PIN_OUT_IO17_DRV_12mA                           0ULL
#define PIN_OUT_IO17_DRV_16mA                           1ULL
#define PIN_OUT_IO18_DRV_12mA                           0ULL
#define PIN_OUT_IO18_DRV_16mA                           1ULL
#define PIN_OUT_IO19_DRV_12mA                           0ULL
#define PIN_OUT_IO19_DRV_16mA                           1ULL
#define PIN_OUT_IO20_DRV_12mA                           0ULL
#define PIN_OUT_IO20_DRV_16mA                           1ULL
#define PIN_OUT_IO21_DRV_12mA                           0ULL
#define PIN_OUT_IO21_DRV_16mA                           1ULL
#define PIN_OUT_IO22_DRV_12mA                           0ULL
#define PIN_OUT_IO22_DRV_16mA                           1ULL
#define PIN_OUT_IO23_DRV_12mA                           0ULL
#define PIN_OUT_IO23_DRV_16mA                           1ULL
#define PIN_OUT_IO24_DRV_12mA                           0ULL
#define PIN_OUT_IO24_DRV_16mA                           1ULL
#define PIN_OUT_IO25_DRV_12mA                           0ULL
#define PIN_OUT_IO25_DRV_16mA                           1ULL
#define PIN_OUT_IO26_DRV_2mA                            0ULL
#define PIN_OUT_IO26_DRV_4mA                            1ULL
#define PIN_OUT_IO27_DRV_2mA                            0ULL
#define PIN_OUT_IO27_DRV_4mA                            1ULL
#define PIN_OUT_IO28_DRV_2mA                            0ULL
#define PIN_OUT_IO28_DRV_4mA                            1ULL
#define PIN_OUT_IO29_DRV_2mA                            0ULL
#define PIN_OUT_IO29_DRV_4mA                            1ULL
#define PIN_OUT_IO30_DRV_2mA                            0ULL
#define PIN_OUT_IO30_DRV_4mA                            1ULL
#define PIN_OUT_IO31_DRV_2mA                            0ULL
#define PIN_OUT_IO31_DRV_4mA                            1ULL
#define PIN_OUT_IO32_DRV_2mA                            0ULL
#define PIN_OUT_IO32_DRV_4mA                            1ULL
#define PIN_OUT_IO33_DRV_2mA                            0ULL
#define PIN_OUT_IO33_DRV_4mA                            1ULL
#define PIN_OUT_IO34_DRV_2mA                            0ULL
#define PIN_OUT_IO34_DRV_4mA                            1ULL
#define PIN_OUT_IO35_DRV_2mA                            0ULL
#define PIN_OUT_IO35_DRV_4mA                            1ULL
#define PIN_OUT_IO36_DRV_2mA                            0ULL
#define PIN_OUT_IO36_DRV_4mA                            1ULL
#define PIN_OUT_IO37_DRV_2mA                            0ULL
#define PIN_OUT_IO37_DRV_4mA                            1ULL
#define PIN_OUT_IO38_DRV_2mA                            0ULL
#define PIN_OUT_IO38_DRV_4mA                            1ULL
#define PIN_OUT_IO39_DRV_2mA                            0ULL
#define PIN_OUT_IO39_DRV_4mA                            1ULL
#define PIN_OUT_IO40_DRV_2mA                            0ULL
#define PIN_OUT_IO40_DRV_4mA                            1ULL
#define PIN_OUT_IO41_DRV_2mA                            0ULL
#define PIN_OUT_IO41_DRV_4mA                            1ULL
#define PIN_OUT_IO42_DRV_2mA                            0ULL
#define PIN_OUT_IO42_DRV_4mA                            1ULL
#define PIN_OUT_IO43_DRV_2mA                            0ULL
#define PIN_OUT_IO43_DRV_4mA                            1ULL
#define PIN_OUT_IO44_DRV_2mA                            0ULL
#define PIN_OUT_IO44_DRV_4mA                            1ULL

/* ************************** PIN CFG Value combination ************************************** */

/* GPIO                                                             GPIO                           IO                        In/Out enable                   Out level                 */
#define PIN_TYPE_GPIO_IN_IO0                            ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO0_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO0                       ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO0                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO0                       ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO0_BITSVAL                                                              ))
#define PIN_TYPE_GPIO_IN_IO1                            ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO1                       ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO1                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO1                       ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO1_BITSVAL                                                              ))
#define PIN_TYPE_GPIO_IN_IO2                            ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO2_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO2                       ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO2                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO2                       ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO2_BITSVAL                                                              ))
#define PIN_TYPE_GPIO_IN_IO3                            ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO3_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO3                       ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO3                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO3                       ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO3_BITSVAL                                                              ))
#define PIN_TYPE_GPIO_IN_IO4                            ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO4                       ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO4                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO4                       ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO4_BITSVAL                                                              ))
#define PIN_TYPE_GPIO_IN_IO5                            ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO5_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO5                       ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO5                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO5                       ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO5_BITSVAL                                                              ))
#define PIN_TYPE_GPIO_IN_IO6                            ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO6_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO6                       ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO6                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO6                       ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO6_BITSVAL                                                              ))
#define PIN_TYPE_GPIO_IN_IO7                            ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO7_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO7                       ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO7                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO7                       ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO7_BITSVAL                                                              ))
#define PIN_TYPE_GPIO_IN_IO8                            ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO8                       ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO8                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO8                       ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO8_BITSVAL                                                              ))
#define PIN_TYPE_GPIO_IN_IO9                            ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO9_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO9                       ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO9                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO9                       ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO9_BITSVAL                                                              ))
#define PIN_TYPE_GPIO_IN_IO10                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO10                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO10                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO10                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO10_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO11                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO11                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO11                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO11                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO11_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO12                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO12_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO12                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO12                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO12                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO12_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO13                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO13_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO13                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO13                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO13                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO13_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO14                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO14_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO14                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO14                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO14                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO14_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO15                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO15                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO15                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO15                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO15_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO16                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO16_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO16                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO16                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO16                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO16_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO17                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO17_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO17                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO17                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO17                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO17_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO18                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO18_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO18                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO18                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO18                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO18_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO19                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO19_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO19                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO19                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO19                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO19_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO20                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO20_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO20                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO20                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO20                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO20_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO21                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO21_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO21                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO21                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO21                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO21_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO22                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO22_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO22                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO22                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO22                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO22_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO23                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO23_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO23                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO23                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO23                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO23_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO24                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO24_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO24                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO24                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO24                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO24_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO25                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO25                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO25                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO25                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO25_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO26                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO26_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO26                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO26                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO26                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO26_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO27                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO27                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO27                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO27                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO27_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO28                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO28_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO28                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO28                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO28                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO28_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO29                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO29                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO29                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO29                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO29_BITSVAL                                                             ))

#define PIN_TYPE_GPIO_IN_IO30                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO30_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO30                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO30                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO30                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO30_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO31                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO31_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO31                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO31                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO31                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO31_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO32                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO32                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO32                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO32                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO32_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO33                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO33                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO33                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO33                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO33_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO34                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO34_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO34                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO34                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO34                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO34_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO35                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO35_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO35                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO35                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO35                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO35_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO36                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO36_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO36                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO36                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO36                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO36_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO37                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO37_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO37                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO37                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO37                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO37_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_IN_IO38                           ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO38_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL                              ))
#define PIN_TYPE_GPIO_OUT_LOW_IO38                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO38                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO38                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO38_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_OUT_LOW_IO39                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO39_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO39                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO39_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO39                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO39_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_OUT_LOW_IO40                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO40_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO40                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO40_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO40                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO40_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_OUT_LOW_IO41                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO41_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO41                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO41_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO41                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO41_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_OUT_LOW_IO42                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO42_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO42                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO42_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO42                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO42_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_OUT_LOW_IO43                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO43_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO43                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO43_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO43                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO43_BITSVAL                                                             ))
#define PIN_TYPE_GPIO_OUT_LOW_IO44                      ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO44_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_LOW_BITSVAL ))
#define PIN_TYPE_GPIO_OUT_HIGH_IO44                     ((uint64_t)(PIN_IO_TYPE_GPIO_OUT_BITSVAL | PIN_IO_NUM_IO44_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUT_LEVEL_HIGH_BITSVAL))
#define PIN_TYPE_GPIO_DISABLE_IO44                      ((uint64_t)(PIN_IO_TYPE_GPIO_IN_BITSVAL  | PIN_IO_NUM_IO44_BITSVAL                                                             ))


/* UART0_CTS_N        Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_UART0_CTS_N_IO4                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART0_CTS_N_IO4_BITSVAL      ))
#define PIN_TYPE_UART0_CTS_N_IO9                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART0_CTS_N_IO9_BITSVAL      ))
#define PIN_TYPE_UART0_CTS_N_IO12                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART0_CTS_N_IO12_BITSVAL     ))
#define PIN_TYPE_UART0_CTS_N_IO15                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART0_CTS_N_IO15_BITSVAL     ))
#define PIN_TYPE_UART0_CTS_N_IO25                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART0_CTS_N_IO25_BITSVAL     ))
#define PIN_TYPE_UART0_CTS_N_IO27                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART0_CTS_N_IO27_BITSVAL     ))
#define PIN_TYPE_UART0_CTS_N_LOW                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_UART0_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART0_CTS_N_LOW_BITSVAL      ))
#define PIN_TYPE_UART0_CTS_N_HIGH                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_UART0_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART0_CTS_N_HIGH_BITSVAL     ))

/* UART0_RXD          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_UART0_RXD_IO1                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_IO1_BITSVAL        ))
#define PIN_TYPE_UART0_RXD_IO2                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_IO2_BITSVAL        ))
#define PIN_TYPE_UART0_RXD_IO8                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_IO8_BITSVAL        ))
#define PIN_TYPE_UART0_RXD_IO11                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_IO11_BITSVAL       ))
#define PIN_TYPE_UART0_RXD_IO17                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_IO17_BITSVAL       ))
#define PIN_TYPE_UART0_RXD_IO18                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_IO18_BITSVAL       ))
#define PIN_TYPE_UART0_RXD_IO20                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_IO20_BITSVAL       ))
#define PIN_TYPE_UART0_RXD_IO29                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_IO29_BITSVAL       ))
#define PIN_TYPE_UART0_RXD_IO30                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_IO30_BITSVAL       ))
#define PIN_TYPE_UART0_RXD_IO33                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_IO33_BITSVAL       ))
#define PIN_TYPE_UART0_RXD_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_HIGH_BITSVAL       ))
#define PIN_TYPE_UART0_RXD_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_UART0_RXD_BITSVAL      | PIN_INGRP_VAL_UART0_RXD_LOW_BITSVAL        ))

/* UART1_CTS_N        Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_UART1_CTS_N_IO4                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_IO4_BITSVAL      ))
#define PIN_TYPE_UART1_CTS_N_IO9                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_IO9_BITSVAL      ))
#define PIN_TYPE_UART1_CTS_N_IO12                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_IO12_BITSVAL     ))
#define PIN_TYPE_UART1_CTS_N_IO14                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_IO14_BITSVAL     ))
#define PIN_TYPE_UART1_CTS_N_IO25                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_IO25_BITSVAL     ))
#define PIN_TYPE_UART1_CTS_N_IO27                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_IO27_BITSVAL     ))
#define PIN_TYPE_UART1_CTS_N_IO29                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_IO29_BITSVAL     ))
#define PIN_TYPE_UART1_CTS_N_IO33                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_IO33_BITSVAL     ))
#define PIN_TYPE_UART1_CTS_N_IO38                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_IO38_BITSVAL     ))
#define PIN_TYPE_UART1_CTS_N_LOW                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_LOW_BITSVAL      ))
#define PIN_TYPE_UART1_CTS_N_HIGH                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_UART1_CTS_N_BITSVAL    | PIN_INGRP_VAL_UART1_CTS_N_HIGH_BITSVAL     ))

/* UART1_RXD          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_UART1_RXD_IO1                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_IO1_BITSVAL        ))
#define PIN_TYPE_UART1_RXD_IO2                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_IO2_BITSVAL        ))
#define PIN_TYPE_UART1_RXD_IO8                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_IO8_BITSVAL        ))
#define PIN_TYPE_UART1_RXD_IO10                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_IO10_BITSVAL       ))
#define PIN_TYPE_UART1_RXD_IO16                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_IO16_BITSVAL       ))
#define PIN_TYPE_UART1_RXD_IO19                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_IO19_BITSVAL       ))
#define PIN_TYPE_UART1_RXD_IO21                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_IO21_BITSVAL       ))
#define PIN_TYPE_UART1_RXD_IO28                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_IO28_BITSVAL       ))
#define PIN_TYPE_UART1_RXD_IO32                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_IO32_BITSVAL       ))
#define PIN_TYPE_UART1_RXD_IO37                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_IO37_BITSVAL       ))
#define PIN_TYPE_UART1_RXD_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_HIGH_BITSVAL       ))
#define PIN_TYPE_UART1_RXD_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_UART1_RXD_BITSVAL      | PIN_INGRP_VAL_UART1_RXD_LOW_BITSVAL        ))


/* APS_UART_CTS_N     Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_APS_UART_CTS_N_IO4                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_APS_UART_CTS_N_IO4_BITSVAL   ))
#define PIN_TYPE_APS_UART_CTS_N_IO9                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_APS_UART_CTS_N_IO9_BITSVAL   ))
#define PIN_TYPE_APS_UART_CTS_N_IO10                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_APS_UART_CTS_N_IO10_BITSVAL  ))
#define PIN_TYPE_APS_UART_CTS_N_IO17                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_APS_UART_CTS_N_IO17_BITSVAL  ))
#define PIN_TYPE_APS_UART_CTS_N_IO27                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_APS_UART_CTS_N_IO27_BITSVAL  ))
#define PIN_TYPE_APS_UART_CTS_N_IO28                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_APS_UART_CTS_N_IO28_BITSVAL  ))
#define PIN_TYPE_APS_UART_CTS_N_IO33                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_APS_UART_CTS_N_IO33_BITSVAL  ))
#define PIN_TYPE_APS_UART_CTS_N_IO35                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_APS_UART_CTS_N_IO35_BITSVAL  ))
#define PIN_TYPE_APS_UART_CTS_N_LOW                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_APS_UART_CTS_N_LOW_BITSVAL   ))
#define PIN_TYPE_APS_UART_CTS_N_HIGH                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_APS_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_APS_UART_CTS_N_HIGH_BITSVAL  ))

/* APS_UART_RXD       Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_APS_UART_RXD_IO1                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_IO1_BITSVAL     ))
#define PIN_TYPE_APS_UART_RXD_IO2                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_IO2_BITSVAL     ))
#define PIN_TYPE_APS_UART_RXD_IO8                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_IO8_BITSVAL     ))
#define PIN_TYPE_APS_UART_RXD_IO11                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_IO11_BITSVAL    ))
#define PIN_TYPE_APS_UART_RXD_IO15                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_IO15_BITSVAL    ))
#define PIN_TYPE_APS_UART_RXD_IO18                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_IO18_BITSVAL    ))
#define PIN_TYPE_APS_UART_RXD_IO20                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_IO20_BITSVAL    ))
#define PIN_TYPE_APS_UART_RXD_IO25                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_IO25_BITSVAL    ))
#define PIN_TYPE_APS_UART_RXD_IO30                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_IO30_BITSVAL    ))
#define PIN_TYPE_APS_UART_RXD_IO32                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_IO32_BITSVAL    ))
#define PIN_TYPE_APS_UART_RXD_HIGH                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_HIGH_BITSVAL    ))
#define PIN_TYPE_APS_UART_RXD_LOW                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_APS_UART_RXD_BITSVAL   | PIN_INGRP_VAL_APS_UART_RXD_LOW_BITSVAL     ))

/* MSQ_UART_CTS_N     Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_MSQ_UART_CTS_N_IO4                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_MSQ_UART_CTS_N_IO4_BITSVAL   ))
#define PIN_TYPE_MSQ_UART_CTS_N_IO9                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_MSQ_UART_CTS_N_IO9_BITSVAL   ))
#define PIN_TYPE_MSQ_UART_CTS_N_IO14                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_MSQ_UART_CTS_N_IO14_BITSVAL  ))
#define PIN_TYPE_MSQ_UART_CTS_N_IO18                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_MSQ_UART_CTS_N_IO18_BITSVAL  ))
#define PIN_TYPE_MSQ_UART_CTS_N_IO22                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_MSQ_UART_CTS_N_IO22_BITSVAL  ))
#define PIN_TYPE_MSQ_UART_CTS_N_IO27                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_MSQ_UART_CTS_N_IO27_BITSVAL  ))
#define PIN_TYPE_MSQ_UART_CTS_N_IO29                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_MSQ_UART_CTS_N_IO29_BITSVAL  ))
#define PIN_TYPE_MSQ_UART_CTS_N_IO38                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_MSQ_UART_CTS_N_IO38_BITSVAL  ))
#define PIN_TYPE_MSQ_UART_CTS_N_LOW                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_MSQ_UART_CTS_N_LOW_BITSVAL   ))
#define PIN_TYPE_MSQ_UART_CTS_N_HIGH                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_MSQ_UART_CTS_N_BITSVAL | PIN_INGRP_VAL_MSQ_UART_CTS_N_HIGH_BITSVAL  ))

/* MSQ_UART_RXD       Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_MSQ_UART_RXD_IO2                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO2_BITSVAL     ))
#define PIN_TYPE_MSQ_UART_RXD_IO8                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO8_BITSVAL     ))
#define PIN_TYPE_MSQ_UART_RXD_IO10                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO10_BITSVAL    ))
#define PIN_TYPE_MSQ_UART_RXD_IO16                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO16_BITSVAL    ))
#define PIN_TYPE_MSQ_UART_RXD_IO19                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO19_BITSVAL    ))
#define PIN_TYPE_MSQ_UART_RXD_IO20                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO20_BITSVAL    ))
#define PIN_TYPE_MSQ_UART_RXD_IO25                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO25_BITSVAL    ))
#define PIN_TYPE_MSQ_UART_RXD_IO28                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO28_BITSVAL    ))
#define PIN_TYPE_MSQ_UART_RXD_IO32                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO32_BITSVAL    ))
#define PIN_TYPE_MSQ_UART_RXD_IO33                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO33_BITSVAL    ))
#define PIN_TYPE_MSQ_UART_RXD_IO36                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_IO36_BITSVAL    ))
#define PIN_TYPE_MSQ_UART_RXD_HIGH                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_HIGH_BITSVAL    ))
#define PIN_TYPE_MSQ_UART_RXD_LOW                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_MSQ_UART_RXD_BITSVAL   | PIN_INGRP_VAL_MSQ_UART_RXD_LOW_BITSVAL     ))

/* I2C_SCL_IN         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_I2C_SCL_IN_IO0                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO0_BITSVAL       ))
#define PIN_TYPE_I2C_SCL_IN_IO1                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO1_BITSVAL       ))
#define PIN_TYPE_I2C_SCL_IN_IO4                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO4_BITSVAL       ))
#define PIN_TYPE_I2C_SCL_IN_IO8                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO8_BITSVAL       ))
#define PIN_TYPE_I2C_SCL_IN_IO10                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO10_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_IO14                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO14_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_IO16                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO16_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_IO18                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO18_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_IO20                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO20_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_IO23                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO23_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_IO25                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO25_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_IO27                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO27_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_IO31                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO31_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_IO32                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_IO32_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_HIGH_BITSVAL      ))
#define PIN_TYPE_I2C_SCL_IN_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_I2C_SCL_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SCL_IN_LOW_BITSVAL       ))

/* I2C_SDA_IN         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_I2C_SDA_IN_IO2                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO2_BITSVAL       ))
#define PIN_TYPE_I2C_SDA_IN_IO5                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO5_BITSVAL       ))
#define PIN_TYPE_I2C_SDA_IN_IO9                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO9_BITSVAL       ))
#define PIN_TYPE_I2C_SDA_IN_IO11                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO11_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_IO15                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO15_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_IO17                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO17_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_IO19                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO19_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_IO21                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO21_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_IO24                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO24_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_IO26                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO26_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_IO29                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO29_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_IO30                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO30_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_IO33                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_IO33_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_HIGH_BITSVAL      ))
#define PIN_TYPE_I2C_SDA_IN_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_I2C_SDA_IN_BITSVAL     | PIN_INGRP_VAL_I2C_SDA_IN_LOW_BITSVAL       ))


/* TDI                Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_TDI_HIGH                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_HIGH_BITSVAL             ))
#define PIN_TYPE_TDI_IO1                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_IO1_BITSVAL              ))
#define PIN_TYPE_TDI_IO4                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_IO4_BITSVAL              ))
#define PIN_TYPE_TDI_IO8                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_IO8_BITSVAL              ))
#define PIN_TYPE_TDI_IO10                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_IO10_BITSVAL             ))
#define PIN_TYPE_TDI_IO14                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_IO14_BITSVAL             ))
#define PIN_TYPE_TDI_IO18                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_IO18_BITSVAL             ))
#define PIN_TYPE_TDI_IO23                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_IO23_BITSVAL             ))
#define PIN_TYPE_TDI_IO25                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_IO25_BITSVAL             ))
#define PIN_TYPE_TDI_IO27                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_IO27_BITSVAL             ))
#define PIN_TYPE_TDI_IO32                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_IO32_BITSVAL             ))
#define PIN_TYPE_TDI_LOW                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_TDI_BITSVAL            | PIN_INGRP_VAL_TDI_LOW_BITSVAL              ))

/* TCK                Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_TCK_LOW                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_LOW_BITSVAL              ))
#define PIN_TYPE_TCK_IO2                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO2_BITSVAL              ))
#define PIN_TYPE_TCK_IO5                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO5_BITSVAL              ))
#define PIN_TYPE_TCK_IO13                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO13_BITSVAL             ))
#define PIN_TYPE_TCK_IO15                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO15_BITSVAL             ))
#define PIN_TYPE_TCK_IO19                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO19_BITSVAL             ))
#define PIN_TYPE_TCK_IO21                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO21_BITSVAL             ))
#define PIN_TYPE_TCK_IO22                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO22_BITSVAL             ))
#define PIN_TYPE_TCK_IO28                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO28_BITSVAL             ))
#define PIN_TYPE_TCK_IO34                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO34_BITSVAL             ))
#define PIN_TYPE_TCK_IO35                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO35_BITSVAL             ))
#define PIN_TYPE_TCK_IO37                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_IO37_BITSVAL             ))
#define PIN_TYPE_TCK_HIGH                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_TCK_BITSVAL            | PIN_INGRP_VAL_TCK_HIGH_BITSVAL             ))

/* TMS                Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_TMS_HIGH                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_TMS_BITSVAL            | PIN_INGRP_VAL_TMS_HIGH_BITSVAL             ))
#define PIN_TYPE_TMS_IO0                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TMS_BITSVAL            | PIN_INGRP_VAL_TMS_IO0_BITSVAL              ))
#define PIN_TYPE_TMS_IO7                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TMS_BITSVAL            | PIN_INGRP_VAL_TMS_IO7_BITSVAL              ))
#define PIN_TYPE_TMS_IO12                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TMS_BITSVAL            | PIN_INGRP_VAL_TMS_IO12_BITSVAL             ))
#define PIN_TYPE_TMS_IO16                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TMS_BITSVAL            | PIN_INGRP_VAL_TMS_IO16_BITSVAL             ))
#define PIN_TYPE_TMS_IO26                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TMS_BITSVAL            | PIN_INGRP_VAL_TMS_IO26_BITSVAL             ))
#define PIN_TYPE_TMS_IO29                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TMS_BITSVAL            | PIN_INGRP_VAL_TMS_IO29_BITSVAL             ))
#define PIN_TYPE_TMS_IO30                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_TMS_BITSVAL            | PIN_INGRP_VAL_TMS_IO30_BITSVAL             ))
#define PIN_TYPE_TMS_LOW                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_TMS_BITSVAL            | PIN_INGRP_VAL_TMS_LOW_BITSVAL              ))

/* APS_SWDI           Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_APS_SWDI_IO0                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO0_BITSVAL         ))
#define PIN_TYPE_APS_SWDI_IO1                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO1_BITSVAL         ))
#define PIN_TYPE_APS_SWDI_IO4                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO4_BITSVAL         ))
#define PIN_TYPE_APS_SWDI_IO7                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO7_BITSVAL         ))
#define PIN_TYPE_APS_SWDI_IO15                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO15_BITSVAL        ))
#define PIN_TYPE_APS_SWDI_IO16                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO16_BITSVAL        ))
#define PIN_TYPE_APS_SWDI_IO18                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO18_BITSVAL        ))
#define PIN_TYPE_APS_SWDI_IO25                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO25_BITSVAL        ))
#define PIN_TYPE_APS_SWDI_IO27                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO27_BITSVAL        ))
#define PIN_TYPE_APS_SWDI_IO29                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO29_BITSVAL        ))
#define PIN_TYPE_APS_SWDI_IO31                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO31_BITSVAL        ))
#define PIN_TYPE_APS_SWDI_IO33                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO33_BITSVAL        ))
#define PIN_TYPE_APS_SWDI_IO35                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_IO35_BITSVAL        ))
#define PIN_TYPE_APS_SWDI_LOW                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_LOW_BITSVAL         ))
#define PIN_TYPE_APS_SWDI_HIGH                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_APS_SWDI_BITSVAL       | PIN_INGRP_VAL_APS_SWDI_HIGH_BITSVAL        ))

/* APS_SWCLK          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_APS_SWCLK_IO2                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO2_BITSVAL        ))
#define PIN_TYPE_APS_SWCLK_IO6                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO6_BITSVAL        ))
#define PIN_TYPE_APS_SWCLK_IO8                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO8_BITSVAL        ))
#define PIN_TYPE_APS_SWCLK_IO14                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO14_BITSVAL       ))
#define PIN_TYPE_APS_SWCLK_IO17                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO17_BITSVAL       ))
#define PIN_TYPE_APS_SWCLK_IO19                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO19_BITSVAL       ))
#define PIN_TYPE_APS_SWCLK_IO20                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO20_BITSVAL       ))
#define PIN_TYPE_APS_SWCLK_IO26                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO26_BITSVAL       ))
#define PIN_TYPE_APS_SWCLK_IO28                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO28_BITSVAL       ))
#define PIN_TYPE_APS_SWCLK_IO30                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO30_BITSVAL       ))
#define PIN_TYPE_APS_SWCLK_IO32                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO32_BITSVAL       ))
#define PIN_TYPE_APS_SWCLK_IO34                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_IO34_BITSVAL       ))
#define PIN_TYPE_APS_SWCLK_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_LOW_BITSVAL        ))
#define PIN_TYPE_APS_SWCLK_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_APS_SWCLK_BITSVAL      | PIN_INGRP_VAL_APS_SWCLK_HIGH_BITSVAL       ))

/* MSQ_SWDI           Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_MSQ_SWDI_IO2                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO2_BITSVAL         ))
#define PIN_TYPE_MSQ_SWDI_IO6                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO6_BITSVAL         ))
#define PIN_TYPE_MSQ_SWDI_IO8                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO8_BITSVAL         ))
#define PIN_TYPE_MSQ_SWDI_IO14                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO14_BITSVAL        ))
#define PIN_TYPE_MSQ_SWDI_IO17                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO17_BITSVAL        ))
#define PIN_TYPE_MSQ_SWDI_IO19                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO19_BITSVAL        ))
#define PIN_TYPE_MSQ_SWDI_IO20                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO20_BITSVAL        ))
#define PIN_TYPE_MSQ_SWDI_IO22                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO22_BITSVAL        ))
#define PIN_TYPE_MSQ_SWDI_IO24                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO24_BITSVAL        ))
#define PIN_TYPE_MSQ_SWDI_IO26                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO26_BITSVAL        ))
#define PIN_TYPE_MSQ_SWDI_IO28                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO28_BITSVAL        ))
#define PIN_TYPE_MSQ_SWDI_IO30                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO30_BITSVAL        ))
#define PIN_TYPE_MSQ_SWDI_IO32                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_IO32_BITSVAL        ))
#define PIN_TYPE_MSQ_SWDI_LOW                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_LOW_BITSVAL         ))
#define PIN_TYPE_MSQ_SWDI_HIGH                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_MSQ_SWDI_BITSVAL       | PIN_INGRP_VAL_MSQ_SWDI_HIGH_BITSVAL        ))

/* MSQ_SWCLK          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_MSQ_SWCLK_IO0                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO0_BITSVAL        ))
#define PIN_TYPE_MSQ_SWCLK_IO1                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO1_BITSVAL        ))
#define PIN_TYPE_MSQ_SWCLK_IO4                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO4_BITSVAL        ))
#define PIN_TYPE_MSQ_SWCLK_IO7                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO7_BITSVAL        ))
#define PIN_TYPE_MSQ_SWCLK_IO15                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO15_BITSVAL       ))
#define PIN_TYPE_MSQ_SWCLK_IO16                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO16_BITSVAL       ))
#define PIN_TYPE_MSQ_SWCLK_IO18                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO18_BITSVAL       ))
#define PIN_TYPE_MSQ_SWCLK_IO23                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO23_BITSVAL       ))
#define PIN_TYPE_MSQ_SWCLK_IO25                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO25_BITSVAL       ))
#define PIN_TYPE_MSQ_SWCLK_IO27                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO27_BITSVAL       ))
#define PIN_TYPE_MSQ_SWCLK_IO29                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO29_BITSVAL       ))
#define PIN_TYPE_MSQ_SWCLK_IO31                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO31_BITSVAL       ))
#define PIN_TYPE_MSQ_SWCLK_IO33                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_IO33_BITSVAL       ))
#define PIN_TYPE_MSQ_SWCLK_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_LOW_BITSVAL        ))
#define PIN_TYPE_MSQ_SWCLK_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_MSQ_SWCLK_BITSVAL      | PIN_INGRP_VAL_MSQ_SWCLK_HIGH_BITSVAL       ))


/* SPI0_RXD0          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI0_RXD0_IO8                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI0_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD0_IO8_BITSVAL        ))
#define PIN_TYPE_SPI0_RXD0_IO14                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI0_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD0_IO14_BITSVAL       ))
#define PIN_TYPE_SPI0_RXD0_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI0_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD0_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI0_RXD0_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI0_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD0_LOW_BITSVAL        ))

/* SPI0_RXD1          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI0_RXD1_IO7                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI0_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD1_IO7_BITSVAL        ))
#define PIN_TYPE_SPI0_RXD1_IO15                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI0_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD1_IO15_BITSVAL       ))
#define PIN_TYPE_SPI0_RXD1_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI0_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD1_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI0_RXD1_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI0_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD1_LOW_BITSVAL        ))

/* SPI0_RXD2          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI0_RXD2_IO5                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI0_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD2_IO5_BITSVAL        ))
#define PIN_TYPE_SPI0_RXD2_IO10                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI0_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD2_IO10_BITSVAL       ))
#define PIN_TYPE_SPI0_RXD2_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI0_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD2_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI0_RXD2_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI0_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD2_LOW_BITSVAL        ))

/* SPI0_RXD3          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI0_RXD3_IO4                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI0_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD3_IO4_BITSVAL        ))
#define PIN_TYPE_SPI0_RXD3_IO11                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI0_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD3_IO11_BITSVAL       ))
#define PIN_TYPE_SPI0_RXD3_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI0_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD3_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI0_RXD3_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI0_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI0_RXD3_LOW_BITSVAL        ))

/* SPI1_RXD0          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI1_RXD0_IO14                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI1_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD0_IO14_BITSVAL       ))
#define PIN_TYPE_SPI1_RXD0_IO8                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI1_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD0_IO8_BITSVAL        ))
#define PIN_TYPE_SPI1_RXD0_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI1_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD0_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI1_RXD0_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI1_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD0_LOW_BITSVAL        ))

/* SPI1_RXD1          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI1_RXD1_IO15                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI1_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD1_IO15_BITSVAL       ))
#define PIN_TYPE_SPI1_RXD1_IO7                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI1_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD1_IO7_BITSVAL        ))
#define PIN_TYPE_SPI1_RXD1_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI1_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD1_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI1_RXD1_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI1_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD1_LOW_BITSVAL        ))

/* SPI1_RXD2          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI1_RXD2_IO10                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI1_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD2_IO10_BITSVAL       ))
#define PIN_TYPE_SPI1_RXD2_IO5                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI1_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD2_IO5_BITSVAL        ))
#define PIN_TYPE_SPI1_RXD2_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI1_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD2_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI1_RXD2_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI1_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD2_LOW_BITSVAL        ))

/* SPI1_RXD3          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI1_RXD3_IO11                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI1_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD3_IO11_BITSVAL       ))
#define PIN_TYPE_SPI1_RXD3_IO4                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI1_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD3_IO4_BITSVAL        ))
#define PIN_TYPE_SPI1_RXD3_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI1_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD3_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI1_RXD3_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI1_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI1_RXD3_LOW_BITSVAL        ))

/* SPI2_CLK           Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI2_CLK_IO1                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_CLK_BITSVAL       | PIN_INGRP_VAL_SPI2_CLK_IO1_BITSVAL         ))
#define PIN_TYPE_SPI2_CLK_IO24                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_CLK_BITSVAL       | PIN_INGRP_VAL_SPI2_CLK_IO24_BITSVAL        ))
#define PIN_TYPE_SPI2_CLK_IO32                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_CLK_BITSVAL       | PIN_INGRP_VAL_SPI2_CLK_IO32_BITSVAL        ))
#define PIN_TYPE_SPI2_CLK_HIGH                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI2_CLK_BITSVAL       | PIN_INGRP_VAL_SPI2_CLK_HIGH_BITSVAL        ))
#define PIN_TYPE_SPI2_CLK_LOW                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI2_CLK_BITSVAL       | PIN_INGRP_VAL_SPI2_CLK_LOW_BITSVAL         ))

/* SPI2_CS_IN         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI2_CS_IN_IO22                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_CS_IN_BITSVAL     | PIN_INGRP_VAL_SPI2_CS_IN_IO22_BITSVAL      ))
#define PIN_TYPE_SPI2_CS_IN_IO25                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_CS_IN_BITSVAL     | PIN_INGRP_VAL_SPI2_CS_IN_IO25_BITSVAL      ))
#define PIN_TYPE_SPI2_CS_IN_IO33                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_CS_IN_BITSVAL     | PIN_INGRP_VAL_SPI2_CS_IN_IO33_BITSVAL      ))
#define PIN_TYPE_SPI2_CS_IN_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI2_CS_IN_BITSVAL     | PIN_INGRP_VAL_SPI2_CS_IN_HIGH_BITSVAL      ))
#define PIN_TYPE_SPI2_CS_IN_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI2_CS_IN_BITSVAL     | PIN_INGRP_VAL_SPI2_CS_IN_LOW_BITSVAL       ))

/* SPI2_RXD           Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI2_RXD_IO26                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_RXD_BITSVAL       | PIN_INGRP_VAL_SPI2_RXD_IO26_BITSVAL        ))
#define PIN_TYPE_SPI2_RXD_IO28                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_RXD_BITSVAL       | PIN_INGRP_VAL_SPI2_RXD_IO28_BITSVAL        ))
#define PIN_TYPE_SPI2_RXD_IO29                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_RXD_BITSVAL       | PIN_INGRP_VAL_SPI2_RXD_IO29_BITSVAL        ))
#define PIN_TYPE_SPI2_RXD_IO32                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_RXD_BITSVAL       | PIN_INGRP_VAL_SPI2_RXD_IO32_BITSVAL        ))
#define PIN_TYPE_SPI2_RXD_IO33                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_RXD_BITSVAL       | PIN_INGRP_VAL_SPI2_RXD_IO33_BITSVAL        ))
#define PIN_TYPE_SPI2_RXD_IO36                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI2_RXD_BITSVAL       | PIN_INGRP_VAL_SPI2_RXD_IO36_BITSVAL        ))
#define PIN_TYPE_SPI2_RXD_HIGH                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI2_RXD_BITSVAL       | PIN_INGRP_VAL_SPI2_RXD_HIGH_BITSVAL        ))
#define PIN_TYPE_SPI2_RXD_LOW                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI2_RXD_BITSVAL       | PIN_INGRP_VAL_SPI2_RXD_LOW_BITSVAL         ))

/* SPI3_RXD0          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI3_RXD0_IO5                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD0_IO5_BITSVAL        ))
#define PIN_TYPE_SPI3_RXD0_IO17                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD0_IO17_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD0_IO27                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD0_IO27_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD0_IO29                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD0_IO29_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD0_IO35                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD0_IO35_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD0_IO36                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD0_IO36_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD0_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI3_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD0_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD0_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI3_RXD0_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD0_LOW_BITSVAL        ))

/* SPI3_RXD1          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI3_RXD1_IO4                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD1_IO4_BITSVAL        ))
#define PIN_TYPE_SPI3_RXD1_IO16                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD1_IO16_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD1_IO29                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD1_IO29_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD1_IO33                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD1_IO33_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD1_IO34                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD1_IO34_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD1_IO36                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD1_IO36_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD1_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI3_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD1_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD1_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI3_RXD1_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD1_LOW_BITSVAL        ))

/* SPI3_RXD2          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI3_RXD2_IO2                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD2_IO2_BITSVAL        ))
#define PIN_TYPE_SPI3_RXD2_IO23                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD2_IO23_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD2_IO28                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD2_IO28_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD2_IO32                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD2_IO32_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD2_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI3_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD2_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD2_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI3_RXD2_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD2_LOW_BITSVAL        ))

/* SPI3_RXD3          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_SPI3_RXD3_IO0                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD3_IO0_BITSVAL        ))
#define PIN_TYPE_SPI3_RXD3_IO1                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD3_IO1_BITSVAL        ))
#define PIN_TYPE_SPI3_RXD3_IO31                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_SPI3_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD3_IO31_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD3_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI3_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD3_HIGH_BITSVAL       ))
#define PIN_TYPE_SPI3_RXD3_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_SPI3_RXD3_BITSVAL      | PIN_INGRP_VAL_SPI3_RXD3_LOW_BITSVAL        ))

/* OTP_OPM_MODE       Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_OTP_OPM_MODE_IO11                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_OTP_OPM_MODE_BITSVAL   | PIN_INGRP_VAL_OTP_OPM_MODE_IO11_BITSVAL    ))
#define PIN_TYPE_OTP_OPM_MODE_IO31                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_OTP_OPM_MODE_BITSVAL   | PIN_INGRP_VAL_OTP_OPM_MODE_IO31_BITSVAL    ))
#define PIN_TYPE_OTP_OPM_MODE_IO33                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_OTP_OPM_MODE_BITSVAL   | PIN_INGRP_VAL_OTP_OPM_MODE_IO33_BITSVAL    ))
#define PIN_TYPE_OTP_OPM_MODE_IO37                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_OTP_OPM_MODE_BITSVAL   | PIN_INGRP_VAL_OTP_OPM_MODE_IO37_BITSVAL    ))
#define PIN_TYPE_OTP_OPM_MODE_HIGH                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_OTP_OPM_MODE_BITSVAL   | PIN_INGRP_VAL_OTP_OPM_MODE_HIGH_BITSVAL    ))
#define PIN_TYPE_OTP_OPM_MODE_LOW                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_OTP_OPM_MODE_BITSVAL   | PIN_INGRP_VAL_OTP_OPM_MODE_LOW_BITSVAL     ))

/* I2S_SDI0           Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_I2S_SDI0_IO4                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_IO4_BITSVAL         ))
#define PIN_TYPE_I2S_SDI0_IO8                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_IO8_BITSVAL         ))
#define PIN_TYPE_I2S_SDI0_IO12                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_IO12_BITSVAL        ))
#define PIN_TYPE_I2S_SDI0_IO17                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_IO17_BITSVAL        ))
#define PIN_TYPE_I2S_SDI0_IO20                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_IO20_BITSVAL        ))
#define PIN_TYPE_I2S_SDI0_IO22                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_IO22_BITSVAL        ))
#define PIN_TYPE_I2S_SDI0_IO26                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_IO26_BITSVAL        ))
#define PIN_TYPE_I2S_SDI0_IO29                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_IO29_BITSVAL        ))
#define PIN_TYPE_I2S_SDI0_IO30                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_IO30_BITSVAL        ))
#define PIN_TYPE_I2S_SDI0_IO36                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_IO36_BITSVAL        ))
#define PIN_TYPE_I2S_SDI0_HIGH                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_HIGH_BITSVAL        ))
#define PIN_TYPE_I2S_SDI0_LOW                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_I2S_SDI0_BITSVAL       | PIN_INGRP_VAL_I2S_SDI0_LOW_BITSVAL         ))


/* PDM_RXD            Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_PDM_RXD_IO7                            ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PDM_RXD_BITSVAL        | PIN_INGRP_VAL_PDM_RXD_IO7_BITSVAL          ))
#define PIN_TYPE_PDM_RXD_IO11                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PDM_RXD_BITSVAL        | PIN_INGRP_VAL_PDM_RXD_IO11_BITSVAL         ))
#define PIN_TYPE_PDM_RXD_IO13                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PDM_RXD_BITSVAL        | PIN_INGRP_VAL_PDM_RXD_IO13_BITSVAL         ))
#define PIN_TYPE_PDM_RXD_IO25                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PDM_RXD_BITSVAL        | PIN_INGRP_VAL_PDM_RXD_IO25_BITSVAL         ))
#define PIN_TYPE_PDM_RXD_IO26                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PDM_RXD_BITSVAL        | PIN_INGRP_VAL_PDM_RXD_IO26_BITSVAL         ))
#define PIN_TYPE_PDM_RXD_IO35                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PDM_RXD_BITSVAL        | PIN_INGRP_VAL_PDM_RXD_IO35_BITSVAL         ))
#define PIN_TYPE_PDM_RXD_IO36                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PDM_RXD_BITSVAL        | PIN_INGRP_VAL_PDM_RXD_IO36_BITSVAL         ))
#define PIN_TYPE_PDM_RXD_HIGH                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_PDM_RXD_BITSVAL        | PIN_INGRP_VAL_PDM_RXD_HIGH_BITSVAL         ))
#define PIN_TYPE_PDM_RXD_LOW                            ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_PDM_RXD_BITSVAL        | PIN_INGRP_VAL_PDM_RXD_LOW_BITSVAL          ))

/* EXT_2IO_CLK_EN     Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_EXT_2IO_CLK_EN_IO4                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_EXT_2IO_CLK_EN_BITSVAL | PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO4_BITSVAL   ))
#define PIN_TYPE_EXT_2IO_CLK_EN_IO22                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_EXT_2IO_CLK_EN_BITSVAL | PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO22_BITSVAL  ))
#define PIN_TYPE_EXT_2IO_CLK_EN_IO23                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_EXT_2IO_CLK_EN_BITSVAL | PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO23_BITSVAL  ))
#define PIN_TYPE_EXT_2IO_CLK_EN_IO35                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_EXT_2IO_CLK_EN_BITSVAL | PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO35_BITSVAL  ))
#define PIN_TYPE_EXT_2IO_CLK_EN_IO36                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_EXT_2IO_CLK_EN_BITSVAL | PIN_INGRP_VAL_EXT_2IO_CLK_EN_IO36_BITSVAL  ))
#define PIN_TYPE_EXT_2IO_CLK_EN_LOW                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_EXT_2IO_CLK_EN_BITSVAL | PIN_INGRP_VAL_EXT_2IO_CLK_EN_LOW_BITSVAL   ))

/* CAM_PCLK           Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_PCLK_IO1                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_PCLK_BITSVAL       | PIN_INGRP_VAL_CAM_PCLK_IO1_BITSVAL         ))
#define PIN_TYPE_CAM_PCLK_IO15                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_PCLK_BITSVAL       | PIN_INGRP_VAL_CAM_PCLK_IO15_BITSVAL        ))
#define PIN_TYPE_CAM_PCLK_IO31                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_PCLK_BITSVAL       | PIN_INGRP_VAL_CAM_PCLK_IO31_BITSVAL        ))
#define PIN_TYPE_CAM_PCLK_HIGH                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_PCLK_BITSVAL       | PIN_INGRP_VAL_CAM_PCLK_HIGH_BITSVAL        ))
#define PIN_TYPE_CAM_PCLK_LOW                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_PCLK_BITSVAL       | PIN_INGRP_VAL_CAM_PCLK_LOW_BITSVAL         ))

/* CAM_HREF           Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_HREF_IO11                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_HREF_BITSVAL       | PIN_INGRP_VAL_CAM_HREF_IO11_BITSVAL        ))
#define PIN_TYPE_CAM_HREF_IO21                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_HREF_BITSVAL       | PIN_INGRP_VAL_CAM_HREF_IO21_BITSVAL        ))
#define PIN_TYPE_CAM_HREF_HIGH                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_HREF_BITSVAL       | PIN_INGRP_VAL_CAM_HREF_HIGH_BITSVAL        ))
#define PIN_TYPE_CAM_HREF_LOW                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_HREF_BITSVAL       | PIN_INGRP_VAL_CAM_HREF_LOW_BITSVAL         ))

/* CAM_VSYNC_IN       Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_VSYNC_IN_IO12                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_VSYNC_IN_BITSVAL   | PIN_INGRP_VAL_CAM_VSYNC_IN_IO12_BITSVAL    ))
#define PIN_TYPE_CAM_VSYNC_IN_IO26                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_VSYNC_IN_BITSVAL   | PIN_INGRP_VAL_CAM_VSYNC_IN_IO26_BITSVAL    ))
#define PIN_TYPE_CAM_VSYNC_IN_HIGH                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_VSYNC_IN_BITSVAL   | PIN_INGRP_VAL_CAM_VSYNC_IN_HIGH_BITSVAL    ))
#define PIN_TYPE_CAM_VSYNC_IN_LOW                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_VSYNC_IN_BITSVAL   | PIN_INGRP_VAL_CAM_VSYNC_IN_LOW_BITSVAL     ))

/* CAM_HSYNC_IN       Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_HSYNC_IN_IO15                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_HSYNC_IN_BITSVAL   | PIN_INGRP_VAL_CAM_HSYNC_IN_IO15_BITSVAL    ))
#define PIN_TYPE_CAM_HSYNC_IN_IO24                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_HSYNC_IN_BITSVAL   | PIN_INGRP_VAL_CAM_HSYNC_IN_IO24_BITSVAL    ))
#define PIN_TYPE_CAM_HSYNC_IN_HIGH                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_HSYNC_IN_BITSVAL   | PIN_INGRP_VAL_CAM_HSYNC_IN_HIGH_BITSVAL    ))
#define PIN_TYPE_CAM_HSYNC_IN_LOW                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_HSYNC_IN_BITSVAL   | PIN_INGRP_VAL_CAM_HSYNC_IN_LOW_BITSVAL     ))

/* CAM_DATA0          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA0_IO3                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA0_BITSVAL      | PIN_INGRP_VAL_CAM_DATA0_IO3_BITSVAL        ))
#define PIN_TYPE_CAM_DATA0_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA0_BITSVAL      | PIN_INGRP_VAL_CAM_DATA0_HIGH_BITSVAL       ))
#define PIN_TYPE_CAM_DATA0_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA0_BITSVAL      | PIN_INGRP_VAL_CAM_DATA0_LOW_BITSVAL        ))

/* CAM_DATA1          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA1_IO4                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA1_BITSVAL      | PIN_INGRP_VAL_CAM_DATA1_IO4_BITSVAL        ))
#define PIN_TYPE_CAM_DATA1_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA1_BITSVAL      | PIN_INGRP_VAL_CAM_DATA1_HIGH_BITSVAL       ))
#define PIN_TYPE_CAM_DATA1_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA1_BITSVAL      | PIN_INGRP_VAL_CAM_DATA1_LOW_BITSVAL        ))

/* CAM_DATA2          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA2_IO5                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA2_BITSVAL      | PIN_INGRP_VAL_CAM_DATA2_IO5_BITSVAL        ))
#define PIN_TYPE_CAM_DATA2_IO32                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA2_BITSVAL      | PIN_INGRP_VAL_CAM_DATA2_IO32_BITSVAL       ))
#define PIN_TYPE_CAM_DATA2_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA2_BITSVAL      | PIN_INGRP_VAL_CAM_DATA2_HIGH_BITSVAL       ))
#define PIN_TYPE_CAM_DATA2_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA2_BITSVAL      | PIN_INGRP_VAL_CAM_DATA2_LOW_BITSVAL        ))

/* CAM_DATA3          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA3_IO6                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA3_BITSVAL      | PIN_INGRP_VAL_CAM_DATA3_IO6_BITSVAL        ))
#define PIN_TYPE_CAM_DATA3_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA3_BITSVAL      | PIN_INGRP_VAL_CAM_DATA3_HIGH_BITSVAL       ))
#define PIN_TYPE_CAM_DATA3_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA3_BITSVAL      | PIN_INGRP_VAL_CAM_DATA3_LOW_BITSVAL        ))

/* CAM_DATA4          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA4_IO7                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA4_BITSVAL      | PIN_INGRP_VAL_CAM_DATA4_IO7_BITSVAL        ))
#define PIN_TYPE_CAM_DATA4_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA4_BITSVAL      | PIN_INGRP_VAL_CAM_DATA4_HIGH_BITSVAL       ))
#define PIN_TYPE_CAM_DATA4_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA4_BITSVAL      | PIN_INGRP_VAL_CAM_DATA4_LOW_BITSVAL        ))

/* CAM_DATA5          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA5_IO8                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA5_BITSVAL      | PIN_INGRP_VAL_CAM_DATA5_IO8_BITSVAL        ))
#define PIN_TYPE_CAM_DATA5_IO33                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA5_BITSVAL      | PIN_INGRP_VAL_CAM_DATA5_IO33_BITSVAL       ))
#define PIN_TYPE_CAM_DATA5_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA5_BITSVAL      | PIN_INGRP_VAL_CAM_DATA5_HIGH_BITSVAL       ))
#define PIN_TYPE_CAM_DATA5_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA5_BITSVAL      | PIN_INGRP_VAL_CAM_DATA5_LOW_BITSVAL        ))

/* CAM_DATA6          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA6_IO9                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA6_BITSVAL      | PIN_INGRP_VAL_CAM_DATA6_IO9_BITSVAL        ))
#define PIN_TYPE_CAM_DATA6_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA6_BITSVAL      | PIN_INGRP_VAL_CAM_DATA6_HIGH_BITSVAL       ))
#define PIN_TYPE_CAM_DATA6_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA6_BITSVAL      | PIN_INGRP_VAL_CAM_DATA6_LOW_BITSVAL        ))

/* CAM_DATA7          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA7_IO10                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA7_BITSVAL      | PIN_INGRP_VAL_CAM_DATA7_IO10_BITSVAL       ))
#define PIN_TYPE_CAM_DATA7_IO20                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA7_BITSVAL      | PIN_INGRP_VAL_CAM_DATA7_IO20_BITSVAL       ))
#define PIN_TYPE_CAM_DATA7_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA7_BITSVAL      | PIN_INGRP_VAL_CAM_DATA7_HIGH_BITSVAL       ))
#define PIN_TYPE_CAM_DATA7_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA7_BITSVAL      | PIN_INGRP_VAL_CAM_DATA7_LOW_BITSVAL        ))

/* CAM_DATA8          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA8_IO27                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA8_BITSVAL      | PIN_INGRP_VAL_CAM_DATA8_IO27_BITSVAL       ))
#define PIN_TYPE_CAM_DATA8_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA8_BITSVAL      | PIN_INGRP_VAL_CAM_DATA8_HIGH_BITSVAL       ))
#define PIN_TYPE_CAM_DATA8_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA8_BITSVAL      | PIN_INGRP_VAL_CAM_DATA8_LOW_BITSVAL        ))

/* CAM_DATA9          Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA9_IO28                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA9_BITSVAL      | PIN_INGRP_VAL_CAM_DATA9_IO28_BITSVAL       ))
#define PIN_TYPE_CAM_DATA9_HIGH                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA9_BITSVAL      | PIN_INGRP_VAL_CAM_DATA9_HIGH_BITSVAL       ))
#define PIN_TYPE_CAM_DATA9_LOW                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA9_BITSVAL      | PIN_INGRP_VAL_CAM_DATA9_LOW_BITSVAL        ))

/* CAM_DATA10         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA10_IO29                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA10_BITSVAL     | PIN_INGRP_VAL_CAM_DATA10_IO29_BITSVAL      ))
#define PIN_TYPE_CAM_DATA10_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA10_BITSVAL     | PIN_INGRP_VAL_CAM_DATA10_HIGH_BITSVAL      ))
#define PIN_TYPE_CAM_DATA10_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA10_BITSVAL     | PIN_INGRP_VAL_CAM_DATA10_LOW_BITSVAL       ))

/* CAM_DATA11         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA11_IO30                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA11_BITSVAL     | PIN_INGRP_VAL_CAM_DATA11_IO30_BITSVAL      ))
#define PIN_TYPE_CAM_DATA11_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA11_BITSVAL     | PIN_INGRP_VAL_CAM_DATA11_HIGH_BITSVAL      ))
#define PIN_TYPE_CAM_DATA11_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA11_BITSVAL     | PIN_INGRP_VAL_CAM_DATA11_LOW_BITSVAL       ))

/* CAM_DATA12         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA12_IO34                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA12_BITSVAL     | PIN_INGRP_VAL_CAM_DATA12_IO34_BITSVAL      ))
#define PIN_TYPE_CAM_DATA12_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA12_BITSVAL     | PIN_INGRP_VAL_CAM_DATA12_HIGH_BITSVAL      ))
#define PIN_TYPE_CAM_DATA12_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA12_BITSVAL     | PIN_INGRP_VAL_CAM_DATA12_LOW_BITSVAL       ))

/* CAM_DATA13         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA13_IO35                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA13_BITSVAL     | PIN_INGRP_VAL_CAM_DATA13_IO35_BITSVAL      ))
#define PIN_TYPE_CAM_DATA13_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA13_BITSVAL     | PIN_INGRP_VAL_CAM_DATA13_HIGH_BITSVAL      ))
#define PIN_TYPE_CAM_DATA13_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA13_BITSVAL     | PIN_INGRP_VAL_CAM_DATA13_LOW_BITSVAL       ))

/* CAM_DATA14         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA14_IO36                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA14_BITSVAL     | PIN_INGRP_VAL_CAM_DATA14_IO36_BITSVAL      ))
#define PIN_TYPE_CAM_DATA14_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA14_BITSVAL     | PIN_INGRP_VAL_CAM_DATA14_HIGH_BITSVAL      ))
#define PIN_TYPE_CAM_DATA14_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA14_BITSVAL     | PIN_INGRP_VAL_CAM_DATA14_LOW_BITSVAL       ))

/* CAM_DATA15         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CAM_DATA15_IO37                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CAM_DATA15_BITSVAL     | PIN_INGRP_VAL_CAM_DATA15_IO37_BITSVAL      ))
#define PIN_TYPE_CAM_DATA15_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA15_BITSVAL     | PIN_INGRP_VAL_CAM_DATA15_HIGH_BITSVAL      ))
#define PIN_TYPE_CAM_DATA15_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CAM_DATA15_BITSVAL     | PIN_INGRP_VAL_CAM_DATA15_LOW_BITSVAL       ))


/* PSRAM_RXD0         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_PSRAM_RXD0_IO39                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO39_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PSRAM_RXD0_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD0_IO39_BITSVAL      ))
#define PIN_TYPE_PSRAM_RXD0_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_PSRAM_RXD0_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD0_HIGH_BITSVAL      ))
#define PIN_TYPE_PSRAM_RXD0_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_PSRAM_RXD0_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD0_LOW_BITSVAL       ))

/* PSRAM_RXD1         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_PSRAM_RXD1_IO40                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO40_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PSRAM_RXD1_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD1_IO40_BITSVAL      ))
#define PIN_TYPE_PSRAM_RXD1_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_PSRAM_RXD1_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD1_HIGH_BITSVAL      ))
#define PIN_TYPE_PSRAM_RXD1_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_PSRAM_RXD1_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD1_LOW_BITSVAL       ))

/* PSRAM_RXD2         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_PSRAM_RXD2_IO41                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO41_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PSRAM_RXD2_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD2_IO41_BITSVAL      ))
#define PIN_TYPE_PSRAM_RXD2_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_PSRAM_RXD2_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD2_HIGH_BITSVAL      ))
#define PIN_TYPE_PSRAM_RXD2_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_PSRAM_RXD2_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD2_LOW_BITSVAL       ))

/* PSRAM_RXD3         Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_PSRAM_RXD3_IO42                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO42_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_PSRAM_RXD3_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD3_IO42_BITSVAL      ))
#define PIN_TYPE_PSRAM_RXD3_HIGH                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_PSRAM_RXD3_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD3_HIGH_BITSVAL      ))
#define PIN_TYPE_PSRAM_RXD3_LOW                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_PSRAM_RXD3_BITSVAL     | PIN_INGRP_VAL_PSRAM_RXD3_LOW_BITSVAL       ))

/* CLK_EXT_APS        Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CLK_EXT_APS_LOW                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_LOW_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_APS_IO4                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO4_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_APS_IO5                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO5_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_APS_IO6                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO6_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_APS_IO7                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO7_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_APS_IO8                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO8_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_APS_IO9                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO9_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_APS_IO10                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO10_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_APS_IO11                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO11_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_APS_IO18                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO18_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_APS_IO19                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO19_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_APS_IO34                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO34_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_APS_IO35                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO35_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_APS_IO38                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_IO38_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_APS_HIGH                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CLK_EXT_APS_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_APS_HIGH_BITSVAL     ))

/* CLK_EXT_MSQ        Input                                         Peripheral                   IO#                       Input enable                   Input group register offset             Input group register value                 */
#define PIN_TYPE_CLK_EXT_MSQ_LOW                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_LOW_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_MSQ_IO4                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO4_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_MSQ_IO5                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO5_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_MSQ_IO6                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO6_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_MSQ_IO7                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO7_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_MSQ_IO8                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO8_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_MSQ_IO9                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO9_BITSVAL      ))
#define PIN_TYPE_CLK_EXT_MSQ_IO10                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO10_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_MSQ_IO11                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO11_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_MSQ_IO18                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO18_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_MSQ_IO19                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO19_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_MSQ_IO34                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO34_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_MSQ_IO35                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO35_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_MSQ_IO38                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_IN_ENABLE_ENABLE_BITSVAL | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_IO38_BITSVAL     ))
#define PIN_TYPE_CLK_EXT_MSQ_HIGH                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_DIS_BITSVAL                                 | PIN_INGRP_OFST_CLK_EXT_MSQ_BITSVAL    | PIN_INGRP_VAL_CLK_EXT_MSQ_HIGH_BITSVAL     ))


/* IO0  Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CS3_IO0                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_SPI0_CS3_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD3_IO0                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_SPI3_TXD3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2C_SCL_OUT_IO0                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_I2C_SCL_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_TXD_IO0                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_UART0_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO0                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_UART1_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG0_IO0                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_DBG0_BITSVAL                                                                                  ))
#define PIN_TYPE_APS_SWDO_IO0                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_APS_SWDO_BITSVAL                                                                              ))
#define PIN_TYPE_APS_UART_TXD_IO0                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_APS_UART_TXD_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_TXD_IO0                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_MSQ_UART_TXD_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI1_CS3_IO0                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_SPI1_CS3_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SCLK_IO0                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO0_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO0_BITSVAL  | PIN_OUTGRP_VAL_IO0_I2S_SCLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))

/* IO1  Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI3_TXD3_IO1                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO1_BITSVAL  | PIN_OUTGRP_VAL_IO1_SPI3_TXD3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2C_SCL_OUT_IO1                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO1_BITSVAL  | PIN_OUTGRP_VAL_IO1_I2C_SCL_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG1_IO1                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO1_BITSVAL  | PIN_OUTGRP_VAL_IO1_DBG1_BITSVAL                                                                                  ))
#define PIN_TYPE_APS_SWDO_IO1                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO1_BITSVAL  | PIN_OUTGRP_VAL_IO1_APS_SWDO_BITSVAL                                                                              ))
#define PIN_TYPE_MSQ_UART_TXD_IO1                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO1_BITSVAL  | PIN_OUTGRP_VAL_IO1_MSQ_UART_TXD_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PDM_CLK_IO1                            ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO1_BITSVAL  | PIN_OUTGRP_VAL_IO1_PDM_CLK_BITSVAL             | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_I2S_SCLK_IO1                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO1_BITSVAL  | PIN_OUTGRP_VAL_IO1_I2S_SCLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_EXT_2IO_CLK_IO1                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO1_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO1_BITSVAL  | PIN_OUTGRP_VAL_IO1_EXT_2IO_CLK_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))

/* IO2  Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CS0_IO2                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO2_BITSVAL  | PIN_OUTGRP_VAL_IO2_SPI0_CS0_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD2_IO2                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO2_BITSVAL  | PIN_OUTGRP_VAL_IO2_SPI3_TXD2_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2C_SDA_OUT_IO2                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO2_BITSVAL  | PIN_OUTGRP_VAL_IO2_I2C_SDA_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG2_IO2                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO2_BITSVAL  | PIN_OUTGRP_VAL_IO2_DBG2_BITSVAL                                                                                  ))
#define PIN_TYPE_MSQ_SWDO_IO2                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO2_BITSVAL  | PIN_OUTGRP_VAL_IO2_MSQ_SWDO_BITSVAL                                                                              ))
#define PIN_TYPE_SPI1_CS0_IO2                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO2_BITSVAL  | PIN_OUTGRP_VAL_IO2_SPI1_CS0_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_WS_OUT_IO2                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO2_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO2_BITSVAL  | PIN_OUTGRP_VAL_IO2_I2S_WS_OUT_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))

/* IO3  Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CS1_IO3                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_SPI0_CS1_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CS1_IO3                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_SPI3_CS1_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_RTS_N_IO3                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_UART0_RTS_N_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_RTS_N_IO3                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_UART1_RTS_N_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG3_IO3                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_DBG3_BITSVAL                                                                                  ))
#define PIN_TYPE_TDO_IO3                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_TDO_BITSVAL                                                                                   ))
#define PIN_TYPE_APS_UART_RTS_N_IO3                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_APS_UART_RTS_N_BITSVAL      | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_RTS_N_IO3                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_MSQ_UART_RTS_N_BITSVAL      | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI1_CS1_IO3                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_SPI1_CS1_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SDO0_IO3                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_I2S_SDO0_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_EXT_2IO_CLK_IO3                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO3_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO3_BITSVAL  | PIN_OUTGRP_VAL_IO3_EXT_2IO_CLK_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))

/* IO4  Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_TXD3_IO4                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO4_BITSVAL  | PIN_OUTGRP_VAL_IO4_SPI0_TXD3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD1_IO4                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO4_BITSVAL  | PIN_OUTGRP_VAL_IO4_SPI3_TXD1_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2C_SCL_OUT_IO4                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO4_BITSVAL  | PIN_OUTGRP_VAL_IO4_I2C_SCL_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG4_IO4                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO4_BITSVAL  | PIN_OUTGRP_VAL_IO4_DBG4_BITSVAL                                                                                  ))
#define PIN_TYPE_APS_SWDO_IO4                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO4_BITSVAL  | PIN_OUTGRP_VAL_IO4_APS_SWDO_BITSVAL                                                                              ))
#define PIN_TYPE_SPI1_TXD3_IO4                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO4_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO4_BITSVAL  | PIN_OUTGRP_VAL_IO4_SPI1_TXD3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))

/* IO5  Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_TXD2_IO5                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO5_BITSVAL  | PIN_OUTGRP_VAL_IO5_SPI0_TXD2_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD0_IO5                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO5_BITSVAL  | PIN_OUTGRP_VAL_IO5_SPI3_TXD0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2C_SDA_OUT_IO5                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO5_BITSVAL  | PIN_OUTGRP_VAL_IO5_I2C_SDA_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_TXD_IO5                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO5_BITSVAL  | PIN_OUTGRP_VAL_IO5_UART0_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO5                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO5_BITSVAL  | PIN_OUTGRP_VAL_IO5_UART1_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG5_IO5                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO5_BITSVAL  | PIN_OUTGRP_VAL_IO5_DBG5_BITSVAL                                                                                  ))
#define PIN_TYPE_APS_UART_TXD_IO5                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO5_BITSVAL  | PIN_OUTGRP_VAL_IO5_APS_UART_TXD_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_TXD_IO5                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO5_BITSVAL  | PIN_OUTGRP_VAL_IO5_MSQ_UART_TXD_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI1_TXD2_IO5                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO5_BITSVAL  | PIN_OUTGRP_VAL_IO5_SPI1_TXD2_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SCLK_IO5                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO5_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO5_BITSVAL  | PIN_OUTGRP_VAL_IO5_I2S_SCLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))

/* IO6  Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CLK_IO6                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_SPI0_CLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_PDM_CLK_IO6                            ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_PDM_CLK_BITSVAL             | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_UART0_RTS_N_IO6                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_UART0_RTS_N_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_RTS_N_IO6                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_UART1_RTS_N_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG6_IO6                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_DBG6_BITSVAL                                                                                  ))
#define PIN_TYPE_TDO_IO6                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_TDO_BITSVAL                                                                                   ))
#define PIN_TYPE_MSQ_SWDO_IO6                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_MSQ_SWDO_BITSVAL                                                                              ))
#define PIN_TYPE_APS_UART_RTS_N_IO6                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_APS_UART_RTS_N_BITSVAL      | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_RTS_N_IO6                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_MSQ_UART_RTS_N_BITSVAL      | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI1_CLK_IO6                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_SPI1_CLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_I2S_WS_OUT_IO6                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO6_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO6_BITSVAL  | PIN_OUTGRP_VAL_IO6_I2S_WS_OUT_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))

/* IO7  Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_TXD1_IO7                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO7_BITSVAL  | PIN_OUTGRP_VAL_IO7_SPI0_TXD1_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_TXD_IO7                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO7_BITSVAL  | PIN_OUTGRP_VAL_IO7_UART0_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO7                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO7_BITSVAL  | PIN_OUTGRP_VAL_IO7_UART1_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG7_IO7                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO7_BITSVAL  | PIN_OUTGRP_VAL_IO7_DBG7_BITSVAL                                                                                  ))
#define PIN_TYPE_APS_SWDO_IO7                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO7_BITSVAL  | PIN_OUTGRP_VAL_IO7_APS_SWDO_BITSVAL                                                                              ))
#define PIN_TYPE_APS_UART_TXD_IO7                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO7_BITSVAL  | PIN_OUTGRP_VAL_IO7_APS_UART_TXD_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_TXD_IO7                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO7_BITSVAL  | PIN_OUTGRP_VAL_IO7_MSQ_UART_TXD_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI1_TXD1_IO7                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO7_BITSVAL  | PIN_OUTGRP_VAL_IO7_SPI1_TXD1_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SDO0_IO7                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO7_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO7_BITSVAL  | PIN_OUTGRP_VAL_IO7_I2S_SDO0_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))

/* IO8  Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_TXD0_IO8                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO8_BITSVAL  | PIN_OUTGRP_VAL_IO8_SPI0_TXD0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CS0_IO8                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO8_BITSVAL  | PIN_OUTGRP_VAL_IO8_SPI3_CS0_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2C_SCL_OUT_IO8                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO8_BITSVAL  | PIN_OUTGRP_VAL_IO8_I2C_SCL_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG8_IO8                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO8_BITSVAL  | PIN_OUTGRP_VAL_IO8_DBG8_BITSVAL                                                                                  ))
#define PIN_TYPE_MSQ_SWDO_IO8                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO8_BITSVAL  | PIN_OUTGRP_VAL_IO8_MSQ_SWDO_BITSVAL                                                                              ))
#define PIN_TYPE_SPI1_TXD0_IO8                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO8_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO8_BITSVAL  | PIN_OUTGRP_VAL_IO8_SPI1_TXD0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))

/* IO9  Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CS2_IO9                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO9_BITSVAL  | PIN_OUTGRP_VAL_IO9_SPI0_CS2_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CLK_IO9                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO9_BITSVAL  | PIN_OUTGRP_VAL_IO9_SPI3_CLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_I2C_SDA_OUT_IO9                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO9_BITSVAL  | PIN_OUTGRP_VAL_IO9_I2C_SDA_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG9_IO9                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO9_BITSVAL  | PIN_OUTGRP_VAL_IO9_DBG9_BITSVAL                                                                                  ))
#define PIN_TYPE_TDO_IO9                                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO9_BITSVAL  | PIN_OUTGRP_VAL_IO9_TDO_BITSVAL                                                                                   ))
#define PIN_TYPE_SPI1_CS2_IO9                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO9_BITSVAL  | PIN_OUTGRP_VAL_IO9_SPI1_CS2_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SDO0_IO9                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO9_BITSVAL  | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO9_BITSVAL  | PIN_OUTGRP_VAL_IO9_I2S_SDO0_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))


/* IO10 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_TXD2_IO10                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO10_BITSVAL | PIN_OUTGRP_VAL_IO10_SPI1_TXD2_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_OTP_PROGRAM_PERIOD_IO10                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO10_BITSVAL | PIN_OUTGRP_VAL_IO10_OTP_PROGRAM_PERIOD_BITSVAL                                                                   ))
#define PIN_TYPE_I2C_SCL_OUT_IO10                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO10_BITSVAL | PIN_OUTGRP_VAL_IO10_I2C_SCL_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_TXD_IO10                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO10_BITSVAL | PIN_OUTGRP_VAL_IO10_UART0_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG10_IO10                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO10_BITSVAL | PIN_OUTGRP_VAL_IO10_DBG10_BITSVAL                                                                                ))
#define PIN_TYPE_PDM_CLK_IO10                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO10_BITSVAL | PIN_OUTGRP_VAL_IO10_PDM_CLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_SPI0_TXD2_IO10                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO10_BITSVAL | PIN_OUTGRP_VAL_IO10_SPI0_TXD2_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SCLK_IO10                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO10_BITSVAL | PIN_OUTGRP_VAL_IO10_I2S_SCLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_EXT_2IO_CLK_IO10                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO10_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO10_BITSVAL | PIN_OUTGRP_VAL_IO10_EXT_2IO_CLK_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))

/* IO11 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_TXD3_IO11                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO11_BITSVAL | PIN_OUTGRP_VAL_IO11_SPI1_TXD3_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2C_SDA_OUT_IO11                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO11_BITSVAL | PIN_OUTGRP_VAL_IO11_I2C_SDA_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO11                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO11_BITSVAL | PIN_OUTGRP_VAL_IO11_UART1_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG11_IO11                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO11_BITSVAL | PIN_OUTGRP_VAL_IO11_DBG11_BITSVAL                                                                                ))
#define PIN_TYPE_TDO_IO11                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO11_BITSVAL | PIN_OUTGRP_VAL_IO11_TDO_BITSVAL                                                                                  ))
#define PIN_TYPE_MSQ_UART_RTS_N_IO11                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO11_BITSVAL | PIN_OUTGRP_VAL_IO11_MSQ_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_TXD3_IO11                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO11_BITSVAL | PIN_OUTGRP_VAL_IO11_SPI0_TXD3_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_WS_OUT_IO11                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO11_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO11_BITSVAL | PIN_OUTGRP_VAL_IO11_I2S_WS_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))

/* IO12 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_CS0_IO12                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO12_BITSVAL | PIN_OUTGRP_VAL_IO12_SPI1_CS0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PDM_CLK_IO12                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO12_BITSVAL | PIN_OUTGRP_VAL_IO12_PDM_CLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_DBG12_IO12                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO12_BITSVAL | PIN_OUTGRP_VAL_IO12_DBG12_BITSVAL                                                                                ))
#define PIN_TYPE_APS_UART_TXD_IO12                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO12_BITSVAL | PIN_OUTGRP_VAL_IO12_APS_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_TXD_IO12                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO12_BITSVAL | PIN_OUTGRP_VAL_IO12_MSQ_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_CS0_IO12                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO12_BITSVAL | PIN_OUTGRP_VAL_IO12_SPI0_CS0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT0_IO12                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO12_BITSVAL | PIN_OUTGRP_VAL_IO12_PU_EPA_OUT0_BITSVAL                                                                          ))
#define PIN_TYPE_CAM_VSYNC_OUT_IO12                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO12_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO12_BITSVAL | PIN_OUTGRP_VAL_IO12_CAM_VSYNC_OUT_BITSVAL                                                                        ))

/* IO13 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_CLK_IO13                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO13_BITSVAL | PIN_OUTGRP_VAL_IO13_SPI1_CLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_UART0_RTS_N_IO13                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO13_BITSVAL | PIN_OUTGRP_VAL_IO13_UART0_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_RTS_N_IO13                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO13_BITSVAL | PIN_OUTGRP_VAL_IO13_UART1_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG13_IO13                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO13_BITSVAL | PIN_OUTGRP_VAL_IO13_DBG13_BITSVAL                                                                                ))
#define PIN_TYPE_APS_UART_RTS_N_IO13                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO13_BITSVAL | PIN_OUTGRP_VAL_IO13_APS_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_TXD_IO13                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO13_BITSVAL | PIN_OUTGRP_VAL_IO13_MSQ_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_CLK_IO13                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO13_BITSVAL | PIN_OUTGRP_VAL_IO13_SPI0_CLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_I2S_SDO0_IO13                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO13_BITSVAL | PIN_OUTGRP_VAL_IO13_I2S_SDO0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT1_IO13                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO13_BITSVAL | PIN_OUTGRP_VAL_IO13_PU_EPA_OUT1_BITSVAL                                                                          ))
#define PIN_TYPE_CAM_XVCLK_IO13                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO13_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO13_BITSVAL | PIN_OUTGRP_VAL_IO13_CAM_XVCLK_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))

/* IO14 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_TXD0_IO14                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_SPI1_TXD0_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM5_IO14                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_PWM5_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SCL_OUT_IO14                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_I2C_SCL_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_RTS_N_IO14                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_UART0_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG14_IO14                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_DBG14_BITSVAL                                                                                ))
#define PIN_TYPE_MSQ_SWDO_IO14                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_MSQ_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_APS_UART_TXD_IO14                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_APS_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_TXD0_IO14                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_SPI0_TXD0_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SCLK_IO14                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_I2S_SCLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_PU_EPA_OUT2_IO14                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_PU_EPA_OUT2_BITSVAL                                                                          ))
#define PIN_TYPE_CAM_SHUTTER_IO14                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO14_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO14_BITSVAL | PIN_OUTGRP_VAL_IO14_CAM_SHUTTER_BITSVAL                                                                          ))

/* IO15 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_TXD1_IO15                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_SPI1_TXD1_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM4_IO15                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_PWM4_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SDA_OUT_IO15                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_I2C_SDA_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_RTS_N_IO15                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_UART1_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG15_IO15                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_DBG15_BITSVAL                                                                                ))
#define PIN_TYPE_APS_SWDO_IO15                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_APS_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_MSQ_UART_TXD_IO15                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_MSQ_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_TXD1_IO15                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_SPI0_TXD1_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CS3_IO15                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_SPI3_CS3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT3_IO15                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_PU_EPA_OUT3_BITSVAL                                                                          ))
#define PIN_TYPE_CAM_HSYNC_OUT_IO15                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO15_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO15_BITSVAL | PIN_OUTGRP_VAL_IO15_CAM_HSYNC_OUT_BITSVAL                                                                        ))

/* IO16 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_PWM3_IO16                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO16_BITSVAL | PIN_OUTGRP_VAL_IO16_PWM3_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SCL_OUT_IO16                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO16_BITSVAL | PIN_OUTGRP_VAL_IO16_I2C_SCL_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_TXD_IO16                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO16_BITSVAL | PIN_OUTGRP_VAL_IO16_UART0_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG16_IO16                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO16_BITSVAL | PIN_OUTGRP_VAL_IO16_DBG16_BITSVAL                                                                                ))
#define PIN_TYPE_APS_SWDO_IO16                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO16_BITSVAL | PIN_OUTGRP_VAL_IO16_APS_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_APS_UART_RTS_N_IO16                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO16_BITSVAL | PIN_OUTGRP_VAL_IO16_APS_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD1_IO16                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO16_BITSVAL | PIN_OUTGRP_VAL_IO16_SPI3_TXD1_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SDO0_IO16                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO16_BITSVAL | PIN_OUTGRP_VAL_IO16_I2S_SDO0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT0_IO16                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO16_BITSVAL | PIN_OUTGRP_VAL_IO16_PU_EPA_OUT0_BITSVAL                                                                          ))
#define PIN_TYPE_CAM_HSYNC_OUT_IO16                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO16_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO16_BITSVAL | PIN_OUTGRP_VAL_IO16_CAM_HSYNC_OUT_BITSVAL                                                                        ))

/* IO17 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_CS3_IO17                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_SPI1_CS3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM2_IO17                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_PWM2_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SDA_OUT_IO17                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_I2C_SDA_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO17                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_UART1_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG17_IO17                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_DBG17_BITSVAL                                                                                ))
#define PIN_TYPE_TDO_IO17                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_TDO_BITSVAL                                                                                  ))
#define PIN_TYPE_MSQ_SWDO_IO17                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_MSQ_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_MSQ_UART_RTS_N_IO17                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_MSQ_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD0_IO17                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_SPI3_TXD0_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT1_IO17                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_PU_EPA_OUT1_BITSVAL                                                                          ))
#define PIN_TYPE_CAM_VSYNC_OUT_IO17                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO17_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO17_BITSVAL | PIN_OUTGRP_VAL_IO17_CAM_VSYNC_OUT_BITSVAL                                                                        ))

/* IO18 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_CS1_IO18                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO18_BITSVAL | PIN_OUTGRP_VAL_IO18_SPI1_CS1_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM1_IO18                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO18_BITSVAL | PIN_OUTGRP_VAL_IO18_PWM1_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SCL_OUT_IO18                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO18_BITSVAL | PIN_OUTGRP_VAL_IO18_I2C_SCL_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO18                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO18_BITSVAL | PIN_OUTGRP_VAL_IO18_UART1_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG18_IO18                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO18_BITSVAL | PIN_OUTGRP_VAL_IO18_DBG18_BITSVAL                                                                                ))
#define PIN_TYPE_APS_SWDO_IO18                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO18_BITSVAL | PIN_OUTGRP_VAL_IO18_APS_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_SPI3_CLK_IO18                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO18_BITSVAL | PIN_OUTGRP_VAL_IO18_SPI3_CLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_I2S_SCLK_IO18                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO18_BITSVAL | PIN_OUTGRP_VAL_IO18_I2S_SCLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_PU_EPA_OUT0_IO18                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO18_BITSVAL | PIN_OUTGRP_VAL_IO18_PU_EPA_OUT0_BITSVAL                                                                          ))
#define PIN_TYPE_CAM_FREX_IO18                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO18_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO18_BITSVAL | PIN_OUTGRP_VAL_IO18_CAM_FREX_BITSVAL                                                                             ))


/* IO19 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_CS2_IO19                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_SPI1_CS2_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM0_IO19                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_PWM0_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SDA_OUT_IO19                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_I2C_SDA_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_TXD_IO19                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_UART0_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_CS1_IO19                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_SPI0_CS1_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG19_IO19                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_DBG19_BITSVAL                                                                                ))
#define PIN_TYPE_MSQ_SWDO_IO19                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_MSQ_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_APS_UART_TXD_IO19                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_APS_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CS0_IO19                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_SPI3_CS0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_WS_OUT_IO19                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_I2S_WS_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT1_IO19                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_PU_EPA_OUT1_BITSVAL                                                                          ))
#define PIN_TYPE_CAM_EXPST_IO19                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO19_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO19_BITSVAL | PIN_OUTGRP_VAL_IO19_CAM_EXPST_BITSVAL                                                                            ))

/* IO20 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CS2_IO20                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO20_BITSVAL | PIN_OUTGRP_VAL_IO20_SPI0_CS2_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM0_IO20                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO20_BITSVAL | PIN_OUTGRP_VAL_IO20_PWM0_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SCL_OUT_IO20                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO20_BITSVAL | PIN_OUTGRP_VAL_IO20_I2C_SCL_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO20                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO20_BITSVAL | PIN_OUTGRP_VAL_IO20_UART1_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_CLK_IO20                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO20_BITSVAL | PIN_OUTGRP_VAL_IO20_SPI0_CLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_DBG20_IO20                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO20_BITSVAL | PIN_OUTGRP_VAL_IO20_DBG20_BITSVAL                                                                                ))
#define PIN_TYPE_TDO_IO20                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO20_BITSVAL | PIN_OUTGRP_VAL_IO20_TDO_BITSVAL                                                                                  ))
#define PIN_TYPE_MSQ_SWDO_IO20                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO20_BITSVAL | PIN_OUTGRP_VAL_IO20_MSQ_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_SPI3_CLK_IO20                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO20_BITSVAL | PIN_OUTGRP_VAL_IO20_SPI3_CLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_PU_EPA_OUT2_IO20                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO20_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO20_BITSVAL | PIN_OUTGRP_VAL_IO20_PU_EPA_OUT2_BITSVAL                                                                          ))

/* IO21 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CS3_IO21                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_SPI0_CS3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM1_IO21                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_PWM1_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SDA_OUT_IO21                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_I2C_SDA_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_TXD_IO21                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_UART0_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_CS0_IO21                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_SPI0_CS0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG21_IO21                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_DBG21_BITSVAL                                                                                ))
#define PIN_TYPE_APS_UART_RTS_N_IO21                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_APS_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_TXD_IO21                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_MSQ_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CS0_IO21                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_SPI3_CS0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SDO0_IO21                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_I2S_SDO0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT3_IO21                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO21_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO21_BITSVAL | PIN_OUTGRP_VAL_IO21_PU_EPA_OUT3_BITSVAL                                                                          ))

/* IO22 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_PWM2_IO22                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO22_BITSVAL | PIN_OUTGRP_VAL_IO22_PWM2_BITSVAL                                                                                 ))
#define PIN_TYPE_UART0_TXD_IO22                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO22_BITSVAL | PIN_OUTGRP_VAL_IO22_UART0_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO22                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO22_BITSVAL | PIN_OUTGRP_VAL_IO22_UART1_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CLK_IO22                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO22_BITSVAL | PIN_OUTGRP_VAL_IO22_SPI3_CLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_DBG22_IO22                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO22_BITSVAL | PIN_OUTGRP_VAL_IO22_DBG22_BITSVAL                                                                                ))
#define PIN_TYPE_MSQ_SWDO_IO22                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO22_BITSVAL | PIN_OUTGRP_VAL_IO22_MSQ_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_APS_UART_TXD_IO22                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO22_BITSVAL | PIN_OUTGRP_VAL_IO22_APS_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PDM_CLK_IO22                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO22_BITSVAL | PIN_OUTGRP_VAL_IO22_PDM_CLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_CAM_XVCLK_IO22                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO22_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO22_BITSVAL | PIN_OUTGRP_VAL_IO22_CAM_XVCLK_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))

/* IO23 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_CS3_IO23                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_SPI1_CS3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM3_IO23                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_PWM3_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SCL_OUT_IO23                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_I2C_SCL_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_RTS_N_IO23                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_UART0_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO23                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_UART1_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_CS2_IO23                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_SPI0_CS2_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG23_IO23                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_DBG23_BITSVAL                                                                                ))
#define PIN_TYPE_APS_UART_TXD_IO23                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_APS_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_RTS_N_IO23                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_MSQ_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD2_IO23                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_SPI3_TXD2_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_WS_OUT_IO23                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_I2S_WS_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_CAM_SHUTTER_IO23                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO23_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO23_BITSVAL | PIN_OUTGRP_VAL_IO23_CAM_SHUTTER_BITSVAL                                                                          ))

/* IO24 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_PWM4_IO24                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_PWM4_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SDA_OUT_IO24                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_I2C_SDA_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_RTS_N_IO24                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_UART1_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PDM_CLK_IO24                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_PDM_CLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_DBG24_IO24                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_DBG24_BITSVAL                                                                                ))
#define PIN_TYPE_TDO_IO24                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_TDO_BITSVAL                                                                                  ))
#define PIN_TYPE_CLK_DS_AUX_78K125_IO24                 ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_CLK_DS_AUX_78K125_BITSVAL  | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_MSQ_SWDO_IO24                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_MSQ_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_APS_UART_TXD_IO24                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_APS_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_TXD_IO24                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_MSQ_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CLK_IO24                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_SPI3_CLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_I2S_SCLK_IO24                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_I2S_SCLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_CAM_HSYNC_OUT_IO24                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO24_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO24_BITSVAL | PIN_OUTGRP_VAL_IO24_CAM_HSYNC_OUT_BITSVAL                                                                        ))

/* IO25 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_PWM5_IO25                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO25_BITSVAL | PIN_OUTGRP_VAL_IO25_PWM5_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SCL_OUT_IO25                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO25_BITSVAL | PIN_OUTGRP_VAL_IO25_I2C_SCL_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG25_IO25                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO25_BITSVAL | PIN_OUTGRP_VAL_IO25_DBG25_BITSVAL                                                                                ))
#define PIN_TYPE_APS_SWDO_IO25                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO25_BITSVAL | PIN_OUTGRP_VAL_IO25_APS_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_SPI3_CS0_IO25                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO25_BITSVAL | PIN_OUTGRP_VAL_IO25_SPI3_CS0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_WS_OUT_IO25                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO25_BITSVAL | PIN_OUTGRP_VAL_IO25_I2S_WS_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT0_IO25                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO25_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO25_BITSVAL | PIN_OUTGRP_VAL_IO25_PU_EPA_OUT0_BITSVAL                                                                          ))

/* IO26 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI2_TXD_IO26                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_SPI2_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM0_IO26                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_PWM0_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SDA_OUT_IO26                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_I2C_SDA_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_RTS_N_IO26                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_UART0_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_RTS_N_IO26                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_UART1_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CS0_IO26                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_SPI3_CS0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG26_IO26                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_DBG26_BITSVAL                                                                                ))
#define PIN_TYPE_MSQ_SWDO_IO26                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_MSQ_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_APS_UART_RTS_N_IO26                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_APS_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_TXD_IO26                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_MSQ_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT0_IO26                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_PU_EPA_OUT0_BITSVAL                                                                          ))
#define PIN_TYPE_CAM_VSYNC_OUT_IO26                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO26_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO26_BITSVAL | PIN_OUTGRP_VAL_IO26_CAM_VSYNC_OUT_BITSVAL                                                                        ))


/* IO27 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CS2_IO27                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO27_BITSVAL | PIN_OUTGRP_VAL_IO27_SPI0_CS2_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM1_IO27                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO27_BITSVAL | PIN_OUTGRP_VAL_IO27_PWM1_BITSVAL                                                                                 ))
#define PIN_TYPE_I2C_SCL_OUT_IO27                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO27_BITSVAL | PIN_OUTGRP_VAL_IO27_I2C_SCL_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CS2_IO27                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO27_BITSVAL | PIN_OUTGRP_VAL_IO27_SPI3_CS2_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG27_IO27                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO27_BITSVAL | PIN_OUTGRP_VAL_IO27_DBG27_BITSVAL                                                                                ))
#define PIN_TYPE_APS_SWDO_IO27                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO27_BITSVAL | PIN_OUTGRP_VAL_IO27_APS_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_SPI3_TXD0_IO27                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO27_BITSVAL | PIN_OUTGRP_VAL_IO27_SPI3_TXD0_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SCLK_IO27                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO27_BITSVAL | PIN_OUTGRP_VAL_IO27_I2S_SCLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_PU_EPA_OUT1_IO27                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO27_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO27_BITSVAL | PIN_OUTGRP_VAL_IO27_PU_EPA_OUT1_BITSVAL                                                                          ))

/* IO28 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI2_TXD_IO28                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO28_BITSVAL | PIN_OUTGRP_VAL_IO28_SPI2_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PWM2_IO28                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO28_BITSVAL | PIN_OUTGRP_VAL_IO28_PWM2_BITSVAL                                                                                 ))
#define PIN_TYPE_DT_DS_AUX_IO28                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO28_BITSVAL | PIN_OUTGRP_VAL_IO28_DT_DS_AUX_BITSVAL                                                                            ))
#define PIN_TYPE_UART0_TXD_IO28                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO28_BITSVAL | PIN_OUTGRP_VAL_IO28_UART0_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CLK_IO28                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO28_BITSVAL | PIN_OUTGRP_VAL_IO28_SPI3_CLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_DBG28_IO28                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO28_BITSVAL | PIN_OUTGRP_VAL_IO28_DBG28_BITSVAL                                                                                ))
#define PIN_TYPE_MSQ_SWDO_IO28                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO28_BITSVAL | PIN_OUTGRP_VAL_IO28_MSQ_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_SPI3_TXD2_IO28                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO28_BITSVAL | PIN_OUTGRP_VAL_IO28_SPI3_TXD2_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SDO0_IO28                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO28_BITSVAL | PIN_OUTGRP_VAL_IO28_I2S_SDO0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_EXT_2IO_CLK_IO28                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO28_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO28_BITSVAL | PIN_OUTGRP_VAL_IO28_EXT_2IO_CLK_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))

/* IO29 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI2_TXD_IO29                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO29_BITSVAL | PIN_OUTGRP_VAL_IO29_SPI2_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PDM_CLK_IO29                           ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO29_BITSVAL | PIN_OUTGRP_VAL_IO29_PDM_CLK_BITSVAL            | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_I2C_SDA_OUT_IO29                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO29_BITSVAL | PIN_OUTGRP_VAL_IO29_I2C_SDA_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD0_IO29                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO29_BITSVAL | PIN_OUTGRP_VAL_IO29_SPI3_TXD0_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG29_IO29                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO29_BITSVAL | PIN_OUTGRP_VAL_IO29_DBG29_BITSVAL                                                                                ))
#define PIN_TYPE_APS_SWDO_IO29                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO29_BITSVAL | PIN_OUTGRP_VAL_IO29_APS_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_APS_UART_RTS_N_IO29                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO29_BITSVAL | PIN_OUTGRP_VAL_IO29_APS_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD1_IO29                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO29_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO29_BITSVAL | PIN_OUTGRP_VAL_IO29_SPI3_TXD1_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))

/* IO30 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_CS1_IO30                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO30_BITSVAL | PIN_OUTGRP_VAL_IO30_SPI1_CS1_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_OTP_PROGRAM_PERIOD_IO30                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO30_BITSVAL | PIN_OUTGRP_VAL_IO30_OTP_PROGRAM_PERIOD_BITSVAL                                                                   ))
#define PIN_TYPE_I2C_SDA_OUT_IO30                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO30_BITSVAL | PIN_OUTGRP_VAL_IO30_I2C_SDA_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_RTS_N_IO30                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO30_BITSVAL | PIN_OUTGRP_VAL_IO30_UART1_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_CS3_IO30                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO30_BITSVAL | PIN_OUTGRP_VAL_IO30_SPI0_CS3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG30_IO30                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO30_BITSVAL | PIN_OUTGRP_VAL_IO30_DBG30_BITSVAL                                                                                ))
#define PIN_TYPE_MSQ_SWDO_IO30                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO30_BITSVAL | PIN_OUTGRP_VAL_IO30_MSQ_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_MSQ_UART_RTS_N_IO30                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO30_BITSVAL | PIN_OUTGRP_VAL_IO30_MSQ_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CS1_IO30                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO30_BITSVAL | PIN_OUTGRP_VAL_IO30_SPI3_CS1_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT2_IO30                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO30_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO30_BITSVAL | PIN_OUTGRP_VAL_IO30_PU_EPA_OUT2_BITSVAL                                                                          ))

/* IO31 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_I2C_SCL_OUT_IO31                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_I2C_SCL_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_TXD_IO31                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_UART0_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO31                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_UART1_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG31_IO31                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_DBG31_BITSVAL                                                                                ))
#define PIN_TYPE_TDO_IO31                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_TDO_BITSVAL                                                                                  ))
#define PIN_TYPE_APS_SWDO_IO31                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_APS_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_APS_UART_TXD_IO31                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_APS_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_RTS_N_IO31                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_MSQ_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD3_IO31                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_SPI3_TXD3_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SDO0_IO31                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_I2S_SDO0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT3_IO31                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO31_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO31_BITSVAL | PIN_OUTGRP_VAL_IO31_PU_EPA_OUT3_BITSVAL                                                                          ))

/* IO32 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI2_TXD_IO32                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO32_BITSVAL | PIN_OUTGRP_VAL_IO32_SPI2_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_OTP_PROGRAM_PERIOD_IO32                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO32_BITSVAL | PIN_OUTGRP_VAL_IO32_OTP_PROGRAM_PERIOD_BITSVAL                                                                   ))
#define PIN_TYPE_I2C_SCL_OUT_IO32                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO32_BITSVAL | PIN_OUTGRP_VAL_IO32_I2C_SCL_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_TXD_IO32                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO32_BITSVAL | PIN_OUTGRP_VAL_IO32_UART0_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD2_IO32                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO32_BITSVAL | PIN_OUTGRP_VAL_IO32_SPI3_TXD2_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG0_IO32                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO32_BITSVAL | PIN_OUTGRP_VAL_IO32_DBG0_BITSVAL                                                                                 ))
#define PIN_TYPE_MSQ_SWDO_IO32                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO32_BITSVAL | PIN_OUTGRP_VAL_IO32_MSQ_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_I2S_WS_OUT_IO32                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO32_BITSVAL | PIN_OUTGRP_VAL_IO32_I2S_WS_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_PU_EPA_OUT1_IO32                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO32_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO32_BITSVAL | PIN_OUTGRP_VAL_IO32_PU_EPA_OUT1_BITSVAL                                                                          ))

/* IO33 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI2_TXD_IO33                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO33_BITSVAL | PIN_OUTGRP_VAL_IO33_SPI2_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2C_SDA_OUT_IO33                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO33_BITSVAL | PIN_OUTGRP_VAL_IO33_I2C_SDA_OUT_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD1_IO33                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO33_BITSVAL | PIN_OUTGRP_VAL_IO33_SPI3_TXD1_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG1_IO33                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO33_BITSVAL | PIN_OUTGRP_VAL_IO33_DBG1_BITSVAL                                                                                 ))
#define PIN_TYPE_TDO_IO33                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO33_BITSVAL | PIN_OUTGRP_VAL_IO33_TDO_BITSVAL                                                                                  ))
#define PIN_TYPE_APS_SWDO_IO33                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO33_BITSVAL | PIN_OUTGRP_VAL_IO33_APS_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_I2S_SCLK_IO33                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO33_BITSVAL | PIN_OUTGRP_VAL_IO33_I2S_SCLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_PU_EPA_OUT2_IO33                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO33_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO33_BITSVAL | PIN_OUTGRP_VAL_IO33_PU_EPA_OUT2_BITSVAL                                                                          ))

/* IO34 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_UART0_RTS_N_IO34                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO34_BITSVAL | PIN_OUTGRP_VAL_IO34_UART0_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_RTS_N_IO34                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO34_BITSVAL | PIN_OUTGRP_VAL_IO34_UART1_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CS3_IO34                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO34_BITSVAL | PIN_OUTGRP_VAL_IO34_SPI3_CS3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG2_IO34                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO34_BITSVAL | PIN_OUTGRP_VAL_IO34_DBG2_BITSVAL                                                                                 ))
#define PIN_TYPE_APS_UART_TXD_IO34                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO34_BITSVAL | PIN_OUTGRP_VAL_IO34_APS_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_TXD_IO34                      ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO34_BITSVAL | PIN_OUTGRP_VAL_IO34_MSQ_UART_TXD_BITSVAL       | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD1_IO34                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO34_BITSVAL | PIN_OUTGRP_VAL_IO34_SPI3_TXD1_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_WS_OUT_IO34                        ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO34_BITSVAL | PIN_OUTGRP_VAL_IO34_I2S_WS_OUT_BITSVAL         | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_CAM_HSYNC_OUT_IO34                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO34_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO34_BITSVAL | PIN_OUTGRP_VAL_IO34_CAM_HSYNC_OUT_BITSVAL                                                                        ))


/* IO35 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_OTP_PROGRAM_PERIOD_IO35                ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO35_BITSVAL | PIN_OUTGRP_VAL_IO35_OTP_PROGRAM_PERIOD_BITSVAL                                                                   ))
#define PIN_TYPE_UART0_TXD_IO35                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO35_BITSVAL | PIN_OUTGRP_VAL_IO35_UART0_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_RTS_N_IO35                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO35_BITSVAL | PIN_OUTGRP_VAL_IO35_UART1_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD0_IO35                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO35_BITSVAL | PIN_OUTGRP_VAL_IO35_SPI3_TXD0_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG3_IO35                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO35_BITSVAL | PIN_OUTGRP_VAL_IO35_DBG3_BITSVAL                                                                                 ))
#define PIN_TYPE_APS_SWDO_IO35                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO35_BITSVAL | PIN_OUTGRP_VAL_IO35_APS_SWDO_BITSVAL                                                                             ))
#define PIN_TYPE_MSQ_UART_RTS_N_IO35                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO35_BITSVAL | PIN_OUTGRP_VAL_IO35_MSQ_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SDO0_IO35                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO35_BITSVAL | PIN_OUTGRP_VAL_IO35_I2S_SDO0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_CAM_VSYNC_OUT_IO35                     ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO35_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO35_BITSVAL | PIN_OUTGRP_VAL_IO35_CAM_VSYNC_OUT_BITSVAL                                                                        ))

/* IO36 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI2_TXD_IO36                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO36_BITSVAL | PIN_OUTGRP_VAL_IO36_SPI2_TXD_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_CLK_DS_AUX_78K125_IO36                 ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO36_BITSVAL | PIN_OUTGRP_VAL_IO36_CLK_DS_AUX_78K125_BITSVAL  | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_UART0_TXD_IO36                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO36_BITSVAL | PIN_OUTGRP_VAL_IO36_UART0_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_TXD_IO36                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO36_BITSVAL | PIN_OUTGRP_VAL_IO36_UART1_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD1_IO36                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO36_BITSVAL | PIN_OUTGRP_VAL_IO36_SPI3_TXD1_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG4_IO36                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO36_BITSVAL | PIN_OUTGRP_VAL_IO36_DBG4_BITSVAL                                                                                 ))
#define PIN_TYPE_TDO_IO36                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO36_BITSVAL | PIN_OUTGRP_VAL_IO36_TDO_BITSVAL                                                                                  ))
#define PIN_TYPE_APS_UART_RTS_N_IO36                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO36_BITSVAL | PIN_OUTGRP_VAL_IO36_APS_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_TXD0_IO36                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO36_BITSVAL | PIN_OUTGRP_VAL_IO36_SPI3_TXD0_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_CAM_FREX_IO36                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO36_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO36_BITSVAL | PIN_OUTGRP_VAL_IO36_CAM_FREX_BITSVAL                                                                             ))

/* IO37 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI1_CS1_IO37                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO37_BITSVAL | PIN_OUTGRP_VAL_IO37_SPI1_CS1_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_RTS_N_IO37                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO37_BITSVAL | PIN_OUTGRP_VAL_IO37_UART0_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG5_IO37                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO37_BITSVAL | PIN_OUTGRP_VAL_IO37_DBG5_BITSVAL                                                                                 ))
#define PIN_TYPE_APS_UART_RTS_N_IO37                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO37_BITSVAL | PIN_OUTGRP_VAL_IO37_APS_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_MSQ_UART_RTS_N_IO37                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO37_BITSVAL | PIN_OUTGRP_VAL_IO37_MSQ_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CS2_IO37                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO37_BITSVAL | PIN_OUTGRP_VAL_IO37_SPI3_CS2_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SCLK_IO37                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO37_BITSVAL | PIN_OUTGRP_VAL_IO37_I2S_SCLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_PU_EPA_OUT3_IO37                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO37_BITSVAL | PIN_OUTGRP_VAL_IO37_PU_EPA_OUT3_BITSVAL                                                                          ))
#define PIN_TYPE_CAM_EXPST_IO37                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO37_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO37_BITSVAL | PIN_OUTGRP_VAL_IO37_CAM_EXPST_BITSVAL                                                                            ))

/* IO38 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CS3_IO38                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO38_BITSVAL | PIN_OUTGRP_VAL_IO38_SPI0_CS3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI3_CLK_IO38                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO38_BITSVAL | PIN_OUTGRP_VAL_IO38_SPI3_CLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_SPI3_CS3_IO38                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO38_BITSVAL | PIN_OUTGRP_VAL_IO38_SPI3_CS3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART0_RTS_N_IO38                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO38_BITSVAL | PIN_OUTGRP_VAL_IO38_UART0_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG6_IO38                              ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO38_BITSVAL | PIN_OUTGRP_VAL_IO38_DBG6_BITSVAL                                                                                 ))
#define PIN_TYPE_APS_UART_RTS_N_IO38                    ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO38_BITSVAL | PIN_OUTGRP_VAL_IO38_APS_UART_RTS_N_BITSVAL     | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_I2S_SDO0_IO38                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO38_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO38_BITSVAL | PIN_OUTGRP_VAL_IO38_I2S_SDO0_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))

/* IO39 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_TXD0_IO39                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO39_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO39_BITSVAL | PIN_OUTGRP_VAL_IO39_SPI0_TXD0_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG12_IO39                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO39_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO39_BITSVAL | PIN_OUTGRP_VAL_IO39_DBG12_BITSVAL                                                                                ))

/* IO40 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_TXD1_IO40                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO40_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO40_BITSVAL | PIN_OUTGRP_VAL_IO40_SPI0_TXD1_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG12_IO40                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO40_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO40_BITSVAL | PIN_OUTGRP_VAL_IO40_DBG12_BITSVAL                                                                                ))

/* IO41 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_TXD2_IO41                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO41_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO41_BITSVAL | PIN_OUTGRP_VAL_IO41_SPI0_TXD2_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG12_IO41                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO41_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO41_BITSVAL | PIN_OUTGRP_VAL_IO41_DBG12_BITSVAL                                                                                ))

/* IO42 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_TXD3_IO42                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO42_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO42_BITSVAL | PIN_OUTGRP_VAL_IO42_SPI0_TXD3_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG12_IO42                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO42_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO42_BITSVAL | PIN_OUTGRP_VAL_IO42_DBG12_BITSVAL                                                                                ))

/* IO43 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CS1_IO43                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO43_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO43_BITSVAL | PIN_OUTGRP_VAL_IO43_SPI0_CS1_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_CS2_IO43                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO43_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO43_BITSVAL | PIN_OUTGRP_VAL_IO43_SPI0_CS2_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_SPI0_CS3_IO43                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO43_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO43_BITSVAL | PIN_OUTGRP_VAL_IO43_SPI0_CS3_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_UART1_RTS_N_IO43                       ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO43_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO43_BITSVAL | PIN_OUTGRP_VAL_IO43_UART1_RTS_N_BITSVAL        | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG15_IO43                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO43_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO43_BITSVAL | PIN_OUTGRP_VAL_IO43_DBG15_BITSVAL                                                                                ))

/* IO44 Output                                                      Peripheral                   IO#                       Output enable                   Output group register offset   Output group register value                      Auto control enable               Auto control level            */
#define PIN_TYPE_SPI0_CLK_IO44                          ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO44_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO44_BITSVAL | PIN_OUTGRP_VAL_IO44_SPI0_CLK_BITSVAL           | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_LOW_BITSVAL ))
#define PIN_TYPE_UART1_TXD_IO44                         ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO44_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO44_BITSVAL | PIN_OUTGRP_VAL_IO44_UART1_TXD_BITSVAL          | PIN_AUTO_CTRL_EN_ENABLE_BITSVAL | PIN_AUTO_CTRL_LVL_HIGH_BITSVAL))
#define PIN_TYPE_DBG11_IO44                             ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO44_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO44_BITSVAL | PIN_OUTGRP_VAL_IO44_DBG11_BITSVAL                                                                                ))
#define PIN_TYPE_TDO_IO44                               ((uint64_t)(PIN_IO_TYPE_PERIPH_BITSVAL | PIN_IO_NUM_IO44_BITSVAL | PIN_OUT_ENABLE_ENABLE_BITSVAL | PIN_OUTGRP_OFST_IO44_BITSVAL | PIN_OUTGRP_VAL_IO44_TDO_BITSVAL                                                                                  ))

/* WARNING: Cannot use the value alone. 
 * Using (PIN_TYPE_xxxx | PIN_INMODE_IOxx_xxxx) as parameter */
#define PIN_INMODE_IO0_FLOATING                         ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO0_PULL_UP                          ((uint64_t)(PIN_INPUT_IO0_UP    << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO1_FLOATING                         ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO1_PULL_UP                          ((uint64_t)(PIN_INPUT_IO1_UP    << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO2_FLOATING                         ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO2_PULL_UP                          ((uint64_t)(PIN_INPUT_IO2_UP    << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO3_FLOATING                         ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO3_PULL_DOWN                        ((uint64_t)(PIN_INPUT_IO3_DOWN  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO4_FLOATING                         ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO4_PULL_UP                          ((uint64_t)(PIN_INPUT_IO4_UP    << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO5_FLOATING                         ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO5_PULL_UP                          ((uint64_t)(PIN_INPUT_IO5_UP    << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO6_FLOATING                         ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO6_PULL_DOWN                        ((uint64_t)(PIN_INPUT_IO6_DOWN  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO7_FLOATING                         ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO7_PULL_UP                          ((uint64_t)(PIN_INPUT_IO7_UP    << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO8_FLOATING                         ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO8_PULL_UP                          ((uint64_t)(PIN_INPUT_IO8_UP    << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO9_FLOATING                         ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO9_PULL_UP                          ((uint64_t)(PIN_INPUT_IO9_UP    << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO10_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO10_PULL_UP                         ((uint64_t)(PIN_INPUT_IO10_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO11_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO11_PULL_UP                         ((uint64_t)(PIN_INPUT_IO11_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO12_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO12_PULL_UP                         ((uint64_t)(PIN_INPUT_IO12_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO13_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO13_PULL_DOWN                       ((uint64_t)(PIN_INPUT_IO13_DOWN << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO14_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO14_PULL_UP                         ((uint64_t)(PIN_INPUT_IO14_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO15_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO15_PULL_UP                         ((uint64_t)(PIN_INPUT_IO15_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO16_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO16_PULL_DOWN                       ((uint64_t)(PIN_INPUT_IO16_DOWN << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO17_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO17_PULL_DOWN                       ((uint64_t)(PIN_INPUT_IO17_DOWN << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO18_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO18_PULL_UP                         ((uint64_t)(PIN_INPUT_IO18_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO19_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO19_PULL_UP                         ((uint64_t)(PIN_INPUT_IO19_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO20_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO20_PULL_DOWN                       ((uint64_t)(PIN_INPUT_IO20_DOWN << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO21_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO21_PULL_DOWN                       ((uint64_t)(PIN_INPUT_IO21_DOWN << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO22_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO22_PULL_UP                         ((uint64_t)(PIN_INPUT_IO22_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO23_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO23_PULL_UP                         ((uint64_t)(PIN_INPUT_IO23_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO24_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO24_PULL_UP                         ((uint64_t)(PIN_INPUT_IO24_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO25_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO25_PULL_UP                         ((uint64_t)(PIN_INPUT_IO25_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO26_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO26_PULL_UP                         ((uint64_t)(PIN_INPUT_IO26_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO27_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO27_PULL_UP                         ((uint64_t)(PIN_INPUT_IO27_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO28_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO28_PULL_UP                         ((uint64_t)(PIN_INPUT_IO28_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO29_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO29_PULL_UP                         ((uint64_t)(PIN_INPUT_IO29_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO30_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO30_PULL_UP                         ((uint64_t)(PIN_INPUT_IO30_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO31_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO31_PULL_UP                         ((uint64_t)(PIN_INPUT_IO31_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO32_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO32_PULL_UP                         ((uint64_t)(PIN_INPUT_IO32_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO33_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO33_PULL_UP                         ((uint64_t)(PIN_INPUT_IO33_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO34_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO34_PULL_UP                         ((uint64_t)(PIN_INPUT_IO34_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO35_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO35_PULL_DOWN                       ((uint64_t)(PIN_INPUT_IO35_DOWN << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO36_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO36_PULL_UP                         ((uint64_t)(PIN_INPUT_IO36_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO37_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO37_PULL_UP                         ((uint64_t)(PIN_INPUT_IO37_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO38_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO38_PULL_UP                         ((uint64_t)(PIN_INPUT_IO38_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO39_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO39_PULL_UP                         ((uint64_t)(PIN_INPUT_IO39_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO40_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO40_PULL_UP                         ((uint64_t)(PIN_INPUT_IO40_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO41_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO41_PULL_UP                         ((uint64_t)(PIN_INPUT_IO41_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO42_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO42_PULL_UP                         ((uint64_t)(PIN_INPUT_IO42_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO43_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO43_PULL_UP                         ((uint64_t)(PIN_INPUT_IO43_UP   << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO44_FLOATING                        ((uint64_t)(PIN_INPUT_FLOATING  << PIN_INPUT_MODE_BIT))
#define PIN_INMODE_IO44_PULL_UP                         ((uint64_t)(PIN_INPUT_IO44_UP   << PIN_INPUT_MODE_BIT))

/* WARNING: Cannot use the value alone. 
 * Using (PIN_TYPE_xxxx | PIN_DRVCRNT_IOxx_xxmA) as parameter */
#define PIN_DRVCRNT_IO0_2mA                             ((uint64_t)(PIN_OUT_IO0_DRV_2mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO0_4mA                             ((uint64_t)(PIN_OUT_IO0_DRV_4mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO1_2mA                             ((uint64_t)(PIN_OUT_IO1_DRV_2mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO1_4mA                             ((uint64_t)(PIN_OUT_IO1_DRV_4mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO2_2mA                             ((uint64_t)(PIN_OUT_IO2_DRV_2mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO2_4mA                             ((uint64_t)(PIN_OUT_IO2_DRV_4mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO3_2mA                             ((uint64_t)(PIN_OUT_IO3_DRV_2mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO3_4mA                             ((uint64_t)(PIN_OUT_IO3_DRV_4mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO4_2mA                             ((uint64_t)(PIN_OUT_IO4_DRV_2mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO4_4mA                             ((uint64_t)(PIN_OUT_IO4_DRV_4mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO5_2mA                             ((uint64_t)(PIN_OUT_IO5_DRV_2mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO5_4mA                             ((uint64_t)(PIN_OUT_IO5_DRV_4mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO6_2mA                             ((uint64_t)(PIN_OUT_IO6_DRV_2mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO6_4mA                             ((uint64_t)(PIN_OUT_IO6_DRV_4mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO7_2mA                             ((uint64_t)(PIN_OUT_IO7_DRV_2mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO7_4mA                             ((uint64_t)(PIN_OUT_IO7_DRV_4mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO8_2mA                             ((uint64_t)(PIN_OUT_IO8_DRV_2mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO8_4mA                             ((uint64_t)(PIN_OUT_IO8_DRV_4mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO9_2mA                             ((uint64_t)(PIN_OUT_IO9_DRV_2mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO9_4mA                             ((uint64_t)(PIN_OUT_IO9_DRV_4mA   << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO10_4mA                            ((uint64_t)(PIN_OUT_IO10_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO10_8mA                            ((uint64_t)(PIN_OUT_IO10_DRV_8mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO11_4mA                            ((uint64_t)(PIN_OUT_IO11_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO11_8mA                            ((uint64_t)(PIN_OUT_IO11_DRV_8mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO12_4mA                            ((uint64_t)(PIN_OUT_IO12_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO12_8mA                            ((uint64_t)(PIN_OUT_IO12_DRV_8mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO13_4mA                            ((uint64_t)(PIN_OUT_IO13_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO13_8mA                            ((uint64_t)(PIN_OUT_IO13_DRV_8mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO14_4mA                            ((uint64_t)(PIN_OUT_IO14_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO14_8mA                            ((uint64_t)(PIN_OUT_IO14_DRV_8mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO15_4mA                            ((uint64_t)(PIN_OUT_IO15_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO15_8mA                            ((uint64_t)(PIN_OUT_IO15_DRV_8mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO16_12mA                           ((uint64_t)(PIN_OUT_IO16_DRV_12mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO16_16mA                           ((uint64_t)(PIN_OUT_IO16_DRV_16mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO17_12mA                           ((uint64_t)(PIN_OUT_IO17_DRV_12mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO17_16mA                           ((uint64_t)(PIN_OUT_IO17_DRV_16mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO18_12mA                           ((uint64_t)(PIN_OUT_IO18_DRV_12mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO18_16mA                           ((uint64_t)(PIN_OUT_IO18_DRV_16mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO19_12mA                           ((uint64_t)(PIN_OUT_IO19_DRV_12mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO19_16mA                           ((uint64_t)(PIN_OUT_IO19_DRV_16mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO20_12mA                           ((uint64_t)(PIN_OUT_IO20_DRV_12mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO20_16mA                           ((uint64_t)(PIN_OUT_IO20_DRV_16mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO21_12mA                           ((uint64_t)(PIN_OUT_IO21_DRV_12mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO21_16mA                           ((uint64_t)(PIN_OUT_IO21_DRV_16mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO22_12mA                           ((uint64_t)(PIN_OUT_IO22_DRV_12mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO22_16mA                           ((uint64_t)(PIN_OUT_IO22_DRV_16mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO23_12mA                           ((uint64_t)(PIN_OUT_IO23_DRV_12mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO23_16mA                           ((uint64_t)(PIN_OUT_IO23_DRV_16mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO24_12mA                           ((uint64_t)(PIN_OUT_IO24_DRV_12mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO24_16mA                           ((uint64_t)(PIN_OUT_IO24_DRV_16mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO25_12mA                           ((uint64_t)(PIN_OUT_IO25_DRV_12mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO25_16mA                           ((uint64_t)(PIN_OUT_IO25_DRV_16mA << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO26_2mA                            ((uint64_t)(PIN_OUT_IO26_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO26_4mA                            ((uint64_t)(PIN_OUT_IO26_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO27_2mA                            ((uint64_t)(PIN_OUT_IO27_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO27_4mA                            ((uint64_t)(PIN_OUT_IO27_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO28_2mA                            ((uint64_t)(PIN_OUT_IO28_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO28_4mA                            ((uint64_t)(PIN_OUT_IO28_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO29_2mA                            ((uint64_t)(PIN_OUT_IO29_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO29_4mA                            ((uint64_t)(PIN_OUT_IO29_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO30_2mA                            ((uint64_t)(PIN_OUT_IO30_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO30_4mA                            ((uint64_t)(PIN_OUT_IO30_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO31_2mA                            ((uint64_t)(PIN_OUT_IO31_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO31_4mA                            ((uint64_t)(PIN_OUT_IO31_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO32_2mA                            ((uint64_t)(PIN_OUT_IO32_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO32_4mA                            ((uint64_t)(PIN_OUT_IO32_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO33_2mA                            ((uint64_t)(PIN_OUT_IO33_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO33_4mA                            ((uint64_t)(PIN_OUT_IO33_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO34_2mA                            ((uint64_t)(PIN_OUT_IO34_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO34_4mA                            ((uint64_t)(PIN_OUT_IO34_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO35_2mA                            ((uint64_t)(PIN_OUT_IO35_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO35_4mA                            ((uint64_t)(PIN_OUT_IO35_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO36_2mA                            ((uint64_t)(PIN_OUT_IO36_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO36_4mA                            ((uint64_t)(PIN_OUT_IO36_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO37_2mA                            ((uint64_t)(PIN_OUT_IO37_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO37_4mA                            ((uint64_t)(PIN_OUT_IO37_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO38_2mA                            ((uint64_t)(PIN_OUT_IO38_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO38_4mA                            ((uint64_t)(PIN_OUT_IO38_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO39_2mA                            ((uint64_t)(PIN_OUT_IO39_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO39_4mA                            ((uint64_t)(PIN_OUT_IO39_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO40_2mA                            ((uint64_t)(PIN_OUT_IO40_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO40_4mA                            ((uint64_t)(PIN_OUT_IO40_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO41_2mA                            ((uint64_t)(PIN_OUT_IO41_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO41_4mA                            ((uint64_t)(PIN_OUT_IO41_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO42_2mA                            ((uint64_t)(PIN_OUT_IO42_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO42_4mA                            ((uint64_t)(PIN_OUT_IO42_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO43_2mA                            ((uint64_t)(PIN_OUT_IO43_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO43_4mA                            ((uint64_t)(PIN_OUT_IO43_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO44_2mA                            ((uint64_t)(PIN_OUT_IO44_DRV_2mA  << PIN_OUT_DRV_CURR_BIT))
#define PIN_DRVCRNT_IO44_4mA                            ((uint64_t)(PIN_OUT_IO44_DRV_4mA  << PIN_OUT_DRV_CURR_BIT))




/**************************************************************/

#define PIN_TYPE_I2C_SCL_IO_IO0                         (PIN_TYPE_I2C_SCL_IN_IO0  | PIN_TYPE_I2C_SCL_OUT_IO0 )
#define PIN_TYPE_APS_SWDIO_IO0                          (PIN_TYPE_APS_SWDI_IO0    | PIN_TYPE_APS_SWDO_IO0    )
#define PIN_TYPE_SPI3_IO3_IO0                           (PIN_TYPE_SPI3_RXD3_IO0   | PIN_TYPE_SPI3_TXD3_IO0   )
#define PIN_TYPE_I2C_SCL_IO_IO1                         (PIN_TYPE_I2C_SCL_IN_IO1  | PIN_TYPE_I2C_SCL_OUT_IO1 )
#define PIN_TYPE_APS_SWDIO_IO1                          (PIN_TYPE_APS_SWDI_IO1    | PIN_TYPE_APS_SWDO_IO1    )
#define PIN_TYPE_SPI3_IO3_IO1                           (PIN_TYPE_SPI3_RXD3_IO1   | PIN_TYPE_SPI3_TXD3_IO1   )
#define PIN_TYPE_I2C_SDA_IO_IO2                         (PIN_TYPE_I2C_SDA_IN_IO2  | PIN_TYPE_I2C_SDA_OUT_IO2 )
#define PIN_TYPE_MSQ_SWDIO_IO2                          (PIN_TYPE_MSQ_SWDI_IO2    | PIN_TYPE_MSQ_SWDO_IO2    )
#define PIN_TYPE_SPI3_IO2_IO2                           (PIN_TYPE_SPI3_RXD2_IO2   | PIN_TYPE_SPI3_TXD2_IO2   )
#define PIN_TYPE_I2C_SCL_IO_IO4                         (PIN_TYPE_I2C_SCL_IN_IO4  | PIN_TYPE_I2C_SCL_OUT_IO4 )
#define PIN_TYPE_APS_SWDIO_IO4                          (PIN_TYPE_APS_SWDI_IO4    | PIN_TYPE_APS_SWDO_IO4    )
#define PIN_TYPE_SPI0_IO3_IO4                           (PIN_TYPE_SPI0_RXD3_IO4   | PIN_TYPE_SPI0_TXD3_IO4   )
#define PIN_TYPE_SPI1_IO3_IO4                           (PIN_TYPE_SPI1_RXD3_IO4   | PIN_TYPE_SPI1_TXD3_IO4   )
#define PIN_TYPE_SPI3_IO1_IO4                           (PIN_TYPE_SPI3_RXD1_IO4   | PIN_TYPE_SPI3_TXD1_IO4   )
#define PIN_TYPE_I2C_SDA_IO_IO5                         (PIN_TYPE_I2C_SDA_IN_IO5  | PIN_TYPE_I2C_SDA_OUT_IO5 )
#define PIN_TYPE_SPI0_IO2_IO5                           (PIN_TYPE_SPI0_RXD2_IO5   | PIN_TYPE_SPI0_TXD2_IO5   )
#define PIN_TYPE_SPI1_IO2_IO5                           (PIN_TYPE_SPI1_RXD2_IO5   | PIN_TYPE_SPI1_TXD2_IO5   )
#define PIN_TYPE_SPI3_IO0_IO5                           (PIN_TYPE_SPI3_RXD0_IO5   | PIN_TYPE_SPI3_TXD0_IO5   )
#define PIN_TYPE_MSQ_SWDIO_IO6                          (PIN_TYPE_MSQ_SWDI_IO6    | PIN_TYPE_MSQ_SWDO_IO6    )
#define PIN_TYPE_APS_SWDIO_IO7                          (PIN_TYPE_APS_SWDI_IO7    | PIN_TYPE_APS_SWDO_IO7    )
#define PIN_TYPE_SPI0_IO1_IO7                           (PIN_TYPE_SPI0_RXD1_IO7   | PIN_TYPE_SPI0_TXD1_IO7   )
#define PIN_TYPE_SPI1_IO1_IO7                           (PIN_TYPE_SPI1_RXD1_IO7   | PIN_TYPE_SPI1_TXD1_IO7   )
#define PIN_TYPE_I2C_SCL_IO_IO8                         (PIN_TYPE_I2C_SCL_IN_IO8  | PIN_TYPE_I2C_SCL_OUT_IO8 )
#define PIN_TYPE_MSQ_SWDIO_IO8                          (PIN_TYPE_MSQ_SWDI_IO8    | PIN_TYPE_MSQ_SWDO_IO8    )
#define PIN_TYPE_SPI0_IO0_IO8                           (PIN_TYPE_SPI0_RXD0_IO8   | PIN_TYPE_SPI0_TXD0_IO8   )
#define PIN_TYPE_SPI1_IO0_IO8                           (PIN_TYPE_SPI1_RXD0_IO8   | PIN_TYPE_SPI1_TXD0_IO8   )
#define PIN_TYPE_I2C_SDA_IO_IO9                         (PIN_TYPE_I2C_SDA_IN_IO9  | PIN_TYPE_I2C_SDA_OUT_IO9 )
#define PIN_TYPE_I2C_SCL_IO_IO10                        (PIN_TYPE_I2C_SCL_IN_IO10 | PIN_TYPE_I2C_SCL_OUT_IO10)
#define PIN_TYPE_SPI0_IO2_IO10                          (PIN_TYPE_SPI0_RXD2_IO10  | PIN_TYPE_SPI0_TXD2_IO10  )
#define PIN_TYPE_SPI1_IO2_IO10                          (PIN_TYPE_SPI1_RXD2_IO10  | PIN_TYPE_SPI1_TXD2_IO10  )
#define PIN_TYPE_I2C_SDA_IO_IO11                        (PIN_TYPE_I2C_SDA_IN_IO11 | PIN_TYPE_I2C_SDA_OUT_IO11)
#define PIN_TYPE_SPI0_IO3_IO11                          (PIN_TYPE_SPI0_RXD3_IO11  | PIN_TYPE_SPI0_TXD3_IO11  )
#define PIN_TYPE_SPI1_IO3_IO11                          (PIN_TYPE_SPI1_RXD3_IO11  | PIN_TYPE_SPI1_TXD3_IO11  )
#define PIN_TYPE_CAM_VSYNC_IO12                         (PIN_TYPE_CAM_VSYNC_IN_IO12 | PIN_TYPE_CAM_VSYNC_OUT_IO12)
#define PIN_TYPE_I2C_SCL_IO_IO14                        (PIN_TYPE_I2C_SCL_IN_IO14 | PIN_TYPE_I2C_SCL_OUT_IO14)
#define PIN_TYPE_MSQ_SWDIO_IO14                         (PIN_TYPE_MSQ_SWDI_IO14   | PIN_TYPE_MSQ_SWDO_IO14   )
#define PIN_TYPE_SPI0_IO0_IO14                          (PIN_TYPE_SPI0_RXD0_IO14  | PIN_TYPE_SPI0_TXD0_IO14  )
#define PIN_TYPE_SPI1_IO0_IO14                          (PIN_TYPE_SPI1_RXD0_IO14  | PIN_TYPE_SPI1_TXD0_IO14  )
#define PIN_TYPE_I2C_SDA_IO_IO15                        (PIN_TYPE_I2C_SDA_IN_IO15 | PIN_TYPE_I2C_SDA_OUT_IO15)
#define PIN_TYPE_APS_SWDIO_IO15                         (PIN_TYPE_APS_SWDI_IO15   | PIN_TYPE_APS_SWDO_IO15   )
#define PIN_TYPE_SPI0_IO1_IO15                          (PIN_TYPE_SPI0_RXD1_IO15  | PIN_TYPE_SPI0_TXD1_IO15  )
#define PIN_TYPE_SPI1_IO1_IO15                          (PIN_TYPE_SPI1_RXD1_IO15  | PIN_TYPE_SPI1_TXD1_IO15  )
#define PIN_TYPE_CAM_HSYNC_IO15                         (PIN_TYPE_CAM_HSYNC_IN_IO15 | PIN_TYPE_CAM_HSYNC_OUT_IO15)
#define PIN_TYPE_I2C_SCL_IO_IO16                        (PIN_TYPE_I2C_SCL_IN_IO16 | PIN_TYPE_I2C_SCL_OUT_IO16)
#define PIN_TYPE_APS_SWDIO_IO16                         (PIN_TYPE_APS_SWDI_IO16   | PIN_TYPE_APS_SWDO_IO16   )
#define PIN_TYPE_SPI3_IO1_IO16                          (PIN_TYPE_SPI3_RXD1_IO16  | PIN_TYPE_SPI3_TXD1_IO16  )
#define PIN_TYPE_I2C_SDA_IO_IO17                        (PIN_TYPE_I2C_SDA_IN_IO17 | PIN_TYPE_I2C_SDA_OUT_IO17)
#define PIN_TYPE_MSQ_SWDIO_IO17                         (PIN_TYPE_MSQ_SWDI_IO17   | PIN_TYPE_MSQ_SWDO_IO17   )
#define PIN_TYPE_SPI3_IO0_IO17                          (PIN_TYPE_SPI3_RXD0_IO17  | PIN_TYPE_SPI3_TXD0_IO17  )
#define PIN_TYPE_I2C_SCL_IO_IO18                        (PIN_TYPE_I2C_SCL_IN_IO18 | PIN_TYPE_I2C_SCL_OUT_IO18)
#define PIN_TYPE_APS_SWDIO_IO18                         (PIN_TYPE_APS_SWDI_IO18   | PIN_TYPE_APS_SWDO_IO18   )
#define PIN_TYPE_I2C_SDA_IO_IO19                        (PIN_TYPE_I2C_SDA_IN_IO19 | PIN_TYPE_I2C_SDA_OUT_IO19)
#define PIN_TYPE_MSQ_SWDIO_IO19                         (PIN_TYPE_MSQ_SWDI_IO19   | PIN_TYPE_MSQ_SWDO_IO19   )
#define PIN_TYPE_I2C_SCL_IO_IO20                        (PIN_TYPE_I2C_SCL_IN_IO20 | PIN_TYPE_I2C_SCL_OUT_IO20)
#define PIN_TYPE_MSQ_SWDIO_IO20                         (PIN_TYPE_MSQ_SWDI_IO20   | PIN_TYPE_MSQ_SWDO_IO20   )
#define PIN_TYPE_I2C_SDA_IO_IO21                        (PIN_TYPE_I2C_SDA_IN_IO21 | PIN_TYPE_I2C_SDA_OUT_IO21)
#define PIN_TYPE_MSQ_SWDIO_IO22                         (PIN_TYPE_MSQ_SWDI_IO22   | PIN_TYPE_MSQ_SWDO_IO22   )
#define PIN_TYPE_I2C_SCL_IO_IO23                        (PIN_TYPE_I2C_SCL_IN_IO23 | PIN_TYPE_I2C_SCL_OUT_IO23)
#define PIN_TYPE_SPI3_IO2_IO23                          (PIN_TYPE_SPI3_RXD2_IO23  | PIN_TYPE_SPI3_TXD2_IO23  )
#define PIN_TYPE_I2C_SDA_IO_IO24                        (PIN_TYPE_I2C_SDA_IN_IO24 | PIN_TYPE_I2C_SDA_OUT_IO24)
#define PIN_TYPE_MSQ_SWDIO_IO24                         (PIN_TYPE_MSQ_SWDI_IO24   | PIN_TYPE_MSQ_SWDO_IO24   )
#define PIN_TYPE_CAM_HSYNC_IO24                         (PIN_TYPE_CAM_HSYNC_IN_IO24 | PIN_TYPE_CAM_HSYNC_OUT_IO24)
#define PIN_TYPE_I2C_SCL_IO_IO25                        (PIN_TYPE_I2C_SCL_IN_IO25 | PIN_TYPE_I2C_SCL_OUT_IO25)
#define PIN_TYPE_APS_SWDIO_IO25                         (PIN_TYPE_APS_SWDI_IO25   | PIN_TYPE_APS_SWDO_IO25   )
#define PIN_TYPE_I2C_SDA_IO_IO26                        (PIN_TYPE_I2C_SDA_IN_IO26 | PIN_TYPE_I2C_SDA_OUT_IO26)
#define PIN_TYPE_MSQ_SWDIO_IO26                         (PIN_TYPE_MSQ_SWDI_IO26   | PIN_TYPE_MSQ_SWDO_IO26   )
#define PIN_TYPE_SPI2_IO_IO26                           (PIN_TYPE_SPI2_RXD_IO26   | PIN_TYPE_SPI2_TXD_IO26   )
#define PIN_TYPE_CAM_VSYNC_IO26                         (PIN_TYPE_CAM_VSYNC_IN_IO26 | PIN_TYPE_CAM_VSYNC_OUT_IO26)
#define PIN_TYPE_I2C_SCL_IO_IO27                        (PIN_TYPE_I2C_SCL_IN_IO27 | PIN_TYPE_I2C_SCL_OUT_IO27)
#define PIN_TYPE_APS_SWDIO_IO27                         (PIN_TYPE_APS_SWDI_IO27   | PIN_TYPE_APS_SWDO_IO27   )
#define PIN_TYPE_SPI3_IO0_IO27                          (PIN_TYPE_SPI3_RXD0_IO27  | PIN_TYPE_SPI3_TXD0_IO27  )
#define PIN_TYPE_MSQ_SWDIO_IO28                         (PIN_TYPE_MSQ_SWDI_IO28   | PIN_TYPE_MSQ_SWDO_IO28   )
#define PIN_TYPE_SPI2_IO_IO28                           (PIN_TYPE_SPI2_RXD_IO28   | PIN_TYPE_SPI2_TXD_IO28   )
#define PIN_TYPE_SPI3_IO2_IO28                          (PIN_TYPE_SPI3_RXD2_IO28  | PIN_TYPE_SPI3_TXD2_IO28  )
#define PIN_TYPE_I2C_SDA_IO_IO29                        (PIN_TYPE_I2C_SDA_IN_IO29 | PIN_TYPE_I2C_SDA_OUT_IO29)
#define PIN_TYPE_APS_SWDIO_IO29                         (PIN_TYPE_APS_SWDI_IO29   | PIN_TYPE_APS_SWDO_IO29   )
#define PIN_TYPE_SPI2_IO_IO29                           (PIN_TYPE_SPI2_RXD_IO29   | PIN_TYPE_SPI2_TXD_IO29   )
#define PIN_TYPE_SPI3_IO0_IO29                          (PIN_TYPE_SPI3_RXD0_IO29  | PIN_TYPE_SPI3_TXD0_IO29  )
#define PIN_TYPE_SPI3_IO1_IO29                          (PIN_TYPE_SPI3_RXD1_IO29  | PIN_TYPE_SPI3_TXD1_IO29  )
#define PIN_TYPE_I2C_SDA_IO_IO30                        (PIN_TYPE_I2C_SDA_IN_IO30 | PIN_TYPE_I2C_SDA_OUT_IO30)
#define PIN_TYPE_MSQ_SWDIO_IO30                         (PIN_TYPE_MSQ_SWDI_IO30   | PIN_TYPE_MSQ_SWDO_IO30   )
#define PIN_TYPE_I2C_SCL_IO_IO31                        (PIN_TYPE_I2C_SCL_IN_IO31 | PIN_TYPE_I2C_SCL_OUT_IO31)
#define PIN_TYPE_APS_SWDIO_IO31                         (PIN_TYPE_APS_SWDI_IO31   | PIN_TYPE_APS_SWDO_IO31   )
#define PIN_TYPE_SPI3_IO3_IO31                          (PIN_TYPE_SPI3_RXD3_IO31  | PIN_TYPE_SPI3_TXD3_IO31  )
#define PIN_TYPE_I2C_SCL_IO_IO32                        (PIN_TYPE_I2C_SCL_IN_IO32 | PIN_TYPE_I2C_SCL_OUT_IO32)
#define PIN_TYPE_MSQ_SWDIO_IO32                         (PIN_TYPE_MSQ_SWDI_IO32   | PIN_TYPE_MSQ_SWDO_IO32   )
#define PIN_TYPE_SPI2_IO_IO32                           (PIN_TYPE_SPI2_RXD_IO32   | PIN_TYPE_SPI2_TXD_IO32   )
#define PIN_TYPE_SPI3_IO2_IO32                          (PIN_TYPE_SPI3_RXD2_IO32  | PIN_TYPE_SPI3_TXD2_IO32  )
#define PIN_TYPE_I2C_SDA_IO_IO33                        (PIN_TYPE_I2C_SDA_IN_IO33 | PIN_TYPE_I2C_SDA_OUT_IO33)
#define PIN_TYPE_APS_SWDIO_IO33                         (PIN_TYPE_APS_SWDI_IO33   | PIN_TYPE_APS_SWDO_IO33   )
#define PIN_TYPE_SPI2_IO_IO33                           (PIN_TYPE_SPI2_RXD_IO33   | PIN_TYPE_SPI2_TXD_IO33   )
#define PIN_TYPE_SPI3_IO1_IO33                          (PIN_TYPE_SPI3_RXD1_IO33  | PIN_TYPE_SPI3_TXD1_IO33  )
#define PIN_TYPE_SPI3_IO1_IO34                          (PIN_TYPE_SPI3_RXD1_IO34  | PIN_TYPE_SPI3_TXD1_IO34  )
#define PIN_TYPE_APS_SWDIO_IO35                         (PIN_TYPE_APS_SWDI_IO35   | PIN_TYPE_APS_SWDO_IO35   )
#define PIN_TYPE_SPI3_IO0_IO35                          (PIN_TYPE_SPI3_RXD0_IO35  | PIN_TYPE_SPI3_TXD0_IO35  )
#define PIN_TYPE_SPI2_IO_IO36                           (PIN_TYPE_SPI2_RXD_IO36   | PIN_TYPE_SPI2_TXD_IO36   )
#define PIN_TYPE_SPI3_IO0_IO36                          (PIN_TYPE_SPI3_RXD0_IO36  | PIN_TYPE_SPI3_TXD0_IO36  )
#define PIN_TYPE_SPI3_IO1_IO36                          (PIN_TYPE_SPI3_RXD1_IO36  | PIN_TYPE_SPI3_TXD1_IO36  )
#define PIN_TYPE_PSRAM_IO0_IO39                         (PIN_TYPE_PSRAM_RXD0_IO39 | PIN_TYPE_SPI0_TXD0_IO39  )
#define PIN_TYPE_PSRAM_IO1_IO40                         (PIN_TYPE_PSRAM_RXD1_IO40 | PIN_TYPE_SPI0_TXD1_IO40  )
#define PIN_TYPE_PSRAM_IO2_IO41                         (PIN_TYPE_PSRAM_RXD2_IO41 | PIN_TYPE_SPI0_TXD2_IO41  )
#define PIN_TYPE_PSRAM_IO3_IO42                         (PIN_TYPE_PSRAM_RXD3_IO42 | PIN_TYPE_SPI0_TXD3_IO42  )
#define PIN_TYPE_PSRAM_TXD0_IO39                        PIN_TYPE_SPI0_TXD0_IO39
#define PIN_TYPE_PSRAM_TXD1_IO40                        PIN_TYPE_SPI0_TXD1_IO40
#define PIN_TYPE_PSRAM_TXD2_IO41                        PIN_TYPE_SPI0_TXD2_IO41
#define PIN_TYPE_PSRAM_TXD3_IO42                        PIN_TYPE_SPI0_TXD3_IO42






/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...



/* Uart mode for switching UART */
typedef enum {
    PIN_MAIN_UART_MODE_AT=0,
    PIN_MAIN_UART_MODE_DBG,
}E_PIN_MAIN_UART_MODE;




#pragma push
#pragma anon_unions
typedef struct
{
    union {
        struct {
            uint64_t IoType:2;           /* [1:0]    Peripheral, GpioInput, GpioOutput, Disable             */
            uint64_t IsSipIo:1;          /* [2]      Indicate this IO is SiP IO                             */
            uint64_t IoNum:6;            /* [8:3]    GPIO Pin 0~40                                          */
            uint64_t InEnable:1;         /* [9]      Enable(1), Disable(0)                                  */
            uint64_t InGrpOfst:11;       /* [20:10]  Register and bit offset.Base=0x68                      */
            uint64_t InGrpValue:4;       /* [24:21]  Input group value                                      */
            uint64_t InInverter:1;       /* [25]     Enable(1), Disable(0)                                  */
            uint64_t IoPE:1;             /* [26]     GPIO input floating, pull up, or pull down.            */
            uint64_t OutEnable:1;        /* [27]     Enable(1), Disable(0)                                  */
            uint64_t OutGrpOfst:11;      /* [38:28]  Register and bit offset.Base=0x68                      */
            uint64_t OutGrpValue:4;      /* [42:39]  Output group value                                     */
            uint64_t OutInverter:1;      /* [43]     Enable(1), Disable(0)                                  */
            uint64_t OutEnInverter:1;    /* [44]     Enable(1), Disable(0)                                  */
            uint64_t OutLevel:1;         /* [45]     Low(Down, 0), High(Up, 1)                              */
            uint64_t OutIoDrv:1;         /* [46]     Out IO driving strength                                */
            uint64_t AutoCtrlEn:1;       /* [47]     Auto control enable                                    */
            uint64_t AutoCtrlLvl:1;      /* [48]     Auto control level                                     */
        };
        uint64_t u64Value;
    };
}S_GPIO_PIN_CFG;
#pragma pop




/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable

// Sec 5: declaration of global function prototype

/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype

#ifdef __cplusplus
}
#endif
#include "hal_pin_def_patch.h"
#endif  // end of __HAL_PIN_DEF_H__
