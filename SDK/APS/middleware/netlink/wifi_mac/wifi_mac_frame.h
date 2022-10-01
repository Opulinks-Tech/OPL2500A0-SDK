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

#ifndef _WIFI_MAC_FRAME_H_
#define _WIFI_MAC_FRAME_H_

#include "ipc.h"

#pragma anon_unions

//MHDR(24)+Addr4(6)+QoS(2)+HTC(4)+ [CCMP_HDR(8) or IV(4)+EIV(4)] +LLC(8) ~Payload~ MIC(8)+ICV(4)+FCS(4)
#define WIFI_TX_FRM_RESV_LEN    68

typedef enum {
	PHY_MODE_11B_LONG = 0,
	PHY_MODE_11B_SHORT,
	PHY_MODE_11B_NUM,

	PHY_MODE_11G_LEGACY = 0,
	PHY_MODE_11N_HT_MIX_LGI,
	PHY_MODE_11N_HT_MIX_SGI,
	PHY_MODE_11N_GREEN,
	PHY_MODE_11GN_NUM,
} phy_mode_t;

typedef enum {
	TX_IN_QUEUE_SUCCESS,
	TX_IN_QUEUE_FAIL,
	TX_QUEUE_FULL
} tx_data_frame_t;

typedef enum {
	MAC_STATE_IDLE,
	MAC_STATE_0,
	MAC_STATE_1,
	MAC_STATE_2,
	MAC_STATE_3,
	MAC_STATE_4
} mac_state_t;

typedef enum
{
    // 11b
    WIFI_RATE_1M = 0,
    WIFI_RATE_2M = 1,
    WIFI_RATE_5_5M = 2,
    WIFI_RATE_11M = 3,
    WIFI_RATE_11B = WIFI_RATE_11M,
    WIFI_RATE_11B_MIN = WIFI_RATE_1M,
    WIFI_RATE_11B_MAX = WIFI_RATE_11M,
    WIFI_RATE_11B_NUM = WIFI_RATE_11B_MAX - WIFI_RATE_11B_MIN + 1,

//    WIFI_RATE_NUM = WIFI_RATE_11B + 1,

    // 11gn
    // Legacy                   LGI/Green   SGI
    WIFI_RATE_6M = 4,       //  6.5         7.2
    WIFI_RATE_9M,           //  13          14.4
    WIFI_RATE_12M,          //  19.5        21.7
    WIFI_RATE_18M,          //  26          28.9
    WIFI_RATE_24M,          //  39          43.3
    WIFI_RATE_36M,          //  52          57.8
    WIFI_RATE_48M,          //  58.5        65
    WIFI_RATE_54M,          //  65          72.2
    WIFI_RATE_11GN = WIFI_RATE_54M,
    WIFI_RATE_11GN_MIN = WIFI_RATE_6M,
    WIFI_RATE_11GN_MAX = WIFI_RATE_54M,
    WIFI_RATE_11GN_NUM = WIFI_RATE_11GN_MAX - WIFI_RATE_11GN_MIN + 1,

    WIFI_RATE_NUM = WIFI_RATE_11GN + 1,
    WIFI_RATE_NOT_FIX = WIFI_RATE_NUM,

    WIFI_RATE_MIN = WIFI_RATE_1M,
    WIFI_RATE_MAX = WIFI_RATE_54M,
} wifi_rate_mode_t;

typedef enum {
	WIFI_STA_MODE      = 0,
	WIFI_AP_MODE       = 1,
	WIFI_AP_STA_MODE   = 2,
	WIFI_RF_MODE       = 3,

    WIFI_RF_TR_MODE    = 6,
    WIFI_PHY_TEST_MODE = 7,
} wifi_device_mode_t;

typedef enum
{
    WIFI_TID_0  = WLAN_TID_0,
    WIFI_TID_1  = WLAN_TID_1,
    WIFI_TID_2  = WLAN_TID_2,
    WIFI_TID_3  = WLAN_TID_3,
    WIFI_TID_4  = WLAN_TID_4,
    WIFI_TID_5  = WLAN_TID_5,
    WIFI_TID_6  = WLAN_TID_6,
    WIFI_TID_7  = WLAN_TID_7,

    WIFI_TID_NA = 0xff,
} E_WIFI_TID;

