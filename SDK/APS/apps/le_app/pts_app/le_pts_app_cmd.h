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

#ifndef _LE_PTS_APP_CMD_H_
#define _LE_PTS_APP_CMD_H_

#include "le.h"
#include "le_pts_app_fn.h"
#include "le_misc_patch.h"

#ifdef INCLUDE_PTS_APP

#define LE_PTS_CMD_PREFIX								"leh+"

#define LE_PTS_CMD_DEBUG_SWITCH							"dbg"
#define LE_PTS_CMD_CHANGE_CONTROLLER					"ctl"
#define LE_PTS_CMD_ACTIVE_LE_HOST						"act"
#define LE_PTS_CMD_ADV_SWITCH							"adv"
#define LE_PTS_CMD_DISCONNECT							"dsc"
#define LE_PTS_CMD_SEND_DATA							"snd"
#define LE_PTS_CMD_EXCHANGE_MTU							"exm"
#define LE_PTS_CMD_PASSKEY_INPUT						"pki"
#define LE_PTS_CMD_USER_CONFIRM							"uca"
#define LE_PTS_CMD_CONN_PRAR_PERM						"cpp"
#define LE_PTS_CMD_SIGNAL_PRAR_RSP						"spr"
#define LE_PTS_CMD_CONN_PRAR_REQ						"cpr"
#define LE_PTS_CMD_CONN_UPDATE_REQ						"cpu"
#define LE_PTS_CMD_SECURITY_SETTINGS					"sec"
#define LE_PTS_CMD_SECURITY_REQ							"seq"
#define LE_PTS_CMD_SET_ADV_PARA							"adp"
#define LE_PTS_CMD_SET_DIRECT_ADV_PARA					"dad"
#define LE_PTS_CMD_SET_RANDON_ADDRESS					"rna"
#define LE_PTS_CMD_FIND_PEER_NAME						"nam"
#define LE_PTS_CMD_RESOLVINGLIST_ADD					"rla"
#define LE_PTS_CMD_RESOLVINGLIST_CLEAR					"rlc"
#define LE_PTS_CMD_SET_SCAN_PARA						"snp"
#define LE_PTS_CMD_SCAN									"scn"
#define LE_PTS_CMD_DISCOVERABLE							"dis"
#define LE_PTS_CMD_SET_PTS_PUB_ADDR						"ppa"
#define LE_PTS_CMD_SET_DIR_ADV_TO_PTS					"dap"
#define LE_PTS_CMD_ESTABLISH_CONNECTION					"con"
#define LE_PTS_CMD_CANCELL_CONNECTION					"ccn"
#define LE_PTS_CMD_SET_OOB_DATA							"oob"
#define LE_PTS_CMD_SET_RPA_TIMEOUT						"rto"
#define LE_PTS_CMD_SET_PRIVACY							"pri"
#define LE_PTS_CMD_PRI_RECONN_ADDR						"pra"
#define LE_PTS_CMD_WR_PTS_PRIVACY						"wpp"
#define LE_PTS_CMD_CLEAR_BOND_INFO						"cbi"

#define LE_PTS_CMD_GAP_SW_CMD							"gap"
#define LE_PTS_CMD_GATT_CMD								"gat"
#define LE_PTS_CMD_PHY_CMD								"phy"
#define LE_PTS_CMD_PTS_SPECIAL_CASE						"psc"
#define LE_PTS_CMD_TEST_CMD								"tst"

#if 0

#define LE_HOST_INIT_COMPLETE				"LE<HIC>"
#define LE_HOST_ENTER_IDLE					"LE<IDL>"
#define LE_HOST_ENTER_ADVERTISING			"LE<ADV>"
#define LE_HOST_EXIT_ADVERTISING			"LE<EAD>"
#define LE_HOST_ENTER_SCANNING				"LE<SCN>"
#define LE_HOST_EXIT_SCANNING				"LE<ESN>"
#define LE_HOST_SCAN_REPORT					"LE<SRP>"
#define LE_HOST_ENTER_CONNECTING			"LE<CNG>"
#define LE_HOST_EXIT_CONNECTING				"LE<ECN>"
#define LE_HOST_ENTER_CONNECTION			"LE<CON>"
#define LE_HOST_ENTER_DISCONNECT			"LE<DSC>"
#define LE_HOST_RECEIVED_PEER_DATA			"LE<RPD>"
#define LE_HOST_DISPLAY_PASSKEY				"LE<DPK>"
#define LE_HOST_PASSKEY_INPUT_REQ			"LE<PKI>"
#define LE_HOST_USER_CONFIRM_DISPLAY		"LE<UCP>"
#define LE_HOST_CONN_PRAR_REQ				"LE<CPR>"
#define LE_HOST_SIGNAL_PRAR_REQ				"LE<SPR>"
#define LE_HOST_CONN_PRAR_UPDATA_CFM		"LE<CPU>"
#define LE_HOST_CONN_ENTER_CONNECTING		"LE<CNG>"
#define LE_HOST_CONN_EXIT_CONNECTING		"LE<ECN>"
#define LE_HOST_LOST_BOND_INFO				"LE<LBI>"
#define LE_HOST_PEER_GATT_REQ				"LE<GAT>"


