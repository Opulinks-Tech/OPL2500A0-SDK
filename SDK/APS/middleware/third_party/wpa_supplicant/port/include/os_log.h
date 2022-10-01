

#ifndef __OPL_LOG_H__
#define __OPL_LOG_H__

#include <stdint.h>
#include <stdarg.h>
#include "msg.h"
#include "os.h"

#include "cmsis_os.h"
#include "task.h"
#include "time.h"

#define CONFIG_LOG_COLORS
#define CONFIG_LOG_DEFAULT_LEVEL 5
#define CONFIG_LOG_TIMESTAMP_FORMAT
    
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Log level
 *
 */
typedef enum {
    LOG_NONE,       /*!< No log output */
    LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO,       /*!< Information messages which describe normal flow of events */
    LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} log_level_t;

typedef int (*vprintf_like_t)(const char *, va_list);

/**
 * @brief Set log level for given tag
 *
 * If logging for given component has already been enabled, changes previous setting.
 *
 * Note that this function can not raise log level above the level set using
 * CONFIG_LOG_DEFAULT_LEVEL setting in menuconfig.
 *
 * To raise log level above the default one for a given file, define
 * LOG_LOCAL_LEVEL to one of the LOG_* values, before including
 * log.h in this file.
 *
 * @param tag Tag of the log entries to enable. Must be a non-NULL zero terminated string.
 *            Value "*" resets log level for all tags to the given value.
 *
 * @param level  Selects log level to enable. Only logs at this and lower verbosity
 * levels will be shown.
 */
void log_level_set(const char* tag, log_level_t level);

/**
 * @brief Set function used to output log entries
 *
 * By default, log output goes to UART0. This function can be used to redirect log
 * output to some other destination, such as file or network. Returns the original
 * log handler, which may be necessary to return output to the previous destination.
 *
 * @param func new Function used for output. Must have same signature as vprintf.
 *
 * @return func old Function used for output.
 */
vprintf_like_t log_set_vprintf(vprintf_like_t func);

/**
 * @brief Function which returns timestamp to be used in log output
 *
 * This function is used in expansion of LOGx macros.
 * In the 2nd stage bootloader, and at early application startup stage
 * this function uses CPU cycle counter as time source. Later when
 * FreeRTOS scheduler start running, it switches to FreeRTOS tick count.
 *
 * For now, we ignore millisecond counter overflow.
 *
 * @return timestamp, in milliseconds
 */
uint32_t log_timestamp(void);

/**
 * @brief Function which returns system timestamp to be used in log output
 *
 * This function is used in expansion of LOGx macros to print
 * the system time as "HH:MM:SS.sss". The system time is initialized to
 * 0 on startup, this can be set to the correct time with an SNTP sync,
 * or manually with standard POSIX time functions.
 *
 * Currently this will not get used in logging from binary blobs
 * (i.e WiFi & Bluetooth libraries), these will still print the RTOS tick time.
 *
 * @return timestamp, in "HH:MM:SS.sss"
 */
char* log_system_timestamp(void);

/**
 * @brief Function which returns timestamp to be used in log output
 *
 * This function uses HW cycle counter and does not depend on OS,
 * so it can be safely used after application crash.
 *
 * @return timestamp, in milliseconds
 */
uint32_t log_early_timestamp(void);

/**
 * @brief Write message into the log
 *
 * This function is not intended to be used directly. Instead, use one of
 * LOGE, LOGW, LOGI, LOGD, LOGV macros.
 *
 * This function or these macros should not be used from an interrupt.
 */
void log_write(log_level_t level, const char* tag, const char* format, ...) __attribute__ ((format (printf, 3, 4)));

/**
 * @brief Write message into the log, va_list variant
 * @see log_write()
 *
 * This function is provided to ease integration toward other logging framework,
 * so that log can be used as a log sink.
 */
void log_writev(log_level_t level, const char* tag, const char* format, va_list args);

/** @cond */

