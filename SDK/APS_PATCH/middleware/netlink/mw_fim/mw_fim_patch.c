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
 *  @file mw_fim_patch.c
 * 
 *  @brief 
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "mw_fim.h"
#include "mw_fim_internal.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define MW_FIM_SIGNATURE_GROUP      0x67726F70  // grop
#define MW_FIM_FLASH_BLOCK_MASK     0x0FFF      // 4KB Mask
/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

/*
*************************************************************************
*                           Declarations of Private Functions
*************************************************************************
*/
void MwFim_FileDataDefaultFill_impl(void);
uint8_t MwFim_GroupBlockDrop_patch(uint32_t ulStartAddr);
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
 




 
 
 
 

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void MwFim_PatchInit(void)
{
    MwFim_GroupBlockDrop = MwFim_GroupBlockDrop_patch;
    MwFim_FileDataDefaultFill   = MwFim_FileDataDefaultFill_impl;
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */



/*************************************************************************
* FUNCTION:
*   MwFim_GroupBlockDrop
*
* DESCRIPTION:
*   Drop the flash of group block. write zero to group header signature
*
* PARAMETERS
*   1. ulStartAddr : [In] the start address
*   2. ulGroupSize : [In] the group size
*
* RETURNS
*   1. MW_FIM_OK   : success
*   2. MW_FIM_FAIL : fail
*
*************************************************************************/
uint8_t MwFim_GroupBlockDrop_patch(uint32_t ulStartAddr)
{
    uint8_t ubRet = MW_FIM_FAIL;
    uint32_t u32Signature = 0;

    if (Hal_Flash_AddrRead_Ext(g_MwFim_FlashCtrlCfg.eSpiIdx, g_MwFim_FlashCtrlCfg.eSlvIdx, ulStartAddr, 0, sizeof(u32Signature), (uint8_t *)&u32Signature))
        goto done;
    
    if (u32Signature != MW_FIM_SIGNATURE_GROUP)
        goto done;
    
    // check the start address
    if (MW_FIM_FLASH_BLOCK_MASK & ulStartAddr)
        goto done;

    // erase the group block
    u32Signature = 0;
    if (0 != Hal_Flash_AddrProgram_Ext(g_MwFim_FlashCtrlCfg.eSpiIdx, g_MwFim_FlashCtrlCfg.eSlvIdx, ulStartAddr, 0, sizeof(u32Signature), (uint8_t *)&u32Signature))
        goto done;

    ubRet = MW_FIM_OK;

done:    
    return ubRet;    
}
