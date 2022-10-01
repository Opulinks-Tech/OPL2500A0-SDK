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

#ifndef _BLE_GAP_IF_H_
#define _BLE_GAP_IF_H_

#include "ble.h"

#define GAP_ADTYPE_FLAGS                        0x01 
#define GAP_ADTYPE_16BIT_MORE                   0x02 
#define GAP_ADTYPE_16BIT_COMPLETE               0x03 
#define GAP_ADTYPE_32BIT_MORE                   0x04 
#define GAP_ADTYPE_32BIT_COMPLETE               0x05 
#define GAP_ADTYPE_128BIT_MORE                  0x06 
#define GAP_ADTYPE_128BIT_COMPLETE              0x07 
#define GAP_ADTYPE_LOCAL_NAME_SHORT             0x08 
#define GAP_ADTYPE_LOCAL_NAME_COMPLETE          0x09 
#define GAP_ADTYPE_POWER_LEVEL                  0x0A 
#define GAP_ADTYPE_OOB_CLASS_OF_DEVICE          0x0D 
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_HASHC     0x0E 
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_RANDR     0x0F 
#define GAP_ADTYPE_SM_TK                        0x10 
#define GAP_ADTYPE_SM_OOB_FLAG                  0x11 
#define GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE    0x12 
#define GAP_ADTYPE_SIGNED_DATA                  0x13 
#define GAP_ADTYPE_SERVICES_LIST_16BIT          0x14 
#define GAP_ADTYPE_SERVICES_LIST_128BIT         0x15 
#define GAP_ADTYPE_SERVICE_DATA                 0x16 
#define GAP_ADTYPE_PUBLIC_TARGET_ADDR           0x17 
#define GAP_ADTYPE_RANDOM_TARGET_ADDR           0x18 
#define GAP_ADTYPE_APPEARANCE                   0x19 
#define GAP_ADTYPE_ADV_INTERVAL                 0x1A 
#define GAP_ADTYPE_LE_BD_ADDR                   0x1B 
#define GAP_ADTYPE_LE_ROLE                      0x1C 
#define GAP_ADTYPE_SIMPLE_PAIRING_HASHC_256     0x1D 
#define GAP_ADTYPE_SIMPLE_PAIRING_RANDR_256     0x1E 
#define GAP_ADTYPE_SERVICE_DATA_32BIT           0x20 
#define GAP_ADTYPE_SERVICE_DATA_128BIT          0x21 
#define GAP_ADTYPE_3D_INFO_DATA                 0x3D 
#define GAP_ADTYPE_MANUFACTURER_SPECIFIC        0xFF 


#define GAP_ADTYPE_FLAGS_LIMITED                0x01 
#define GAP_ADTYPE_FLAGS_GENERAL                0x02 
#define GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED    0x04 


#define GAPBOND_PAIRING_MODE_NO_PAIRING         0x00 
#define GAPBOND_PAIRING_MODE_WAIT_FOR_REQ       0x01 
#define GAPBOND_PAIRING_MODE_INITIATE           0x02 

#define GAPBOND_IO_CAP_DISPLAY_ONLY             0x00 
#define GAPBOND_IO_CAP_DISPLAY_YES_NO           0x01 
#define GAPBOND_IO_CAP_KEYBOARD_ONLY            0x02 
#define GAPBOND_IO_CAP_NO_INPUT_NO_OUTPUT       0x03 
#define GAPBOND_IO_CAP_KEYBOARD_DISPLAY         0x04 

#define GAP_SCAN_TYPE_PASSIVE					0
#define GAP_SCAN_TYPE_ACTIVE					1

#define GAP_PUBLIC_ADDR							0
#define GAP_RAND_ADDR_STATIC					1
#define GAP_RAND_ADDR_NRPA						2
#define GAP_RAND_ADDR_RPA						3

#define GAP_TX_PWR_CURR_VAL						0
#define GAP_TX_PWR_MAX_VAL						1

#ifdef BLE_FEATURE_5P0

