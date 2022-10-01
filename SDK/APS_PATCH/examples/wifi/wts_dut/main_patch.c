/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/
#include <stdio.h>

#include "sys_init.h"
#include "hal_patch.h"

#include "wts_dut.h"

static void Main_AppInit_patch(void);

void __Patch_EntryPoint(void) __attribute__((section("ENTRY_POINT")));
void __Patch_EntryPoint(void)
{
    /* Don't remove SysInit_EntryPoint */
    SysInit_EntryPoint();

    // apply the sw patch from here
    Sys_AppInit = Main_AppInit_patch;
}

/*************************************************************************
* FUNCTION:
*   Main_AppInit_patch
*
* DESCRIPTION:
*   the initial of application
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
static void Main_AppInit_patch(void)
{
    wts_dut_main_init();
}
