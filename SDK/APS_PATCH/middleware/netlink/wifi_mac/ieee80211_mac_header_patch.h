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

#ifndef _IEEE80211_MAC_HEADER_H_
#error "Please include ieee80211_mac_header.h. Don't directly include ieee80211_mac_header_patch.h"
#endif

#ifndef _IEEE80211_MAC_HEADER_PATCH_H_
#define _IEEE80211_MAC_HEADER_PATCH_H_

#pragma anon_unions

typedef struct
{
    union
    {
        struct
        {
            UINT32  u32VHT:1;       // Value must be 1
            UINT32  u32HE:1;        // Value must be 1
            UINT32  u32CtrlID:4;
            UINT32  u32Reserved:26;
        } __PACKED;
        struct uph_ctrl     // Control ID is 4
        {
            UINT32  u32VHT:1;       // Value must be 1
            UINT32  u32HE:1;        // Value must be 1
            UINT32  u32CtrlID:4;
            UINT32  u32UlPwrHeadroom:5;
            UINT32  u32MinTransitPwrFlg:1;
            UINT32  u32Rsrvd:20;
        } __PACKED sUphCtrl;
        struct ones         // Control ID is 15
        {
            UINT32  u32VHT:1;       // Value must be 1
            UINT32  u32HE:1;        // Value must be 1
            UINT32  u32CtrlID:4;
            UINT32  u32AllOne:26;           // Set to all 1s
        } __PACKED sOnes;
    } __PACKED;
} __PACKED S_WLAN_MHDR_HT_CTRL_HE;
#define WLAN_MHDR_HT_CTRL_HE_LEN        sizeof(S_WLAN_MHDR_HT_CTRL_HE)

#define WLAN_MPDU_LEN_HT_MAX_3839       3839


#endif /* _IEEE80211_MAC_HEADER_PATCH_H_ */
