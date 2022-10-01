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
 *  @file wpa2_main.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _WPA2_ENT_MAIN_H_
#define _WPA2_ENT_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "utils/includes.h"

#include "utils/common.h"
#include "eap_peer/eap_i.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define WPA2_ENT_MAX_LEN_ID        128
#define WPA2_ENT_MAX_LEN_USER_NAME 128
#define WPA2_ENT_MAX_LEN_USER_PWD  128 //TBD

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
typedef enum {
    EAP_TTLS_PHASE2_EAP,
    EAP_TTLS_PHASE2_MSCHAPV2,
    EAP_TTLS_PHASE2_MSCHAP,
    EAP_TTLS_PHASE2_PAP,
    EAP_TTLS_PHASE_CHAP
} wpa2_ent_eap_ttls_phase2_types_t;

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
int wpa2_ent_is_init(struct eap_sm *sm);
int wpa2_ent_init(struct eap_sm **sm);
int wpa2_ent_deinit(struct eap_sm **sm);
int wpa2_ent_set_identity(const uint8_t *identity, int len);
int wpa2_ent_clear_identity(void);
int wpa2_ent_set_username(const uint8_t *username, int len);
int wpa2_ent_get_username(uint8_t *username, int len);
int wpa2_ent_clear_username(void);
int wpa2_ent_set_user_password(const uint8_t *password, int len);
int wpa2_ent_get_user_password(uint8_t *password, int len);
int wpa2_ent_set_cert_key(const uint8_t *client_cert, int client_cert_len, const uint8_t *private_key, int private_key_len, 
                          const uint8_t *private_key_passwd, int private_key_passwd_len);
int wpa2_ent_clear_cert_key(void);
int wpa2_ent_set_ca_cert(const uint8_t *ca_cert, int ca_cert_len);
int wpa2_ent_clear_ca_cert(void);
int wpa2_ent_clear_password(void);
int wpa2_ent_set_ttls_phase2_method(wpa2_ent_eap_ttls_phase2_types_t type);
int wpa2_ent_eap_sm_rx_eapol(struct eap_sm *sm, const uint8_t *src_addr, const uint8_t *buf, uint32_t len);
int wpa2_ent_eap_start_eapol(struct eap_sm *sm);

#ifdef __cplusplus
}
#endif
#endif /* _WPA2_ENT_MAIN_H_ */
