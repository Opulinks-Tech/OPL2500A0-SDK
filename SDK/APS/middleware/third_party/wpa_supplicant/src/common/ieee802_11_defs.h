/*
 * IEEE 802.11 Frame type definitions
 * Copyright (c) 2002-2009, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2007-2008 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef IEEE802_11_DEFS_H
#define IEEE802_11_DEFS_H

/* IEEE 802.11 defines */

#define WLAN_FC_PVER		0x0003
#define WLAN_FC_TODS		0x0100
#define WLAN_FC_FROMDS		0x0200
#define WLAN_FC_MOREFRAG	0x0400
#define WLAN_FC_RETRY		0x0800
#define WLAN_FC_PWRMGT		0x1000
#define WLAN_FC_MOREDATA	0x2000
#define WLAN_FC_ISWEP		0x4000
#define WLAN_FC_ORDER		0x8000

#define WLAN_FC_GET_TYPE(fc)	(((fc) & 0x000c) >> 2)
#define WLAN_FC_GET_STYPE(fc)	(((fc) & 0x00f0) >> 4)

#define WLAN_GET_SEQ_FRAG(seq) ((seq) & (BIT(3) | BIT(2) | BIT(1) | BIT(0)))
#define WLAN_GET_SEQ_SEQ(seq) \
	(((seq) & (~(BIT(3) | BIT(2) | BIT(1) | BIT(0)))) >> 4)

#define WLAN_FC_TYPE_MGMT		0
#define WLAN_FC_TYPE_CTRL		1
#define WLAN_FC_TYPE_DATA		2

/* management */
#define WLAN_FC_STYPE_ASSOC_REQ		0
#define WLAN_FC_STYPE_ASSOC_RESP	1
#define WLAN_FC_STYPE_REASSOC_REQ	2
#define WLAN_FC_STYPE_REASSOC_RESP	3
#define WLAN_FC_STYPE_PROBE_REQ		4
#define WLAN_FC_STYPE_PROBE_RESP	5
#define WLAN_FC_STYPE_BEACON		8
#define WLAN_FC_STYPE_ATIM		9
#define WLAN_FC_STYPE_DISASSOC		10
#define WLAN_FC_STYPE_AUTH		11
#define WLAN_FC_STYPE_DEAUTH		12
#define WLAN_FC_STYPE_ACTION		13

/* control */
#define WLAN_FC_STYPE_PSPOLL		10
#define WLAN_FC_STYPE_RTS		11
#define WLAN_FC_STYPE_CTS		12
#define WLAN_FC_STYPE_ACK		13
#define WLAN_FC_STYPE_CFEND		14
#define WLAN_FC_STYPE_CFENDACK		15

/* data */
#define WLAN_FC_STYPE_DATA		0
#define WLAN_FC_STYPE_DATA_CFACK	1
#define WLAN_FC_STYPE_DATA_CFPOLL	2
#define WLAN_FC_STYPE_DATA_CFACKPOLL	3
#define WLAN_FC_STYPE_NULLFUNC		4
#define WLAN_FC_STYPE_CFACK		5
#define WLAN_FC_STYPE_CFPOLL		6
#define WLAN_FC_STYPE_CFACKPOLL		7
#define WLAN_FC_STYPE_QOS_DATA		8

/* Authentication algorithms */
#define WLAN_AUTH_OPEN			0
#define WLAN_AUTH_SHARED_KEY		1
#define WLAN_AUTH_FT			2
#define WLAN_AUTH_SAE			3
#define WLAN_AUTH_FILS_SK		4
#define WLAN_AUTH_FILS_SK_PFS		5
#define WLAN_AUTH_FILS_PK		6
#define WLAN_AUTH_LEAP			128

#define WLAN_AUTH_CHALLENGE_LEN 128

#define WLAN_CAPABILITY_ESS BIT(0)
#define WLAN_CAPABILITY_IBSS BIT(1)
#define WLAN_CAPABILITY_CF_POLLABLE BIT(2)
#define WLAN_CAPABILITY_CF_POLL_REQUEST BIT(3)
#define WLAN_CAPABILITY_PRIVACY BIT(4)
#define WLAN_CAPABILITY_SHORT_PREAMBLE BIT(5)
#define WLAN_CAPABILITY_PBCC BIT(6)
#define WLAN_CAPABILITY_CHANNEL_AGILITY BIT(7)
#define WLAN_CAPABILITY_SPECTRUM_MGMT BIT(8)
#define WLAN_CAPABILITY_SHORT_SLOT_TIME BIT(10)
#define WLAN_CAPABILITY_DSSS_OFDM BIT(13)

