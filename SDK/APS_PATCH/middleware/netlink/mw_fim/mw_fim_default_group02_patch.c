/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  mw_fim_default_group02_patch.c
*
*  Project:
*  --------
*  OPL2500 Project - the Flash Item Management (FIM) implement file
*
*  Description:
*  ------------
*  This implement file is include the Flash Item Management (FIM) function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
//#include "ipc.h"
#include "wifi_mac_common.h"
#include "mw_fim_default_group02_patch.h"
#include "mw_fim_default.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable

extern const uint8_t gMwFimDefaultwifiStaSkipDtim;

extern const uint8_t gMwFimDefaultMacDataRate;

extern uint32_t gMwFimAddrWifiStaSkipDtim[MW_FIM_STA_SKIP_DTIM_NUM];


/* Default value of DHCP ARP check */
extern const uint8_t gMwFimDefaultDhcpArpChk;

/* DHCP ARP check */
extern uint32_t gMwFimAddrDhcpArpChk[MW_FIM_DHCP_ARP_CHK_NUM];

/* MAC TX data rate */
extern uint32_t gMwFimAddrMacDataRate[MW_FIM_MAC_TX_DATA_RATE_NUM];

/* AT TCPIP Transparent link data */
extern const at_nvm_trans_config_t gMwFimDefaultTransLinkData;
extern uint32_t gMwFimAddrTransLinkData[MW_FIM_AT_TCPIP_TRANS_CFG_NUM];

/* AT BLE WiFi mode */
extern const uint8_t gMwFimDefaultAtBleWifi;
extern uint32_t gMwFimAddrAtBleWifi[MW_FIM_AT_BLE_WIFI_NUM];

extern uint32_t gMwFimAddrStaCountryCode[MW_FIM_STA_COUNTRY_CODE_NUM];

extern uint32_t gMwFimAddrStaTxDbgEn[MW_FIM_STA_TX_DBG_EN_NUM];

/* Stored DHCP IP */
extern const uint8_t gMwFimDefaultDhcpIp;
extern uint32_t gMwFimAddrStoredDhcpIp[MW_FIM_DHCP_IP_NUM];


/* Default value of Auto Connect Mode */
const uint32_t gMwFimDefaultAutoConnectMode = true;

/* Default value of Auto Connect AP number */
const uint32_t gMwFimDefaultAutoConnectAPNum = 0;

const mw_fim_wifi_auto_connect_ap_info_t gMwFimDefaultAutoConnectAPInfo = {
    .in_used = 0,
    .pw = {0},
    .ssid = {0},
    .bssid = {0},
    .channel = 0,
    .psk = {0},
    .psk_len = 0,
};

const mw_fim_wifi_auto_connect_cfg_t gMwFimDefaultAutoConnectCfg = {
    .curr_ap_idx = 0,
    .max_save_num = WAPI_MAX_AUTO_CONNECT_STORE_NUM,
};


/* Auto Connection Mode */
// the address buffer of Auto Connection Mode
uint32_t gMwFimAddrAutoConnectMode[MW_FIM_AUTO_CONN_MODE_NUM];

/* Auto Connection AP number */
uint32_t gMwFimAddrAutoConnectAPNum[MW_FIM_AUTO_CONN_AP_NUM];

/* Auto Connection Configuration */
uint32_t gMwFimAddrAutoConnectCfg[MW_FIM_WIFI_AUTO_CONN_CFG_NUM];

/* Auto Connection Information */
uint32_t gMwFimAddrBufferFastConnectApInfo[MW_FIM_AUTO_CONN_INFO_NUM];



