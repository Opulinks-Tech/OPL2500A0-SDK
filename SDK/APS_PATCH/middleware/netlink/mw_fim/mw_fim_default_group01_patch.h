/******************************************************************************
*  Copyright 2017 - 2023, Opulinks Technology Ltd.
*  ----------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2023
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  mw_fim_default_group01_patch.h
*
*  Project:
*  --------
*  OPL2500 Project - the Flash Item Management (FIM) definition file
*
*  Description:
*  ------------
*  This include file is the Flash Item Management (FIM) definition file
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
#ifndef _MW_FIM_DEFAULT_GROUP01_PATCH_H_
#define _MW_FIM_DEFAULT_GROUP01_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "sys_cfg.h"
#include "mw_fim_default_group01.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous



/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const T_RfCfg g_tMwFimDefaultRfConfig_patch;

// Sec 5: declaration of global function prototype
void MwFim_Group01_patch( void );

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP01_H_