/* Status codes (IEEE 802.11-2007, 7.3.1.9, Table 7-23) */
#define WLAN_STATUS_SUCCESS 0
#define WLAN_STATUS_UNSPECIFIED_FAILURE 1
#define WLAN_STATUS_CAPS_UNSUPPORTED 10
#define WLAN_STATUS_REASSOC_NO_ASSOC 11
#define WLAN_STATUS_ASSOC_DENIED_UNSPEC 12
#define WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG 13
#define WLAN_STATUS_UNKNOWN_AUTH_TRANSACTION 14
#define WLAN_STATUS_CHALLENGE_FAIL 15
#define WLAN_STATUS_AUTH_TIMEOUT 16
#define WLAN_STATUS_AP_UNABLE_TO_HANDLE_NEW_STA 17
#define WLAN_STATUS_ASSOC_DENIED_RATES 18
/* IEEE 802.11b */
#define WLAN_STATUS_ASSOC_DENIED_NOSHORT 19
#define WLAN_STATUS_ASSOC_DENIED_NOPBCC 20
#define WLAN_STATUS_ASSOC_DENIED_NOAGILITY 21
/* IEEE 802.11h */
#define WLAN_STATUS_SPEC_MGMT_REQUIRED 22
#define WLAN_STATUS_PWR_CAPABILITY_NOT_VALID 23
#define WLAN_STATUS_SUPPORTED_CHANNEL_NOT_VALID 24
/* IEEE 802.11g */
#define WLAN_STATUS_ASSOC_DENIED_NO_SHORT_SLOT_TIME 25
#define WLAN_STATUS_ASSOC_DENIED_NO_ER_PBCC 26
#define WLAN_STATUS_ASSOC_DENIED_NO_DSSS_OFDM 27
#define WLAN_STATUS_R0KH_UNREACHABLE 28
/* IEEE 802.11w */
#define WLAN_STATUS_ASSOC_REJECTED_TEMPORARILY 30
#define WLAN_STATUS_ROBUST_MGMT_FRAME_POLICY_VIOLATION 31
#define WLAN_STATUS_UNSPECIFIED_QOS_FAILURE 32
#define WLAN_STATUS_REQUEST_DECLINED 37
#define WLAN_STATUS_INVALID_PARAMETERS 38
/* IEEE 802.11i */
#define WLAN_STATUS_INVALID_IE 40
#define WLAN_STATUS_GROUP_CIPHER_NOT_VALID 41
#define WLAN_STATUS_PAIRWISE_CIPHER_NOT_VALID 42
#define WLAN_STATUS_AKMP_NOT_VALID 43
#define WLAN_STATUS_UNSUPPORTED_RSN_IE_VERSION 44
#define WLAN_STATUS_INVALID_RSN_IE_CAPAB 45
#define WLAN_STATUS_CIPHER_REJECTED_PER_POLICY 46
#define WLAN_STATUS_TS_NOT_CREATED 47
#define WLAN_STATUS_DIRECT_LINK_NOT_ALLOWED 48
#define WLAN_STATUS_DEST_STA_NOT_PRESENT 49
#define WLAN_STATUS_DEST_STA_NOT_QOS_STA 50
#define WLAN_STATUS_ASSOC_DENIED_LISTEN_INT_TOO_LARGE 51
/* IEEE 802.11r */
#define WLAN_STATUS_INVALID_FT_ACTION_FRAME_COUNT 52
#define WLAN_STATUS_INVALID_PMKID 53
#define WLAN_STATUS_INVALID_MDIE 54
#define WLAN_STATUS_INVALID_FTIE 55
#define WLAN_STATUS_GAS_ADV_PROTO_NOT_SUPPORTED 59
#define WLAN_STATUS_NO_OUTSTANDING_GAS_REQ 60
#define WLAN_STATUS_GAS_RESP_NOT_RECEIVED 61
#define WLAN_STATUS_STA_TIMED_OUT_WAITING_FOR_GAS_RESP 62
#define WLAN_STATUS_GAS_RESP_LARGER_THAN_LIMIT 63
#define WLAN_STATUS_REQ_REFUSED_HOME 64
#define WLAN_STATUS_ADV_SRV_UNREACHABLE 65
#define WLAN_STATUS_REQ_REFUSED_SSPN 67
#define WLAN_STATUS_REQ_REFUSED_UNAUTH_ACCESS 68
#define WLAN_STATUS_INVALID_RSNIE 72
#define WLAN_STATUS_ANTI_CLOGGING_TOKEN_REQ 76
#define WLAN_STATUS_FINITE_CYCLIC_GROUP_NOT_SUPPORTED 77
#define WLAN_STATUS_TRANSMISSION_FAILURE 79
#define WLAN_STATUS_REJECTED_WITH_SUGGESTED_BSS_TRANSITION 82
#define WLAN_STATUS_PENDING_ADMITTING_FST_SESSION 86
#define WLAN_STATUS_QUERY_RESP_OUTSTANDING 95
#define WLAN_STATUS_DENIED_WITH_SUGGESTED_BAND_AND_CHANNEL 99
#define WLAN_STATUS_ASSOC_DENIED_NO_VHT 104
#define WLAN_STATUS_UNKNOWN_PASSWORD_IDENTIFIER 123

/* Reason codes (IEEE 802.11-2007, 7.3.1.7, Table 7-22) */
#define WLAN_REASON_UNSPECIFIED 1
#define WLAN_REASON_PREV_AUTH_NOT_VALID 2
#define WLAN_REASON_DEAUTH_LEAVING 3
#define WLAN_REASON_DISASSOC_DUE_TO_INACTIVITY 4
#define WLAN_REASON_DISASSOC_AP_BUSY 5
#define WLAN_REASON_CLASS2_FRAME_FROM_NONAUTH_STA 6
#define WLAN_REASON_CLASS3_FRAME_FROM_NONASSOC_STA 7
#define WLAN_REASON_DISASSOC_STA_HAS_LEFT 8
#define WLAN_REASON_STA_REQ_ASSOC_WITHOUT_AUTH 9
/* IEEE 802.11h */
#define WLAN_REASON_PWR_CAPABILITY_NOT_VALID 10
#define WLAN_REASON_SUPPORTED_CHANNEL_NOT_VALID 11
/* IEEE 802.11i */
#define WLAN_REASON_INVALID_IE 13
#define WLAN_REASON_MICHAEL_MIC_FAILURE 14
#define WLAN_REASON_4WAY_HANDSHAKE_TIMEOUT 15
#define WLAN_REASON_GROUP_KEY_UPDATE_TIMEOUT 16
#define WLAN_REASON_IE_IN_4WAY_DIFFERS 17
#define WLAN_REASON_GROUP_CIPHER_NOT_VALID 18
#define WLAN_REASON_PAIRWISE_CIPHER_NOT_VALID 19
#define WLAN_REASON_AKMP_NOT_VALID 20
#define WLAN_REASON_UNSUPPORTED_RSN_IE_VERSION 21
#define WLAN_REASON_INVALID_RSN_IE_CAPAB 22
#define WLAN_REASON_IEEE_802_1X_AUTH_FAILED 23
#define WLAN_REASON_CIPHER_SUITE_REJECTED 24


