/******************************************************************************
*  Copyright 2017 - 2021, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2021
 *******************************************************************************
 *
 *  @file example.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

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

/* SPI_BAUDRATE the bit rate in SPI_CLK pin
 *
 * Note: 
 *     OPL2500 SPI slave hardware SPI rate limitation
 *       RX-only mode: SPI pclk > 12 * SPI_BAUDRATE
 *       TRX mode: SPI pclk > 6 * SPI_BAUDRATE
 *
 *
 * Note:
 *     OPL2500 DMA is using system clock, please udpate system clock to higher than SPI data rate.
 *     But if the system clock is too high, maybe over 100MHz, it needs to set wait state for SRAM or it will get error when read or writing SRAM.
 *     Call Hal_Sys_SramDffBypass(0) to set wait state, i.e. disable SRAM DFF bypass.
 */
#define SPI_BAUDRATE            13333333    /* TRX mode, slave minimum spi bit rate = 160M/12 */
#define SPI_RX_SAMPLE_DELAY     2       /* Related to SPI rate and physical wire length */
#define SPI_MASTER_TX_ENABLE    1
#define SPI_MASTER_RX_ENABLE    1
#define EN_DATA_CHECKING        (1 & SPI_MASTER_RX_ENABLE)
#define CALC_BLK_CNT            10000       /* To calculate throughput block counts */

#if (!SPI_MASTER_TX_ENABLE && !SPI_MASTER_RX_ENABLE)
#error "Please enable either TX or RX or both of TRX."
#endif
/*************************************************************************************/
#define STRESS_TEST_MODE        0           /* 1: Stress test, not to calculate throughput.
                                             * 0: Calculate throughput */
#define STRESS_TEST_SHORT_REPORT_NUM    5000  /* When TRX how many blocks, report current state */

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
void example( void );

#ifdef __cplusplus
}
#endif
#endif  /* _EXAMPLE_H_ */
