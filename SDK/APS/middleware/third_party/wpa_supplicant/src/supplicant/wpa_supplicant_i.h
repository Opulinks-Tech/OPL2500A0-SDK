/*
 * wpa_supplicant - Internal definitions
 * Copyright (c) 2003-2014, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef WPA_SUPPLICANT_I_H
#define WPA_SUPPLICANT_I_H

#include "driver/driver.h"
#include "common/ieee802_11_defs.h"
#include "supplicant/config.h"

#ifdef CONFIG_WPA_ENTERPRISE
#include "eap_peer/eap_i.h"
#endif

#ifdef CONFIG_SAE
/**
  * SME (station management entity)
  */
struct wpa_sme {
    struct sae_data sae;
    struct wpabuf *sae_token;
    int sae_group_index;
    unsigned int sae_pmksa_caching:1;
    u16 seq_num;
    u8 ext_auth_bssid[ETH_ALEN];
    u8 ext_auth_ssid[SSID_MAX_LEN];
    size_t ext_auth_ssid_len;
};
#endif

/**
 * struct wpa_supplicant - Internal data for wpa_supplicant interface
 *
 * This structure contains the internal data for core wpa_supplicant code. This
 * should be only used directly from the core code. However, a pointer to this
 * data is used from other files as an arbitrary context pointer in calls to
 * core functions.
 */
struct wpa_supplicant {
    struct wpa_config *conf;
    struct wpa_sm *wpa;
    struct wpa_bss *current_bss;
    struct dl_list bss; /* struct wpa_bss::list */
    struct dl_list bss_id; /* struct wpa_bss::list_id */

    /**
     * Pointers to BSS entries in the order they were in the last scan
     * results.
     */
    struct wpa_bss **last_scan_res;
    struct os_reltime last_scan;
    const struct wpa_driver_ops *driver;

#ifdef CONFIG_WPA_ENTERPRISE
    struct eap_sm *eap;
#endif

#ifdef CONFIG_SAE
    struct wpa_sme sme;
    char  *auth_sae_buf;     /* include the ieee80211_mgmt header length */
    int    auth_sae_buf_len;
    char  *auth_sae_rx_buf;  /* copy receive data by driver */
    int    auth_sae_rx_buf_len;
#endif

    int debug_log_level;
    uint16_t aid;
    unsigned int bss_update_idx;
    unsigned int bss_next_id;
    size_t num_bss;
    unsigned int last_scan_res_used;
    unsigned int last_scan_res_size;

};

extern struct wpa_supplicant *wpa_s;

#endif
