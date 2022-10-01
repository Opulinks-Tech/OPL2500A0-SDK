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
*  hal_uart.h
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of uart .
*
*  Author:
*  -------
*  Chung-chun Wang
******************************************************************************/

#ifndef __HAL_UART_H__
#define __HAL_UART_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "opulinks.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

/* The number of ABR detection pulse. */
#define ABR_MONITOR_PULSE           9

#define ABR_MIN_BAUDRATE            600
#define ABR_MAX_BAUDRATE            230400
#define XTAL_MIN                    12000000
#define XTAL_MAX                    26000000

#define ABR_CNT_MIN                 ((uint32_t)( ((float)XTAL_MIN/ABR_MAX_BAUDRATE) * 0.9F ))
#define ABR_CNT_MAX                 ((uint32_t)( ((float)XTAL_MAX/ABR_MIN_BAUDRATE) * 1.1F ))

#define ABR_Tx_CNT_Pos              (0U)
#define ABR_Tx_CNT_Msk              (0x0000FFFFU << ABR_Tx_CNT_Pos) 
#define ABR_Tx_ORIENT_Pos           (16U)
#define ABR_Tx_ORIENT_Msk           (0x00000001U << ABR_Tx_ORIENT_Pos)
#define ABR_TX_ORIENT_POSTIVE       ABR_Tx_ORIENT_Msk
#define ABR_TX_ORIENT_NEGATIVE      0
#define ABR_Tx_VALID_Pos            (17U)
#define ABR_Tx_VALID_Msk            (0x00000001U << ABR_Tx_VALID_Pos)

#define ABR_PERIODx_CNT(t)          (((t) & ABR_Tx_CNT_Msk) >> ABR_Tx_CNT_Pos)
#define ABR_PERIODx_ORIENT(t)       (((t) & ABR_Tx_ORIENT_Msk) >> ABR_Tx_ORIENT_Pos)
#define ABR_PERIODx_VALID(t)        (((t) & ABR_Tx_VALID_Msk) >> ABR_Tx_VALID_Pos)



/*
 * baud rate:115200 -> 100 us per char, 
 * Max core 176Mhz -> 1ms = 176*1000 tick = 0x2AF80 => 0x30000
 */
#define UART_TIMEOUT_COUNT_MAX          ( 0x60000 )
#define UART_TIMEOUT_MS_MAX             ( Hal_Tick_MilliSecMax() )

#define HAL_UART_STS_ERROR              RESULT_FAIL
#define HAL_UART_STS_SUCCESS            RESULT_SUCCESS

#define UART_BAUD_DIV_MAX               0xFFFF
#define UART_BAUD_DIV_L_MASK            0xFF
#define UART_BAUD_DIV_H_MASK            0xFF

#define UART_DIV_FRACTION_BIT_NUM       4
#define UART_DIV_FRACTION_BIT_MSK       ((1 << UART_DIV_FRACTION_BIT_NUM) - 1)

/* ***** Register field definition **** */            
#define UART_DMA_MODE_0                 (0UL << UART_FCR_DMAM_Pos)
#define UART_DMA_MODE_1                 (1UL << UART_FCR_DMAM_Pos)

#define UART_TX_FIFO_TRIG_EMPTY         (0UL << UART_FCR_TET_Pos)
#define UART_TX_FIFO_TRIG_CHAR_2        (1UL << UART_FCR_TET_Pos)
#define UART_TX_FIFO_TRIG_QUARTER_FULL  (2UL << UART_FCR_TET_Pos)
#define UART_TX_FIFO_TRIG_HALF_FULL     (3UL << UART_FCR_TET_Pos)

#define UART_RX_FIFO_TRIG_CHAR_1        (0UL << UART_FCR_RT_Pos)
#define UART_RX_FIFO_TRIG_QUARTER_FULL  (1UL << UART_FCR_RT_Pos)
#define UART_RX_FIFO_TRIG_HALF_FULL     (2UL << UART_FCR_RT_Pos)
#define UART_RX_FIFO_TRIG_2_LESS_FULL   (3UL << UART_FCR_RT_Pos)