// the information table of group 02
const T_MwFimFileInfo g_taMwFimGroupTable02_patch[] =
{
    {MW_FIM_IDX_STA_SKIP_DTIM,            MW_FIM_STA_SKIP_DTIM_NUM,       MW_FIM_STA_SKIP_DTIM_SIZE,             (uint8_t*)&gMwFimDefaultwifiStaSkipDtim,   gMwFimAddrWifiStaSkipDtim},
    {MW_FIM_IDX_DHCP_ARP_CHK,             MW_FIM_DHCP_ARP_CHK_NUM,        MW_FIM_DHCP_ARP_CHK_SIZE,              (uint8_t*)&gMwFimDefaultDhcpArpChk,        gMwFimAddrDhcpArpChk},
    {MW_FIM_IDX_DHCP_IP,                  MW_FIM_DHCP_IP_NUM,             MW_FIM_DHCP_IP_SIZE,                   (uint8_t*)&gMwFimDefaultDhcpIp,            gMwFimAddrStoredDhcpIp},
    {MW_FIM_IDX_MAC_TX_DATA_RATE,         MW_FIM_MAC_TX_DATA_RATE_NUM,    MW_FIM_MAC_TX_DATA_RATE_SIZE,          (uint8_t*)&gMwFimDefaultMacDataRate,       gMwFimAddrMacDataRate},
    {MW_FIM_IDX_AT_TCPIP_TRANS_CFG,       MW_FIM_AT_TCPIP_TRANS_CFG_NUM,  MW_FIM_AT_TCPIP_TRANS_CFG_SIZE,        (uint8_t*)&gMwFimDefaultTransLinkData,     gMwFimAddrTransLinkData},
    {MW_FIM_IDX_AT_BLE_WIFI,              MW_FIM_AT_BLE_WIFI_NUM,         MW_FIM_AT_BLE_WIFI_SIZE,               (uint8_t*)&gMwFimDefaultAtBleWifi,         gMwFimAddrAtBleWifi},
    {MW_FIM_IDX_STA_COUNTRY_CODE,         MW_FIM_STA_COUNTRY_CODE_NUM,    MW_FIM_STA_COUNTRY_CODE_SIZE,          NULL,                                      gMwFimAddrStaCountryCode},
    {MW_FIM_IDX_STA_TX_DBG_EN,            MW_FIM_STA_TX_DBG_EN_NUM,       MW_FIM_STA_TX_DBG_EN_SIZE,             NULL,                                      gMwFimAddrStaTxDbgEn},
    {MW_FIM_IDX_WIFI_AUTO_CONN_MODE,      MW_FIM_AUTO_CONN_MODE_NUM,      MW_FIM_AUTO_COMM_MODE_SIZE,            (uint8_t*)&gMwFimDefaultAutoConnectMode,   gMwFimAddrAutoConnectMode},
    {MW_FIM_IDX_WIFI_AUTO_CONN_CFG,       MW_FIM_WIFI_AUTO_CONN_CFG_NUM,  MW_FIM_WIFI_AUTO_CONN_CFG_SIZE,        (uint8_t*)&gMwFimDefaultAutoConnectCfg,    gMwFimAddrAutoConnectCfg},
    {MW_FIM_IDX_WIFI_AUTO_CONN_AP_NUM,    MW_FIM_AUTO_CONN_AP_NUM,        MW_FIM_AUTO_COMM_AP_SIZE,              (uint8_t*)&gMwFimDefaultAutoConnectAPNum,  gMwFimAddrAutoConnectAPNum},
    {MW_FIM_IDX_WIFI_AUTO_CONN_AP_INFO,   MW_FIM_AUTO_CONN_INFO_NUM,      MW_FIM_AUTO_CONN_INFO_SIZE,            (uint8_t*)&gMwFimDefaultAutoConnectAPInfo, gMwFimAddrBufferFastConnectApInfo},
    // the end, don't modify and remove it
    {0xFFFFFFFF,            0x00,              0x00,               NULL,                            NULL}
};


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
void MwFim_Group02_patch( void )
{
    MwFim_GroupInfoUpdate(0, 2, (T_MwFimFileInfo *)g_taMwFimGroupTable02_patch);
    MwFim_GroupVersionUpdate(0, 2, MW_FIM_VER02+2);
}

