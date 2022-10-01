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

#ifndef _BLE_SMP_IF_H_
#define _BLE_SMP_IF_H_

#include "ble.h"
#include "ble_msg.h"


#define LE_SM_IO_CAP_DISP_ONLY                 		0x00
#define LE_SM_IO_CAP_DISP_YES_NO               		0x01
#define LE_SM_IO_CAP_KEYBOARD_ONLY             		0x02
#define LE_SM_IO_CAP_NO_IO                     		0x03
#define LE_SM_IO_CAP_KEYBOARD_DISP             		0x04

#define LE_SM_PAIR_OOB_NO                      		0x00
#define LE_SM_PAIR_OOB_YES                     		0x01
#define LE_SM_PAIR_MITM_NO                      	0x00
#define LE_SM_PAIR_MITM_YES                     	0x01
#define LE_SM_PAIR_SC_NO                      		0x00
#define LE_SM_PAIR_SC_YES                     		0x01


enum
{
	LE_SMP_MSG_SLAVE_SECURITY_REQUEST_IND = LE_SMP_MSG_BASE,

	LE_SMP_MSG_PAIRING_ACTION_IND,
	LE_SMP_MSG_PASSKEY_DISPLAY_IND,
	LE_SMP_MSG_PASSKEY_INPUT_IND,
	LE_SMP_MSG_OOB_DATA_REQUEST_IND,
	LE_SMP_MSG_SC_OOB_DATA_REQUEST_IND,
	LE_SMP_MSG_USER_CONFIRM_IND,

	LE_SMP_MSG_ENCRYPTION_CHANGE_IND,
	LE_SMP_MSG_ENCRYPTION_REFRESH_IND,
	LE_SMP_MSG_PAIRING_COMPLETE_IND,
    
	LE_SMP_LONG_TERM_KEY_REQ,
	LE_SMP_KEYS_IND,

    LE_SMP_MSG_TOP
};

enum
{
	LE_SMP_PAIR_JUST_WORK,
	LE_SMP_PAIR_OOB,
	LE_SMP_PAIR_PASSKEY_INPUT,
	LE_SMP_PAIR_DISPLAY,
	LE_SMP_PAIR_NUM_COMPARE
};

typedef struct
{
    UINT16				conn_hdl;
	UINT8				bondable;
	UINT8				mitm;
	UINT8				sc;
	UINT8				keypress;
} LE_SMP_MSG_SLAVE_SECURITY_REQUEST_IND_T;

typedef struct
{
    UINT16				conn_hdl;
	UINT8				sc;
	UINT8				action;
    BOOL				lost_bond;
} LE_SMP_MSG_PAIRING_ACTION_IND_T;

typedef struct
{
    UINT16				conn_hdl;
	UINT32				passkey;
} LE_SMP_MSG_PASSKEY_DISPLAY_IND_T;

typedef struct
{
    UINT16				conn_hdl;
	UINT32				confirm_num;
} LE_SMP_MSG_USER_CONFIRM_IND_T;

typedef struct
{
    UINT16				conn_hdl;
} LE_SMP_MSG_PASSKEY_INPUT_IND_T;

typedef struct
{
    UINT16				conn_hdl;
} LE_SMP_MSG_OOB_DATA_REQUEST_IND_T;

typedef struct
{
    UINT16				conn_hdl;
} LE_SMP_MSG_SC_OOB_DATA_REQUEST_IND_T;

typedef struct
{
    UINT16				conn_hdl;
    BOOL				enable;
} LE_SMP_MSG_ENCRYPTION_CHANGE_IND_T;

typedef struct
{
    LE_ERR_STATE		status;
    UINT16				conn_hdl;
} LE_SMP_MSG_ENCRYPTION_REFRESH_IND_T;

typedef struct
{
    LE_ERR_STATE		status;
    UINT16				conn_hdl;
    UINT8				authenticated;
    UINT8				sc;
    UINT8				bonded;
    LE_BT_ADDR_T		peer_id_addr;
} LE_SMP_MSG_PAIRING_COMPLETE_IND_T;

typedef struct
{
	UINT8				rand[16];
	UINT8				confirm[16];
} LE_SMP_SC_OOB_DATA_T;


LE_ERR_STATE LeSmpInit(TASK appTask);

LE_ERR_STATE LeSmpMasterPairingReq(UINT16 conn_hdl);

LE_ERR_STATE LeSmpSecurityRsp(UINT16 conn_hdl, BOOL accept);

LE_ERR_STATE LeSmpSecurityReq(UINT16 conn_hdl);

LE_ERR_STATE LeSmpOobPresent(UINT16 conn_hdl, BOOL oob_present);

LE_ERR_STATE LeSmpPasskeyInput(UINT16 conn_hdl, UINT32 passkey);

LE_ERR_STATE LeSmpOobAuthDataRsp(UINT16 conn_hdl, UINT8 *data, UINT16 len);

LE_ERR_STATE LeSmpScOobComputeConfirmVal(UINT8 *rand, UINT8 *confirm);

LE_ERR_STATE LeSmpScOobDataRsp(UINT16 conn_hdl, UINT8 *our_rand, LE_SMP_SC_OOB_DATA_T *peer_oob_data);

LE_ERR_STATE LeSmpUserConfirmRsp(UINT16 conn_hdl, BOOL accept);

LE_ERR_STATE LeSmpSetDefaultConfig(UINT8 iocap, BOOL mitm, BOOL sc, BOOL bond);

LE_ERR_STATE LeSmpRemoveBond(LE_BT_ADDR_T *peer_addr);

BOOL LeSmpResolveRpa(UINT8 *irk, BD_ADDR addr);

UINT8 LeSmpGetBondIdFromAddr(LE_BT_ADDR_T *peer_addr);

UINT8 LeSmpGetIdAddrFormAddr(LE_BT_ADDR_T *peer_addr, LE_BT_ADDR_T *id_addr);


#endif