#define UART_DATA_LEN_5BITS             (0UL << UART_LCR_DLS_Pos)
#define UART_DATA_LEN_6BITS             (1UL << UART_LCR_DLS_Pos)
#define UART_DATA_LEN_7BITS             (2UL << UART_LCR_DLS_Pos)
#define UART_DATA_LEN_8BITS             (3UL << UART_LCR_DLS_Pos)
             
#define UART_STOP_BIT_1_BIT             (0UL << UART_LCR_STOP_Pos)
#define UART_STOP_BIT_1p5_OR_2_BIT      (1UL << UART_LCR_STOP_Pos)

#define UART_PARITY_NONE                (0UL << UART_LCR_PEN_Pos)
#define UART_PARITY_EVEN                (UART_LCR_PEN | (1UL << UART_LCR_EPS_Pos))
#define UART_PARITY_ODD                 (UART_LCR_PEN | (0UL << UART_LCR_EPS_Pos))

#define UART_FIFO_DEPTH                 64

#define UART_FCR_CFG_NON_DMA            (UART_RX_FIFO_TRIG_CHAR_1 | UART_TX_FIFO_TRIG_HALF_FULL | UART_FCR_FIFOE)


/* Compatible for validation project */
#define DbgUart_RxCallBack              g_fpaUart_RxCallBackFuncs[UART_IDX_DBG] 
#define Uart_0_RxCallBack               g_fpaUart_RxCallBackFuncs[UART_IDX_0] 
#define Uart_1_RxCallBack               g_fpaUart_RxCallBackFuncs[UART_IDX_1]

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list..
typedef enum 
{
    PARITY_NONE = 0,
    PARITY_EVEN,
    PARITY_ODD
} E_UartParity_t;

typedef enum 
{
    STOP_BIT_1 = 0,
    STOP_BIT_1P5,
    STOP_BIT_2
} E_UartStopBit_t;

typedef enum 
{
    DATA_BIT_5 = 0,
    DATA_BIT_6,
    DATA_BIT_7,
    DATA_BIT_8
} E_UartDataBit_t;

typedef enum 
{
    UART_IDX_0 = 0,
    UART_IDX_1,
    UART_IDX_DBG,
    UART_IDX_MAX
} E_UartIdx_t;

typedef enum 
{
    UART_INT_MODEM_STAT   = 0x0,
    UART_INT_NONE         = 0x1,
    UART_INT_TX_EMPTY     = 0x2,
    UART_INT_RX_AVALIBLE  = 0x4,
    UART_INT_RECV_LINE    = 0x6,
    UART_INT_BUSY         = 0x7,
    UART_INT_CHAR_TIMEOUT = 0xC
} E_UartIntId_t;

typedef enum
{
    UART_DMA_MODE_SINGLE = UART_DMA_MODE_0,
    UART_DMA_MODE_MULTI = UART_DMA_MODE_1,
} E_UartDmaMode_t;

typedef enum
{
    UART_DMA_TX_LVL_EMPTY   = UART_TX_FIFO_TRIG_EMPTY,
    UART_DMA_TX_LVL_2       = UART_TX_FIFO_TRIG_CHAR_2,
    UART_DMA_TX_QUARTER     = UART_TX_FIFO_TRIG_QUARTER_FULL,
    UART_DMA_TX_HALF        = UART_TX_FIFO_TRIG_HALF_FULL
} E_UartDmaTxLevel_t;

typedef enum
{
    UART_DMA_RX_LVL_1       = UART_RX_FIFO_TRIG_CHAR_1,
    UART_DMA_RX_QUARTER     = UART_RX_FIFO_TRIG_QUARTER_FULL,
    UART_DMA_RX_HALF        = UART_RX_FIFO_TRIG_HALF_FULL,
    UART_DMA_RX_LESS_2      = UART_RX_FIFO_TRIG_2_LESS_FULL,
} E_UartDmaRxLevel_t;


