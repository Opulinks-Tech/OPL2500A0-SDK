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
 *  @file image_sensor.c
 * 
 *  @brief Image Sensor API
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "cmsis_os.h"
#include "image_sensor.h"
#include "sccb.h"
#include "ov2640.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */


#define SENSOR_RESET_TIMING_MS 2
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

T_ImgSensor_Init ImgSensor_Init = ImgSensor_Init_impl;
T_ImgSensor_DeInit ImgSensor_DeInit = ImgSensor_DeInit_impl;
T_ImgSensor_Reset ImgSensor_Reset = ImgSensor_Reset_impl;
T_ImgSensor_Power ImgSensor_Power = ImgSensor_Power_impl;


const S_CAMERA_FRAMESIZE g_saCamera_FrameSize[CAMERA_FRAMESIZE_NUM] = {
    /* Output Width,  OutputHeight */
    {           128,            96}, /* SQCIF  128X96    */
    {           160,           120}, /* QQVGA  160x120   */
    {           176,           144}, /* QCIF   176x144   */
    {           320,           240}, /* QVGA   320x240   */
    {           352,           288}, /* CIF    352x288   */
    {           480,           320}, /* HVGA   480x320   */
    {           640,           480}, /* VGA    640x480   */
    {           800,           600}, /* SVGA   800x600   */
    {          1024,           768}, /* XGA    1024x768  */
    {          1280,           720}, /* HD     1280x720  */
    {          1280,          1024}, /* SXGA   1280x1024 */
    {          1600,          1200}, /* UXGA   1600x1200 */
};




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

uint32_t ImgSensor_Init_impl(S_IMG_SENSOR *psSensor, uint32_t u32Device, E_GpioIdx_t ePinReset, E_GpioIdx_t ePinPwdn)
{
    uint32_t u32Ret = 1;
    
    psSensor->ePinCameraReset = ePinReset;
    psSensor->ePinCameraPwdn = ePinPwdn;
    
    switch (u32Device)
    {
        case IMAGE_SENSOR_OV2640:
            OV2640_Init(psSensor);
            SCCB_Init(OV2640_SCCB_ADDRESS, I2C_SPEED_FAST, psSensor->u32SccbWRegDelayMs);
            ImgSensor_Power(psSensor, ENABLE);
            ImgSensor_Reset(psSensor);
            u32Ret = OV2640_ReadID(psSensor);
            break;
        default:
            break;
    }
    return u32Ret;
}


void ImgSensor_DeInit_impl(S_IMG_SENSOR *psSensor)
{
    SCCB_DeInit();
    ImgSensor_Power(psSensor, DISABLE);
}




void ImgSensor_Reset_impl(S_IMG_SENSOR *psSensor)
{
    Hal_Gpio_Output(psSensor->ePinCameraReset, psSensor->eResetActiveLevel);
    osDelay(psSensor->u32ResetTimingMs);
    Hal_Gpio_Output(psSensor->ePinCameraReset, (E_GpioLevel_t)!psSensor->eResetActiveLevel);
    osDelay(psSensor->u32ResetTimingMs);
}

void ImgSensor_Power_impl(S_IMG_SENSOR *psSensor, E_FUNC_ST eFunc)
{
    if (eFunc == ENABLE)
        Hal_Gpio_Output(psSensor->ePinCameraPwdn, psSensor->ePwdnLevel);
    else
        Hal_Gpio_Output(psSensor->ePinCameraPwdn, (E_GpioLevel_t)!psSensor->ePwdnLevel);
    
}

/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

