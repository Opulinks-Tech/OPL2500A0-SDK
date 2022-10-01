/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
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
*  sys_common.h
*
*  Project:
*  --------
*  OPL2500 Project - the system common definition file
*
*  Description:
*  ------------
*  This include file is the system common definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _SYS_COMMON_H_
#define _SYS_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdint.h>
#include "msg.h"
#include "section.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// retention region -> Removed
#define RET_DATA 

// memory read/write
#define MEM_READ_DW(addr)		(*(volatile uint32_t *)(addr))
#define MEM_WRITE_DW(addr, val)	(*(volatile uint32_t *)(addr) = (val))

// reg read/write
#define reg_read(addr)          MEM_READ_DW(addr)
#define reg_write(addr, val)    MEM_WRITE_DW(addr, val)

/* Math */
#define DIV_ROUND(D, d)         ( ((D) + (d/2)  )/(d) )              /* D / d */
#define DIV_CEILING(D, d)       ( ((D) + (d) - 1)/(d) )              /* D / d */


/* Bit band access. Only support 0x400XXXXX */
#define BITBAND_ADDR(addr, bitnum)      ( ((addr) & 0xF0000000) + 0x2000000 + (((addr) & 0xFFFFF) << 5) + ((bitnum) << 2) )
#define REG_BB_READ(addr, bitnum)       ( reg_read(BITBAND_ADDR((uint32_t)(addr), (uint32_t)(bitnum))) )
#define REG_BB_WRITE(addr, bitnum, val) ( reg_write(BITBAND_ADDR((uint32_t)(addr), (uint32_t)(bitnum)), ((uint32_t)(val) & 1)) )


#define BIT_GET(_WORD_, _IDX_)          ((_WORD_) >> (_IDX_) &1)
#define BIT_SET(_WORD_, _IDX_, _BIT_)   (_WORD_) = ((_WORD_) & ~(1 << (_IDX_)) | ((_BIT_) << (_IDX_)))

#define SYSYEM_TICK_PER_US       ( SystemCoreClockGet()/1000000 )
#define US_TO_NS                 1000

#define IRQ_PRIORITY_IPC0       0x04        /* will be left shift 4 bits */
#define IRQ_PRIORITY_IPC1       0x04
#define IRQ_PRIORITY_IPC2       0x04
#define IRQ_PRIORITY_IPC3       0x0A
#define IRQ_PRIORITY_GPIO       0x0A
#define IRQ_PRIORITY_SCRT       0x0A
#define IRQ_PRIORITY_MSQ        0x0A
#define IRQ_PRIORITY_UART       0x0A
#define IRQ_PRIORITY_DBG_UART   0x0A
#define IRQ_PRIORITY_UART0      0x0A
#define IRQ_PRIORITY_UART1      0x0A
#define IRQ_PRIORITY_I2C        0x0A
#define IRQ_PRIORITY_SPI0       0x0A
#define IRQ_PRIORITY_SPI1       0x0A
#define IRQ_PRIORITY_SPI2       0x0A
#define IRQ_PRIORITY_SPI3       0x0A
#define IRQ_PRIORITY_TMR0       0x0A
#define IRQ_PRIORITY_TMR1       0x0A
#define IRQ_PRIORITY_WDT        0x0A
#define IRQ_PRIORITY_JTAG       0x0A
#define IRQ_PRIORITY_DMA        0x0A

#define IRQ_PRIORITY_SYS        0x10         /* directly value, it is used for critical section in FreeRTOS */
#define IRQ_PRIORITY_TICK       0xF0         /* directly value, it is used for sys tick in FreeRTOS */



/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif
#ifdef OPL2500_PATCH
#include "sys_common_patch.h"
#endif /* OPL2500_PATCH */
#endif // _SYS_COMMON_H_
