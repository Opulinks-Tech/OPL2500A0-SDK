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
 * @file ov2640.c
 * 
 * @brief OV2640 control API
 * 
 *
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "ov2640.h"
#include "ov2640_cfg.h"
#include "sccb.h"
#include "camera_if.h"
#include "camera.h"

#include "hal_gpio.h"
#include "hal_tick.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
#define OV2640_SKIP_PICTURE_AFTER_CFG   10
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




/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

/* Init */
T_OV2640_Init   OV2640_Init     = OV2640_Init_impl;
T_OV2640_ReadID OV2640_ReadID   = OV2640_ReadID_impl;

/* Windows and size configuration */
T_OV2640_ImageWindowSet OV2640_ImageWindowSet   = OV2640_ImageWindowSet_impl;
T_OV2640_OutputSizeSet  OV2640_OutputSizeSet    = OV2640_OutputSizeSet_impl;

/* Effect */
T_OV2640_LightModeSet       OV2640_LightModeSet     = OV2640_LightModeSet_impl;
T_OV2640_SaturationSet      OV2640_SaturationSet    = OV2640_SaturationSet_impl;
T_OV2640_BrightnessSet      OV2640_BrightnessSet    = OV2640_BrightnessSet_impl;
T_OV2640_ContrastSet        OV2640_ContrastSet      = OV2640_ContrastSet_impl;
T_OV2640_SpecialEffectSet   OV2640_SpecialEffectSet = OV2640_SpecialEffectSet_impl;
T_OV2640_YuvSeqSet          OV2640_YuvSeqSet        = OV2640_YuvSeqSet_impl;


const S_OV2640_ImgWindowSize g_saOV2640_ImageWindowSize[CAMERA_FRAMESIZE_NUM] = {
   /* Offset_X, Offset_Y,    Hsize,   Vsize */
    {        0,        0,      256,     192}, /* SQCIF  128X96    */
    {        0,        0,      320,     240}, /* QQVGA  160x120   */
    {        0,        0,      352,     288}, /* QCIF   176x144   */
    {        0,        0,      320,     240}, /* QVGA   320x240   */
    {        0,        0,      352,     288}, /* CIF    352x288   */
    {        0,        0,      480,     320}, /* HVGA   480x320   */
    {        0,        0,      640,     480}, /* VGA    640x480   */
    {        0,        0,      800,     600}, /* SVGA   800x600   */
    {        0,        0,     1600,    1200}, /* XGA    1024x768  */
    {        0,        0,     1280,     720}, /* HD     1280x720  */
    {        0,        0,     1500,    1200}, /* SXGA   1280x1024 */
    {        0,        0,     1600,    1200}, /* UXGA   1600x1200 */
};



/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */

uint8_t u8Ov2640_WriteRegDelayMs = 2;

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

uint32_t OV2640_Init_impl(S_IMG_SENSOR *psSensor)
{   
    psSensor->pfSensorSetup = OV2640_Setup;
    psSensor->pfSensorSet = OV2640_SetKey;
    psSensor->pfSensorGet = OV2640_GetKey;
    
    psSensor->eResetActiveLevel = GPIO_LEVEL_LOW;
    psSensor->u32ResetTimingMs = OV2640_RESET_DELAY_MS;
    psSensor->ePwdnLevel = GPIO_LEVEL_HIGH;
    psSensor->u32VsyncSkipCnt = 5;
    psSensor->u32SccbWRegDelayMs = 2;
    return 0;
}

