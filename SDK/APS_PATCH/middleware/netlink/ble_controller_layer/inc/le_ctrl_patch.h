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
 *  @file le_ctrl_patch.h
 *
 *  @brief
 *
 ******************************************************************************/

#ifndef _LE_CTRL_PATCH_H_
#define _LE_CTRL_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "le_ctrl.h"


/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/* For instrument to directly tx/rx HCI commands/events via uart. */
// #define DEFAULT_UART_HCI_MODE


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
// le_ctrl_func_map
void le_ctrl_pre_patch_init(void);

// le_ctrl_cmd
void le_ctrl_throughput_test_patch(uint8_t tx_octets, uint8_t duration);
void vendor_specific_throughput_test_command_patch(hci_command_packet_01 *cmd_packet);

// le_ctrl_data

// le_ctrl_enc

// le_ctrl_fim

// le_ctrl_hci
void le_ctrl_init_patch(void);

// le_ctrl_hci

// le_ctrl_hci_handler

// le_ctrl_hci_uart_tl


#ifdef __cplusplus
}
#endif
#endif  /* _LE_CTRL_PATCH_H_ */
