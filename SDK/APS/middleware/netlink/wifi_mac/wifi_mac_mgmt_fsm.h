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

#ifndef __WIFI_MAC_MGMT_FSM_H
#define __WIFI_MAC_MGMT_FSM_H

extern u8 own_mac_addr[MAC_ADDR_LEN];
extern u8 bss_addr[MAC_ADDR_LEN];


void wifi_mac_bss_info_dbg(void);

#endif // __WIFI_MAC_MGMT_FSM_H
