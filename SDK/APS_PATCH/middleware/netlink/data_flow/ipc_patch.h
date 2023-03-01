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

#ifndef __IPC_H__
#error "Please include ipc.h. Don't directly include ipc_patch.h"
#endif

#ifndef __IPC_PATCH_H__
#define __IPC_PATCH_H__


#include "ipc.h"

#define IPC_SPARE0_PATCH_SYNC_BIT                  13
#define IPC_SPARE0_PATCH_SYNC_MASK                 (1UL << IPC_SPARE0_32K_SRC_SEL_BIT)
#define IPC_SPARE0_WARMBOOT_XTAL_NOT_RDY_BIT       14
#define IPC_SPARE0_WARMBOOT_XTAL_NOT_RDY_MASK      (1UL << IPC_SPARE0_WARMBOOT_XTAL_NOT_RDY_BIT)

#define IPC_WIFI_DECRYPT_BUF_SIZE       2400

#define IPC_WIFI_MSQ_TX_DESC_NUM_PATCH  8


extern uint32_t g_u32IpcWifiDecryptBufAddr;

void Ipc_PatchInit(void);


#endif //#ifndef __IPC_PATCH_H__

