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

#ifndef _WIFI_MAC_FRAME_H_
#error "Please include wifi_mac_frame.h. Don't directly include wifi_mac_frame_patch.h"
#endif

#ifndef _WIFI_MAC_FRAME_PATCH_H_
#define _WIFI_MAC_FRAME_PATCH_H_

void wifi_sta_create_data_mac_header_patch(UINT8 *header, UINT8 u8TID, UINT16 encrypt, const UINT8* eth_dest);

void wifi_mac_frm_init_patch(void);

#endif /* _WIFI_MAC_FRAME_PATCH_H_ */

