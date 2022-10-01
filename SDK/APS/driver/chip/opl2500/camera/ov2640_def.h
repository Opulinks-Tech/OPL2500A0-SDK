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
 *  @file ov2640_def.h 
 * 
 *  @brief OV2640 registers and specification definition
 *
 *  
 ******************************************************************************/

#ifndef _OV2640_DEF_H_
#define _OV2640_DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define OV2640_SCCB_ADDRESS                     0x30
#define OV2640_PRODUCT_ID_H                     0x26
#define OV2640_PRODUCT_ID_L                     0x42
#define OV2640_MANUFACTURER_ID_H                0x7F
#define OV2640_MANUFACTURER_ID_L                0xA2



 
 
/* Register definition */
#define OV2640_DSP_REG_BYPASS                   0x05    /* Bypass DSP */
#define OV2640_DSP_REG_QS                       0x44    /* Quantization Scale Factor */
#define OV2640_DSP_REG_CTRLL                    0x50
#define OV2640_DSP_REG_HSIZE                    0x51
#define OV2640_DSP_REG_VSIZE                    0x52
#define OV2640_DSP_REG_XOFFL                    0x53
#define OV2640_DSP_REG_YOFFL                    0x54
#define OV2640_DSP_REG_VHYX                     0x55
#define OV2640_DSP_REG_DPRP                     0x56
#define OV2640_DSP_REG_TEST                     0x57
#define OV2640_DSP_REG_ZMOW                     0x5A
#define OV2640_DSP_REG_ZMOH                     0x5B
#define OV2640_DSP_REG_ZMHH                     0x5C
#define OV2640_DSP_REG_BPADDR                   0x7C    /* SDE Indirect Register Access: Address */
#define OV2640_DSP_REG_BPDATA                   0x7D    /* SDE Indirect Register Access: Data */
#define OV2640_DSP_REG_CTRL2                    0x86    /* Module Enable */
#define OV2640_DSP_REG_CTRL3                    0x87    /* Module Enable */
#define OV2640_DSP_REG_SIZEL                    0x8C
#define OV2640_DSP_REG_HSIZE8                   0xC0    /* Image Horizontal Size HSIZE[10:3] */
#define OV2640_DSP_REG_VSIZE8                   0xC1    /* Image Vertical Size VSIZE[10:3] */
#define OV2640_DSP_REG_CTRL0                    0xC2    /* Module Enable */
#define OV2640_DSP_REG_CTRL1                    0xC3    /* Module Enable */
#define OV2640_DSP_REG_DVP_SP                   0xD3    /* DVP output speed control */
#define OV2640_DSP_REG_IMAGE_MODE               0xDA    /* Image Output Format Select */
#define OV2640_DSP_REG_RESET                    0xE0    /* Reset */
#define OV2640_DSP_REG_REGED                    0xED
#define OV2640_DSP_REG_MS_SP                    0xF0    /* SCCB Master Speed */
#define OV2640_DSP_REG_SS_ID                    0xF7    /* SCCB Slave ID */
#define OV2640_DSP_REG_SS_CTRL                  0xF8    /* SCCB Slave Control */
#define OV2640_DSP_REG_MC_BIST                  0xF9
#define OV2640_DSP_REG_MC_AL                    0xFA    /* Program Memory Pointer Address Low Byte */
#define OV2640_DSP_REG_MC_AH                    0xFB    /* Program Memory Pointer Address High Byte */
#define OV2640_DSP_REG_MC_D                     0xFC    /* Program Memory Pointer Access Address */
#define OV2640_DSP_REG_P_CMD                    0xFD    /* SCCB Protocol Command Register */
#define OV2640_DSP_REG_P_STATUS                 0xFE    /* SCCB Protocol Status Register */
#define OV2640_REG_RA_DLMT                      0xFF    /* Register Bank Select */

#define OV2640_SENSOR_REG_GAIN                  0x00    /* AGC Gain Control LSBs */
#define OV2640_SENSOR_REG_COM1                  0x03    /* Common Control 1 */
#define OV2640_SENSOR_REG_REG04                 0x04
#define OV2640_SENSOR_REG_REG08                 0x08    /* Frame Exposure One-pin Control Pre-charge Row Number */
#define OV2640_SENSOR_REG_COM2                  0x09    /* Common Control 2 */
#define OV2640_SENSOR_REG_PIDH                  0x0A    /* Product ID Number MSB (Read only) */
#define OV2640_SENSOR_REG_PIDL                  0x0B    /* Product ID Number LSB (Read only) */
#define OV2640_SENSOR_REG_COM3                  0x0C
#define OV2640_SENSOR_REG_AEC                   0x10    /* Automatic Exposure Control 8 bits for AEC[9:2] */
#define OV2640_SENSOR_REG_CLKRC                 0x11    /* Clock Rate Control */
#define OV2640_SENSOR_REG_COM7                  0x12    /* Common Control 7 */
#define OV2640_SENSOR_REG_COM8                  0x13    /* Common Control 8 */
#define OV2640_SENSOR_REG_COM9                  0x14    /* Common Control 9 */
#define OV2640_SENSOR_REG_COM10                 0x15
#define OV2640_SENSOR_REG_HREFST                0x17    /* Horizontal Window Start MSB 8 bits, each LSB represents two pixels */
#define OV2640_SENSOR_REG_HREFEND               0x18    /* Horizontal Window End MSB 8 bits, each LSB represents two pixels */
#define OV2640_SENSOR_REG_VSTRT                 0x19    /* Vertical Window Line Start MSB 8 bits */
#define OV2640_SENSOR_REG_VEND                  0x1A    /* Vertical Window Line End MSB 8 bits */
#define OV2640_SENSOR_REG_MIDH                  0x1C    /* Manufacturer ID Byte - High (Read only = 0x7F) */
#define OV2640_SENSOR_REG_MIDL                  0x1D    /* Manufacturer ID Byte - Low (Read only = 0xA2) */
#define OV2640_SENSOR_REG_AEW                   0x24    /* Luminance Signal High Range for AEC/AGC Operation */
#define OV2640_SENSOR_REG_AEB                   0x25    /* Luminance Signal Low Range for AEC/AGC Operation */
#define OV2640_SENSOR_REG_VV                    0x26    /* Fast Mode Large Step Range Threshold */
#define OV2640_SENSOR_REG_REG2A                 0x2A
#define OV2640_SENSOR_REG_FRARL                 0x2B    /* Line Interval Adjustment Value LSB 8 bits */
#define OV2640_SENSOR_REG_ADDVSL                0x2D    /* VSYNC Pulse Width LSB 8 bits */
#define OV2640_SENSOR_REG_ADDVSH                0x2E    /* VSYNC Pulse Width MSB 8 bits */
#define OV2640_SENSOR_REG_YAVG                  0x2F    /* Luminance Average = (BAVG[7:0] + (2 x GbAVG[7:0]) + RAVG[7:0]) x 0.25 */
#define OV2640_SENSOR_REG_REG32                 0x32
#define OV2640_SENSOR_REG_ARCOM2                0x34
#define OV2640_SENSOR_REG_REG45                 0x45
#define OV2640_SENSOR_REG_FLL                   0x46    /* Frame Length Adjustment LSBs */
#define OV2640_SENSOR_REG_FLH                   0x47    /* Frame Length Adjustment MSBs */
#define OV2640_SENSOR_REG_COM19                 0x48
#define OV2640_SENSOR_REG_ZOOMS                 0x49    /* Zoom Mode Vertical Window Start Point 8 MSBs */
#define OV2640_SENSOR_REG_COM22                 0x4B    /* Common Control 22 */
#define OV2640_SENSOR_REG_COM25                 0x4E    /* Common Control 25 */
#define OV2640_SENSOR_REG_BD50                  0x4F    /* 50Hz Banding AEC 8 LSBs */
#define OV2640_SENSOR_REG_BD60                  0x50    /* 60Hz Banding AEC 8 LSBs */
#define OV2640_SENSOR_REG_REG5D                 0x5D
#define OV2640_SENSOR_REG_REG5E                 0x5E
#define OV2640_SENSOR_REG_REG5F                 0x5F
#define OV2640_SENSOR_REG_REG60                 0x60
#define OV2640_SENSOR_REG_HISTO_LOW             0x61    /* Histogram Algorithm Low Level */
#define OV2640_SENSOR_REG_HISTO_HIGH            0x62    /* Histogram Algorithm High Level */






/******************************************************************************/
/*                                                                            */
/*                   OV2640 Bank0 DSP Definition. 0xFF = 0                    */
/*                                                                            */
/******************************************************************************/

/*********  Bit definition for OV2640_DSP_REG_BYPASS (0x05) register  *********/
#define OV2640_DSP_BYPASS_SEL_Pos                           (0U)                                                                /*!< Bypass DSP select */
#define OV2640_DSP_BYPASS_SEL_Msk                           (0x01U << OV2640_DSP_BYPASS_SEL_Pos)                                /*!< 0x01 */

/***********  Bit definition for OV2640_DSP_REG_QS (0x44) register  ***********/
#define OV2640_DSP_QS_FACTOR_Pos                            (0U)                                                                /*!< Quantization Scale Factor */
#define OV2640_DSP_QS_FACTOR_Msk                            (0xFFU << OV2640_DSP_QS_FACTOR_Pos)                                 /*!< 0xFF */

/*********  Bit definition for OV2640_DSP_REG_CTRLL (0x50) register  **********/
#define OV2640_DSP_CTRLL_H_DIV_Pos                          (0U)                                                                /*!< H_DIVIDER */
#define OV2640_DSP_CTRLL_H_DIV_Msk                          (0x07U << OV2640_DSP_CTRLL_H_DIV_Pos)                               /*!< 0x07 */
#define OV2640_DSP_CTRLL_V_DIV_Pos                          (3U)                                                                /*!< V_DIVIDER */
#define OV2640_DSP_CTRLL_V_DIV_Msk                          (0x07U << OV2640_DSP_CTRLL_V_DIV_Pos)                               /*!< 0x38 */
#define OV2640_DSP_CTRLL_ROUND_Pos                          (6U)                                                                /*!< Round */
#define OV2640_DSP_CTRLL_ROUND_Msk                          (0x01U << OV2640_DSP_CTRLL_ROUND_Pos)                               /*!< 0x40 */
#define OV2640_DSP_CTRLL_LP_DP_Pos                          (7U)                                                                /*!< LP_DP */
#define OV2640_DSP_CTRLL_LP_DP_Msk                          (0x01U << OV2640_DSP_CTRLL_LP_DP_Pos)                               /*!< 0x80 */

