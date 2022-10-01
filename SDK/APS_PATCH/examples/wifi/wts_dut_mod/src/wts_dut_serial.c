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

#include <stdlib.h>
#include <string.h>

#include "wfa_portall.h"
#include "wts_dut_serial.h"
#include "hal_uart.h"

wts_dut_serial_ring_buf_t g_wts_dut_serial_ring = {0};

static void wts_dut_serial_uart0_rx_int_handler(uint32_t u32Data)
{
    if (SERIAL_IS_FULL(g_wts_dut_serial_ring)) {
        printf("The rx serial buffer is full\r\n");
        return;
    }
    else {
        SERIAL_PUT_VAL(g_wts_dut_serial_ring, u32Data & 0xFF);
    }
}

int wts_dut_serial_read(uint32_t buf_len, char *buf, uint32_t *rece_len)
{
    uint32_t in_count;
    uint32_t received = 0;
    uint32_t num_byte = 0;
    in_count = SERIAL_GET_COUNT(g_wts_dut_serial_ring);
    if (in_count == 0) {
        *rece_len = 0;
        return -1;
    }

    while (received < buf_len) {
        SERIAL_GET_VAL(g_wts_dut_serial_ring, buf[num_byte]);
        num_byte++;
        received++;

        in_count = SERIAL_GET_COUNT(g_wts_dut_serial_ring);
        if (in_count == 0)
            break;
    }

    *rece_len = received;

    return 0;
}

int wts_dut_serial_read_line(uint32_t buf_len, char *buf, uint32_t *rece_len, uint32_t timeout_ms)
{
#define CR_BIT 0
#define LF_BIT 1
    uint32_t in_count;
    uint32_t received = 0;
    uint32_t num_byte = 0;
    uint32_t timeout_count = 0;
    uint8_t  new_line_bit = 0;

    do {
        in_count = SERIAL_GET_COUNT(g_wts_dut_serial_ring);
        if (in_count == 0) {
//            usleep(1000);
            osDelay(1);
            timeout_count++;
            continue;
        }

        SERIAL_GET_VAL(g_wts_dut_serial_ring, buf[num_byte]);
        received++;

        if (buf[num_byte] == 0x0D)
            new_line_bit |= (1 << CR_BIT);
        else if (buf[num_byte] == 0x0A)
            new_line_bit |= (1 << LF_BIT);

        num_byte++;

        if (received == buf_len) {
            break;
        }

        if (new_line_bit == 0x3) {
            break;
        }

    } while(timeout_count < timeout_ms);

    *rece_len = received;

    return 0;
}

int wts_dut_serial_write(uint32_t len, char *buf)
{
    for (int i=0; i<len; i++) {
        wUSLEEP(1000);
        Hal_Uart_DataSend(UART_IDX_0, buf[i]);
    }
    return 0;
}

int wts_dut_serial_config(uint32_t buf_size, uint8_t *buf)
{
    if (buf_size == 0)
        return -1;

    g_wts_dut_serial_ring.num  = buf_size;
    g_wts_dut_serial_ring.mask = buf_size - 1;
    g_wts_dut_serial_ring.pbuf = buf;

    return 0;
}

int wts_dut_serial_init(void)
{
    Hal_Uart_RxCallBackFuncSet(UART_IDX_0, wts_dut_serial_uart0_rx_int_handler);
    Hal_Uart_RxIntEn(UART_IDX_0, IRQ_PRIORITY_UART1, 1);

    return 0;
}
