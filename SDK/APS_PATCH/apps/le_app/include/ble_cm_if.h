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

#ifndef _LE_CM_IF_H_
#define _LE_CM_IF_H_

#include "ble.h"
#include "ble_msg.h"

#define LE_CM_MSG_TOP 	(LE_CM_MSG_BASE + 0xFF)


enum
{
	LE_CM_MSG_INIT_COMPLETE_CFM = LE_CM_MSG_BASE,

	LE_CM_MSG_SET_DISCONNECT_CFM,
	LE_CM_MSG_DISCONNECT_COMPLETE_IND,

	LE_CM_MSG_SET_ADVERTISING_DATA_CFM,
	LE_CM_MSG_SET_SCAN_RSP_DATA_CFM,
	LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM,
	LE_CM_MSG_ENTER_ADVERTISING_CFM,
	LE_CM_MSG_EXIT_ADVERTISING_CFM,

	LE_CM_MSG_SET_SCAN_PARAMS_CFM,
	LE_CM_MSG_ENTER_SCANNING_CFM,
	LE_CM_MSG_EXIT_SCANNING_CFM,
    
	LE_CM_MSG_CREATE_CONNECTION_CFM,
	LE_CM_MSG_CANCEL_CONNECTION_CFM,

	LE_CM_MSG_READ_TX_POWER_CFM,
	LE_CM_MSG_READ_BD_ADDR_CFM,
    
	LE_CM_MSG_READ_RSSI_CFM,
	LE_CM_MSG_SET_RANDOM_ADDRESS_CFM,
	LE_CM_MSG_READ_ADV_TX_POWER_CFM,

	LE_CM_MSG_READ_WHITE_LIST_SIZE_CFM,
	LE_CM_MSG_CLEAR_WHITE_LIST_CFM,
    LE_CM_MSG_ADD_TO_WHITE_LIST_CFM,
	LE_CM_MSG_REMOVE_FROM_WHITE_LIST_CFM,

	LE_CM_MSG_SET_CHANNEL_MAP_CFM,
	LE_CM_MSG_READ_CHANNEL_MAP_CFM,

	LE_CM_MSG_SET_DATA_LENGTH_CFM,
	LE_CM_MSG_DATA_LEN_CHANGE_IND,

	LE_CM_MSG_ADD_TO_RESOLVING_LIST_CFM,
	LE_CM_MSG_REMOVE_FROM_RESOLVING_LIST_CFM,
	LE_CM_MSG_CLEAR_RESOLVING_LIST_CFM,
	LE_CM_MSG_READ_RESOLVING_LIST_SIZE_CFM,
	LE_CM_MSG_SET_RPA_TIMEOUT_CFM,

	LE_CM_MSG_READ_REMOTE_VERSION_CFM,
	LE_CM_MSG_REMOTE_VERSION_IND,

    LE_CM_MSG_SIG_CONN_UPDATE_REQ,
//	LE_CM_MSG_SIG_CONN_UPDATE_RSP,

    LE_CM_MSG_CONN_UPDATE_CFM,
    LE_CM_MSG_CONN_UPDATE_COMPLETE_IND,
    LE_CM_MSG_CONN_PARA_REQ,

	LE_CM_MSG_ENCRYPTION_CHANGE_IND,
	LE_CM_MSG_ENCRYPTION_REFRESH_IND,
	LE_CM_MSG_LTK_REQ_IND,

	LE_CM_MSG_ADVERTISE_REPORT_IND,
	LE_CM_MSG_DIRECT_ADV_REPORT_IND,
    LE_CM_MSG_CONNECTION_COMPLETE_IND,
	LE_CM_MSG_READ_LOCAL_RPA_CFM,