/*********  Bit definition for OV2640_DSP_REG_HSIZE (0x51) register  **********/
#define OV2640_DSP_HSIZE_H_SIZE_0_Pos                       (0U)                                                                /*!< H_SIZE[7:0] (real/4) */
#define OV2640_DSP_HSIZE_H_SIZE_0_Msk                       (0xFFU << OV2640_DSP_HSIZE_H_SIZE_0_Pos)                            /*!< 0xFF */

/*********  Bit definition for OV2640_DSP_REG_VSIZE (0x52) register  **********/
#define OV2640_DSP_VSIZE_V_SIZE_0_Pos                       (0U)                                                                /*!< V_SIZE[7:0] (real/4) */
#define OV2640_DSP_VSIZE_V_SIZE_0_Msk                       (0xFFU << OV2640_DSP_VSIZE_V_SIZE_0_Pos)                            /*!< 0xFF */

/*********  Bit definition for OV2640_DSP_REG_XOFFL (0x53) register  **********/
#define OV2640_DSP_XOFFL_OFST_X_L_Pos                       (0U)                                                                /*!< OFFSET_X[7:0] */
#define OV2640_DSP_XOFFL_OFST_X_L_Msk                       (0xFFU << OV2640_DSP_XOFFL_OFST_X_L_Pos)                            /*!< 0xFF */

/*********  Bit definition for OV2640_DSP_REG_YOFFL (0x54) register  **********/
#define OV2640_DSP_YOFFL_OFST_Y_L_Pos                       (0U)                                                                /*!< OFFSET_Y[7:0] */
#define OV2640_DSP_YOFFL_OFST_Y_L_Msk                       (0xFFU << OV2640_DSP_YOFFL_OFST_Y_L_Pos)                            /*!< 0xFF */

/**********  Bit definition for OV2640_DSP_REG_VHYX (0x55) register  **********/
#define OV2640_DSP_VHYX_OFST_X_H_Pos                        (0U)                                                                /*!< OFFSET_X[10:8] */
#define OV2640_DSP_VHYX_OFST_X_H_Msk                        (0x07U << OV2640_DSP_VHYX_OFST_X_H_Pos)                             /*!< 0x07 */
#define OV2640_DSP_VHYX_H_SIZE_1_Pos                        (3U)                                                                /*!< H_SIZE[8] */
#define OV2640_DSP_VHYX_H_SIZE_1_Msk                        (0x01U << OV2640_DSP_VHYX_H_SIZE_1_Pos)                             /*!< 0x08 */
#define OV2640_DSP_VHYX_OFST_Y_H_Pos                        (4U)                                                                /*!< OFFSET_Y[10:8] */
#define OV2640_DSP_VHYX_OFST_Y_H_Msk                        (0x07U << OV2640_DSP_VHYX_OFST_Y_H_Pos)                             /*!< 0x70 */
#define OV2640_DSP_VHYX_V_SIZE_1_Pos                        (7U)                                                                /*!< V_SIZE[8] */
#define OV2640_DSP_VHYX_V_SIZE_1_Msk                        (0x01U << OV2640_DSP_VHYX_V_SIZE_1_Pos)                             /*!< 0x80 */

/**********  Bit definition for OV2640_DSP_REG_DPRP (0x56) register  **********/
#define OV2640_DSP_DPRP_DP_SELX_Pos                         (0U)                                                                /*!< DP_SELX */
#define OV2640_DSP_DPRP_DP_SELX_Msk                         (0x0FU << OV2640_DSP_DPRP_DP_SELX_Pos)                              /*!< 0x0F */
#define OV2640_DSP_DPRP_DP_SELY_Pos                         (4U)                                                                /*!< DP_SELY */
#define OV2640_DSP_DPRP_DP_SELY_Msk                         (0x0FU << OV2640_DSP_DPRP_DP_SELY_Pos)                              /*!< 0xF0 */

/**********  Bit definition for OV2640_DSP_REG_TEST (0x57) register  **********/
#define OV2640_DSP_TEST_H_SIZE_2_Pos                        (7U)                                                                /*!< H_SIZE[9] */
#define OV2640_DSP_TEST_H_SIZE_2_Msk                        (0x01U << OV2640_DSP_TEST_H_SIZE_2_Pos)                             /*!< 0x80 */

/**********  Bit definition for OV2640_DSP_REG_ZMOW (0x5A) register  **********/
#define OV2640_DSP_ZMOW_OUTW_L_Pos                          (0U)                                                                /*!< OUTW[7:0] (real/4) */
#define OV2640_DSP_ZMOW_OUTW_L_Msk                          (0xFFU << OV2640_DSP_ZMOW_OUTW_L_Pos)                               /*!< 0xFF */

/**********  Bit definition for OV2640_DSP_REG_ZMOH (0x5B) register  **********/
#define OV2640_DSP_ZMOH_OUTH_L_Pos                          (0U)                                                                /*!< OUTH[7:0] (real/4) */
#define OV2640_DSP_ZMOH_OUTH_L_Msk                          (0xFFU << OV2640_DSP_ZMOH_OUTH_L_Pos)                               /*!< 0xFF */

/**********  Bit definition for OV2640_DSP_REG_ZMHH (0x5C) register  **********/
#define OV2640_DSP_ZMHH_OUTW_H_Pos                          (0U)                                                                /*!< OUTW[9:8] */
#define OV2640_DSP_ZMHH_OUTW_H_Msk                          (0x03U << OV2640_DSP_ZMHH_OUTW_H_Pos)                               /*!< 0x03 */
#define OV2640_DSP_ZMHH_OUTH_H_Pos                          (2U)                                                                /*!< OUTH[8] */
#define OV2640_DSP_ZMHH_OUTH_H_Msk                          (0x01U << OV2640_DSP_ZMHH_OUTH_H_Pos)                               /*!< 0x04 */
#define OV2640_DSP_ZMHH_ZMSPD_Pos                           (4U)                                                                /*!< zoom speed */
#define OV2640_DSP_ZMHH_ZMSPD_Msk                           (0x0FU << OV2640_DSP_ZMHH_ZMSPD_Pos)                                /*!< 0xF0 */

/*********  Bit definition for OV2640_DSP_REG_BPADDR (0x7C) register  *********/
#define OV2640_DSP_BPADDR_ADDR_Pos                          (0U)                                                                /*!< SDE Indirect Register Access: Address */
#define OV2640_DSP_BPADDR_ADDR_Msk                          (0x0FU << OV2640_DSP_BPADDR_ADDR_Pos)                               /*!< 0x0F */

/*********  Bit definition for OV2640_DSP_REG_BPDATA (0x7D) register  *********/
#define OV2640_DSP_BPDATA_DATA_Pos                          (0U)                                                                /*!< SDE Indirect Register Access: Data */
#define OV2640_DSP_BPDATA_DATA_Msk                          (0xFFU << OV2640_DSP_BPDATA_DATA_Pos)                               /*!< 0xFF */

/*********  Bit definition for OV2640_DSP_REG_CTRL2 (0x86) register  **********/
#define OV2640_DSP_CTRL2_CMX_Pos                            (0U)                                                                /*!< Enable CMX */
#define OV2640_DSP_CTRL2_CMX_Msk                            (0x01U << OV2640_DSP_CTRL2_CMX_Pos)                                 /*!< 0x01 */
#define OV2640_DSP_CTRL2_UV_AVG_Pos                         (2U)                                                                /*!< Enable UV_AVG */
#define OV2640_DSP_CTRL2_UV_AVG_Msk                         (0x01U << OV2640_DSP_CTRL2_UV_AVG_Pos)                              /*!< 0x04 */
#define OV2640_DSP_CTRL2_UV_ADJ_Pos                         (3U)                                                                /*!< Enable UV_ADJ */
#define OV2640_DSP_CTRL2_UV_ADJ_Msk                         (0x01U << OV2640_DSP_CTRL2_UV_ADJ_Pos)                              /*!< 0x08 */
#define OV2640_DSP_CTRL2_SDE_Pos                            (4U)                                                                /*!< Enable SDE */
#define OV2640_DSP_CTRL2_SDE_Msk                            (0x01U << OV2640_DSP_CTRL2_SDE_Pos)                                 /*!< 0x10 */
#define OV2640_DSP_CTRL2_DCW_Pos                            (5U)                                                                /*!< Enable DCW */
#define OV2640_DSP_CTRL2_DCW_Msk                            (0x01U << OV2640_DSP_CTRL2_DCW_Pos)                                 /*!< 0x20 */

/*********  Bit definition for OV2640_DSP_REG_CTRL3 (0x87) register  **********/
#define OV2640_DSP_CTRL3_WPC_Pos                            (6U)                                                                /*!< Enable WPC */
#define OV2640_DSP_CTRL3_WPC_Msk                            (0x01U << OV2640_DSP_CTRL3_WPC_Pos)                                 /*!< 0x40 */
#define OV2640_DSP_CTRL3_BPC_Pos                            (7U)                                                                /*!< Enable BPC */
#define OV2640_DSP_CTRL3_BPC_Msk                            (0x01U << OV2640_DSP_CTRL3_BPC_Pos)                                 /*!< 0x80 */

