/******************************************************************************
*  Copyright 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

#include <string.h>

#include "console.h"
#include "common_cli.h"
#include "cmd_parser.h"
#include "opulinks_log.h"
#include "wifi_cli.h"

#include "wts_dut.h"

static const char *TAG="[WTS][CMD]:";

int dbg_mode = 1;

static int commands_help(int argc, char** argv)
{
    LOGI_DRCT(TAG, "WTS DUT");
    LOGI_DRCT(TAG, "  wfa_dut [interface] [local port]      Enter WFA processing.");
    return 0;
}

static int cmd_buf_to_arg(int *argc, char **argv, char *pbuf)
{
    int count = 0;
    char *p = strtok(pbuf, " ");
    
    argv[count] = p;
    count++;
    
    while ((p = strtok(NULL, " ")) != NULL) {
        argv[count] = p;
        count++;
    }
    
    *argc = count;
    return 0;
}

int cmd_parser_func(char *pbuf, int len)
{
    char *argv[10] = {0};
    char *tmp[32] = {0};
    int argc = 0;
    
    memcpy(&tmp[0], pbuf, len);
    cmd_buf_to_arg(&argc, &argv[0], pbuf);
    
    tracer_drct_printf("\n");
    if (!strcmp(argv[0], "wfa_dut")) {
        wts_dut_task_send_msg(WTS_DUT_INIT_CMD, len, tmp);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "scan")) {
        wifi_cmd_scan(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "connect")) {
        wifi_cmd_sta(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "disconnect")) {
        wifi_cmd_disconnect(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "query")) {
        wifi_cmd_query(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "help")) {
        commands_help(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "ping")) {
        common_cmd_ping(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "reset")) {
        common_cmd_reset(argc, argv);
        return CMD_FINISHED;
    }
    else if (!strcmp(argv[0], "version")) {
        common_cmd_version(argc, argv);
        return CMD_FINISHED;
    }
    else if(!strcmp(argv[0], "dbg")) {
        LOGI_DRCT(TAG, "Enter debug mode");
        dbg_mode = 1;
        return CMD_FINISHED;
    }
    else {
        if (dbg_mode == 1) {
            memcpy(pbuf, &tmp[0], len);
        }
    }
    
    return CMD_CONTINUE;
}
