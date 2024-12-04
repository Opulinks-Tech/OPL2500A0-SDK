/******************************************************************************
*  Copyright 2017 - 2023, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2023
******************************************************************************/
#ifndef _SYS_COMMON_CTRL_H_
#error "Please include sys_common_ctrl.h. Don't directly include sys_common_ctrl_patch.h"
#endif

#ifndef _SYS_COMMON_CTRL_PATCH_H_
#define _SYS_COMMON_CTRL_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

int sys_set_tca(uint8_t enable);
int sys_set_msq_clk(uint8_t u8ClkIdx, uint8_t u8Div);
int sys_set_dto(uint16_t u16para, uint16_t u16val);
int sys_set_ra(uint8_t u8Index, int8_t s8val);

void sys_common_ctrl_func_patch(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_COMMON_CTRL_PATCH_H_ */