/* Information Element IDs (IEEE Std 802.11-2016, 9.4.2.1, Table 9-77) */
#define WLAN_EID_SSID 0
#define WLAN_EID_SUPP_RATES 1
#define WLAN_EID_DS_PARAMS 3
#define WLAN_EID_CF_PARAMS 4
#define WLAN_EID_TIM 5
#define WLAN_EID_IBSS_PARAMS 6
#define WLAN_EID_COUNTRY 7
#define WLAN_EID_REQUEST 10
#define WLAN_EID_BSS_LOAD 11
#define WLAN_EID_EDCA_PARAM_SET 12
#define WLAN_EID_TSPEC 13
#define WLAN_EID_TCLAS 14
#define WLAN_EID_SCHEDULE 15
#define WLAN_EID_CHALLENGE 16
#define WLAN_EID_PWR_CONSTRAINT 32
#define WLAN_EID_PWR_CAPABILITY 33
#define WLAN_EID_TPC_REQUEST 34
#define WLAN_EID_TPC_REPORT 35
#define WLAN_EID_SUPPORTED_CHANNELS 36
#define WLAN_EID_CHANNEL_SWITCH 37
#define WLAN_EID_MEASURE_REQUEST 38
#define WLAN_EID_MEASURE_REPORT 39
#define WLAN_EID_QUIET 40
#define WLAN_EID_IBSS_DFS 41
#define WLAN_EID_ERP_INFO 42
#define WLAN_EID_TS_DELAY 43
#define WLAN_EID_TCLAS_PROCESSING 44
#define WLAN_EID_HT_CAP 45
#define WLAN_EID_QOS 46
#define WLAN_EID_RSN 48
#define WLAN_EID_EXT_SUPP_RATES 50
#define WLAN_EID_AP_CHANNEL_REPORT 51
#define WLAN_EID_NEIGHBOR_REPORT 52
#define WLAN_EID_RCPI 53
#define WLAN_EID_MOBILITY_DOMAIN 54
#define WLAN_EID_FAST_BSS_TRANSITION 55
#define WLAN_EID_TIMEOUT_INTERVAL 56
#define WLAN_EID_RIC_DATA 57
#define WLAN_EID_DSE_REGISTERED_LOCATION 58
#define WLAN_EID_SUPPORTED_OPERATING_CLASSES 59
#define WLAN_EID_EXT_CHANSWITCH_ANN 60
#define WLAN_EID_HT_OPERATION 61
#define WLAN_EID_SECONDARY_CHANNEL_OFFSET 62
#define WLAN_EID_BSS_AVERAGE_ACCESS_DELAY 63
#define WLAN_EID_ANTENNA 64
#define WLAN_EID_RSNI 65
#define WLAN_EID_MEASUREMENT_PILOT_TRANSMISSION 66
#define WLAN_EID_BSS_AVAILABLE_ADM_CAPA 67
#define WLAN_EID_BSS_AC_ACCESS_DELAY 68 /* note: also used by WAPI */
#define WLAN_EID_TIME_ADVERTISEMENT 69
#define WLAN_EID_RRM_ENABLED_CAPABILITIES 70
#define WLAN_EID_MULTIPLE_BSSID 71
#define WLAN_EID_20_40_BSS_COEXISTENCE 72
#define WLAN_EID_20_40_BSS_INTOLERANT 73
#define WLAN_EID_OVERLAPPING_BSS_SCAN_PARAMS 74
#define WLAN_EID_RIC_DESCRIPTOR 75
#define WLAN_EID_MMIE 76
#define WLAN_EID_EVENT_REQUEST 78
#define WLAN_EID_EVENT_REPORT 79
#define WLAN_EID_DIAGNOSTIC_REQUEST 80
#define WLAN_EID_DIAGNOSTIC_REPORT 81
#define WLAN_EID_LOCATION_PARAMETERS 82
#define WLAN_EID_NONTRANSMITTED_BSSID_CAPA 83
#define WLAN_EID_SSID_LIST 84
#define WLAN_EID_MULTIPLE_BSSID_INDEX 85
#define WLAN_EID_FMS_DESCRIPTOR 86
#define WLAN_EID_FMS_REQUEST 87
#define WLAN_EID_FMS_RESPONSE 88
#define WLAN_EID_QOS_TRAFFIC_CAPABILITY 89
#define WLAN_EID_BSS_MAX_IDLE_PERIOD 90
#define WLAN_EID_TFS_REQ 91
#define WLAN_EID_TFS_RESP 92
#define WLAN_EID_WNMSLEEP 93
#define WLAN_EID_TIM_BROADCAST_REQUEST 94
#define WLAN_EID_TIM_BROADCAST_RESPONSE 95
#define WLAN_EID_COLLOCATED_INTERFERENCE_REPORT 96
#define WLAN_EID_CHANNEL_USAGE 97
#define WLAN_EID_TIME_ZONE 98
#define WLAN_EID_DMS_REQUEST 99
#define WLAN_EID_DMS_RESPONSE 100
#define WLAN_EID_LINK_ID 101
#define WLAN_EID_WAKEUP_SCHEDULE 102
#define WLAN_EID_CHANNEL_SWITCH_TIMING 104
#define WLAN_EID_PTI_CONTROL 105
#define WLAN_EID_TPU_BUFFER_STATUS 106
#define WLAN_EID_INTERWORKING 107
#define WLAN_EID_ADV_PROTO 108
#define WLAN_EID_EXPEDITED_BANDWIDTH_REQ 109
#define WLAN_EID_QOS_MAP_SET 110
#define WLAN_EID_ROAMING_CONSORTIUM 111
#define WLAN_EID_EMERGENCY_ALERT_ID 112
#define WLAN_EID_MESH_CONFIG 113
#define WLAN_EID_MESH_ID 114
#define WLAN_EID_MESH_LINK_METRIC_REPORT 115
#define WLAN_EID_CONGESTION_NOTIFICATION 116
#define WLAN_EID_PEER_MGMT 117
#define WLAN_EID_MESH_CHANNEL_SWITCH_PARAMETERS 118
#define WLAN_EID_MESH_AWAKE_WINDOW 119
#define WLAN_EID_BEACON_TIMING 120
#define WLAN_EID_MCCAOP_SETUP_REQUEST 121
#define WLAN_EID_MCCAOP_SETUP_REPLY 122
#define WLAN_EID_MCCAOP_ADVERTISEMENT 123
#define WLAN_EID_MCCAOP_TEARDOWN 124
#define WLAN_EID_GANN 125
#define WLAN_EID_RANN 126
#define WLAN_EID_EXT_CAPAB 127
#define WLAN_EID_PREQ 130
#define WLAN_EID_PREP 131
#define WLAN_EID_PERR 132
#define WLAN_EID_PXU 137
#define WLAN_EID_PXUC 138
#define WLAN_EID_AMPE 139
#define WLAN_EID_MIC 140
#define WLAN_EID_DESTINATION_URI 141
#define WLAN_EID_U_APSD_COEX 142
#define WLAN_EID_DMG_WAKEUP_SCHEDULE 143
#define WLAN_EID_EXTENDED_SCHEDULE 144
#define WLAN_EID_STA_AVAILABILITY 145
#define WLAN_EID_DMG_TSPEC 146
#define WLAN_EID_NEXT_DMG_ATI 147
#define WLAN_EID_DMG_CAPABILITIES 148
#define WLAN_EID_DMG_OPERATION 151
#define WLAN_EID_DMG_BSS_PARAMETER_CHANGE 152
#define WLAN_EID_DMG_BEAM_REFINEMENT 153
#define WLAN_EID_CHANNEL_MEASUREMENT_FEEDBACK 154
#define WLAN_EID_CCKM 156
#define WLAN_EID_AWAKE_WINDOW 157
#define WLAN_EID_MULTI_BAND 158
#define WLAN_EID_ADDBA_EXTENSION 159
#define WLAN_EID_NEXTPCP_LIST 160
#define WLAN_EID_PCP_HANDOVER 161
#define WLAN_EID_DMG_LINK_MARGIN 162
#define WLAN_EID_SWITCHING_STREAM 163
#define WLAN_EID_SESSION_TRANSITION 164
#define WLAN_EID_DYNAMIC_TONE_PAIRING_REPORT 165
#define WLAN_EID_CLUSTER_REPORT 166
#define WLAN_EID_REPLAY_CAPABILITIES 167
#define WLAN_EID_RELAY_TRANSFER_PARAM_SET 168
#define WLAN_EID_BEAMLINK_MAINTENANCE 169
#define WLAN_EID_MULTIPLE_MAC_SUBLAYERS 170
#define WLAN_EID_U_PID 171
#define WLAN_EID_DMG_LINK_ADAPTATION_ACK 172
#define WLAN_EID_MCCAOP_ADVERTISEMENT_OVERVIEW 174
#define WLAN_EID_QUIET_PERIOD_REQUEST 175
#define WLAN_EID_QUIET_PERIOD_RESPONSE 177
#define WLAN_EID_QMF_POLICY 181
#define WLAN_EID_ECAPC_POLICY 182
#define WLAN_EID_CLUSTER_TIME_OFFSET 183
#define WLAN_EID_INTRA_ACCESS_CATEGORY_PRIORITY 184
#define WLAN_EID_SCS_DESCRIPTOR 185
#define WLAN_EID_QLOAD_REPORT 186
#define WLAN_EID_HCCA_TXOP_UPDATE_COUNT 187
#define WLAN_EID_HIGHER_LAYER_STREAM_ID 188
#define WLAN_EID_GCR_GROUP_ADDRESS 189
#define WLAN_EID_ANTENNA_SECTOR_ID_PATTERN 190
#define WLAN_EID_VHT_CAP 191
#define WLAN_EID_VHT_OPERATION 192
#define WLAN_EID_VHT_EXTENDED_BSS_LOAD 193
#define WLAN_EID_VHT_WIDE_BW_CHSWITCH  194
#define WLAN_EID_VHT_TRANSMIT_POWER_ENVELOPE 195
#define WLAN_EID_VHT_CHANNEL_SWITCH_WRAPPER 196
#define WLAN_EID_VHT_AID 197
#define WLAN_EID_VHT_QUIET_CHANNEL 198
#define WLAN_EID_VHT_OPERATING_MODE_NOTIFICATION 199
#define WLAN_EID_UPSIM 200
#define WLAN_EID_REDUCED_NEIGHBOR_REPORT 201
#define WLAN_EID_TVHT_OPERATION 202
#define WLAN_EID_DEVICE_LOCATION 204
#define WLAN_EID_WHITE_SPACE_MAP 205
#define WLAN_EID_FTM_PARAMETERS 206
#define WLAN_EID_VENDOR_SPECIFIC 221
#define WLAN_EID_CAG_NUMBER 237
#define WLAN_EID_AP_CSN 239
#define WLAN_EID_FILS_INDICATION 240
#define WLAN_EID_DILS 241
#define WLAN_EID_FRAGMENT 242
#define WLAN_EID_EXTENSION 255