uint32_t OV2640_ReadID_impl(S_IMG_SENSOR *psSensor)
{   
    uint32_t u32Ret = 0;
    uint8_t u8PID, u8Ver;
    uint8_t u8MIDL, u8MIDH;
    
    u32Ret = SCCB_Write(OV2640_REG_RA_DLMT, OV2640_DSP_RA_DLMT_BANK_SENSOR);
    
    u32Ret |= SCCB_Read(OV2640_SENSOR_REG_PIDH, &u8PID);
    u32Ret |= SCCB_Read(OV2640_SENSOR_REG_PIDL, &u8Ver);
    u32Ret |= SCCB_Read(OV2640_SENSOR_REG_MIDH, &u8MIDH);
    u32Ret |= SCCB_Read(OV2640_SENSOR_REG_MIDL, &u8MIDL);
    if (u32Ret)
        goto done;
    
    psSensor->u16MID = (u8MIDH << 8) | u8MIDL;
    psSensor->u8PID = u8PID;
    psSensor->u8Ver = u8Ver;
    
    u32Ret = 0;
done:
    return u32Ret;
}



uint32_t OV2640_Setup(S_IMG_SENSOR *psSensor, E_CAMERA_FRAMESIZE eFrameSize, E_CAMERA_PIXEL_FMT ePixelFormat)
{
    uint32_t u32Ret = 1;
    const S_OV2640_ImgWindowSize * const psWindowSize = &g_saOV2640_ImageWindowSize[eFrameSize];
    const S_CAMERA_FRAMESIZE * const psFrmSize = &g_saCamera_FrameSize[eFrameSize];
    
    if (eFrameSize <= CAMERA_FRAMESIZE_CIF)
    {   /* CIF */
        //u32Ret = SCCB_WriteRegs((const uint8_t *)OV2640_CfgCif, sizeof(OV2640_CfgCif)/2);
        u32Ret = SCCB_WriteRegs((const uint8_t *)OV2640_CfgSvga, sizeof(OV2640_CfgSvga)/2);
    }
    else if (eFrameSize <= CAMERA_FRAMESIZE_SVGA)
    {   /* SVGA */
        u32Ret = SCCB_WriteRegs((const uint8_t *)OV2640_CfgSvga, sizeof(OV2640_CfgSvga)/2);
    }
    else if (eFrameSize <= CAMERA_FRAMESIZE_UXGA)
    {   /* UXGA */
        u32Ret = SCCB_WriteRegs((const uint8_t *)OV2640_CfgUxga, sizeof(OV2640_CfgUxga)/2);
    }
    /* else - Invalid */
    if (u32Ret)
        goto done;
    
    //u32Ret = 1;
    osDelay(psSensor->u32SccbWRegDelayMs);
    switch (ePixelFormat)
    {
        case CAMERA_PIXEL_FMT_RGB565:
            u32Ret = 0;
            break;
        case CAMERA_PIXEL_FMT_YUV422:
            u32Ret = 0;
            break;
        case CAMERA_PIXEL_FMT_JPEG:
            u32Ret = SCCB_WriteRegs((const uint8_t *)OV2640_CfgJpeg, sizeof(OV2640_CfgJpeg)/2);
            break;
        default:
            break;
    }
    if (u32Ret)
        goto done;

    u32Ret += OV2640_ImageWindowSet(psWindowSize->u16OffsetX, psWindowSize->u16OffsetY, psWindowSize->u16HSize, psWindowSize->u16VSize);
    u32Ret += OV2640_OutputSizeSet(psFrmSize->u16Width, psFrmSize->u16Height);
    if (u32Ret)
        goto done;
    
    //OV2640_LightModeSet(OV2640_LIGHTMODE_OFFICE);
    u32Ret = 0;
done:
    return u32Ret;
}


uint32_t OV2640_SetKey(S_IMG_SENSOR *psSensor, uint32_t u32SetKey, uint32_t u32SetValue)
{
    return 0;
}
uint32_t OV2640_GetKey(S_IMG_SENSOR *psSensor, uint32_t u32GetKey, uint32_t *pu32GetValue)
{
    return 0;
}




