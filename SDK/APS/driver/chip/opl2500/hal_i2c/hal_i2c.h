/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

/******************************************************************************
*  Filename:
*  ---------
*  hal_i2c.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of I2C .
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define I2C_DUMMY                0xFF
#define I2C_CLK_STD              100000
#define I2C_CLK_FAST             400000
#define I2C_CLK_HIGH             3400000
#define I2C_CLK_SUPPORT_MAX      I2C_CLK_FAST
#define I2C_DISABLE_TIMEOUT_TICK 10*( SystemCoreClockGet()/I2C_CLK_SUPPORT_MAX )
#define I2C_TIMEOUT_COUNT_MAX    (0x30000)

#define I2C_CON_SPEED_STD           (1 << I2C_CON_SPEED_Pos)
#define I2C_CON_SPEED_FAST          (2 << I2C_CON_SPEED_Pos)
#define I2C_CON_SPEED_HIGH          (3 << I2C_CON_SPEED_Pos)

#define I2C_CON_SLAVE_10BIT         (1 << I2C_CON_10BITADDR_SLAVE_Pos)
#define I2C_CON_MASTER_07BIT        (0 << I2C_CON_10BITADDR_MASTER_Pos)
#define I2C_CON_MASTER_10BIT        (1 << I2C_CON_10BITADDR_MASTER_Pos)


#define I2C_DATA_CMD_WRITE          (0 << I2C_DATA_CMD_CMD_Pos)
#define I2C_DATA_CMD_READ           (1 << I2C_DATA_CMD_CMD_Pos)
/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...
typedef enum 
{
    I2C_SPEED_STANDARD, /* 100K */
    I2C_SPEED_FAST      /* 400K */
    //I2C_SPEED_HIGH    /* 3.4M, not supported */
} E_I2cSpeed_t;

typedef enum 
{
    I2C_07BIT,
    I2C_10BIT
} E_I2cAddrMode_t;

typedef enum 
{
    I2C_TX_CMD_WRITE,
    I2C_TX_CMD_READ
} E_I2cTxCmd_t;

typedef enum 
{
    I2C_TX_RESTART_NO,
    I2C_TX_RESTART_NEED
} E_I2cTxRestart_t;

typedef enum 
{
    I2C_TX_STOP_BIT_NO,
    I2C_TX_STOP_BIT_NEED
} E_I2cTxStopBit_t;

typedef void (*T_Hal_I2c_DeInit)(void);
typedef E_I2cSpeed_t (*T_Hal_I2c_SpeedGet)(void);
typedef uint32_t (*T_Hal_I2c_SpeedSet)(E_I2cSpeed_t eSpeed);
typedef uint32_t (*T_Hal_I2c_UpdateClk)(void);
typedef void (*T_Hal_I2c_IntHandler)(void);

/* Master mode relative */
typedef uint32_t (*T_Hal_I2c_TargetAddrSet)(uint16_t u16TargetAddr);
typedef uint32_t (*T_Hal_I2c_WaitForMasterCompleted)(void);
typedef uint32_t (*T_Hal_I2c_MasterReceive)(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit);
typedef uint32_t (*T_Hal_I2c_MasterReceive_Ex)(uint8_t *pu8Data);
typedef uint32_t (*T_Hal_I2c_MasterTrasmit)(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit);
typedef uint32_t (*T_Hal_I2c_MasterTrasmit_Ex)(uint8_t *pu8Data, uint32_t u32Length, E_I2cTxCmd_t eCmd, E_I2cTxRestart_t eRestart, E_I2cTxStopBit_t eStopBit);
typedef uint32_t (*T_Hal_I2c_MasterInit)(E_I2cAddrMode_t eAddrMode, E_I2cSpeed_t eSpeed);

/* Slave mode relative */
typedef void (*T_Hal_I2c_SlaveIntTxCallBack)(void);
typedef void (*T_Hal_I2c_SlaveIntRxCallBack)(uint8_t u8Data);
typedef uint32_t (*T_Hal_I2c_SlaveReceive)(uint8_t *pu8Data, uint32_t u32Length);
typedef uint32_t (*T_Hal_I2c_SlaveTrasmit)(uint8_t *pu8Data, uint32_t u32Length);
typedef uint32_t (*T_Hal_I2c_SlaveInit)(E_I2cAddrMode_t eAddrMode, uint16_t u16SlaveAddr);
typedef uint32_t (*T_Hal_I2c_SlaveIntTxEn)(uint8_t u8Enable);
typedef uint32_t (*T_Hal_I2c_SlaveIntRxEn)(uint8_t u8Enable);
typedef void (*T_Hal_I2c_SlaveIntTxCallBackFuncSet)(T_Hal_I2c_SlaveIntTxCallBack tFunc);
typedef void (*T_Hal_I2c_SlaveIntRxCallBackFuncSet)(T_Hal_I2c_SlaveIntRxCallBack tFunc);


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