	LE_CM_MSG_READ_PHY_CFM,
	LE_CM_MSG_SET_DEFAULT_PHY_CFM,
	LE_CM_MSG_SET_PHY_CFM,
	LE_CM_MSG_PHY_UPDATE_COMPLETE_IND,

#ifdef LE_FEATURE_CBC
	LE_CM_MSG_CBC_CONNECT_IND,
	LE_CM_MSG_CBC_CONNECT_COMPLETE_IND,
	LE_CM_MSG_CBC_TX_CFM,
	LE_CM_MSG_CBC_DISCONNECT_COMPLETE_IND,
	LE_CM_MSG_CBC_CREDITS_IND,
	LE_CM_MSG_CBC_MORE_DATA_IND,
#endif

#ifdef BLE_FEATURE_5P0
	LE_CM_MSG_CREATE_EXT_ADV_CFM,
	LE_CM_MSG_SET_ADV_SET_RANDOM_ADDRESS_CFM,
	LE_CM_MSG_SET_EXT_ADV_PARAMS_CFM,
	LE_CM_MSG_SET_EXT_ADV_DATA_CFM,
	LE_CM_MSG_SET_EXT_SCAN_RSP_DATA_CFM,
	LE_CM_MSG_SET_EXT_ADVERTISING_ENABLE_CFM,
	LE_CM_MSG_SET_EXT_ADVERTISING_DISABLE_CFM,
	LE_CM_MSG_ADVERTISING_SET_TERMINATED_IND,
	LE_CM_MSG_SCAN_REQ_RECEIVED_IND,
	
	LE_CM_MSG_READ_MAX_ADV_DATA_LEN_CFM,
	LE_CM_MSG_READ_NUM_SUPPORT_ADV_SETS_CFM,
	LE_CM_MSG_REMOVE_ADV_SET_CFM,
	LE_CM_MSG_CLEAR_ADV_SETS_CFM,
	
	LE_CM_MSG_SET_PERIODIC_ADV_PARAMS_CFM,
	LE_CM_MSG_SET_PERIODIC_ADV_DATA_CFM,
	LE_CM_MSG_SET_PERIODIC_ADV_ENABLE_CFM,

	LE_CM_MSG_SET_EXT_SCAN_PARAMS_CFM,
	LE_CM_MSG_SET_EXT_SCAN_ENABLE_CFM,
	LE_CM_MSG_SET_EXT_SCAN_DISABLE_CFM,
	LE_CM_MSG_SCANNING_TIMEOUT_IND,
	LE_CM_MSG_EXTENDED_ADV_REPORT_IND,
	LE_CM_MSG_EXTENDED_SCAN_RSP_REPORT_IND,
	
	LE_CM_MSG_CREATE_EXT_CONNECTION_CFM,
	LE_CM_MSG_CANCEL_EXT_CONNECTION_CFM,

	LE_CM_MSG_PERIODIC_ADV_CREATE_SYNC_CFM,
	LE_CM_MSG_PERIODIC_ADV_CREATE_SYNC_CANCEL_CFM,
	LE_CM_MSG_PERIODIC_ADV_TEMINATE_SYNC_CFM,
	LE_CM_MSG_PERIODIC_ADV_SYNC_ESTABLISH_IND,
	LE_CM_MSG_PERIODIC_ADV_REPORT_IND,
	LE_CM_MSG_PERIODIC_ADV_SYNC_LOST_IND,

	LE_CM_MSG_ADD_DEV_TO_PERIODIC_ADV_LIST_CFM,
	LE_CM_MSG_REMOVE_DEV_FROM_PERIODIC_ADV_LIST_CFM,
	LE_CM_MSG_CLEAR_PERIODIC_ADV_LIST_CFM,
	LE_CM_MSG_READ_PERIODIC_ADV_LIST_SIZE_CFM,
	LE_CM_MSG_LE_READ_TX_POWER_RANGE_CFM,

	LE_CM_MSG_READ_RF_PATH_COMPENSATION_CFM,
	LE_CM_MSG_WRITE_RF_PATH_COMPENSATION_CFM,
	LE_CM_MSG_SET_PRIVACY_MODE_CFM,

	LE_CM_MSG_CHANNEL_SELECT_ALGORITHM_IND,

#endif
};


 typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_RANDOM_ADDRESS_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_ADVERTISING_DATA_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_ADVERTISING_PARAMS_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_ENTER_ADVERTISING_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_EXIT_ADVERTISING_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_SCAN_RSP_DATA_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_SCAN_PARAMS_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_ENTER_SCANNING_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_EXIT_SCANNING_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_CREATE_CONNECTION_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_CANCEL_CONNECTION_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_CLEAR_WHITE_LIST_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_CHANNEL_MAP_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_CLEAR_RESOLVING_LIST_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_RPA_TIMEOUT_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_DEFAULT_PHY_CFM_T;
 
