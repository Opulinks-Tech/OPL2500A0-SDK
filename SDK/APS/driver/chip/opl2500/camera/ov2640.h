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
 *  @file ov2640.h 
 * 
 *  @brief OV2640 control API
 *
 *  
 ******************************************************************************/

#ifndef _OV2640_H_
#define _OV2640_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
#include "ov2640_def.h"
#include "image_sensor.h"

/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

#define _OV2640_DUMP_REG_EN     1
    
#define OV2640_RESET_DELAY_MS       1
    
#define OV2640_SCCB_SLAVE_ADDR      
    
#define OV2640_JPEG_QUALITY_MIN     0
#define OV2640_JPEG_QUALITY_MAX     63
#define OV2640_JPEG_QUALITY_RANGE   (OV2640_JPEG_QUALITY_MAX - OV2640_JPEG_QUALITY_MIN + 1)


    
/* From register mask 0 ~ 63, convert to level 100 ~ 1 */
#define OV2640_JPEG_QUALITY_LEVEL_TO_REG(lvl)       (DIV_ROUND( ((CAM_JPEG_QUALITY_MAX - (lvl)) * OV2640_JPEG_QUALITY_RANGE), CAM_JPEG_QUALITY_RANGE ) + OV2640_JPEG_QUALITY_MIN)

/* From level 100 ~ 1, convert to register mask 0 ~ 63 */
#define OV2640_JPEG_QUALITY_REG_TO_LEVEL(reg)       (DIV_ROUND( ((OV2640_JPEG_QUALITY_MAX - (reg)) * CAM_JPEG_QUALITY_RANGE), OV2640_JPEG_QUALITY_RANGE ) + CAM_JPEG_QUALITY_MIN)

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

typedef enum
{
    OV2640_LIGHTMODE_SUNNY,
    OV2640_LIGHTMODE_CLOUDY,
    OV2640_LIGHTMODE_OFFICE,
    OV2640_LIGHTMODE_HOME,
    OV2640_LIGHTMODE_AUTO,
} E_OV2640_LIGHTMODE;

typedef enum
{
    OV2640_SATURATION_PLUS2 = 0x68,
    OV2640_SATURATION_PLUS1 = 0x58,
    OV2640_SATURATION_ZERO  = 0x48,
    OV2640_SATURATION_MINUS1= 0x38,
    OV2640_SATURATION_MINUS2= 0x28,
} E_OV2640_SATURATION_LEVEL;

typedef enum
{
    OV2640_BRIGHTNESS_PLUS2 = 0x40,
    OV2640_BRIGHTNESS_PLUS1 = 0x30,
    OV2640_BRIGHTNESS_ZERO  = 0x20,
    OV2640_BRIGHTNESS_MINUS1= 0x10,
    OV2640_BRIGHTNESS_MINUS2= 0x00,
} E_OV2640_BRIGHTNESS_LEVEL;

typedef enum
{
    OV2640_CONTRAST_PLUS2,
    OV2640_CONTRAST_PLUS1,
    OV2640_CONTRAST_ZERO,
    OV2640_CONTRAST_MINUS1,
    OV2640_CONTRAST_MINUS2,
} E_OV2640_CONTRAST_LEVEL;

typedef enum
{
    OV2640_SE_ANTIQUE,
    OV2640_SE_BLUISH,
    OV2640_SE_GREENISH,
    OV2640_SE_REDDISH,
    OV2640_SE_BW,
    OV2640_SE_NEGATIVE,
    OV2640_SE_BW_NEGATIVE,
    OV2640_SE_NORMAL,
} E_OV2640_SPECIAL_EFFECTS_MODE;


typedef enum
{
    OV2640_RES_UXGA = OV2640_SENSOR_COM7_RES_SEL_UXGA << OV2640_SENSOR_COM7_RES_SEL_Pos,
    OV2640_RES_CIF  = OV2640_SENSOR_COM7_RES_SEL_CIF << OV2640_SENSOR_COM7_RES_SEL_Pos,
    OV2640_RES_SVGA = OV2640_SENSOR_COM7_RES_SEL_SVGA << OV2640_SENSOR_COM7_RES_SEL_Pos,
} E_OV2640_RESOLUTION;


typedef enum
{
    E_OV2640_YUV_SEQ_YUYV = (OV2640_CTRL0_VFIRST_DISABLE << OV2640_DSP_CTRL0_VFIRST_Pos) | (OV2640_DSP_IMAGE_MODE_BYTE_SWAP_MSB_FIRST << OV2640_DSP_IMAGE_MODE_BYTE_SWAP_Pos),
    E_OV2640_YUV_SEQ_YVYU = (OV2640_CTRL0_VFIRST_ENABLE << OV2640_DSP_CTRL0_VFIRST_Pos)  | (OV2640_DSP_IMAGE_MODE_BYTE_SWAP_MSB_FIRST << OV2640_DSP_IMAGE_MODE_BYTE_SWAP_Pos),
    E_OV2640_YUV_SEQ_VYUY = (OV2640_CTRL0_VFIRST_ENABLE << OV2640_DSP_CTRL0_VFIRST_Pos)  | (OV2640_DSP_IMAGE_MODE_BYTE_SWAP_LSB_FIRST << OV2640_DSP_IMAGE_MODE_BYTE_SWAP_Pos),
    E_OV2640_YUV_SEQ_UYVY = (OV2640_CTRL0_VFIRST_DISABLE << OV2640_DSP_CTRL0_VFIRST_Pos) | (OV2640_DSP_IMAGE_MODE_BYTE_SWAP_LSB_FIRST << OV2640_DSP_IMAGE_MODE_BYTE_SWAP_Pos),
} E_OV2640_YUV_SEQ;



