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
 *  @file hal_gpio_patch.c
 * 
 *  @brief GPIO patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
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
void Hal_Gpio_PatchInit(void)
{
    Hal_Gpio_Direction = Hal_Gpio_Direction_patch;
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */
/**
 * @brief Hal_Gpio_Direction
 *        set the GPIO direction
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @param eDirection[in] the GPIO direction
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Gpio_Direction_patch(E_GpioIdx_t eIdx, E_GpioDirection_t eDirection)
{   
    if (eIdx >= GPIO_IDX_NUM)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    if (eIdx >= GPIO_SIP_START_IDX)
    {   /* SiP IO */
        GPIO_CTRL_SIP_IO_DIR_SET(eIdx, (eDirection==GPIO_INPUT)?1:0);
    }
    
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    REG_BB_WRITE(PIN->PD_DIR, eIdx, (eDirection==GPIO_INPUT)?1:0);
    return RESULT_SUCCESS;
}
