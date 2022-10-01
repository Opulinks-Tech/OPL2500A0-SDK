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

#ifndef _WIFI_MAC_STA_H_
#define _WIFI_MAC_STA_H_

/***** Need to sync with MSQ in wifi_mac_interface.h *****/
typedef struct {
    uint8_t     u8MacMode;
    uint8_t     u8Standard;
    uint8_t     u8Dot11ShortRetryLimit;
    uint8_t     u8Dot11LongRetryLimit;
    uint8_t     u8Sifs;
    uint8_t     u8Difs;
    uint8_t     u8Slot;
    uint8_t     u8R2TLen;           // Unit: us
    uint8_t     u8T2RLen;           // Unit: us
    uint8_t     u8AmpduTxSuppTid;   // Which TID support AMPDU transmission
    // 11th
    uint8_t     u8AmpduRxSuppTid;   // Which TID support AMPDU reception
    uint8_t     u8AmpduBuffSizeMax;
    uint8_t     u8SilenceSec;       // Unit: WLAN_TIME_UNIT * WLAN_TIME_UNIT. Maximum: 25
    // 21st
    // 31st
    // 41st
    // N bytes

    uint8_t     u8aReserved[51];    // 64 - N bytes
} S_WIFI_MIB;

typedef struct
{
    uint8_t     u8NonERPPresent:1;
    uint8_t     u8UseProtection:1;
    uint8_t     u8BarkerPreambleMode:1;
    uint8_t     u8Reserved:5;
} __PACKED S_WLAN_ERP_PARAM;

typedef struct
{
    UINT16  u16DefPEDur:3;
    UINT16  u16TWTRequired:1;
    UINT16  u16TXOPDurRTSThr:10;
    UINT16  u16VHTOpInfoPres:1;
    UINT16  u16CoHostedBSS:1;
    UINT8   u8ERSUDisable:1;
    UINT8   u86GHzOpInfoPres:1;
    UINT8   u8Reserved:6;
} __PACKED S_WLAN_HE_OP_PARAM;

typedef struct
{
    uint8_t     u8BssColor:6;
    uint8_t     u8PartialBssColor:1;
    uint8_t     u8BssColorDis:1;
} __PACKED S_WLAN_BSS_COLOR_INFO;

/***** Need to sync with MSQ in wifi_mac_sta.h *****/
typedef struct {
    void                *pBssInfoExt;       // Used for extending BSS INFO structure
	uint8_t             bssid[MAC_ADDR_LEN];
	int8_t              ssid[IEEE80211_MAX_SSID_LEN + 1];
    uint8_t             ssid_len;
	mac_state_t mac_state;
	uint16_t            beacon_interval;
	uint16_t            auth_algorithm;
	uint16_t            capabilities;
	uint16_t            u16HtCapabilities;
	uint8_t             current_channel;    //Current RF Channel. Table 17-9
	uint8_t             u8DtimCount;    // The value is equal to AP
	uint8_t             u8DtimPeriod;   // The value is equal to AP
    S_WLAN_ERP_PARAM    sERPParam;
    S_WLAN_HE_OP_PARAM      sHeOpParam;
    S_WLAN_BSS_COLOR_INFO   sBssColorInfo;
	int8_t              rssi;
	uint8_t             u8Standard;
} S_WIFI_BSS_INFO;

extern S_WIFI_BSS_INFO *g_ps_bss_info;
extern S_WIFI_MIB *g_ps_mib;
extern S_WIFI_MIB *ps_StaInfo;

void wifi_mac_sta_func_init(void);

#endif /* _WIFI_MAC_STA_H_ */

