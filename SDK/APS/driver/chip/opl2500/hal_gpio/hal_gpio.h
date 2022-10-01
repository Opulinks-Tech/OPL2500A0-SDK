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
*  hal_gpio.h
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of GPIO functions
*


## Sleep IO
### GPIO output
- As is
- User control can force GPIO output pin state

### GPIO input
- Disabled except wakeup pin

### Peripheral output
- Most are enabled by auto control. Refer to following table
- Using User control API can force the output disable or change level.
- e.g.
    - IO8 is APS_UART_TX in run mode
    - Auto control: Set Output high in sleep mode
    - If needed, using user control API to force IO8 disable output in sleep mode.
        ```c
        Hal_Gpio_SleepIoUsrCtrlSet(GPIO_IDX_08, SLEEP_OUTPUT_DISABLE);
        ```
### Peripheral input
- All disabled
  
  
##Sleep mode IO output auto control
  
|  Block      |  Pin    | I/O | Output enable  |   Level  |                   Note                  |
| :--------:  | :----:  | :-: | :------------: | :------: | :-------------------------------------- |
|  GPIO       | Input   |  I  |  Disable       |    x     |                                         |
|  GPIO       | Output  |  O  |  Keep setting  |   Keep   | Output setting is kept                  |
|  UART       |  TX     |  O  |  Enable        |   High   |                                         |
|  UART       |  RX     |  I  |  Disable       |    x     |                                         |
|  UART       |  RTS    |  O  |  Enable        |   High   |                                         |
|  UART       |  CTS    |  I  |  Disable       |    x     |                                         |
|  I2C        |  SDA    | I/O |  Enable        | **High** | User control the IO when in slave mode  |
|  I2C        |  SCK    | I/O |  Enable        | **High** | User control the IO when in slave mode  |
|  PWM        |  PWM    |  O  |  **Disable**   |    x     | By application                          |
|  SPI Master |  CS     |  O  |  Enable        |   High   |                                         |
|  SPI Master |  CLK    |  O  |  Enable        | **Low**  | User control when polarity not matched  |
|  SPI Master |  MOSI   |  O  |  Enable        |   High   |                                         |
|  SPI Master |  MISO   |  I  |  Disable       |    x     |                                         |
|  SPI Master |  IO0    |  O  |  Enable        |   High   |                                         |
|  SPI Master |  IO1    |  O  |  Enable        |   High   |                                         |
|  SPI Master |  IO2    |  O  |  Enable        |   High   | (WP for flash)                          |
|  SPI Master |  IO3    |  O  |  Enable        |   High   | (HOLD for flash)                        |
|  I2S        |  SDI    |  I  |  Disable       |    x     |                                         |
|  I2S        |  SCLK   |  O  |  Enable        |   Low    |                                         |
|  I2S        |  WS     |  O  |  Enable        |   High   |                                         |
|  I2S        |  SDO    |  O  |  Enable        |   High   |                                         |
|  ExtClk2IO  |  CLKOUT |  O  |  Enable        |   Low    |                                         |
|  ExtClk2IO  |  EN     |  I  |  Disable       |    x     |                                         |
|  PDM        |  RX     |  I  |  Disable       |    x     |                                         |
|  PDM        |  CLK    |  O  |  Enable        |   Low    |                                         |
|  SPI Slave  |  CS     |  I  |  Disable       |    x     |                                         |
|  SPI Slave  |  CLK    |  I  |  Disable       |    x     |                                         |
|  SPI Slave  |  RX     |  I  |  Disable       |    x     |                                         |
|  SPI Slave  |  TX     |  O  |  Enable        |   High   |                                         |
|  Others     |         |  x  |  Disable       |    x     |                                         |
  

******************************************************************************/

#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include <stdint.h>
#include <stddef.h>
#include "opulinks.h"
#include "hal_vic.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/* GPIO IO# pull UP resistor. When the bit is one, it means the IO is pulled up.
 * Otherwise, it is pulled down. */
#define GPIO_IO_PULL_UP_PAD                                     0x00001FF7FFCCDFB7ULL

