/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#ifndef _WIFI_MAC_DATA_H_
#define _WIFI_MAC_DATA_H_

#include "utils/wifi_common.h"
#include "ieee80211_crypto.h"


struct sk_buff;
struct sk_buff_head;

s32 rx_data_frame_sim(u8 *pframe, u16 mpdu_len); //for simulator
s32 rx_data_frame_impl(u8 *data, u16 data_len, u8 u8HdrLen);
s32 ieee80211_rx_frame_impl(u8 *data, u16 len, u8 u8HdrLen);

void wifi_mac_rx_reasm_timeout_handle_impl(void *pParam);
void rx_data_reasm_timeout_handle_impl(void const *pArg);
int rx_data_reasm_tmr_start_impl(void);
int rx_data_reasm_tmr_stop_impl(void);
void rx_data_reasm_cb_reset_impl(void);
int rx_data_reasm_impl(ieee80211_key *conf_key, u8 *u8aFrame, u32 u32HdrLen, u16 u16Fc, u16 u16SeqCtrl, u32 *pu32MpduLen, u16 *pu16PlLen, u8 **pu8aReasmFrame);

int wifi_mac_xmit_eapol_frame_impl(u8 *buf, size_t len, const u8 *eth_dest, u16 proto);
u32 wifi_mac_xmit_frame_impl(u8 *tx_data, u32 tx_data_len);
int ieee80211_tx_select_key_impl(ieee80211_key **key, u8 *data, u16 len);
int ieee80211_get_hdrlen_impl(u16 fc);

int wifi_mac_get_encrypt_eapol_frame_impl(u8 *en);
int wifi_mac_set_encrypt_eapol_frame_impl(u8 en);


/* Rom patch interface */
typedef int (*ieee80211_is_eapol_frame_fp_t)(u8 *data, u16 len, u16 hdrlen);
typedef int (*ieee80211_rx_select_key_fp_t)(ieee80211_key **key, u8 *data, u16 len);
typedef s32 (*ieee80211_rx_frame_fp_t)(u8 *data, u16 len, u8 u8HdrLen);
typedef void (*wifi_rx_data_eapol_fp_t)(u8 *buf, u16 len);
typedef void (*rx_data_eapol_fp_t)(const u8 *dst, const u8 *src, const u8 *data, size_t len, int prot);
typedef s32  (*rx_data_frame_fp_t)(u8 *pframe, u16 mpdu_len, u8 u8HdrLen);

typedef u32 (*wifi_mac_xmit_frame_fp_t)(u8 *tx_data, u32 tx_data_len);
typedef int (*wifi_mac_xmit_eapol_frame_fp_t)(u8 *buf, size_t len, const u8 *eth_dest, u16 proto);
typedef int (*ieee80211_tx_select_key_fp_t)(ieee80211_key **key, u8 *data, u16 len);
typedef int (*ieee80211_get_hdrlen_fp_t)(u16 fc);

typedef void (*wifi_mac_rx_reasm_timeout_handle_fp_t)(void *pParam);
typedef void (*rx_data_reasm_timeout_handle_fp_t)(void const *pArg);
typedef int (*rx_data_reasm_tmr_start_fp_t)(void);
typedef int (*rx_data_reasm_tmr_stop_fp_t)(void);
typedef void (*rx_data_reasm_cb_reset_fp_t)(void);
typedef int (*rx_data_reasm_fp_t)(ieee80211_key *conf_key, u8 *u8aFrame, u32 u32HdrLen, u16 u16Fc, u16 u16SeqCtrl, u32 *pu32MpduLen, u16 *pu16PlLen, u8 **pu8aReasmFrame);

typedef int (*wifi_mac_get_encrypt_eapol_frame_fp_t)(u8 *en);
typedef int (*wifi_mac_set_encrypt_eapol_frame_fp_t)(u8 en);

typedef int (*wifi_mac_send_eapol_frame_fp_t)(void *buf, int len);
typedef int (*wifi_mac_set_send_eapol_frame_cb_fp_t)(wifi_mac_send_eapol_frame_fp_t cb);

typedef int (*ieee80211_data_to_8023_fp_t)(struct sk_buff *skb, u16 data_len);
typedef void (*ieee80211_amsdu_to_8023s_fp_t)(struct sk_buff *skb, struct sk_buff_head *list, const unsigned int extra_headroom);
typedef void (*ieee80211_amsdu_decap_fp_t)(struct sk_buff *skb, const unsigned int extra_headroom);
typedef int (*ieee80211_deliver_skb_fp_t)(u8 *msdu, u16 frmlen);



/* Export interface function pointer */
extern ieee80211_is_eapol_frame_fp_t ieee80211_is_eapol_frame;
extern ieee80211_rx_frame_fp_t ieee80211_rx_frame;
extern ieee80211_rx_select_key_fp_t ieee80211_rx_select_key;
extern wifi_rx_data_eapol_fp_t wifi_rx_data_eapol;
extern rx_data_eapol_fp_t rx_data_eapol;
extern rx_data_frame_fp_t rx_data_frame;

extern wifi_mac_xmit_frame_fp_t wifi_mac_xmit_frame;
extern wifi_mac_xmit_eapol_frame_fp_t wifi_mac_xmit_eapol_frame;
extern ieee80211_tx_select_key_fp_t ieee80211_tx_select_key;
extern ieee80211_get_hdrlen_fp_t ieee80211_get_hdrlen;

extern wifi_mac_rx_reasm_timeout_handle_fp_t wifi_mac_rx_reasm_timeout_handle;
extern rx_data_reasm_timeout_handle_fp_t rx_data_reasm_timeout_handle;
extern rx_data_reasm_tmr_start_fp_t rx_data_reasm_tmr_start;
extern rx_data_reasm_tmr_stop_fp_t rx_data_reasm_tmr_stop;
extern rx_data_reasm_cb_reset_fp_t rx_data_reasm_cb_reset;
extern rx_data_reasm_fp_t rx_data_reasm;

extern wifi_mac_get_encrypt_eapol_frame_fp_t wifi_mac_get_encrypt_eapol_frame;
extern wifi_mac_set_encrypt_eapol_frame_fp_t wifi_mac_set_encrypt_eapol_frame;

extern wifi_mac_set_send_eapol_frame_cb_fp_t wifi_mac_set_send_eapol_frame;

extern ieee80211_data_to_8023_fp_t      ieee80211_data_to_8023;
extern ieee80211_amsdu_to_8023s_fp_t    ieee80211_amsdu_to_8023s;
extern ieee80211_amsdu_decap_fp_t       ieee80211_amsdu_decap;
extern ieee80211_deliver_skb_fp_t       ieee80211_deliver_skb;

#include "wifi_mac_data_patch.h"
#endif