/***********
C Functions
***********/
// Sec 8: C Functions
extern T_Hal_I2c_DeInit     Hal_I2c_DeInit;
extern T_Hal_I2c_SpeedGet   Hal_I2c_SpeedGet;
extern T_Hal_I2c_SpeedSet   Hal_I2c_SpeedSet;
extern T_Hal_I2c_UpdateClk  Hal_I2c_UpdateClk;
extern T_Hal_I2c_IntHandler Hal_I2c_IntHandler;

/* Master mode relative */
extern T_Hal_I2c_TargetAddrSet          Hal_I2c_TargetAddrSet;
extern T_Hal_I2c_WaitForMasterCompleted Hal_I2c_WaitForMasterCompleted;
extern T_Hal_I2c_MasterReceive          Hal_I2c_MasterReceive;
extern T_Hal_I2c_MasterReceive_Ex       Hal_I2c_MasterReceive_Ex;
extern T_Hal_I2c_MasterTrasmit          Hal_I2c_MasterTrasmit;
extern T_Hal_I2c_MasterTrasmit_Ex       Hal_I2c_MasterTrasmit_Ex;
extern T_Hal_I2c_MasterInit             Hal_I2c_MasterInit;

/* Slave mode relative */
extern T_Hal_I2c_SlaveReceive              Hal_I2c_SlaveReceive;
extern T_Hal_I2c_SlaveTrasmit              Hal_I2c_SlaveTrasmit;
extern T_Hal_I2c_SlaveInit                 Hal_I2c_SlaveInit;
extern T_Hal_I2c_SlaveIntTxEn              Hal_I2c_SlaveIntTxEn;
extern T_Hal_I2c_SlaveIntRxEn              Hal_I2c_SlaveIntRxEn;
extern T_Hal_I2c_SlaveIntTxCallBackFuncSet Hal_I2c_SlaveIntTxCallBackFuncSet;
extern T_Hal_I2c_SlaveIntRxCallBackFuncSet Hal_I2c_SlaveIntRxCallBackFuncSet;

/* Internal */;
uint32_t _Hal_I2c_Eanble_impl(uint8_t u8Enable);
/* Comom */
void Hal_I2c_DeInit_impl(void);
uint32_t Hal_I2c_WaitForMasterCompleted_impl(void);
E_I2cSpeed_t Hal_I2c_SpeedGet_impl(void);
uint32_t Hal_I2c_SpeedSet_impl(E_I2cSpeed_t eSpeed);
uint32_t Hal_I2c_UpdateClk_impl(void);
void Hal_I2c_IntHandler_impl(void);
/* Master mode relative */
uint32_t Hal_I2c_TargetAddrSet_impl(uint16_t u16TargetAddr);
uint32_t Hal_I2c_MasterTrasmit_impl(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit);
uint32_t Hal_I2c_MasterTrasmit_Ex_impl(uint8_t *pu8Data, uint32_t u32Length, E_I2cTxCmd_t eCmd, E_I2cTxRestart_t eRestart, E_I2cTxStopBit_t eStopBit);
uint32_t Hal_I2c_MasterReceive_impl(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit);
uint32_t Hal_I2c_MasterReceive_Ex_impl(uint8_t *pu8Data);
uint32_t Hal_I2c_MasterInit_impl(E_I2cAddrMode_t eAddrMode, E_I2cSpeed_t eSpeed);
/* Slave mode relative */
uint32_t Hal_I2c_SlaveTrasmit_impl(uint8_t *pu8Data, uint32_t u32Length);
uint32_t Hal_I2c_SlaveReceive_impl(uint8_t *pu8Data, uint32_t u32Length);
uint32_t Hal_I2c_SlaveInit_impl(E_I2cAddrMode_t eAddrMode, uint16_t u16SlaveAddr);
uint32_t Hal_I2c_SlaveIntTxEn_impl(uint8_t u8Enable);
uint32_t Hal_I2c_SlaveIntRxEn_impl(uint8_t u8Enable);
void Hal_I2c_SlaveIntTxCallBackFuncSet_impl(T_Hal_I2c_SlaveIntTxCallBack tFunc);
void Hal_I2c_SlaveIntRxCallBackFuncSet_impl(T_Hal_I2c_SlaveIntRxCallBack tFunc);
#endif
