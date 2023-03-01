/******************************************************************************
*  Copyright 2017 - 2022, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef __WIFI_API_PRIV_H__
#define __WIFI_API_PRIV_H__

#include <stdint.h>
#include <stdbool.h>

#include "wifi_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************
 *                    Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
#define WAPI_MAX_AUTO_CONNECT_STORE_NUM 3
#define WAPI_MAX_AUTO_CONNECT_RETRY     2
#define WAPI_MAX_AUTO_CONNECT_ROUNDS    3

#define WAPI_PMK_LEN 32

/******************************************************
 *                   Enumerations
 ******************************************************/


/******************************************************
 *                 Type Definitions
 ******************************************************/


/******************************************************
 *                    Structures
 ******************************************************/
 typedef struct {
    uint8_t in_used;
    uint8_t retry_count;
    uint8_t pw[WIFI_LENGTH_PASSPHRASE];
    uint8_t ssid[WIFI_MAX_LENGTH_OF_SSID];
    uint8_t bssid[WIFI_MAC_ADDRESS_LENGTH];
    uint8_t channel;
    uint8_t pmk[WAPI_PMK_LEN];
    uint8_t pmk_len;
} wifi_auto_connect_ap_list_t;

typedef struct {
    uint8_t mode;
    uint8_t inited;
    uint8_t retry_rounds;
    uint8_t curr_ap_idx;
    uint8_t max_save_num;
    uint8_t is_find;
    wifi_auto_connect_ap_list_t list[WAPI_MAX_AUTO_CONNECT_STORE_NUM];
} wifi_auto_connect_config_t;


/** @brief This structure defines the configuration of Wi-Fi auto connection stored in flash
  */
typedef struct {
    uint8_t curr_ap_idx;
    uint8_t max_save_num;
} mw_fim_wifi_auto_connect_cfg_t;


/**
 * @brief This structure defines the inforamtion of Wi-Fi auto-connect APs info stored in flash
*/
typedef struct {
    uint8_t in_used;
    uint8_t pw[WIFI_LENGTH_PASSPHRASE];
    uint8_t ssid[WIFI_MAX_LENGTH_OF_SSID];
    uint8_t bssid[WIFI_MAC_ADDRESS_LENGTH];
    uint8_t channel;
    uint8_t psk[WAPI_PMK_LEN];
    uint8_t psk_len;
} mw_fim_wifi_auto_connect_ap_info_t;

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_API_PRIV_H__ */
