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

#ifndef _LE_CMD_APP_H_
#define _LE_CMD_APP_H_

#include "le.h"
#include "le_host.h"
#include "le_gap_if.h"
#include "le_gatt_if.h"
#include "le_smp.h"

#include "le_host_test_cmd.h"
#include "le_cmd_app_fn.h"
#include "le_cmd_app_cmd.h"
#include "le_cmd_app_db.h"


#ifdef INCLUDE_CMD_APP

#define LE_APP_ROLE_GATT_CLIENT			1
#define LE_APP_ROLE_GATT_SERVER			2

#define LE_APP_MAX_CONNECTION			8


#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)


enum
{
	LE_APP_USR_MSG_DISCONNECTION = LE_APP_USR_MSG_BASE,

	LE_APP_USR_MSG_BUILD_GATT_DB,
	LE_APP_USR_MSG_START_GATT_DB,
	LE_APP_USR_MSG_SHOW_GATT_SVC,
	LE_APP_USR_MSG_SHOW_GATT_CHAR,
	LE_APP_USR_MSG_START_ADV,
	LE_APP_USR_MSG_SEND_GATT_NOTIFY,

    
	LE_APP_USR_MSG_TOP
};

enum
{
	LE_APP_STATE_UNINIT,
	LE_APP_STATE_IDLE 					= (1 << 0),
	LE_APP_STATE_ADVERTISING 			= (1 << 1),
	LE_APP_STATE_SCANNING 				= (1 << 2),
	LE_APP_STATE_CONNECTING 			= (1 << 3),
	LE_APP_STATE_CONNECTED 				= (1 << 4),
};

enum
{
	LE_APP_ACTION_START_ADVERTISING 	= (1 << 0),
	LE_APP_ACTION_END_ADVERTISING 		= (1 << 1),
	LE_APP_ACTION_START_CONNECTING 		= (1 << 2),
	LE_APP_ACTION_CANCEL_CONNECTING 	= (1 << 3),
	LE_APP_ACTION_START_SCANNING 		= (1 << 4),
	LE_APP_ACTION_END_SCANNING          = (1 << 5),
	
	LE_APP_ACTION_FIND_ALL_SERVICE      = (1 << 6),
};

enum
{
	LE_PEER_ACTION_FIND_ALL_SERVICE = 1,
	LE_PEER_ACTION_FIND_INCL_SERVICE,
	LE_PEER_ACTION_FIND_ALL_CHAR,
	LE_PEER_ACTION_READ_LONG_CHAR_VAL,
	LE_PEER_ACTION_WRITE_CHAR_VAL,


};


typedef struct _LE_APP_GATT_CHAR_T
{
	LE_GATT_ATTR_T		*attr;
    LE_GATT_ATTR_T		*desc;
	UINT16				count;
} LE_APP_GATT_CHAR_T;

typedef struct _LE_APP_GATT_SVC_T
{
	LE_GATT_SERVICE_T	*id;
    LE_APP_GATT_CHAR_T	*chara;
	UINT16				count;
    UINT8				type;
    BOOL				start;
} LE_APP_GATT_SVC_T;

typedef struct
{
    UINT16				conn_hdl;
    UINT16				handle;
    UINT16				len;
    UINT16				idx;
    UINT8				type;
    UINT8				prop;
    LE_GATT_SERVICE_T 	*svc;
	UINT8				*buf;
} LE_APP_GATT_SEND_T;

typedef struct _LE_APP_GATT_SVR_T
{
	LE_APP_GATT_SVC_T	*svc;
	UINT16				count;
    BOOL				init;
} LE_APP_GATT_SVR_T;

typedef struct _LE_APP_PEER_ATTR_T
{
	UINT16				handle;
    BOOL				valid;
	UINT8				type;
	UINT16				uuid[8];
} LE_APP_PEER_ATTR_T;

typedef struct _LE_APP_PEER_CHAR_T
{
	LE_APP_PEER_ATTR_T	*attr;
	LE_APP_PEER_ATTR_T	*desc;
    UINT16				count;
	UINT8				property;
    UINT16				val_hdl;
	UINT16				start_hdl;
    UINT16				end_hdl;
} LE_APP_PEER_CHAR_T;

typedef struct _LE_APP_PEER_SVC_T
{
	UINT16				start_hdl;
    UINT16				end_hdl;
    UINT16				type;
	UINT8				format;
	UINT16				uuid[8];

	LE_APP_PEER_ATTR_T	*attr;
	UINT16				attr_count;
	LE_APP_PEER_ATTR_T	*incl;
	UINT16				incl_count;
    LE_APP_PEER_CHAR_T	*chara;
	UINT16				count;
} LE_APP_PEER_SVC_T;

