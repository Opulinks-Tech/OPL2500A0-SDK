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

#ifndef __AT_CMD_WIFI_H__
#define __AT_CMD_WIFI_H__

#define AT_WIFI_SHOW_ECN_BIT        0x00000001U
#define AT_WIFI_SHOW_SSID_BIT       0x00000002U
#define AT_WIFI_SHOW_RSSI_BIT       0x00000004U
#define AT_WIFI_SHOW_MAC_BIT        0x00000008U
#define AT_WIFI_SHOW_CHANNEL_BIT    0x00000010U
#define AT_WIFI_SHOW_ALL_BIT        0x0000001FU

#define MAX_NUM_OF_HOST_NAME_SIZE   32

#define AT_WIFI_MAX_PASSWD_LEN      64
#define AT_WIFI_MIN_PASSWD_LEN      8
#define AT_WIFI_MAX_SSID_LEN        32
#define AT_WIFI_MAC_ADDR_LEN        6

#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

typedef enum {
    AT_WIFI_SKIP_DTIM_CFG,
    AT_WIFI_MAX_NUM,
} at_wifi_mac_cfg_e;

typedef struct {
    uint8_t sort_en;
    uint8_t mask;
    uint8_t times;
    uint8_t probe_counters;
} at_lap_opt_t;

/*
 * @brief AT Command Interface Initialization for Wi-Fi modules
 *
 */
void at_cmd_wifi_func_init(void);

#endif //__AT_CMD_WIFI_H__