/* GPIO SiP IO definition. SiP IO control is in different register */
#define GPIO_IO_SIP_DEF                                         0x00001F8000000000ULL

/* GPIO SiP IO start index. */
#define GPIO_SIP_START_IDX                                      GPIO_IDX_39




#define GPIO_SIP_IE_BIT_OFST(eIdx)                          ((eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_IE_PSRAM_39_44_Pos)
#define GPIO_SIP_PE_OFST(eIdx)                              ((eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_PE_PSRAM_39_44_Pos)
#define GPIO_SIP_O_INV_OFST(eIdx)                           ((eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_O_INV_PSRAM_39_44_Pos)
#define GPIO_SIP_DS_OFST(eIdx)                              ((eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_DS_PSRAM_39_44_Pos)
#define GPIO_SIP_GPO_OFST(eIdx)                             ((eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_GPO_PSRAM_39_44_Pos)
#define GPIO_SIP_I_INV_OFST(eIdx)                           ((eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_PD_I_INV_PSRAM_39_44_Pos)
#define GPIO_SIP_DIR_OFST(eIdx)                             ((eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_PD_DIR_PSRAM_39_44_Pos)
#define GPIO_SIP_OEN_INV_OFST(eIdx)                         ((eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_PD_OEN_INV_PSRAM_39_44_Pos)
#define GPIO_SIP_PDOC_OFST(eIdx)                            ((eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_3_PDOC_PSRAM_39_44_Pos)
#define GPIO_SIP_PDOV_OFST(eIdx)                            ((eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_3_PDOV_PSRAM_39_44_Pos)


#define GPIO_CTRL_SIP_IO_IE_SET(eIdx, u32Value)             REG_BB_WRITE(&PIN->PSRAM_CONTROL_1, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_IE_PSRAM_39_44_Pos,        (u32Value))
#define GPIO_CTRL_SIP_IO_PE_SET(eIdx, u32Value)             REG_BB_WRITE(&PIN->PSRAM_CONTROL_1, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_PE_PSRAM_39_44_Pos,        (u32Value))
#define GPIO_CTRL_SIP_IO_O_INV_SET(eIdx, u32Value)          REG_BB_WRITE(&PIN->PSRAM_CONTROL_1, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_O_INV_PSRAM_39_44_Pos,     (u32Value))
#define GPIO_CTRL_SIP_IO_DS_SET(eIdx, u32Value)             REG_BB_WRITE(&PIN->PSRAM_CONTROL_1, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_DS_PSRAM_39_44_Pos,        (u32Value))
#define GPIO_CTRL_SIP_IO_GPO_SET(eIdx, u32Value)            REG_BB_WRITE(&PIN->PSRAM_CONTROL_2, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_GPO_PSRAM_39_44_Pos,          (u32Value))
#define GPIO_CTRL_SIP_IO_I_INV_SET(eIdx, u32Value)          REG_BB_WRITE(&PIN->PSRAM_CONTROL_2, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_PD_I_INV_PSRAM_39_44_Pos,     (u32Value))
#define GPIO_CTRL_SIP_IO_DIR_SET(eIdx, u32Value)            REG_BB_WRITE(&PIN->PSRAM_CONTROL_2, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_PD_DIR_PSRAM_39_44_Pos,       (u32Value))
#define GPIO_CTRL_SIP_IO_OEN_INV_SET(eIdx, u32Value)        REG_BB_WRITE(&PIN->PSRAM_CONTROL_2, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_PD_OEN_INV_PSRAM_39_44_Pos,   (u32Value))
#define GPIO_CTRL_SIP_IO_PDOC_SET(eIdx, u32Value)           REG_BB_WRITE(&PIN->PSRAM_CONTROL_3, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_3_PDOC_PSRAM_39_44_Pos,         (u32Value))
#define GPIO_CTRL_SIP_IO_PDOV_SET(eIdx, u32Value)           REG_BB_WRITE(&PIN->PSRAM_CONTROL_2, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_3_PDOV_PSRAM_39_44_Pos,         (u32Value))