#define GAP_INVALID_ADV_HANDLE						0xFF
#define GAP_INVALID_ADV_SID							0xFF

// Advertising types
#define GAP_ADV_TYPE_CONN_SCAN						0		/* ADV_IND */
#define GAP_ADV_TYPE_CONN_DIR_HD					1		/* ADV_DIRECT_IND high duty cycle */
#define GAP_ADV_TYPE_SCAN							2		/* ADV_SCAN_IND */
#define GAP_ADV_TYPE_NONCONN_NONSCAN				3		/* ADV_NONCONN_IND */
#define GAP_ADV_TYPE_CONN_DIR_LD					4		/* ADV_DIRECT_IND low duty cycle */
#define GAP_ADV_TYPE_EXT_CONN_DIR					5		/* ADV_EXT_IND with Connectable directed event */
#define GAP_ADV_TYPE_EXT_SCAN						6		/* ADV_EXT_IND with Scannable undirected event */
#define GAP_ADV_TYPE_EXT_NONCONN_NONSCAN			7		/* ADV_EXT_IND with Non-connectable and non-scannable undirected event */
#define GAP_ADV_TYPE_EXT_CONN						8		/* ADV_EXT_IND with Connectable undirected event */
#define GAP_ADV_TYPE_EXT_SCAN_DIR					9		/* ADV_EXT_IND with Scannable directed event */
#define GAP_ADV_TYPE_EXT_NONCONN_NONSCAN_DIR		10		/* ADV_EXT_IND with Non-connectable and non-scannable directed event */

// ADV Report types
#define GAP_ADV_RPT_TYPE_CONN_SCAN					0		/* ADV_IND */
#define GAP_ADV_RPT_TYPE_CONN_DIR					1		/* ADV_DIRECT_IND */
#define GAP_ADV_RPT_TYPE_SCAN						2		/* ADV_SCAN_IND */
#define GAP_ADV_RPT_TYPE_NONCONN_NONSCAN			3		/* ADV_NONCONN_IND */
#define GAP_ADV_RPT_TYPE_SCAN_RSP					4		/* SCAN_RSP */

#define GAP_ADV_RPT_TYPE_EXT_CONN_DIR				5		/* ADV_EXT_IND with Connectable directed event */
#define GAP_ADV_RPT_TYPE_EXT_SCAN					6		/* ADV_EXT_IND with Scannable undirected event */
#define GAP_ADV_RPT_TYPE_EXT_NONCONN_NONSCAN		7		/* ADV_EXT_IND with Non-connectable and non-scannable undirected event */
#define GAP_ADV_RPT_TYPE_EXT_CONN					8		/* ADV_EXT_IND with Connectable undirected event */
#define GAP_ADV_RPT_TYPE_EXT_SCAN_DIR				9		/* ADV_EXT_IND with Scannable directed event */
#define GAP_ADV_RPT_TYPE_EXT_NONCONN_NONSCAN_DIR	10		/* ADV_EXT_IND with Non-connectable and non-scannable directed event */

// Extended SCAN RSP Report types
#define GAP_SCAN_RSP_RPT_TYPE_CONN_SCAN				0		/* SCAN_RSP from an advertiser of ADV_IND */
#define GAP_SCAN_RSP_RPT_TYPE_SCAN					1		/* SCAN_RSP from an advertiser of ADV_SCAN_IND */
#define GAP_AUX_SCAN_RSP_RPT_TYPE_EXT_SCAN			2		/* AUX_SCAN_RSP from an advertiser of ADV_EXT_IND with Scannable undirected event */
#define GAP_AUX_SCAN_RSP_RPT_TYPE_EXT_SCAN_DIR		3		/* AUX_SCAN_RSP from an advertiser of ADV_EXT_IND with Scannable directed event */

// ADV Report Data Status
#define GAP_RPT_DATA_COMPLETE						0
#define	GAP_RPT_DATA_INCOMPLETE_MORE				1
#define	GAP_RPT_DATA_INCOMPLETE_TRUNCATED			2

