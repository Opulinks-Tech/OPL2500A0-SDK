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

/******************************************************************************
*  Filename:
*  ---------
*  hal_dbg_uart.h
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  Re-direct to HAL_UART
*
*  Author:
*  -------
*  Chung-chun Wang
******************************************************************************/

#ifndef __HAL_DBG_UART_H__
#define __HAL_DBG_UART_H__

#include "hal_uart.h"


/* Re-direct Hal_DbgUart functions */
/* - Status */
#define HAL_DBG_UART_RX_TIMEOUT                             RESULT_FAIL
#define HAL_DBG_UART_RX_SUCCESS                             RESULT_SUCCESS
/* - Callback type */
#define T_DbgUart_RxCallBack                                T_Uart_RxCallBack
/* - Function */
#define Hal_DbgUart_RxIntEn(u8Enable)                       Hal_Uart_RxIntEn(UART_IDX_DBG, IRQ_PRIORITY_DBG_UART, u8Enable)
#define Hal_DbgUart_RxIntEnStatusGet()                      Hal_Uart_RxIntEnStatusGet(UART_IDX_DBG)
#define Hal_DbgUart_BaudRateSet(u32Baud)                    Hal_Uart_BaudRateSet(UART_IDX_DBG, u32Baud)
#define Hal_DbgUart_BaudRateGet()                           Hal_Uart_BaudRateGet(UART_IDX_DBG)
#define Hal_DbgUart_Init(u32Baud)                           Hal_Uart_Init(UART_IDX_DBG, u32Baud, DATA_BIT_8, PARITY_NONE, STOP_BIT_1, 0)
#define Hal_DbgUart_RxCallBackFuncSet(tFunc)                Hal_Uart_RxCallBackFuncSet(UART_IDX_DBG, tFunc)
#define Hal_DbgUart_DataSend(u32Data)                       Hal_Uart_DataSend(UART_IDX_DBG, u32Data)
#define Hal_DbgUart_DataRecv(pu32Data)                      Hal_Uart_DataRecv(UART_IDX_DBG, pu32Data)
#define Hal_DbgUart_DataSendTimeOut(u32Data, u32MilliSec)   Hal_Uart_DataSendTimeOut(UART_IDX_DBG, u32Data, u32MilliSec)
#define Hal_DbgUart_DataRecvTimeOut(pu32Data, u32MilliSec)  Hal_Uart_DataRecvTimeOut(UART_IDX_DBG, pu32Data, u32MilliSec)


#endif