typedef struct
{
    uint16_t u16OffsetX;    /* Offset from horizontal zero */
    uint16_t u16OffsetY;    /* Offset from veritical zero */
    uint16_t u16HSize;      /* Image windows width in sensor window, DSP input */
    uint16_t u16VSize;      /* Image windows width in sensor window, DSP input */
} S_OV2640_ImgWindowSize;





typedef uint32_t (*T_OV2640_Init)(S_IMG_SENSOR *psSensor);
typedef uint32_t (*T_OV2640_ReadID)(S_IMG_SENSOR *psSensor);
/* Windows and size configuration */
typedef uint8_t (*T_OV2640_ImageWindowSet)(uint16_t u16OffsetX, uint16_t u16OffsetY, uint16_t u16HSize, uint16_t u16VSize);
typedef uint8_t (*T_OV2640_OutputSizeSet)(uint16_t u16HSize, uint16_t u16VSize);
/* Effect */
typedef void (*T_OV2640_LightModeSet)(E_OV2640_LIGHTMODE eMode);
typedef void (*T_OV2640_SaturationSet)(E_OV2640_SATURATION_LEVEL eLevel);
typedef void (*T_OV2640_BrightnessSet)(E_OV2640_BRIGHTNESS_LEVEL eLevel);
typedef void (*T_OV2640_ContrastSet)(E_OV2640_CONTRAST_LEVEL eLevel);
typedef void (*T_OV2640_SpecialEffectSet)(E_OV2640_SPECIAL_EFFECTS_MODE eMode);
typedef void (*T_OV2640_YuvSeqSet)(E_OV2640_YUV_SEQ eSeq);


/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

extern T_OV2640_Init OV2640_Init;
extern T_OV2640_ReadID OV2640_ReadID;
/* Windows and size configuration */
extern T_OV2640_ImageWindowSet OV2640_ImageWindowSet;
extern T_OV2640_OutputSizeSet OV2640_OutputSizeSet;

/* Effect */
extern T_OV2640_LightModeSet OV2640_LightModeSet;
extern T_OV2640_SaturationSet OV2640_SaturationSet;
extern T_OV2640_BrightnessSet OV2640_BrightnessSet;
extern T_OV2640_ContrastSet OV2640_ContrastSet;
extern T_OV2640_SpecialEffectSet OV2640_SpecialEffectSet;
extern T_OV2640_YuvSeqSet OV2640_YuvSeqSet;


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
 

uint32_t OV2640_Setup(S_IMG_SENSOR *psSensor, E_CAMERA_FRAMESIZE eFrameSize, E_CAMERA_PIXEL_FMT ePixelFormat);
uint32_t OV2640_SetKey(S_IMG_SENSOR *psSensor, uint32_t u32SetKey, uint32_t u32SetValue);
uint32_t OV2640_GetKey(S_IMG_SENSOR *psSensor, uint32_t u32GetKey, uint32_t *pu32GetValue);

uint32_t OV2640_Init_impl(S_IMG_SENSOR *psSensor);
uint32_t OV2640_ReadID_impl(S_IMG_SENSOR *psSensor);
/* Windows and size configuration */
uint8_t OV2640_ImageWindowSet_impl(uint16_t u16OffsetX, uint16_t u16OffsetY, uint16_t u16HSize, uint16_t u16VSize);
uint8_t OV2640_OutputSizeSet_impl(uint16_t u16HSize, uint16_t u16VSize);

/* Effect */
void OV2640_LightModeSet_impl(E_OV2640_LIGHTMODE eMode);
void OV2640_SaturationSet_impl(E_OV2640_SATURATION_LEVEL eLevel);
void OV2640_BrightnessSet_impl(E_OV2640_BRIGHTNESS_LEVEL eLevel);
void OV2640_ContrastSet_impl(E_OV2640_CONTRAST_LEVEL eLevel);
void OV2640_SpecialEffectSet_impl(E_OV2640_SPECIAL_EFFECTS_MODE eMode);
void OV2640_YuvSeqSet_impl(E_OV2640_YUV_SEQ eSeq);



void OV2640_ShowBuff(void);


#if _OV2640_DUMP_REG_EN
uint8_t OV2640_DumpReg(void);
#else
#define OV2640_DumpReg()
#endif /* _OV2640_DUMP_REG_EN */

#ifdef __cplusplus
}
#endif
#endif  /* _OV2640_H_ */