/**
 * @brief Set Image horizontal and vertical windows
 *        Related to HSIZE, VHYX, TEST, XOFFL, YOFFL
 * @param u16OffsetX [in] Image horizontal start
 * @param u16OffsetY [in] Image veritical start
 * @param u16HSize [in] Image windows width, must be multiple of 4
 * @param u16VSize [in] Image windows height, must be multiple of 4
 * @return Set status.
 * @retval 0 - Success
 * @retval 1 - Fail
 */
uint8_t OV2640_ImageWindowSet_impl(uint16_t u16OffsetX, uint16_t u16OffsetY, uint16_t u16HSize, uint16_t u16VSize)
{
    uint8_t u8Reg, u8Ret = 1;
    
    /* Must be multiple of 4 */
    if ((u16HSize % 4) || (u16VSize % 4))
        return 1;
    
    if (SCCB_Write(OV2640_REG_RA_DLMT, OV2640_DSP_RA_DLMT_BANK_DSP))
        goto done;
    u16HSize /= 4; /* real/4 */
    u16VSize /= 4; /* real/4 */
    
    /* VHYX - [2:0]=OFFSET_X[10:8], [3]=H_SIZE[8], [6:4]=OFFSET_Y[10:8], [7]=V_SIZE[8] */
    if (SCCB_Read(OV2640_DSP_REG_VHYX, &u8Reg))
        goto done;
    u8Reg &= ~(OV2640_DSP_VHYX_H_SIZE_1_Msk | OV2640_DSP_VHYX_V_SIZE_1_Msk | OV2640_DSP_VHYX_OFST_X_H_Msk | OV2640_DSP_VHYX_OFST_Y_H_Msk);    /* Clear bit first */
    u8Reg |= ((u16HSize   >> OV2640_IMG_WIN_HSIZE_1_DSP_VHYX_Pos)  << OV2640_DSP_VHYX_H_SIZE_1_Pos) & OV2640_DSP_VHYX_H_SIZE_1_Msk;
    u8Reg |= ((u16VSize   >> OV2640_IMG_WIN_VSIZE_1_DSP_VHYX_Pos)  << OV2640_DSP_VHYX_V_SIZE_1_Pos) & OV2640_DSP_VHYX_V_SIZE_1_Msk;
    u8Reg |= ((u16OffsetX >> OV2640_IMG_WIN_OFFSET_X_DSP_VHYX_Pos) << OV2640_DSP_VHYX_OFST_X_H_Pos) & OV2640_DSP_VHYX_OFST_X_H_Msk;
    u8Reg |= ((u16OffsetY >> OV2640_IMG_WIN_OFFSET_Y_DSP_VHYX_Pos) << OV2640_DSP_VHYX_OFST_Y_H_Pos) & OV2640_DSP_VHYX_OFST_Y_H_Msk;
    if (SCCB_Write(OV2640_DSP_REG_VHYX, u8Reg))
        goto done;
    
    osDelay(u8Ov2640_WriteRegDelayMs);    
    /* TEST - [7]=HSIZE[9] */
    if (SCCB_Read(OV2640_DSP_REG_TEST, &u8Reg))
        goto done;
    u8Reg &= ~OV2640_DSP_TEST_H_SIZE_2_Msk;
    u8Reg |= ((u16HSize >> OV2640_IMG_WIN_HSIZE_2_DSP_TEST_Pos) << OV2640_DSP_TEST_H_SIZE_2_Pos) & OV2640_DSP_TEST_H_SIZE_2_Msk;
    if (SCCB_Write(OV2640_DSP_REG_TEST, u8Reg))
        goto done;
    
    osDelay(u8Ov2640_WriteRegDelayMs);
    /* HSIZE - HSIZE[7:0] (real/4) */
    u8Reg = (u16HSize >> OV2640_IMG_WIN_HSIZE_0_DSP_HSIZE_Pos) & OV2640_DSP_HSIZE_H_SIZE_0_Msk;
    if (SCCB_Write(OV2640_DSP_REG_HSIZE, u8Reg))
        goto done;
        
    osDelay(u8Ov2640_WriteRegDelayMs);
    /* VSIZE - VSIZE[7:0] (real/4) */
    u8Reg = (u16VSize >> OV2640_IMG_WIN_VSIZE_0_DSP_VSIZE_Pos) & OV2640_DSP_VSIZE_V_SIZE_0_Msk;
    if (SCCB_Write(OV2640_DSP_REG_VSIZE, u8Reg))
        goto done;
    
    osDelay(u8Ov2640_WriteRegDelayMs);
    /* XOFFL - OFFSET_X[7:0] */
    u8Reg = (u16OffsetX >> OV2640_IMG_WIN_OFFSET_X_DSP_XOFFL_Pos) & OV2640_DSP_XOFFL_OFST_X_L_Msk;
    if (SCCB_Write(OV2640_DSP_REG_XOFFL, u8Reg))
        goto done;
    
    osDelay(u8Ov2640_WriteRegDelayMs);
    /* YOFFL - OFFSET_Y[7:0] */
    u8Reg = (u16OffsetY >> OV2640_IMG_WIN_OFFSET_Y_DSP_YOFFL_Pos) & OV2640_DSP_YOFFL_OFST_Y_L_Msk;
    if (SCCB_Write(OV2640_DSP_REG_YOFFL, u8Reg))
        goto done;
    
    osDelay(u8Ov2640_WriteRegDelayMs);
    u8Ret = 0;
done:
    return u8Ret;
}

