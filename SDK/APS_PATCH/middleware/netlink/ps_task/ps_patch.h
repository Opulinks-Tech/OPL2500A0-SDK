/* *****************************************************************************
 *  Copyright 2017 - 2022, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2022
 *
 *******************************************************************************
 *
 *  @file ps_patch.h 
 * 
 *  @brief PS module patch
 *  
 *******************************************************************************/
#ifndef __PS_H__
#error "Please include ps.h. Don't directly include ps_patch.h"
#endif

#ifndef _PS_PATCH_H_
#define _PS_PATCH_H_

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
void ps_update_xtal32_tick_value(double tick);
void ps_patch_init(void);
void ps_init_patch(void);
void sleep_warm_init_patch(void);
void ps_task_create_patch(void);
void ps_task_delete_patch(void);
void ps_psp_wkup_clk_ready(void);
void ps_psp_enable(uint32_t enable);

#ifdef __cplusplus
}
#endif
#endif  /* _PS_PATCH_H_ */
