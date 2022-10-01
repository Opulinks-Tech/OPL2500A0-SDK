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

/******************************************************************************
*  Filename:
*  ---------
*  mw_fim_default_group06.h
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
#ifndef _MW_FIM_DEFAULT_GROUP06_H_
#define _MW_FIM_DEFAULT_GROUP06_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file


// Sec 1: Include File
#include "mw_fim.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
// the file ID
// xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx
// ^^^^ ^^^^ Zone (0~3)
//           ^^^^ ^^^^ Group (0~8), 0 is reserved for swap
//                     ^^^^ ^^^^ ^^^^ ^^^^ File ID, start from 0
typedef enum
{
    MW_FIM_IDX_GP06_START = 0x00060000,             // the start IDX of group 06
    MW_FIM_IDX_GR06_MSQ_FIM_LOAD = MW_FIM_IDX_GP06_START,   /* Define the which tables will be loaded in cold boot */
    MW_FIM_IDX_GP06_MSQ_RF_MODE_ADDR_TABLE,         /* 1 */
    MW_FIM_IDX_GP06_MSQ_RF_MODE_VALUE_TABLE,        /* 2 */
    MW_FIM_IDX_GP06_MSQ_WIFI_MIB,                   /* 3 */
    MW_FIM_IDX_GP06_MAX
} E_MwFimIdxGroup06;

#define MW_FIM_MSQ_FIM_LOAD_FILE_MAX            63
#define MW_FIM_MSQ_FIM_LOAD_TABLE_SIZE          (sizeof(T_MW_FIM_MSQ_FIM_LOAD) + sizeof(T_MW_FIM_MSQ_FIM_FILE)*MW_FIM_MSQ_FIM_LOAD_FILE_MAX)
#define MW_FIM_MSQ_FIM_LOAD_TABLE_NUM           1

#define MW_FIM_MSQ_RF_MODE_ITEM_NUM             25
#define MW_FIM_MSQ_RF_MODE_TABLE_SIZE           (MW_FIM_MSQ_RF_MODE_ITEM_NUM * sizeof(uint32_t))
#define MW_FIM_MSQ_RF_MODE_ADDR_TABLE_NUM       1
#define MW_FIM_MSQ_RF_MODE_VALUE_TABLE_NUM      6       /* 6 MODE, IDLE, WiFi RX, WiFi TX_LP, WiFi TX_HP, BLE RX, and BLE TX */

#define MW_FIM_MSQ_WIFI_MIB_SIZE                (64)
#define MW_FIM_MSQ_WIFI_MIB_NUM                 3

/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
typedef struct
{   /* For patch use */
    uint32_t u32FimLoadFileMax;
    uint32_t u32FimLoadTableSize;
    uint8_t u8RfRegNum;
    uint8_t u8RfModeNum;
    char **pcRfModeNames;
} T_MW_FIM_MSQ_FIM_PARAM_DEF;


typedef struct
{
    uint16_t u16FileId;
    uint16_t u16RecId; 
} T_MW_FIM_MSQ_FIM_FILE;

typedef struct
{
    uint32_t u32ValidNum;
    T_MW_FIM_MSQ_FIM_FILE taValidFiles[];
} T_MW_FIM_MSQ_FIM_LOAD;


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern const T_MwFimFileInfo g_taMwFimGroupTable06[];
extern T_MW_FIM_MSQ_FIM_PARAM_DEF g_tMwFim_MsqFimParam;

// Sec 5: declaration of global function prototype
typedef uint8_t (*T_MwFim_MsqFimLoadWrite)(T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad);
typedef uint8_t (*T_MwFim_MsqFimLoadRead)(T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad);

extern T_MwFim_MsqFimLoadWrite          MwFim_MsqFimLoadWrite;
extern T_MwFim_MsqFimLoadRead           MwFim_MsqFimLoadRead;

uint8_t MwFim_MsqFimLoadWrite_impl(T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad);
uint8_t MwFim_MsqFimLoadRead_impl(T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad);
/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable

// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif

#endif // _MW_FIM_DEFAULT_GROUP03_H_
