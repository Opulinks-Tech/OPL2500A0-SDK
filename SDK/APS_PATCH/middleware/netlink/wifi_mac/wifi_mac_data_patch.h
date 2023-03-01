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

#ifndef _WIFI_MAC_DATA_H_
#error "Please include wifi_mac_data.h. Don't directly include wifi_mac_data_patch.h"
#endif

#ifndef _WIFI_MAC_DATA_PATCH_H_
#define _WIFI_MAC_DATA_PATCH_H_

#if (REFINE_RX_PROC == 2)
#include "lwip/pbuf.h"

extern volatile uint8_t g_u8RxFrameHdrLen;
#endif

void wifi_mac_rx_data_init_patch(void);
void wifi_mac_tx_data_init_patch(void);


#endif /* _WIFI_MAC_DATA_PATCH_H_ */