/*** For TRX use ***/
/***** Need to sync with MSQ in trx_wifi.h *****/
typedef struct
{
    UINT16  u16Reserved:14;
    UINT16  u16CrcInvert:1;     /* [14] CRC inverter. The function will be actived after bit 9 of 0x400030fc is 1 */
    UINT16  u16Reserved2:1;
    UINT16  u16MpduLen:14;      /* [16..29] MPDU length, including FCS */
    UINT16  u16Reserved1:1;
    UINT16  u16EOF:1;           /* [31] EOF for HE use */
} S_TRX_WIFI_TX_DSC;
#define TRX_WIFI_TX_DSC_LEN         sizeof(S_TRX_WIFI_TX_DSC)

/***** Need to sync with MSQ in trx_wifi.h *****/
typedef struct
{
    union
    {
        /* 11b */
        struct
        {
            UINT8   u8Rate:7;           /* 0x0A=1M, 0x14=2M, 0x37=5.5M, 0x6E=11M */
            UINT8   u8Padding1:1;
            UINT8   u8PreambleType:1;   /* 0:Long, 1:Short  */
            UINT8   u8Padding2:7;
        };
        /* 11gn */
        struct
        {
            UINT16  u16Rate:7;          /* [0..6] Data rate, 0 ~ 7 */
            UINT16  u16STBC:2;          /* [7..8] STBC, not support */
            UINT16  u16GI:1;            /* [9] Guard interval, 0: Long, 1: Short */
            UINT16  u16Aggregation:1;   /* [10] Aggregation, 0: off, 1: on */
            UINT16  u16Mode:1;          /* [11] Legacy/Green, 0: Legacy, 1: Green */
            UINT16  u16HTMode:1;        /* [12] HT mode, 0: Non-HT, 1: HT */
            UINT16  u16Sounding:1;      /* [13] Sounding, not support */
            UINT16  u16NumEss:2;        /* [14..15] Number ESS, not support */
        };
    };
    UINT16 u16FrmLen:14;        /* Number of bytes in MAC packet */
    UINT16 u16Reserved:1;
    UINT16 u16EOF:1;            /* [31] EOF for HE use */
} S_TRX_WIFI_RX_DSC;
#define TRX_WIFI_RX_DSC_LEN         sizeof(S_TRX_WIFI_RX_DSC)
/*** For TRX use ***/

typedef int32_t (*FP_WIFI_TX_INIT)(void *pData);
typedef int32_t (*FP_WIFI_TX_DONE)(bool result);

/***** Need to sync with APS in wifi_mac_frame.h *****/
typedef struct
{
    FP_WIFI_TX_INIT fpTxInit;       /* Callback function for Init before TX en */
    void            *pInitData;     /* Data for Init before TX en */
    FP_WIFI_TX_DONE fpTxDone;       /* Callback function for TX done/drop */
    void            *pFrmBuf;       /* Frame buffer start address */
    UINT16          u16FrmLen;      /* Number of bytes in MAC packet, including MAC header, without FCS */
    bool            bNeedEncrypt;   /* Need encrypt frame */
} S_WIFI_TX_FRM_INFO;
#define WIFI_TX_FRAME_INFO_LEN  sizeof(S_WIFI_TX_FRM_INFO)

/***** Need to sync with APS in wifi_mac_frame.h *****/
typedef struct
{
    UINT32  u32RxStatus;            /* Rx status from TRX */
    UINT32  u32TimeStampLow;        /* Time snap of 7th word of MAC header, low 32-bit. Only available when received Beacon & Probe response */
    UINT32  u32TimeStampHigh;       /* Time snap of 7th word of MAC header, high 32-bit. Only available when received Beacon & Probe response */
    void    *pFrmBuf;               /* Frame buffer start address */
    UINT16  u16FrmLen;              /* Number of bytes in MAC packet, including MAC header, without FCS */
    UINT8   u8HdrLen:7;             /* The length of MAC header */
    UINT8   u8DecryptedFrm:1;       /* Whether decrypt received frame */
    INT8    s8Rssi;                 /* RSSI */
} S_WIFI_RX_FRM_INFO;
#define WIFI_RX_FRAME_INFO_LEN   sizeof(S_WIFI_RX_FRM_INFO)