#ifndef LOG_LOCAL_LEVEL
#define LOG_LOCAL_LEVEL  CONFIG_LOG_DEFAULT_LEVEL
#endif

/** @endcond */

/**
 * @brief Log a buffer of hex bytes at specified level, separated into 16 bytes each line.
 *
 * @param  tag      description tag
 * @param  buffer   Pointer to the buffer array
 * @param  buff_len length of buffer in bytes
 * @param  level    level of the log
 *
 */
#define LOG_BUFFER_HEX_LEVEL( tag, buffer, buff_len, level ) \
    do {\
        if ( LOG_LOCAL_LEVEL >= (level) ) { \
            log_buffer_hex_internal( tag, buffer, buff_len, level ); \
        } \
    } while(0)

/**
 * @brief Log a buffer of characters at specified level, separated into 16 bytes each line. Buffer should contain only printable characters.
 *
 * @param  tag      description tag
 * @param  buffer   Pointer to the buffer array
 * @param  buff_len length of buffer in bytes
 * @param  level    level of the log
 *
 */
#define LOG_BUFFER_CHAR_LEVEL( tag, buffer, buff_len, level ) \
    do {\
        if ( LOG_LOCAL_LEVEL >= (level) ) { \
            log_buffer_char_internal( tag, buffer, buff_len, level ); \
        } \
    } while(0)

/**
 * @brief Dump a buffer to the log at specified level.
 *
 * The dump log shows just like the one below:
 *
 *      W (195) log_example: 0x3ffb4280   4F 50 4C 31 78 20 69 73  20 67 72 65 61 74 2c 20  |OPL1x is great, |
 *      W (195) log_example: 0x3ffb4290   77 6f 72 6b 69 6e 67 20  61 6c 6f 6e 67 20 77 69  |working along wi|
 *      W (205) log_example: 0x3ffb42a0   74 68 20 74 68 65 20 49  44 46 2e 00              |th the IDF..|
 *
 * It is highly recommend to use terminals with over 102 text width.
 *
 * @param tag description tag
 * @param buffer Pointer to the buffer array
 * @param buff_len length of buffer in bytes
 * @param level level of the log
 */
#define LOG_BUFFER_HEXDUMP( tag, buffer, buff_len, level ) \
    do { \
        if ( LOG_LOCAL_LEVEL >= (level) ) { \
            log_buffer_hexdump_internal( tag, buffer, buff_len, level); \
        } \
    } while(0)

/**
 * @brief Log a buffer of hex bytes at Info level
 *
 * @param  tag      description tag
 * @param  buffer   Pointer to the buffer array
 * @param  buff_len length of buffer in bytes
 *
 * @see ``log_buffer_hex_level``
 *
 */
#define LOG_BUFFER_HEX(tag, buffer, buff_len) \
    do { \
        if (LOG_LOCAL_LEVEL >= LOG_INFO) { \
            LOG_BUFFER_HEX_LEVEL( tag, buffer, buff_len, LOG_INFO ); \
        }\
    } while(0)

/**
 * @brief Log a buffer of characters at Info level. Buffer should contain only printable characters.
 *
 * @param  tag      description tag
 * @param  buffer   Pointer to the buffer array
 * @param  buff_len length of buffer in bytes
 *
 * @see ``log_buffer_char_level``
 *
 */
#define LOG_BUFFER_CHAR(tag, buffer, buff_len) \
    do { \
        if (LOG_LOCAL_LEVEL >= LOG_INFO) { \
            LOG_BUFFER_CHAR_LEVEL( tag, buffer, buff_len, LOG_INFO ); \
        }\
    } while(0)

/** @cond */

//to be back compatible
#define log_buffer_hex      LOG_BUFFER_HEX
#define log_buffer_char     LOG_BUFFER_CHAR