typedef struct
{
	UINT16				handle[4];
	LE_APP_PEER_ATTR_T	*attr;
	UINT16				count;
} LE_APP_PEER_ATTR_ITEM_T;

typedef struct
{
	UINT16				handle[4];
	UINT16				svc_idx;
	LE_APP_PEER_SVC_T	*svc;
} LE_APP_PEER_INC_T;

typedef struct _LE_CMD_APP_PEER_T
{
	UINT16						conn_hdl;
	UINT16						mtu;
    UINT8						idx;
    UINT8						valid;
    UINT8						bondid;
	LE_BT_ADDR_T				bd_addr;
    UINT16						itv;
    UINT16						latency;      
    UINT16						sv_tmo;

    UINT16						action;
    UINT16						find;
    UINT8						*wrd_buf;
    UINT16						wrd_len;
	LE_APP_PEER_ATTR_T			*attr;
	LE_APP_PEER_SVC_T			*svc;
	UINT16						attr_count;
	UINT16						svc_count;

    LE_QLIST_T					atti;
	LE_APP_PEER_ATTR_ITEM_T 	*curr_atti;
    
    LE_QLIST_T					srv;
    LE_QLIST_T					inc;

    BOOL						discon;
    UINT16						svr_ind;
} LE_CMD_APP_PEER_T;