#define GPIO_CTRL_SIP_IO_IE_GET(eIdx, u32Value)             REG_BB_READ(&PIN->PSRAM_CONTROL_1, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_IE_PSRAM_39_44_Pos,        (u32Value))
#define GPIO_CTRL_SIP_IO_PE_GET(eIdx, u32Value)             REG_BB_READ(&PIN->PSRAM_CONTROL_1, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_PE_PSRAM_39_44_Pos,        (u32Value))
#define GPIO_CTRL_SIP_IO_O_INV_GET(eIdx, u32Value)          REG_BB_READ(&PIN->PSRAM_CONTROL_1, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_O_INV_PSRAM_39_44_Pos,     (u32Value))
#define GPIO_CTRL_SIP_IO_DS_GET(eIdx, u32Value)             REG_BB_READ(&PIN->PSRAM_CONTROL_1, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_1_PD_DS_PSRAM_39_44_Pos,        (u32Value))
#define GPIO_CTRL_SIP_IO_GPO_GET(eIdx, u32Value)            REG_BB_READ(&PIN->PSRAM_CONTROL_2, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_GPO_PSRAM_39_44_Pos,          (u32Value))
#define GPIO_CTRL_SIP_IO_I_INV_GET(eIdx, u32Value)          REG_BB_READ(&PIN->PSRAM_CONTROL_2, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_PD_I_INV_PSRAM_39_44_Pos,     (u32Value))
#define GPIO_CTRL_SIP_IO_DIR_GET(eIdx, u32Value)            REG_BB_READ(&PIN->PSRAM_CONTROL_2, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_PD_DIR_PSRAM_39_44_Pos,       (u32Value))
#define GPIO_CTRL_SIP_IO_OEN_INV_GET(eIdx, u32Value)        REG_BB_READ(&PIN->PSRAM_CONTROL_2, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_2_PD_OEN_INV_PSRAM_39_44_Pos,   (u32Value))
#define GPIO_CTRL_SIP_IO_PDOC_GET(eIdx, u32Value)           REG_BB_READ(&PIN->PSRAM_CONTROL_3, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_3_PDOC_PSRAM_39_44_Pos,         (u32Value))
#define GPIO_CTRL_SIP_IO_PDOV_GET(eIdx, u32Value)           REG_BB_READ(&PIN->PSRAM_CONTROL_2, (eIdx) - GPIO_SIP_START_IDX + PIN_PSRAM_CONTROL_3_PDOV_PSRAM_39_44_Pos,         (u32Value))


#define GPIO_REG_BITS_MSK                           ((uint64_t)((1ULL << GPIO_SIP_START_IDX) - 1))
#define GPIO_SIP_REG_BITS_MSK                       ((uint32_t)(GPIO_IO_SIP_DEF >> GPIO_SIP_START_IDX))

#define SLEEP_CFG_OUT_EN_BIT_SHIFT      (offsetof(S_SLEEP_CFG_DEF, u8OutEn)*8)
#define SLEEP_CFG_OUT_LVL_BIT_SHIFT     (offsetof(S_SLEEP_CFG_DEF, u8OutLvl)*8)
#define SLEEP_CFG_IO_CTRL_BIT_SHIFT     (offsetof(S_SLEEP_CFG_DEF, u8IoCtl)*8)
    

#define GPIO_INT_TYPE1_IO_START                     GPIO_IDX_16
#define GPIO_INT_TYPE2_IO_START                     GPIO_IDX_32
#define GPIO_CTRL_REG_HIGH_START                    GPIO_IDX_32

#define GPIO_IO_IDX_TO_BIT_IDX(io)                  ((io) & 0x1F)   /* Bit 0 ~ 31 */
#define GPIO_IO_IDX_TO_TYPE_BIT_IDX(io)             (((io) * 2) & 0x1F)
#define GPIO_SET_INT_TYPE_BITS(reg, bit, value)     ((reg) = ((reg) & ~(3UL << (bit))) | ((value) & 3UL) << (bit))

