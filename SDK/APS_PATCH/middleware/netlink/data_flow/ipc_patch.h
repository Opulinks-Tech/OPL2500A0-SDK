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

#undef IPC_LOGE
#define IPC_LOGE(...)

//#define IPC_WIFI_DESC_DUMP
//#define IPC_WIFI_DESC_CMD

#define IPC_SPARE0_PATCH_SYNC_BIT                  13
#define IPC_SPARE0_PATCH_SYNC_MASK                 (1UL << IPC_SPARE0_32K_SRC_SEL_BIT)
#define IPC_SPARE0_WARMBOOT_XTAL_NOT_RDY_BIT       14
#define IPC_SPARE0_WARMBOOT_XTAL_NOT_RDY_MASK      (1UL << IPC_SPARE0_WARMBOOT_XTAL_NOT_RDY_BIT)

#ifdef WIFI_TPUT
    #ifdef PTA_FEATURE
        #define IPC_WIFI_DECRYPT_BUF_SIZE       2368
    #else
        #define IPC_WIFI_DECRYPT_BUF_SIZE       2384
    #endif
#else
    #ifdef PTA_FEATURE
        #define IPC_WIFI_DECRYPT_BUF_SIZE       2384
    #else
        #define IPC_WIFI_DECRYPT_BUF_SIZE       2400
    #endif
#endif

#if 1
    #define IPC_WIFI_MSQ_TX_DESC_NUM_PATCH  5
    #define IPC_WIFI_MSQ_RX_DESC_NUM_PATCH  12
    #define IPC_WIFI_APS_RX_DESC_NUM_PATCH  20
        
    #define IPC_WIFI_MSQ_RX_LAST_MEM_BLK_SIZE_PATCH     1584
    #define IPC_WIFI_APS_RX_LAST_MEM_BLK_SIZE_PATCH     1584
#else
    #define IPC_WIFI_MSQ_TX_DESC_NUM_PATCH  6
    #define IPC_WIFI_MSQ_RX_DESC_NUM_PATCH  8
#endif

#ifdef WIFI_TPUT
    #define IPC_WIFI_TPUT_ADDR_SIZE     (sizeof(T_IpcWifiTputAddr))
    #define IPC_WIFI_TPUT_INFO_SIZE     64
#endif

#ifdef PTA_FEATURE

    #ifdef PTA_DBG_IO
    #define IPC_WIFI_APS_TX_DESC_NUM_PATCH  12
    #else
    #define IPC_WIFI_APS_TX_DESC_NUM_PATCH  14
    #endif

#define IPC_PTA_ADDR_SIZE               (sizeof(T_IpcPtaAddr))
#define IPC_PTA_CFG_SIZE                16
#endif

#define IPC_WIFI_DESC_SIZE              sizeof(T_IpcWifiDesc) // Size of descriptor must be multiple of 16 because the beginning is Tx/Rx DMA command.


typedef struct
{
    uint32_t u32DescPool;
    uint32_t u32MemPool;
    uint32_t u32ApsTxNum;
    uint32_t u32ApsRxNum;
} T_IpcDescCfg;

#ifdef WIFI_TPUT
typedef struct
{
    uint32_t u32StatsAddr;
} T_IpcWifiTputAddr;
#endif

#ifdef PTA_FEATURE
typedef struct
{
    uint32_t u32CfgAddr;
} T_IpcPtaAddr;
#endif


extern uint32_t g_u32IpcWifiDecryptBufAddr;

#ifdef WIFI_TPUT
extern uint32_t g_u32IpcWifiTputStatsAddr;
#endif

#ifdef PTA_FEATURE
extern uint32_t g_u32IpcPtaCfgAddr;
#endif

void Ipc_PatchInit(void);

void ipc_desc_update(T_IpcDescCfg *ptCfg);


#endif //#ifndef __IPC_PATCH_H__