/* Element ID Extension (EID 255) values */
#define WLAN_EID_EXT_ASSOC_DELAY_INFO 1
#define WLAN_EID_EXT_FILS_REQ_PARAMS 2
#define WLAN_EID_EXT_FILS_KEY_CONFIRM 3
#define WLAN_EID_EXT_FILS_SESSION 4
#define WLAN_EID_EXT_FILS_HLP_CONTAINER 5
#define WLAN_EID_EXT_FILS_IP_ADDR_ASSIGN 6
#define WLAN_EID_EXT_KEY_DELIVERY 7
#define WLAN_EID_EXT_FILS_WRAPPED_DATA 8
#define WLAN_EID_EXT_FTM_SYNC_INFO 9
#define WLAN_EID_EXT_EXTENDED_REQUEST 10
#define WLAN_EID_EXT_ESTIMATED_SERVICE_PARAMS 11
#define WLAN_EID_EXT_FILS_PUBLIC_KEY 12
#define WLAN_EID_EXT_FILS_NONCE 13
#define WLAN_EID_EXT_FUTURE_CHANNEL_GUIDANCE 14
#define WLAN_EID_EXT_OWE_DH_PARAM 32
#define WLAN_EID_EXT_PASSWORD_IDENTIFIER 33
#define WLAN_EID_EXT_HE_CAPABILITIES 35
#define WLAN_EID_EXT_HE_OPERATION 36
#define WLAN_EID_EXT_HE_MU_EDCA_PARAMS 38
#define WLAN_EID_EXT_SPATIAL_REUSE 39
#define WLAN_EID_EXT_OCV_OCI 54


/* Action frame categories (IEEE 802.11-2007, 7.3.1.11, Table 7-24) */
#define WLAN_ACTION_SPECTRUM_MGMT 0
#define WLAN_ACTION_QOS 1
#define WLAN_ACTION_DLS 2
#define WLAN_ACTION_BLOCK_ACK 3
#define WLAN_ACTION_PUBLIC 4
#define WLAN_ACTION_RADIO_MEASUREMENT 5
#define WLAN_ACTION_FT 6
#define WLAN_ACTION_HT 7
#define WLAN_ACTION_SA_QUERY 8
#define WLAN_ACTION_WMM 17 /* WMM Specification 1.1 */

/* SA Query Action frame (IEEE 802.11w/D8.0, 7.4.9) */
#define WLAN_SA_QUERY_REQUEST 0
#define WLAN_SA_QUERY_RESPONSE 1

#define WLAN_SA_QUERY_TR_ID_LEN 2

/* Timeout Interval Type */
#define WLAN_TIMEOUT_REASSOC_DEADLINE 1
#define WLAN_TIMEOUT_KEY_LIFETIME 2
#define WLAN_TIMEOUT_ASSOC_COMEBACK 3


#ifdef _MSC_VER
#pragma pack(push, 1)
#endif /* _MSC_VER */

struct ieee80211_hdr {
	le16 frame_control;
	le16 duration_id;
	u8 addr1[6];
	u8 addr2[6];
	u8 addr3[6];
	le16 seq_ctrl;
	/* followed by 'u8 addr4[6];' if ToDS and FromDS is set in data frame
	 */
} STRUCT_PACKED;

#define IEEE80211_DA_FROMDS addr1
#define IEEE80211_BSSID_FROMDS addr2
#define IEEE80211_SA_FROMDS addr3

#define IEEE80211_HDRLEN (sizeof(struct ieee80211_hdr))

#define IEEE80211_FC(type, stype) host_to_le16((type << 2) | (stype << 4))