/*********  Bit definition for OV2640_DSP_REG_SIZEL (0x8C) register  **********/
#define OV2640_DSP_SIZEL_VSIZE_0_Pos                        (0U)                                                                /*!< VSIZE[2:0] */
#define OV2640_DSP_SIZEL_VSIZE_0_Msk                        (0x07U << OV2640_DSP_SIZEL_VSIZE_0_Pos)                             /*!< 0x07 */
#define OV2640_DSP_SIZEL_HSIZE_0_Pos                        (3U)                                                                /*!< HSIZE[2:0] */
#define OV2640_DSP_SIZEL_HSIZE_0_Msk                        (0x03U << OV2640_DSP_SIZEL_HSIZE_0_Pos)                             /*!< 0x18 */
#define OV2640_DSP_SIZEL_HSIZE_2_Pos                        (5U)                                                                /*!< HSIZE[11] */
#define OV2640_DSP_SIZEL_HSIZE_2_Msk                        (0x01U << OV2640_DSP_SIZEL_HSIZE_2_Pos)                             /*!< 0x20 */

/*********  Bit definition for OV2640_DSP_REG_HSIZE8 (0xC0) register  *********/
#define OV2640_DSP_HSIZE8_HSIZE_1_Pos                       (0U)                                                                /*!< Image Horizontal Size HSIZE[10:3] */
#define OV2640_DSP_HSIZE8_HSIZE_1_Msk                       (0xFFU << OV2640_DSP_HSIZE8_HSIZE_1_Pos)                            /*!< 0xFF */

/*********  Bit definition for OV2640_DSP_REG_VSIZE8 (0xC1) register  *********/
#define OV2640_DSP_VSIZE8_VSIZE_1_Pos                       (0U)                                                                /*!< Image Vertical Size VSIZE[10:3] */
#define OV2640_DSP_VSIZE8_VSIZE_1_Msk                       (0xFFU << OV2640_DSP_VSIZE8_VSIZE_1_Pos)                            /*!< 0xFF */

/*********  Bit definition for OV2640_DSP_REG_CTRL0 (0xC2) register  **********/
#define OV2640_DSP_CTRL0_RAW_EN_Pos                         (0U)                                                                /*!< Enable RAW */
#define OV2640_DSP_CTRL0_RAW_EN_Msk                         (0x01U << OV2640_DSP_CTRL0_RAW_EN_Pos)                              /*!< 0x01 */
#define OV2640_DSP_CTRL0_RGB_EN_Pos                         (1U)                                                                /*!< Enable RGB */
#define OV2640_DSP_CTRL0_RGB_EN_Msk                         (0x01U << OV2640_DSP_CTRL0_RGB_EN_Pos)                              /*!< 0x02 */
#define OV2640_DSP_CTRL0_YUV_EN_Pos                         (2U)                                                                /*!< Enable YUV */
#define OV2640_DSP_CTRL0_YUV_EN_Msk                         (0x01U << OV2640_DSP_CTRL0_YUV_EN_Pos)                              /*!< 0x04 */
#define OV2640_DSP_CTRL0_YUV422_Pos                         (3U)                                                                /*!< Enable YUV422 */
#define OV2640_DSP_CTRL0_YUV422_Msk                         (0x01U << OV2640_DSP_CTRL0_YUV422_Pos)                              /*!< 0x08 */
#define OV2640_DSP_CTRL0_VFIRST_Pos                         (4U)                                                                /*!< Enable VFIRST */
#define OV2640_DSP_CTRL0_VFIRST_Msk                         (0x01U << OV2640_DSP_CTRL0_VFIRST_Pos)                              /*!< 0x10 */
#define OV2640_DSP_CTRL0_STAT_SEL_Pos                       (5U)                                                                /*!< Enable STAT_SEL */
#define OV2640_DSP_CTRL0_STAT_SEL_Msk                       (0x01U << OV2640_DSP_CTRL0_STAT_SEL_Pos)                            /*!< 0x20 */
#define OV2640_DSP_CTRL0_AEC_SEL_Pos                        (6U)                                                                /*!< Enable AEC_SEL */
#define OV2640_DSP_CTRL0_AEC_SEL_Msk                        (0x01U << OV2640_DSP_CTRL0_AEC_SEL_Pos)                             /*!< 0x40 */
#define OV2640_DSP_CTRL0_AEC_EN_Pos                         (7U)                                                                /*!< Enable AEC_EN */
#define OV2640_DSP_CTRL0_AEC_EN_Msk                         (0x01U << OV2640_DSP_CTRL0_AEC_EN_Pos)                              /*!< 0x80 */

/*********  Bit definition for OV2640_DSP_REG_CTRL1 (0xC3) register  **********/
#define OV2640_DSP_CTRL1_PRE_Pos                            (0U)                                                                /*!< Enable PRE */
#define OV2640_DSP_CTRL1_PRE_Msk                            (0x01U << OV2640_DSP_CTRL1_PRE_Pos)                                 /*!< 0x01 */
#define OV2640_DSP_CTRL1_LENC_Pos                           (1U)                                                                /*!< Enable LENC */
#define OV2640_DSP_CTRL1_LENC_Msk                           (0x01U << OV2640_DSP_CTRL1_LENC_Pos)                                /*!< 0x02 */
#define OV2640_DSP_CTRL1_AWB_GAIN_Pos                       (2U)                                                                /*!< Enable AWB Gain */
#define OV2640_DSP_CTRL1_AWB_GAIN_Msk                       (0x01U << OV2640_DSP_CTRL1_AWB_GAIN_Pos)                            /*!< 0x04 */
#define OV2640_DSP_CTRL1_AWB_Pos                            (3U)                                                                /*!< Enable AWB */
#define OV2640_DSP_CTRL1_AWB_Msk                            (0x01U << OV2640_DSP_CTRL1_AWB_Pos)                                 /*!< 0x08 */
#define OV2640_DSP_CTRL1_DG_Pos                             (4U)                                                                /*!< Enable DG */
#define OV2640_DSP_CTRL1_DG_Msk                             (0x01U << OV2640_DSP_CTRL1_DG_Pos)                                  /*!< 0x10 */
#define OV2640_DSP_CTRL1_RAW_GMA_Pos                        (5U)                                                                /*!< Enable RAW GMA */
#define OV2640_DSP_CTRL1_RAW_GMA_Msk                        (0x01U << OV2640_DSP_CTRL1_RAW_GMA_Pos)                             /*!< 0x20 */
#define OV2640_DSP_CTRL1_DMY_Pos                            (6U)                                                                /*!< Enable DMY */
#define OV2640_DSP_CTRL1_DMY_Msk                            (0x01U << OV2640_DSP_CTRL1_DMY_Pos)                                 /*!< 0x40 */
#define OV2640_DSP_CTRL1_CIP_Pos                            (7U)                                                                /*!< Enable CIP */
#define OV2640_DSP_CTRL1_CIP_Msk                            (0x01U << OV2640_DSP_CTRL1_CIP_Pos)                                 /*!< 0x80 */

/*********  Bit definition for OV2640_DSP_REG_DVP_SP (0xD3) register  *********/
#define OV2640_DSP_DVP_SP_DVP_SPD_Pos                       (0U)                                                                /*!< DVP output speed control */
#define OV2640_DSP_DVP_SP_DVP_SPD_Msk                       (0x7FU << OV2640_DSP_DVP_SP_DVP_SPD_Pos)                            /*!< 0x7F */
#define OV2640_DSP_DVP_SP_AUTO_Pos                          (7U)                                                                /*!< Auto mode */
#define OV2640_DSP_DVP_SP_AUTO_Msk                          (0x01U << OV2640_DSP_DVP_SP_AUTO_Pos)                               /*!< 0x80 */

/*******  Bit definition for OV2640_DSP_REG_IMAGE_MODE (0xDA) register  *******/
#define OV2640_DSP_IMAGE_MODE_BYTE_SWAP_Pos                 (0U)                                                                /*!< Byte swap enable for DVP */
#define OV2640_DSP_IMAGE_MODE_BYTE_SWAP_Msk                 (0x01U << OV2640_DSP_IMAGE_MODE_BYTE_SWAP_Pos)                      /*!< 0x01 */
#define OV2640_DSP_IMAGE_MODE_HREF_TIMING_Pos               (1U)                                                                /*!< HREF timing select in DVP JPEG output mode */
#define OV2640_DSP_IMAGE_MODE_HREF_TIMING_Msk               (0x01U << OV2640_DSP_IMAGE_MODE_HREF_TIMING_Pos)                    /*!< 0x02 */
#define OV2640_DSP_IMAGE_MODE_DVP_OUT_FMT_Pos               (2U)                                                                /*!< DVP output format */
#define OV2640_DSP_IMAGE_MODE_DVP_OUT_FMT_Msk               (0x03U << OV2640_DSP_IMAGE_MODE_DVP_OUT_FMT_Pos)                    /*!< 0x0C */
#define OV2640_DSP_IMAGE_MODE_JPEG_OUT_EN_Pos               (4U)                                                                /*!< JPEG output enable */
#define OV2640_DSP_IMAGE_MODE_JPEG_OUT_EN_Msk               (0x01U << OV2640_DSP_IMAGE_MODE_JPEG_OUT_EN_Pos)                    /*!< 0x10 */
#define OV2640_DSP_IMAGE_MODE_Y8_EN_DVP_Pos                 (6U)                                                                /*!< Y8 enable for DVP */
#define OV2640_DSP_IMAGE_MODE_Y8_EN_DVP_Msk                 (0x01U << OV2640_DSP_IMAGE_MODE_Y8_EN_DVP_Pos)                      /*!< 0x40 */

