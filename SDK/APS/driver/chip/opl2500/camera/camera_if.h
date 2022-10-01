/* *****************************************************************************
 *  Copyright 2018 - 2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file camera_if.h 
 * 
 *  @brief Camera interface driver
 *
 *  
 ******************************************************************************/

#ifndef _CAMERA_IF_H_
#define _CAMERA_IF_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
#include "opulinks.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define CAM_SPI         SPI3
    
    
/* CAM_IF CAM_MODE@0x104, CAM_MODE */
#define CAM_REG_CAM_MODE_MASTER     (0 << CAM_CAM_MODE_CAM_MODE_Pos)
#define CAM_REG_CAM_MODE_SLAVE      (1 << CAM_CAM_MODE_CAM_MODE_Pos)

/* CAM_IF CAM_WIDTH@0x104, CAM_WIDTH */
typedef enum
{
    CAM_IF_DATAIO_WIDTH_1  = 0,
    CAM_IF_DATAIO_WIDTH_2  = 1,
    CAM_IF_DATAIO_WIDTH_3  = 2,
    CAM_IF_DATAIO_WIDTH_4  = 3,
    CAM_IF_DATAIO_WIDTH_5  = 4,
    CAM_IF_DATAIO_WIDTH_6  = 5,
    CAM_IF_DATAIO_WIDTH_7  = 6,
    CAM_IF_DATAIO_WIDTH_8  = 7,
    CAM_IF_DATAIO_WIDTH_9  = 8,
    CAM_IF_DATAIO_WIDTH_10 = 9,
    CAM_IF_DATAIO_WIDTH_11 = 10,
    CAM_IF_DATAIO_WIDTH_12 = 11,
    CAM_IF_DATAIO_WIDTH_13 = 12,
    CAM_IF_DATAIO_WIDTH_14 = 13,
    CAM_IF_DATAIO_WIDTH_15 = 14,
    CAM_IF_DATAIO_WIDTH_16 = 15,
} E_CamIf_CamDataIoNum;
#define CAM_IF_DATAIO_NUM_TO_WIDTH(num)     (((num) - 1) & CAM_CAM_WIDTH_CAM_WIDTH_Msk)

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */




typedef uint32_t (*T_CamIf_Init)(uint32_t u32DataIoNum, uint32_t u32DmaRxLvl);
typedef void (*T_CamIf_DeInit)(void);
typedef uint32_t (*T_CamIf_Start)(void);
typedef void (*T_CamIf_Stop)(void);
typedef void (*T_CamIf_WaitFifoTransDone)(void);

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

extern T_CamIf_Init         CamIf_Init;
extern T_CamIf_DeInit       CamIf_DeInit;
extern T_CamIf_Start        CamIf_Start;
extern T_CamIf_Stop         CamIf_Stop;
extern T_CamIf_WaitFifoTransDone CamIf_WaitFifoTransDone;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */


uint32_t CamIf_Init_impl(uint32_t u32DataIoNum, uint32_t u32DmaRxLvl);
void CamIf_DeInit_impl(void);
uint32_t CamIf_Start_impl(void);
void CamIf_Stop_impl(void);
void CamIf_WaitFifoTransDone_impl(void);


#ifdef __cplusplus
}
#endif
#endif  /* _CAMERA_IF_H_ */
