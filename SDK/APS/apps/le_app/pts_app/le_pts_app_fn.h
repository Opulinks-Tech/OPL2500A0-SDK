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
#ifndef _LE_PTS_APP_FN_H_
#define _LE_PTS_APP_FN_H_

#include "le_host.h"
#include "le_gap_if.h"
#include "le_gatt_if.h"
#include "le_cm_if.h"

#define INCLUDE_PTS_APP

#ifdef INCLUDE_PTS_APP

// le_pts_app
typedef struct _LE_HOST_PTS_T LE_HOST_PTS_T;
typedef struct _LE_HOST_PTS_APP_DATA_T LE_HOST_PTS_APP_DATA_T;
typedef struct _LE_TEST_GPA_BV_11_T LE_TEST_GPA_BV_11_T;

typedef BOOL (*T_LePtsAppSetGetPtsAddrFp)(BOOL set, LE_BT_ADDR_T *pts_addr);
typedef BOOL (*T_LePtsAppSetDirectAdvtisingParaFp)(LE_BT_ADDR_T *own_rnd_addr, LE_BT_ADDR_T *peer_addr);
typedef void (*T_LePtsAppSetAdvtisingParaFp)(UINT8 type, UINT8 own_addr_type, LE_BT_ADDR_T *peer_addr, UINT8 filter);
typedef void (*T_LePtsAppSendPasskeyDisplayFp)(UINT32 passkey);
typedef void (*T_LePtsAppSendUserConfirmDisplayFp)(UINT32 passkey);

typedef void (*T_LePtsAppShowAdvReportFp)(LE_CM_MSG_ADVERTISE_REPORT_IND_T *ind);
typedef BOOL (*T_LePtsAppFindAdvTypeSectionFp)(UINT16 len, UINT8 *data, UINT8 type, UINT8 **begin);
typedef void (*T_LePtsAppHandleAdvertisingReportIndFp)(LE_CM_MSG_ADVERTISE_REPORT_IND_T *ind);
typedef void (*T_LePtsAppHandleDisconnectCompleteIndFp)(LE_CM_MSG_DISCONNECT_COMPLETE_IND_T *ind);

typedef void (*T_LePtsAppCmTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_LePtsAppUserTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_LePtsAppSmpTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_LePtsAppTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);
typedef void (*T_LePtsAppSetAdvScanDataFp)(void);
typedef void (*T_LePtsAppInitFp)(void);
typedef LE_HOST_PTS_T* (*T_LeHostPtsFp)(void);

extern T_LeHostPtsFp LeHostPtsApi;

// le_pts_app_db
typedef struct _LE_PTS_GATT_TABLE_T LE_PTS_GATT_TABLE_T;

typedef void* (*T_LePtsAppBuildSvcAttrFp)(LE_GATT_ATTR_T *att_db, UINT16 count, UINT16 *start_hdl);
typedef UINT16 (*T_LePtsAppBuildGapSvcTblFp)(UINT16 *start_hdl, LE_PTS_GATT_TABLE_T *att_tbl);
typedef UINT16 (*T_LePtsAppBuildBlpsSvcTblFp)(UINT16 *start_hdl, LE_PTS_GATT_TABLE_T *att_tbl);
typedef UINT16 (*T_LePtsAppBuildHidSvcTblFp)(UINT16 *start_hdl, LE_PTS_GATT_TABLE_T *att_tbl);
typedef UINT16 (*T_LePtsAppBuildGattSvcTblFp)(UINT16 *start_hdl, LE_PTS_GATT_TABLE_T *att_tbl);
typedef void (*T_LePtsAppGattDbInitFp)(void);
typedef LE_PTS_GATT_TABLE_T* (*T_LePtsAppSvcTblFp)(UINT16 sid);

// le_pts_app_gatt

