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

#ifndef _LE_PTS_APP_H_
#define _LE_PTS_APP_H_

#include "le.h"
#include "le_host.h"
#include "le_gap_if.h"
#include "le_smp.h"
#include "le_pts_app_fn.h"
#include "le_pts_app_db.h"
#include "le_host_test_cmd.h"


#ifdef INCLUDE_PTS_APP

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)


enum
{
	LE_APP_USR_MSG_STOP_ADVERTISING = LE_APP_USR_MSG_BASE,
	LE_APP_USR_MSG_START_ADVERTISING,
	LE_APP_USR_MSG_STOP_SCANNING,
	LE_APP_USR_MSG_START_SCANNING,
    
	LE_APP_USR_MSG_DISCONNECTION,
	
	LE_APP_USR_MSG_SHOW_ONLY,
	LE_APP_USR_MSG_TEST_NOTIFY,
	LE_APP_USR_MSG_TEST_INDICATION,

	LE_APP_USR_MSG_TEST_HOST_CMD,
	LE_APP_USR_MSG_DUMP_MM,

	LE_APP_USR_MSG_TEST_TIMER,

	LE_APP_USR_MSG_RPA_TIMEOUT,

	LE_APP_USR_MSG_CHECK_CMD,
	
	LE_APP_USR_MSG_TOP
};

enum
{
	LE_APP_STATE_IDLE,
	LE_APP_STATE_ADVERTISING 	= (1 << 0),
	LE_APP_STATE_SCANNING 		= (1 << 1),
	LE_APP_STATE_CONNECTING 	= (1 << 2),
	LE_APP_STATE_CONNECTED 		= (1 << 3),
};

enum
{
	LE_APP_ACTION_START_ADVERTISING 	= (1 << 0),
	LE_APP_ACTION_END_ADVERTISING 		= (1 << 1),
	LE_APP_ACTION_START_CONNECTING 		= (1 << 2),
	LE_APP_ACTION_CANCEL_CONNECTING 	= (1 << 3),
	LE_APP_ACTION_START_SCANNING 		= (1 << 4),
	LE_APP_ACTION_END_SCANNING          = (1 << 5),
};


typedef struct
{
	UINT8 enable;
	UINT8 duplicate;
} LE_APP_USR_MSG_SCANNING_T;

typedef struct
{
	UINT16			handle;
	UINT16			type;
    UINT16			len;
	UINT8			val[32];
} LE_TEST_PEER_ATTR_T;

typedef struct _LE_TEST_GPA_BV_11_T
{
	UINT8					action;
    UINT16					start_hdl;
    UINT16					end_hdl;
    UINT16					count;
    UINT16					curr_idx;
    LE_TEST_PEER_ATTR_T		*attr;

} LE_TEST_GPA_BV_11_T;

typedef struct
{
	UINT16			len;
	UINT8			data[31];
} LE_PTS_ADV_DATA_T;