struct ieee80211_mgmt {
	le16 frame_control;
	le16 duration;
	u8 da[6];
	u8 sa[6];
	u8 bssid[6];
	le16 seq_ctrl;
	union {
		struct {
			le16 auth_alg;
			le16 auth_transaction;
			le16 status_code;
			/* possibly followed by Challenge text */
			u8 variable[];
		} STRUCT_PACKED auth;
		struct {
			le16 reason_code;
		} STRUCT_PACKED deauth;
		struct {
			le16 capab_info;
			le16 listen_interval;
			/* followed by SSID and Supported rates */
			u8 variable[];
		} STRUCT_PACKED assoc_req;
		struct {
			le16 capab_info;
			le16 status_code;
			le16 aid;
			/* followed by Supported rates */
			u8 variable[];
		} STRUCT_PACKED assoc_resp, reassoc_resp;
		struct {
			le16 capab_info;
			le16 listen_interval;
			u8 current_ap[6];
			/* followed by SSID and Supported rates */
			u8 variable[];
		} STRUCT_PACKED reassoc_req;
		struct {
			le16 reason_code;
		} STRUCT_PACKED disassoc;
		struct {
			u8 timestamp[8];
			le16 beacon_int;
			le16 capab_info;
			/* followed by some of SSID, Supported rates,
			 * FH Params, DS Params, CF Params, IBSS Params, TIM */
			u8 variable[];
		} STRUCT_PACKED beacon;
		struct {
			/* only variable items: SSID, Supported rates */
			u8 variable;
		} STRUCT_PACKED probe_req;
		struct {
			u8 timestamp[8];
			le16 beacon_int;
			le16 capab_info;
			/* followed by some of SSID, Supported rates,
			 * FH Params, DS Params, CF Params, IBSS Params */
			u8 variable[];
		} STRUCT_PACKED probe_resp;
		struct {
			u8 category;
			union {
				struct {
					u8 action_code;
					u8 dialog_token;
					u8 status_code;
					u8 variable[];
				} STRUCT_PACKED wmm_action;
				struct{
					u8 action_code;
					u8 element_id;
					u8 length;
					u8 switch_mode;
					u8 new_chan;
					u8 switch_count;
				} STRUCT_PACKED chan_switch;
				struct {
					u8 action;
					u8 sta_addr[ETH_ALEN];
					u8 target_ap_addr[ETH_ALEN];
					u8 variable[]; /* FT Request */
				} STRUCT_PACKED ft_action_req;
				struct {
					u8 action;
					u8 sta_addr[ETH_ALEN];
					u8 target_ap_addr[ETH_ALEN];
					le16 status_code;
					u8 variable[]; /* FT Request */
				} STRUCT_PACKED ft_action_resp;
				struct {
					u8 action;
					u8 trans_id[WLAN_SA_QUERY_TR_ID_LEN];
				} STRUCT_PACKED sa_query_req;
				struct {
					u8 action; /* */
					u8 trans_id[WLAN_SA_QUERY_TR_ID_LEN];
				} STRUCT_PACKED sa_query_resp;
			} u;
		} STRUCT_PACKED action;
	} u;
} STRUCT_PACKED;


struct ieee80211_ht_capabilities {
	le16 ht_capabilities_info;
	u8 a_mpdu_params;
	u8 supported_mcs_set[16];
	le16 ht_extended_capabilities;
	le32 tx_bf_capability_info;
	u8 asel_capabilities;
} STRUCT_PACKED;


struct ieee80211_ht_operation {
	u8 control_chan;
	u8 ht_param;
	le16 operation_mode;
	le16 stbc_param;
	u8 basic_set[16];
} STRUCT_PACKED;

struct ieee80211_vht_capabilities {
	le32 vht_capabilities_info;
	struct {
		le16 rx_map;
		le16 rx_highest;
		le16 tx_map;
		le16 tx_highest;
	} vht_supported_mcs_set;
} STRUCT_PACKED;

struct ieee80211_vht_operation {
	u8 vht_op_info_chwidth;
	u8 vht_op_info_chan_center_freq_seg0_idx;
	u8 vht_op_info_chan_center_freq_seg1_idx;
	le16 vht_basic_mcs_set;
} STRUCT_PACKED;

#define FILS_NONCE_LEN 16
#define FILS_SESSION_LEN 8
#define FILS_CACHE_ID_LEN 2
#define FILS_MAX_KEY_AUTH_LEN 48

#ifdef _MSC_VER
#pragma pack(pop)
#endif /* _MSC_VER */

#define ERP_INFO_NON_ERP_PRESENT BIT(0)
#define ERP_INFO_USE_PROTECTION BIT(1)
#define ERP_INFO_BARKER_PREAMBLE_MODE BIT(2)


#define HT_CAP_INFO_LDPC_CODING_CAP		((u16) BIT(0))
#define HT_CAP_INFO_SUPP_CHANNEL_WIDTH_SET	((u16) BIT(1))
#define HT_CAP_INFO_SMPS_MASK			((u16) (BIT(2) | BIT(3)))
#define HT_CAP_INFO_SMPS_STATIC			((u16) 0)
#define HT_CAP_INFO_SMPS_DYNAMIC		((u16) BIT(2))
#define HT_CAP_INFO_SMPS_DISABLED		((u16) (BIT(2) | BIT(3)))
#define HT_CAP_INFO_GREEN_FIELD			((u16) BIT(4))
#define HT_CAP_INFO_SHORT_GI20MHZ		((u16) BIT(5))
#define HT_CAP_INFO_SHORT_GI40MHZ		((u16) BIT(6))
#define HT_CAP_INFO_TX_STBC			((u16) BIT(7))
#define HT_CAP_INFO_RX_STBC_MASK		((u16) (BIT(8) | BIT(9)))
#define HT_CAP_INFO_RX_STBC_1			((u16) BIT(8))
#define HT_CAP_INFO_RX_STBC_12			((u16) BIT(9))
#define HT_CAP_INFO_RX_STBC_123			((u16) (BIT(8) | BIT(9)))
#define HT_CAP_INFO_DELAYED_BA			((u16) BIT(10))
#define HT_CAP_INFO_MAX_AMSDU_SIZE		((u16) BIT(11))
#define HT_CAP_INFO_DSSS_CCK40MHZ		((u16) BIT(12))
#define HT_CAP_INFO_PSMP_SUPP			((u16) BIT(13))
#define HT_CAP_INFO_40MHZ_INTOLERANT		((u16) BIT(14))
#define HT_CAP_INFO_LSIG_TXOP_PROTECT_SUPPORT	((u16) BIT(15))


