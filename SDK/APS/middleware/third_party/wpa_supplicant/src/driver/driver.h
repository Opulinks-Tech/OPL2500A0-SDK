/*
 * Driver interface definition
 * Copyright (c) 2003-2017, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This file defines a driver interface used by both %wpa_supplicant and
 * hostapd. The first part of the file defines data structures used in various
 * driver operations. This is followed by the struct wpa_driver_ops that each
 * driver wrapper will beed to define with callback functions for requesting
 * driver operations. After this, there are definitions for driver event
 * reporting with wpa_supplicant_event() and some convenience helper functions
 * that can be used to report events.
 */

#ifndef DRIVER_H
#define DRIVER_H

#include "utils/includes.h"

#include "utils/common.h"
#include "common/defs.h"
#include "common/wpa_common.h"

extern const struct wpa_driver_ops wpa_driver_opl_ops;


/**
 * struct wpa_driver_scan_params - Scan parameters
 * Data for struct wpa_driver_ops::scan2().
 */
struct wpa_driver_scan_params {
    uint8_t  bssid[ETH_ALEN];
    uint8_t  ssid[WPA_MAX_SSID_LEN + 1];
    uint8_t  channel;             /* 1 - 14 */
    uint8_t  scan_type;           /* 0:Active, 1:Passive, 2:Mix mode */
    uint16_t scan_time;           /* units: millisecond. */
    uint8_t  loop_scan_by_driver; /* true:loop scan by driver, false: loop scan by MSQ */
    uint8_t  sorting;             /* Sort the scan result */
};

struct wpa_driver_connect_params {
    uint8_t  bssid[ETH_ALEN];
    uint8_t  ssid[WPA_MAX_SSID_LEN];
    uint8_t  ssid_len;
    uint8_t  channel;
    uint8_t  password[64];
    uint8_t  password_len;
    uint32_t auth_type;           /* 0:OPEN, 1:WPA, 2:WPA2, 3:WPA3 */
    uint8_t  sorting;             /* Sort the scan result */
    void    *wpa_rsn_buf;
    uint32_t wpa_rsn_len;
    void    *sae_buf;
    uint32_t sae_buf_len;
    void    *sae_rx_buf;
    uint32_t sae_rx_buf_len;

    #ifdef OPL_AP_INFO_TO_DRIVER
    struct wpa_scan_res *p_ap_info;
    #endif
};

#define DRV_SCAN_SORT_DEFAULT 0   /* Default is channel */
#define DRV_SCAN_SORT_RSSI    1
#define DRV_SCAN_SORT_SSID    2

#define IEEE80211_CAP_ESS	0x0001
#define IEEE80211_CAP_IBSS	0x0002
#define IEEE80211_CAP_PRIVACY	0x0010
#define IEEE80211_CAP_RRM	0x1000

/**
 * struct wpa_scan_res - Scan result for an BSS/IBSS
 * @flags: information flags about the BSS/IBSS (WPA_SCAN_*)
 * @bssid: BSSID
 * @freq: frequency of the channel in MHz (e.g., 2412 = channel 1)
 * @beacon_int: beacon interval in TUs (host byte order)
 * @caps: capability information field in host byte order
 * @qual: signal quality
 * @noise: noise level
 * @level: signal level
 * @tsf: Timestamp
 * @age: Age of the information in milliseconds (i.e., how many milliseconds
 * ago the last Beacon or Probe Response frame was received)
 * @snr: Signal-to-noise ratio in dB (calculated during scan result processing)
 * @parent_tsf: Time when the Beacon/Probe Response frame was received in terms
 * of TSF of the BSS specified by %tsf_bssid.
 * @tsf_bssid: The BSS that %parent_tsf TSF time refers to.
 * @ie_len: length of the following IE field in octets
 * @beacon_ie_len: length of the following Beacon IE field in octets
 *
 * This structure is used as a generic format for scan results from the
 * driver. Each driver interface implementation is responsible for converting
 * the driver or OS specific scan results into this format.
 *
 * If the driver does not support reporting all IEs, the IE data structure is
 * constructed of the IEs that are available. This field will also need to
 * include SSID in IE format. All drivers are encouraged to be extended to
 * report all IEs to make it easier to support future additions.
 *
 * This structure data is followed by ie_len octets of IEs from Probe Response
 * frame (or if the driver does not indicate source of IEs, these may also be
 * from Beacon frame). After the first set of IEs, another set of IEs may follow
 * (with beacon_ie_len octets of data) if the driver provides both IE sets.
 */
