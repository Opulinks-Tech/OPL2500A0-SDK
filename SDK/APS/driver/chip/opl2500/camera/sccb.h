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
 *  @file sccb.h 
 * 
 *  @brief SCCB interface
 *
 *  
 ******************************************************************************/

#ifndef _SCCB_H_
#define _SCCB_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_i2c.h"


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


typedef uint8_t (*T_SCCB_WriteRegs)(const uint8_t *pu8aRegPairss, uint32_t u32NumPairs);
typedef uint8_t (*T_SCCB_Read)(uint8_t u8Reg, uint8_t *pu8Value);
typedef uint8_t (*T_SCCB_Write)(uint8_t u8Reg, uint8_t u8Value);
typedef void (*T_SCCB_Init)(uint8_t u8SlaveAddr, E_I2cSpeed_t eSpeed, uint32_t u32WriteRegsDelayMs);
typedef void (*T_SCCB_DeInit)(void);


/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */


extern T_SCCB_WriteRegs SCCB_WriteRegs;
extern T_SCCB_Read SCCB_Read;
extern T_SCCB_Write SCCB_Write;
extern T_SCCB_Init SCCB_Init;
extern T_SCCB_DeInit SCCB_DeInit;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

uint8_t SCCB_WriteRegs_impl(const uint8_t *pu8aRegPairss, uint32_t u32NumPairs);
uint8_t SCCB_Read_impl(uint8_t u8Reg, uint8_t *pu8Value);
uint8_t SCCB_Write_impl(uint8_t u8Reg, uint8_t u8Value);
void SCCB_Init_impl(uint8_t u8SlaveAddr, E_I2cSpeed_t eSpeed, uint32_t u32WriteRegsDelayMs);
void SCCB_DeInit_impl(void);

#ifdef __cplusplus
}
#endif
#endif  /* _SCCB_H_ */