#define EXT_HT_CAP_INFO_PCO			((u16) BIT(0))
#define EXT_HT_CAP_INFO_TRANS_TIME_OFFSET	1
#define EXT_HT_CAP_INFO_MCS_FEEDBACK_OFFSET	8
#define EXT_HT_CAP_INFO_HTC_SUPPORTED		((u16) BIT(10))
#define EXT_HT_CAP_INFO_RD_RESPONDER		((u16) BIT(11))


#define TX_BEAMFORM_CAP_TXBF_CAP ((u32) BIT(0))
#define TX_BEAMFORM_CAP_RX_STAGGERED_SOUNDING_CAP ((u32) BIT(1))
#define TX_BEAMFORM_CAP_TX_STAGGERED_SOUNDING_CAP ((u32) BIT(2))
#define TX_BEAMFORM_CAP_RX_ZLF_CAP ((u32) BIT(3))
#define TX_BEAMFORM_CAP_TX_ZLF_CAP ((u32) BIT(4))
#define TX_BEAMFORM_CAP_IMPLICIT_ZLF_CAP ((u32) BIT(5))
#define TX_BEAMFORM_CAP_CALIB_OFFSET 6
#define TX_BEAMFORM_CAP_EXPLICIT_CSI_TXBF_CAP ((u32) BIT(8))
#define TX_BEAMFORM_CAP_EXPLICIT_UNCOMPR_STEERING_MATRIX_CAP ((u32) BIT(9))
#define TX_BEAMFORM_CAP_EXPLICIT_BF_CSI_FEEDBACK_CAP ((u32) BIT(10))
#define TX_BEAMFORM_CAP_EXPLICIT_BF_CSI_FEEDBACK_OFFSET 11
#define TX_BEAMFORM_CAP_EXPLICIT_UNCOMPR_STEERING_MATRIX_FEEDBACK_OFFSET 13
#define TX_BEAMFORM_CAP_EXPLICIT_COMPRESSED_STEERING_MATRIX_FEEDBACK_OFFSET 15
#define TX_BEAMFORM_CAP_MINIMAL_GROUPING_OFFSET 17
#define TX_BEAMFORM_CAP_CSI_NUM_BEAMFORMER_ANT_OFFSET 19
#define TX_BEAMFORM_CAP_UNCOMPRESSED_STEERING_MATRIX_BEAMFORMER_ANT_OFFSET 21
#define TX_BEAMFORM_CAP_COMPRESSED_STEERING_MATRIX_BEAMFORMER_ANT_OFFSET 23
#define TX_BEAMFORM_CAP_SCI_MAX_OF_ROWS_BEANFORMER_SUPPORTED_OFFSET 25


#define ASEL_CAPABILITY_ASEL_CAPABLE ((u8) BIT(0))
#define ASEL_CAPABILITY_EXPLICIT_CSI_FEEDBACK_BASED_TX_AS_CAP ((u8) BIT(1))
#define ASEL_CAPABILITY_ANT_INDICES_FEEDBACK_BASED_TX_AS_CAP ((u8) BIT(2))
#define ASEL_CAPABILITY_EXPLICIT_CSI_FEEDBACK_CAP ((u8) BIT(3))
#define ASEL_CAPABILITY_ANT_INDICES_FEEDBACK_CAP ((u8) BIT(4))
#define ASEL_CAPABILITY_RX_AS_CAP ((u8) BIT(5))
#define ASEL_CAPABILITY_TX_SOUND_PPDUS_CAP ((u8) BIT(6))

#define HT_INFO_HT_PARAM_SECONDARY_CHNL_OFF_MASK	((u8) BIT(0) | BIT(1))
#define HT_INFO_HT_PARAM_SECONDARY_CHNL_ABOVE		((u8) BIT(0))
#define HT_INFO_HT_PARAM_SECONDARY_CHNL_BELOW		((u8) BIT(0) | BIT(1))
#define HT_INFO_HT_PARAM_REC_TRANS_CHNL_WIDTH		((u8) BIT(2))
#define HT_INFO_HT_PARAM_RIFS_MODE			((u8) BIT(3))
#define HT_INFO_HT_PARAM_CTRL_ACCESS_ONLY		((u8) BIT(4))
#define HT_INFO_HT_PARAM_SRV_INTERVAL_GRANULARITY	((u8) BIT(5))


#define OP_MODE_PURE                    0
#define OP_MODE_MAY_BE_LEGACY_STAS      1
#define OP_MODE_20MHZ_HT_STA_ASSOCED    2
#define OP_MODE_MIXED                   3

#define HT_INFO_OPERATION_MODE_OP_MODE_MASK	\
		((le16) (0x0001 | 0x0002))
#define HT_INFO_OPERATION_MODE_OP_MODE_OFFSET		0
#define HT_INFO_OPERATION_MODE_NON_GF_DEVS_PRESENT	((u8) BIT(2))
#define HT_INFO_OPERATION_MODE_TRANSMIT_BURST_LIMIT	((u8) BIT(3))
#define HT_INFO_OPERATION_MODE_NON_HT_STA_PRESENT	((u8) BIT(4))

#define HT_INFO_STBC_PARAM_DUAL_BEACON			((u16) BIT(6))
#define HT_INFO_STBC_PARAM_DUAL_STBC_PROTECT		((u16) BIT(7))
#define HT_INFO_STBC_PARAM_SECONDARY_BCN		((u16) BIT(8))
#define HT_INFO_STBC_PARAM_LSIG_TXOP_PROTECT_ALLOWED	((u16) BIT(9))
#define HT_INFO_STBC_PARAM_PCO_ACTIVE			((u16) BIT(10))
#define HT_INFO_STBC_PARAM_PCO_PHASE			((u16) BIT(11))


#define OUI_MICROSOFT 0x0050f2 /* Microsoft (also used in Wi-Fi specs)
				* 00:50:F2 */
#define WPA_IE_VENDOR_TYPE 0x0050f201
#define WMM_IE_VENDOR_TYPE 0x0050f202
#define WPS_IE_VENDOR_TYPE 0x0050f204
#define OUI_WFA 0x506f9a
#define P2P_IE_VENDOR_TYPE 0x506f9a09
#define WFD_IE_VENDOR_TYPE 0x506f9a0a
#define WFD_OUI_TYPE 10
#define HS20_IE_VENDOR_TYPE 0x506f9a10
#define OSEN_IE_VENDOR_TYPE 0x506f9a12
#define MBO_IE_VENDOR_TYPE 0x506f9a16
#define MBO_OUI_TYPE 22
#define OWE_IE_VENDOR_TYPE 0x506f9a1c
#define OWE_OUI_TYPE 28
#define MULTI_AP_OUI_TYPE 0x1B

