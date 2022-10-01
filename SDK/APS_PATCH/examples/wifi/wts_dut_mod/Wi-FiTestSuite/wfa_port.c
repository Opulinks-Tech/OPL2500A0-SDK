/****************************************************************************
Copyright (c) 2016 Wi-Fi Alliance.  All Rights Reserved

Permission to use, copy, modify, and/or distribute this software for any purpose with or
without fee is hereby granted, provided that the above copyright notice and this permission
notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

******************************************************************************/

/*
 *   File: wfa_cs.c -- configuration and setup
 *   This file contains all implementation for the dut setup and control
 *   functions, such as network interfaces, ip address and wireless specific
 *   setup with its supplicant.
 *
 *   The current implementation is to show how these functions
 *   should be defined in order to support the Agent Control/Test Manager
 *   control commands. To simplify the current work and avoid any GPL licenses,
 *   the functions mostly invoke shell commands by calling linux system call,
 *   system("<commands>").
 *
 *   It depends on the differnt device and platform, vendors can choice their
 *   own ways to interact its systems, supplicants and process these commands
 *   such as using the native APIs.
 *
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmsis_os.h"
#include "util_func.h"

//#include "FreeRTOS_POSIX.h"
//#include "FreeRTOS_POSIX/pthread.h"
#include "FreeRTOS_POSIX.h"
#include "FreeRTOS_POSIX/time.h"
#include "FreeRTOS_POSIX/utils.h"
#include "lwip/sockets.h" //for timeval

//int gettimeofday(struct timeval *tv, void *tzp)
//{
//    uint32_t now_sec, now_ms;
//    util_get_current_time(&now_sec, &now_ms);
//    tv->tv_sec = now_sec;
//    tv->tv_usec = now_ms * 1000;
//    return 0;
//}

#if 0
unsigned sleep( unsigned seconds )
{
    vTaskDelay( pdMS_TO_TICKS( seconds * 1000 ) );

    return 0;
}

int usleep( unsigned long usec )
{
    /* To avoid delaying for less than usec, always round up. */
    vTaskDelay( pdMS_TO_TICKS( usec / 1000 + ( usec % 1000 != 0 ) ) );

    return 0;
}
#endif