typedef struct _LE_HOST_CMD_APP_DATA_T
{
	TASKPACK							task;
	UINT16								state;
    UINT16								action;

	LE_APP_GATT_SVR_T					server;
	LE_APP_GATT_SEND_T					*send;

	UINT8								init_role;
#ifdef LE_ACTION_FOR_DEVELOP
	UINT8								adv_data[31];
	UINT8								adv_data_size;
	UINT8								scan_data[31];
	UINT8								scan_data_size;
#endif
	BD_ADDR								pub_addr;
	BD_ADDR								sta_addr;
	UINT8								sta_addr_change;

	//UINT8								*dev_name;
	//UINT16							name_len;
	LE_GATT_SERVICE_T 					*name_svc;
	UINT16 								name_attrid;

    UINT8 								occupy_flag;
    UINT16								occupy_len;
    UINT16								occupy_idx;
    UINT8								occupy_buf[512];

	LE_GAP_ADVERTISING_PARAM_T			adv_para;
	UINT8								adv_para_change;

	LE_CONN_PARA_T 						conn_param;

	
	LE_CMD_APP_PEER_T					peer[LE_APP_MAX_CONNECTION];
	UINT8								peer_count;

	// le_cmd_app_db
	UINT32								db_base;
	UINT32								db_magic_num;
	LE_GATT_DB_T 						*gatt_db;
#ifdef LE_DB_FROM_INTERNAL
	UINT8 								*gatt_db_raw_data;
	LE_GATT_TABLE_T 					svc_tbl_gatt;
	LE_GATT_TABLE_T 					svc_tbl_gap;
	LE_GATT_TABLE_T 					svc_tbl_blps;
	LE_GATT_TABLE_T 					svc_tbl_hid;
#endif

	// le_cmd_app
#ifdef LE_ACTION_FOR_DEVELOP
	T_LeCmdAppSetAdvScanDataFp						LeCmdAppSetAdvScanData;
	T_LeCmdAppSetAdvParaFp							LeCmdAppSetAdvPara;
	T_LeCmdAppSendPasskeyDisplayFp					LeCmdAppSendPasskeyDisplay;
	T_LeCmdAppSendUserConfirmDisplayFp				LeCmdAppSendUserConfirmDisplay;
#endif
	T_LeCmdAppGetPeerByHdlFp						LeCmdAppGetPeerByHdl;
	T_LeCmdAppGetPeerFreeFp							LeCmdAppGetPeerFree;
	T_LeCmdAppFreePeerGattDbFp						LeCmdAppFreePeerGattDb;
	T_LeCmdAppHandleCmInitCompleteCfmFp				LeCmdAppHandleCmInitCompleteCfm;
	T_LeCmdAppHandleCmExitAdvtiseCfmFp				LeCmdAppHandleCmExitAdvtiseCfm;
	T_LeCmdAppHandleCmEnterAdvtiseCfmFp				LeCmdAppHandleCmEnterAdvtiseCfm;
	T_LeCmdAppHandleCmConnParaReqFp					LeCmdAppHandleCmConnParaReq;
	T_LeCmdAppHandleCmSignalUpdataReqFp				LeCmdAppHandleCmSignalUpdataReq;
	T_LeCmdAppHandleCmConnUpdateCompleteIndFp		LeCmdAppHandleCmConnUpdateCompleteInd;
	T_LeCmdAppHandleCmConnectCompleteIndFp			LeCmdAppHandleCmConnectCompleteInd;
	T_LeCmdAppHandleCmDisconnectCompleteIndFp		LeCmdAppHandleCmDisconnectCompleteInd;
	T_LeCmdAppCmTaskOtherHandlerFp					LeCmdAppCmTaskOtherHandler;
	T_LeCmdAppCmTaskHandlerFp						LeCmdAppCmTaskHandler;
	T_LeCmdAppUserTaskHandlerFp						LeCmdAppUserTaskHandler;
	T_LeCmdAppSmpTaskHandlerFp						LeCmdAppSmpTaskHandler;
	T_LeCmdAppTaskHandlerFp							LeCmdAppTaskHandler;

	// le_cmd_app_db
#ifdef LE_DB_FROM_INTERNAL
	T_LeGattDbCalRawDbSizeFp						LeGattDbCalRawDbSize;
	T_LeGattDbInternalFillFp						LeGattDbInternalFill;
	T_LeGattDbBuildSvcAttrFp						LeGattDbBuildSvcAttr;
	T_LeGattDbBuildGattSvcTblFp						LeGattDbBuildGattSvcTbl;
	T_LeGattDbBuildGapSvcTblFp						LeGattDbBuildGapSvcTbl;
	T_LeGattDbBuildBlpsSvcTblFp						LeGattDbBuildBlpsSvcTbl;
	T_LeGattDbBuildHidSvcTblFp						LeGattDbBuildHidSvcTbl;
#ifdef VER_AO_ROM
	T_LeGattDbDumpCsvTableFp						LeGattDbDumpCsvTable;
#endif
	T_LeGattDbInternalInitFp						LeGattDbInternalInit;
#endif
	T_LeReadFromFlashFp								LeReadFromFlash;
	T_LeGattDbClearFp								LeGattDbClear;
	T_LeGattDbCheckItemFp							LeGattDbCheckItem;
	T_LeGattDbBuildHeaderFp							LeGattDbBuildHeader;
	T_LeGattDbFindIncludeServiceFp					LeGattDbFindIncludeService;
	T_LeGattDbCheckIncludeServiceFp					LeGattDbCheckIncludeService;
	T_LeGattDbCheckPermitFp							LeGattDbCheckPermit;
	T_LeGattDbCheckAttrIsValidFp					LeGattDbCheckAttrIsValid;
	T_LeGattDbCheckAttrFp							LeGattDbCheckAttr;
	T_LeGattDbBuildAttrFp							LeGattDbBuildAttr;
	T_LeGattDbRegServiceDbFp						LeGattDbRegServiceDb;
	T_LeGattDbBuildServiceDbFp						LeGattDbBuildServiceDb;
	T_LeGattDbGetServiceIdFp						LeGattDbGetServiceId;

	// le_cmd_app_gatt
	T_LeCmdAppGetCharFp								LeCmdAppGetChar;
	T_LeCmdAppBuildGattTableFp						LeCmdAppBuildGattTable;
	T_LeCmdAppHandleGattInitCfmFp					LeCmdAppHandleGattInitCfm;
    T_LeCmdAppHandleGattAccessReadIndFp				LeCmdAppHandleGattAccessReadInd;
    T_LeCmdAppHandleGattAccessWriteIndFp			LeCmdAppHandleGattAccessWriteInd;
	T_LeCmdAppHandleGattExchangeMtuIndFp			LeCmdAppHandleGattExchangeMtuInd;
	T_LeCmdAppHandleGattExchangeMtuCfmFp			LeCmdAppHandleGattExchangeMtuCfm;
	T_LeCmdAppHandleGattServiceInfoIndFp			LeCmdAppHandleGattServiceInfoInd;
	T_LeCmdAppHandleGattFindAllPrimaryServiceCfmFp	LeCmdAppHandleGattFindAllPrimaryServiceCfm;
	T_LeCmdAppHandleGattCharDescInfoIndFp			LeCmdAppHandleGattCharDescInfoInd;
	T_LeCmdAppBuildPeerGattTableFp					LeCmdAppBuildPeerGattTable;
	T_LeCmdAppHandleGattFindAllCharDescCfmFp		LeCmdAppHandleGattFindAllCharDescCfm;
	T_LeCmdAppHandleGattInclServiceInfoIndFp		LeCmdAppHandleGattInclServiceInfoInd;
	T_LeCmdAppHandleGattInclServiceInfoCfmFp		LeCmdAppHandleGattInclServiceInfoCfm;
	T_LeCmdAppHandleGattCharInfoIndFp				LeCmdAppHandleGattCharInfoInd;
	T_LeCmdAppHandleFindGattCharCfmFp				LeCmdAppHandleFindGattCharCfm;
	T_LeCmdAppHandleGattCharValIndFp				LeCmdAppHandleGattCharValInd;
	T_LeCmdAppHandleGattReadLongCharValCfmFp		LeCmdAppHandleGattReadLongCharValCfm;
	T_LeCmdAppHandleGattWriteCharValCfmFp			LeCmdAppHandleGattWriteCharValCfm;
	T_LeCmdAppHandleGattConfirmCfmFp				LeCmdAppHandleGattConfirmCfm;
	T_LeCmdAppGattTaskHandlerFp						LeCmdAppGattTaskHandler;
	T_LeCmdAppHandleGattOperationTimeoutFp			LeCmdAppHandleGattOperationTimeout;
	T_LeCmdAppGattTaskOtherHandlerFp				LeCmdAppGattTaskOtherHandler;

	// le_cmd_app_cmd
	T_LeCmdAppSendAtCmdRspFp						LeCmdAppSendAtCmdRsp;
	T_LeCmdAppSendAtCmdAckFp						LeCmdAppSendAtCmdAck;
    
	T_LeCmdAppSendToUartFp							LeCmdAppSendToUart;
	T_LeCmdAppBuildAddrStrFp						LeCmdAppBuildAddrStr;
	T_LeCmdAppBuildUuidStrFp						LeCmdAppBuildUuidStr;
	T_LeCmdAppShowStateConnectedFp					LeCmdAppShowStateConnected;
	T_LeCmdAppSendBlePubAddrRspFp					LeCmdAppSendBlePubAddrRsp;
	T_LeCmdAppSendBleAdvParamRspFp					LeCmdAppSendBleAdvParamRsp;
	T_LeCmdAppSendBleGattcPrimSrvRspFp				LeCmdAppSendBleGattcPrimSrvRsp;
	T_LeCmdAppSendBleGattcNoPrimSrvRspFp			LeCmdAppSendBleGattcNoPrimSrvRsp;
	T_LeCmdAppSendBleGattcInclSrvRspFp				LeCmdAppSendBleGattcInclSrvRsp;
	T_LeCmdAppSendBleGattcCharInfoRspFp				LeCmdAppSendBleGattcCharInfoRsp;
	T_LeCmdAppSendBleGattcDescInfoRspFp				LeCmdAppSendBleGattcDescInfoRsp;
	T_LeCmdAppSendBleGattcCharReadRspFp				LeCmdAppSendBleGattcCharReadRsp;
	T_LeCmdAppSendBleDisconnRspFp					LeCmdAppSendBleDisconnRsp;
	T_LeCmdAppSendBleGattsSrvRspFp					LeCmdAppSendBleGattsSrvRsp;
	T_LeCmdAppSendBleGattsCharRspFp					LeCmdAppSendBleGattsCharRsp;
	T_LeCmdAppHandleOccupyUartFp					LeCmdAppHandleOccupyUart;
	T_LeCmdAppGetAddrFromStrFp						LeCmdAppGetAddrFromStr;
	T_LeCmdAppFindAttrByUuidFp						LeCmdAppFindAttrByUuid;
	T_LeCmdAppCheckInitRoleFp						LeCmdAppCheckInitRole;
	T_LeCmdAppCheckPeerIsBusyFp						LeCmdAppCheckPeerIsBusy;
	T_LeCmdAppCheckConnIdxFp						LeCmdAppCheckConnIdx;
	T_LeCmdAppCheckPeerSrvIdxFp						LeCmdAppCheckPeerSrvIdx;
	T_LeCmdAppCheckPeerCharIdxFp					LeCmdAppCheckPeerCharIdx;
	T_LeCmdAppCheckPeerDescIdxFp					LeCmdAppCheckPeerDescIdx;
	T_LeCmdAppOccupyUartStartFp						LeCmdAppOccupyUartStart;
	T_LeCmdAppCheckOurSrvIdxFp						LeCmdAppCheckOurSrvIdx;
	T_LeCmdAppCheckOurCharIdxFp						LeCmdAppCheckOurCharIdx;
	T_LeCmdAppClearSendFlagFp						LeCmdAppClearSendFlag;
    
} LE_HOST_CMD_APP_DATA_T;


LE_HOST_CMD_APP_DATA_T* LeHostCmdApp(void);

#endif

#endif