#ifdef BLE_FEATURE_5P0

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_CLEAR_ADV_SETS_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_EXT_SCAN_PARAMS_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_EXT_SCAN_ENABLE_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_SET_EXT_SCAN_DISABLE_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_CREATE_EXT_CONNECTION_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_CANCEL_EXT_CONNECTION_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_PERIODIC_ADV_CREATE_SYNC_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_PERIODIC_ADV_CREATE_SYNC_CANCEL_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_CLEAR_PERIODIC_ADV_LIST_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
} LE_CM_MSG_WRITE_RF_PATH_COMPENSATION_CFM_T;

 
#endif

typedef struct
{
	LE_ERR_STATE		status;
#ifdef BLE_FEATURE_5P0
    UINT8				hci_ver;
    UINT16				hci_rever;
    BD_ADDR				public_addr;
	UINT16				wl_size;
	UINT16				rl_size;
	UINT16				conn_size;
	UINT16				max_adv_len;
	UINT8				support_adv_sets;
#endif
} LE_CM_MSG_INIT_COMPLETE_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				handle;
} LE_CM_MSG_SET_DISCONNECT_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				handle;
} LE_CM_MSG_READ_REMOTE_VERSION_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				conn_hdl;
    INT8				tx_power;
} LE_CM_MSG_READ_TX_POWER_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    BD_ADDR				bd_addr;
} LE_CM_MSG_READ_BD_ADDR_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				conn_hdl;
    INT8				rssi;
} LE_CM_MSG_READ_RSSI_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    INT8				pwr_level;
} LE_CM_MSG_READ_ADV_TX_POWER_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				size;
} LE_CM_MSG_READ_WHITE_LIST_SIZE_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				addr_type;
    BD_ADDR				addr;
} LE_CM_MSG_ADD_TO_WHITE_LIST_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				addr_type;
    BD_ADDR				addr;
} LE_CM_MSG_REMOVE_FROM_WHITE_LIST_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				addr_type;
    BD_ADDR				addr;
} LE_CM_MSG_ADD_TO_RESOLVING_LIST_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				addr_type;
    BD_ADDR				addr;
} LE_CM_MSG_REMOVE_FROM_RESOLVING_LIST_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				conn_hdl;
    UINT8				ch_map[5];
} LE_CM_MSG_READ_CHANNEL_MAP_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				conn_hdl;
} LE_CM_MSG_SET_DATA_LENGTH_CFM_T;

typedef struct
{
    UINT16				conn_hdl;
    UINT16				max_tx_octets;
    UINT16				max_tx_time;
    UINT16				max_rx_octets;
    UINT16				max_rx_time;
} LE_CM_MSG_DATA_LEN_CHANGE_IND_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				size;
} LE_CM_MSG_READ_RESOLVING_LIST_SIZE_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT16				conn_hdl;
	UINT8				reason;
} LE_CM_MSG_DISCONNECT_COMPLETE_IND_T;

typedef struct
{
	UINT8				event_type;
	UINT8				addr_type;
	BD_ADDR				addr;
    INT8				rssi;
    UINT8				len;
    UINT8				data[1];
} LE_CM_MSG_ADVERTISE_REPORT_IND_T;

typedef struct
{
	UINT8				peer_addr_type;
	BD_ADDR				peer_addr;
	UINT8				direct_addr_type;
	BD_ADDR				direct_addr;
    INT8				rssi;
} LE_CM_MSG_DIRECT_ADV_REPORT_IND_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT16				conn_hdl;
    UINT16				dev_id;
	UINT8				role;
	UINT8				peer_addr_type;
	BD_ADDR				peer_addr;
	BD_ADDR				local_rpa;
	BD_ADDR				peer_rpa;
	UINT16				conn_interval;
	UINT16				conn_latency;
	UINT16				supervison_timeout;
} LE_CM_MSG_CONNECTION_COMPLETE_IND_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT16				conn_hdl;
    UINT16				dev_id;
	UINT8				version;
	UINT16				manufacturer_name;
	UINT16				subversion;
} LE_CM_MSG_REMOTE_VERSION_IND_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT16				conn_hdl;
    UINT16				devid;
    BOOL				enabled;
} LE_CM_MSG_ENCRYPTION_REFRESH_IND_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT16				conn_hdl;
    UINT16				devid;
	UINT8				enabled;
} LE_CM_MSG_ENCRYPTION_CHANGE_IND_T;

typedef struct
{
	UINT16				conn_hdl;
    UINT16				devid;
    UINT8				rand[8];
	UINT16				ediv;
} LE_CM_MSG_LTK_REQ_IND_T;

