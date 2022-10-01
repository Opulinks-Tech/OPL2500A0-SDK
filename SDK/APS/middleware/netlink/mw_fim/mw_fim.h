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
*  mw_fim.h
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
#ifndef _MW_FIM_H_
#define _MW_FIM_H_

#ifdef __cplusplus
extern "C" {
#endif

// Sec 0: Comment block of the file

/* ROM default Zone 0 Group size is 0x1000 (4kB)
 * Including - 
 * (1) Group header (T_MwFimGroupHeader) = 12 bytes
 * (2) Each files header and files data
 * (2.1) File header (T_MwFimFileHeader) = 12 bytes
 */
    
    

// Sec 1: Include File
#include <stdio.h>
#include <stdint.h>
#include "hal_flash.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define MW_FIM_OK                   1
#define MW_FIM_FAIL                 0


#define MW_FIM_FILEID_GRP_MSK       0xFFFF0000UL
/******************************
Declaration of data structure
******************************/
// Sec 3: structure, uniou, enum, linked list
    
typedef struct
{
    E_SpiIdx_t      eSpiIdx;
    E_SpiSlave_t    eSlvIdx;
} T_MwFimFlashCtrlCfg;
    
// zone information (16 bytes)
typedef struct
{
    uint32_t ulBaseAddr;
    uint32_t ulBlockSize;
    uint32_t ulBlockNum;
    uint8_t *pubVerTable;               // the version table of group
} T_MwFimZoneInfo;

// file information (16 bytes)
typedef struct
{
    uint32_t ulFileId;
    uint16_t uwRecordMax;
    uint16_t uwDataSize;
    uint8_t *pubDefaultValue;           // the default value of file
    uint32_t *pulDataAddr;              // prepare the address buffer
} T_MwFimFileInfo;

// the header of group (12 bytes)
typedef struct
{
    uint32_t ulSignature;               // the signature of group
    uint8_t ubZoneIdx;                  // the zone index
    uint8_t ubGroupIdx;                 // the group index
    uint8_t ubMajorVersion;             // the version of group
    uint8_t ubMinorVersion;             // the sequence number
    uint32_t ulCheckSum;
} T_MwFimGroupHeader;

// the header of file (12 bytes)
typedef struct
{
    uint32_t ulSignature;               // the signature of file
    uint32_t ulFileId;                  // the file ID = Zone(8) + Group(8) + File ID(16)
    uint16_t uwRecordIdx;               // the index of record
    uint16_t uwDataSize;                // the size of file (without the header)
} T_MwFimFileHeader;

// group status (8 bytes)
typedef struct
{
    uint32_t ulFreeOffset;              // the address offset of free space
    uint8_t ubMajorVersion;             // the version of group
    uint8_t ubMinorVersion;             // the sequence number
    uint8_t ubBlockIdx;                 // the block index of group
    uint8_t ubReserved;
} T_MwFimGroupStatus;

// file data (12 bytes)
typedef struct
{
    uint32_t ulFileId;
    uint16_t uwRecIdx;
    uint16_t uwFileSize;
    uint8_t *pubFileData;
} T_MwFimFileData;

// flash layout update
typedef void (*T_MwFim_FlashLayoutUpdate_Fp)(void);


/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global variable
extern T_MwFim_FlashLayoutUpdate_Fp MwFim_FlashLayoutUpdate;


// Sec 5: declaration of global function prototype
typedef void (*T_MwFim_Init_Fp)(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
typedef uint8_t (*T_MwFim_FileRead_Fp)(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData);
typedef uint8_t (*T_MwFim_FileWrite_Fp)(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData);
typedef uint8_t (*T_MwFim_FileWriteDefault_Fp)(uint32_t ulFileId, uint16_t uwRecIdx);
typedef uint8_t (*T_MwFim_FileDelete_Fp)(uint32_t ulFileId, uint16_t uwRecIdx);

typedef uint8_t (*T_MwFim_ZoneInfoUpdate_Fp)(uint32_t ulZoneIdx, T_MwFimZoneInfo *ptZoneInfo);
typedef uint8_t (*T_MwFim_GroupInfoUpdate_Fp)(uint32_t ulZoneIdx, uint32_t ulGroupIdx, T_MwFimFileInfo *ptGroupInfo);
typedef uint8_t (*T_MwFim_GroupVersionUpdate_Fp)(uint32_t ulZoneIdx, uint32_t ulGroupIdx, uint8_t ubVersion);
typedef uint16_t (*T_MwFim_FileSizeGet)(uint32_t ulFileId);

extern T_MwFim_Init_Fp MwFim_Init;
extern T_MwFim_FileRead_Fp MwFim_FileRead;
extern T_MwFim_FileWrite_Fp MwFim_FileWrite;
extern T_MwFim_FileWriteDefault_Fp MwFim_FileWriteDefault;
extern T_MwFim_FileDelete_Fp MwFim_FileDelete;

extern T_MwFim_ZoneInfoUpdate_Fp MwFim_ZoneInfoUpdate;
extern T_MwFim_GroupInfoUpdate_Fp MwFim_GroupInfoUpdate;
extern T_MwFim_GroupVersionUpdate_Fp MwFim_GroupVersionUpdate;
extern T_MwFim_FileSizeGet MwFim_FileSizeGet;

void MwFim_PreInitCold(void);

void MwFim_Init_impl(E_SpiIdx_t eSpiIdx, E_SpiSlave_t eSlvIdx);
uint8_t MwFim_InitSemaphore(void);
uint8_t MwFim_FileRead_impl(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData);
uint8_t MwFim_FileWrite_impl(uint32_t ulFileId, uint16_t uwRecIdx, uint16_t uwFileSize, uint8_t *pubFileData);
uint8_t MwFim_FileWriteDefault_impl(uint32_t ulFileId, uint16_t uwRecIdx);
uint8_t MwFim_FileDelete_impl(uint32_t ulFileId, uint16_t uwRecIdx);
uint8_t MwFim_ZoneInfoUpdate_impl(uint32_t ulZoneIdx, T_MwFimZoneInfo *ptZoneInfo);
uint8_t MwFim_GroupInfoUpdate_impl(uint32_t ulZoneIdx, uint32_t ulGroupIdx, T_MwFimFileInfo *ptGroupInfo);
uint8_t MwFim_GroupVersionUpdate_impl(uint32_t ulZoneIdx, uint32_t ulGroupIdx, uint8_t ubVersion);
uint16_t MwFim_FileSizeGet_impl(uint32_t ulFileId);
/***************************************************
Declaration of static Global Variables & Functions
***************************************************/
// Sec 6: declaration of static global variable
extern T_MwFimFlashCtrlCfg g_MwFim_FlashCtrlCfg;

// Sec 7: declaration of static function prototype


#ifdef __cplusplus
}
#endif
#include "mw_fim_patch.h"
#endif // _MW_FIM_H_
