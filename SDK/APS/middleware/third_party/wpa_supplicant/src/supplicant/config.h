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
 *  @file wpa2_config.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "utils/common.h"
#include "config_ssid.h"
#include "common/ieee802_11_defs.h"
#include "common/sae.h"

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
/**
 * struct wpa_config - wpa_supplicant configuration data
 *
 * This data structure is presents the per-interface (radio) configuration
 * data. In many cases, there is only one struct wpa_config instance, but if
 * more than one network interface is being controlled, one instance is used
 * for each.
 */
struct wpa_config {
    /**
     * ssid - Head of the global network list
     *
     * This is the head for the list of all the configured networks.
     */
    struct wpa_ssid *ssid;

    /**
     * pssid - Per-priority network lists (in priority order)
     */
    struct wpa_ssid **pssid;

    uint8_t ieee80211b;
#ifdef CONFIG_IEEE80211G
    uint8_t ieee80211g;
#endif
#ifdef CONFIG_IEEE80211N
    uint8_t ieee80211n;
#endif
#ifdef CONFIG_IEEE80211AX
    uint8_t ieee80211ax;
#endif
    uint8_t short_preamble;
    uint8_t wmm_ie;

    uint32_t auth_proto;            /* refer to def.h */
    uint32_t auth_alg;              /* refer to def.h */
    uint32_t auth_cipher;           /* refer to def.h */
                                    
    uint16_t aid;                   /* STA's unique AID (1 .. 2007) or 0 if not yet assigned */

    /*
     * Capability Info:
     *  .... .... .... ...1 = ESS capabilities: Transmitter is an AP
     *  .... .... .... ..0. = IBSS status: Transmitter belongs to a BSS
     *  .... ..0. .... 00.. = CFP participation capabilities: No point coordinator at AP (0x00)
     *  .... .... ...0 .... = Privacy: AP/STA cannot support WEP
     *  .... .... ..0. .... = Short Preamble: Not Allowed
     *  .... .... .0.. .... = PBCC: Not Allowed
     *  .... .... 0... .... = Channel Agility: Not in use
     *  .... ...0 .... .... = Spectrum Management: Implemented
     *  .... .0.. .... .... = Short Slot Time: Not in use
     *  .... 0... .... .... = Automatic Power Save Delivery: Not Implemented
     *  ...0 .... .... .... = Radio Measurement: Implemented
     *  ..0. .... .... .... = DSSS-OFDM: Not Allowed
     *  .0.. .... .... .... = Delayed Block Ack: Not Implemented
     *  0... .... .... .... = Immediate Block Ack: Not Implemented
     */
    uint16_t capability;
    uint16_t listen_interval;
    uint8_t  supported_rates[32];   /* Maximum number of supported rates (from both Supported Rates and Extended
                                     * Supported Rates IEs).
                                     */
    uint32_t supported_rates_len;

    uint8_t oui[3];

#ifdef CONFIG_IEEE80211N
    struct ieee80211_ht_capabilities *ht_capabilities;
#endif

#ifdef CONFIG_IEEE80211AX
    struct ieee80211_he_capabilities *he_capab;
    uint32_t he_capab_len;
#endif

    /**
     * country - Country code
     *
     * This is the ISO/IEC alpha2 country code for which we are operating
     * in
     */
    char country[2];
    uint8_t start_channel;
    uint8_t total_channel;
    int8_t max_tx_power;

    /**
     * sae_groups - Preference list of enabled groups for SAE
     *
     * By default (if this parameter is not set), the mandatory group 19
     * (ECC group defined over a 256-bit prime order field) is preferred,
     * but other groups are also enabled. If this parameter is set, the
     * groups will be tried in the indicated order.
     */
    int *sae_groups;
};

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
void wpa_config_free_ssid(struct wpa_ssid *ssid);
void wpa_config_clear_ssid(struct wpa_ssid *ssid);
int wpa2_config_init(void *argu);
int wpa2_config_deinit(void *argu);
int wpa2_config_set_default(void *argu);

#ifdef __cplusplus
}
#endif
#endif /* _CONFIG_H_ */