#define Hal_Vic_GpioIntEn                           Hal_Gpio_IntEn
#define Hal_Vic_GpioIntClear                        Hal_Gpio_IntClear
#define Hal_Vic_GpioIntMask                         Hal_Gpio_IntMask
#define Hal_Vic_GpioIntTypeSel                      Hal_Gpio_IntTypeSel
#define Hal_Vic_GpioIntInv                          Hal_Gpio_IntInv
#define Hal_Vic_GpioIntStatRead                     Hal_Gpio_IntStatRead
#define Hal_Vic_GpioCallBackFuncSet                 Hal_Gpio_CallBackFuncSet
#define Hal_Vic_GpioInit                            Hal_Gpio_VicInit
#define Hal_Vic_GpioInput                           Hal_Gpio_Input
#define Hal_Vic_GpioOutput                          Hal_Gpio_Output
#define Hal_Vic_GpioDirection                       Hal_Gpio_Direction
#define Hal_Vic_GpioPinmux                          Hal_Gpio_Pinmux
#define Hal_Vic_MmFactorResumeByGpioIntEn           Hal_Gpio_MmFactorResumeByGpioIntEn
#define Hal_Vic_MmFactorResumeByGpioIntStatRead     Hal_Gpio_MmFactorResumeByGpioIntStatRead

/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

#pragma push
#pragma anon_unions
typedef struct
{
    union
    {
        uint32_t u32Value;
        struct
        {
            uint8_t u8OutEn;
            uint8_t u8OutLvl;
            uint8_t u8IoCtl;
            uint8_t u8Reserved;
        };
    };
} S_SLEEP_CFG_DEF;
#pragma pop


typedef struct
{
    uint64_t u64UsrCtrl;        /* Output control by User. If the bit is set to one, the IO setting will use oen_user and level_user.
                                   Otherwise, user oen_auto and level_auto. */
    uint64_t u64UsrOutEn;       /* Output enable. Control by user, priority is higher than oen_auto. (1: Enable, 0:Disable) */
    uint64_t u64UsrOutLvl;      /* Otuput level. Control by user, priority is higher than level_auto. (1: High, 0: Low) */
    uint64_t u64AutoOutEn;      /* Output enable. Auto check by PIN and GPIO API. (1: Enable, 0:Disable) */
    uint64_t u64AutoOutLvl;     /* Output level. Auto check by PIN and GPIO API. (1: High, 0: Low) */
} S_SLEEP_IO_CFG;


typedef struct
{
    uint32_t PD_IE[2];
    uint32_t PD_PE[2];
    uint32_t PD_O_INV[2];
    uint32_t PD_DS[2];
    uint32_t PD_I_INV[2];
    uint32_t PDOV_MODE[2];
    uint32_t PD_DIR[2];
    uint32_t PD_OEN_INV[2];
    uint32_t PDOC_MODE[2];
    uint32_t GPO_ST[2];
    uint32_t PSRAM_CONTROL_1;
    uint32_t PSRAM_CONTROL_2;
    uint32_t PSRAM_CONTROL_3;
} S_PIN_MUX_CACHE;


typedef enum 
{
    GPIO_IDX_00 = 0x00,
    GPIO_IDX_01,
    GPIO_IDX_02,
    GPIO_IDX_03,
    GPIO_IDX_04,
    GPIO_IDX_05,
    GPIO_IDX_06,
    GPIO_IDX_07,
    GPIO_IDX_08,
    GPIO_IDX_09,
    GPIO_IDX_10,
    GPIO_IDX_11,
    GPIO_IDX_12,
    GPIO_IDX_13,
    GPIO_IDX_14,
    GPIO_IDX_15,
    GPIO_IDX_16,
    GPIO_IDX_17,
    GPIO_IDX_18,
    GPIO_IDX_19,
    GPIO_IDX_20,
    GPIO_IDX_21,
    GPIO_IDX_22,
    GPIO_IDX_23,
    GPIO_IDX_24,
    GPIO_IDX_25,
    GPIO_IDX_26,
    GPIO_IDX_27,
    GPIO_IDX_28,
    GPIO_IDX_29,
    GPIO_IDX_30,
    GPIO_IDX_31,
    GPIO_IDX_32,
    GPIO_IDX_33,
    GPIO_IDX_34,
    GPIO_IDX_35,
    GPIO_IDX_36,
    GPIO_IDX_37,
    GPIO_IDX_38,
    GPIO_IDX_39,
    GPIO_IDX_40,
    GPIO_IDX_41,
    GPIO_IDX_42,
    GPIO_IDX_43,
    GPIO_IDX_44,
    GPIO_IDX_NUM,
    GPIO_IDX_INVALID = 0xFF,
} E_GpioIdx_t;

