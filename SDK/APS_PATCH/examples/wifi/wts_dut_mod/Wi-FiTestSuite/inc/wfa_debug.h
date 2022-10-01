/****************************************************************************
*
* Copyright (c) 2016 Wi-Fi Alliance
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
* SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
* NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
* USE OR PERFORMANCE OF THIS SOFTWARE.
*
*****************************************************************************/


#ifndef WFA_DEBUG_H
#define WFA_DEBUG_H

#include "msg.h"

#define WFA_ERR         stdout  /* error: can be redefined to a log file */
#define WFA_OUT         stdout  /* info:  can be redefined to a log file */
#define WFA_WNG         stdout  /* warning: can be redefined to a log file */

#define WFA_DEBUG_DEFAULT          0x0001
#define WFA_DEBUG_ERR              0x0001
#define WFA_DEBUG_INFO             0x0002
#define WFA_DEBUG_WARNING          0x0004

#define WFA_DEBUG 1

extern unsigned short wfa_defined_debug;

#if 1
#define DPRINT_ERR(out, fmt, args...) \
    if(wfa_defined_debug & WFA_DEBUG_ERR) \
        tracer_log(LOG_HIGH_LEVEL, "[%s]: E %s %d"fmt, "WTS", __FILE__, __LINE__, ##args)

#define DPRINT_INFO(out, fmt, args...) \
    if(wfa_defined_debug & WFA_DEBUG_INFO) \
        tracer_log(LOG_HIGH_LEVEL, "[%s]: I "fmt, "WTS", ##args)

#define DPRINT_WARNING(out, fmt, args...) \
    if(wfa_defined_debug & WFA_DEBUG_WARNING) \
        tracer_log(LOG_HIGH_LEVEL, "[%s]: W "fmt, "WTS", ##args)

#else

#define DPRINT_ERR      fprintf(WFA_ERR, "File %s, Line %ld: ", \
                               __FILE__, (long)__LINE__); \
                        fprintf

#define DPRINT_INFO     if(wfa_defined_debug & WFA_DEBUG_INFO) \
                            fprintf

#define DPRINT_WARNING  if(wfa_defined_debug & WFA_DEBUG_WARNING) \
                            fprintf
#endif

#endif /* WFA_DEBUG_H */
