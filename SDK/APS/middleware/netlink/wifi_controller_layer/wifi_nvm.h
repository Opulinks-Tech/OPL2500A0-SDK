/******************************************************************************
*  Copyright 2018, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlnik Communication Corp. (C) 2018
******************************************************************************/

#ifndef _WIFI_NVM_H_
#define _WIFI_NVM_H_

#include "mw_fim_default_group02.h"
#include "mw_fim_default_group03.h"

typedef enum {
    WIFI_NVM_ID_AUTO_CONNECT_MODE,
    WIFI_NVM_ID_AUTO_CONNECT_AP_NUM,
    WIFI_NVM_ID_AUTO_CONNECT_AP_CFG,
    WIFI_NVM_ID_AUTO_CONNECT_AP_INFO,
    
    WIFI_NVM_MAX_NUM,
} wifi_nvm_index_e;

typedef enum {
    WIFI_NVM_STA_INFO_ID_MAC_ADDR,
    /* 1 ~ 29 reserved */
    WIFI_NVM_STA_INFO_ID_SKIP_DTIM = 30,
    WIFI_NVM_MAC_TX_DATA_RATE,
    /* Device information for CBS use*/
    WIFI_NVM_STA_INFO_MANUFACTURE_NAME = 50,
    WIFI_NVM_STA_INFO_ALL,
    WIFI_NVM_STA_INFO_ID_MAX_NUM,
} wifi_nvm_sta_info_cfg_id_e;

typedef uint16_t (*wifi_nvm_sta_info_write_fp_t) (uint16_t id, uint16_t len, void *ptr);
typedef uint16_t (*wifi_nvm_sta_info_read_fp_t) (uint16_t id, uint16_t len, void *buf);
typedef int (*wifi_nvm_sta_def_mac_get_fp_t)(uint8_t* mac);

extern wifi_nvm_sta_info_write_fp_t wifi_nvm_sta_info_write;
extern wifi_nvm_sta_info_read_fp_t wifi_nvm_sta_info_read;
extern wifi_nvm_sta_def_mac_get_fp_t wifi_nvm_sta_def_mac_get;

uint16_t wifi_nvm_sta_info_write_impl(uint16_t id, uint16_t len, void *ptr);
uint16_t wifi_nvm_sta_info_read_impl(uint16_t id, uint16_t len, void *buf);
int wifi_nvm_sta_def_mac_get_impl(uint8_t *mac);


#include "wifi_nvm_patch.h"

#endif /* _WIFI_NVM_H_ */


