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
 *  @file mw_ota_boot_patch.c
 * 
 *  @brief OTA boot patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "mw_ota.h"
#include "mw_ota_boot.h"
#include "hal_spi.h"
#include "hal_flash.h"
#include "hal_flash_internal.h"
#include "hal_system.h"
#include "boot_sequence.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define PATCH_HEAD_LEN      8
#define PATCH_DATA_LEN      256

#define CHECK               "<CHECK>"
#define ACK                 "<ACK>"
#define NACK                "<NACK>"
#define START               "<START>"
#define ESCAPE              "<ESCAPE>"
#define AGENT               "<AGENT>"

#define BOOT_CHECK_RETRY    100     // 100ms

#define PATTERN_ENTRY       0xfe
#define PATTERN_BOOT        'u'
#define PATTERN_PATCH       'p'
#define PATTERN_ESCAPE      'e'
#define PATTERN_CHANGE      'c'     /* Change Baud rate */
#define PATTERN_WRITE       '1'
#define PATTERN_ERASE       '2'
#define PATTERN_HEADER      '3'     // OTA image header

#define CHANGE_BAUD_ACK     0x5A                /* 'Z' */
#define CHANGE_BAUD_NACK    0x00
#define DEFAULT_WAIT_UART_DELAY         1       /* unit: ms */
#define BEFORE_CHANGE_BAUD_DELAY        5       /* unit: ms */
#define CHANGE_BAUD_SEND_SYNC_DELAY    95       /* unit: ms */

#define INIT_BAUD_RATE               115200


#define SWITCH_CS_TO_INTERNAL()     (g_eMwOtaFlashCsIdxCurr = g_eaMwOtaFlashCsIdx[MW_OTA_FLASH_INTERNAL])
#define SWITCH_CS_TO_EXTERNAL()     (g_eMwOtaFlashCsIdxCurr = g_eaMwOtaFlashCsIdx[MW_OTA_FLASH_EXTERNAL])

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

typedef enum
{
    MW_OTA_FLASH_INTERNAL=0,
    MW_OTA_FLASH_EXTERNAL,
    MW_OTA_FLASH_NUM,
} E_MW_OTA_FLASH_IDX;
/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
uint8_t MwOta_Init_impl(T_MwOtaLayoutInfo *ptInfo, uint8_t ubBoot);
uint32_t Boot_RecvMultiData(uint8_t *data, uint32_t u32Length);

uint8_t MwOta_Boot_Init_patch(void);
uint8_t MwOta_Init_patch(T_MwOtaLayoutInfo *ptInfo, uint8_t ubBoot);
uint32_t MwOta_EraseFunc(E_SpiIdx_t u32SpiIdx, uint32_t u32SecAddr);
uint32_t MwOta_WriteFunc(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint32_t MwOta_ReadFunc(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data);
uint8_t MwOta_Boot_HeaderPaser_patch(void);
uint8_t MwOta_PrepareExt(uint32_t ulImageSize, uint32_t ulImageSum);
/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */
 
extern int8_t g_bMwOtaImageIdx;
extern uint32_t g_u32BA_PrevSuccBaud;
extern uint8_t g_ubMwOtaBoot; 
// the function pointer of flash
extern T_Hal_Flash_4KSectorAddrErase g_tMwOtaEraseFunc;
extern T_Hal_Flash_AddrProgram g_tMwOtaWriteFunc;
extern T_Hal_Flash_AddrRead g_tMwOtaReadFunc;


E_SpiSlave_t g_eMwOtaFlashCsIdxCurr;
E_SpiSlave_t g_eaMwOtaFlashCsIdx[MW_OTA_FLASH_NUM];
T_MwOtaLayoutInfoExtFlash g_tMwOtaLayoutInfoExtFlash;
T_MwOtaFlashHeader_ExtFlash g_tMwOtaPrepareHeaderInfoExtFlash;
uint32_t g_ulMwOtaPrepareExtFlashImageAddr;            // the start address of image
uint32_t g_ulMwOtaPrepareExtFlashWriteAddr;            // write the image from the address
uint32_t g_ulMwOtaPrepareExtFlashWriteSize;            // the size of the written data
uint8_t g_ubMwOtaPrepareExtFlashStatus;

// the function pointer of flash
T_Hal_Flash_4KSectorAddrErase_Ext g_tMwOtaEraseFuncExt;
T_Hal_Flash_AddrProgram_Ext g_tMwOtaWriteFuncExt;
T_Hal_Flash_AddrRead_Ext g_tMwOtaReadFuncExt;


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void MwOta_PatchInit(void)
{
    g_eaMwOtaFlashCsIdx[MW_OTA_FLASH_INTERNAL] = g_eMwOtaFlashCsIdxCurr = SPI_SLAVE_0;
    g_eaMwOtaFlashCsIdx[MW_OTA_FLASH_EXTERNAL] = SPI_SLAVE_MAX;
    memset((void *)&g_tMwOtaLayoutInfoExtFlash, 0, sizeof(g_tMwOtaLayoutInfoExtFlash));
    memset((void *)&g_tMwOtaPrepareHeaderInfoExtFlash, 0, sizeof(g_tMwOtaPrepareHeaderInfoExtFlash));
    
    MwOta_Boot_Init = MwOta_Boot_Init_patch;
    MwOta_Init = MwOta_Init_patch;
    MwOta_Boot_HeaderPaser = MwOta_Boot_HeaderPaser_patch;
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

/*************************************************************************
* FUNCTION:
*   MwOta_Boot_Init
*
* DESCRIPTION:
*   the init of OTA boot
*
* PARAMETERS
*   none
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_Boot_Init_patch(void)
{
    T_MwOtaLayoutInfo tLayout;
    S_MW_OTA_BOOT_STS *psOtaBootSts = (S_MW_OTA_BOOT_STS *)MW_OTA_BOOT_STATUS_DATA_ADDR;
    
    // give the layout information
    tLayout.ulaHeaderAddr[0] = MW_OTA_HEADER_ADDR_1;
    tLayout.ulaHeaderAddr[1] = MW_OTA_HEADER_ADDR_2;
    tLayout.ulaImageAddr[0] = MW_OTA_IMAGE_ADDR_1;
    tLayout.ulaImageAddr[1] = MW_OTA_IMAGE_ADDR_2;
    tLayout.ulImageSize = MW_OTA_IMAGE_SIZE;
    if (MW_OTA_OK != MwOta_Init(&tLayout, 1))
    {
        return MW_OTA_FAIL;
    }
    
    SYSTEM_SPARE_REG_OTA_STATUS = MW_OTA_BOOT_STATUS_DATA_ADDR;
    psOtaBootSts->Signature = MW_OTA_BOOT_STATUS_SIGNATURE;
    psOtaBootSts->OtaExtImgEnable = (g_tMwOtaLayoutInfoExtFlash.ulImageSize > 0)?1:0;
    psOtaBootSts->CurrOtaImagAddr = tLayout.ulaImageAddr[(g_bMwOtaImageIdx)?1:0];
    psOtaBootSts->CurrOtaExtImgAddr = g_tMwOtaLayoutInfoExtFlash.ulaImageAddr[(g_bMwOtaImageIdx)?1:0];
    if (g_tMwOtaLayoutInfoExtFlash.ulImageSize == 0)
    {   /* Not enabled external flash */
        psOtaBootSts->CurrOtaExtImgAddr = 0;
    }
    
    // give the fake current index
    MwOta_CurrentIdxFake();
    
    g_u32BA_PrevSuccBaud = INIT_BAUD_RATE;
    
    return MW_OTA_OK;
}


