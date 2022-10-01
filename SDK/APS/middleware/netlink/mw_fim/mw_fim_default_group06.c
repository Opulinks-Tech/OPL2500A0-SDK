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
*  mw_fim_default_group06.c
*
*  Project:
*  --------
*  OPL2500 Project - the Flash Item Management (FIM) implement file
*
*  Description:
*  ------------
*  This implement file is include the Flash Item Management (FIM) function and api.
*
*  Author:
*  -------
*  Jeff Kuo
*
******************************************************************************/
/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file


// Sec 1: Include File
#include <stdlib.h>
#include <string.h>
#include "mw_fim_default_group06.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

#define MW_FIM_MSQ_FIM_LOAD_TABLE_REC_ID        0


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable


char *caMwFim_MsqFimRfCmdModesName[MW_FIM_MSQ_RF_MODE_ITEM_NUM] = {"Idle", "WT_L", "WRX", "BT_L", "BRX", "WT_H"};
T_MW_FIM_MSQ_FIM_PARAM_DEF g_tMwFim_MsqFimParam = {
    .u32FimLoadFileMax = MW_FIM_MSQ_FIM_LOAD_FILE_MAX,
    .u32FimLoadTableSize = MW_FIM_MSQ_FIM_LOAD_TABLE_SIZE,
    .u8RfRegNum = MW_FIM_MSQ_RF_MODE_ITEM_NUM,
    .u8RfModeNum = MW_FIM_MSQ_RF_MODE_VALUE_TABLE_NUM,
    .pcRfModeNames = caMwFim_MsqFimRfCmdModesName,
};

uint32_t g_ulaMwFimAddrBufferMsqFimLoad[MW_FIM_MSQ_FIM_LOAD_TABLE_NUM];
uint32_t g_ulaMwFimAddrBufferRfModeAddrTables[MW_FIM_MSQ_RF_MODE_ADDR_TABLE_NUM];
uint32_t g_ulaMwFimAddrBufferRfModeValueTables[MW_FIM_MSQ_RF_MODE_VALUE_TABLE_NUM];
uint32_t g_ulaMwFimAddrBufferWifiMib[MW_FIM_MSQ_WIFI_MIB_NUM];
// the information table of group 06

/* Group 6 is for MSQ FIM use, don't add any other type data 
 * The FILE ID must be listed from small to big.
 */
const T_MwFimFileInfo g_taMwFimGroupTable06[] =
{
    {MW_FIM_IDX_GR06_MSQ_FIM_LOAD,              MW_FIM_MSQ_FIM_LOAD_TABLE_NUM,      MW_FIM_MSQ_FIM_LOAD_TABLE_SIZE, NULL,   g_ulaMwFimAddrBufferMsqFimLoad},
    {MW_FIM_IDX_GP06_MSQ_RF_MODE_ADDR_TABLE,    MW_FIM_MSQ_RF_MODE_ADDR_TABLE_NUM,  MW_FIM_MSQ_RF_MODE_TABLE_SIZE,  NULL,   g_ulaMwFimAddrBufferRfModeAddrTables},
    {MW_FIM_IDX_GP06_MSQ_RF_MODE_VALUE_TABLE,   MW_FIM_MSQ_RF_MODE_VALUE_TABLE_NUM, MW_FIM_MSQ_RF_MODE_TABLE_SIZE,  NULL,   g_ulaMwFimAddrBufferRfModeValueTables},
    {MW_FIM_IDX_GP06_MSQ_WIFI_MIB,              MW_FIM_MSQ_WIFI_MIB_NUM,            MW_FIM_MSQ_WIFI_MIB_SIZE,       NULL,   g_ulaMwFimAddrBufferWifiMib},
    
    // the end, don't modify and remove it
    {0xFFFFFFFF,            0x00,              0x00,               NULL,                            NULL}
};


// Sec 5: declaration of global function prototype

T_MwFim_MsqFimLoadWrite    MwFim_MsqFimLoadWrite  = MwFim_MsqFimLoadWrite_impl;
T_MwFim_MsqFimLoadRead     MwFim_MsqFimLoadRead   = MwFim_MsqFimLoadRead_impl;

/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable


// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions

/**
 * @brief MwFim_MsqFimValidFileCompare
 *        compare function for qsort MSQ FIM load table
 *        If P1 File ID equals P2 File ID, comparing the Rec ID of P1 and P2.
 *        When P1 ID less than, equal to or greater than P2 ID, return -1, 0, or 1.
 * @param p1 [in] File 1
 * @param p2 [in] File 2
 * @return Comparison for two files
 */
int MwFim_MsqFimValidFileCompare(const void *p1, const void *p2)
{
    T_MW_FIM_MSQ_FIM_FILE *ptFile1, *ptFile2;
    ptFile1 = (T_MW_FIM_MSQ_FIM_FILE *)p1;
    ptFile2 = (T_MW_FIM_MSQ_FIM_FILE *)p2;
    
    if (ptFile1->u16FileId < ptFile2->u16FileId)
        return -1;
    
    else if (ptFile1->u16FileId > ptFile2->u16FileId)
        return 1;
    
    else
    {   /* File ID equals */
        if (ptFile1->u16RecId < ptFile2->u16RecId)
            return -1;
        if (ptFile1->u16RecId > ptFile2->u16RecId)
            return 1;
    }
    return 0;
}

/**
 * @brief MwFim_MsqFimLoadWrite
 *        Write MSQ FIM valid file table.
 *        The file ID MW_FIM_IDX_GR06_MSQ_FIM_LOAD will not be stored.
 *        In this function, the files id will be sorted from small to big.
 * @param ptFimLoad [in] The FIM load table content
 * @return Write FIM status
 *
 */
uint8_t MwFim_MsqFimLoadWrite_impl(T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad)
{
    uint8_t u8Ret;
    
    qsort(ptFimLoad->taValidFiles, ptFimLoad->u32ValidNum, sizeof(T_MW_FIM_MSQ_FIM_FILE), MwFim_MsqFimValidFileCompare);
    
    u8Ret = MwFim_FileWrite(MW_FIM_IDX_GR06_MSQ_FIM_LOAD, MW_FIM_MSQ_FIM_LOAD_TABLE_REC_ID, g_tMwFim_MsqFimParam.u32FimLoadTableSize, (uint8_t *)ptFimLoad);
    return u8Ret;
}

/**
 * @brief MwFim_MsqFimLoadRead
 *        Get MSQ FIM valid file load table. It will be used for initialization, only load FIM which listed in the file
 *        Considering boot time, the files list must be sorted from small to big.
 * @param ptFimLoad [out] The FIM load table content. The buffer must be allocated before calling this function.
 *
 */
uint8_t MwFim_MsqFimLoadRead_impl(T_MW_FIM_MSQ_FIM_LOAD *ptFimLoad)
{
    uint8_t u8Ret;
    
    if (!ptFimLoad)
        return MW_FIM_FAIL;
    
    u8Ret = MwFim_FileRead(MW_FIM_IDX_GR06_MSQ_FIM_LOAD, MW_FIM_MSQ_FIM_LOAD_TABLE_REC_ID, g_tMwFim_MsqFimParam.u32FimLoadTableSize, (uint8_t *)ptFimLoad);
    if (u8Ret == MW_FIM_FAIL)
        ptFimLoad->u32ValidNum = 0;
    
    return u8Ret;
}