typedef enum
{
    GPIO_OUTPUT     = 0,
    GPIO_INPUT      = 1
} E_GpioDirection_t;

typedef enum
{
    GPIO_LEVEL_LOW  = 0,
    GPIO_LEVEL_HIGH = 1,
    GPIO_LEVEL_FLOATING = 2,    /* Only for input */
} E_GpioLevel_t;


typedef enum
{
    SLEEP_IO_FAIL = 0,
    SLEEP_IO_SUCCESS,
} E_SLEEP_IO_STATUS;


typedef enum 
{
    SLEEP_USER_CTRL_OFF         = 0,
    SLEEP_OUTPUT_DISABLE        = (1 << SLEEP_CFG_IO_CTRL_BIT_SHIFT) | (0 << SLEEP_CFG_OUT_EN_BIT_SHIFT),
    SLEEP_OUTPUT_ENABLE_HIGH    = (1 << SLEEP_CFG_IO_CTRL_BIT_SHIFT) | (1 << SLEEP_CFG_OUT_EN_BIT_SHIFT) | (1 << SLEEP_CFG_OUT_LVL_BIT_SHIFT),
    SLEEP_OUTPUT_ENABLE_LOW     = (1 << SLEEP_CFG_IO_CTRL_BIT_SHIFT) | (1 << SLEEP_CFG_OUT_EN_BIT_SHIFT) | (0 << SLEEP_CFG_OUT_LVL_BIT_SHIFT),
} E_SLEEP_IO_CFG;


// GPIO module
typedef void (*T_Gpio_CallBack)(E_GpioIdx_t eIdx);

typedef uint32_t (*T_Hal_Gpio_IntInit)(E_GpioIdx_t eIdx, E_VIC_INT_TRIG_TYPE eTrigType, T_Gpio_CallBack tFunc);
typedef uint32_t (*T_Hal_Gpio_IntDeInit)(E_GpioIdx_t eIdx);
typedef uint32_t (*T_Hal_Gpio_IntEn)(E_GpioIdx_t eIdx, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Gpio_IntClear)(E_GpioIdx_t eIdx);
typedef uint32_t (*T_Hal_Gpio_IntMask)(E_GpioIdx_t eIdx, uint8_t u8Enable);
typedef uint32_t (*T_Hal_Gpio_IntTypeSel)(E_GpioIdx_t eIdx, E_ItrType_t eType);
typedef uint32_t (*T_Hal_Gpio_IntInv)(E_GpioIdx_t eIdx, uint8_t u8Enable);
typedef uint64_t (*T_Hal_Gpio_IntStatRead)(void);
typedef uint32_t (*T_Hal_Gpio_CallBackFuncSet)(E_GpioIdx_t eIdx, T_Gpio_CallBack tFunc);

typedef void (*T_Hal_Gpio_VicInit)(void);
typedef E_GpioLevel_t (*T_Hal_Gpio_Input)(E_GpioIdx_t eIdx);
typedef uint32_t (*T_Hal_Gpio_Output)(E_GpioIdx_t eIdx, E_GpioLevel_t tLevel);
typedef uint32_t (*T_Hal_Gpio_Direction)(E_GpioIdx_t eIdx, E_GpioDirection_t tDirection);
typedef uint32_t (*T_Hal_Gpio_Pinmux)(E_GpioIdx_t eIdx, E_GpioDirection_t eDirection, E_GpioLevel_t eLevel);

// MmFactor relative
typedef uint32_t (*T_Hal_Gpio_MmFactorResumeByGpioIntEn)(E_GpioIdx_t eIdx, uint8_t u8Enable);
typedef uint64_t (*T_Hal_Gpio_MmFactorResumeByGpioIntStatRead)(void);