typedef struct
{
    /* Configuration */
    uint32_t    dbg_tx_count_set;               // [1]:total TX frame will be sent , 0 is unlimited (dbg_tx_retry = 0 is MUST)
    uint16_t    dbg_tx_data_len;                // [10]:data_type frame. range: 0 ~ 104 (WIFI_MAC_APS_TX_BUF_SIZE - 24)
    uint8_t     dbg_tx_phy_mode;                // [0]: PHY_MODE_11B_LONG, [1]: PHY_MODE_11B_SHORT
    uint8_t     dbg_tx_data_rate;               // [0]: 1M, [1]: 2M, [2]: 5.5M, [3]: 11M
    uint16_t    dbg_interval;                   // [0]:set SIFS, range: 5 ~ 63 (dbg_tx_wait_ack = 0,  dbg_tx_with_dcf = 0 is MUST)
    /* Logs */
    int8_t      dbg_rssi;                       // Read only. Update value when INT of TRX_RX_FRAME_DONE happen
    uint32_t    dbg_rx_uc_count;                // read only, total unicast frames were rx (CRC ok)
    uint32_t    dbg_rx_bc_count;                // read only, total broadcast frames were rx (CRC ok)
    uint32_t    dbg_rx_er_count;                // read only, total error frames were rx
} S_WIFI_RF_TEST;

typedef struct{
	u16 capabilities;
	u16 listen_interval;
} association_request_t;
#define ASSOCIATION_REQUEST_LEN 4

typedef struct{
	u16 capabilities;
	u16 listen_interval;
	u8 current_ap[MAC_ADDR_LEN];
} reassociation_request_t;
#define REASSOCIATION_REQUEST_LEN 4

typedef struct{
	//u8 timestamp[8];
	u32 timestamp_low;
	u32 timestamp_high;
	u16 beacon_interval;
	u16 capabilities;
} probe_response_t;
#define PROBE_RESPONSE_LEN 12

typedef struct{
	u16 capabilities;
	u16 status;
	u16 aid;
} association_response_t;
#define ASSOCIATION_RESPONSE_LEN 6

typedef struct {
	u16 auth_algorithm;
	u16 auth_seq_num;
	u16 auth_status;
} auth_req_resp_t;
#define AUTH_REQ_RESP_LEN 6

typedef struct{
	u8 element_id;
	u8 element_length;
} element_header_t;
#define ELEMENT_HEADER_LEN 2

typedef struct{
	element_header_t header;
	u8				data[256];
} infomation_element_t;

typedef struct
{
	/**
	 * psk - WPA pre-shared key (256 bits)
	 */
	u8 psk[32];

	/**
	 * psk_set - Whether PSK field is configured
	 */
	int psk_set;

	/**
	 * passphrase - WPA ASCII passphrase
	 *
	 * If this is set, psk will be generated using the SSID and passphrase
	 * configured for the network. ASCII passphrase must be between 8 and
	 * 63 characters (inclusive).
	 */
	char *passphrase;

	/**
	 * pairwise_cipher - Bitfield of allowed pairwise ciphers, WPA_CIPHER_*
	 */
	int pairwise_cipher;

	/**
	 * group_cipher - Bitfield of allowed group ciphers, WPA_CIPHER_*
	 */
	int group_cipher;

	/**
	 * key_mgmt - Bitfield of allowed key management protocols
	 *
	 * WPA_KEY_MGMT_*
	 */
	int key_mgmt;

	int mgmt_group_cipher;	//for 802.11w?
	/**
	 * proto - Bitfield of allowed protocols, WPA_PROTO_*
	 */
	int proto;

#define EAPOL_FLAG_REQUIRE_KEY_UNICAST BIT(0)
#define EAPOL_FLAG_REQUIRE_KEY_BROADCAST BIT(1)
	/**
	 * eapol_flags - Bit field of IEEE 802.1X/EAPOL options (EAPOL_FLAG_*)
	 */
	int eapol_flags;

	/**
	 * eap - EAP peer configuration for this network
	 */
	//struct eap_peer_config eap;
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
}wpa_ssid_t;

