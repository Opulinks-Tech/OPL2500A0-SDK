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
 *  @file controller_wifi.h
 *
 *  @brief `
 *
 ******************************************************************************/

#ifndef __CONTROLLER_WIFI_H__
#define __CONTROLLER_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
#include <stdbool.h>

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

#define CTRL_WIFI_APS_HANDLE_AUTH_ASSOC

/*
 * Ctrl WiFi state machine.
 */
#define CTRL_WIFI_DISCONNECT_ST             0
#define CTRL_WIFI_SCANNING_ST               1
#define CTRL_WIFI_PROBE_ST                  2
#define CTRL_WIFI_AUTH_ST                   3
#define CTRL_WIFI_ASSOC_ST                  4
#define CTRL_WIFI_CONNECTED_ST              5  /**< Include OPEN or Security condition */
#define CTRL_WIFI_CONNECTED_SCAN_ST         6  /**< Do scan in connected state */

/*
 * Ctrl WiFi Error Code
 */
#define CTRL_WIFI_ERR_OK                    0
#define CTRL_WIFI_ERR_AC_FAILED             200  /**< Auto connect fail */
#define CTRL_WIFI_ERR_NOT_FOUND             201  /**< AP did not in scan results */
#define CTRL_WIFI_ERR_SSID_NOT_MATCH        202  /**< SSID not match in scan results */
#define CTRL_WIFI_ERR_PROBE_TIMEOUT         203  /**< Did not receive a probe response from ap */
#define CTRL_WIFI_ERR_AUTH_TIMEOUT          204  /**< Did not receive a authentication response from ap */
#define CTRL_WIFI_ERR_ASSOC_TIMEOUT         205  /**< Did not receive a association response from ap */
#define CTRL_WIFI_ERR_AUTH_FAILED           206  /**< Got an error code in authentication stage */
#define CTRL_WIFI_ERR_ASSOC_FAILED          207  /**< Got an error code in association stage */
#define CTRL_WIFI_ERR_ABORT                 254
#define CTRL_WIFI_INTERNAL_ERR              255  /**< Command timeout or others*/

#define CTRL_WIFI_AUTH_TIMEOUT              501

/*
 * Ctrl WiFi common definitions
 */
#define CTRL_WIFI_MAC_ADDR_LEN              6
#define CTRL_WIFI_MAX_SSID_LEN              32
#define CTRL_WIFI_MAX_SKIP_DTIM             255

/*
 * Ctrl WiFi scan definitions
 */
#define CTRL_WIFI_NUM_OF_SCAN_RES           5

#define CTRL_WIFI_SCAN_ACTIVE               0  /**< Actively scan a network by sending 802.11 probe(s)         */
#define CTRL_WIFI_SCAN_PASSIVE              1  /**< Passively scan a network by listening for beacons from APs */
#define CTRL_WIFI_SCAN_MIX                  2  /**< Active + Passive */

#define CTRL_WIFI_SCAN_ALL_CHANNELS         0
#define CTRL_WIFI_SCAN_MAX_NUM_CHANNELS     14

#define CTRL_WIFI_SCAN_ACTIVE_MIN_DUR_TIME  100
#define CTRL_WIFI_SCAN_PASSIVE_MIN_DUR_TIME 150
#define CTRL_WIFI_SCAN_PROBE_REQ_COUNTERS   1
#define CTRL_WIFI_SCAN_MAX_NUM_OF_DUR_TIME  1500
#define CTRL_WIFI_SCAN_MIN_DURATION_TIME    10

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef enum sta_mlme_cmd
{
    STA_MCMD_INIT = 0,        /**< Reset the MSQ status to default. */
    STA_MCMD_SCAN,            /**< The request of WiFi scan.*/
    STA_MCMD_SCAN_ABORT,      /**< Stop the scan request */
    STA_MCMD_CONNECT,         /**< The request of WiFi connect.*/
    STA_MCMD_AUTH_CONFIRM,    /**< For WPA3 used. The stage of SAE confirms the request.*/
    STA_MCMD_CONNECT_DONE,    /**< Notify to driver the Authentication completed or not.
                                   0: Connect failed, 1:Connected */
    STA_MCMD_DISCONNECT,      /**< The request of WiFi disconnect.*/
    STA_MCMD_STA_MODE,        /**< arg1: force power-saving mode with E_WIFI_STA_MODE */
    STA_MCMD_SET_PARAM,       /**< arg1: parameter id. prvData: Value or pointer to be set */
    STA_MCMD_GET_PARAM,       /**< arg1: parameter id. prvData: Target memory to be written */
    STA_MCMD_TRX,             /**< The setting of Transmit/Receive packets directly to MSQ*/
                              /**< 0x80 Reserved. */
} ctrl_wifi_sta_mlme_cmd_e;

typedef enum mlme_evt_type
{
    MLME_EVT_SCAN_RESULTS = 0,/**< Scan results available */
    MLME_EVT_PROBE,           /**< Probe request success or fail(timeout), 0:OK*/
    MLME_EVT_AUTH,            /**< Authentication stage. For MSQ handle the auth/assoc used only. 501:timeout */
    MLME_EVT_ASSOC,           /**< Association completed. For MSQ handle the auth/assoc used only. 501:timeout */
    MLME_EVT_DEAUTH,          /**< DeAuth from AP or MSQ. error :500 */
    MLME_EVT_TX_ACK,          /**< The acknowledge of Wi-Fi transmission if the bit of acked has
                                   been raised in tx_frm_info. */
    MLME_EVT_SET_PARAM_CNF,
    MLME_EVT_GET_PARAM_CNF,
    MLME_EVT_TRX,             /**< The acknowledge of the setting of Transmit/Receive packets.*/
    MLME_EVT_UPDATE_DTIM,     /**< Receive the event of AP's DTIM info after Assoc pass. */
} ctrl_wifi_mlme_evt_type_e;

/*
 * WiFi Mac Power Saving POLL
 * Used for arg1 of MLME command of STA_MCMD_STA_MODE
 */
typedef enum {
    STA_MODE_LIGHT_PS,      /**< Default */
    STA_MODE_DEEP_PS,
    STA_MODE_AUTO_PS,
    /* Reserved for other power saving mode */
    STA_MODE_BCN_TRACK = 5,
    STA_MODE_PERFORMANCE,   /**< No Power Saving POLL */
    STA_MODE_DEEP_SLEEP,
    STA_MODE_NUM,
} E_WIFI_STA_MODE;

typedef enum {
    WIFI_MMODE_INVALID,
    WIFI_MMODE_STA,         /**< Default */
    WIFI_MMODE_AP,
    WIFI_MMODE_TRX,

    WIFI_MMODE_NUM,
} E_WIFI_MAC_MODE;

typedef enum
{
    /* 0 - 0xEF are reserved */
    WIFI_GCMD_MODE_INIT = 0xF0,
    WIFI_GCMD_MODE_RESET,
} E_WIFI_GLOBAL_CMD;

typedef enum
{
    DISC_IMME,                  // Immediately implement the disconnecting flow without notifying AP
    DISC_NOTIFY_AP,             // Notify AP with Deauthentication that we are leaving
    DISC_GOT_FROM_AP = 0xFF,    // Receive Deauthentication/Disassociation from AP
} E_WIFI_DISC_MODE;

typedef struct
{
    uint16_t    u16ReasonCode;
    uint8_t     u8DiscMode;
    uint8_t     u8Reserved;
} S_WIFI_MLME_CMD_DISC_DATA;

typedef struct
{
    uint32_t  cmd_type:8;     /**< Refer to ctrl_wifi_sta_mlme_cmd_e*/
    uint32_t  arg1:24;
    void     *prvData;
} S_WIFI_CMD;

typedef enum {
    E_WIFI_PARAM_MAC_ADDRESS        = 0,
    E_WIFI_PARAM_SKIP_DTIM_PERIODS,
    E_WIFI_PARAM_DATA_RATE_FIX,
    E_WIFI_PARAM_LISTEN_INTERVAL,
    E_WIFI_PARAM_UNICAST_KEY,
    E_WIFI_PARAM_IGTK_KEY,

    E_WIFI_PARAM_BSS_DTIM_PERIOD    = 200,  /**< Read only parameters */
}E_WIFI_PARAM_ID;

typedef struct
{
    uint32_t evt_type;        /**< Refer to ctrl_wifi_mlme_evt_type_e*/
    void    *prvData;
} S_WIFI_EVT;

/**
 * Allocate a buffer for MSQ stuff the return value.
 */
typedef struct
{
    uint32_t sid;             /**< Transmit sequence ID, start from 0. */
    uint32_t total_bkof_time; /**< Total of back of time.*/
    uint8_t  total_retry_cnt; /**< Total of retry count (0-14 times, 15:Tx fail or others) */
    uint16_t u16SeqNum;
} ctrl_wifi_mac_tx_acked_info_t;

typedef struct
{
    uint32_t sid:31;          /**< Transmit sequence ID, start from 0. */
    uint32_t acked:1;         /**< 0:No ack, 1:Acked*/
    ctrl_wifi_mac_tx_acked_info_t acked_buf; /**< stuffed by MSQ */
} ctrl_wifi_mac_tx_info_t;

typedef struct
{
    uint32_t in_used:1;
    uint32_t length:31;
    uint32_t start_time;      /**< milliseconds */
    ctrl_wifi_mac_tx_info_t info;
} ctrl_wifi_mac_tx_list_t;

typedef struct
{
    uint32_t tx_ok;
    uint32_t tx_drop;
    uint32_t tx_bkof_time_avg;
    uint32_t tx_retry_cnt_avg;
    uint32_t tx_rtt_avg;
} ctrl_wifi_mac_tx_cnt_t;

typedef struct
{
    uint32_t total_tx_cnt;
    uint32_t total_rx_cnt;
    uint32_t tx_alloc_fail_cnt;
    ctrl_wifi_mac_tx_cnt_t len_less;    /**< length < 100 bytes */
    ctrl_wifi_mac_tx_cnt_t len_100;     /**< length >= 100 < 500 bytes */
    ctrl_wifi_mac_tx_cnt_t len_500;     /**< length >= 500 < 1000 bytes */
    ctrl_wifi_mac_tx_cnt_t len_1000;    /**< length >= 1000 bytes */
} ctrl_wifi_mac_tx_stats_t;

typedef struct
{
    uint8_t  num_of_tx_buf;
    uint32_t sid:31;
    uint32_t dbg_en:1;        /**< 0:disable, 1:enable, default is disable. */
    void    *semaphore;
    void    *sem_blking;      /**< Reserved. */
    ctrl_wifi_mac_tx_stats_t stats;
    ctrl_wifi_mac_tx_info_t  no_acked_tmp;
    ctrl_wifi_mac_tx_list_t *list;
} ctrl_wifi_mac_tx_mgmt_t;

typedef struct
{
    void    *tx_buf;          /**< For WPA3 used. SAE commit. STA -> AP */
    uint32_t tx_buf_len;
    void    *rx_buf;          /**< For WPA3 used. SAE commit. AP  -> STA */
    uint32_t rx_buf_len;
    uint16_t status;          /**< Authentication status code, range is 0 - 501 */
} ctrl_wifi_auth_data_t;

typedef struct
{
    void    *pScanInfo;       /**< Pointer to scan result which index. */
    uint32_t auth_type;       /**< 0:OPEN, 1:WPA, 2:WPA2, 3:WPA3 */
    void    *wpa_rsn_buf;     /**< WPA/RSN buffer, created by APS. */
    uint32_t wpa_rsn_len;     /**< 0 = Non-Security */
    ctrl_wifi_auth_data_t      auth_data;
} ctrl_wifi_connect_to_msq_t;

typedef struct
{
    uint16_t status;          /**< Association status code, range is 0 - 203 */
    uint16_t aid:12;          /**< Association ID. (12 bits, 0 - 4095) */
    uint16_t resvered:4;
} ctrl_wifi_assoc_info_t;

typedef struct
{
    /**
     * Country code (String):
     *    "CN", "US", "JP", "EU"
     */
    char cc[3];

    /**
     * Start from channel 1.
     */
    uint8_t start_chan;

    /**
     * Number of Channel:
     *    "CN" : 13
     *    "US" : 11
     *    "JP" : 14
     *    "EU" : 13
     */
    uint8_t num_chan;
} ctrl_wifi_country_info_t;

/*
 * Deprecation
 */
typedef struct _wpa_ie_data
{
    int proto;
    int pairwise_cipher;
    int group_cipher;
    int key_mgmt;
    int capabilities;
    size_t num_pmkid;
    const uint8_t *pmkid;
    int mgmt_group_cipher;
}wpa_ie_data_t;

typedef struct {
    uint8_t max_num_scan_results;
} ctrl_wifi_resource_t;

typedef struct {
    uint32_t  flags;
    uint8_t   ssid[CTRL_WIFI_MAX_SSID_LEN];
    uint8_t   ssid_len;
    uint8_t   bssid[CTRL_WIFI_MAC_ADDR_LEN];
    int       channel;
    uint16_t  beacon_int;
    uint16_t  dtim_period;
    uint16_t  caps;
    int       noise;
    int       level;
    uint64_t  tsf;
    uint32_t  age;
    uint64_t  parent_tsf;
    uint8_t   tsf_bssid[CTRL_WIFI_MAC_ADDR_LEN];
    size_t    ie_len;
    size_t    beacon_ie_len;
    uint8_t   ie_buf[100];                              /**< followed by ie_len octets of IEs */
    /**< followed by beacon_ie_len octets of IEs */
} ctrl_wifi_scan_res_t;

typedef struct
{
    bool          free_ocpy;                            /**< scan info buffer is free or occupied, 0:free, 1:occupied */
    uint8_t       bssid[CTRL_WIFI_MAC_ADDR_LEN];        /**< BSS ID - 48 bit HW address */
    uint8_t       ap_channel;                           /**< Which Channel */
    uint64_t      latest_beacon_rx_time;                /**< Timestamp - Last interaction with BSS */
    char          ssid[CTRL_WIFI_MAX_SSID_LEN + 1];     /**< SSID of the BSS - 33 bytes */
    uint8_t       ssid_len;                             /**< Length of SSID */
    int8_t        rssi;                                 /**< Last observed Rx Power (dBm) */
    uint16_t      beacon_interval;                      /**< Beacon interval - In time units of 1024 us */
    uint16_t      capabilities;                         /**< Supported capabilities */
    uint8_t       dtim_prod;                            /**< DTIM Period */
    wpa_ie_data_t wpa_data;                             /**< Deprecation */
    uint8_t       rsn_ie[256];                          /**< ELEMENT_HEADER_LEN + IEEE80211_MAX_RSN_LEN */
    uint8_t       wpa_ie[257];                          /**< ELEMENT_HEADER_LEN + IEEE80211_MAX_WPA_LEN */
} ctrl_wifi_ap_info_t;

typedef struct
{
    uint32_t  uScanApNum;
    ctrl_wifi_ap_info_t  *pScanInfo;
}ctrl_wifi_scan_report_t;

/** The parameter of STA_MCMD_SCAN */
typedef struct {
    ctrl_wifi_scan_report_t *ptScanReport;          /**< The scan report which filled by MSQ, report to APS */
    uint32_t u32ActiveScanDur;                      /**< Scan duration per scan counter in channel. units: millisecond */
    uint32_t u32PassiveScanDur;                     /**< Scan duration per channel. units: millisecond */
    uint8_t tScanType;                              /**< scan type. active, passive, or mix mode */
    uint8_t u8Channel;                              /**< Only specific channel or scan all channels */
    uint8_t u8aSsid[CTRL_WIFI_MAX_SSID_LEN + 1];    /**< Not supported yet. SSID of AP */
    uint8_t u8aBssid[CTRL_WIFI_MAC_ADDR_LEN];       /**< Not supported yet. MAC address of AP */
    uint8_t u8MaxScanApNum;                         /**< Max scan AP number. When scanned AP number over this value,
                                                         MSQ will drop the AP with smallest RSSI value */
    uint8_t u8ResendCnt;                            /**< Send probe req counter per channel when active scan.
                                                         After send probe req, it will wait active scan time,
                                                         and then send next probe req.
                                                         The total time will be increased by a factor of this value */
} S_WIFI_MLME_SCAN_CFG;

typedef struct {
    int num_of_ipc_cmd_retry;
} ctrl_wifi_ipc_sync_t;

typedef struct {
    uint32_t mac_tx_data_rate;
    uint32_t skip_dtim;
    uint8_t  mac_addr[CTRL_WIFI_MAC_ADDR_LEN];
    ctrl_wifi_country_info_t country;
    void     *p_ext;
} ctrl_wifi_sta_params_t;

typedef struct {
    uint8_t  do_loop;
    uint32_t start_ch;
    uint32_t end_ch;
} ctrl_wifi_loop_scan_t;

/**
 * The configuration of connecting to AP parameters by User specified.
 */
typedef struct {
    uint8_t  do_before_scan;
    uint8_t  ssid[CTRL_WIFI_MAX_SSID_LEN];
    uint8_t  ssid_len;
    uint8_t  bssid[CTRL_WIFI_MAC_ADDR_LEN];
    uint8_t  channel;
    uint32_t auth_type;       /**< 0:OPEN, 1:WPA, 2:WPA2, 3:WPA3 */
    void    *wpa_rsn_buf;     /**< WPA/RSN buffer, created by APS. */
    uint32_t wpa_rsn_len;     /**< 0 = Non-Security */
    void    *sae_commit_buf;  /**< For WPA3 used. SAE commit. STA -> AP */
    uint32_t sae_commit_buf_len;
    void    *sae_confirm_buf;
    uint32_t sae_confirm_buf_len;
} ctrl_wifi_connect_cfg_t;

typedef struct
{
    int (*scan_done)  (void *argu, uint8_t ap_num, int reason);
    int (*add_bss)    (void *argu);
    int (*prob_start) (void *argu, void *buf, uint32_t buf_len);
    int (*auth_stage) (void *argu, void *buf, uint32_t buf_len);
    int (*assoc_stage)(void *argu, void *buf, uint32_t buf_len);
    int (*disconnect) (void *argu, int reason);
    int (*set_param)  (void *argu, void *buf, uint32_t buf_len);
} ctrl_wifi_notify_cb_t;

/*
 * This structure contains the internal data for core ctrl wifi used.
 */
typedef struct
{
    uint8_t sta_state;
    ctrl_wifi_resource_t       res;
    S_WIFI_MLME_SCAN_CFG       scan_cfg;
    ctrl_wifi_ipc_sync_t       ipc_sync;
    ctrl_wifi_sta_params_t     sta_params;
    ctrl_wifi_scan_report_t    scan_report;
    ctrl_wifi_loop_scan_t      loop_scan;
    ctrl_wifi_connect_to_msq_t conn_cfg;
    ctrl_wifi_mac_tx_mgmt_t    tx_mgmt;
    ctrl_wifi_notify_cb_t      cb;
} ctrl_wifi_t;

typedef void (*ctrl_wifi_handler_fp)(void *ctx, void *data, int len);
typedef struct
{
    uint32_t ulEventId;
    ctrl_wifi_handler_fp fpFunc;
} ctrl_wifi_handler_tb_t;

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern ctrl_wifi_handler_tb_t g_ctrl_wifi_cmd_table[];
extern ctrl_wifi_handler_tb_t g_ctrl_wifi_evt_table[];

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
int ctrl_wifi_register_scan_done_cb(int (*cb)(void *argu, uint8_t ap_num, int reason));
int ctrl_wifi_register_add_bss_cb(int (*cb)(void *argu));
int ctrl_wifi_register_probe_start_cb(int (*cb)(void *argu, void *buf, uint32_t buf_len));
int ctrl_wifi_register_auth_stage_cb(int (*cb)(void *argu, void *buf, uint32_t buf_len));
int ctrl_wifi_register_assoc_stage_cb(int (*cb)(void *argu, void *buf, uint32_t buf_len));
int ctrl_wifi_register_disconnect_cb(int (*cb)(void *argu, int reason));
int ctrl_wifi_register_set_param_cb(int (*cb)(void *argu, void *buf, uint32_t buf_len));

/********** Rom patch interface **********/
typedef char *(*ctrl_wifi_state_txt_fp_t)(int st);
typedef int   (*ctrl_wifi_set_state_fp_t)(ctrl_wifi_t *p_ctrl, int st, int status);
typedef int   (*ctrl_wifi_get_state_fp_t)(ctrl_wifi_t *p_ctrl);
typedef int   (*ctrl_wifi_scan_resource_init_fp_t)(ctrl_wifi_t *p_ctrl);
typedef void  (*ctrl_wifi_scan_cfg_init_fp_t)(ctrl_wifi_t *p_ctrl, S_WIFI_MLME_SCAN_CFG *pScanCfg);
typedef void  (*ctrl_wifi_sta_info_init_fp_t)(void);
typedef int   (*ctrl_wifi_cmd_handler_fp_t)(S_WIFI_CMD *pWifiCmd);
typedef int   (*ctrl_wifi_evt_handler_fp_t)(S_WIFI_EVT *pWifiEvt);
typedef int   (*ctrl_wifi_ipc_cmd_send_fp_t)(ctrl_wifi_t *p_ctrl, S_WIFI_CMD *pWifiCmd);
typedef int   (*ctrl_wifi_config_ipc_send_fp_t)(ctrl_wifi_t *p_ctrl, uint32_t idx, void *val);
typedef int   (*ctrl_wifi_sta_mac_addr_init_fp_t)(ctrl_wifi_t *p_ctrl);
typedef void  (*ctrl_wifi_scan_default_fp_t)(ctrl_wifi_t *p_ctrl, S_WIFI_MLME_SCAN_CFG *cfg);
typedef int   (*ctrl_wifi_scan_result_add_fp_t)(ctrl_wifi_t *p_ctrl);
typedef int   (*ctrl_wifi_stop_loop_scan_fp_t)(ctrl_wifi_t *p_ctrl);
typedef int   (*ctrl_wifi_do_loop_scan_fp_t)(ctrl_wifi_t *p_ctrl);
typedef int   (*ctrl_wifi_set_ps_force_fp_t)(E_WIFI_STA_MODE mode);
typedef int   (*ctrl_wifi_scan_start_fp_t)(uint8_t *bssid, uint8_t *ssid, uint8_t channel, uint8_t scan_type, uint16_t scan_time, uint8_t loop_scan);
typedef int   (*ctrl_wifi_scan_stop_fp_t)(void *argu);
typedef int   (*ctrl_wifi_connect_fp_t)(ctrl_wifi_connect_cfg_t *p_cfg_in);
typedef int   (*ctrl_wifi_sae_command_fp_t)(ctrl_wifi_connect_cfg_t *p_cfg_in);
typedef int   (*ctrl_wifi_probe_before_connect_fp_t)(ctrl_wifi_connect_cfg_t *p_cfg_in);
typedef int   (*ctrl_wifi_auth_confirm_request_fp_t)(void *buf, uint32_t buf_len);
typedef int   (*ctrl_wifi_auth_confirm_done_fp_t)(void);
typedef int   (*ctrl_wifi_disconnect_fp_t)(uint16_t reason);
typedef int   (*ctrl_wifi_sta_join_complete_fp_t)(uint8_t is_success);
typedef int   (*ctrl_wifi_bssid_get_fp_t)(uint8_t *p_bssid);
typedef int   (*ctrl_wifi_sta_mac_address_get_fp_t)(uint8_t *p_mac);
typedef int   (*ctrl_wifi_set_country_info_fp_t)(ctrl_wifi_country_info_t *info);
typedef int   (*ctrl_wifi_get_country_info_fp_t)(ctrl_wifi_country_info_t *info);
typedef int   (*ctrl_wifi_set_sta_param_fp_t)(E_WIFI_PARAM_ID idx, uint32_t *val);
typedef int   (*ctrl_wifi_get_sta_param_fp_t)(E_WIFI_PARAM_ID idx, uint32_t *val);
typedef int   (*ctrl_wifi_reset_to_idle_fp_t)(void);
typedef int   (*ctrl_wifi_tx_info_semaphore_free_fp_t)(ctrl_wifi_t *p_ctrl);
typedef int   (*ctrl_wifi_tx_info_list_all_free_fp_t)(ctrl_wifi_t *p_ctrl);
typedef int   (*ctrl_wifi_tx_info_init_fp_t)(ctrl_wifi_t *p_ctrl);
typedef void  (*ctrl_wifi_tx_info_stats_cal_by_len_fp_t)(ctrl_wifi_mac_tx_mgmt_t *p_mgmt, ctrl_wifi_mac_tx_list_t *p_list, uint32_t len);
typedef void  (*ctrl_wifi_tx_info_stats_cal_fp_t)(ctrl_wifi_mac_tx_info_t *p_info);
typedef void  (*ctrl_wifi_tx_info_print_fp_t)(ctrl_wifi_mac_tx_info_t *p_info);
typedef void *(*ctrl_wifi_tx_info_buf_allocate_fp_t)(uint8_t acked, uint32_t length);
typedef int   (*ctrl_wifi_tx_info_list_clear_fp_t)(uint32_t sid);
typedef int   (*ctrl_wifi_get_tx_info_dbg_en_fp_t)(void);
typedef int   (*ctrl_wifi_set_tx_info_dbg_en_fp_t)(void);
typedef int   (*ctrl_wifi_tx_info_stats_clear_fp_t)(void);
typedef void  (*ctrl_wifi_tx_info_stats_dump_fp_t)(void);
typedef void  (*ctrl_wifi_tx_info_dump_fp_t)(void);

typedef int   (*CtrlWifi_PsStateForce_fp_t)(E_WIFI_STA_MODE mode, uint32_t timeout);


/********** Export interface funtion pointer **********/
extern ctrl_wifi_sta_info_init_fp_t            ctrl_wifi_sta_info_init;
extern ctrl_wifi_cmd_handler_fp_t              ctrl_wifi_cmd_handler;
extern ctrl_wifi_evt_handler_fp_t              ctrl_wifi_evt_handler;
extern ctrl_wifi_set_ps_force_fp_t             ctrl_wifi_set_ps_force;
extern ctrl_wifi_scan_start_fp_t               ctrl_wifi_scan_start;
extern ctrl_wifi_scan_stop_fp_t                ctrl_wifi_scan_stop;
extern ctrl_wifi_connect_fp_t                  ctrl_wifi_connect;
extern ctrl_wifi_sae_command_fp_t              ctrl_wifi_sae_command;
extern ctrl_wifi_probe_before_connect_fp_t     ctrl_wifi_probe_before_connect;
extern ctrl_wifi_auth_confirm_request_fp_t     ctrl_wifi_auth_confirm_request;
extern ctrl_wifi_auth_confirm_done_fp_t        ctrl_wifi_auth_confirm_done;
extern ctrl_wifi_disconnect_fp_t               ctrl_wifi_disconnect;
extern ctrl_wifi_sta_join_complete_fp_t        ctrl_wifi_sta_join_complete;
extern ctrl_wifi_bssid_get_fp_t                ctrl_wifi_bssid_get;
extern ctrl_wifi_sta_mac_address_get_fp_t      ctrl_wifi_sta_mac_address_get;
extern ctrl_wifi_set_country_info_fp_t         ctrl_wifi_set_country_info;
extern ctrl_wifi_get_country_info_fp_t         ctrl_wifi_get_country_info;
extern ctrl_wifi_set_sta_param_fp_t            ctrl_wifi_set_sta_param;
extern ctrl_wifi_get_sta_param_fp_t            ctrl_wifi_get_sta_param;
extern ctrl_wifi_reset_to_idle_fp_t            ctrl_wifi_reset_to_idle;
extern ctrl_wifi_tx_info_semaphore_free_fp_t   ctrl_wifi_tx_info_semaphore_free;
extern ctrl_wifi_tx_info_list_all_free_fp_t    ctrl_wifi_tx_info_list_all_free;
extern ctrl_wifi_tx_info_init_fp_t             ctrl_wifi_tx_info_init;
extern ctrl_wifi_tx_info_stats_cal_by_len_fp_t ctrl_wifi_tx_info_stats_cal_by_len;
extern ctrl_wifi_tx_info_stats_cal_fp_t        ctrl_wifi_tx_info_stats_cal;
extern ctrl_wifi_tx_info_print_fp_t            ctrl_wifi_tx_info_print;
extern ctrl_wifi_tx_info_buf_allocate_fp_t     ctrl_wifi_tx_info_buf_allocate;
extern ctrl_wifi_tx_info_list_clear_fp_t       ctrl_wifi_tx_info_list_clear;
extern ctrl_wifi_get_tx_info_dbg_en_fp_t       ctrl_wifi_get_tx_info_dbg_en;
extern ctrl_wifi_set_tx_info_dbg_en_fp_t       ctrl_wifi_set_tx_info_dbg_en;
extern ctrl_wifi_tx_info_stats_clear_fp_t      ctrl_wifi_tx_info_stats_clear;
extern ctrl_wifi_tx_info_stats_dump_fp_t       ctrl_wifi_tx_info_stats_dump;
extern ctrl_wifi_tx_info_dump_fp_t             ctrl_wifi_tx_info_dump;

extern CtrlWifi_PsStateForce_fp_t              CtrlWifi_PsStateForce;


#ifdef __cplusplus
}
#endif

#include "controller_wifi_patch.h"
#endif  /* __CONTROLLER_WIFI_H__ */