/**
 * @brief Set output horizontal and vertical size
 *        Related to ZMOW, ZMOH, and ZMHH. which are DSP output.
 * @param u16Width [in] Output horizontal size, must be multiple of 4
 * @param u16Height [in] Output veritical size, must be multiple of 4
 * @return Set status.
 * @retval 0 - Success
 * @retval 1 - Fail
 */
uint8_t OV2640_OutputSizeSet_impl(uint16_t u16Width, uint16_t u16Height)
{
    uint8_t u8Reg, u8Ret = 1;
    
    /* Must be multiple of 4 */
    if ((u16Width % 4) || (u16Height % 4))
        return 1;
    
    if (SCCB_Write(OV2640_REG_RA_DLMT, OV2640_DSP_RA_DLMT_BANK_DSP))
        goto done;
    u16Width /= 4;  /* real/4 */
    u16Height /= 4; /* real/4 */
    
    /* ZMHH - [1:0]=OUTW[9:8], [2]=OUTH[8] */    
    if (SCCB_Read(OV2640_DSP_REG_ZMHH, &u8Reg))
        goto done;
    u8Reg &= ~(OV2640_DSP_ZMHH_OUTW_H_Msk | OV2640_DSP_ZMHH_OUTH_H_Msk);    /* Clear bit first */
    u8Reg |= ((u16Width  >> OV2640_OUT_WIDTH_DSP_ZMHH_Pos)  << OV2640_DSP_ZMHH_OUTW_H_Pos) & OV2640_DSP_ZMHH_OUTW_H_Msk;
    u8Reg |= ((u16Height >> OV2640_OUT_HEIGHT_DSP_ZMHH_Pos) << OV2640_DSP_ZMHH_OUTH_H_Pos) & OV2640_DSP_ZMHH_OUTH_H_Msk;
    if (SCCB_Write(OV2640_DSP_REG_ZMHH, u8Reg))
        goto done;
    
    osDelay(u8Ov2640_WriteRegDelayMs);
    /* ZMOW - OUTW[7:0] (real/4) */
    u8Reg = (u16Width >> OV2640_OUT_WIDTH_DSP_ZMOW_Pos) & OV2640_DSP_ZMOW_OUTW_L_Msk;
    if (SCCB_Write(OV2640_DSP_REG_ZMOW, u8Reg))
        goto done;
    
    osDelay(u8Ov2640_WriteRegDelayMs);
    /* ZMOH - OUTH[7:0] (real/4) */
    u8Reg = (u16Height >> OV2640_OUT_HEIGHT_DSP_ZMOH_Pos) & OV2640_DSP_ZMOH_OUTH_L_Msk;
    if (SCCB_Write(OV2640_DSP_REG_ZMOH, u8Reg))
        goto done;

    osDelay(u8Ov2640_WriteRegDelayMs);
    u8Ret = 0;
done:
    return u8Ret;
}
void OV2640_LightModeSet_impl(E_OV2640_LIGHTMODE eMode)
{
    if (eMode >= OV2640_LIGHTMODE_AUTO)
    {
        SCCB_Write(OV2640_REG_RA_DLMT, OV2640_DSP_RA_DLMT_BANK_DSP);
        SCCB_Write(0xC7, 0x00);
        return;
    }
    
    for (uint8_t u8Idx=0; u8Idx<OV2640_CFG_LIGHTMODE_REG_NUM; u8Idx++)
    {
        uint8_t u8RegAddr = OV2640_u8aCfgLightModeRegAddr[u8Idx];
        uint8_t u8RegValue = OV2640_u8aCfgLightModeRegValue[eMode][u8Idx];
        SCCB_Write(u8RegAddr, u8RegValue);
    }
}