typedef struct _LE_HOST_PTS_APP_DATA_T
{
	TASKPACK		task;
	UINT16			state;
    UINT16			action;
    UINT32			gap_flag;
    UINT8			peer_adv_flag;
	UINT8			wl_size;
	UINT16			rpa_timeout;

    UINT16			default_mtu;
	UINT16			curr_mtu;
    UINT16			ucp_id;
    
	UINT16			len;
	UINT8			buf[256];

	UINT16			conn_hdl;
    LE_BT_ADDR_T	btaddr;
    UINT8			oob_data[16];
	BOOL			oob_present;
    
    LE_SMP_SC_OOB_DATA_T	peer_oob;
    LE_SMP_SC_OOB_DATA_T	our_oob;

	BOOL			author_enable;
	BOOL			author_enable2;
	BOOL			author_enable3;
	BOOL			author_enable4;
	BOOL			author_enable5;

	LE_TEST_GPA_BV_11_T		gpa_bv_11;

	UINT8			cla_count;
    UINT8			cla_idx;
    UINT8			client_action[10];

    UINT8			bonded;
    UINT8			encrypted;
    UINT8			privacy;

    UINT8			find_name;
    UINT16			peer_name_hdl;

    LE_BT_ADDR_T	pts_addr;
    UINT8			dir_adv;

	LE_PTS_ADV_DATA_T adv;
	LE_PTS_ADV_DATA_T scan;

    UINT8			gap_sec_aut_bv_14;
    UINT8			gap_gat_bv_08;

    UINT8			role;
    UINT8			limit_disc;
    UINT8			peer_irk[16];
    LE_BT_ADDR_T	pts_reconn_addr;
    UINT8			pts_reconn_flag;
    BOOL			pts_reverse_addr;
    UINT8			pts_prda_bv02;
    UINT8			pts_gcep_bv05;
    
    UINT8			pts_ucon_bv05;
    BD_ADDR			our_reconn_addr;
    UINT8			our_reconn_flag;
    UINT8			our_privacy;
    BOOL			auto_adv;
    
    BOOL			force_pair;

	LE_PTS_GATT_TABLE_T 	db_gap;
	LE_PTS_GATT_TABLE_T 	db_blps;
	LE_PTS_GATT_TABLE_T 	db_hid;
	LE_PTS_GATT_TABLE_T 	db_gatt;

	LE_GATT_SERVICE_T 		*svc_gap;
	LE_GATT_SERVICE_T 		*svc_hid;
	LE_GATT_SERVICE_T 		*svc_bp;
	LE_GATT_SERVICE_T 		*svc_gatt;

	LE_PTS_CMD_ITEM_T 			*cmd_table;
	LE_PTS_CMD_ITEM_T 			*ext_cmd_table;
	LE_PTS_SUB_CMD_ITEM_T		*gatt_cmd_table;
	LE_PTS_SUB_CMD_ITEM_T		*special_cmd_table;
	LE_PTS_SUB_CMD_ITEM_T		*test_cmd_table;

	LE_GAP_CONN_PHY_T 			conn_phy;

	void						*additional;

	// le_pts_app
	T_LePtsAppSetGetPtsAddrFp 						LePtsAppSetGetPtsAddr;
	T_LePtsAppSetDirectAdvtisingParaFp 				LePtsAppSetDirectAdvtisingPara;
	T_LePtsAppSetAdvtisingParaFp 					LePtsAppSetAdvtisingPara;
	T_LePtsAppSendPasskeyDisplayFp 					LePtsAppSendPasskeyDisplay;
	T_LePtsAppSendUserConfirmDisplayFp 				LePtsAppSendUserConfirmDisplay;
    T_LePtsAppShowAdvReportFp						LePtsAppShowAdvReport;
    T_LePtsAppFindAdvTypeSectionFp					LePtsAppFindAdvTypeSection;
    T_LePtsAppHandleAdvertisingReportIndFp			LePtsAppHandleAdvertisingReportInd;
    T_LePtsAppHandleDisconnectCompleteIndFp			LePtsAppHandleDisconnectCompleteInd;
	T_LePtsAppCmTaskHandlerFp 						LePtsAppCmTaskHandler;
	T_LePtsAppUserTaskHandlerFp 					LePtsAppUserTaskHandler;
	T_LePtsAppSmpTaskHandlerFp 						LePtsAppSmpTaskHandler;
	T_LePtsAppTaskHandlerFp 						LePtsAppTaskHandler;
	T_LePtsAppSetAdvScanDataFp 						LePtsAppSetAdvScanData;

	// le_pts_app_db
	T_LePtsAppBuildSvcAttrFp						LePtsAppBuildSvcAttr;
	T_LePtsAppBuildGapSvcTblFp						LePtsAppBuildGapSvcTbl;
	T_LePtsAppBuildBlpsSvcTblFp						LePtsAppBuildBlpsSvcTbl;
	T_LePtsAppBuildHidSvcTblFp						LePtsAppBuildHidSvcTbl;
	T_LePtsAppBuildGattSvcTblFp						LePtsAppBuildGattSvcTbl;
	T_LePtsAppGattDbInitFp							LePtsAppGattDbInit;
	T_LePtsAppSvcTblFp 								LePtsAppSvcTbl;

	// le_pts_app_gatt
	T_LePtsAppGetSvcFp 								LePtsAppGetSvc;
	T_LePtsAppGetServiceHandleFp 					LePtsAppGetServiceHandle;
	T_LePtsAppShowReceivedDataFp					LePtsAppShowReceivedData;
	T_LePtsAppHandleGapServiceReadFp				LePtsAppHandleGapServiceRead;
	T_LePtsAppHandleGapServiceWriteFp				LePtsAppHandleGapServiceWrite;
	T_LePtsAppHandleHidServiceReadFp				LePtsAppHandleHidServiceRead;
	T_LePtsAppHandleHidServiceWriteFp				LePtsAppHandleHidServiceWrite;
	T_LePtsAppHandleBpServiceReadFp					LePtsAppHandleBpServiceRead;
	T_LePtsAppHandleBpServiceWriteFp				LePtsAppHandleBpServiceWrite;
	T_LePtsAppHandleGattServiceReadFp				LePtsAppHandleGattServiceRead;
	T_LePtsAppHandleGattServiceWriteFp				LePtsAppHandleGattServiceWrite;
	T_LePtsAppGattHandleReadIndFp					LePtsAppGattHandleReadInd;
	T_LePtsAppGattHandleWriteIndFp					LePtsAppGattHandleWriteInd;
	T_LePtsAppGapBv11ShowResultFp					LePtsAppGapBv11ShowResult;
	T_LePtsAppHandleGattInitCfmFp					LePtsAppHandleGattInitCfm;
	T_LePtsAppHandleGattServiceInfoIndFp			LePtsAppHandleGattServiceInfoInd;
	T_LePtsAppHandleGattIncludeServiceInfoIndFp		LePtsAppHandleGattIncludeServiceInfoInd;
	T_LePtsAppHandleGattCharDeclInfoIndFp			LePtsAppHandleGattCharDeclInfoInd;
	T_LePtsAppHandleGattFindCharCfmFp				LePtsAppHandleGattFindCharCfm;
	T_LePtsAppHandleGattCharValIndFp				LePtsAppHandleGattCharValInd;
	T_LePtsAppHandleGattReadCharValByUuidCfmFp		LePtsAppHandleGattReadCharValByUuidCfm;
	T_LePtsAppHandleGattReadLongCharValCfmFp		LePtsAppHandleGattReadLongCharValCfm;
	T_LePtsAppHandleGattReadMultipleCharValCfmFp	LePtsAppHandleGattReadMultipleCharValCfm;
	T_LePtsAppHandleGattIndicateIndFp				LePtsAppHandleGattIndicateInd;
	T_LePtsAppHandleGattNotifyIndFp					LePtsAppHandleGattNotifyInd;
	T_LePtsAppGattTaskHandlerFp						LePtsAppGattTaskHandler;

	// le_pts_app_cmd
	T_LePtsGetAttrLenFp 							LePtsGetAttrLen;
	T_LeHtcUtilHexStrToNumFp 						LeHtcUtilHexStrToNum;
	T_LePtsGattCmdTableFp 							LePtsGattCmdTable;
    T_LePtsSpecialCmdTableFp						LePtsSpecialCmdTable;
    T_LePtsTestCmdTableFp							LePtsTestCmdTable;
	T_LePtsAppFnSetFp								LePtsGattCmdTablePatch;
	T_LePtsAppFnSetFp								LePtsSpecialCmdTablePatch;
	T_LePtsAppFnSetFp								LePtsTestCmdTablePatch;
	T_LePtsSendScanningMsgFp 						LePtsSendScanningMsg;
	T_LePtsTestCmdUtilGenerateKeyPairFp 			LePtsTestCmdUtilGenerateKeyPair;
	T_LePtsTestCmdUtilGenDhkeyFp 					LePtsTestCmdUtilGenDhkey;

	LePtsCmdHandler									LePtsRunCmdPhyRead;
	LePtsCmdHandler									LePtsRunCmdPhyDefault;
	LePtsCmdHandler									LePtsRunCmdPhySet;

} LE_HOST_PTS_APP_DATA_T;


LE_HOST_PTS_APP_DATA_T* LeHostPtsApp(void);

#endif

#endif