typedef enum 
{
    ABR_SRC_SEL_UART0 = 0,
    ABR_SRC_SEL_UART1,
    ABR_SRC_SEL_APS_UART,
    ABR_SRC_SEL_MSQ_UART,
    ABR_SRC_SEL_MAX,
} E_UartAbrSrc_t;


typedef enum
{
    _UART_STATUS_REG_MSK            = 0x0F000000,
    _UART_STATUS_LSR                = 0x01000000,
    _UART_STATUS_USR                = 0x02000000,
    
    /* LSR */
    UART_STATUS_TX_EMPTY            = _UART_STATUS_LSR | UART_LSR_TEMT,
    UART_STATUS_RX_READY            = _UART_STATUS_LSR | UART_LSR_DR,
                                      
    /* USR */                         
    UART_STATUS_BUSY                = _UART_STATUS_USR | UART_USR_BUSY,
    UART_STATUS_TX_FIFO_NOT_FULL    = _UART_STATUS_USR | UART_USR_TFNF,
    UART_STATUS_TX_FIFO_EMPTY       = _UART_STATUS_USR | UART_USR_TFE,
    UART_STATUS_RX_FIFO_NOT_EMPTY   = _UART_STATUS_USR | UART_USR_RFNE,
    UART_STATUS_RX_FIFO_FULL        = _UART_STATUS_USR | UART_USR_RFF,
} E_UART_STATUS;




// the information of Uart config to be stored in flash
typedef struct
{
    uint32_t ulBuadrate;
    uint8_t ubDataBit;
    uint8_t ubStopBit;
    uint8_t ubParity;
    uint8_t ubFlowCtrl;
} T_HalUartConfig;


// the UART runtime setting
typedef struct
{
    uint8_t u8Enable;       /* UART is init or not */
    uint32_t u32ModemCtrl;  /* MCR */
    uint8_t u8DivIntLow;    /* DLL */
    uint8_t u8DivIntHigh;   /* DLH */
    uint8_t u8DivFrac;      /* DLF */
    uint32_t u32LineCtrl;   /* LCR */
    uint32_t u32FifoCtrl;   /* FCR */
    uint32_t u32IntEn;      /* IER */
} T_HalUartRtSetting;


typedef void (*T_Uart_RxCallBack)(uint32_t);

typedef uint32_t (*T_Hal_Uart_RxIntEn)(E_UartIdx_t eUartIdx, uint32_t u32IrqPriorty, uint8_t u8Enable);
typedef uint8_t  (*T_Hal_Uart_RxIntEnStatusGet)(E_UartIdx_t eUartIdx);
typedef uint32_t (*T_Hal_Uart_IntIdRead)(E_UartIdx_t eUartIdx);
typedef uint32_t (*T_Hal_Uart_IntClear)(E_UartIdx_t eUartIdx, E_UartIntId_t eIntID, uint32_t *pu32Buff);
typedef uint32_t (*T_Hal_Uart_BaudRateDetectEn)(E_UartAbrSrc_t eAbrUartSrc);
typedef uint32_t (*T_Hal_Uart_BaudRateDetectDone)(uint32_t *pu32Cnt, uint32_t u32MilliSec);
typedef uint32_t (*T_Hal_Uart_BaudRateSet)(E_UartIdx_t eUartIdx, uint32_t u32Baud);
typedef uint32_t (*T_Hal_Uart_BaudRateGet)(E_UartIdx_t eUartIdx);
typedef void (*T_Hal_Uart_ClockUpdate)(E_UartIdx_t eUartIdx);
typedef uint32_t (*T_Hal_Uart_Init)(E_UartIdx_t eUartIdx, uint32_t u32Baud, E_UartDataBit_t u8Bits, E_UartParity_t u8Parity, E_UartStopBit_t u8StopBit, uint8_t u8EnFlowCtl);
typedef void (*T_Hal_Uart_WakeupResume)(E_UartIdx_t eUartIdx);
typedef uint8_t  (*T_Hal_Uart_EnStatusGet)(E_UartIdx_t eUartIdx);
typedef uint8_t  (*T_Hal_Uart_StatusGet)(E_UartIdx_t eUartIdx, E_UART_STATUS eStatus);
typedef uint32_t (*T_Hal_Uart_ConfigGet)(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig);
typedef uint32_t (*T_Hal_Uart_ConfigSet)(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig);
typedef uint32_t (*T_Hal_Uart_DataSend)(E_UartIdx_t eUartIdx, uint32_t u32Data);
typedef uint32_t (*T_Hal_Uart_DataRecv)(E_UartIdx_t eUartIdx, uint32_t* pu32Data);
typedef void     (*T_Hal_Uart_RxCallBackFuncSet)(E_UartIdx_t eUartIdx, T_Uart_RxCallBack tFunc);
typedef uint8_t (*T_Hal_Uart_Halt)(E_UartIdx_t eUartIdx, uint8_t u8Halt, uint32_t *pu32PinBkp);
typedef uint8_t (*T_Hal_Uart_DmaConfig)(E_UartIdx_t eUartIdx, E_UartDmaMode_t eDmaMode, E_UartDmaTxLevel_t eTxLvl, E_UartDmaRxLevel_t eRxLvl);
typedef uint8_t (*T_Hal_Uart_ResetFifo)(E_UartIdx_t eUartIdx, uint8_t u8ResetTx, uint8_t u8ResetRx);

