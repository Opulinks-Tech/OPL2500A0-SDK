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

#ifndef _WPA_CLI_H_
#define _WPA_CLI_H_

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
#define WPA_CLI_MAX_ARGS 11

typedef int (*cmd_handler_fp_t)(int argc, char *argv[]);
typedef int (*user_cb)(void *ctx, void *buf, int buf_len);

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */
struct wpa_command {
    const char *cmd;                /**< Command String. */
    cmd_handler_fp_t cmd_handle;    /**< Command Handler. */
    const char *cmd_usage;          /**< Command Description. */
};

enum wpa_cli_cmd_flags {
    cli_cmd_flag_none      = 0x00,
    cli_cmd_flag_sensitive = 0x01
};

struct wpa_cli_cmd {
    const char *cmd;
    int (*handler)(void *ctrl, int argc, char *argv[]);
    char ** (*completion)(const char *str, int pos);
    enum wpa_cli_cmd_flags flags;
    const char *usage;
};

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
int wpa_cli_init(void (**cb)(void *ctx, char *cmd));
int wpa_cli_cmd_cb_get(int (**cb)(void *ctx, char *cmd));
int wpa_cli_register_user_connected_cb(user_cb cb);
int wpa_cli_register_user_disconnected_cb(user_cb cb);
int wpa_cli_register_user_scan_done_cb(user_cb cb);

#ifdef __cplusplus
}
#endif
#endif /* _WPA_CLI_H_ */
