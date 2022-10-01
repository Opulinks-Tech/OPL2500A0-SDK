/******************************************************************************
*  Copyright 2017 - 2021, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef _PING_SOCK_H_
#define _PING_SOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/ip_addr.h"


/**
* @brief Type of "ping" callback functions
*
*/
typedef struct {
    /**
    * @brief arguments for callback functions
    *
    */
    void *cb_args;

    /**
    * @brief Invoked by internal ping thread when received ICMP echo reply packet
    *
    */
    void (*on_ping_success)(void *hdl, void *args);

    /**
    * @brief Invoked by internal ping thread when receive ICMP echo reply packet timeout
    *
    */
    void (*on_ping_timeout)(void *hdl, void *args);

    /**
    * @brief Invoked by internal ping thread when a ping session is finished
    *
    */
    void (*on_ping_end)(void *hdl, void *args);
} ping_callbacks_t;

/**
* @brief Type of "ping" configuration
*
*/
typedef struct {
    uint32_t count;           /*!< A "ping" session contains count procedures */
    uint32_t interval_ms;     /*!< Milliseconds between each ping procedure */
    uint32_t timeout_ms;      /*!< Timeout value (in milliseconds) of each ping procedure */
    uint32_t data_size;       /*!< Size of the data next to ICMP packet header */
    uint8_t tos;              /*!< Type of Service, a field specified in the IP header */
    ip_addr_t target_addr;    /*!< Target IP address, either IPv4 or IPv6 */
    uint32_t task_stack_size; /*!< Stack size of internal ping task */
    uint32_t task_prio;       /*!< Priority of internal ping task */
    uint32_t interface;       /*!< Netif index, interface=0 means NETIF_NO_INDEX*/
} ping_config_t;

/**
 * @brief Default ping configuration
 *
 */
#define PING_DEFAULT_CONFIG()            \
    {                                    \
        .count = 3,                      \
        .interval_ms = 1000,             \
        .timeout_ms = 1000,              \
        .data_size = 64,                 \
        .tos = 0,                        \
        .target_addr = *(IP_ANY_TYPE),   \
        .task_stack_size = 512,          \
        .task_prio = 0,                  \
        .interface = 0,\
    }

/**
* @brief Profile of ping session
*
*/
typedef enum {
    PING_PROF_SEQNO,   /*!< Sequence number of a ping procedure */
    PING_PROF_TTL,     /*!< Time to live of a ping procedure */
    PING_PROF_REQUEST, /*!< Number of request packets sent out */
    PING_PROF_REPLY,   /*!< Number of reply packets received */
    PING_PROF_IPADDR,  /*!< IP address of replied target */
    PING_PROF_SIZE,    /*!< Size of received packet */
    PING_PROF_TIMEGAP, /*!< Elapsed time between request and reply packet */
    PING_PROF_DURATION /*!< Elapsed time of the whole ping session */
} ping_profile_t;

typedef struct _ping_found {
    uint32_t resp_time;
    uint32_t timeout_count;
    uint32_t send_count;
    uint32_t recv_count;
    uint32_t err_count;
    uint32_t bytes;
    uint32_t total_bytes;
    uint32_t total_time;
    uint32_t min_time;
    uint32_t max_time;
    int8_t  ping_err;
} ping_found;

typedef enum {
    PING_TARGET_IP_ADDRESS          = 50,   /**< target IP address */
    PING_TARGET_IP_ADDRESS_COUNT    = 51,   /**< target IP address total counter */
    PING_TARGET_RCV_TIMEO           = 52,   /**< receive timeout in milliseconds */
    PING_TARGET_DELAY_TIME          = 53,   /**< delay time in milliseconds */
    PING_TARGET_ID                  = 54,   /**< identifier */
    PING_TARGET_RES_FN              = 55,   /**< ping result callback function */
    PING_TARGET_RES_RESET           = 56,   /**< ping result statistic reset */
    PING_TARGET_DATA_LEN            = 57,   /**< ping data length*/
    PING_TARGET_IP_TOS              = 58,    /**< ping QOS*/
    PING_TARGET_IF_INDEX            = 59    /**< ping if index*/
} ping_target_id_t;

typedef enum {
    PING_RES_TIMEOUT = 0,
    PING_RES_OK      = 1,
    PING_RES_FINISH  = 2,
} ping_res_t;

typedef void (* ping_found_fn)(ping_target_id_t found_id, ping_found *found_val);

/**
 * @brief Create a ping session
 *
 * @param config ping configuration
 * @param cbs a bunch of callback functions invoked by internal ping task
 * @param hdl_out handle of ping session
 * @return
 *      - 0x102: invalid parameters (e.g. configuration is null, etc)
 *      - 0x101: out of memory
 *      - -1: other internal error (e.g. socket error)
 *      - 0: create ping session successfully, user can take the ping handle to do follow-on jobs
 */
int ping_new_session(const ping_config_t *config, const ping_callbacks_t *cbs, void **hdl_out);

/**
 * @brief Delete a ping session
 *
 * @param hdl handle of ping session
 * @return
 *      - 0x102: invalid parameters (e.g. ping handle is null, etc)
 *      - 0: delete ping session successfully
 */
int ping_delete_session(void *hdl);

/**
 * @brief Start the ping session
 *
 * @param hdl handle of ping session
 * @return
 *      - 0x102: invalid parameters (e.g. ping handle is null, etc)
 *      - 0: start ping session successfully
 */
int ping_start(void *hdl);

/**
 * @brief Stop the ping session
 *
 * @param hdl handle of ping session
 * @return
 *      - 0x102: invalid parameters (e.g. ping handle is null, etc)
 *      - 0: stop ping session successfully
 */
int ping_stop(void *hdl);

/**
 * @brief Get runtime profile of ping session
 *
 * @param hdl handle of ping session
 * @param profile type of profile
 * @param data profile data
 * @param size profile data size
 * @return
 *      - 0x102: invalid parameters (e.g. ping handle is null, etc)
 *      - 0x104: the actual profile data size doesn't match the "size" parameter
 *      - 0: get profile successfully
 */
int ping_get_profile(void *hdl, ping_profile_t profile, void *data, uint32_t size);

/**
 * @brief  Set PING function option
 *
 * @param[in]  opt_id: option index, 50 for IP, 51 for COUNT, 52 for RCV TIMEOUT, 53 for DELAY TIME, 54 for ID
 * @param[in]  opt_val: option parameter
 * @param[in]  opt_len: option length
 *
 * @return
 *      - 0
 *      - -1
 */
int ping_set_target(ping_target_id_t opt_id, void *opt_val, uint32_t opt_len);

/**
 * @brief  Get PING function option
 *
 * @param[in]  opt_id: option index, 50 for IP, 51 for COUNT, 52 for RCV TIMEOUT, 53 for DELAY TIME, 54 for ID
 * @param[in]  opt_val: option parameter
 * @param[in]  opt_len: option length
 *
 * @return
 *      - 0
 *      - -1
 */
int ping_get_target(ping_target_id_t opt_id, void *opt_val, uint32_t opt_len);

/**
 * @brief  Get PING function result action
 *
 * @param[in]  res_val: ping function action, 1 for successful, 0 for fail.
 *             res_len: response bytes
 *             res_time: response time
 *
 * @return
 *      - 0
 *      - -1
 */
int ping_result(uint8_t res_val, uint16_t res_len, uint32_t res_time);

#ifdef __cplusplus
}
#endif

#endif /* _PING_SOCK_H_ */
