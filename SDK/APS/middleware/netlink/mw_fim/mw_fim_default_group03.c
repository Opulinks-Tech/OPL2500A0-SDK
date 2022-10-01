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
*  mw_fim_default_group03.c
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
#include "mw_fim_default_group03.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
// the default value and address buffer of mac address source
const uint8_t g_tMwFimDefaultMacAddrWifiSTASrc    = BASE_NVM_MAC_SRC_TYPE_ID_OTP;
RET_DATA uint32_t g_MwFimAddrBufferMacAddrWifiSTASrc[MW_FIM_MAC_ADDR_SRC_WIFI_STA_NUM];

const uint8_t g_tMwFimDefaultMacAddrWifiSoftAPSrc = BASE_NVM_MAC_SRC_TYPE_ID_OTP;
RET_DATA uint32_t g_MwFimAddrBufferMacAddrWifiAPSrc[MW_FIM_MAC_ADDR_SRC_WIFI_SOFTAP_NUM];

const uint8_t g_tMwFimDefaultMacAddrBleSrc        = BASE_NVM_MAC_SRC_TYPE_ID_OTP;
RET_DATA uint32_t g_MwFimAddrBufferMacAddrBleSrc[MW_FIM_MAC_ADDR_SRC_WIFI_BLE_NUM];

/* Default value of Wifi Sta configuration */
// please refer to WifiSta_StaInfoInit()
const uint8_t gMwFimDefaultWifiStaMacAddr[6] = {  //[0000526]
    0x22, 0x33, 0x44, 0x55, 0x66, 0x76
};
/* WIFI STA Information configuration */
RET_DATA uint32_t gMwFimAddrWifiStaMacAddr[MW_FIM_STA_MAC_ADDR_NUM];  //[0000526]

const uint8_t gMwFimDefaultManufName [STA_INFO_MAX_MANUF_NAME_SIZE] = {
     /* C.B.S XXX, encoding GB2312 */
     0x43, 0x2E, 0x42, 0x2E, 0x53, 0x20, 
     0xB4, 0xB4,
     0xB2, 0xA9,
     0xCA, 0xC0,
};
/* For blewifi CBS store use */
RET_DATA uint32_t gMwFimAddrManufName[MW_FIM_DEVICE_MANUF_NAME_NUM];

const le_cfg_t g_tMwFimDefaultLeCfg = 
{
    .hci_revision = FIM_HCI_Version,
    .manufacturer_name = FIM_Manufacturer_Name, 
    .lmp_pal_subversion = FIM_LMP_PAL_Subversion, 
    .hci_version = FIM_HCI_Version,
    .lmp_pal_version = FIM_LMP_PAL_Version,
    .bd_addr = {0x66, 0x55, 0x44, 0x33, 0x22, 0x11}
};
// the address buffer of LE config
RET_DATA uint32_t g_u32aMwFimAddrLeCfg[MW_FIM_LE_CFG_NUM]; 

// the settings of system mode
const T_MwFim_SysMode g_tMwFimDefaultSysMode =
{
    MW_FIM_SYS_MODE_INIT,   // uint8_t ubSysMode;
    0xFF, 0xFF, 0xFF        // uint8_t ubaReserved[3];
};
// the address buffer of system mode
RET_DATA uint32_t g_ulaMwFimAddrBufferSysMode[MW_FIM_SYS_MODE_NUM];

// the information table of group 03
const T_MwFimFileInfo g_taMwFimGroupTable03[] =
{
    {MW_FIM_IDX_GP03_MAC_ADDR_WIFI_STA_SRC, MW_FIM_MAC_ADDR_SRC_WIFI_STA_NUM, MW_FIM_MAC_ADDR_SRC_WIFI_STA_SIZE, (uint8_t*)&g_tMwFimDefaultMacAddrWifiSTASrc, g_MwFimAddrBufferMacAddrWifiSTASrc},
    {MW_FIM_IDX_GP03_MAC_ADDR_WIFI_SOFTAP_SRC, MW_FIM_MAC_ADDR_SRC_WIFI_SOFTAP_NUM, MW_FIM_MAC_ADDR_SRC_WIFI_SOFT_AP_SIZE, (uint8_t*)&g_tMwFimDefaultMacAddrWifiSoftAPSrc, g_MwFimAddrBufferMacAddrWifiAPSrc},
    {MW_FIM_IDX_GP03_MAC_ADDR_BLE_SRC,  MW_FIM_MAC_ADDR_SRC_WIFI_BLE_NUM, MW_FIM_MAC_ADDR_SRC_WIFI_BLE_SIZE, (uint8_t*)&g_tMwFimDefaultMacAddrBleSrc, g_MwFimAddrBufferMacAddrBleSrc},

    {MW_FIM_IDX_GP03_STA_MAC_ADDR,      MW_FIM_STA_MAC_ADDR_NUM,        MW_FIM_STA_MAC_ADDR_SIZE,              (uint8_t*)&gMwFimDefaultWifiStaMacAddr,    gMwFimAddrWifiStaMacAddr}, /* [0000526] */
    {MW_FIM_IDX_GP03_DEVICE_MANUF_NAME, MW_FIM_DEVICE_MANUF_NAME_NUM,   MW_FIM_DEVICE_MANUF_NAME_SIZE,         (uint8_t*)&gMwFimDefaultManufName,         gMwFimAddrManufName},

    {MW_FIM_IDX_GP03_LE_CFG,            MW_FIM_LE_CFG_NUM, MW_FIM_IDX_LE_CFG_SIZE, (uint8_t*)&g_tMwFimDefaultLeCfg, g_u32aMwFimAddrLeCfg},
    {MW_FIM_IDX_GP03_SYS_MODE,          MW_FIM_SYS_MODE_NUM,        MW_FIM_SYS_MODE_SIZE,        (uint8_t*)&g_tMwFimDefaultSysMode,   g_ulaMwFimAddrBufferSysMode},

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
