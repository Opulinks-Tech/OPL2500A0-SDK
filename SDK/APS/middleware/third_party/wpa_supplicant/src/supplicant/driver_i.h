/*
 * wpa_supplicant - Internal driver interface wrappers
 * Copyright (c) 2003-2015, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef DRIVER_I_H
#define DRIVER_I_H

#include "driver/driver.h"
#include "wpa_supplicant_i.h"

#include "supplicant/wpa_err.h"
#include "supplicant/ieee80211.h"

#define WPA2_STA_DEAUTH_FRAME_LEN    64

#ifdef CONFIG_APS_HANDLE_AUTH_ASSOC
static inline int wpa_drv_send_mgmt_frame(struct wpa_supplicant *wpa_s, void *buf, u32 len);
#endif

static inline int wpa_drv_scan(struct wpa_supplicant *wpa_s, void *params)
{
    if (wpa_s->driver->scan2)
        return wpa_s->driver->scan2(wpa_s, params);
    return WPA_ERR_FAIL;
}

static inline int wpa_drv_scan_stop(struct wpa_supplicant *wpa_s, void *params)
{
    if (wpa_s->driver->scan_stop)
        return wpa_s->driver->scan_stop(wpa_s, params);
    return WPA_ERR_FAIL;
}

static inline int wpa_drv_sta_connect_start(struct wpa_supplicant *wpa_s, void *params)
{
    if (wpa_s->driver->sta_connect)
        return wpa_s->driver->sta_connect(wpa_s, params);
    return WPA_ERR_FAIL;
}

#ifdef CONFIG_APS_HANDLE_AUTH_ASSOC
static int wpa_drv_sta_send_disc_frame(struct wpa_supplicant *wpa_s, uint32_t reason)
{
    int ret = WPA_ERR_OK;
    int len;

    char *buf = os_zalloc(WPA2_STA_DEAUTH_FRAME_LEN);
    if (buf == NULL) {
        wpa_printf(MSG_ERROR, "malloc tx deauth frame fail");
        ret = WPA_ERR_MALLOC_FAIL;
        goto exit;
    }

    len = ieee80211_build_sta_deauth_request(wpa_s, buf, WPA2_STA_DEAUTH_FRAME_LEN, reason);
    if (len < 0) {
        wpa_printf(MSG_ERROR, "build tx deauth frame fail");
        ret = WPA_ERR_FAIL;
        goto exit;
    }

    ret = wpa_drv_send_mgmt_frame(wpa_s, buf, len);
    if (ret != WPA_ERR_OK) {
        wpa_printf(MSG_ERROR, "send tx auth fail");
    }

exit:
    if (buf)
        os_free(buf);

    return ret;
}
#endif /* CONFIG_APS_HANDLE_AUTH_ASSOC */

static inline int wpa_drv_sta_disconnect(struct wpa_supplicant *wpa_s, void *params)
{
#ifdef CONFIG_APS_HANDLE_AUTH_ASSOC
    wpa_drv_sta_send_disc_frame(wpa_s, (uint32_t)params);

    /* TODO - waiting the driver sends out. */
    os_sleep(0, 200000);

    if (wpa_s->driver->sta_disconnect)
        return wpa_s->driver->sta_disconnect(wpa_s, params, 0);
#else
    if (wpa_s->driver->sta_disconnect)
        return wpa_s->driver->sta_disconnect(wpa_s, params, NULL);
#endif
    return WPA_ERR_FAIL;
}

static inline int wpa_drv_get_bssid(struct wpa_supplicant *wpa_s, u8 *bssid)
{
    if (wpa_s->driver->get_bssid)
        return wpa_s->driver->get_bssid(wpa_s, bssid);
    return WPA_ERR_FAIL;
}

static inline int wpa_drv_get_sta_mac_addr(struct wpa_supplicant *wpa_s, u8 *bssid)
{
    if (wpa_s->driver->get_sta_mac_addr)
        return wpa_s->driver->get_sta_mac_addr(wpa_s, bssid);
    return WPA_ERR_FAIL;
}

static inline int wpa_drv_send_to(struct wpa_supplicant *wpa_s, void *buf, u32 len, u8 *dest, u16 proto)
{
    if (wpa_s->driver->send_to)
        return wpa_s->driver->send_to(wpa_s, buf, len, dest, proto);
    return WPA_ERR_FAIL;
}

#ifdef CONFIG_APS_HANDLE_AUTH_ASSOC
static inline int wpa_drv_send_mgmt_frame(struct wpa_supplicant *wpa_s, void *buf, u32 len)
{
    int ret;
    if (wpa_s->driver->send_mgmt_frame)
        ret = wpa_s->driver->send_mgmt_frame(wpa_s, buf, len);
    if (ret != WPA_ERR_OK)
        return WPA_ERR_BUF_SEND_FAIL;
    else
        return WPA_ERR_OK;
}
#endif

