/******************************************************************************
*  Copyright 2017 - 2022, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2022
******************************************************************************/
/**
 ****************************************************************************** 
 * @file opl2500_patch.h
 * @author Opulinks Software Team
 * @version V1.1.0
 * @date Jun 1, 2022
 * @brief CMSIS Cortex-M4 Device Peripheral Access Layer Header file.
 *        This file includes all peripheral registers definitions and memory mapping.
 */
#ifndef _OPL2500_H_
#error "Please include opl2500.h. Don't directly include opl2500_patch.h"
#endif
 
#ifndef _OPL2500_PATCH_H_
#define _OPL2500_PATCH_H_

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup opl2500
  * @{
  */
 

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Peripheral_registers_structures_patch
  * @{
  */ 

#pragma push
#pragma anon_unions

/** @brief System control register (SYS) */
typedef struct
{
          uint32_t RESERVED0[3];                            /* Offset: 0x000 ~ 0x008                                                                                */
    __IOM uint32_t SRAM_BYPASS;                             /* Offset: 0x00C (R/W) Bypass SRAM                                                                      */
    __IOM uint32_t SW_RESET_EN;                             /* Offset: 0x010 (R/W) Software reset enable                                                            */
    __IOM uint32_t SW_DBG_EN;                               /* Offset: 0x014 (R/W) Software debug enable                                                            */
    __IM  uint32_t BOOT_STS;                                /* Offset: 0x018 (R/ ) Boot status for ROM code                                                         */
    __IM  uint32_t CHIP_ID;                                 /* Offset: 0x01C (R/ ) Chip identification                                                              */
    __IOM uint32_t DMA_DBG_SEL;                             /* Offset: 0x020 (R/W) DMA debug enable                                                                 */
    __IOM uint32_t DET_XTAL;                                /* Offset: 0x024 (R/W) Detect XTAL clock                                                                */
    __IM  uint32_t DET_XTAL_STS;                            /* Offset: 0x028 (R/ ) Detect XTAL state                                                                */
    __IM  uint32_t TOP_DBG_O;                               /* Offset: 0x02C (R/ )                                                                                  */
          uint32_t RESERVED1[7];                            /* Offset: 0x030 ~ 0x048                                                                                */
    __IOM uint32_t UART_SCLK_FON;                           /* Offset: 0x04C (R/W)                                                                                  */
          uint32_t RESERVED2[32];                           /* Offset: 0x050 ~ 0x0CC                                                                                */
    __IOM uint32_t APS_I_PATCH[32];                         /* Offset: 0x0D0 ~ 0x14C (R/W) APS i-Bus patch                                                          */
          uint32_t RESERVED3[66];                           /* Offset: 0x150 ~ 0x254                                                                                */
    __IOM uint32_t APS_I_PATCH_EN;                          /* Offset: 0x258 (R/W) APS i-Bus patch eanble                                                           */
          uint32_t RESERVED4[2];                            /* Offset: 0x25C ~ 0x260                                                                                */
    __IOM uint32_t SPARE;                                   /* Offset: 0x264 (R/W)                                                                                  */
    __IOM uint32_t SPARE1;                                  /* Offset: 0x268 (R/W)                                                                                  */
    __IOM uint32_t SPARE2;                                  /* Offset: 0x26C (R/W)                                                                                  */
    __IOM uint32_t SPARE3;                                  /* Offset: 0x270 (R/W)                                                                                  */
    __IOM uint32_t SPARE4;                                  /* Offset: 0x274 (R/W)                                                                                  */
    __IOM uint32_t SPARE5;                                  /* Offset: 0x278 (R/W)                                                                                  */
    __IOM uint32_t SPARE6;                                  /* Offset: 0x27C (R/W)                                                                                  */
    __IOM uint32_t SPARE7;                                  /* Offset: 0x280 (R/W)                                                                                  */
    __IOM uint32_t SPARE8;                                  /* Offset: 0x284 (R/W)                                                                                  */
          uint32_t RESERVED5[70];                           /* Offset: 0x288 ~ 0x39C                                                                                */
    __IOM uint32_t MSQ_I_PATCH[64];                         /* Offset: 0x3A0 ~ 0x49C (R/W) MSQ i-Bus patch                                                          */
          uint32_t RESERVED6[3];                            /* Offset: 0x4A0 ~ 0x4A8                                                                                */
    __IOM uint32_t MSQ_I_PATCH_EN[2];                       /* Offset: 0x4AC ~ 0x4B0 (R/W) MSQ i-Bus patch enable                                                   */
          uint32_t RESERVED7[21];                           /* Offset: 0x4B4 ~ 0x504                                                                                */
    __IOM uint32_t ARB_ENABLE;                              /* Offset: 0x508 (R/W) Enable ARB                                                                       */
          uint32_t RESERVED8[61];                           /* Offset: 0x50C ~ 0x5FC                                                                                */
    __IOM uint32_t ABR_CTRL;                                /* Offset: 0x600 (R/W)                                                                                  */
    __IM  uint32_t ABR_STATUS;                              /* Offset: 0x604 (R/ )                                                                                  */
    __IM  uint32_t ABR_P_MIN_T1;                            /* Offset: 0x608 (R/ )                                                                                  */
    __IM  uint32_t ABR_P_MIN_T2;                            /* Offset: 0x60C (R/ )                                                                                  */
    __IM  uint32_t ABR_N_MIN_T1;                            /* Offset: 0x610 (R/ )                                                                                  */
    __IM  uint32_t ABR_N_MIN_T2;                            /* Offset: 0x614 (R/ )                                                                                  */
    __IM  uint32_t ABR_N_MAX_T1;                            /* Offset: 0x618 (R/ )                                                                                  */
    __IM  uint32_t ABR_N_MAX_T2;                            /* Offset: 0x61C (R/ )                                                                                  */
    __IOM uint32_t ABR_2_CTRL;                              /* Offset: 0x620 (R/W)                                                                                  */
    __IM  uint32_t ABR_2_T0;                                /* Offset: 0x624 (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T1;                                /* Offset: 0x628 (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T2;                                /* Offset: 0x62C (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T3;                                /* Offset: 0x630 (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T4;                                /* Offset: 0x634 (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T5;                                /* Offset: 0x638 (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T6;                                /* Offset: 0x63C (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T7;                                /* Offset: 0x640 (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T8;                                /* Offset: 0x644 (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T9;                                /* Offset: 0x648 (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T10;                               /* Offset: 0x64C (R/ )                                                                                  */
    __IM  uint32_t ABR_2_T11;                               /* Offset: 0x650 (R/ )                                                                                  */
    __IOM uint32_t XIP_CACHE_CONTROL;                       /* Offset: 0x654 (R/W)                                                                                  */
    __IM  uint32_t XIP_CACHE_HIT;                           /* Offset: 0x658 (R/ )                                                                                  */
    __IM  uint32_t XIP_CACHE_MISS;                          /* Offset: 0x65C (R/ )                                                                                  */
    __IM  uint32_t XIP_PREFETCH_HIT;                        /* Offset: 0x660 (R/ )                                                                                  */
    __IM  uint32_t XIP_PREFETCH_MISS;                       /* Offset: 0x664 (R/ )                                                                                  */
    __IM  uint32_t XIP_CACHE_HSEL_COUNT;                    /* Offset: 0x668 (R/ )                                                                                  */
    __IM  uint32_t XIP_CACHE_HSEL_WAIT_COUNT;               /* Offset: 0x66C (R/ )                                                                                  */
} S_SYS_Reg_t_patch;

#pragma pop

/** @}
  * //Peripheral_registers_structures
  */


/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */
  
/** @addtogroup Peripheral_declaration_patch
  * @{
  */
#undef SYS
#define SYS                     ((S_SYS_Reg_t_patch *)SYS_BASE)
/** @}
  * //Peripheral_declaration_patch
  */
  
  
/** @addtogroup Peripheral_Registers_Bits_Definition_patch
  * @{
  */
/******************************************************************************/
/*                         Peripheral Registers_Bits_Definition               */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                       System control register (SYS)                        */
/*                                                                            */
/******************************************************************************/


/**********  Bit definition for SYS_UART_SCLK_FON (0x04C) register  ***********/
#define SYS_UART_SCLK_FON_UART0_CLK_FON_Pos                                     (0U)
#define SYS_UART_SCLK_FON_UART0_CLK_FON_Msk                                     (0x00000001U << SYS_UART_SCLK_FON_UART0_CLK_FON_Pos)                                      /*!< 0x00000001 */
#define SYS_UART_SCLK_FON_UART0_CLK_FON                                         SYS_UART_SCLK_FON_UART0_CLK_FON_Msk                                                       /*!< 1: force uart0 sclk hw on/off control to always on 0: use hw control */
#define SYS_UART_SCLK_FON_UART1_CLK_FON_Pos                                     (1U)
#define SYS_UART_SCLK_FON_UART1_CLK_FON_Msk                                     (0x00000001U << SYS_UART_SCLK_FON_UART1_CLK_FON_Pos)                                      /*!< 0x00000002 */
#define SYS_UART_SCLK_FON_UART1_CLK_FON                                         SYS_UART_SCLK_FON_UART1_CLK_FON_Msk                                                       /*!< 1: force uart1 sclk hw on/off control to always on 0: use hw control */
#define SYS_UART_SCLK_FON_APS_UART_CLK_FON_Pos                                  (2U)
#define SYS_UART_SCLK_FON_APS_UART_CLK_FON_Msk                                  (0x00000001U << SYS_UART_SCLK_FON_APS_UART_CLK_FON_Pos)                                   /*!< 0x00000004 */
#define SYS_UART_SCLK_FON_APS_UART_CLK_FON                                      SYS_UART_SCLK_FON_APS_UART_CLK_FON_Msk                                                    /*!< 1: force aps uart sclk hw on/off control to always on 0: use hw control */
#define SYS_UART_SCLK_FON_MSQ_UART_CLK_FON_Pos                                  (3U)
#define SYS_UART_SCLK_FON_MSQ_UART_CLK_FON_Msk                                  (0x00000001U << SYS_UART_SCLK_FON_MSQ_UART_CLK_FON_Pos)                                   /*!< 0x00000008 */
#define SYS_UART_SCLK_FON_MSQ_UART_CLK_FON                                      SYS_UART_SCLK_FON_MSQ_UART_CLK_FON_Msk                                                    /*!< 1: force m0 uart sclk hw on/off control to always on 0: use hw control */
#define SYS_UART_SCLK_FON_SPARE_HW_Pos                                          (4U)
#define SYS_UART_SCLK_FON_SPARE_HW_Msk                                          (0x0FFFFFFFU << SYS_UART_SCLK_FON_SPARE_HW_Pos)                                           /*!< 0xFFFFFFF0 */
#define SYS_UART_SCLK_FON_SPARE_HW                                              SYS_UART_SCLK_FON_SPARE_HW_Msk                                                            /*!< Spare registers for HW use */




/** @}
  * //Peripheral_Registers_Bits_Definition_patch
  */
  
  
/** @}
  * //opl2500
  */

/** @}
  * //CMSIS
  */

  
#ifdef __cplusplus
}
#endif

#endif  /* _OPL2500_PATCH_H_ */