#define WMM_OUI_TYPE 2
#define WMM_OUI_SUBTYPE_INFORMATION_ELEMENT 0
#define WMM_OUI_SUBTYPE_PARAMETER_ELEMENT 1
#define WMM_OUI_SUBTYPE_TSPEC_ELEMENT 2
#define WMM_VERSION 1

#define WMM_ACTION_CODE_ADDTS_REQ 0
#define WMM_ACTION_CODE_ADDTS_RESP 1
#define WMM_ACTION_CODE_DELTS 2

#define WMM_ADDTS_STATUS_ADMISSION_ACCEPTED 0
#define WMM_ADDTS_STATUS_INVALID_PARAMETERS 1
/* 2 - Reserved */
#define WMM_ADDTS_STATUS_REFUSED 3
/* 4-255 - Reserved */

/* WMM TSPEC Direction Field Values */
#define WMM_TSPEC_DIRECTION_UPLINK 0
#define WMM_TSPEC_DIRECTION_DOWNLINK 1
/* 2 - Reserved */
#define WMM_TSPEC_DIRECTION_BI_DIRECTIONAL 3

/*
 * WMM Information Element (used in (Re)Association Request frames; may also be
 * used in Beacon frames)
 */
struct wmm_information_element {
	/* Element ID: 221 (0xdd); Length: 7 */
	/* required fields for WMM version 1 */
	u8 oui[3]; /* 00:50:f2 */
	u8 oui_type; /* 2 */
	u8 oui_subtype; /* 0 */
	u8 version; /* 1 for WMM version 1.0 */
	u8 qos_info; /* AP/STA specific QoS info */

} STRUCT_PACKED;

#define WMM_AC_AIFSN_MASK 0x0f
#define WMM_AC_AIFNS_SHIFT 0
#define WMM_AC_ACM 0x10
#define WMM_AC_ACI_MASK 0x60
#define WMM_AC_ACI_SHIFT 5

#define WMM_AC_ECWMIN_MASK 0x0f
#define WMM_AC_ECWMIN_SHIFT 0
#define WMM_AC_ECWMAX_MASK 0xf0
#define WMM_AC_ECWMAX_SHIFT 4

struct wmm_ac_parameter {
	u8 aci_aifsn; /* AIFSN, ACM, ACI */
	u8 cw; /* ECWmin, ECWmax (CW = 2^ECW - 1) */
	le16 txop_limit;
}  STRUCT_PACKED;

/*
 * WMM Parameter Element (used in Beacon, Probe Response, and (Re)Association
 * Response frmaes)
 */
struct wmm_parameter_element {
	/* Element ID: 221 (0xdd); Length: 24 */
	/* required fields for WMM version 1 */
	u8 oui[3]; /* 00:50:f2 */
	u8 oui_type; /* 2 */
	u8 oui_subtype; /* 1 */
	u8 version; /* 1 for WMM version 1.0 */
	u8 qos_info; /* AP/STA specif QoS info */
	u8 reserved; /* 0 */
	struct wmm_ac_parameter ac[4]; /* AC_BE, AC_BK, AC_VI, AC_VO */

} STRUCT_PACKED;

/* WMM TSPEC Element */
struct wmm_tspec_element {
	u8 eid; /* 221 = 0xdd */
	u8 length; /* 6 + 55 = 61 */
	u8 oui[3]; /* 00:50:f2 */
	u8 oui_type; /* 2 */
	u8 oui_subtype; /* 2 */
	u8 version; /* 1 */
	/* WMM TSPEC body (55 octets): */
	u8 ts_info[3];
	le16 nominal_msdu_size;
	le16 maximum_msdu_size;
	le32 minimum_service_interval;
	le32 maximum_service_interval;
	le32 inactivity_interval;
	le32 suspension_interval;
	le32 service_start_time;
	le32 minimum_data_rate;
	le32 mean_data_rate;
	le32 peak_data_rate;
	le32 maximum_burst_size;
	le32 delay_bound;
	le32 minimum_phy_rate;
	le16 surplus_bandwidth_allowance;
	le16 medium_time;
} STRUCT_PACKED;


/* Access Categories / ACI to AC coding */
enum {
	WMM_AC_BE = 0 /* Best Effort */,
	WMM_AC_BK = 1 /* Background */,
	WMM_AC_VI = 2 /* Video */,
	WMM_AC_VO = 3 /* Voice */
};


#define OUI_BROADCOM 0x00904c /* Broadcom (Epigram) */

#define VENDOR_HT_CAPAB_OUI_TYPE 0x33 /* 00-90-4c:0x33 */

/* cipher suite selectors */
#define WLAN_CIPHER_SUITE_USE_GROUP	0x000FAC00
#define WLAN_CIPHER_SUITE_WEP40		0x000FAC01
#define WLAN_CIPHER_SUITE_TKIP		0x000FAC02
/* reserved: 				0x000FAC03 */
#define WLAN_CIPHER_SUITE_CCMP		0x000FAC04
#define WLAN_CIPHER_SUITE_WEP104	0x000FAC05
#define WLAN_CIPHER_SUITE_AES_CMAC	0x000FAC06

/* AKM suite selectors */
#define WLAN_AKM_SUITE_8021X		0x000FAC01
#define WLAN_AKM_SUITE_PSK		0x000FAC02

#define OUI_BROADCOM 0x00904c /* Broadcom (Epigram) */
#define VENDOR_VHT_TYPE		0x04
#define VENDOR_VHT_SUBTYPE	0x08
#define VENDOR_VHT_SUBTYPE2	0x00

#define VENDOR_HT_CAPAB_OUI_TYPE 0x33 /* 00-90-4c:0x33 */

/* Wi-Fi Direct (P2P) */

#define P2P_OUI_TYPE 9

#define HS20_INDICATION_OUI_TYPE 16
#define HS20_ANQP_OUI_TYPE 17
#define HS20_OSEN_OUI_TYPE 18
#define HS20_ROAMING_CONS_SEL_OUI_TYPE 29
#define HS20_STYPE_QUERY_LIST 1
#define HS20_STYPE_CAPABILITY_LIST 2
#define HS20_STYPE_OPERATOR_FRIENDLY_NAME 3
#define HS20_STYPE_WAN_METRICS 4
#define HS20_STYPE_CONNECTION_CAPABILITY 5
#define HS20_STYPE_NAI_HOME_REALM_QUERY 6
#define HS20_STYPE_OPERATING_CLASS 7
#define HS20_STYPE_OSU_PROVIDERS_LIST 8
#define HS20_STYPE_ICON_REQUEST 10
#define HS20_STYPE_ICON_BINARY_FILE 11
#define HS20_STYPE_OPERATOR_ICON_METADATA 12
#define HS20_STYPE_OSU_PROVIDERS_NAI_LIST 13

