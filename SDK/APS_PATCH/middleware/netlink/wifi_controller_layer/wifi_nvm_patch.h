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

#ifndef _WIFI_NVM_PATCH_H_
#define _WIFI_NVM_PATCH_H_

#ifndef _WIFI_NVM_H_
#error "Please include wifi_nvm.h. Don't directly include wifi_nvm_patch.h"
#endif

uint16_t wifi_nvm_auto_connect_read(uint16_t id, uint16_t len, uint8_t idx, void *buf);
uint16_t wifi_nvm_auto_connect_write(uint16_t id, uint16_t len, uint8_t idx, void *buf);

#endif /* _WIFI_NVM_PATCH_H_ */


