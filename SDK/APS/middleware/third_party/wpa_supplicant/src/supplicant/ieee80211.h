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
 *  @file ieee80211.h
 * 
 *  @brief `
 *  
 ******************************************************************************/
#ifndef _IEEE80211_H_H
#define _IEEE80211_H_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "utils/common.h"

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
int ieee80211_build_sta_deauth_request(struct wpa_supplicant *wpa_s, char *buf, uint32_t buf_len, uint16_t reason);
int ieee80211_build_sta_auth_request(struct wpa_supplicant *wpa_s, char *buf, uint32_t buf_len, int seq, 
                                     uint32_t wlan_auth, char *ext_buf, uint32_t ext_buf_len);
int ieee80211_build_sta_assoc_request(struct wpa_supplicant *wpa_s, char *buf, uint32_t buf_len);
int ieee80211_parse_sta_auth_response(struct wpa_supplicant *wpa_s, char *buf, uint32_t buf_len);
int ieee80211_parse_sta_assoc_response(struct wpa_supplicant *wpa_s, char *buf, uint32_t buf_len);
uint8_t *ieee80211_sta_auth_sae_pos_get(struct wpa_supplicant *wpa_s, char *buf, uint32_t buf_len);
uint32_t ieee80211_sta_auth_sae_len_get(struct wpa_supplicant *wpa_s, char *buf, uint32_t buf_len);

#ifdef __cplusplus
}
#endif
#endif /* _IEEE80211_H_H */