typedef struct
{
	UINT16				conn_hdl;
	UINT16				identifier;
    UINT16 				interval_min;
    UINT16 				interval_max;
    UINT16 				slave_latency;
    UINT32 				timeout_multiplier;
} LE_CM_MSG_SIG_CONN_UPDATE_REQ_T;

typedef struct
{
	UINT16				conn_hdl;
	BOOL				accept;
} LE_CM_MSG_SIG_CONN_UPDATE_RSP_T;

typedef struct
{
    LE_ERR_STATE		status;
	UINT16				conn_hdl;
} LE_CM_MSG_CONN_UPDATE_CFM_T;

typedef struct
{
    LE_ERR_STATE		status;
	UINT16				conn_hdl;
    UINT16 				interval;
    UINT16 				latency;
    UINT32 				supervision_timeout;
} LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T;

typedef struct
{
	UINT16				conn_hdl;
	UINT16				itv_min;
    UINT16 				itv_max;
    UINT16 				latency;
    UINT32 				sv_tmo;
} LE_CM_MSG_CONN_PARA_REQ_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				conn_hdl;
    UINT8				tx_phy;
    UINT8				rx_phy;
} LE_CM_MSG_READ_PHY_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				conn_hdl;
} LE_CM_MSG_SET_PHY_CFM_T;

typedef LE_CM_MSG_READ_PHY_CFM_T LE_CM_MSG_PHY_UPDATE_COMPLETE_IND_T;

#ifdef LE_FEATURE_CBC

typedef struct
{
	UINT16				conn_hdl;
	UINT16				cid;
    UINT16 				psm;
    UINT16 				mtu;
    UINT16 				init_credit;
} LE_CM_MSG_CBC_CONNECT_IND_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT16				conn_hdl;
	UINT16				cid;
    UINT16 				mtu;
    UINT16 				mps;
    UINT16				peer_credit;
    UINT16				local_credit;
} LE_CM_MSG_CBC_CONNECT_COMPLETE_IND_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT16				conn_hdl;
	UINT16				cid;
    UINT16				peer_credit;
} LE_CM_MSG_CBC_TX_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT16				conn_hdl;
	UINT16				cid;
} LE_CM_MSG_CBC_DISCONNECT_COMPLETE_IND_T;

typedef struct
{
	UINT16				conn_hdl;
	UINT16				cid;
    UINT16				peer_credit;
    UINT16				local_credit;
} LE_CM_MSG_CBC_CREDITS_IND_T;

typedef struct
{
	UINT16				conn_hdl;
	UINT16				cid;
    UINT16				local_credit;
    UINT16				len;
    UINT8				*data;
} LE_CM_MSG_CBC_MORE_DATA_IND_T;

#endif

#ifdef BLE_FEATURE_5P0

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				adv_hdl;
    UINT8				sid;
    INT8				tx_pwr;
} LE_CM_MSG_CREATE_EXT_ADV_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				adv_hdl;
} LE_CM_MSG_SET_ADV_SET_RANDOM_ADDRESS_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				adv_hdl;
    INT8				tx_pwr;
} LE_CM_MSG_SET_EXT_ADV_PARAMS_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				adv_hdl;
    UINT8				op;
} LE_CM_MSG_SET_EXT_ADV_DATA_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				adv_hdl;
    UINT8				op;
} LE_CM_MSG_SET_EXT_SCAN_RSP_DATA_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				adv_hdl;
} LE_CM_MSG_SET_EXT_ADVERTISING_ENABLE_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    BOOL				all;
    UINT8				adv_hdl;
} LE_CM_MSG_SET_EXT_ADVERTISING_DISABLE_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				max_adv_len;
} LE_CM_MSG_READ_MAX_ADV_DATA_LEN_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				max_adv_sets;
} LE_CM_MSG_READ_NUM_SUPPORT_ADV_SETS_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				adv_hdl;
} LE_CM_MSG_REMOVE_ADV_SET_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				adv_hdl;
} LE_CM_MSG_SET_PERIODIC_ADV_PARAMS_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				adv_hdl;
    UINT8				op;
} LE_CM_MSG_SET_PERIODIC_ADV_DATA_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				adv_hdl;
    BOOL				enable;
} LE_CM_MSG_SET_PERIODIC_ADV_ENABLE_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				sync_hdl;
	UINT8				sid;
	UINT8				addr_type;
	BD_ADDR				addr;
	UINT8				phy;
	UINT16				interval;
	UINT8				accuracy;
} LE_CM_MSG_PERIODIC_ADV_SYNC_ESTABLISH_IND_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT16				sync_hdl;
} LE_CM_MSG_PERIODIC_ADV_TEMINATE_SYNC_CFM_T;

