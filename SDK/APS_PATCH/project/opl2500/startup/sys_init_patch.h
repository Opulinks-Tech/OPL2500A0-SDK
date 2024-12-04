/* *****************************************************************************
 *  Copyright 2019, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2018
 *
 *******************************************************************************
 *
 *  @file sys_init_patch.h 
 * 
 *  @brief Patch for Sys init patch
 *  
 ******************************************************************************/

#ifndef _SYS_INIT_H_
#error "Please include sys_init.h. Don't directly include sys_init_patch.h"
#endif

#ifndef _SYS_INIT_PATCH_H_
#define _SYS_INIT_PATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "sys_init.h"
#include "hal_spi.h"
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
/**
 * Setup XIP mode.
 *
 * - XIP_MODE_OTA_BUNDLE
 *   XIP content is within OTA image. Example project is APS_PATCH\examples\get_started\XIP
 *   When updating firmware, the XIP content is also updated.
 *
 * - XIP_MODE_STAND_ALONE
 *   XIP content is NOT in OTA image. Example project is APS_PATCH\examples\get_started\XIP_Standalone
 *   In this mode, user must handle XIP content manually.
 *    1. Assign the flash address where XIP content is.
 *       e.g.
 *           Sys_XipSetup(XIP_MODE_STAND_ALONE, SPI_SLAVE_0, XIP_FLASH_ADDRESS)
 *    2. Erase and write flash for XIP content.
 *       Besides downloading the Application firmware, to erase and download XIP binary file is also needed.
 * - XIP_MODE_EXT_FLASH
 *   [Need Boot Agent, Pack tool (Download tool) support]
 *   XIP content is in external flash and the content format is as OTA image format.
 *   Boot Agent will update the external flash according to the OTA header.
 * - XIP_MODE_EXT_PSRAM
 *   XIP content is in external PSRAM.
 *   When system initialization, it will copy XIP content (RO region) from internal flash to external PSRAM,
 *   copy RW region to PSRAM, and zero initialize the ZI region in PSRAM.
 *   The XIP content size is limited by OTA image size.
 */
typedef enum
{
    XIP_MODE_DISABLE=1,
    XIP_MODE_OTA_BUNDLE,        /* Single flash (internal) */
    XIP_MODE_STAND_ALONE,       /* Single flash (internal) */
    XIP_MODE_EXT_FLASH,         /* !!!!!!! Not support yet. Two flash. XIP in external flash */
    XIP_MODE_EXT_PSRAM,         /* XIP in external PSRAM */
} E_XIP_MODE;


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
void SysInit_EntryPoint(void);


/**
 * @brief XIP setup
 *        Please call this function at __Patch_EntryPoint and before SysInit_EntryPoint
 *        The setup procedure only run when cold boot. If called when warm boot, it will be ingored.
 * @warning -------------------------------------------------------------------------------------------------
 *          | XIP limitation:                                                                               |
 *          | 1. When flash erasing and polling busy, the XIP will be delayed until flash process finished. |
 *          |    Including FLASH API and FIM API                                                            |
 *          | 2. QSPI DMA is not available                                                                  |
 *          |    Hal_Qspi_Dma_Access is invalid                                                             |
 *          -------------------------------------------------------------------------------------------------
 *
 * @param eXipMode [in] To setup the XIP mode.
 *                      XIP_MODE_DISABLE: There is no XIP content in flash.
 *                      XIP_MODE_OTA_BUNDLE: The XIP content is within OTA image
 *                      XIP_MODE_STAND_ALONE: The XIP content is NOT within OTA image. User will handle it.
 *                      XIP_MODE_EXT_FLASH: The full external flash are XIP content
 *                      XIP_MODE_EXT_PSRAM: The full external PSRAM are XIP content, and support write to PSRAM
 *                      When the mode is set to XIP_MODE_EXT_FLASH or XIP_MODE_EXT_PSRAM, the eSlvIdx should be CS1 ~ CS3.
 * @param eSlvIdx [in] Assign XIP mode slave index. Default is CS0.
 *                     SPI_SLAVE_0 for SPI0_CS0
 *                     SPI_SLAVE_1 for SPI0_CS1
 *                     SPI_SLAVE_2 for SPI0_CS2
 *                     SPI_SLAVE_3 for SPI0_CS3
 *                     Ingored when XIP mode is XIP_MODE_DISABLE.
 * @param u32FlashAddress [in] Only valid when XIP_MODE_STAND_ALONE mode.
 *                             It is ignored in other modes.
 * @return Setup status
 * @retval RESULT_SUCCESS: configuration is valid
 * @retval RESULT_FAIL: configuration is invalid
 */
E_RESULT_COMMON Sys_XipSetup(E_XIP_MODE eXipMode, E_SpiSlave_t eSlvIdx, uint32_t u32FlashAddress);

#ifdef __cplusplus
}
#endif
#endif  /* _SYS_INIT_PATCH_H_ */