typedef E_SLEEP_IO_STATUS (*T_Hal_Gpio_SleepIoUsrCtrlSet)(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG eSleepIoCfg);
typedef E_SLEEP_IO_STATUS (*T_Hal_Gpio_SleepIoUsrCtrlGet)(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG *peSleepIoCfg);
typedef E_SLEEP_IO_STATUS (*T_Hal_Gpio_SleepIoAutoCtrlSet)(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG eSleepIoCfg);
typedef E_SLEEP_IO_STATUS (*T_Hal_Gpio_SleepIoAutoCtrlGet)(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG *peSleepIoCfg);
typedef void (*T_Hal_Gpio_VicInit)(void);


typedef void (*T_Hal_Gpio_StoreRunModePinmux)(void);
typedef void (*T_Hal_Gpio_RestoreRunModePinmux)(void);
typedef void (*T_Hal_Gpio_SleepIoApply)(void);


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
/** The MAX index(excluded) for current IO package */
extern uint32_t g_u32Hal_IoPkgNum;
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

extern T_Gpio_CallBack  g_taHalVicGpioCallBack[GPIO_IDX_NUM];

extern T_Hal_Gpio_IntInit           Hal_Gpio_IntInit;
extern T_Hal_Gpio_IntDeInit         Hal_Gpio_IntDeInit;
extern T_Hal_Gpio_IntEn             Hal_Gpio_IntEn;
extern T_Hal_Gpio_IntClear          Hal_Gpio_IntClear;
extern T_Hal_Gpio_IntMask           Hal_Gpio_IntMask;
extern T_Hal_Gpio_IntTypeSel        Hal_Gpio_IntTypeSel;
extern T_Hal_Gpio_IntInv            Hal_Gpio_IntInv;
extern T_Hal_Gpio_IntStatRead       Hal_Gpio_IntStatRead;
extern T_Hal_Gpio_CallBackFuncSet   Hal_Gpio_CallBackFuncSet;

extern T_Hal_Gpio_VicInit           Hal_Gpio_VicInit;
extern T_Hal_Gpio_Input             Hal_Gpio_Input;
extern T_Hal_Gpio_Output            Hal_Gpio_Output;
extern T_Hal_Gpio_Direction         Hal_Gpio_Direction;
extern T_Hal_Gpio_Pinmux            Hal_Gpio_Pinmux;

// MmFactor relative
extern T_Hal_Gpio_MmFactorResumeByGpioIntEn       Hal_Gpio_MmFactorResumeByGpioIntEn;
extern T_Hal_Gpio_MmFactorResumeByGpioIntStatRead Hal_Gpio_MmFactorResumeByGpioIntStatRead;

/**
 * @brief Set user control pin setting in sleep mode
 *
 *        In OPL2500, if the IO is input by GPIO or peripheral, it will be disable in sleep mode.
 *        If the IO is GPIO output, it will keep output setting in sleep mode.
 *        If the IO is peripheral output, it might be enable high/low or disable according to the peripheral.
 *        Besides the auto configuration, uesr can use this function to assign IO pin as output high/low or disable in sleep mode.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param eSleepIoCfg [in] To configure the pin setting in sleep mode
 *          SLEEP_USER_CTRL_OFF: Using auto control.
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Configuration status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
extern T_Hal_Gpio_SleepIoUsrCtrlSet Hal_Gpio_SleepIoUsrCtrlSet;

/**
 * @brief Get user control pin setting in sleep mode
 *
 *        In OPL2500, if the IO is input by GPIO or peripheral, it will be disable in sleep mode.
 *        If the IO is GPIO output, it will keep output setting in sleep mode.
 *        If the IO is peripheral output, it might be enable high/low or disable according to the peripheral.
 *        Besides the auto configuration, uesr can use this function to assign IO pin as output high/low or disable in sleep mode.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param peSleepIoCfg [out] Read pin user control setting in sleep mode
 *          SLEEP_USER_CTRL_OFF: Using auto control.
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Read control status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
extern T_Hal_Gpio_SleepIoUsrCtrlGet Hal_Gpio_SleepIoUsrCtrlGet;

/**
 * @brief Set auto control pin setting in sleep mode
 * 
 * @warning Only called by Hal driver. Don't call this function in user layer.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param peSleepIoCfg [out] Read pin user control setting in sleep mode
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Read control status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
extern T_Hal_Gpio_SleepIoAutoCtrlSet Hal_Gpio_SleepIoAutoCtrlSet;

/**
 * @brief Read auto control pin setting in sleep mode
 *
 *        In OPL2500, if the IO is input by GPIO or peripheral, it will be disable in sleep mode.
 *        If the IO is GPIO output, it will keep output setting in sleep mode.
 *        If the IO is peripheral output, it might be enable high/low or disable according to the peripheral.
 *        Besides the auto configuration, uesr can use this function to assign IO pin as output high/low or disable in sleep mode.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param peSleepIoCfg [out] Read pin user control setting in sleep mode
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Read control status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
extern T_Hal_Gpio_SleepIoAutoCtrlGet Hal_Gpio_SleepIoAutoCtrlGet;

/** Intiail GPIO sleep configuration */
extern T_Hal_Gpio_VicInit                 Hal_Gpio_VicInit;


