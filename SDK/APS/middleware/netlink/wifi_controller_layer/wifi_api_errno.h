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

#ifndef _WIFI_API_ERRNO_H_
#define _WIFI_API_ERRNO_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                    Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
#define WAPI_ERR_OK                 0
#define WAPI_ERR_FAIL              -1
#define WAPI_ERR_PARAM_INVALID     -2
#define WAPI_ERR_AP_NOT_EXISTS     -3
#define WAPI_ERR_NO_MEMORY         -4

#define WAPI_ERR_ST_IN_SCANNING    -10
#define WAPI_ERR_ST_IN_CONNECTING  -11
#define WAPI_ERR_ST_IN_CONNECTED   -12

#define WAPI_ERR_BSSID_INVALID     -20
#define WAPI_ERR_SSID_INVALID      -21
#define WAPI_ERR_PASSWORD_INVALID  -22
#define WAPI_ERR_CH_INVALID        -24
#define WAPI_ERR_SCAN_TYPE_INVALID -25
#define WAPI_ERR_SCAN_TIME_INVALID -26
#define WAPI_ERR_SORT_INVALID      -27

#define WAPI_ERR_SENT_TO_USER_FAIL -30

#define WAPI_ERR_AUTO_MODE_ERR     -40
#define WAPI_ERR_AUTO_EMPTY        -41
#define WAPI_ERR_AUTO_NO_ENABLE    -42
#define WAPI_ERR_AUTO_NOT_IN_USE   -43
#define WAPI_ERR_AUTO_OUT_OF_INDEX -44
#define WAPI_ERR_AUTO_RETRY_END    -45
#define WAPI_ERR_AUTO_FIM_ERR      -46

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _WIFI_API_ERRNO_H_ */