// ADV Data Operation
#define GAP_EXT_DATA_OP_INTER      					0
#define GAP_EXT_DATA_OP_FIRST     					1
#define GAP_EXT_DATA_OP_LAST      					2
#define GAP_EXT_DATA_OP_COMPLETE  					3
#define GAP_EXT_DATA_OP_UNCHANGED 					4

// ADV PHY
#define GAP_EXT_ADV_PHY_1M       					(LE_HCI_PHY_1M)
#define GAP_EXT_ADV_PHY_2M       					(LE_HCI_PHY_2M)
#define GAP_EXT_ADV_PHY_CODED     					(LE_HCI_PHY_CODED)

// SCAN PHY
#define GAP_EXT_SCAN_PHY_1M       					(LE_HCI_PHY_1M_PREF_MASK)
#define GAP_EXT_SCAN_PHY_CODED     					(LE_HCI_PHY_CODED_PREF_MASK)

// Create Connection PHY
#define GAP_EXT_CONN_PHY_1M       					(LE_HCI_PHY_1M_PREF_MASK)
#define GAP_EXT_CONN_PHY_2M       					(LE_HCI_PHY_2M_PREF_MASK)
#define GAP_EXT_CONN_PHY_CODED     					(LE_HCI_PHY_CODED_PREF_MASK)

// SYNC Data Status
#define GAP_SYNC_DATA_COMPLETE						(LE_HCI_SYNC_DATA_COMPLETE)
#define	GAP_SYNC_DATA_INCOMPLETE_MORE				(LE_HCI_SYNC_DATA_INCOMPLETE_MORE)
#define	GAP_SYNC_DATA_INCOMPLETE_TRUNCATED			(LE_HCI_SYNC_DATA_INCOMPLETE_TRUNCATED)

// Create Sync Option
#define	GAP_SYNC_OPTION_USE_PERIODIC_LIST			1
#define	GAP_SYNC_OPTION_PERIODIC_RPT_DISABLE		2


// Privacy Mode
#define GAP_PRIVACY_MODE_NETWORK					0
#define GAP_PRIVACY_MODE_DEVICE						1


#endif

typedef struct
{
	UINT16			interval_min;
	UINT16			interval_max;
	UINT8			type;
	UINT8			own_addr_type;
	UINT8			peer_addr_type;
	BD_ADDR			peer_addr;
	UINT8			channel_map;
	UINT8			filter_policy;
} LE_GAP_ADVERTISING_PARAM_T;

typedef struct
{
	UINT8			type;
	UINT16			interval;
	UINT16			window;
	UINT8			own_addr_type;
    UINT8			filter_policy;
} LE_GAP_SCAN_PARAM_T;

typedef struct
{
	UINT16			scan_interval;
	UINT16			scan_window;
	UINT16			interval_min;
	UINT16			interval_max;
	UINT16			latency;
	UINT16			supervision_timeout;
} LE_GAP_CONN_PHY_T;

#ifdef BLE_FEATURE_5P0
typedef struct
{
	BOOL			anonymous;
	BOOL			inc_tx_pwr;
    UINT8			adv_type;
	UINT32			prim_adv_itv_min;
	UINT32			prim_adv_itv_max;
	UINT8			prim_adv_ch_map;
	UINT8			own_addr_type;
	UINT8			peer_addr_type;
    BD_ADDR			peer_addr;
	UINT8			filter;
	INT8			tx_pwr;
	UINT8			prim_adv_phy;
	UINT8			scnd_adv_phy;
    UINT8			scnd_adv_max_skip;
	UINT8			scan_req_notify;
} LE_GAP_EXT_ADV_PARAMS_T;

typedef struct
{
    UINT8			type;
    UINT16			interval;
    UINT16			window;
} LE_GAP_EXT_SCAN_TYPE_T;

