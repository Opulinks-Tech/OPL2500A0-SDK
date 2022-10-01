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

#ifndef __DIAG_TASK_H__
#define __DIAG_TASK_H__

/* Scheduler includes. */
#include "cmsis_os.h"

#define DIAG_UART_EVENT             (0x0001)
#define DIAG_TEST_EVENT             (0x0002)

#define DIAG_QUEUE_SIZE             (5)
#define RBUF_SIZE                   128

/* For user command response.
 * When user command found and finished, return CMD_FINISHED to stop searching diag_task commands.
 * Or return CMD_CONTINUE to go on searching diag_task commands and run. */
typedef enum
{
    CMD_FINISHED=0,
    CMD_CONTINUE=1,
}E_CLI_CMD_PROC;

typedef struct
{
    unsigned char in;                                // Next In Index
    unsigned char out;                               // Next Out Index
    char buf[RBUF_SIZE];                             // Buffer
} uart_buffer_t;

typedef struct
{
	unsigned int event;
	unsigned int length;
	char *pcMessage;
} xDiagMessage;

/* ----------------------------------------------------------------------- */

extern osMessageQId xDiagQueue;
extern osThreadId DiagTaskHandle;

void uartdbg_rx_int_handler(uint32_t u32Data);


/**
 * @brief User CLI command in DbgUart.
 *        OPL2500 DbgUart command parser is after this function
 * @param pbuf [in] The input buffer pointer.
 * @param len [in] The length of input buffer.
 * @return Command searching status.
 * @retval CMD_FINISHED - Search command success. CLI command finished here.
 * @retval CMD_CONTINUE - Search command fail. After this function, it will run OPL2500 CLI commands.
 */
typedef E_CLI_CMD_PROC(*T_Cli_UserCmdProcess_fp)(char *pbuf, int len);
typedef void (*Diag_UserCmdSet_fp_t)(T_Cli_UserCmdProcess_fp ptUserCmdFunc);
typedef void (*diag_wpa_cli_cb_fp_t)(void *ctx, char *cmd);
typedef void (*diag_wpa_cli_cb_set_fp_t)(diag_wpa_cli_cb_fp_t cb);
typedef void (*diag_task_create_fp_t)(void);
typedef osStatus (*diag_task_send_fp_t)(xDiagMessage txMsg);
typedef void (*diag_task_fp_t)( void *pvParameters );
typedef void (*CmdProcess_fp_t)(char *pbuf, int len);
typedef void (*ParseWPACommand_fp_t)(char* pszData);
typedef void (*ParseLWIPCommand_fp_t)(char* pszData);
typedef void (*ParsePINGCommand_fp_t)(char* pszData);
typedef void (*ParseWIFICommand_fp_t)(char* pszData);
#ifdef __PMP_ENABLE__
typedef void (*ParsePmpCommand_fp_t)(char* pszData);
#endif // #__PMP_ENABLE__
//#ifdef ENHANCE_IPC
typedef void (*ParseIpcCommand_fp_t)(char *sCmd);
//#endif // #ENHANCE_IPC
typedef void (*ParseTracerCommand_fp_t)(char *sCmd);
typedef void (*ParseSYSCommand_fp_t)(char* pszData);
typedef void (*ParseDownloadCommand_fp_t)(char *sCmd);
typedef void (*ParseAtVersionCommand_fp_t)(char *sCmd);
typedef void (*ParseSimpleAtCommand_fp_t)(char *sCmd);
typedef void (*ParseLeCfgCommand_fp_t)(char *sCmd);
typedef void (*ParseLeTestCommand_fp_t)(char *sCmd);
typedef void (*ParseUnknownCommand_fp_t)(char *sCmd);
typedef void (*ParseSwitchAT_DBGCommand_fp_t)(char *sCmd);
typedef void (*ParseRegCtrlCommand_fp_t)(char *sCmd);
typedef void (*Diag_ShowRevision_fp_t)(char *sCmd);
typedef void (*ParseMsqFimLoadTableCommand_fp_t)(char *sCmd);
typedef void (*ParseMsqFimCommand_fp_t)(char *sCmd);
typedef uint32_t (*ParseParam_fp_t)(char *sCmd, char **ppbParam, uint32_t dwNum);
typedef int  (*wifi_cli_handler_fp_t)(int argc, char *argv[]);

extern diag_task_create_fp_t diag_task_create;
extern diag_task_send_fp_t diag_task_send;
extern diag_task_fp_t diag_task;
extern CmdProcess_fp_t CmdProcess;
extern ParseWPACommand_fp_t ParseWPACommand;
extern ParseLWIPCommand_fp_t ParseLWIPCommand;
extern ParsePINGCommand_fp_t ParsePINGCommand;
extern ParseWIFICommand_fp_t ParseWIFICommand;
#ifdef __PMP_ENABLE__
extern ParsePmpCommand_fp_t ParsePmpCommand;
#endif // #__PMP_ENABLE__
//#ifdef ENHANCE_IPC
extern ParseIpcCommand_fp_t ParseIpcCommand;
//#endif // #ENHANCE_IPC
extern ParseTracerCommand_fp_t ParseTracerCommand;
extern ParseSYSCommand_fp_t ParseSYSCommand;
extern ParseDownloadCommand_fp_t ParseDownloadCommand;
extern ParseAtVersionCommand_fp_t ParseAtVersionCommand;
extern ParseSimpleAtCommand_fp_t ParseSimpleAtCommand;
extern ParseLeCfgCommand_fp_t ParseLeCfgCommand;
extern ParseLeTestCommand_fp_t ParseLeTestCommand;
extern ParseUnknownCommand_fp_t ParseUnknownCommand;
extern ParseSwitchAT_DBGCommand_fp_t ParseSwitchAT_DBGCommand;
extern ParseRegCtrlCommand_fp_t ParseRegCtrlCommand;
extern Diag_ShowRevision_fp_t Diag_ShowRevision;
extern ParseMsqFimCommand_fp_t ParseMsqFimCommand;
extern ParseMsqFimLoadTableCommand_fp_t ParseMsqFimLoadTableCommand;

extern ParseParam_fp_t ParseParam;
extern Diag_UserCmdSet_fp_t Diag_UserCmdSet;
extern diag_wpa_cli_cb_set_fp_t diag_wpa_cli_cb_set;
extern wifi_cli_handler_fp_t wifi_cli_handler;

/*
   Interface Initialization: DIAG TASK
 */
void Diag_UserCmdSet_impl(T_Cli_UserCmdProcess_fp ptUserCmdFunc);
void Diag_ShowRevision_impl(char *sCmd);
void diag_wpa_cli_cb_set_impl(diag_wpa_cli_cb_fp_t cb);

#include "diag_task_patch.h"
#endif
