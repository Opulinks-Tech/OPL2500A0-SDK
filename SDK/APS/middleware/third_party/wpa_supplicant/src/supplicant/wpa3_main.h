/* *****************************************************************************
 *  Copyright 2017-2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file wpa3_main.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _WPA3_MAIN_H_
#define _WPA3_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif


/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
struct wpabuf * wpa3_build_sae_commit(struct wpa_supplicant *wpa_s, u8 *bssid);
struct wpabuf * wpa3_build_sae_confirm(struct wpa_supplicant *wpa_s);
int wpa3_parse_sae_commit(struct wpa_supplicant *wpa_s, uint8_t *buf, uint32_t len);
int wpa3_parse_sae_confirm(struct wpa_supplicant *wpa_s, uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif
#endif /* _WPA3_MAIN_H_ */