struct wpa_scan_res {
    unsigned int flags;
#ifdef OPL_SUPPLICANT
    u8 ssid[SSID_MAX_LEN];
    u8 ssid_len;
#endif

    int rsn_ie_offset;
    int wpa_ie_offset;

    u8 bssid[ETH_ALEN];
    int chan;
    u16 beacon_int;
    u16 dtim_period;
    u16 caps;
    int noise;
    int level;
    u64 tsf;
    unsigned int age;
    u64 parent_tsf;
    u8 tsf_bssid[ETH_ALEN];
    size_t ie_len;
    size_t beacon_ie_len;
    /* Followed by ie_len + beacon_ie_len octets of IE data */
};

/**
 * struct wpa_driver_ops - Driver interface API definition
 *
 * This structure defines the API that each driver interface needs to implement
 * for core wpa_supplicant code. All driver specific functionality is captured
 * in this wrapper.
 */
struct wpa_driver_ops {
    /** Name of the driver interface */
    const char *name;
    /** One line description of the driver interface */
    const char *desc;

    /**
     * scan2 - Request the driver to initiate scan
     * @priv: private driver interface data
     * @params: Scan parameters
     *
     * Returns: 0 on success, -1 on failure
     *
     * Once the scan results are ready, the driver should report scan
     * results event for wpa_supplicant which will eventually request the
     * results with wpa_driver_get_scan_results2().
     */
    int (*scan2)(void *priv, void *params);
    int (*scan_stop)(void *priv, void *params);
    int (*sta_connect)(void *priv, void *params);
    int (*sta_disconnect)(void *priv, void *params, void *params2);
    int (*get_bssid)(void *priv, u8 *bssid);
    int (*get_sta_mac_addr)(void *priv, u8 *bssid);
    int (*send_to)(void *priv, void *buf, u32 len, u8 *dest, u16 proto);
#ifdef CONFIG_APS_HANDLE_AUTH_ASSOC
    int (*send_mgmt_frame)(void *priv, void *buf, u32 len);
#endif
    int (*start_encrypt_eapol_frame)(void *priv, Boolean en);
    int (*install_ptk)(void *priv, uint32_t alg, uint32_t key_idx, uint32_t tk1_len);
    int (*install_gtk)(void *priv, uint32_t alg, uint32_t key_idx, uint32_t key_len, uint8_t *keys);
#ifdef CONFIG_SAE
    int (*install_igtk)(void *priv, uint32_t alg, uint32_t key_idx, uint32_t key_len, uint8_t *keys, uint8_t *pn);
#endif
    int (*clear_ptk_gtk_keys)(void *priv);
    int (*sta_join_done)(void *priv, uint8_t reason);

#ifdef CONFIG_SAE
    int (*sta_sae_auth_tx_data)(void *priv, void *buf, uint32_t buf_len);
#endif

#ifdef OPL_PLATFORM_HW_SECURITY
    int (*hw_hmac_sha_1)(uint8_t *sk, int sk_len, uint8_t *in_data, int in_data_len, uint8_t *mac);
#endif

    /**
     * Register the callback into driver to notify wpa_supplicant task.
     */
    int (*register_scan_done_cb)(void *priv, int (*cb)(void *argu, uint8_t ap_num, int reason));
    int (*register_add_bss_cb)(void *priv, int(*cb)(void *argu));
#ifdef CONFIG_APS_HANDLE_AUTH_ASSOC
    int (*register_auth_start_cb)(void *priv, int (*cb)(void *argu, void *buf, uint32_t buf_len));
    int (*register_rx_mgmt_frame_cb)(void *priv, int (*cb)(void *buf, uint32_t len));
#endif
    int (*register_probe_start_cb)(void *priv, int (*cb)(void *argu, void *buf, uint32_t buf_len));
    int (*register_auth_cb)(void *priv, int (*cb)(void *argu, void *buf, uint32_t buf_len));
    int (*register_assoc_cb)(void *priv, int (*cb)(void *argu, void *buf, uint32_t buf_len));
    int (*register_disconnect_cb)(void *priv, int (*cb)(void *argu, int reason));
    int (*register_rx_eapol_frame_cb)(void *priv, int (*cb)(void *buf, uint32_t len));
};

#endif /* DRIVER_H */