typedef struct
{
	BOOL					use_whitelist;
	UINT8 					own_addr_type;
	LE_BT_ADDR_T 			peer_addr;
	UINT8 					phys;
	LE_GAP_CONN_PHY_T		phy_1m;
	LE_GAP_CONN_PHY_T		phy_2m;
	LE_GAP_CONN_PHY_T		phy_coded;
} LE_GAP_EXT_CREATE_CONN_PARAMS_T;

typedef struct
{
    UINT8			option;
	UINT8			sid;
	LE_BT_ADDR_T 	peer_addr;
	UINT16			skip;
	UINT8			timeout;
    UINT8			unused;
} LE_GAP_CREATE_SYNC_PARAMS_T;


#endif

LE_ERR_STATE LeGapGetBdAddr(BD_ADDR addr);

LE_ERR_STATE LeGapDisconnectReq(UINT16 conn_hdl);

LE_ERR_STATE LeGapDisconnectWithReason(UINT16 conn_hdl, UINT8 reason);

LE_ERR_STATE LeGapReadRemoteVersionInfo(UINT16 conn_hdl);

LE_ERR_STATE LeGapSetAdvData(UINT8 len, UINT8 *data);

LE_ERR_STATE LeGapSetScanRspData(UINT8 len, UINT8 *data);

LE_ERR_STATE LeGapSetAdvParameter(LE_GAP_ADVERTISING_PARAM_T *params);

LE_ERR_STATE LeGapGenRandAddr(UINT8 type, BD_ADDR addr);

LE_ERR_STATE LeGapSetStaticAddr(BD_ADDR addr);

LE_ERR_STATE LeGapSetRandAddr(BD_ADDR addr);

LE_ERR_STATE LeGapAdvertisingEnable(BOOL start);

LE_ERR_STATE LeGapSetScanParameter(LE_GAP_SCAN_PARAM_T *params);

LE_ERR_STATE LeGapSetScanEnable(BOOL filter);

LE_ERR_STATE LeGapSetScanDisable(void);

LE_ERR_STATE LeGapCentralCreateConnect(UINT8 own_addr_type, LE_BT_ADDR_T *peer_addr, LE_GAP_CONN_PHY_T *phy_1m);

LE_ERR_STATE LeGapCentralCancelConnect(void);

LE_ERR_STATE LeGapWhiteListSize(void);

LE_ERR_STATE LeGapWhiteListClear(void);

LE_ERR_STATE LeGapWhiteListAdd(UINT8 addr_type, BD_ADDR addr);

LE_ERR_STATE LeGapWhiteListRemove(UINT8 addr_type, BD_ADDR addr);

LE_ERR_STATE LeGapReadTxPower(UINT16 conn_hdl, UINT8 type);

LE_ERR_STATE LeGapReadRssi(UINT16 conn_hdl);

LE_ERR_STATE LeGapResolvingListSize(void);

LE_ERR_STATE LeGapResolvingListClear(void);

LE_ERR_STATE LeGapResolvingListAdd(UINT8 addr_type, BD_ADDR addr, UINT8 *irk);

LE_ERR_STATE LeGapResolvingListRemove(UINT8 addr_type, BD_ADDR addr);

LE_ERR_STATE LeGapReadAdvChannelTxPower(void);

LE_ERR_STATE LeGapSetDataChannelMap(UINT8 *ch);

LE_ERR_STATE LeGapReadDataChannelMap(UINT16 conn_hdl);

LE_ERR_STATE LeGapSetDataChannelPduLen(UINT16 conn_hdl, UINT16 tx_octets, UINT16 tx_time);

LE_ERR_STATE LeGapSetRpaTimeout(UINT16 timeout);

LE_ERR_STATE LeGapReadBdAddr(void);

LE_ERR_STATE LeGapSigConnUpdateResponse(UINT16 conn_hdl, UINT8 identifier, BOOL accept);

LE_ERR_STATE LeGapConnUpdateRequest(UINT16 conn_hdl, LE_CONN_PARA_T *para);

LE_ERR_STATE LeGapConnParamRequest(UINT16 conn_hdl, LE_CONN_PARA_T *para);