void MwOta_ExtFlashInit(uint32_t SpiSlaveIdx, uint32_t ExtFlashStart, uint32_t ExtFlashSize)
{
    E_SpiSlave_t eSlvIdx = (E_SpiSlave_t)SpiSlaveIdx;
    if ((eSlvIdx < SPI_SLAVE_MAX) && (eSlvIdx != g_eaMwOtaFlashCsIdx[MW_OTA_FLASH_INTERNAL]))
    {
        g_eaMwOtaFlashCsIdx[MW_OTA_FLASH_EXTERNAL] = eSlvIdx;
        g_tMwOtaLayoutInfoExtFlash.ulaImageAddr[0] = ExtFlashStart;
        g_tMwOtaLayoutInfoExtFlash.ulaImageAddr[1] = ExtFlashStart + ExtFlashSize;
        g_tMwOtaLayoutInfoExtFlash.ulImageSize = ExtFlashSize;
    }
}

/*************************************************************************
* FUNCTION:
*   MwOta_Init
*   Add external flash access chip select index function
*
* DESCRIPTION:
*   OTA init
*
* PARAMETERS
*   1. ptInfo : [In] the layout information
*   2. ubBoot : [In] is 2nd boot loader or not
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_Init_patch(T_MwOtaLayoutInfo *ptInfo, uint8_t ubBoot)
{
    uint8_t ret;
    ret = MwOta_Init_impl(ptInfo, ubBoot);
    if (ret == MW_OTA_FAIL)
        return MW_OTA_FAIL;
    
    /* Update functions */
    if (g_ubMwOtaBoot == 1)
    {
        g_tMwOtaEraseFuncExt = Hal_Flash_4KSectorAddrErase_Internal_Ext;
        g_tMwOtaWriteFuncExt = Hal_Flash_AddrProgram_Internal_Ext;
        g_tMwOtaReadFuncExt = Hal_Flash_AddrRead_Internal_Ext;
    }
    else
    {
        g_tMwOtaEraseFuncExt = Hal_Flash_4KSectorAddrErase_Ext;
        g_tMwOtaWriteFuncExt = Hal_Flash_AddrProgram_Ext;
        g_tMwOtaReadFuncExt = Hal_Flash_AddrRead_Ext;
    }

    g_tMwOtaEraseFunc = MwOta_EraseFunc;
    g_tMwOtaWriteFunc = MwOta_WriteFunc;
    g_tMwOtaReadFunc = MwOta_ReadFunc;
    return MW_OTA_OK;
}

