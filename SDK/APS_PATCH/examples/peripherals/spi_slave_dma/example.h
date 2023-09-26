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

#define SPI_SLAVE_TX_ENABLE     1
#define SPI_SLAVE_RX_ENABLE     1
#define EN_DATA_CHECKING        (1 && SPI_SLAVE_RX_ENABLE)
#define CALC_BLK_CNT            10000       /* To calculate throughput block counts */

#if (!SPI_SLAVE_TX_ENABLE && !SPI_SLAVE_RX_ENABLE)
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
