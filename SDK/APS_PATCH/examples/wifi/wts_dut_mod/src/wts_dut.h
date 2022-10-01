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

#ifndef _WTS_DUT_MAIN_H_
#define _WTS_DUT_MAIN_H_

#include <stdint.h>

#define WTS_DUT_INIT_CMD      1

#define WTS_DUT_SERIAL_BUF_LEN 2048

void wts_dut_main_init(void);
int wts_dut_is_wifi_connected(void);
int wts_dut_is_got_ip(void);
int wts_dut_task_send_msg(uint32_t cmd, uint32_t buf_len, void *buf);

#endif /* _WTS_DUT_MAIN_H_ */