// Those function are used for general case, time-out with default value(1 ms)
typedef uint32_t (*T_Hal_Uart_DataSend)(E_UartIdx_t eUartIdx, uint32_t u32Data);
typedef uint32_t (*T_Hal_Uart_DataRecv)(E_UartIdx_t eUartIdx, uint32_t* pu32Data);

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
typedef uint32_t (*T_Hal_Uart_DataSendTimeOut)(E_UartIdx_t eUartIdx, uint32_t u32Data, uint32_t u32MilliSec);
typedef uint32_t (*T_Hal_Uart_DataRecvTimeOut)(E_UartIdx_t eUartIdx, uint32_t* pu32Data, uint32_t u32MilliSec);


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable

// Sec 5: declaration of global function prototype

/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable
extern const T_HalUartConfig g_ctHal_Uart_DefaultConfig;
// Sec 7: declaration of static function prototype

/***********
C Functions
***********/
// Sec 8: C Functions
extern T_Uart_RxCallBack                g_fpaUart_RxCallBackFuncs[UART_IDX_MAX];

extern T_Hal_Uart_RxIntEn               Hal_Uart_RxIntEn;
extern T_Hal_Uart_RxIntEnStatusGet      Hal_Uart_RxIntEnStatusGet;
extern T_Hal_Uart_IntIdRead             Hal_Uart_IntIdRead;
extern T_Hal_Uart_IntClear              Hal_Uart_IntClear;
extern T_Hal_Uart_BaudRateDetectEn      Hal_Uart_BaudRateDetectEn;
extern T_Hal_Uart_BaudRateDetectDone    Hal_Uart_BaudRateDetectDone;
extern T_Hal_Uart_BaudRateSet           Hal_Uart_BaudRateSet;
extern T_Hal_Uart_BaudRateGet           Hal_Uart_BaudRateGet;
extern T_Hal_Uart_ClockUpdate           Hal_Uart_ClockUpdate;
extern T_Hal_Uart_Init                  Hal_Uart_Init;
extern T_Hal_Uart_WakeupResume          Hal_Uart_WakeupResume;
extern T_Hal_Uart_EnStatusGet           Hal_Uart_EnStatusGet;
extern T_Hal_Uart_StatusGet             Hal_Uart_StatusGet;
extern T_Hal_Uart_ConfigGet             Hal_Uart_ConfigGet;
extern T_Hal_Uart_ConfigSet             Hal_Uart_ConfigSet;
extern T_Hal_Uart_RxCallBackFuncSet     Hal_Uart_RxCallBackFuncSet;
extern T_Hal_Uart_Halt                  Hal_Uart_Halt;
extern T_Hal_Uart_DmaConfig             Hal_Uart_DmaConfig;
extern T_Hal_Uart_ResetFifo             Hal_Uart_ResetFifo;