/*********  Bit definition for OV2640_DSP_REG_RESET (0xE0) register  **********/
#define OV2640_DSP_RESET_CIF_Pos                            (0U)                                                                /*!< Reset CIF */
#define OV2640_DSP_RESET_CIF_Msk                            (0x01U << OV2640_DSP_RESET_CIF_Pos)                                 /*!< 0x01 */
#define OV2640_DSP_RESET_IPU_Pos                            (1U)                                                                /*!< Reset IPU */
#define OV2640_DSP_RESET_IPU_Msk                            (0x01U << OV2640_DSP_RESET_IPU_Pos)                                 /*!< 0x02 */
#define OV2640_DSP_RESET_DVP_Pos                            (2U)                                                                /*!< Reset DVP */
#define OV2640_DSP_RESET_DVP_Msk                            (0x01U << OV2640_DSP_RESET_DVP_Pos)                                 /*!< 0x04 */
#define OV2640_DSP_RESET_JPEG_Pos                           (4U)                                                                /*!< Reset JPEG */
#define OV2640_DSP_RESET_JPEG_Msk                           (0x01U << OV2640_DSP_RESET_JPEG_Pos)                                /*!< 0x10 */
#define OV2640_DSP_RESET_SCCB_Pos                           (5U)                                                                /*!< Reset SCCB */
#define OV2640_DSP_RESET_SCCB_Msk                           (0x01U << OV2640_DSP_RESET_SCCB_Pos)                                /*!< 0x20 */
#define OV2640_DSP_RESET_MCU_Pos                            (6U)                                                                /*!< Reset MCU */
#define OV2640_DSP_RESET_MCU_Msk                            (0x01U << OV2640_DSP_RESET_MCU_Pos)                                 /*!< 0x40 */

/*********  Bit definition for OV2640_DSP_REG_REGED (0xED) register  **********/
#define OV2640_DSP_REGED_CO_PDWN_Pos                        (4U)                                                                /*!< Clock output power-down pin status */
#define OV2640_DSP_REGED_CO_PDWN_Msk                        (0x01U << OV2640_DSP_REGED_CO_PDWN_Pos)                             /*!< 0x10 */

/*********  Bit definition for OV2640_DSP_REG_MS_SP (0xF0) register  **********/
#define OV2640_DSP_MS_SP_SPEED_Pos                          (0U)                                                                /*!< SCCB Master Speed */
#define OV2640_DSP_MS_SP_SPEED_Msk                          (0xFFU << OV2640_DSP_MS_SP_SPEED_Pos)                               /*!< 0xFF */

/*********  Bit definition for OV2640_DSP_REG_SS_ID (0xF7) register  **********/
#define OV2640_DSP_SS_ID_ID_Pos                             (0U)                                                                /*!< SCCB Slave ID */
#define OV2640_DSP_SS_ID_ID_Msk                             (0xFFU << OV2640_DSP_SS_ID_ID_Pos)                                  /*!< 0xFF */

/********  Bit definition for OV2640_DSP_REG_SS_CTRL (0xF8) register  *********/
#define OV2640_DSP_SS_CTRL_PT_ACS_Pos                       (0U)                                                                /*!< Enable sensor pass through access */
#define OV2640_DSP_SS_CTRL_PT_ACS_Msk                       (0x01U << OV2640_DSP_SS_CTRL_PT_ACS_Pos)                            /*!< 0x01 */
#define OV2640_DSP_SS_CTRL_MST_ACS_Pos                      (1U)                                                                /*!< Enable SCCB master access */
#define OV2640_DSP_SS_CTRL_MST_ACS_Msk                      (0x01U << OV2640_DSP_SS_CTRL_MST_ACS_Pos)                           /*!< 0x02 */
#define OV2640_DSP_SS_CTRL_DLY_CLK_Pos                      (2U)                                                                /*!< Delay SCCB master clock */
#define OV2640_DSP_SS_CTRL_DLY_CLK_Msk                      (0x01U << OV2640_DSP_SS_CTRL_DLY_CLK_Pos)                           /*!< 0x04 */
#define OV2640_DSP_SS_CTRL_EN_Pos                           (3U)                                                                /*!< SCCB enable */
#define OV2640_DSP_SS_CTRL_EN_Msk                           (0x01U << OV2640_DSP_SS_CTRL_EN_Pos)                                /*!< 0x08 */
#define OV2640_DSP_SS_CTRL_ADDR_INC_Pos                     (5U)                                                                /*!< Address auto-increase enable */
#define OV2640_DSP_SS_CTRL_ADDR_INC_Msk                     (0x01U << OV2640_DSP_SS_CTRL_ADDR_INC_Pos)                          /*!< 0x20 */

/********  Bit definition for OV2640_DSP_REG_MC_BIST (0xF9) register  *********/
#define OV2640_DSP_MC_BIST_LAUNCH_Pos                       (0U)                                                                /*!< Launch BIST */
#define OV2640_DSP_MC_BIST_LAUNCH_Msk                       (0x01U << OV2640_DSP_MC_BIST_LAUNCH_Pos)                            /*!< 0x01 */
#define OV2640_DSP_MC_BIST_BUSY_Pos                         (1U)                                                                /*!< BIST busy bit for read; One-shot reset of microcontroller for write */
#define OV2640_DSP_MC_BIST_BUSY_Msk                         (0x01U << OV2640_DSP_MC_BIST_BUSY_Pos)                              /*!< 0x02 */
#define OV2640_DSP_MC_BIST_ERR_0_512B_Pos                   (2U)                                                                /*!< R/W 0 error for 512-byte memory */
#define OV2640_DSP_MC_BIST_ERR_0_512B_Msk                   (0x01U << OV2640_DSP_MC_BIST_ERR_0_512B_Pos)                        /*!< 0x04 */
#define OV2640_DSP_MC_BIST_ERR_1_512B_Pos                   (3U)                                                                /*!< R/W 1 error for 512-byte memory */
#define OV2640_DSP_MC_BIST_ERR_1_512B_Msk                   (0x01U << OV2640_DSP_MC_BIST_ERR_1_512B_Pos)                        /*!< 0x08 */
#define OV2640_DSP_MC_BIST_ERR_0_12KB_Pos                   (4U)                                                                /*!< R/W 0 error for 12K-byte memory */
#define OV2640_DSP_MC_BIST_ERR_0_12KB_Msk                   (0x01U << OV2640_DSP_MC_BIST_ERR_0_12KB_Pos)                        /*!< 0x10 */
#define OV2640_DSP_MC_BIST_ERR_1_12KB_Pos                   (5U)                                                                /*!< R/W 1 error for 12K-byte memory */
#define OV2640_DSP_MC_BIST_ERR_1_12KB_Msk                   (0x01U << OV2640_DSP_MC_BIST_ERR_1_12KB_Pos)                        /*!< 0x20 */
#define OV2640_DSP_MC_BIST_ROM_SEL_Pos                      (6U)                                                                /*!< Boot ROM select */
#define OV2640_DSP_MC_BIST_ROM_SEL_Msk                      (0x01U << OV2640_DSP_MC_BIST_ROM_SEL_Pos)                           /*!< 0x40 */
#define OV2640_DSP_MC_BIST_MCU_RST_Pos                      (7U)                                                                /*!< Microcontroller Reset */
#define OV2640_DSP_MC_BIST_MCU_RST_Msk                      (0x01U << OV2640_DSP_MC_BIST_MCU_RST_Pos)                           /*!< 0x80 */

/*********  Bit definition for OV2640_DSP_REG_MC_AL (0xFA) register  **********/
#define OV2640_DSP_MC_AL_ADDR_L_Pos                         (0U)                                                                /*!< Program Memory Pointer Address Low Byte */
#define OV2640_DSP_MC_AL_ADDR_L_Msk                         (0xFFU << OV2640_DSP_MC_AL_ADDR_L_Pos)                              /*!< 0xFF */

/*********  Bit definition for OV2640_DSP_REG_MC_AH (0xFB) register  **********/
#define OV2640_DSP_MC_AH_ADDR_H_Pos                         (0U)                                                                /*!< Program Memory Pointer Address High Byte */
#define OV2640_DSP_MC_AH_ADDR_H_Msk                         (0xFFU << OV2640_DSP_MC_AH_ADDR_H_Pos)                              /*!< 0xFF */

/**********  Bit definition for OV2640_DSP_REG_MC_D (0xFC) register  **********/
#define OV2640_DSP_MC_D_ADDR_Pos                            (0U)                                                                /*!< Program Memory Pointer Access Address */
#define OV2640_DSP_MC_D_ADDR_Msk                            (0xFFU << OV2640_DSP_MC_D_ADDR_Pos)                                 /*!< 0xFF */

/*********  Bit definition for OV2640_DSP_REG_P_CMD (0xFD) register  **********/
#define OV2640_DSP_P_CMD_CMD_Pos                            (0U)                                                                /*!< SCCB Protocol Command */
#define OV2640_DSP_P_CMD_CMD_Msk                            (0xFFU << OV2640_DSP_P_CMD_CMD_Pos)                                 /*!< 0xFF */

/********  Bit definition for OV2640_DSP_REG_P_STATUS (0xFE) register  ********/
#define OV2640_DSP_P_STATUS_STS_Pos                         (0U)                                                                /*!< SCCB Protocol Status */
#define OV2640_DSP_P_STATUS_STS_Msk                         (0xFFU << OV2640_DSP_P_STATUS_STS_Pos)                              /*!< 0xFF */

/********  Bit definition for OV2640_DSP_REG_RA_DLMT (0xFF) register  *********/
#define OV2640_DSP_RA_DLMT_BANK_Pos                         (0U)                                                                /*!< Register bank select */
#define OV2640_DSP_RA_DLMT_BANK_Msk                         (0x01U << OV2640_DSP_RA_DLMT_BANK_Pos)                              /*!< 0x01 */



/******************************************************************************/
/*                                                                            */
/*                  OV2640 Bank1 Sensor Definition, 0xFF = 1                  */
/*                                                                            */
/******************************************************************************/

/********  Bit definition for OV2640_SENSOR_REG_GAIN (0x00) register  *********/
#define OV2640_SENSOR_GAIN_GAIN_L_Pos                       (0U)                                                                /*!< AGC Gain Control LSBs */
#define OV2640_SENSOR_GAIN_GAIN_L_Msk                       (0xFFU << OV2640_SENSOR_GAIN_GAIN_L_Pos)                            /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_COM1 (0x03) register  *********/
#define OV2640_SENSOR_COM1_VWIN_S_L_Pos                     (0U)                                                                /*!< Vertical window start line control 2 LSBs. */
#define OV2640_SENSOR_COM1_VWIN_S_L_Msk                     (0x03U << OV2640_SENSOR_COM1_VWIN_S_L_Pos)                          /*!< 0x03 */
#define OV2640_SENSOR_COM1_VWIN_E_L_Pos                     (2U)                                                                /*!< Vertical window end line control 2 LSBs. */
#define OV2640_SENSOR_COM1_VWIN_E_L_Msk                     (0x03U << OV2640_SENSOR_COM1_VWIN_E_L_Pos)                          /*!< 0x0C */
#define OV2640_SENSOR_COM1_DUMMY_FC_Pos                     (6U)                                                                /*!< Dummy frame control */
#define OV2640_SENSOR_COM1_DUMMY_FC_Msk                     (0x03U << OV2640_SENSOR_COM1_DUMMY_FC_Pos)                          /*!< 0xC0 */

