/* *****************************************************************************
 *  Copyright 2017-2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file controller_wifi.h
 *
 *  @brief `
 *
 ******************************************************************************/

#ifndef __CONTROLLER_WIFI_H__
#error "Please include controller_wifi.h. Don't directly include controller_wifi_patch.h"
#endif

#ifndef __CONTROLLER_WIFI_PATCH_H__
#define __CONTROLLER_WIFI_PATCH_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef struct
{
    bool          free_ocpy;                            /**< scan info buffer is free or occupied, 0:free, 1:occupied */
    uint8_t       bssid[CTRL_WIFI_MAC_ADDR_LEN];        /**< BSS ID - 48 bit HW address */
    uint8_t       ap_channel;                           /**< Which Channel */
    uint64_t      latest_beacon_rx_time;                /**< Timestamp - Last interaction with BSS */
    char          ssid[CTRL_WIFI_MAX_SSID_LEN + 1];     /**< SSID of the BSS - 33 bytes */
    uint8_t       ssid_len;                             /**< Length of SSID */
    int8_t        rssi;                                 /**< Last observed Rx Power (dBm) */
    uint16_t      beacon_interval;                      /**< Beacon interval - In time units of 1024 us */
    uint16_t      capabilities;                         /**< Supported capabilities */
    uint8_t       dtim_prod;                            /**< DTIM Period */
    wpa_ie_data_t wpa_data;                             /**< Deprecation */
    uint8_t       rsn_ie[256];                          /**< ELEMENT_HEADER_LEN + IEEE80211_MAX_RSN_LEN */
    uint8_t       wpa_ie[257];                          /**< ELEMENT_HEADER_LEN + IEEE80211_MAX_WPA_LEN */

    /* Add parameters from here to reduce patch effort */
    uint8_t       tx_bssid[CTRL_WIFI_MAC_ADDR_LEN];     /**< BSS ID - 48 bit HW address. Transmitted BSSID */
} ctrl_wifi_ap_info_patch_t;

typedef struct {
    uint32_t  flags;
    uint8_t   ssid[CTRL_WIFI_MAX_SSID_LEN];
    uint8_t   ssid_len;
    int       rsn_ie_offset;
    int       wpa_ie_offset;
    uint8_t   bssid[CTRL_WIFI_MAC_ADDR_LEN];
    int       channel;
    uint16_t  beacon_int;
    uint16_t  dtim_period;
    uint16_t  caps;
    int       noise;
    int       level;
    uint64_t  tsf;
    uint32_t  age;
    uint64_t  parent_tsf;
    uint8_t   tsf_bssid[CTRL_WIFI_MAC_ADDR_LEN];
    size_t    ie_len;
    size_t    beacon_ie_len;
    uint8_t   ie_buf[100];                              /**< followed by ie_len octets of IEs */
    /**< followed by beacon_ie_len octets of IEs */
} ctrl_wifi_scan_res_patch_t;

typedef struct {
    uint8_t  do_before_scan;
    uint8_t  ssid[CTRL_WIFI_MAX_SSID_LEN];
    uint8_t  ssid_len;
    uint8_t  bssid[CTRL_WIFI_MAC_ADDR_LEN];
    uint8_t  channel;
    uint32_t auth_type;       /**< 0:OPEN, 1:WPA, 2:WPA2, 3:WPA3 */
    void    *wpa_rsn_buf;     /**< WPA/RSN buffer, created by APS. */
    uint32_t wpa_rsn_len;     /**< 0 = Non-Security */
    void    *sae_commit_buf;  /**< For WPA3 used. SAE commit. STA -> AP */
    uint32_t sae_commit_buf_len;
    void    *sae_confirm_buf;
    uint32_t sae_confirm_buf_len;

    ctrl_wifi_scan_res_patch_t *p_ap_info;
} ctrl_wifi_connect_cfg_patch_t;

typedef struct
{
    uint16_t status;          /**< Association status code, range is 0 - 203 */
    uint16_t aid:12;          /**< Association ID. (12 bits, 0 - 4095) */
    uint16_t ht_ctrl_en:1;    /**< +HTC HE Support */
    uint16_t resvered:3;
} ctrl_wifi_assoc_info_patch_t;

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
void ctrl_wifi_init_patch(void);
int ctrl_wifi_rssi_get(int8_t *p_rssi);
#ifdef __cplusplus
}
#endif

#endif  /* __CONTROLLER_WIFI_PATCH_H__ */