#ifdef CONFIG_LOG_COLORS
#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR_YELLOW  "93"
#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D       LOG_COLOR(LOG_COLOR_YELLOW)
#define LOG_COLOR_V       LOG_COLOR(LOG_COLOR_CYAN)
#else //CONFIG_LOG_COLORS
#define LOG_COLOR_E
#define LOG_COLOR_W
#define LOG_COLOR_I
#define LOG_COLOR_D
#define LOG_COLOR_V
#define LOG_RESET_COLOR
#endif //CONFIG_LOG_COLORS

#define LOG_FORMAT(letter, format)              LOG_COLOR_ ## letter #letter " %s %s: " format LOG_RESET_COLOR "\r\n"
#define LOG_SYSTEM_TICK_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " (%u) %s: " format LOG_RESET_COLOR "\r\n"
#define LOG_SYSTEM_TIME_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " (%s) %s: " format LOG_RESET_COLOR "\r\n"

/** @endcond */

/// macro to output logs in startup code, before heap allocator and syscalls have been initialized. log at ``LOG_ERROR`` level. @see ``printf``,``LOGE``
#define EARLY_LOGE( tag, format, ... ) LOG_EARLY_IMPL(tag, format, LOG_ERROR,   E, ##__VA_ARGS__)
/// macro to output logs in startup code at ``LOG_WARN`` level.  @see ``EARLY_LOGE``,``LOGE``, ``printf``
#define EARLY_LOGW( tag, format, ... ) LOG_EARLY_IMPL(tag, format, LOG_WARN,    W, ##__VA_ARGS__)
/// macro to output logs in startup code at ``LOG_INFO`` level.  @see ``EARLY_LOGE``,``LOGE``, ``printf``
#define EARLY_LOGI( tag, format, ... ) LOG_EARLY_IMPL(tag, format, LOG_INFO,    I, ##__VA_ARGS__)
/// macro to output logs in startup code at ``LOG_DEBUG`` level.  @see ``EARLY_LOGE``,``LOGE``, ``printf``
#define EARLY_LOGD( tag, format, ... ) LOG_EARLY_IMPL(tag, format, LOG_DEBUG,   D, ##__VA_ARGS__)
/// macro to output logs in startup code at ``LOG_VERBOSE`` level.  @see ``EARLY_LOGE``,``LOGE``, ``printf``
#define EARLY_LOGV( tag, format, ... ) LOG_EARLY_IMPL(tag, format, LOG_VERBOSE, V, ##__VA_ARGS__)

#define LOG_EARLY_IMPL(tag, format, log_level, log_tag_letter, ...) do {                         \
        if (LOG_LOCAL_LEVEL >= log_level) {                                                          \
            printf(LOG_FORMAT(log_tag_letter, format), log_timestamp(), tag, ##__VA_ARGS__); \
        }} while(0)

#ifndef BOOTLOADER_BUILD
#define LOGE( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_ERROR,   tag, format, ##__VA_ARGS__)
#define LOGW( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_WARN,    tag, format, ##__VA_ARGS__)
#define LOGI( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_INFO,    tag, format, ##__VA_ARGS__)
#define LOGD( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_DEBUG,   tag, format, ##__VA_ARGS__)
#define LOGV( tag, format, ... ) LOG_LEVEL_LOCAL(LOG_VERBOSE, tag, format, ##__VA_ARGS__)
#else
/**
 * macro to output logs at LOG_ERROR level.
 *
 * @param tag tag of the log, which can be used to change the log level by ``log_level_set`` at runtime.
 *
 * @see ``printf``
 */
#define LOGE( tag, format, ... )  EARLY_LOGE(tag, format, ##__VA_ARGS__)
/// macro to output logs at ``LOG_WARN`` level.  @see ``LOGE``
#define LOGW( tag, format, ... )  EARLY_LOGW(tag, format, ##__VA_ARGS__)
/// macro to output logs at ``LOG_INFO`` level.  @see ``LOGE``
#define LOGI( tag, format, ... )  EARLY_LOGI(tag, format, ##__VA_ARGS__)
/// macro to output logs at ``LOG_DEBUG`` level.  @see ``LOGE``
#define LOGD( tag, format, ... )  EARLY_LOGD(tag, format, ##__VA_ARGS__)
/// macro to output logs at ``LOG_VERBOSE`` level.  @see ``LOGE``
#define LOGV( tag, format, ... )  EARLY_LOGV(tag, format, ##__VA_ARGS__)
#endif  // BOOTLOADER_BUILD