/********  Bit definition for OV2640_SENSOR_REG_REG04 (0x04) register  ********/
#define OV2640_SENSOR_REG04_AEC_0_Pos                       (0U)                                                                /*!< AEC[1:0] */
#define OV2640_SENSOR_REG04_AEC_0_Msk                       (0x03U << OV2640_SENSOR_REG04_AEC_0_Pos)                            /*!< 0x03 */
#define OV2640_SENSOR_REG04_HREF_L_Pos                      (3U)                                                                /*!< HREF bit[0] */
#define OV2640_SENSOR_REG04_HREF_L_Msk                      (0x01U << OV2640_SENSOR_REG04_HREF_L_Pos)                           /*!< 0x08 */
#define OV2640_SENSOR_REG04_VREF_L_Pos                      (4U)                                                                /*!< VREF bit[0] */
#define OV2640_SENSOR_REG04_VREF_L_Msk                      (0x01U << OV2640_SENSOR_REG04_VREF_L_Pos)                           /*!< 0x10 */
#define OV2640_SENSOR_REG04_V_FLIP_Pos                      (6U)                                                                /*!< Vertical flip */
#define OV2640_SENSOR_REG04_V_FLIP_Msk                      (0x01U << OV2640_SENSOR_REG04_V_FLIP_Pos)                           /*!< 0x40 */
#define OV2640_SENSOR_REG04_H_MIRROR_Pos                    (7U)                                                                /*!< Horizontal mirror */
#define OV2640_SENSOR_REG04_H_MIRROR_Msk                    (0x01U << OV2640_SENSOR_REG04_H_MIRROR_Pos)                         /*!< 0x80 */

/********  Bit definition for OV2640_SENSOR_REG_REG08 (0x08) register  ********/
#define OV2640_SENSOR_REG08_FEXPS_PIN_Pos                   (0U)                                                                /*!< Frame Exposure One-pin Control Pre-charge Row Number */
#define OV2640_SENSOR_REG08_FEXPS_PIN_Msk                   (0xFFU << OV2640_SENSOR_REG08_FEXPS_PIN_Pos)                        /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_COM2 (0x09) register  *********/
#define OV2640_SENSOR_COM2_O_DRV_SEL_Pos                    (0U)                                                                /*!< Output drive select */
#define OV2640_SENSOR_COM2_O_DRV_SEL_Msk                    (0x03U << OV2640_SENSOR_COM2_O_DRV_SEL_Pos)                         /*!< 0x03 */
#define OV2640_SENSOR_COM2_PIN_SLV_Pos                      (2U)                                                                /*!< Pin PWDN/RESETB used as SLVS/SLHS */
#define OV2640_SENSOR_COM2_PIN_SLV_Msk                      (0x01U << OV2640_SENSOR_COM2_PIN_SLV_Pos)                           /*!< 0x04 */
#define OV2640_SENSOR_COM2_STBY_EN_Pos                      (4U)                                                                /*!< Standby mode enable */
#define OV2640_SENSOR_COM2_STBY_EN_Msk                      (0x01U << OV2640_SENSOR_COM2_STBY_EN_Pos)                           /*!< 0x10 */

/********  Bit definition for OV2640_SENSOR_REG_PIDH (0x0A) register  *********/
#define OV2640_SENSOR_PIDH_PID_H_Pos                        (0U)                                                                /*!< Product ID Number MSB (Read only) */
#define OV2640_SENSOR_PIDH_PID_H_Msk                        (0xFFU << OV2640_SENSOR_PIDH_PID_H_Pos)                             /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_PIDL (0x0B) register  *********/
#define OV2640_SENSOR_PIDL_PID_L_Pos                        (0U)                                                                /*!< Product ID Number LSB (Read only) */
#define OV2640_SENSOR_PIDL_PID_L_Msk                        (0xFFU << OV2640_SENSOR_PIDL_PID_L_Pos)                             /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_COM3 (0x0C) register  *********/
#define OV2640_SENSOR_COM3_SNAPSHOT_OPT_Pos                 (0U)                                                                /*!< Snapshot option */
#define OV2640_SENSOR_COM3_SNAPSHOT_OPT_Msk                 (0x01U << OV2640_SENSOR_COM3_SNAPSHOT_OPT_Pos)                      /*!< 0x01 */
#define OV2640_SENSOR_COM3_AUTO_BD_Pos                      (1U)                                                                /*!< Auto set banding */
#define OV2640_SENSOR_COM3_AUTO_BD_Msk                      (0x01U << OV2640_SENSOR_COM3_AUTO_BD_Pos)                           /*!< 0x02 */
#define OV2640_SENSOR_COM3_BD_Pos                           (2U)                                                                /*!< Set banding manually */
#define OV2640_SENSOR_COM3_BD_Msk                           (0x01U << OV2640_SENSOR_COM3_BD_Pos)                                /*!< 0x04 */

/*********  Bit definition for OV2640_SENSOR_REG_AEC (0x10) register  *********/
#define OV2640_SENSOR_AEC_AEC_1_Pos                         (0U)                                                                /*!< Automatic Exposure Control 8 bits for AEC[9:2] */
#define OV2640_SENSOR_AEC_AEC_1_Msk                         (0xFFU << OV2640_SENSOR_AEC_AEC_1_Pos)                              /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_CLKRC (0x11) register  ********/
#define OV2640_SENSOR_CLKRC_CLK_DIV_Pos                     (0U)                                                                /*!< Clock divider */
#define OV2640_SENSOR_CLKRC_CLK_DIV_Msk                     (0x3FU << OV2640_SENSOR_CLKRC_CLK_DIV_Pos)                          /*!< 0x3F */
#define OV2640_SENSOR_CLKRC_IN_DBL_Pos                      (7U)                                                                /*!< Internal frequency doublers ON/OFF selection */
#define OV2640_SENSOR_CLKRC_IN_DBL_Msk                      (0x01U << OV2640_SENSOR_CLKRC_IN_DBL_Pos)                           /*!< 0x80 */

/********  Bit definition for OV2640_SENSOR_REG_COM7 (0x12) register  *********/
#define OV2640_SENSOR_COM7_COLOR_BAR_PTN_Pos                (1U)                                                                /*!< Color bar test pattern */
#define OV2640_SENSOR_COM7_COLOR_BAR_PTN_Msk                (0x01U << OV2640_SENSOR_COM7_COLOR_BAR_PTN_Pos)                     /*!< 0x02 */
#define OV2640_SENSOR_COM7_ZOOM_MODE_Pos                    (2U)                                                                /*!< Zoom mode */
#define OV2640_SENSOR_COM7_ZOOM_MODE_Msk                    (0x01U << OV2640_SENSOR_COM7_ZOOM_MODE_Pos)                         /*!< 0x04 */
#define OV2640_SENSOR_COM7_RES_SEL_Pos                      (4U)                                                                /*!< Resolution selection */
#define OV2640_SENSOR_COM7_RES_SEL_Msk                      (0x07U << OV2640_SENSOR_COM7_RES_SEL_Pos)                           /*!< 0x70 */
#define OV2640_SENSOR_COM7_SRST_Pos                         (7U)                                                                /*!< Initiates system reset */
#define OV2640_SENSOR_COM7_SRST_Msk                         (0x01U << OV2640_SENSOR_COM7_SRST_Pos)                              /*!< 0x80 */

/********  Bit definition for OV2640_SENSOR_REG_COM8 (0x13) register  *********/
#define OV2640_SENSOR_COM8_EXPS_CTRL_Pos                    (0U)                                                                /*!< Exposure control */
#define OV2640_SENSOR_COM8_EXPS_CTRL_Msk                    (0x01U << OV2640_SENSOR_COM8_EXPS_CTRL_Pos)                         /*!< 0x01 */
#define OV2640_SENSOR_COM8_AGC_EN_Pos                       (2U)                                                                /*!< AGC auto/manual control selection */
#define OV2640_SENSOR_COM8_AGC_EN_Msk                       (0x01U << OV2640_SENSOR_COM8_AGC_EN_Pos)                            /*!< 0x04 */
#define OV2640_SENSOR_COM8_BD_FLTR_Pos                      (5U)                                                                /*!< Banding filter selection */
#define OV2640_SENSOR_COM8_BD_FLTR_Msk                      (0x01U << OV2640_SENSOR_COM8_BD_FLTR_Pos)                           /*!< 0x20 */

/********  Bit definition for OV2640_SENSOR_REG_COM9 (0x14) register  *********/
#define OV2640_SENSOR_COM9_AGC_GAIN_CEILING_Pos             (5U)                                                                /*!< AGC gain ceiling */
#define OV2640_SENSOR_COM9_AGC_GAIN_CEILING_Msk             (0x07U << OV2640_SENSOR_COM9_AGC_GAIN_CEILING_Pos)                  /*!< 0xE0 */

/********  Bit definition for OV2640_SENSOR_REG_COM10 (0x15) register  ********/
#define OV2640_SENSOR_COM10_VSYNC_POL_Pos                   (1U)                                                                /*!< VSYNC polarity */
#define OV2640_SENSOR_COM10_VSYNC_POL_Msk                   (0x01U << OV2640_SENSOR_COM10_VSYNC_POL_Pos)                        /*!< 0x02 */
#define OV2640_SENSOR_COM10_HREF_POL_Pos                    (3U)                                                                /*!< HREF output polarity */
#define OV2640_SENSOR_COM10_HREF_POL_Msk                    (0x01U << OV2640_SENSOR_COM10_HREF_POL_Pos)                         /*!< 0x08 */
#define OV2640_SENSOR_COM10_PCLK_EDGE_Pos                   (4U)                                                                /*!< PCLK edge selection */
#define OV2640_SENSOR_COM10_PCLK_EDGE_Msk                   (0x01U << OV2640_SENSOR_COM10_PCLK_EDGE_Pos)                        /*!< 0x10 */
#define OV2640_SENSOR_COM10_PCLK_OUT_SEL_Pos                (5U)                                                                /*!< PCLK output selection */
#define OV2640_SENSOR_COM10_PCLK_OUT_SEL_Msk                (0x01U << OV2640_SENSOR_COM10_PCLK_OUT_SEL_Pos)                     /*!< 0x20 */