typedef struct
{
    //u8 * frame_buffer;
    u8 frame_buffer[384];
    unsigned int frame_length;
}rx_eapol_report;

typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	u16 frame_length;
} rts_info_t;

/*
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
} ack_info_t;
*/

/*
MLME-SCAN Request
	BSSType, BSSID, SSID, ScanType, ProbeDelay, ChannelList, MinChannelTime, MaxChannelTime,
	VendorSpecificInfo
MLME-SCAN Confirm
	BBSDescriptionSet, ResultCode, VendorSpecificInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	//u8 bssid[MAC_ADDR_LEN];
	//u8 ssid[IEEE80211_MAX_SSID_LEN + 1];
} probe_req_info_t;

/*
MLME-AUTHENTICATE Request
	PeerSTAAddress, AuthenticationType, AuthenticateFailureTimeout, VendorSpecificInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	//auth_req_resp_t auth_request;
	//u16 auth_algorithm;
	//u16 auth_seq_num;
	//u16 auth_status;
} auth_req_info_t;

/*
MLME-SCAN Confirm
	BBSDescriptionSet, ResultCode, VendorSpecificInfo
*/
typedef struct {
	//u8 management_frame_type;
	probe_response_t probe_response;
	u8 bssid[MAC_ADDR_LEN];
	u8 ssid[IEEE80211_MAX_SSID_LEN + 1];
	u8 supported_rates[IEEE80211_MAX_SUPPORTED_RATES];
} probe_resp_info_t;

/*
MLME-AUTHENTICATE Response
	PeerSTAAddress, ResultCode, VendorSpecificInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	auth_req_resp_t auth_response;
} auth_resp_info_t;

/*
MLME-ASSOCIATE Request
	PeerSTAAddress, AssociationFailureTimeout, CapabilityInformation, ListenInterval, Supported Channels, RSN, QoSCapability, VendorSpeciticInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	//association_request_t association_request;
} assoc_req_info_t;

/*
MLME-REASSOCIATE Response
	PeerSTAAddress, ResultCode, CapabilityInformation, AssociationID, EDCAParameterSet,	VendorSpeciticInfo
*/
/*
MLME-ASSOCIATE Response
	PeerSTAAddress, ResultCode, CapabilityInformation, AssociationID, EDCAParameterSet, VendorSpeciticInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	association_response_t association_response;
	u8 supported_rates[IEEE80211_MAX_SUPPORTED_RATES];
} assoc_resp_info_t;

/*
MLME-DEAUTHENTICATE Request
	PeerSTAAdress, ReasonCode, VendorSpecificInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	u32 reason_code;
} deauth_req_info_t;

/*
MLME-Disassociate Request
	PeerSTAAddress, ReasonCode, VendorSpecifinInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	u16 reason_code;
} disassoc_req_info_t;

/*
MLME-REASSOCIATE Request
	NewAPAddress, ReassociationFailureTimeout, CapabilityInformation, ListenInterval, Supported Channels, RSN, QoSCapability, VendorSpeciticInfo
*/
typedef struct {
	//u8 management_frame_type;
	u8 *mpdu_buf;
	//reassociation_request_t reassociation_request;
} reassoc_req_info_t;

typedef void (*wifi_sta_create_data_mac_header_fp)(UINT8 *header, UINT8 u8TID, UINT16 encrypt, const UINT8* eth_dest);

extern uint8_t g_u8aApsTxQToTid[];

extern wifi_sta_create_data_mac_header_fp wifi_sta_create_data_mac_header;

void bss_reset(void);
void wifi_sta_create_data_mac_header_impl(UINT8 *header, UINT8 u8TID, UINT16 encrypt, const UINT8* eth_dest);

#endif /* _WIFI_MAC_FRAME_H_ */

