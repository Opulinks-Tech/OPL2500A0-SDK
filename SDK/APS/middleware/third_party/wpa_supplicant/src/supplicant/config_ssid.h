/*
 * WPA Supplicant / Network configuration structures
 * Copyright (c) 2003-2013, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef CONFIG_SSID_H
#define CONFIG_SSID_H

#include "utils/includes.h"

#include "utils/common.h"
#include "common/defs.h"
#include "utils/list.h"
#include "eap_peer/eap_config.h"

/**
 * struct wpa_ssid - Network configuration data
 *
 * This structure includes all the configuration variables for a network. This
 * data is included in the per-interface configuration data as an element of
 * the network list, struct wpa_config::ssid. Each network block in the
 * configuration is mapped to a struct wpa_ssid instance.
 */
struct wpa_ssid {
	/**
	 * next - Next network in global list
	 *
	 * This pointer can be used to iterate over all networks. The head of
	 * this list is stored in the ssid field of struct wpa_config.
	 */
	struct wpa_ssid *next;

	/**
	 * pnext - Next network in per-priority list
	 *
	 * This pointer can be used to iterate over all networks in the same
	 * priority class. The heads of these list are stored in the pssid
	 * fields of struct wpa_config.
	 */
	struct wpa_ssid *pnext;

	/**
	 * id - Unique id for the network
	 *
	 * This identifier is used as a unique identifier for each network
	 * block when using the control interface. Each network is allocated an
	 * id when it is being created, either when reading the configuration
	 * file or when a new network is added through the control interface.
	 */
	int id;

	/**
	 * ssid - Service set identifier (network name)
	 *
	 * This is the SSID for the network. For wireless interfaces, this is
	 * used to select which network will be used. If set to %NULL (or
	 * ssid_len=0), any SSID can be used. For wired interfaces, this must
	 * be set to %NULL. Note: SSID may contain any characters, even nul
	 * (ASCII 0) and as such, this should not be assumed to be a nul
	 * terminated string. ssid_len defines how many characters are valid
	 * and the ssid field is not guaranteed to be nul terminated.
	 */
	u8 *ssid;

	/**
	 * ssid_len - Length of the SSID
	 */
	size_t ssid_len;

	/**
	 * bssid - BSSID
	 *
	 * If set, this network block is used only when associating with the AP
	 * using the configured BSSID
	 *
	 * If this is a persistent P2P group (disabled == 2), this is the GO
	 * Device Address.
	 */
	u8 bssid[ETH_ALEN];

	/**
	 * psk - WPA pre-shared key (256 bits)
	 */
	u8 psk[32];

	/**
	 * passphrase - WPA ASCII passphrase
	 *
	 * If this is set, psk will be generated using the SSID and passphrase
	 * configured for the network. ASCII passphrase must be between 8 and
	 * 63 characters (inclusive).
	 */
	char *passphrase;

	/**
	 * sae_password - SAE password
	 *
	 * This parameter can be used to set a password for SAE. By default, the
	 * passphrase value is used if this separate parameter is not used, but
	 * passphrase follows the WPA-PSK constraints (8..63 characters) even
	 * though SAE passwords do not have such constraints.
	 */
	char *sae_password;

	/**
	 * sae_password_id - SAE password identifier
	 *
	 * This parameter can be used to identify a specific SAE password. If
	 * not included, the default SAE password is used instead.
	 */
	char *sae_password_id;

	/**
	 * pairwise_cipher - Bitfield of allowed pairwise ciphers, WPA_CIPHER_*
	 */
	int pairwise_cipher;

	/**
	 * group_cipher - Bitfield of allowed group ciphers, WPA_CIPHER_*
	 */
	int group_cipher;

	/**
	 * group_mgmt_cipher - Bitfield of allowed group management ciphers
	 *
	 * This is a bitfield of WPA_CIPHER_AES_128_CMAC and WPA_CIPHER_BIP_*
	 * values. If 0, no constraint is used for the cipher, i.e., whatever
	 * the AP uses is accepted.
	 */
	int group_mgmt_cipher;

	/**
	 * key_mgmt - Bitfield of allowed key management protocols
	 *
	 * WPA_KEY_MGMT_*
	 */
	int key_mgmt;

	/**
	 * proto - Bitfield of allowed protocols, WPA_PROTO_*
	 */
	int proto;

	/**
	 * auth_alg -  Bitfield of allowed authentication algorithms
	 *
	 * WPA_AUTH_ALG_*
	 */
	int auth_alg;

#ifdef IEEE8021X_EAPOL
#define EAPOL_FLAG_REQUIRE_KEY_UNICAST BIT(0)
#define EAPOL_FLAG_REQUIRE_KEY_BROADCAST BIT(1)
	/**
	 * eapol_flags - Bit field of IEEE 802.1X/EAPOL options (EAPOL_FLAG_*)
	 */
	int eapol_flags;

	/**
	 * eap - EAP peer configuration for this network
	 */
	struct eap_peer_config eap;
#endif /* IEEE8021X_EAPOL */

#ifdef IEEE8021X_EAPOL

	/**
	 * leap - Number of EAP methods using LEAP
	 *
	 * This field should be set to 1 if LEAP is enabled. This is used to
	 * select IEEE 802.11 authentication algorithm.
	 */
	int leap;

	/**
	 * non_leap - Number of EAP methods not using LEAP
	 *
	 * This field should be set to >0 if any EAP method other than LEAP is
	 * enabled. This is used to select IEEE 802.11 authentication
	 * algorithm.
	 */
	int non_leap;

	/**
	 * eap_workaround - EAP workarounds enabled
	 *
	 * wpa_supplicant supports number of "EAP workarounds" to work around
	 * interoperability issues with incorrectly behaving authentication
	 * servers. This is recommended to be enabled by default because some
	 * of the issues are present in large number of authentication servers.
	 *
	 * Strict EAP conformance mode can be configured by disabling
	 * workarounds with eap_workaround = 0.
	 */
	unsigned int eap_workaround;

#endif /* IEEE8021X_EAPOL */

#ifdef CONFIG_IEEE80211W
	/**
	 * ieee80211w - Whether management frame protection is enabled
	 *
	 * This value is used to configure policy for management frame
	 * protection (IEEE 802.11w). 0 = disabled, 1 = optional, 2 = required.
	 * This is disabled by default unless the default value has been changed
	 * with the global pmf=1/2 parameter.
	 *
	 * Internally, special value 3 is used to indicate that the parameter
	 * was not specified in the configuration (i.e., default behavior is
	 * followed).
	 */
	enum mfp_options ieee80211w;
#endif /* CONFIG_IEEE80211W */

	/**
	 * dtim_period - DTIM period in Beacon intervals
	 * By default: 2
	 */
	int dtim_period;

	/**
	 * beacon_int - Beacon interval (default: 100 TU)
	 */
	int beacon_int;
};

#endif /* CONFIG_SSID_H */