#endif

enum
{
	LE_PTS_CMD_ID_DEBUG_SWITCH = 1,
	LE_PTS_CMD_ID_CHANGE_CONTROLLER,

	LE_PTS_CMD_ID_ACTIVE_LE_HOST = 0x20,
	LE_PTS_CMD_ID_ADV_SWITCH,
	LE_PTS_CMD_ID_DISCONNECT,
	LE_PTS_CMD_ID_SEND_DATA,
	LE_PTS_CMD_ID_EXCHANGE_MTU,
	LE_PTS_CMD_ID_PASSKEY_INPUT,
	LE_PTS_CMD_ID_USER_CONFIRM,
	LE_PTS_CMD_ID_CONN_PRAR_PERM,
	LE_PTS_CMD_ID_SIGNAL_PRAR_RSP,
    LE_PTS_CMD_ID_CONN_PRAR_REQ,
    LE_PTS_CMD_ID_CONN_UPDATE_REQ,
	LE_PTS_CMD_ID_SECURITY_SETTINGS,
	LE_PTS_CMD_ID_SECURITY_REQ,
	LE_PTS_CMD_ID_SET_ADV_PARA,
    LE_PTS_CMD_ID_SET_DIRECT_ADV_PARA,
	LE_PTS_CMD_ID_SET_RANDON_ADDRESS,
	LE_PTS_CMD_ID_FIND_PEER_NAME,
	LE_PTS_CMD_ID_RESOLVINGLIST_ADD,
	LE_PTS_CMD_ID_RESOLVINGLIST_CLEAR,
	LE_PTS_CMD_ID_SET_SCAN_PARA,
	LE_PTS_CMD_ID_SCAN,
	LE_PTS_CMD_ID_DISCOVERABLE,
	LE_PTS_CMD_ID_SET_PTS_PUB_ADDR,
	LE_PTS_CMD_ID_SET_DIR_ADV_TO_PTS,
	LE_PTS_CMD_ID_ESTABLISH_CONNECTION,
	LE_PTS_CMD_ID_CANCELL_CONNECTION,
    LE_PTS_CMD_ID_SET_OOB_DATA,
    LE_PTS_CMD_ID_SET_RPA_TIMEOUT,
    LE_PTS_CMD_ID_SET_PRIVACY,
    LE_PTS_CMD_ID_PRI_RECONN_ADDR,
	LE_PTS_CMD_ID_WR_PTS_PRIVACY,
	LE_PTS_CMD_ID_CLEAR_BOND_INFO,
	
	LE_PTS_CMD_ID_GAP_SW_CMD,
	LE_PTS_CMD_ID_GATT_CMD,
	LE_PTS_CMD_ID_PHY_CMD,
    LE_PTS_CMD_ID_PTS_SPECIAL_CASE,
    LE_PTS_CMD_ID_TEST_CMD
};


typedef struct _LE_PTS_CMD_ITEM_T
{
	UINT16			id;
	UINT16			len;
	const char		*cmd;
    void			*handler;
    UINT8			count;
} LE_PTS_CMD_ITEM_T;

typedef struct _LE_PTS_SUB_CMD_ITEM_T
{
	UINT16			id;
    UINT8			count;
    void			*handler;
} LE_PTS_SUB_CMD_ITEM_T;

typedef struct _LE_PTS_CMD_T
{
	UINT8			handle[8];
    UINT16			id;
    UINT16			len;
    UINT8			*cmd;
    UINT8			*para;
    void			*hdl;
    UINT8			count;
} LE_PTS_CMD_T;


#define LePtsAppNumToDecStr(x, y) LeHtcUtilNumToDecStr(x, y)
#define LePtsAppNumToStr(x, y, z) LeHtcUtilOctetNumToHexStr(x, y, z)

#define LE_TEST_APP_SHOW_MSG	LeHtcUtilShowMsg
#define LeHostSendCmdRsp(x)		LeHtcUtilSendCmdRsp(x)
#define NumToStr(x, y, z)		LeHtcUtilOctetNumToHexStr(x, y, z)

#endif

#endif