void OV2640_SaturationSet_impl(E_OV2640_SATURATION_LEVEL eLevel)
{
    SCCB_Write(OV2640_REG_RA_DLMT, OV2640_DSP_RA_DLMT_BANK_DSP);
    SCCB_Write(OV2640_DSP_REG_BPADDR, 0x00);
    SCCB_Write(OV2640_DSP_REG_BPDATA, 0x02);
    SCCB_Write(OV2640_DSP_REG_BPADDR, 0x03);
    SCCB_Write(OV2640_DSP_REG_BPDATA, eLevel);
    SCCB_Write(OV2640_DSP_REG_BPDATA, eLevel);
}


void OV2640_BrightnessSet_impl(E_OV2640_BRIGHTNESS_LEVEL eLevel)
{
    SCCB_Write(OV2640_REG_RA_DLMT, OV2640_DSP_RA_DLMT_BANK_DSP);
    SCCB_Write(OV2640_DSP_REG_BPADDR, 0x00);
    SCCB_Write(OV2640_DSP_REG_BPDATA, 0x04);
    SCCB_Write(OV2640_DSP_REG_BPADDR, 0x09);
    SCCB_Write(OV2640_DSP_REG_BPDATA, eLevel);
    SCCB_Write(OV2640_DSP_REG_BPDATA, 0x00);
}


void OV2640_ContrastSet_impl(E_OV2640_CONTRAST_LEVEL eLevel)
{
    SCCB_Write(OV2640_REG_RA_DLMT, OV2640_DSP_RA_DLMT_BANK_DSP);
    SCCB_Write(OV2640_DSP_REG_BPADDR, 0x00);
    SCCB_Write(OV2640_DSP_REG_BPDATA, 0x04);
    SCCB_Write(OV2640_DSP_REG_BPADDR, 0x07);
    SCCB_Write(OV2640_DSP_REG_BPDATA, 0x20);
    SCCB_Write(OV2640_DSP_REG_BPDATA, OV2640_u8aCfgContrastRegValue[eLevel][0]);
    SCCB_Write(OV2640_DSP_REG_BPDATA, OV2640_u8aCfgContrastRegValue[eLevel][1]);
    SCCB_Write(OV2640_DSP_REG_BPDATA, 0x06);
}


void OV2640_SpecialEffectSet_impl(E_OV2640_SPECIAL_EFFECTS_MODE eMode)
{
    for (uint8_t u8Idx=0; u8Idx<OV2640_CFG_SE_REG_NUM; u8Idx++)
    {
        uint8_t u8RegAddr = OV2640_u8aCfgSpecialEffectRegAddr[u8Idx];
        uint8_t u8RegValue = OV2640_u8aCfgSpecialEffectRegValue[eMode][u8Idx];
        SCCB_Write(u8RegAddr, u8RegValue);
    }
}