struct ieee80211_he_capabilities {
	u8 he_mac_capab_info[6];
	u8 he_phy_capab_info[11];
	/* Followed by 4, 8, or 12 octets of Supported HE-MCS And NSS Set field
	* and optional variable length PPE Thresholds field. */
	u8 optional[];
} STRUCT_PACKED;

struct ieee80211_he_operation {
	le32 he_oper_params; /* HE Operation Parameters[3] and
			      * BSS Color Information[1] */
	le16 he_mcs_nss_set;
	u8 vht_op_info_chwidth;
	u8 vht_op_info_chan_center_freq_seg0_idx;
	u8 vht_op_info_chan_center_freq_seg1_idx;
	/* Followed by conditional MaxBSSID Indicator subfield (u8) */
} STRUCT_PACKED;

/* HE Capabilities Information defines */
#define HE_PHYCAP_CHANNEL_WIDTH_SET_IDX		0
#define HE_PHYCAP_CHANNEL_WIDTH_MASK		((u8) (BIT(1) | BIT(2) | \
						      BIT(3) | BIT(4)))
#define HE_PHYCAP_CHANNEL_WIDTH_SET_40MHZ_IN_2G         ((u8) BIT(1))
#define HE_PHYCAP_CHANNEL_WIDTH_SET_40MHZ_80MHZ_IN_5G	((u8) BIT(2))
#define HE_PHYCAP_CHANNEL_WIDTH_SET_160MHZ_IN_5G	((u8) BIT(3))
#define HE_PHYCAP_CHANNEL_WIDTH_SET_80PLUS80MHZ_IN_5G	((u8) BIT(4))

#define HE_PHYCAP_SU_BEAMFORMER_CAPAB_IDX	3
#define HE_PHYCAP_SU_BEAMFORMER_CAPAB		((u8) BIT(7))
#define HE_PHYCAP_SU_BEAMFORMEE_CAPAB_IDX	4
#define HE_PHYCAP_SU_BEAMFORMEE_CAPAB		((u8) BIT(0))
#define HE_PHYCAP_MU_BEAMFORMER_CAPAB_IDX	4
#define HE_PHYCAP_MU_BEAMFORMER_CAPAB		((u8) BIT(1))

#define HE_PHYCAP_PPE_THRESHOLD_PRESENT_IDX	6
#define HE_PHYCAP_PPE_THRESHOLD_PRESENT		((u8) BIT(7))

/* HE PPE Threshold define */
#define HE_PPE_THRES_RU_INDEX_BITMASK_MASK	0xf
#define HE_PPE_THRES_RU_INDEX_BITMASK_SHIFT	3
#define HE_PPE_THRES_NSS_MASK			0x7

/* HE Operation defines */
/* HE Operation Parameters and BSS Color Information fields */
#define HE_OPERATION_DFLT_PE_DURATION_MASK	((u32) (BIT(0) | BIT(1) | \
							BIT(2)))
#define HE_OPERATION_DFLT_PE_DURATION_OFFSET	0
#define HE_OPERATION_TWT_REQUIRED		((u32) BIT(3))
#define HE_OPERATION_RTS_THRESHOLD_MASK	((u32) (BIT(4) | BIT(5) | \
						BIT(6) | BIT(7) | \
						BIT(8) | BIT(9) | \
						BIT(10) | BIT(11) | \
						BIT(12) | BIT(13)))
#define HE_OPERATION_RTS_THRESHOLD_OFFSET	4
#define HE_OPERATION_BSS_COLOR_MASK		((u32) (BIT(24) | BIT(25) | \
							BIT(26) | BIT(27) | \
							BIT(28) | BIT(29)))
#define HE_OPERATION_PARTIAL_BSS_COLOR		((u32) BIT(30))
#define HE_OPERATION_BSS_COLOR_DISABLED		((u32) BIT(31))
#define HE_OPERATION_BSS_COLOR_OFFSET		24

/* Spatial Reuse defines */
#define SPATIAL_REUSE_SRP_DISALLOWED		BIT(0)
#define SPATIAL_REUSE_NON_SRG_OBSS_PD_SR_DISALLOWED	BIT(1)
#define SPATIAL_REUSE_NON_SRG_OFFSET_PRESENT	BIT(2)
#define SPATIAL_REUSE_SRG_INFORMATION_PRESENT	BIT(3)
#define SPATIAL_REUSE_HESIGA_SR_VAL15_ALLOWED	BIT(4)

struct ieee80211_he_mu_edca_parameter_set {
	u8 he_qos_info;
	u8 he_mu_ac_be_param[3];
	u8 he_mu_ac_bk_param[3];
	u8 he_mu_ac_vi_param[3];
	u8 he_mu_ac_vo_param[3];
} STRUCT_PACKED;

/* HE MU AC parameter record field format */
/* ACI/AIFSN */
#define HE_MU_AC_PARAM_ACI_IDX 0
#define HE_MU_AC_PARAM_AIFSN ((u8) (BIT(0) | BIT(1) | BIT(2) | BIT(3)))
#define HE_MU_AC_PARAM_ACM ((u8) BIT(4))
#define HE_MU_AC_PARAM_ACI ((u8) (BIT(5) | BIT(6)))
/* B7: Reserved */

/* ECWmin/ECWmax */
#define HE_MU_AC_PARAM_ECW_IDX 1
#define HE_MU_AC_PARAM_ECWMIN ((u8) (BIT(0) | BIT(1) | BIT(2) | BIT(3)))
#define HE_MU_AC_PARAM_ECWMAX ((u8) (BIT(4) | BIT(5) | BIT(6) | BIT(7)))

/* MU EDCA Timer */
#define HE_MU_AC_PARAM_TIMER_IDX 2

/* HE QoS Info field */
#define HE_QOS_INFO_EDCA_PARAM_SET_COUNT ((u8) (BIT(0) | BIT(1) | \
						BIT(2) | BIT(3)))
#define HE_QOS_INFO_Q_ACK ((u8) (BIT(4)))
#define HE_QOS_INFO_QUEUE_REQUEST ((u8) (BIT(5)))
#define HE_QOS_INFO_TXOP_REQUEST ((u8) (BIT(6)))
/* B7: Reserved if sent by an AP; More Data Ack if sent by a non-AP STA */
#define HE_QOS_INFO_MORE_DATA_ACK ((u8) (BIT(7)))

/* DPP Public Action frame identifiers - OUI_WFA */
#define DPP_OUI_TYPE 0x1A

#endif /* IEEE802_11_DEFS_H */
