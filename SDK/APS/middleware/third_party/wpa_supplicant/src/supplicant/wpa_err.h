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
 *  @file wpa_err.h
 *
 *  @brief `
 *
 ******************************************************************************/

#ifndef _WPA_ERR_H_
#define _WPA_ERR_H_

#define WPA_ERR_OK                         0
#define WPA_ERR_FAIL                      -1
#define WPA_ERR_INVALID_PARAMETER         -2
#define WPA_ERR_ALREADY_EXISTS            -3
#define WPA_ERR_NOT_EXISTS                -4
#define WPA_ERR_INVALID_BSSID             -5
#define WPA_ERR_INVALID_SSID              -6
#define WPA_ERR_INVALID_PASSWORD          -7
#define WPA_ERR_INVALID_FORMAT            -8
#define WPA_ERR_MALLOC_FAIL               -9

#define WPA_ERR_STA_AP_NOT_FOUND         -10
#define WPA_ERR_STA_AUTH_TIMEOUT         -11
#define WPA_ERR_STA_ASSOC_TIMEOUT        -12
#define WPA_ERR_STA_WRONG_BSSID          -13
#define WPA_ERR_STA_AUTH_ST_FAIL         -14
#define WPA_ERR_STA_AUTH_INVALID_ALG     -15
#define WPA_ERR_STA_ASSOC_ST_FAIL        -16
#define WPA_ERR_STA_ASSOC_NOT_SUPP_RATES -17
#define WPA_ERR_STA_INVALID_FRAME_TYPE   -18

#define WPA_ERR_CMD_ENQUEUE_FAIL         -20
#define WPA_ERR_CMD_TIMEOUT              -21
#define WPA_ERR_CMD_CANCELED             -22
#define WPA_ERR_CMD_OVERFLOW             -23
#define WPA_ERR_CMD_PROCESSING           -24
#define WPA_ERR_CMD_EVT_NOT_MATCH        -25
#define WPA_ERR_CMD_NOT_FOUND            -26
#define WPA_ERR_CMD_MULTI_MATCH          -27

#define WPA_ERR_STA_TASK_NOT_INIT        -30
#define WPA_ERR_STA_TASK_INIT_DONE       -31

#define WPA_ERR_BSS_NOT_EXISTS           -40

#define WPA_ERR_EAP_CONFIG_FAIL          -50
#define WPA_ERR_EAP_BLOB_FAIL            -51
#define WPA_ERR_EAP_TLS_FAIL             -52
#define WPA_ERR_EAP_REG_FAIL             -53
#define WPA_ERR_EAP_RECV_4_TIMES         -54

#define WPA_ERR_BUF_OUT_OF_SIZE          -60
#define WPA_ERR_BUF_PARSE_FAIL           -61
#define WPA_ERR_BUF_SEND_FAIL            -62

#define WPA_ERR_MGMT_INVALID_SEQ         -70
#define WPA_ERR_MGMT_INVALID_LEN         -71
#define WPA_ERR_MGMT_UNKNOWN_STYPE       -72

#define WPA_ERR_KEY_MGMT_INVALID         -80
#define WPA_ERR_KEY_MGMT_NOT_SUPP        -81

#define WPA_ERR_PMKSA_OK                 -90

#endif /* _WPA_ERR_H_ */

