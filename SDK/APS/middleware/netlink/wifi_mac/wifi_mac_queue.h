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

#ifndef _WIFI_MAC_QUEUE_H_
#define _WIFI_MAC_QUEUE_H_

#include "ipc.h"


#define BSS_INFO_ADDR               IPC_WIFI_BSS_INFO_START
#define DBG_TRX_CONFIG_PARAM_ADDR   IPC_DBG_TRX_PARAM_START
#define MIB_ADDR                    IPC_WIFI_MIB_START

#define WIFI_MAC_MSQ_TX_POLL_MAX    IPC_WIFI_MSQ_TX_ENTRY_NUM
#define WIFI_MAC_MSQ_RX_POLL_MAX    IPC_WIFI_MSQ_RX_ENTRY_NUM
#define WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE   IEEE80211_MAX_FRAME_LEN
#define WIFI_MAC_MSQ_TX_BUF_SIZE    (WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE + sizeof(S_WIFI_TX_FRM_INFO))
#define WIFI_MAC_MSQ_RX_BUF_SIZE    (WIFI_MAC_MSQ_MPDU_RX_MAX_SIZE + sizeof(S_WIFI_RX_FRM_INFO))

#define WIFI_MAC_APS_TX_POLL_MAX    IPC_WIFI_APS_TX_ENTRY_NUM
#define WIFI_MAC_APS_RX_POLL_MAX    IPC_WIFI_APS_RX_ENTRY_NUM
#define WIFI_MAC_APS_MPDU_RX_MAX_SIZE   IEEE80211_MAX_FRAME_LEN
#define WIFI_MAC_APS_TX_BUF_SIZE    (WIFI_MAC_APS_MPDU_RX_MAX_SIZE + sizeof(S_WIFI_TX_FRM_INFO))
#define WIFI_MAC_APS_RX_BUF_SIZE    (WIFI_MAC_APS_MPDU_RX_MAX_SIZE + sizeof(S_WIFI_RX_FRM_INFO))

#define WIFI_Q_APS_TX_ETNRY_SET_DONE    ipc_wifi_aps_tx_set_done
#define WIFI_Q_APS_RX_ETNRY_SET_DONE    ipc_wifi_aps_rx_set_done


typedef enum {
    Q_CLI_COMMAND,
    Q_DATA_MSQ_TX,
    Q_DATA_MSQ_RX,
    Q_DATA_APS_TX,
    Q_DATA_APS_RX,
} E_ENTRY_TYPE;


typedef struct
{
    union
    {
        S_WIFI_TX_FRM_INFO  sTxFrmInfo;
        S_WIFI_RX_FRM_INFO  sRxFrmInfo;
    };

    T_IpcWifiDesc       *psDsc;
    void                *pvNextEntry;
    UINT16              u16SeqCtrl;

    UINT8   u8Type;             // cli_command,
                                // aps_tx_data, aps_rx_data,
                                // msq_tx_data, msq_rx_data
} S_WIFI_ENTRY, S_WIFI_TX_ENTRY, S_WIFI_RX_ENTRY;


typedef void *(*WifiQ_TxEntrySet_fp)(uint32_t u32Len);
typedef void *(*WifiQ_ApsTxEntrySet_fp)(uint32_t u32Handle, uint32_t u32Len);
typedef void *(*WifiQ_RxEntrySet_fp)(uint32_t u32Len);
typedef void *(*WifiQ_EntryGet_fp)(void);
typedef void *(*WifiQ_ApsTxEntryGet_fp)(uint32_t u32Handle);
typedef void (*WifiQ_EntryGetDone_fp)(uint32_t u32Num);
typedef void (*WifiQ_ApsTxEntryGetDone_fp)(uint32_t u32Handle, uint32_t u32Num);
typedef void (*WifiQ_EntryReset_fp)(void);
typedef void (*WifiQ_ApsTxEntryReset_fp)(uint32_t u32Handle);


extern WifiQ_EntryGetDone_fp WifiQ_MsqTxEntryGetDone;
extern WifiQ_EntryReset_fp WifiQ_MsqTxEntryReset;

extern WifiQ_EntryGetDone_fp WifiQ_MsqRxEntryGetDone;
extern WifiQ_EntryReset_fp WifiQ_MsqRxEntryReset;

extern WifiQ_ApsTxEntrySet_fp WifiQ_ApsTxEntrySet;
extern WifiQ_ApsTxEntryGetDone_fp WifiQ_ApsTxEntryGetDone;
extern WifiQ_ApsTxEntryReset_fp WifiQ_ApsTxEntryReset;

extern WifiQ_RxEntrySet_fp WifiQ_ApsRxEntrySet;
extern WifiQ_EntryGet_fp WifiQ_ApsRxEntryGet;
extern WifiQ_EntryGetDone_fp WifiQ_ApsRxEntryGetDone;
extern WifiQ_EntryReset_fp WifiQ_ApsRxEntryReset;

extern WifiQ_EntryReset_fp WifiQ_DataEntryReset;


void WifiQ_MsqTxEntryGetDone_impl(uint32_t u32Num);
void WifiQ_MsqTxEntryReset_impl(void);

void WifiQ_MsqRxEntryGetDone_impl(uint32_t u32Num);
void WifiQ_MsqRxEntryReset_impl(void);

void *WifiQ_ApsTxEntrySet_impl(uint32_t u32Handle, uint32_t u32Len);
void WifiQ_ApsTxEntryGetDone_impl(uint32_t u32Handle, uint32_t u32Num);
void WifiQ_ApsTxEntryReset_impl(uint32_t u32Handle);

void *WifiQ_ApsRxEntrySet_impl(uint32_t u32Len);
void *WifiQ_ApsRxEntryGet_impl(void);
void WifiQ_ApsRxEntryGetDone_impl(uint32_t u32Num);
void WifiQ_ApsRxEntryReset_impl(void);

void WifiQ_DataEntryReset_impl(void);


#endif // _WIFI_QUEUE_H_