uint32_t MwOta_EraseFunc(E_SpiIdx_t u32SpiIdx, uint32_t u32SecAddr)
{
    return g_tMwOtaEraseFuncExt(u32SpiIdx, g_eMwOtaFlashCsIdxCurr, u32SecAddr);
}
uint32_t MwOta_WriteFunc(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    return g_tMwOtaWriteFuncExt(u32SpiIdx, g_eMwOtaFlashCsIdxCurr, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
}
uint32_t MwOta_ReadFunc(E_SpiIdx_t u32SpiIdx, uint32_t u32StartAddr, uint8_t u8UseQuadMode, uint32_t u32Size, uint8_t *pu8Data)
{
    return g_tMwOtaReadFuncExt(u32SpiIdx, g_eMwOtaFlashCsIdxCurr, u32StartAddr, u8UseQuadMode, u32Size, pu8Data);
}

/*************************************************************************
* FUNCTION:
*   MwOta_Boot_HeaderPaser
*
* DESCRIPTION:
*   get the header information and prepare the information for OTA write
*
* PARAMETERS
*   none
*
* RETURNS
*   MW_OTA_OK   : successful
*   MW_OTA_FAIL : fail
*
*************************************************************************/
uint8_t MwOta_Boot_HeaderPaser_patch(void)
{
    uint8_t ubaData[64];
    T_MwOtaFlashHeader *ptHeader;
    T_MwOtaFlashHeader_ExtFlash *ptHeaderExt;
    
    Boot_SendMultiData(START);
    
    // receive the 64 bytes data
    if (0 == Boot_RecvMultiData(ubaData, 64))
    {
        // prepare the information
        ptHeader = (T_MwOtaFlashHeader*)ubaData;
        if (MW_OTA_OK != MwOta_Prepare(ptHeader->uwProjectId, ptHeader->uwChipId, ptHeader->uwFirmwareId, ptHeader->ulImageSize, ptHeader->ulImageSum))
        {
            Boot_SendMultiData(NACK);
            return MW_OTA_FAIL;
        }
        ptHeaderExt = (T_MwOtaFlashHeader_ExtFlash *)(ptHeader + 1);
        if (ptHeaderExt->ExtImageEnable)
        {
            if (MW_OTA_OK != MwOta_PrepareExt(ptHeaderExt->ExtImageSize, ptHeaderExt->ExtImageSum))
            {
                Boot_SendMultiData(NACK);
                return MW_OTA_FAIL;
            }
        }
    }
    
    Boot_SendMultiData(ACK);
    return MW_OTA_OK;
}

uint8_t MwOta_PrepareExt(uint32_t ulImageSize, uint32_t ulImageSum)
{
    uint32_t ulHeaderSum;
    uint32_t ulSize;
    uint8_t *pubData;
    
    uint32_t i;
    
    // error check
    if (g_bMwOtaImageIdx == MW_OTA_IDX_INVALID)
        return MW_OTA_FAIL;
    
    /* Not enable external flash */
    if (ulImageSize > g_tMwOtaLayoutInfoExtFlash.ulImageSize)
        return MW_OTA_OK;

    // compute the check sum
    ulHeaderSum = 0;
    ulSize = sizeof(T_MwOtaFlashHeader_ExtFlash) - sizeof(uint32_t);    // without the size of check sum
    pubData = (uint8_t*)&g_tMwOtaPrepareHeaderInfoExtFlash;
    for (i=0; i<ulSize; i++)
    {
        ulHeaderSum += pubData[i];
    }
    g_tMwOtaPrepareHeaderInfoExtFlash.ExtFuncHeaderSum = ulHeaderSum;
    
    // the prepare address of header and image
    if (g_bMwOtaImageIdx == 0)
    {
        g_ulMwOtaPrepareExtFlashImageAddr = g_tMwOtaLayoutInfoExtFlash.ulaImageAddr[1];
        g_ulMwOtaPrepareExtFlashWriteAddr = g_tMwOtaLayoutInfoExtFlash.ulaImageAddr[1];
    }
    else
    {
        g_ulMwOtaPrepareExtFlashImageAddr = g_tMwOtaLayoutInfoExtFlash.ulaImageAddr[0];
        g_ulMwOtaPrepareExtFlashWriteAddr = g_tMwOtaLayoutInfoExtFlash.ulaImageAddr[0];
    }
    // the size of the written data
    g_ulMwOtaPrepareExtFlashWriteSize = 0;
    
    
    SWITCH_CS_TO_EXTERNAL();
    // erase the flash sector
    // image
    for (i=0; i<g_tMwOtaLayoutInfoExtFlash.ulImageSize; i+= 0x1000)
    {
        if (0 != g_tMwOtaEraseFunc(SPI_IDX_0, g_ulMwOtaPrepareExtFlashImageAddr + i))
        {
            SWITCH_CS_TO_INTERNAL();
            printf("To erase the image [%u] of MW_OTA is fail.\n", g_ulMwOtaPrepareExtFlashImageAddr + i);
            return MW_OTA_FAIL;
        }
    }
    SWITCH_CS_TO_INTERNAL();
    
    // update the prepare status 
    g_ubMwOtaPrepareExtFlashStatus = MW_OTA_PREPARE_READY;
    
    return MW_OTA_OK;
}

