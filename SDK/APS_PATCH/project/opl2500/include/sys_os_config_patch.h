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

/***********************
Head Block of The File
***********************/
#ifndef _SYS_OS_CONFIG_H_
#error "Please include sys_os_config.h. Don't directly include sys_os_config_patch.h"
#endif

#ifndef _SYS_OS_CONFIG_PATCH_H_
#define _SYS_OS_CONFIG_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define OS_TASK_STACK_SIZE_REFINED


#ifdef OS_TASK_STACK_SIZE_REFINED

    // Application Tasks
    #define OS_TASK_STACK_SIZE_BLEWIFI_CTRL         (400)
    #define OS_TASK_STACK_SIZE_BLEWIFI_USER_APP     (248)
    #define OS_TASK_STACK_SIZE_SENSOR               (180)
    #define OS_TASK_STACK_SIZE_SENSOR_DATA          (920)

#else //#ifdef OS_TASK_STACK_SIZE_REFINED
    
    // Application Tasks
    #define OS_TASK_STACK_SIZE_BLEWIFI_CTRL         (512)
    #define OS_TASK_STACK_SIZE_BLEWIFI_USER_APP     (512)
    #define OS_TASK_STACK_SIZE_SENSOR               (256)
    #define OS_TASK_STACK_SIZE_SENSOR_DATA          (1024)

#endif //#ifdef OS_TASK_STACK_SIZE_REFINED


/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _SYS_OS_CONFIG_PATCH_H_