static inline char *log_get_system_timestamp(void)
{
    static char buffer[18] = {0};
    struct tm now_time;
    long cur_sys_tick = osKernelSysTick();
    time_t cur_secs = cur_sys_tick / 1000;
    localtime_r((time_t *)&cur_secs, &now_time);
    snprintf(buffer, sizeof(buffer),
             "%02d:%02d:%02d.%03ld",
             now_time.tm_hour,
             now_time.tm_min,
             now_time.tm_sec,
             cur_sys_tick % 1000);
    return buffer;
}

/** runtime macro to output logs at a specified level.
 *
 * @param tag tag of the log, which can be used to change the log level by ``log_level_set`` at runtime.
 * @param level level of the output log.
 * @param format format of the output log. see ``printf``
 * @param ... variables to be replaced into the log. see ``printf``
 *
 * @see ``printf``
 */
#ifdef CONFIG_LOG_TIMESTAMP_FORMAT
#define LOG_LEVEL(level, tag, format, ...) do {                     \
        if (level==LOG_ERROR )          { tracer_log(LOG_HIGH_LEVEL, LOG_SYSTEM_TIME_FORMAT(E, format), log_get_system_timestamp(), tag, ##__VA_ARGS__); } \
        else if (level==LOG_WARN )      { tracer_log(LOG_HIGH_LEVEL, LOG_SYSTEM_TIME_FORMAT(W, format), log_get_system_timestamp(), tag, ##__VA_ARGS__); } \
        else if (level==LOG_DEBUG )     { tracer_log(LOG_HIGH_LEVEL, LOG_SYSTEM_TIME_FORMAT(D, format), log_get_system_timestamp(), tag, ##__VA_ARGS__); } \
        else if (level==LOG_VERBOSE )   { tracer_log(LOG_HIGH_LEVEL, LOG_SYSTEM_TIME_FORMAT(V, format), log_get_system_timestamp(), tag, ##__VA_ARGS__); } \
        else                            { tracer_log(LOG_HIGH_LEVEL, LOG_SYSTEM_TIME_FORMAT(I, format), log_get_system_timestamp(), tag, ##__VA_ARGS__); } \
    } while(0)
#else
#define LOG_LEVEL(level, tag, format, ...) do {                     \
        if (level==LOG_ERROR )          { tracer_log(LOG_HIGH_LEVEL, LOG_FORMAT(E, format), NULL, tag, ##__VA_ARGS__); } \
        else if (level==LOG_WARN )      { tracer_log(LOG_HIGH_LEVEL, LOG_FORMAT(W, format), NULL, tag, ##__VA_ARGS__); } \
        else if (level==LOG_DEBUG )     { tracer_log(LOG_HIGH_LEVEL, LOG_FORMAT(D, format), NULL, tag, ##__VA_ARGS__); } \
        else if (level==LOG_VERBOSE )   { tracer_log(LOG_HIGH_LEVEL, LOG_FORMAT(V, format), NULL, tag, ##__VA_ARGS__); } \
        else                            { tracer_log(LOG_HIGH_LEVEL, LOG_FORMAT(I, format), NULL, tag, ##__VA_ARGS__); } \
    } while(0)
#endif
    
/** runtime macro to output logs at a specified level. Also check the level with ``LOG_LOCAL_LEVEL``.
 *
 * @see ``printf``, ``LOG_LEVEL``
 */
#define LOG_LEVEL_LOCAL(level, tag, format, ...) do {               \
        if ( LOG_LOCAL_LEVEL >= level ) LOG_LEVEL(level, tag, format, ##__VA_ARGS__); \
    } while(0)

#ifdef __cplusplus
}
#endif


#endif /* __OPL_LOG_H__ */