typedef struct
{
    UINT16				sync_hdl;
} LE_CM_MSG_PERIODIC_ADV_SYNC_LOST_IND_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT8				sid;
	UINT8				addr_type;
	BD_ADDR				addr;
} LE_CM_MSG_ADD_DEV_TO_PERIODIC_ADV_LIST_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT8				sid;
	UINT8				addr_type;
	BD_ADDR				addr;
} LE_CM_MSG_REMOVE_DEV_FROM_PERIODIC_ADV_LIST_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    UINT8				size;
} LE_CM_MSG_READ_PERIODIC_ADV_LIST_SIZE_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    INT8				pwr_min;
    INT8				pwr_max;
} LE_CM_MSG_LE_READ_TX_POWER_RANGE_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
    INT16				tx;
    INT16				rx;
} LE_CM_MSG_READ_RF_PATH_COMPENSATION_CFM_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT8				addr_type;
	BD_ADDR				addr;
	UINT8				mode;
} LE_CM_MSG_SET_PRIVACY_MODE_CFM_T;

typedef struct
{
	UINT8				rpt_type;
	BOOL				adv_dir;
    UINT8				data_status;
	UINT8				addr_type;
	BD_ADDR				addr;
	UINT8				prim_phy;
	UINT8				sec_phy;
	UINT8				sid;
    INT8				tx_pwr;
	INT8				rssi;
	UINT16				periodic_adv_itvl;
	UINT8				dir_addr_type;
	BD_ADDR				dir_addr;
    UINT8				len;
    UINT8				*data;
} LE_CM_MSG_EXTENDED_ADV_REPORT_IND_T;

typedef struct
{
	UINT8				type;
    UINT8				data_status;
	UINT8				addr_type;
	BD_ADDR				addr;
	UINT8				prim_phy;
	UINT8				sec_phy;
	UINT8				sid;
    INT8				tx_pwr;
	INT8				rssi;
	UINT16				periodic_adv_itvl;
	UINT8				dir_addr_type;
	BD_ADDR				dir_addr;
    UINT8				len;
    UINT8				*data;
} LE_CM_MSG_EXTENDED_SCAN_RSP_REPORT_IND_T;

typedef struct
{
	UINT16				sync_hdl;
    UINT8				data_state;
    UINT8				len;
    UINT8				*data;
	INT8				tx_pwr;
	INT8				rssi;
    UINT8				unused;
} LE_CM_MSG_PERIODIC_ADV_REPORT_IND_T;

typedef struct
{
	LE_ERR_STATE		status;
	UINT8				adv_hdl;
    UINT16				conn_hdl;
    UINT8				num_adv_evt;
} LE_CM_MSG_ADVERTISING_SET_TERMINATED_IND_T;

typedef struct
{
	UINT8				adv_hdl;
    UINT8				scanner_addr_type;
	BD_ADDR				scanner_addr;
} LE_CM_MSG_SCAN_REQ_RECEIVED_IND_T;

typedef struct
{
    UINT16				conn_hdl;
	UINT8				algorithm;
} LE_CM_MSG_CHANNEL_SELECT_ALGORITHM_IND_T;

#endif

LE_ERR_STATE LeHostInit(TASK appTask);

#ifdef LE_FEATURE_CBC

LE_ERR_STATE LeCmCbcRegisterPsm(UINT16 psm, UINT16 credit);

LE_ERR_STATE LeCmCbcUnRegisterPsm(UINT16 psm);

LE_ERR_STATE LeCmCbcConnectReq(UINT16 conn_hdl, UINT16 mtu, UINT16 local_psm, UINT16 remote_psm);

LE_ERR_STATE LeCmCbcConnectRsp(UINT16 conn_hdl, UINT16 cid, UINT16 mtu, UINT16 result);

LE_ERR_STATE LeCmCbcSendData(UINT16 conn_hdl, UINT16 cid, UINT16 len, UINT8 *data);

LE_ERR_STATE LeCmCbcDisconnectReq(UINT16 conn_hdl, UINT16 cid);

LE_ERR_STATE LeCmCbcSendCredits(UINT16 conn_hdl, UINT16 cid, UINT16 credits);

#endif

#endif