void OV2640_YuvSeqSet_impl(E_OV2640_YUV_SEQ eSeq)
{
    uint8_t u8ByteSwap, u8VFirst;
    uint8_t u8Reg = 0;
    u8ByteSwap = eSeq & OV2640_DSP_IMAGE_MODE_BYTE_SWAP_Msk;
    u8VFirst = eSeq & OV2640_DSP_CTRL0_VFIRST_Msk;
    
    SCCB_Write(OV2640_REG_RA_DLMT, OV2640_DSP_RA_DLMT_BANK_DSP);
    SCCB_Read(OV2640_DSP_REG_IMAGE_MODE, &u8Reg);
    if ((u8Reg & OV2640_DSP_IMAGE_MODE_BYTE_SWAP_Msk) ^ u8ByteSwap)
    {
        u8Reg = (u8Reg & ~OV2640_DSP_IMAGE_MODE_BYTE_SWAP_Msk) | u8ByteSwap;
        SCCB_Write(OV2640_DSP_REG_IMAGE_MODE, u8Reg);
    }
        
    SCCB_Read(OV2640_DSP_REG_CTRL0, &u8Reg);
    if ((u8Reg & OV2640_DSP_CTRL0_VFIRST_Msk) ^ u8VFirst)
    {
        u8Reg = (u8Reg & ~OV2640_DSP_CTRL0_VFIRST_Msk) | u8VFirst;
        SCCB_Write(OV2640_DSP_REG_CTRL0, u8Reg);
    }
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */


#if _OV2640_DUMP_REG_EN
#define REG_ARRAY_LEN   255
uint8_t u8aValues[REG_ARRAY_LEN];
uint8_t OV2640_DumpReg(void)
{
    uint8_t u8Reg;
    uint8_t u8Ret;
    
    u8Ret = SCCB_Write(OV2640_REG_RA_DLMT, OV2640_DSP_RA_DLMT_BANK_DSP);
    tracer_drct_printf("\nDSP Bank\n");
    tracer_drct_printf(" 0 1 2 3 4 5 6 7 8 9 A B C D E F");
    for (u8Reg = 0; u8Reg < REG_ARRAY_LEN; u8Reg++)
    {
        u8Ret = SCCB_Read(u8Reg, &u8aValues[u8Reg]);
        if (u8Ret)
            goto fail;
    }
    for (u8Reg = 0; u8Reg < REG_ARRAY_LEN; u8Reg++)
    {
        if (u8Reg % 16 == 0)
            tracer_drct_printf("\n%02X: ", u8Reg);
        tracer_drct_printf("%02X ", u8aValues[u8Reg]);
    }
    
    
    u8Ret = SCCB_Write(OV2640_REG_RA_DLMT, OV2640_DSP_RA_DLMT_BANK_SENSOR);
    tracer_drct_printf("\n\nSensor Bank\n");
    tracer_drct_printf(" 0 1 2 3 4 5 6 7 8 9 A B C D E F");
    for (u8Reg = 0; u8Reg < REG_ARRAY_LEN; u8Reg++)
    {
        u8Ret = SCCB_Read(u8Reg, &u8aValues[u8Reg]);
        if (u8Ret)
            goto fail;
    }
    for (u8Reg = 0; u8Reg < REG_ARRAY_LEN; u8Reg++)
    {
        if (u8Reg % 16 == 0)
            tracer_drct_printf("\n%02X: ", u8Reg);
        tracer_drct_printf("%02X ", u8aValues[u8Reg]);
    }
    return 0;
fail:
    tracer_drct_printf("Read fail at reg%02X\n", u8Reg);
    return 0;
}
#endif /* _OV2640_DUMP_REG_EN */
