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
*  mw_fim_default_group02.c
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
#include "ipc.h"
#include "mw_fim_default_group02.h"
#include "wifi_mac_common.h"
#include "wifi_mac_sta.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable

const uint8_t gMwFimDefaultwifiStaSkipDtim = 0;

const uint8_t gMwFimDefaultMacDataRate = 0xFF;

uint32_t gMwFimAddrWifiStaSkipDtim[MW_FIM_STA_SKIP_DTIM_NUM];


/* Default value of DHCP ARP check */
const uint8_t gMwFimDefaultDhcpArpChk = true;

/* DHCP ARP check */
uint32_t gMwFimAddrDhcpArpChk[MW_FIM_DHCP_ARP_CHK_NUM];

/* MAC TX data rate */
uint32_t gMwFimAddrMacDataRate[MW_FIM_MAC_TX_DATA_RATE_NUM];

/* AT TCPIP Transparent link data */
const at_nvm_trans_config_t gMwFimDefaultTransLinkData = {0};
uint32_t gMwFimAddrTransLinkData[MW_FIM_AT_TCPIP_TRANS_CFG_NUM];

/* AT BLE WiFi mode */
const uint8_t gMwFimDefaultAtBleWifi = 0;
uint32_t gMwFimAddrAtBleWifi[MW_FIM_AT_BLE_WIFI_NUM];

uint32_t gMwFimAddrStaCountryCode[MW_FIM_STA_COUNTRY_CODE_NUM];

uint32_t gMwFimAddrStaTxDbgEn[MW_FIM_STA_TX_DBG_EN_NUM];

/* Stored DHCP IP */
const uint8_t gMwFimDefaultDhcpIp = 0;
uint32_t gMwFimAddrStoredDhcpIp[MW_FIM_DHCP_IP_NUM];

// the information table of group 02
const T_MwFimFileInfo g_taMwFimGroupTable02[] =
{
    {MW_FIM_IDX_STA_SKIP_DTIM,            MW_FIM_STA_SKIP_DTIM_NUM,       MW_FIM_STA_SKIP_DTIM_SIZE,             (uint8_t*)&gMwFimDefaultwifiStaSkipDtim,   gMwFimAddrWifiStaSkipDtim},
    {MW_FIM_IDX_DHCP_ARP_CHK,             MW_FIM_DHCP_ARP_CHK_NUM,        MW_FIM_DHCP_ARP_CHK_SIZE,              (uint8_t*)&gMwFimDefaultDhcpArpChk,        gMwFimAddrDhcpArpChk},
    {MW_FIM_IDX_DHCP_IP,                  MW_FIM_DHCP_IP_NUM,             MW_FIM_DHCP_IP_SIZE,                   (uint8_t*)&gMwFimDefaultDhcpIp,            gMwFimAddrStoredDhcpIp},
    {MW_FIM_IDX_MAC_TX_DATA_RATE,         MW_FIM_MAC_TX_DATA_RATE_NUM,    MW_FIM_MAC_TX_DATA_RATE_SIZE,          (uint8_t*)&gMwFimDefaultMacDataRate,       gMwFimAddrMacDataRate},
    {MW_FIM_IDX_AT_TCPIP_TRANS_CFG,       MW_FIM_AT_TCPIP_TRANS_CFG_NUM,  MW_FIM_AT_TCPIP_TRANS_CFG_SIZE,        (uint8_t*)&gMwFimDefaultTransLinkData,     gMwFimAddrTransLinkData},
    {MW_FIM_IDX_AT_BLE_WIFI,              MW_FIM_AT_BLE_WIFI_NUM,         MW_FIM_AT_BLE_WIFI_SIZE,               (uint8_t*)&gMwFimDefaultAtBleWifi,         gMwFimAddrAtBleWifi},
    {MW_FIM_IDX_STA_COUNTRY_CODE,         MW_FIM_STA_COUNTRY_CODE_NUM,    MW_FIM_STA_COUNTRY_CODE_SIZE,          NULL,                                      gMwFimAddrStaCountryCode},
    {MW_FIM_IDX_STA_TX_DBG_EN,            MW_FIM_STA_TX_DBG_EN_NUM,       MW_FIM_STA_TX_DBG_EN_SIZE,             NULL,                                      gMwFimAddrStaTxDbgEn},

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