static inline int wpa_drv_start_encrypt_eapol_frame(struct wpa_supplicant *wpa_s, Boolean en)
{
    if (wpa_s->driver->start_encrypt_eapol_frame)
        return wpa_s->driver->start_encrypt_eapol_frame(wpa_s, en);
    return WPA_ERR_FAIL;
}

static inline int wpa_drv_install_ptk(struct wpa_supplicant *wpa_s, uint32_t alg, uint32_t key_idx, uint32_t tk1_len)
{
    if (wpa_s->driver->install_ptk)
        return wpa_s->driver->install_ptk(wpa_s, alg, key_idx, tk1_len);
    return WPA_ERR_FAIL;
}

static inline int wpa_drv_install_gtk(struct wpa_supplicant *wpa_s, uint32_t alg, uint32_t key_idx, uint32_t key_len, uint8_t *keys)
{
    if (wpa_s->driver->install_gtk)
        return wpa_s->driver->install_gtk(wpa_s, alg, key_idx, key_len, keys);
    return WPA_ERR_FAIL;
}

#ifdef CONFIG_SAE
static inline int wpa_drv_install_igtk(struct wpa_supplicant *wpa_s, uint32_t alg, uint32_t key_idx, uint32_t key_len, uint8_t *keys, uint8_t *pn)
{
    if (wpa_s->driver->install_igtk)
        return wpa_s->driver->install_igtk(wpa_s, alg, key_idx, key_len, keys, pn);
    return WPA_ERR_FAIL;
}
#endif

static inline int wpa_drv_clear_keys(struct wpa_supplicant *wpa_s)
{
    if (wpa_s->driver->clear_ptk_gtk_keys)
        return wpa_s->driver->clear_ptk_gtk_keys(wpa_s);
    return WPA_ERR_FAIL;
}

static inline int wpa_drv_sta_join_done(struct wpa_supplicant *wpa_s, uint8_t reason)
{
    if (wpa_s->driver->sta_join_done)
        return wpa_s->driver->sta_join_done(wpa_s, reason);
    return WPA_ERR_FAIL;
}

#ifdef CONFIG_SAE
static inline int wpa_drv_sta_sae_tx_data(struct wpa_supplicant *wpa_s, void *buf, uint32_t buf_len)
{
    if (wpa_s->driver->sta_sae_auth_tx_data)
        return wpa_s->driver->sta_sae_auth_tx_data(wpa_s, buf, buf_len);
    return WPA_ERR_FAIL;
}
#endif

#ifdef OPL_PLATFORM_HW_SECURITY
static inline int wpa_drv_hw_hmac_sha_1(uint8_t *sk, int sk_len, uint8_t *in_data, int in_data_len, uint8_t *mac)
{
    if (wpa_s->driver->hw_hmac_sha_1)
        return wpa_s->driver->hw_hmac_sha_1(sk, sk_len, in_data, in_data_len, mac);
    return WPA_ERR_FAIL;
}
#endif

/*************************************************************************/

static inline int wpa_drv_register_scan_done_cb(int (*cb)(void *argu, uint8_t ap_num, int reason))
{
    return wpa_s->driver->register_scan_done_cb(NULL, cb);
}

static inline int wpa_drv_register_add_bss_cb(int (*cb)(void *argu))
{
    return wpa_s->driver->register_add_bss_cb(NULL, cb);
}

#ifdef CONFIG_APS_HANDLE_AUTH_ASSOC
static inline int wpa_drv_register_auth_start_cb(int (*cb)(void *argu, void *buf, uint32_t buf_len))
{
    return wpa_s->driver->register_auth_start_cb(NULL, cb);
}
static inline int wpa_drv_register_rx_mgmt_frame_cb(int (*cb)(void *buf, uint32_t len))
{
    return wpa_s->driver->register_rx_mgmt_frame_cb(NULL, cb);
}
#endif

static inline int wpa_drv_register_probe_start_cb(int (*cb)(void *argu, void *buf, uint32_t buf_len))
{
    return wpa_s->driver->register_probe_start_cb(NULL, cb);
}

static inline int wpa_drv_register_auth_cb(int (*cb)(void *argu, void *buf, uint32_t buf_len))
{
    return wpa_s->driver->register_auth_cb(NULL, cb);
}

static inline int wpa_drv_register_assoc_cb(int (*cb)(void *argu, void *buf, uint32_t buf_len))
{
    return wpa_s->driver->register_assoc_cb(NULL, cb);
}

static inline int wpa_drv_register_disconnect_cb(int (*cb)(void *argu, int reason))
{
    return wpa_s->driver->register_disconnect_cb(NULL, cb);
}

static inline int wpa_drv_register_rx_eapol_frame_cb(int (*cb)(void *buf, uint32_t len))
{
    return wpa_s->driver->register_rx_eapol_frame_cb(NULL, cb);
}

#endif /* DRIVER_I_H */