typedef LE_GATT_SERVICE_T* (*T_LePtsAppGetSvcFp)(UINT16 sid);
typedef UINT16 (*T_LePtsAppGetServiceHandleFp)(UINT16 sid, UINT16 attId);
typedef void (*T_LePtsAppShowReceivedDataFp)(UINT16 handle, UINT8 flag, UINT16 offset, UINT16 len, UINT8 *data, UINT8 type);
typedef void (*T_LePtsAppHandleGapServiceReadFp)(LE_GATT_MSG_ACCESS_READ_IND_T *ind);
typedef void (*T_LePtsAppHandleGapServiceWriteFp)(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind);
typedef void (*T_LePtsAppHandleHidServiceReadFp)(LE_GATT_MSG_ACCESS_READ_IND_T *ind);
typedef void (*T_LePtsAppHandleHidServiceWriteFp)(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind);
typedef void (*T_LePtsAppHandleBpServiceReadFp)(LE_GATT_MSG_ACCESS_READ_IND_T *ind);
typedef void (*T_LePtsAppHandleBpServiceWriteFp)(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind);
typedef void (*T_LePtsAppHandleGattServiceReadFp)(LE_GATT_MSG_ACCESS_READ_IND_T *ind);
typedef void (*T_LePtsAppHandleGattServiceWriteFp)(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind);
typedef void (*T_LePtsAppGattHandleReadIndFp)(LE_GATT_MSG_ACCESS_READ_IND_T *ind);
typedef void (*T_LePtsAppGattHandleWriteIndFp)(LE_GATT_MSG_ACCESS_WRITE_IND_T *ind);
typedef void (*T_LePtsAppGapBv11ShowResultFp)(LE_TEST_GPA_BV_11_T *gpa_bv_11);
typedef void (*T_LePtsAppHandleGattInitCfmFp)(void);
typedef void (*T_LePtsAppHandleGattServiceInfoIndFp)(LE_GATT_MSG_SERVICE_INFO_IND_T *ind);
typedef void (*T_LePtsAppHandleGattIncludeServiceInfoIndFp)(LE_GATT_MSG_INCLUDE_SERVICE_INFO_IND_T *ind);
typedef void (*T_LePtsAppHandleGattCharDeclInfoIndFp)(LE_GATT_MSG_CHARACTERISTIC_DECL_INFO_IND_T *ind);
typedef void (*T_LePtsAppHandleGattFindCharCfmFp)(LE_GATT_MSG_FIND_CHARACTERISTIC_CFM_T *cfm);
typedef void (*T_LePtsAppHandleGattCharValIndFp)(LE_GATT_MSG_CHARACTERISTIC_VAL_IND_T *ind);
typedef void (*T_LePtsAppHandleGattReadCharValByUuidCfmFp)(LE_GATT_MSG_READ_CHAR_VAL_BY_UUID_CFM_T *cfm);
typedef void (*T_LePtsAppHandleGattReadLongCharValCfmFp)(LE_GATT_MSG_READ_LONG_CHAR_VAL_CFM_T *cfm);
typedef void (*T_LePtsAppHandleGattReadMultipleCharValCfmFp)(LE_GATT_MSG_READ_MULTIPLE_CHAR_VAL_CFM_T *cfm);
typedef void (*T_LePtsAppHandleGattIndicateIndFp)(LE_GATT_MSG_INDICATE_IND_T *ind);
typedef void (*T_LePtsAppHandleGattNotifyIndFp)(LE_GATT_MSG_NOTIFY_IND_T *ind);
typedef void (*T_LePtsAppGattTaskHandlerFp)(TASK task, MESSAGEID id, MESSAGE message);

// le_pts_app_cmd
typedef struct _LE_PTS_CMD_T LE_PTS_CMD_T;
typedef struct _LE_PTS_CMD_ITEM_T LE_PTS_CMD_ITEM_T;
typedef struct _LE_PTS_SUB_CMD_ITEM_T LE_PTS_SUB_CMD_ITEM_T;