/*******  Bit definition for OV2640_SENSOR_REG_HREFST (0x17) register  ********/
#define OV2640_SENSOR_HREFST_HWIN_S_Pos                     (0U)                                                                /*!< Horizontal Window Start MSB 8 bits */
#define OV2640_SENSOR_HREFST_HWIN_S_Msk                     (0xFFU << OV2640_SENSOR_HREFST_HWIN_S_Pos)                          /*!< 0xFF */

/*******  Bit definition for OV2640_SENSOR_REG_HREFEND (0x18) register  *******/
#define OV2640_SENSOR_HREFEND_HWIN_E_Pos                    (0U)                                                                /*!< Horizontal Window End MSB 8 bits */
#define OV2640_SENSOR_HREFEND_HWIN_E_Msk                    (0xFFU << OV2640_SENSOR_HREFEND_HWIN_E_Pos)                         /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_VSTRT (0x19) register  ********/
#define OV2640_SENSOR_VSTRT_VWIN_S_Pos                      (0U)                                                                /*!< Vertical Window Line Start MSB 8 bits */
#define OV2640_SENSOR_VSTRT_VWIN_S_Msk                      (0xFFU << OV2640_SENSOR_VSTRT_VWIN_S_Pos)                           /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_VEND (0x1A) register  *********/
#define OV2640_SENSOR_VEND_VWIN_E_Pos                       (0U)                                                                /*!< Vertical Window Line End MSB 8 bits */
#define OV2640_SENSOR_VEND_VWIN_E_Msk                       (0xFFU << OV2640_SENSOR_VEND_VWIN_E_Pos)                            /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_MIDH (0x1C) register  *********/
#define OV2640_SENSOR_MIDH_MID_H_Pos                        (0U)                                                                /*!< Manufacturer ID Byte - High (Read only = 0x7F) */
#define OV2640_SENSOR_MIDH_MID_H_Msk                        (0xFFU << OV2640_SENSOR_MIDH_MID_H_Pos)                             /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_MIDL (0x1D) register  *********/
#define OV2640_SENSOR_MIDL_MID_L_Pos                        (0U)                                                                /*!< Manufacturer ID Byte - Low (Read only = 0xA2) */
#define OV2640_SENSOR_MIDL_MID_L_Msk                        (0xFFU << OV2640_SENSOR_MIDL_MID_L_Pos)                             /*!< 0xFF */

/*********  Bit definition for OV2640_SENSOR_REG_AEW (0x24) register  *********/
#define OV2640_SENSOR_AEW_AEW_Pos                           (0U)                                                                /*!< Luminance Signal High Range for AEC/AGC Operation */
#define OV2640_SENSOR_AEW_AEW_Msk                           (0xFFU << OV2640_SENSOR_AEW_AEW_Pos)                                /*!< 0xFF */

/*********  Bit definition for OV2640_SENSOR_REG_AEB (0x25) register  *********/
#define OV2640_SENSOR_AEB_AEB_Pos                           (0U)                                                                /*!< Luminance Signal Low Range for AEC/AGC Operation */
#define OV2640_SENSOR_AEB_AEB_Msk                           (0xFFU << OV2640_SENSOR_AEB_AEB_Pos)                                /*!< 0xFF */

/*********  Bit definition for OV2640_SENSOR_REG_VV (0x26) register  **********/
#define OV2640_SENSOR_VV_LOW_TH_Pos                         (0U)                                                                /*!< Low threshold */
#define OV2640_SENSOR_VV_LOW_TH_Msk                         (0x0FU << OV2640_SENSOR_VV_LOW_TH_Pos)                              /*!< 0x0F */
#define OV2640_SENSOR_VV_HIGH_TH_Pos                        (4U)                                                                /*!< High threshold */
#define OV2640_SENSOR_VV_HIGH_TH_Msk                        (0x0FU << OV2640_SENSOR_VV_HIGH_TH_Pos)                             /*!< 0xF0 */

/********  Bit definition for OV2640_SENSOR_REG_REG2A (0x2A) register  ********/
#define OV2640_SENSOR_REG2A_FRAR_H_Pos                      (4U)                                                                /*!< Line interval adjust value 4 MSBs */
#define OV2640_SENSOR_REG2A_FRAR_H_Msk                      (0x0FU << OV2640_SENSOR_REG2A_FRAR_H_Pos)                           /*!< 0xF0 */

/********  Bit definition for OV2640_SENSOR_REG_FRARL (0x2B) register  ********/
#define OV2640_SENSOR_FRARL_FRAR_L_Pos                      (0U)                                                                /*!< Line Interval Adjustment Value LSB 8 bits */
#define OV2640_SENSOR_FRARL_FRAR_L_Msk                      (0xFFU << OV2640_SENSOR_FRARL_FRAR_L_Pos)                           /*!< 0xFF */

/*******  Bit definition for OV2640_SENSOR_REG_ADDVSL (0x2D) register  ********/
#define OV2640_SENSOR_ADDVSL_ADDVS_L_Pos                    (0U)                                                                /*!< VSYNC Pulse Width LSB 8 bits */
#define OV2640_SENSOR_ADDVSL_ADDVS_L_Msk                    (0xFFU << OV2640_SENSOR_ADDVSL_ADDVS_L_Pos)                         /*!< 0xFF */

/*******  Bit definition for OV2640_SENSOR_REG_ADDVSH (0x2E) register  ********/
#define OV2640_SENSOR_ADDVSH_ADDVS_H_Pos                    (0U)                                                                /*!< VSYNC Pulse Width MSB 8 bits */
#define OV2640_SENSOR_ADDVSH_ADDVS_H_Msk                    (0xFFU << OV2640_SENSOR_ADDVSH_ADDVS_H_Pos)                         /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_YAVG (0x2F) register  *********/
#define OV2640_SENSOR_YAVG_YAVG_Pos                         (0U)                                                                /*!< Luminance Average */
#define OV2640_SENSOR_YAVG_YAVG_Msk                         (0xFFU << OV2640_SENSOR_YAVG_YAVG_Pos)                              /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_REG32 (0x32) register  ********/
#define OV2640_SENSOR_REG32_HWIN_S_L_Pos                    (0U)                                                                /*!< Horizontal window start position 3 LSBs */
#define OV2640_SENSOR_REG32_HWIN_S_L_Msk                    (0x07U << OV2640_SENSOR_REG32_HWIN_S_L_Pos)                         /*!< 0x07 */
#define OV2640_SENSOR_REG32_HWIN_E_L_Pos                    (3U)                                                                /*!< Horizontal window end position 3 LSBs */
#define OV2640_SENSOR_REG32_HWIN_E_L_Msk                    (0x07U << OV2640_SENSOR_REG32_HWIN_E_L_Pos)                         /*!< 0x38 */
#define OV2640_SENSOR_REG32_PCLK_DIV_Pos                    (6U)                                                                /*!< Pixel clock divide option */
#define OV2640_SENSOR_REG32_PCLK_DIV_Msk                    (0x03U << OV2640_SENSOR_REG32_PCLK_DIV_Pos)                         /*!< 0xC0 */

/*******  Bit definition for OV2640_SENSOR_REG_ARCOM2 (0x34) register  ********/
#define OV2640_SENSOR_ARCOM2_ZOOM_HWIN_S_Pos                (2U)                                                                /*!< Zoom window horizontal start point */
#define OV2640_SENSOR_ARCOM2_ZOOM_HWIN_S_Msk                (0x01U << OV2640_SENSOR_ARCOM2_ZOOM_HWIN_S_Pos)                     /*!< 0x04 */

/********  Bit definition for OV2640_SENSOR_REG_REG45 (0x45) register  ********/
#define OV2640_SENSOR_REG45_AEC_2_Pos                       (0U)                                                                /*!< AEC[15:10], AEC MSBs */
#define OV2640_SENSOR_REG45_AEC_2_Msk                       (0x3FU << OV2640_SENSOR_REG45_AEC_2_Pos)                            /*!< 0x3F */
#define OV2640_SENSOR_REG45_AGC_H_Pos                       (6U)                                                                /*!< AGC[9:8], AGC highest gain control */
#define OV2640_SENSOR_REG45_AGC_H_Msk                       (0x03U << OV2640_SENSOR_REG45_AGC_H_Pos)                            /*!< 0xC0 */

/*********  Bit definition for OV2640_SENSOR_REG_FLL (0x46) register  *********/
#define OV2640_SENSOR_FLL_FL_L_Pos                          (0U)                                                                /*!< Frame Length Adjustment LSBs */
#define OV2640_SENSOR_FLL_FL_L_Msk                          (0xFFU << OV2640_SENSOR_FLL_FL_L_Pos)                               /*!< 0xFF */

/*********  Bit definition for OV2640_SENSOR_REG_FLH (0x47) register  *********/
#define OV2640_SENSOR_FLH_FL_H_Pos                          (0U)                                                                /*!< Frame Length Adjustment MSBs */
#define OV2640_SENSOR_FLH_FL_H_Msk                          (0xFFU << OV2640_SENSOR_FLH_FL_H_Pos)                               /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_COM19 (0x48) register  ********/
#define OV2640_SENSOR_COM19_ZOOM_VWIN_S_L_Pos               (0U)                                                                /*!< Zoom Mode Vertical Window Start Point 2 LSBs */
#define OV2640_SENSOR_COM19_ZOOM_VWIN_S_L_Msk               (0x03U << OV2640_SENSOR_COM19_ZOOM_VWIN_S_L_Pos)                    /*!< 0x03 */