extern T_Hal_Gpio_StoreRunModePinmux Hal_Gpio_StoreRunModePinmux;
extern T_Hal_Gpio_RestoreRunModePinmux Hal_Gpio_RestoreRunModePinmux;
extern T_Hal_Gpio_SleepIoApply Hal_Gpio_SleepIoApply;


uint32_t Hal_Gpio_IntInit_impl(E_GpioIdx_t eIdx, E_VIC_INT_TRIG_TYPE eTrigType, T_Gpio_CallBack tFunc);
uint32_t Hal_Gpio_IntDeInit_impl(E_GpioIdx_t eIdx);
uint32_t Hal_Gpio_IntEn_impl(E_GpioIdx_t eIdx, uint8_t u8Enable);
uint32_t Hal_Gpio_IntClear_impl(E_GpioIdx_t eIdx);
uint32_t Hal_Gpio_IntMask_impl(E_GpioIdx_t eIdx, uint8_t u8Enable);
uint32_t Hal_Gpio_IntTypeSel_impl(E_GpioIdx_t eIdx, E_ItrType_t eType);
uint32_t Hal_Gpio_IntInv_impl(E_GpioIdx_t eIdx, uint8_t u8Enable);
uint64_t Hal_Gpio_IntStatRead_impl(void);
uint32_t Hal_Gpio_CallBackFuncSet_impl(E_GpioIdx_t eIdx, T_Gpio_CallBack tFunc);
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoUsrCtrlSet_impl(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG eSleepIoCfg);
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoUsrCtrlGet_impl(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG *peSleepIoCfg);
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoAutoCtrlSet_impl(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG eSleepIoCfg);
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoAutoCtrlGet_impl(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG *peSleepIoCfg);
void Hal_Gpio_VicInit_impl(void);
E_GpioLevel_t Hal_Gpio_Input_impl(E_GpioIdx_t eIdx);
uint32_t Hal_Gpio_Output_impl(E_GpioIdx_t eIdx, E_GpioLevel_t tLevel);
uint32_t Hal_Gpio_Direction_impl(E_GpioIdx_t eIdx, E_GpioDirection_t eDirection);
uint32_t Hal_Gpio_Pinmux_impl(E_GpioIdx_t eIdx, E_GpioDirection_t eDirection, E_GpioLevel_t eLevel);
uint32_t Hal_Gpio_MmFactorResumeByGpioIntEn_impl(E_GpioIdx_t eIdx, uint8_t u8Enable);
uint64_t Hal_Gpio_MmFactorResumeByGpioIntStatRead_impl(void);

void Hal_Gpio_StoreRunModePinmux_impl(void);
void Hal_Gpio_RestoreRunModePinmux_impl(void);
void Hal_Gpio_SleepIoApply_impl(void);
void Hal_Gpio_PreInit(uint32_t u32SleepIoAddr);

#endif  /* __HAL_GPIO_H__ */
