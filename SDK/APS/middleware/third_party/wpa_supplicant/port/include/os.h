/*
 * OS specific functions
 * Copyright (c) 2005-2009, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef OS_H
#define OS_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef long os_time_t;

struct os_time {
	os_time_t sec;
	os_time_t usec;
};

#define os_reltime os_time
#define os_get_reltime os_get_time

struct os_tm {
    int sec; /* 0..59 or 60 for leap seconds */
    int min; /* 0..59 */
    int hour; /* 0..23 */
    int day; /* 1..31 */
    int month; /* 1..12 */
    int year; /* Four digit year */
};

/* Helper macros for handling struct os_time */

static inline int os_time_before(struct os_time *a, struct os_time *b)
{
	return (a->sec < b->sec) ||
	       (a->sec == b->sec && a->usec < b->usec);
}

static inline void os_time_sub(struct os_time *a, struct os_time *b,
			       struct os_time *res)
{
	res->sec = a->sec - b->sec;
	res->usec = a->usec - b->usec;
	if (res->usec < 0) {
		res->sec--;
		res->usec += 1000000;
	}
}

/**
 * os_get_time - Get current time (sec, usec)
 * @t: Pointer to buffer for the time
 * Returns: 0 on success, -1 on failure
 */
int os_get_time(struct os_time *t);

/**
 * os_sleep - Sleep (sec, usec)
 * @sec: Number of seconds to sleep
 * @usec: Number of microseconds to sleep
 */
void os_sleep(os_time_t sec, os_time_t usec);

/**
 * os_mktime - Convert broken-down time into seconds since 1970-01-01
 * @year: Four digit year
 * @month: Month (1 .. 12)
 * @day: Day of month (1 .. 31)
 * @hour: Hour (0 .. 23)
 * @min: Minute (0 .. 59)
 * @sec: Second (0 .. 60)
 * @t: Buffer for returning calendar time representation (seconds since
 * 1970-01-01 00:00:00)
 * Returns: 0 on success, -1 on failure
 *
 * Note: The result is in seconds from Epoch, i.e., in UTC, not in local time
 * which is used by POSIX mktime().
 */
int os_mktime(int year, int month, int day, int hour, int min, int sec,
              os_time_t *t);

/**
 * os_get_random - Get cryptographically strong pseudo random data
 * @buf: Buffer for pseudo random data
 * @len: Length of the buffer
 * Returns: 0 on success, -1 on failure
 */
int os_get_random(unsigned char *buf, size_t len);

/*
 * The following functions are wrapper for standard ANSI C or POSIX functions.
 * By default, they are just defined to use the standard function name and no
 * os_*.c implementation is needed for them. This avoids extra function calls
 * by allowing the C pre-processor take care of the function name mapping.
 *
 * If the target system uses a C library that does not provide these functions,
 * build_config.h can be used to define the wrappers to use a different
 * function name. This can be done on function-by-function basis since the
 * defines here are only used if build_config.h does not define the os_* name.
 * If needed, os_*.c file can be used to implement the functions that are not
 * included in the C library on the target system. Alternatively,
 * OS_NO_C_LIB_DEFINES can be defined to skip all defines here in which case
 * these functions need to be implemented in os_*.c file for the target system.
 */

#ifndef os_malloc
//#define os_malloc(s) malloc((s))
void * os_malloc(int size);
#endif
#ifndef os_realloc
//#define os_realloc(p, s) realloc((p), (s))
void * os_realloc(void *ptr, size_t size);
#endif
#ifndef os_zalloc
//#define os_zalloc(s) calloc(1, (s))
void * os_zalloc(int size);
#endif
#ifndef os_free
//#define os_free(p) free((p))
void os_free(void *ptr);
#endif

#ifndef os_bzero
#define os_bzero(s, n) os_memset(s, 0, n)
#endif
#ifndef os_strdup
//#define os_strdup(s) strdup(s)
char * os_strdup(const char *s);
#endif
#ifndef os_memcpy
#define os_memcpy(d, s, n) memcpy((d), (s), (n))
#endif
#ifndef os_memmove
#define os_memmove(d, s, n) memmove((d), (s), (n))
#endif
#ifndef os_memset
#define os_memset(s, c, n) memset(s, c, n)
#endif
#ifndef os_memcmp
#define os_memcmp(s1, s2, n) memcmp((s1), (s2), (n))
#endif
#ifndef os_memcmp_const
//#define os_memcmp_const(s1, s2, n) memcmp((s1), (s2), (n))
int os_memcmp_const(const void *a, const void *b, int len);
#endif
#ifndef os_strlen
#define os_strlen(s) strlen(s)
#endif
#ifndef os_strcasecmp
#define os_strcasecmp(s1, s2) strcasecmp((s1), (s2))
#endif
#ifndef os_strncasecmp
#define os_strncasecmp(s1, s2, n) strncasecmp((s1), (s2), (n))
#endif
#ifndef os_strchr
#define os_strchr(s, c) strchr((s), (c))
#endif
#ifndef os_strcmp
#define os_strcmp(s1, s2) strcmp((s1), (s2))
#endif
#ifndef os_strncmp
#define os_strncmp(s1, s2, n) strncmp((s1), (s2), (n))
#endif
#ifndef os_strncpy
#define os_strncpy(d, s, n) strncpy((d), (s), (n))
#endif
#ifndef os_strrchr
#define os_strrchr(s, c)  strrchr((s), (c))
#endif
#ifndef os_strstr
#define os_strstr(h, n) strstr((h), (n))
#endif
#ifndef os_strlcpy
#define os_strlcpy(d, s, n) strlcpy((d), (s), (n))
#endif
#ifndef os_snprintf
#define os_snprintf snprintf
#endif

static inline void forced_memzero(void *ptr, size_t len)
{
	os_memset(ptr, 0, len);
//	if (len) {
//		forced_memzero_val = ((uint8_t *) ptr)[0];
//	}
}

static inline int os_snprintf_error(size_t size, int res)
{
    return res < 0 || (unsigned int) res >= size;
}

void os_sleep(os_time_t sec, os_time_t usec);
int os_timer_create(uint32_t *timer, char *timer_name, void (*callback) (void *argument), void *argument, unsigned char type);
int os_timer_start_once(void *timer, unsigned int millisec);
int os_timer_stop(void *timer);
int os_timer_delete(void *timer);
int os_task_create(const char *task_name, const int pri, const int stack_size, void (*task) (void *argument), uint32_t *task_handler);
int os_task_delete(void *handler);
int os_message_queue_create(int item_size, int queue_length, void *task_hander, uint32_t *queue_handler);
int os_message_queue_delete(void *handler);
void * os_message_queue_get(void *handler, uint32_t wait_ms);
int os_message_queue_put(void *handler, void *msg);
void os_enter_critical(void);
void os_exit_critical(void);

#endif /* OS_H */