/********  Bit definition for OV2640_SENSOR_REG_ZOOMS (0x49) register  ********/
#define OV2640_SENSOR_ZOOMS_ZOOM_VWIN_S_H_Pos               (0U)                                                                /*!< Zoom Mode Vertical Window Start Point 8 MSBs */
#define OV2640_SENSOR_ZOOMS_ZOOM_VWIN_S_H_Msk               (0xFFU << OV2640_SENSOR_ZOOMS_ZOOM_VWIN_S_H_Pos)                    /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_COM22 (0x4B) register  ********/
#define OV2640_SENSOR_COM22_FLASH_CTRL_Pos                  (0U)                                                                /*!< Flash light control */
#define OV2640_SENSOR_COM22_FLASH_CTRL_Msk                  (0xFFU << OV2640_SENSOR_COM22_FLASH_CTRL_Pos)                       /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_COM25 (0x4E) register  ********/
#define OV2640_SENSOR_COM25_BD60_H_Pos                      (4U)                                                                /*!< 60HZ Banding AEC 2 MSBs */
#define OV2640_SENSOR_COM25_BD60_H_Msk                      (0x03U << OV2640_SENSOR_COM25_BD60_H_Pos)                           /*!< 0x30 */
#define OV2640_SENSOR_COM25_BD50_H_Pos                      (6U)                                                                /*!< 50Hz Banding AEC 2 MSBs */
#define OV2640_SENSOR_COM25_BD50_H_Msk                      (0x03U << OV2640_SENSOR_COM25_BD50_H_Pos)                           /*!< 0xC0 */

/********  Bit definition for OV2640_SENSOR_REG_BD50 (0x4F) register  *********/
#define OV2640_SENSOR_BD50_BD50_L_Pos                       (0U)                                                                /*!< 50Hz Banding AEC 8 LSBs */
#define OV2640_SENSOR_BD50_BD50_L_Msk                       (0xFFU << OV2640_SENSOR_BD50_BD50_L_Pos)                            /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_BD60 (0x50) register  *********/
#define OV2640_SENSOR_BD60_BD60_L_Pos                       (0U)                                                                /*!< 60Hz Banding AEC 8 LSBs */
#define OV2640_SENSOR_BD60_BD60_L_Msk                       (0xFFU << OV2640_SENSOR_BD60_BD60_L_Pos)                            /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_REG5D (0x5D) register  ********/
#define OV2640_SENSOR_REG5D_AVG_SEL_0_Pos                   (0U)                                                                /*!< AVGsel[7:0], 16-zone average weight option */
#define OV2640_SENSOR_REG5D_AVG_SEL_0_Msk                   (0xFFU << OV2640_SENSOR_REG5D_AVG_SEL_0_Pos)                        /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_REG5E (0x5E) register  ********/
#define OV2640_SENSOR_REG5E_AVG_SEL_1_Pos                   (0U)                                                                /*!< AVGsel[15:8], 16-zone average weight option */
#define OV2640_SENSOR_REG5E_AVG_SEL_1_Msk                   (0xFFU << OV2640_SENSOR_REG5E_AVG_SEL_1_Pos)                        /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_REG5F (0x5F) register  ********/
#define OV2640_SENSOR_REG5F_AVG_SEL_2_Pos                   (0U)                                                                /*!< AVGsel[23:16], 16-zone average weight option */
#define OV2640_SENSOR_REG5F_AVG_SEL_2_Msk                   (0xFFU << OV2640_SENSOR_REG5F_AVG_SEL_2_Pos)                        /*!< 0xFF */

/********  Bit definition for OV2640_SENSOR_REG_REG60 (0x60) register  ********/
#define OV2640_SENSOR_REG60_AVG_SEL_3_Pos                   (0U)                                                                /*!< AVGsel[31:24], 16-zone average weight option */
#define OV2640_SENSOR_REG60_AVG_SEL_3_Msk                   (0xFFU << OV2640_SENSOR_REG60_AVG_SEL_3_Pos)                        /*!< 0xFF */

/******  Bit definition for OV2640_SENSOR_REG_HISTO_LOW (0x61) register  ******/
#define OV2640_SENSOR_HISTO_LOW_HISTO_L_Pos                 (0U)                                                                /*!< Histogram Algorithm Low Level */
#define OV2640_SENSOR_HISTO_LOW_HISTO_L_Msk                 (0xFFU << OV2640_SENSOR_HISTO_LOW_HISTO_L_Pos)                      /*!< 0xFF */

/*****  Bit definition for OV2640_SENSOR_REG_HISTO_HIGH (0x62) register  ******/
#define OV2640_SENSOR_HISTO_HIGH_HISTO_H_Pos                (0U)                                                                /*!< Histogram Algorithm High Level */
#define OV2640_SENSOR_HISTO_HIGH_HISTO_H_Msk                (0xFFU << OV2640_SENSOR_HISTO_HIGH_HISTO_H_Pos)                     /*!< 0xFF */

/*******  Bit definition for OV2640_SENSOR_REG_RA_DLMT (0xFF) register  *******/
#define OV2640_SENSOR_RA_DLMT_BANK_Pos                      (0U)                                                                /*!< Register bank select */
#define OV2640_SENSOR_RA_DLMT_BANK_Msk                      (0x01U << OV2640_SENSOR_RA_DLMT_BANK_Pos)                           /*!< 0x01 */




/* ********* Bank 0 ************** */
/* OV2640_DSP_BYPASS_SEL (0x05[0])  Bypass DSP select */
#define OV2640_DSP_BYPASS_SEL_DSP                           0   /* DSP */
#define OV2640_DSP_BYPASS_SEL_DIRECT                        1   /* Bypass DSP, sensor out directly */
/* OV2640_CTRL0 (0xC2[4])  VFIRST */
#define OV2640_CTRL0_VFIRST_DISABLE                          0   /* Disable VFIRST */
#define OV2640_CTRL0_VFIRST_ENABLE                           1   /* Enable VFIRST */
/* OV2640_DSP_IMAGE_MODE_JPEG_OUT_EN (0xDA[4])  JPEG output enable */
#define OV2640_DSP_IMAGE_MODE_JPEG_OUT_ENNOT_COMPRS         0   /* Non-compressed */
#define OV2640_DSP_IMAGE_MODE_JPEG_OUT_ENOUT_JPEG           1   /* JPEG output */
/* OV2640_DSP_IMAGE_MODE_DVP_OUT_FMT (0xDA[3:2])  DVP output format */
#define OV2640_DSP_IMAGE_MODE_DVP_OUT_FMT_YUV422            0   /* YUV422 */
#define OV2640_DSP_IMAGE_MODE_DVP_OUT_FMT_RAW10             1   /* RAW10 (DVP) */
#define OV2640_DSP_IMAGE_MODE_DVP_OUT_FMT_RGB565            2   /* RGB565 */
/* OV2640_DSP_IMAGE_MODE_HREF_TIMING (0xDA[1])  HREF timing select in DVP JPEG output mode */
#define OV2640_DSP_IMAGE_MODE_HREF_TIMING_SENSOR            0   /* HREF is same as sensor */
#define OV2640_DSP_IMAGE_MODE_HREF_TIMING_VSYNC             1   /* HREF = VSYNC */
/* OV2640_DSP_IMAGE_MODE_BYTE_SWAP (0xDA[0])  Byte swap enable for DVP */
#define OV2640_DSP_IMAGE_MODE_BYTE_SWAP_MSB_FIRST           0   /*  YUYV (@C2[4]:VFIRST=0) YVYU (@C2[4]:VFIRST=1) */
#define OV2640_DSP_IMAGE_MODE_BYTE_SWAP_LSB_FIRST           1   /*  UYVY (@C2[4]:VFIRST=0) VYUY (@C2[4]:VFIRST=1) */
/* OV2640_DSP_REGED_CO_PDWN (0xED[4]) Clock output power-down pin status  */
#define OV2640_DSP_REGED_CO_PDWN_HOLD_LAST                  0   /* Data output pin hold at last state before power-down */
#define OV2640_DSP_REGED_CO_PDWN_TRISTATE                   1   /* Tri-state data output pin upon power-down */
/* OV2640_DSP_RA_DLMT_BANK (0xFF[0])  Register bank select */
#define OV2640_DSP_RA_DLMT_BANK_DSP                         0   /* DSP address */
#define OV2640_DSP_RA_DLMT_BANK_SENSOR                      1   /* Sensor address */

