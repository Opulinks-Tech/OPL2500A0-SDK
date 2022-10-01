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

#ifndef _WTS_DUT_SERIAL_H_
#define _WTS_DUT_SERIAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
typedef struct {
    uint32_t write_index;
    uint32_t read_index;
    uint32_t num;
    uint32_t mask;
    uint8_t *pbuf;
} wts_dut_serial_ring_buf_t;

#define SERIAL_GET_NUM(st) \
    ((st).num)

#define SERIAL_GET_MASK(st) \
    ((st).mask)

#define SERIAL_IS_FULL(st) \
    (((uint32_t)((st).write_index - (st).read_index)) >= SERIAL_GET_NUM((st)))

#define SERIAL_GET_COUNT(st) \
    (((st).write_index - (st).read_index) & SERIAL_GET_MASK(st))

#define SERIAL_PUT_VAL(st, value) \
    { \
        (st).pbuf[(st).write_index & SERIAL_GET_MASK((st))] = (value); \
        ++(st).write_index; \
    }

#define SERIAL_GET_VAL(st, value) \
    { \
        (value) = (st).pbuf[(st).read_index & SERIAL_GET_MASK((st))] ; \
        ++(st).read_index; \
    }

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
int wts_dut_serial_init(void);
int wts_dut_serial_config(uint32_t buf_size, uint8_t *buf);
int wts_dut_serial_write(uint32_t len, char *buf);
int wts_dut_serial_read(uint32_t buf_len, char *buf, uint32_t *rece_len);
int wts_dut_serial_read_line(uint32_t buf_len, char *buf, uint32_t *rece_len, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif
#endif /* _WTS_DUT_SERIAL_H_ */
