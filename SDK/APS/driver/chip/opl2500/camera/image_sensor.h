/* *****************************************************************************
 *  Copyright 2017 - 2021, Opulinks Technology Ltd.
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
 *  @file image_sensor.h 
 * 
 *  @brief Image Sensor API
 *  
 *******************************************************************************/

#ifndef _IMAGE_SENSOR_H_
#define _IMAGE_SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <stdint.h>
#include "hal_gpio.h"

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
typedef enum
{
    CAMERA_POLARITY_POSITIVE=0,     /* Receive data when SYNC is high */
    CAMERA_POLARITY_NEGATIVE=1,     /* Receive data when SYNC is low */
    CAMERA_POLARITY_NUM,
} E_CAMERA_SYNC_POLARITY;

 
typedef enum
{
    CAMERA_PIXEL_FMT_RGB565,
    CAMERA_PIXEL_FMT_YUV422,
    CAMERA_PIXEL_FMT_JPEG,
    CAMERA_PIXEL_FMT_RAW,
} E_CAMERA_PIXEL_FMT;


typedef enum 
{
    CAMERA_FRAMESIZE_SQCIF,   /* 128  x 96   */
    CAMERA_FRAMESIZE_QQCIF,   /* 160  x 120  */
    CAMERA_FRAMESIZE_QCIF,    /* 176  x 144  */
    CAMERA_FRAMESIZE_QVGA,    /* 320  x 240  */
    CAMERA_FRAMESIZE_CIF,     /* 352  x 288  */
    CAMERA_FRAMESIZE_HVGA,    /* 480  x 320  */
    CAMERA_FRAMESIZE_VGA,     /* 640  x 480  */
    CAMERA_FRAMESIZE_SVGA,    /* 800  x 600  */
    CAMERA_FRAMESIZE_XGA,     /* 1024 x 768  */
    CAMERA_FRAMESIZE_HD,      /* 1280 x 720  */
    CAMERA_FRAMESIZE_SXGA,    /* 1280 x 1024 */
    CAMERA_FRAMESIZE_UXGA,    /* 1600 x 1200 */
    CAMERA_FRAMESIZE_NUM,
} E_CAMERA_FRAMESIZE;


typedef enum
{
    CAMERA_LIGHTMODE_AUTO=0,
    CAMERA_LIGHTMODE_SUNNY,
    CAMERA_LIGHTMODE_CLOUDY,
    CAMERA_LIGHTMODE_OFFICE,
    CAMERA_LIGHTMODE_HOME,
} E_CAMERA_LIGHTMODE;

typedef enum
{
    CAMERA_SE_ANTIQUE,
    CAMERA_SE_BLUISH,
    CAMERA_SE_GREENISH,
    CAMERA_SE_REDDISH,
    CAMERA_SE_BW,
    CAMERA_SE_NEGATIVE,
    CAMERA_SE_BW_NEGATIVE,
    CAMERA_SE_NORMAL,
} E_CAMERA_SPECIAL_EFFECTS_MODE;


typedef struct
{
    uint16_t u16Width;
    uint16_t u16Height;
} S_CAMERA_FRAMESIZE;




/* Supported image sensor list */
typedef enum
{
    IMAGE_SENSOR_OV2640 = 0,
    IMAGE_SENSOR_INVALID,
} E_IMGSENSOR_DEVICES;


typedef struct img_sensor S_IMG_SENSOR;
typedef struct img_sensor
{
    /* ID version */
    uint16_t u16MID;
    uint8_t u8PID;
    uint8_t u8Ver;
    
    uint32_t u32SccbWRegDelayMs;
    E_GpioIdx_t ePinCameraReset;
    E_GpioLevel_t eResetActiveLevel;    /* Which level will reset sensor */
    uint32_t u32ResetTimingMs;          /* Pull reset pin active level for how many ms */
    E_GpioIdx_t ePinCameraPwdn;
    E_GpioLevel_t ePwdnLevel;           /* Which level will cause power down */
    uint32_t u32VsyncSkipCnt;           /* Skip how many picture after configuration */
    
    /* Functions */
    uint32_t (*pfSensorSetup)(S_IMG_SENSOR *psSensor, E_CAMERA_FRAMESIZE eFrameSize, E_CAMERA_PIXEL_FMT ePixelFormat);
    uint32_t (*pfSensorSet)(S_IMG_SENSOR *psSensor, uint32_t u32SetKey, uint32_t u32SetValue);
    uint32_t (*pfSensorGet)(S_IMG_SENSOR *psSensor, uint32_t u32GetKey, uint32_t *pu32GetValue);
} S_IMG_SENSOR;


typedef uint32_t (*T_ImgSensor_Init)(S_IMG_SENSOR *psSensor, uint32_t u32Device, E_GpioIdx_t ePinReset, E_GpioIdx_t ePinPwdn);
typedef void (*T_ImgSensor_DeInit)(S_IMG_SENSOR *psSensor);
typedef void (*T_ImgSensor_Reset)(S_IMG_SENSOR *psSensor);
typedef void (*T_ImgSensor_Power)(S_IMG_SENSOR *psSensor, E_FUNC_ST eFunc);


/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */

extern T_ImgSensor_Init ImgSensor_Init;
extern T_ImgSensor_DeInit ImgSensor_DeInit;
extern T_ImgSensor_Reset ImgSensor_Reset;
extern T_ImgSensor_Power ImgSensor_Power;

extern const S_CAMERA_FRAMESIZE g_saCamera_FrameSize[CAMERA_FRAMESIZE_NUM];
/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
 

 
uint32_t ImgSensor_Init_impl(S_IMG_SENSOR *psSensor, uint32_t u32Device, E_GpioIdx_t ePinReset, E_GpioIdx_t ePinPwdn);
void ImgSensor_DeInit_impl(S_IMG_SENSOR *psSensor);
void ImgSensor_Reset_impl(S_IMG_SENSOR *psSensor);
void ImgSensor_Power_impl(S_IMG_SENSOR *psSensor, E_FUNC_ST eFunc);

#ifdef __cplusplus
}
#endif
#endif  /* _IMAGE_SENSOR_H_ */