typedef UINT8 (*LePtsCmdHandler)(UINT16 argc, UINT8 **arg);
typedef UINT8 (*T_LePtsGetAttrLenFp)(UINT16 sid, UINT16 attrId, UINT16 *valLen);
typedef BOOL (*T_LeHtcUtilHexStrToNumFp)(UINT8 *data, UINT16 len, UINT8 *buf, UINT16 bufLen);
typedef void* (*T_LePtsGattCmdTableFp)(void);
typedef void* (*T_LePtsSpecialCmdTableFp)(void);
typedef void* (*T_LePtsTestCmdTableFp)(void);
typedef void (*T_LePtsSendScanningMsgFp)(UINT8 enable, UINT8 filter, UINT16 id);
typedef UINT16 (*T_LePtsTestCmdUtilGenerateKeyPairFp)(void *pub, UINT32 *priv, BOOL gen_rnd);
typedef UINT16 (*T_LePtsTestCmdUtilGenDhkeyFp)(UINT8 *peer_pub_key_x, UINT8 *peer_pub_key_y, UINT32 *our_priv_key, void *dhkey);
typedef void* (*T_LePtsCmdTableFp)(void);
typedef BOOL (*T_LePtsAppFindCmdIdxFp)(LE_PTS_CMD_ITEM_T *tbl, UINT16 cmdLen, UINT8 *cmd, UINT16 *idx);
typedef BOOL (*T_LePtsParseCmdParaFp)(UINT8 *pBuf, UINT16 *argc, UINT8 **arg);
typedef UINT8 (*T_LePtsAppRunCmdFp)(LE_PTS_CMD_T *cmd);
typedef void (*T_LePtsLoopHandlerFp)(void);
typedef void (*T_LePtsCmdFinishFp)(UINT16 cmd_id);
typedef BOOL (*T_LePtsAppFindCmdFp)(UINT8 *str, UINT16 len, UINT8 *cmdLen, UINT8 **cmd);
typedef BOOL (*T_LePtsAppCheckCmdFp)(UINT8 *data, UINT16 len);
typedef void (*T_ParseLePtsTestCommandFp)(char* pszData, int dataLen);

// static data

typedef void (*T_LePtsAppFnSetFp)(void);


typedef struct _LE_HOST_PTS_T
{
	TASKPACK						task;

	void							*app;
	LE_PTS_CMD_ITEM_T 				*cmd_table;
	LE_PTS_CMD_ITEM_T 				*ext_cmd_table;
    LE_QLIST_T						cmdq;
    void							*curr_cmd;
    BOOL							cmd_hdlr;
	void							*additional;

	// cmd table patch
	T_LePtsAppFnSetFp				LePtsCmdTablePatch;
    
    // set function
	T_LePtsAppFnSetFp				LePtsAppPatch_FnSet;
	T_LePtsAppFnSetFp				LePtsApp_FnSet;
	T_LePtsAppFnSetFp				LePtsAppDb_FnSet;
	T_LePtsAppFnSetFp				LePtsAppGatt_FnSet;
	T_LePtsAppFnSetFp				LePtsAppCmd_FnSet;
    
	// internal
	T_LePtsCmdTableFp 				LePtsCmdTable;
    T_LePtsAppFindCmdIdxFp			LePtsAppFindCmdIdx;
	T_LePtsParseCmdParaFp 			LePtsParseCmdPara;
	T_LePtsAppRunCmdFp 				LePtsAppRunCmd;
    T_LePtsCmdFinishFp				LePtsCmdFinish;
    T_LePtsLoopHandlerFp			LePtsLoopHandler;
	T_LePtsAppFindCmdFp 			LePtsAppFindCmd;
	T_LePtsAppCheckCmdFp 			LePtsAppCheckCmd;

    // API
	T_LePtsAppInitFp 				LePtsAppInit;
    T_ParseLePtsTestCommandFp 		ParseLePtsTestCommandApi;
} LE_HOST_PTS_T;


LE_HOST_PTS_T* LeHostPts(void);

void ParseLePtsTestCommand(char* pszData, int dataLen);

void LePtsAppPatch(void);

void LePtsAppDbPatch(void);

void LePtsAppGattPatch(void);

void LePtsAppCmdPatch(void);

#endif

#endif