// Those function are used for general case, time-out with default value(1 ms)
extern T_Hal_Uart_DataSend              Hal_Uart_DataSend;
extern T_Hal_Uart_DataRecv              Hal_Uart_DataRecv;

// Those functions are recommand used only for bootROM and ISR, time-out with the given value
extern T_Hal_Uart_DataSendTimeOut       Hal_Uart_DataSendTimeOut;
extern T_Hal_Uart_DataRecvTimeOut       Hal_Uart_DataRecvTimeOut;

uint32_t Hal_Uart_RxIntEn_impl(E_UartIdx_t eUartIdx, uint32_t u32IrqPriorty, uint8_t u8Enable);
uint32_t Hal_Uart_IntIdRead_impl(E_UartIdx_t eUartIdx);
uint32_t Hal_Uart_IntClear_impl(E_UartIdx_t eUartIdx, E_UartIntId_t eIntID, uint32_t *pu32Buff);
uint32_t Hal_Uart_BaudRateDetectEn_impl(E_UartAbrSrc_t eAbrUartSrc);
uint32_t Hal_Uart_BaudRateDetectDone_impl(uint32_t *pu32Cnt, uint32_t u32MilliSec);
uint32_t Hal_Uart_BaudRateSet_impl(E_UartIdx_t eUartIdx, uint32_t u32Baud);
uint32_t Hal_Uart_BaudRateGet_impl(E_UartIdx_t eUartIdx);
void Hal_Uart_ClockUpdate_impl(E_UartIdx_t eUartIdx);
uint32_t Hal_Uart_Init_impl(E_UartIdx_t eUartIdx, uint32_t u32Baud, E_UartDataBit_t u8Bits, E_UartParity_t u8Parity, E_UartStopBit_t u8StopBit, uint8_t u8EnFlowCtl);
void Hal_Uart_WakeupResume_impl(E_UartIdx_t eUartIdx);
uint32_t Hal_Uart_ConfigGet_impl(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig);
uint32_t Hal_Uart_ConfigSet_impl(E_UartIdx_t eUartIdx, T_HalUartConfig *ptConfig);
uint32_t Hal_Uart_DataSendTimeOut_impl(E_UartIdx_t eUartIdx, uint32_t u32Data, uint32_t u32MilliSec);
uint32_t Hal_Uart_DataSend_impl(E_UartIdx_t eUartIdx, uint32_t u32Data);
uint32_t Hal_Uart_DataRecvTimeOut_impl(E_UartIdx_t eUartIdx, uint32_t* pu32Data, uint32_t u32MilliSec);
uint32_t Hal_Uart_DataRecv_impl(E_UartIdx_t eUartIdx, uint32_t* pu32Data);
void Hal_Uart_RxCallBackFuncSet_impl(E_UartIdx_t eUartIdx, T_Uart_RxCallBack tFunc);
uint8_t Hal_Uart_RxIntEnStatusGet_impl(E_UartIdx_t eUartIdx);
uint8_t Hal_Uart_EnStatusGet_impl(E_UartIdx_t eUartIdx);
uint8_t Hal_Uart_StatusGet_impl(E_UartIdx_t eUartIdx, E_UART_STATUS eStatus);
uint8_t Hal_Uart_Halt_impl(E_UartIdx_t eUartIdx, uint8_t u8Halt, uint32_t *pu32PinBkp);
uint8_t Hal_Uart_DmaConfig_impl(E_UartIdx_t eUartIdx, E_UartDmaMode_t eDmaMode, E_UartDmaTxLevel_t eTxLvl, E_UartDmaRxLevel_t eRxLvl);
uint8_t Hal_Uart_ResetFifo_impl(E_UartIdx_t eUartIdx, uint8_t u8ResetTx, uint8_t u8ResetRx);

#include "hal_uart_patch.h"
#endif

