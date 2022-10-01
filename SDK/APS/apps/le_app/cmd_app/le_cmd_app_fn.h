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

#ifndef _LE_CMD_APP_FN_H_
#define _LE_CMD_APP_FN_H_

#include "le_host.h"
#include "le_gap_if.h"
#include "le_gatt_if.h"
#include "le_cm_if.h"

#define INCLUDE_CMD_APP

#ifdef INCLUDE_CMD_APP

#define LE_ACTION_FOR_DEVELOP
#define LE_DB_FROM_INTERNAL
#define VER_AO_ROM
#define LE_CMD_APP_OPTION_CMD

#define LE_HTC_SUCCESS			0

// le_cmd_app
typedef struct _LE_HOST_ATCMD_DEV_T LE_HOST_ATCMD_DEV_T;
typedef struct _LE_HOST_CMD_APP_DATA_T LE_HOST_CMD_APP_DATA_T;
typedef struct _LE_CMD_APP_PEER_T LE_CMD_APP_PEER_T;

#ifdef LE_ACTION_FOR_DEVELOP
typedef void (*T_LeCmdAppSetAdvScanDataFp)(void);
typedef void (*T_LeCmdAppSetAdvParaFp)(void);
typedef void (*T_LeCmdAppSendPasskeyDisplayFp)(UINT32 passkey);
typedef void (*T_LeCmdAppSendUserConfirmDisplayFp)(UINT32 passkey);
#endif
typedef void* (*T_LeCmdAppGetPeerFreeFp)(UINT16 conn_hdl);
typedef void (*T_LeCmdAppHandleCmInitCompleteCfmFp)(void);
typedef void (*T_LeCmdAppHandleCmExitAdvtiseCfmFp)(LE_CM_MSG_EXIT_ADVERTISING_CFM_T *cfm);
typedef void (*T_LeCmdAppHandleCmEnterAdvtiseCfmFp)(LE_CM_MSG_ENTER_ADVERTISING_CFM_T *cfm);
typedef void (*T_LeCmdAppHandleCmConnParaReqFp)(LE_CM_MSG_CONN_PARA_REQ_T *req);
typedef void (*T_LeCmdAppHandleCmSignalUpdataReqFp)(LE_CM_MSG_SIG_CONN_UPDATE_REQ_T *req);
typedef void (*T_LeCmdAppHandleCmConnUpdateCompleteIndFp)(LE_CM_MSG_CONN_UPDATE_COMPLETE_IND_T *ind);
typedef void (*T_LeCmdAppHandleCmConnectCompleteIndFp)(LE_CM_MSG_CONNECTION_COMPLETE_IND_T *ind);
typedef void (*T_LeCmdAppHandleCmDisconnectCompleteIndFp)(LE_CM_MSG_DISCONNECT_COMPLETE_IND_T *ind);
typedef void (*T_LeCmdAppCmTaskOtherHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_LeCmdAppCmTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_LeCmdAppUserTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_LeCmdAppSmpTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_LeCmdAppTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void* (*T_LeCmdAppGetPeerByHdlFp)(UINT16 conn_hdl);
typedef void (*T_LeCmdAppFreePeerGattDbFp)(LE_CMD_APP_PEER_T *peer);
typedef void (*T_LeCmdAppClearSendFlagFp)(void);
typedef void (*T_LeCmdAppInitFp)(void);
typedef LE_HOST_ATCMD_DEV_T* (*T_LeHostAtCmdDevFp)(void);

extern T_LeHostAtCmdDevFp LeHostAtCmdDevApi;

// le_cmd_app_db
typedef struct _LE_GATT_DB_SVC_T LE_GATT_DB_SVC_T;
typedef struct _LE_GATT_DB_T LE_GATT_DB_T;
#ifdef LE_DB_FROM_INTERNAL
typedef struct _LE_GATT_TABLE_T LE_GATT_TABLE_T;

typedef UINT16 (*T_LeGattDbCalRawDbSizeFp)(LE_GATT_ATTR_T *svc, UINT16 count);
typedef void (*T_LeGattDbInternalFillFp)(LE_GATT_ATTR_T *svc, UINT16 count, UINT8 **buf, UINT16 *start_hdl);
typedef void* (*T_LeGattDbBuildSvcAttrFp)(LE_GATT_ATTR_T *att_db, UINT16 count, UINT16 *start_hdl);
typedef UINT16 (*T_LeGattDbBuildGattSvcTblFp)(UINT16 *start_hdl, LE_GATT_TABLE_T *att_tbl);
typedef UINT16 (*T_LeGattDbBuildGapSvcTblFp)(UINT16 *start_hdl, LE_GATT_TABLE_T *att_tbl);
typedef UINT16 (*T_LeGattDbBuildBlpsSvcTblFp)(UINT16 *start_hdl, LE_GATT_TABLE_T *att_tbl);
typedef UINT16 (*T_LeGattDbBuildHidSvcTblFp)(UINT16 *start_hdl, LE_GATT_TABLE_T *att_tbl);
#ifdef VER_AO_ROM
typedef void (*T_LeGattDbDumpCsvTableFp)(UINT8 *raw);
#endif
typedef void (*T_LeGattDbInternalInitFp)(void);
#endif
typedef BOOL (*T_LeReadFromFlashFp)(void *buf, UINT16 len, UINT16 offset);
typedef void (*T_LeGattDbClearFp)(void);
typedef LE_ERR_STATE (*T_LeGattDbCheckItemFp)(LE_GATT_DB_T *db);
typedef LE_ERR_STATE (*T_LeGattDbBuildHeaderFp)(LE_GATT_DB_T *db);
typedef BOOL (*T_LeGattDbFindIncludeServiceFp)(LE_GATT_DB_T *db, UINT16 svcIdx, UINT16 incAttrId);
typedef LE_ERR_STATE (*T_LeGattDbCheckIncludeServiceFp)(LE_GATT_DB_T *db);
typedef UINT16 (*T_LeGattDbCheckPermitFp)(UINT16 dbperm);
typedef BOOL (*T_LeGattDbCheckAttrIsValidFp)(LE_GATT_DB_SVC_T *svc, UINT16 idx);
typedef LE_ERR_STATE (*T_LeGattDbCheckAttrFp)(LE_GATT_DB_T *db);
typedef LE_ERR_STATE (*T_LeGattDbBuildAttrFp)(LE_GATT_DB_T *db);
typedef LE_ERR_STATE (*T_LeGattDbRegServiceDbFp)(LE_GATT_DB_T *db);
typedef LE_ERR_STATE (*T_LeGattDbBuildServiceDbFp)(UINT16 *count);
typedef LE_GATT_SERVICE_T* (*T_LeGattDbGetServiceIdFp)(UINT16 idx);
#ifdef LE_DB_FROM_INTERNAL
typedef void (*T_LeGattDbEnableInternalDbFp)(void);
#endif

// le_cmd_app_cmd
typedef struct _LE_APP_PEER_SVC_T LE_APP_PEER_SVC_T;
typedef struct _LE_APP_PEER_CHAR_T LE_APP_PEER_CHAR_T;
typedef struct _LE_APP_PEER_ATTR_T LE_APP_PEER_ATTR_T;
typedef struct _LE_APP_GATT_SVR_T LE_APP_GATT_SVR_T;
typedef struct _LE_APP_GATT_SVC_T LE_APP_GATT_SVC_T;
typedef struct _LE_APP_GATT_CHAR_T LE_APP_GATT_CHAR_T;
typedef struct _LE_HOST_AT_CMD_T LE_HOST_AT_CMD_T;
typedef struct _LE_AT_CMD_ITEM_T LE_AT_CMD_ITEM_T;

typedef void (*T_LeCmdAppSendToUartFp)(char *fmt, ...);
typedef void (*T_LeCmdAppBuildAddrStrFp)(BD_ADDR addr, void *buf);
typedef void (*T_LeCmdAppBuildUuidStrFp)(UINT8 format, void *uuid, UINT8 *buf);
typedef void (*T_LeCmdAppSendBlePubAddrRspFp)(BD_ADDR addr);
typedef void (*T_LeCmdAppSendBleAdvParamRspFp)(LE_GAP_ADVERTISING_PARAM_T *adv_para);
typedef void (*T_LeCmdAppSendBleGattsSrvRspFp)(UINT16 srv_idx, BOOL start, UINT8 format, UINT16 *uuid, UINT8 srv_type);
typedef void (*T_LeCmdAppSendBleGattsCharRspFp)(UINT16 srv_idx, UINT16 char_idx, UINT8 format, void *uuid, UINT8 char_prop);
typedef BOOL (*T_LeCmdAppHandleOccupyUartFp)(UINT8 *data, UINT16 len);
typedef BOOL (*T_LeCmdAppGetAtCmdFp)(UINT8 *data, UINT16 len, UINT8 *cmd_type, UINT8 *cmd_len);
typedef UINT8 (*T_LeCmdAppParseCmdParaFp)(UINT8 *pBuf, UINT16 *argCount, UINT8 **arg);
typedef BOOL (*T_LeCmdAppGetAddrFromStrFp)(UINT8 *addr_str, BD_ADDR addr);
typedef BOOL (*T_LeCmdAppFindAttrByUuidFp)(LE_GATT_SERVICE_T **svc, UINT16 *attrId, UINT8 format, UINT16 *uuid);
typedef UINT16 (*T_LeCmdAppCheckInitRoleFp)(UINT8 role);
typedef UINT16 (*T_LeCmdAppCheckPeerIsBusyFp)(LE_CMD_APP_PEER_T *peer);
typedef UINT16 (*T_LeCmdAppCheckConnIdxFp)(UINT8 *idstr, LE_CMD_APP_PEER_T **curr_peer);
typedef UINT16 (*T_LeCmdAppCheckPeerSrvIdxFp)(UINT8 *idstr, LE_CMD_APP_PEER_T *peer, LE_APP_PEER_SVC_T **curr_svc);
typedef UINT16 (*T_LeCmdAppCheckPeerCharIdxFp)(UINT8 *idstr, LE_APP_PEER_SVC_T *svc, LE_APP_PEER_CHAR_T **curr_chara);
typedef UINT16 (*T_LeCmdAppCheckPeerDescIdxFp)(UINT8 *idstr, LE_APP_PEER_CHAR_T *chara, LE_APP_PEER_ATTR_T **desc);
typedef void (*T_LeCmdAppOccupyUartStartFp)(UINT16 len);
typedef UINT16 (*T_LeCmdAppCheckOurSrvIdxFp)(UINT8 *idstr, LE_APP_GATT_SVR_T *srv, LE_APP_GATT_SVC_T **curr_svc);
typedef UINT16 (*T_LeCmdAppCheckOurCharIdxFp)(UINT8 *idstr, LE_APP_GATT_SVC_T *svc, LE_APP_GATT_CHAR_T **curr_char);

typedef void (*T_LeHostAtCmdHandlerFp)(UINT8 cmd_type, UINT16 argc, UINT8 **arg);
typedef void (*T_LeCmdAppCmdHandlerFp)(LE_HOST_AT_CMD_T *cmd);
typedef void (*T_LeCmdAppDataHandlerFp)(void);
typedef void (*T_LeAtCmdTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE msg);
typedef void* (*T_LeHostAtCmdTableFp)(void);
typedef BOOL (*T_LeCmdAppCheckAtCmdFp)(char* pszData, int dataLen);
typedef BOOL (*T_LeCmdAppCheckDataFromUartFp)(char* pszData, int dataLen);

typedef void (*T_LeCmdAppSendAtCmdRspFp)(char *fmt, ...);
typedef void (*T_LeCmdAppShowStateConnectedFp)(UINT8 conn_idx, UINT8 addr_type, BD_ADDR addr);
typedef void (*T_LeCmdAppSendAtCmdAckFp)(void *str, UINT16 err);
typedef void (*T_LeCmdAppSendBleGattcPrimSrvRspFp)(UINT16 conn_idx, UINT16 srv_idx, UINT8 format, UINT16 *uuid, UINT8 srv_type);
typedef void (*T_LeCmdAppSendBleGattcNoPrimSrvRspFp)(UINT16 conn_idx);
typedef void (*T_LeCmdAppSendBleGattcInclSrvRspFp)(LE_CMD_APP_PEER_T *peer, UINT16 srv_idx, UINT8 format, UINT16 *incl_uuid, UINT8 incl_type);
typedef void (*T_LeCmdAppSendBleGattcCharInfoRspFp)(UINT16 conn_idx, UINT16 srv_idx, UINT16 char_idx, UINT8 format, UINT16 *uuid, UINT8 char_prop);
typedef void (*T_LeCmdAppSendBleGattcDescInfoRspFp)(UINT16 conn_idx, UINT16 srv_idx, UINT16 char_idx, UINT16 desc_idx, UINT8 format, UINT16 *uuid);
typedef void (*T_LeCmdAppSendBleGattcCharReadRspFp)(UINT16 conn_idx, UINT16 len, UINT8 *val);
typedef void (*T_LeCmdAppSendBleDisconnRspFp)(UINT16 conn_idx, BD_ADDR addr);
typedef void (*T_ParseLeCmdTestCommandFp)(char* pszData, int dataLen);

// le_cmd_app_gatt
typedef UINT16 (*T_LeCmdAppGetCharFp)(LE_GATT_SERVICE_T *svc, LE_APP_GATT_CHAR_T **chara);
typedef LE_ERR_STATE (*T_LeCmdAppBuildGattTableFp)(void);
typedef void (*T_LeCmdAppHandleGattInitCfmFp)(void);
typedef void (*T_LeCmdAppHandleGattAccessReadIndFp)(LE_GATT_MSG_ACCESS_READ_IND_T *ind);
typedef void (*T_LeCmdAppHandleGattAccessWriteIndFp)(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind);
typedef void (*T_LeCmdAppHandleGattExchangeMtuIndFp)(LE_GATT_MSG_EXCHANGE_MTU_IND_T *ind);
typedef void (*T_LeCmdAppHandleGattExchangeMtuCfmFp)(LE_GATT_MSG_EXCHANGE_MTU_CFM_T *cfm);
typedef void (*T_LeCmdAppHandleGattServiceInfoIndFp)(LE_GATT_MSG_SERVICE_INFO_IND_T *ind);
typedef void (*T_LeCmdAppHandleGattFindAllPrimaryServiceCfmFp)(LE_GATT_MSG_FIND_ALL_PRIMARY_SERVICE_CFM_T *cfm);
typedef void (*T_LeCmdAppHandleGattCharDescInfoIndFp)(LE_GATT_MSG_CHAR_DESCRIPTOR_INFO_IND_T *ind);
typedef BOOL (*T_LeCmdAppBuildPeerGattTableFp)(LE_CMD_APP_PEER_T *peer);
typedef void (*T_LeCmdAppHandleGattFindAllCharDescCfmFp)(LE_GATT_MSG_FIND_ALL_CHAR_DESC_CFM_T *cfm);
typedef void (*T_LeCmdAppHandleGattInclServiceInfoIndFp)(LE_GATT_MSG_INCLUDE_SERVICE_INFO_IND_T *ind);
typedef void (*T_LeCmdAppHandleGattInclServiceInfoCfmFp)(LE_GATT_MSG_FIND_INCLUDED_SERVICE_CFM_T *cfm);
typedef void (*T_LeCmdAppHandleGattCharInfoIndFp)(LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND_T *ind);
typedef void (*T_LeCmdAppHandleFindGattCharCfmFp)(LE_GATT_MSG_FIND_CHARACTERISTIC_CFM_T *cfm);
typedef void (*T_LeCmdAppHandleGattCharValIndFp)(LE_GATT_MSG_CHARACTERISTIC_VAL_IND_T *ind);
typedef void (*T_LeCmdAppHandleGattReadLongCharValCfmFp)(LE_GATT_MSG_READ_LONG_CHAR_VAL_CFM_T *cfm);
typedef void (*T_LeCmdAppHandleGattWriteCharValCfmFp)(LE_GATT_MSG_WRITE_CHAR_VALUE_CFM_T *cfm);
typedef void (*T_LeCmdAppHandleGattConfirmCfmFp)(LE_GATT_MSG_CONFIRMATION_CFM_T *cfm);

typedef void (*T_LeCmdAppHandleGattOperationTimeoutFp)(LE_GATT_MSG_OPERATION_TIMEOUT_T *ind);
typedef void (*T_LeCmdAppGattTaskOtherHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);

typedef void (*T_LeCmdAppGattTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);

// static data
typedef void (*T_LeCmdAppFnSetFp)(void);


typedef struct _LE_HOST_ATCMD_DEV_T
{
	TASKPACK						task;
	void							*app;
	UINT8							role;
    LE_AT_CMD_ITEM_T				*cmd_table;
    UINT8							uart_sw;
#ifdef LE_ACTION_FOR_DEVELOP
	BOOL							auto_test;
#endif
#ifdef LE_DB_FROM_INTERNAL
	BOOL							inner_db;
#endif
	// cmd table patch
	T_LeCmdAppFnSetFp				LeAtCmdTablePatch;

    // set function
	T_LeCmdAppFnSetFp    			LeCmdAppPatch_FnSet;
	T_LeCmdAppFnSetFp    			LeCmdApp_FnSet;
	T_LeCmdAppFnSetFp    			LeCmdAppDb_FnSet;
	T_LeCmdAppFnSetFp    			LeCmdAppGatt_FnSet;
	T_LeCmdAppFnSetFp    			LeCmdAppCmd_FnSet;
    
	// internal
	T_LeHostAtCmdTableFp			LeHostAtCmdTable;
	T_LeCmdAppCheckAtCmdFp			LeCmdAppCheckAtCmd;
	T_LeCmdAppGetAtCmdFp			LeCmdAppGetAtCmd;
	T_LeCmdAppParseCmdParaFp		LeCmdAppParseCmdPara;
	T_LeAtCmdTaskHandlerFp			LeAtCmdTaskHandler;
	T_LeCmdAppCmdHandlerFp			LeCmdAppCmdHandler;
	T_LeCmdAppDataHandlerFp			LeCmdAppDataHandler;
	T_LeCmdAppSendAtCmdAckFp		LeCmdAppSendAtCmdAck;
    
    // API
	T_LeCmdAppSendAtCmdRspFp		LeCmdAppSendAtCmdRsp;
    
	T_LeCmdAppInitFp				LeCmdAppInit;
	T_ParseLeCmdTestCommandFp		ParseLeCmdTestCommandApi;
	T_LeCmdAppCheckDataFromUartFp	LeCmdAppCheckDataFromUart;

} LE_HOST_ATCMD_DEV_T;

LE_HOST_ATCMD_DEV_T* LeHostAtCmdDev(void);

void ParseLeCmdTestCommand(char* pszData, int dataLen);

void LeCmdAppPatch(void);

void LeCmdAppDbPatch(void);

void LeCmdAppGattPatch(void);

void LeCmdAppCmdPatch(void);

#endif

#endif