LE_ERR_STATE LeGapConnParamResponse(UINT16 conn_hdl, BOOL accept);

LE_ERR_STATE LeGapReadPhy(UINT16 conn_hdl);

LE_ERR_STATE LeGapSetDefaultPhy(UINT8 tx, UINT8 rx);

LE_ERR_STATE LeGapSetPhy(UINT16 conn_hdl, UINT8 tx, UINT8 rx, UINT16 option);

#ifdef BLE_FEATURE_5P0

LE_ERR_STATE LeGapCreateExtAdv(LE_GAP_EXT_ADV_PARAMS_T *para);

LE_ERR_STATE LeGapSetExtAdvParams(UINT8 adv_hdl, LE_GAP_EXT_ADV_PARAMS_T *para);

LE_ERR_STATE LeGapSetExtAdvRandomAddr(UINT8 adv_hdl, BD_ADDR rnd_addr);

LE_ERR_STATE LeGapSetExtAdvData(UINT8 adv_hdl, UINT8 op, UINT16 len, UINT8 *data);

LE_ERR_STATE LeGapSetExtScanRspData(UINT8 adv_hdl, UINT8 op, UINT16 len, UINT8 *data);

LE_ERR_STATE LeGapSetExtAdvEnable(UINT8 adv_hdl, UINT16 duration, UINT8 max_event);

LE_ERR_STATE LeGapSetExtAdvDisable(UINT8 adv_hdl);

LE_ERR_STATE LeGapReadMaxAdvDataLen(void);

LE_ERR_STATE LeGapReadMaxSupportAdvSets(void);

LE_ERR_STATE LeGapRemoveAdvSet(UINT8 adv_hdl);

LE_ERR_STATE LeGapClearAdvSets(void);

LE_ERR_STATE LeGapSetPeriodicAdvParams(UINT8 adv_hdl, UINT16 itv_min, UINT16 itv_max, BOOL tx_pwr);

LE_ERR_STATE LeGapSetPeriodicAdvData(UINT8 adv_hdl, UINT8 op, UINT16 len, UINT8 *data);

LE_ERR_STATE LeGapSetPeriodicAdvEnable(UINT8 adv_hdl, BOOL enable);

LE_ERR_STATE LeGapSetExtScanParams(UINT8 own_addr_type, UINT8 filter, LE_GAP_EXT_SCAN_TYPE_T *phy_1m, LE_GAP_EXT_SCAN_TYPE_T *coded_phy);

LE_ERR_STATE LeGapSetExtScanEnable(UINT8 duplicate, UINT16 duration, UINT16 period);

LE_ERR_STATE LeGapSetExtScanDisable(void);

LE_ERR_STATE LeGapCentralCreateExtConnect(LE_GAP_EXT_CREATE_CONN_PARAMS_T *param);

LE_ERR_STATE LeGapCentralCancelExtConnect(void);

LE_ERR_STATE LeGapPeriodicAdvSyncCreate(LE_GAP_CREATE_SYNC_PARAMS_T *param);

LE_ERR_STATE LeGapPeriodicAdvSyncCancel(void);

LE_ERR_STATE LeGapPeriodicAdvSyncTerminate(UINT16 sync_hdl);

LE_ERR_STATE LeGapPeriodicAdvListAdd(UINT8 addr_type, BD_ADDR addr, UINT8 sid);

LE_ERR_STATE LeGapPeriodicAdvListRemove(UINT8 addr_type, BD_ADDR addr, UINT8 sid);

LE_ERR_STATE LeGapPeriodicAdvListClear(void);

LE_ERR_STATE LeGapPeriodicAdvListSize(void);

LE_ERR_STATE LeGapReadTxPowerRange(void);

LE_ERR_STATE LeGapReadRfPathCompensation(void);

LE_ERR_STATE LeGapWriteRfPathCompensation(INT16 tx, INT16 rx);

LE_ERR_STATE LeGapSetPrivacyMode(UINT8 addr_type, BD_ADDR addr, UINT8 mode);

#endif

#endif