/* ********* Bank 1 ************** */
/* OV2640_SENSOR_COM1_DUMMY_FC (0x03[7:6])   Dummy frame control*/
#define OV2640_SENSOR_COM1_DUMMY_FC_1                       1   /* Allow 1 dummy frame */
#define OV2640_SENSOR_COM1_DUMMY_FC_3                       2   /* Allow 3 dummy frames */
#define OV2640_SENSOR_COM1_DUMMY_FC_7                       3   /* Allow 7 dummy frames */
/* OV2640_SENSOR_COM2_STBY_EN (0x09[4])  Standby mode enable */
#define OV2640_SENSOR_COM2_STBY_EN_NORMAL                   0   /* Normal mode */
#define OV2640_SENSOR_COM2_STBY_EN_STANDBY                  1   /* Standby mode */
/* OV2640_SENSOR_COM2_O_DRV_SEL (0x09[1:0])  Output drive select */
#define OV2640_SENSOR_COM2_O_DRV_SEL_1X_CAP                 0   /* 1x capability */
#define OV2640_SENSOR_COM2_O_DRV_SEL_3X_CAP                 1   /* 3x capability */
#define OV2640_SENSOR_COM2_O_DRV_SEL_2X_CAP                 2   /* 2x capability */
#define OV2640_SENSOR_COM2_O_DRV_SEL_4X_CAP                 3   /* 4x capability */
/* OV2640_SENSOR_COM3_BD (0x0c[2])  Set banding manually */
#define OV2640_SENSOR_COM3_BD_60HZ                          0   /* 60 Hz */
#define OV2640_SENSOR_COM3_BD_50HZ                          1   /* 50 Hz */
/* OV2640_SENSOR_COM3_SNAPSHOT_OPT (0x0C[0])  Snapshot option */
#define OV2640_SENSOR_COM3_SNAPSHOT_OPT_LIVE_VIDEO          0   /* Enable live video output after snapshot sequence */
#define OV2640_SENSOR_COM3_SNAPSHOT_OPT_SINGLE_FRAME        1   /* Output single frame only */
/* OV2640_SENSOR_CLKRC_IN_DBL (0x11[7])  Internal frequency doublers ON/OFF selection */
#define OV2640_SENSOR_CLKRC_IN_DBL_OFF                      0   /* OFF */
#define OV2640_SENSOR_CLKRC_IN_DBL_ON                       1   /* ON */
/* OV2640_SENSOR_COM7_RES_SEL (0x12[6:4])  Resolution selection */
#define OV2640_SENSOR_COM7_RES_SEL_UXGA                     0   /* UXGA (full size) mode */
#define OV2640_SENSOR_COM7_RES_SEL_CIF                      2   /* CIF mode */
#define OV2640_SENSOR_COM7_RES_SEL_SVGA                     4   /* SVGA mode */
/* OV2640_SENSOR_COM7_COLOR_BAR_PTN (0x12[1])  Color bar test pattern */
#define OV2640_SENSOR_COM7_COLOR_BAR_PTN_OFF                0   /* OFF */
#define OV2640_SENSOR_COM7_COLOR_BAR_PTN_ON                 1   /* ON */
/* OV2640_SENSOR_COM8_BD_FLTR (0x13[5])  Banding filter selection */
#define OV2640_SENSOR_COM8_BD_FLTR_OFF                      0   /* OFF */
#define OV2640_SENSOR_COM8_BD_FLTR_ON                       1   /* ON, set minimum exposure time to 1/120s */
/* OV2640_SENSOR_COM8_AGC_EN (0x13[2])  AGC auto/manual control selection */
#define OV2640_SENSOR_COM8_AGC_EN_DISABLE                   0   /* Manual */
#define OV2640_SENSOR_COM8_AGC_EN_ENABLE                    1   /* Auto */
/* OV2640_SENSOR_COM8_EXPS_CTRL (0x13[0])  Exposure control */
#define OV2640_SENSOR_COM8_EXPS_CTRL_MANUALY                0   /* Manual */
#define OV2640_SENSOR_COM8_EXPS_CTRL_AUTO                   1   /* Auto */
/* OV2640_SENSOR_COM9_AGC_GAIN_CEILING (0x14[7:5]) AGC gain ceiling, GH[2:0] */
#define OV2640_SENSOR_COM9_AGC_GAIN_CEILING_2X              0   /* 000: 2x   */
#define OV2640_SENSOR_COM9_AGC_GAIN_CEILING_4X              1   /* 001: 4x   */
#define OV2640_SENSOR_COM9_AGC_GAIN_CEILING_8X              2   /* 010: 8x   */
#define OV2640_SENSOR_COM9_AGC_GAIN_CEILING_16X             3   /* 011: 16x  */
#define OV2640_SENSOR_COM9_AGC_GAIN_CEILING_32X             4   /* 100: 32x  */
#define OV2640_SENSOR_COM9_AGC_GAIN_CEILING_64X             5   /* 101: 64x  */
#define OV2640_SENSOR_COM9_AGC_GAIN_CEILING_128             6   /* 11x: 128x */
/* OV2640_SENSOR_COM10_PCLK_OUT_SEL (0x15[5]) PCLK output selection */
#define OV2640_SENSOR_COM10_PCLK_OUT_SEL_ALWAYS             0   /* PCLK always output */
#define OV2640_SENSOR_COM10_PCLK_OUT_SEL_HREF               1   /* PCLK output qualified by HREF */
/* OV2640_SENSOR_COM10_PCLK_EDGE (0x15[4]) PCLK edge selection */
#define OV2640_SENSOR_COM10_PCLK_EDGE_FALLING               0   /* Data is updated at the falling edge of PCLK (user can latch data at the next rising edge of PCLK) */
#define OV2640_SENSOR_COM10_PCLK_EDGE_RISING                1   /* Data is updated at the rising edge of PCLK (user can latch data at the next falling edge of PCLK) */
/* OV2640_SENSOR_COM10_HREF_POL (0x15[3]) HREF output polarity */
#define OV2640_SENSOR_COM10_HREF_POL_POSTIVIE               0   /* Output positive HREF */
#define OV2640_SENSOR_COM10_HREF_POL_NEGATIVE               1   /* Output negative HREF, HREF negative for data valid */
/* OV2640_SENSOR_COM10_VSYNC_POL (0x15[1]) VSYNC polarity */
#define OV2640_SENSOR_COM10_VSYNC_POL_POSTIVIVE             0   /* Positive */
#define OV2640_SENSOR_COM10_VSYNC_POL_NEGATIVE              1   /* Negative */
/* OV2640_SENSOR_REG32_PCLK_DIV (0x32[7:6]) Pixel clock divide option */
#define OV2640_SENSOR_REG32_PCLK_DIV_NONE                   0   /* No effect on PCLK */
#define OV2640_SENSOR_REG32_PCLK_DIV_2                      2   /* PCLK frequency divide by 2 */
#define OV2640_SENSOR_REG32_PCLK_DIV_4                      3   /* PCLK frequency divide by 4 */

/* OV2640 Windows and Image size related */
/* Windows - H_START */
#define OV2640_WIN_HSTART_SENSOR_REG32_Pos                  0   /* The Windows H start value [2:0] bit position (write to REG32) */
#define OV2640_WIN_HSTART_SENSOR_HREFST_Pos                 3   /* The Windows H start value [10:3] bit position (write to HREFST) */
/* Windows - H_END */
#define OV2640_WIN_HEND_SENSOR_REG32_Pos                    0   /* The Windows H end value [2:0] bit position (write to REG32) */
#define OV2640_WIN_HEND_SENSOR_HREFEND_Pos                  3   /* The Windows H end value [10L3] bit position (write to HREFEND) */
/* Windows - V_START */
#define OV2640_WIN_VSTART_SENSOR_COM1_Pos                   0   /* The Windows V start value [1:0] bit position (write to COM1) */
#define OV2640_WIN_VSTART_SENSOR_VSTRT_Pos                  2   /* The Windows V start value [9:2] bit position (write to VSTRT) */
/* Windows - V_END */
#define OV2640_WIN_VEND_SENSOR_COM1_Pos                     0   /* The Windows V end value [1:0] bit position (write to COM1) */
#define OV2640_WIN_VEND_SENSOR_VEND_Pos                     2   /* The Windows V end value [9:2] bit position (write to VEND) */
/* Image size - H_SIZE */
#define OV2640_IMG_HSIZE_0_DSP_SIZEL_Pos                    0   /* The Image horizontal size [2:0] bit position (write to SIZEL) */
#define OV2640_IMG_HSIZE_1_DSP_HSIZE8_Pos                   3   /* The Image horizontal size [10:3] bit position (write to HSIZE8) */
#define OV2640_IMG_HSIZE_2_DSP_SIZEL_Pos                   11   /* The Image horizontal size [11] bit position (write to SIZEL) */
/* Image size - V_SIZE */
#define OV2640_IMG_VSIZE_0_DSP_SIZEL_Pos                    0   /* The Image vertical size [2:0] bit position (write to SIZEL) */
#define OV2640_IMG_VSIZE_1_DSP_VSIZE8_Pos                   3   /* The Image vertical size [10:3] bit position (write to VSIZE8) */
/* Image Windows - H_SIZE */
#define OV2640_IMG_WIN_HSIZE_0_DSP_HSIZE_Pos                0   /* The Image windows horitzontal size [7:0] bit position (write to HSIZE) */
#define OV2640_IMG_WIN_HSIZE_1_DSP_VHYX_Pos                 8   /* The Image windows horitzontal size [8] bit position (write to VHYX) */
#define OV2640_IMG_WIN_HSIZE_2_DSP_TEST_Pos                 9   /* The Image windows horitzontal size [9] bit position (write to TEST) */
/* Image Windows - V_SIZE */
#define OV2640_IMG_WIN_VSIZE_0_DSP_VSIZE_Pos                0   /* The Image windows vertical size [7:0] bit position (write to VSIZE) */
#define OV2640_IMG_WIN_VSIZE_1_DSP_VHYX_Pos                 8   /* The Image windows vertical size [8] bit position (write to VHYX) */
/* Image Windows - OFFSET_X */
#define OV2640_IMG_WIN_OFFSET_X_DSP_XOFFL_Pos               0   /* The Image windows horitzontal offset [7:0] bit position (write to XOFFL) */
#define OV2640_IMG_WIN_OFFSET_X_DSP_VHYX_Pos                8   /* The Image windows horitzontal offset [10:8] bit position (write to VHYX) */
/* Image Windows - OFFSET_Y */
#define OV2640_IMG_WIN_OFFSET_Y_DSP_YOFFL_Pos               0   /* The Image windows vertical offset [7:0] bit position (write to YOFFL) */
#define OV2640_IMG_WIN_OFFSET_Y_DSP_VHYX_Pos                8   /* The Image windows vertical offset [10:8] bit position (write to VHYX) */
/* Output size - OUTW */
#define OV2640_OUT_WIDTH_DSP_ZMOW_Pos                       0   /* The output horizontal size [7:0] bit position (write to ZMOW) */
#define OV2640_OUT_WIDTH_DSP_ZMHH_Pos                       8   /* The output horizontal size [9:8] bit position (write to ZMHH) */
/* Output size - OUTH */
#define OV2640_OUT_HEIGHT_DSP_ZMOH_Pos                      0   /* The output vertical size [7:0] bit position (write to ZMOH) */
#define OV2640_OUT_HEIGHT_DSP_ZMHH_Pos                      8   /* The output vertical size [9:8] bit position (write to ZMHH) */
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */



/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */


#ifdef __cplusplus
}
#endif
#endif  /* _OV2640_DEF_H_ */
