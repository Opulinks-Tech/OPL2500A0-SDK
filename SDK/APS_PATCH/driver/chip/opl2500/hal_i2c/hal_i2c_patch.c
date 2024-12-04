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
 *  @file hal_i2c_patch.c
 * 
 *  @brief I2C module patch
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "hal_i2c.h"
#include "hal_system.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

#define I2C_STANDARD_SCLK_LOW_MIN   4700 //ns
#define I2C_STANDARD_SCLK_HIGH_MIN  4000 //ns
#define I2C_STANDARD_SPIKE_LEN      2
#define I2C_FAST_SCLK_LOW_MIN       1300 //ns
#define I2C_FAST_SCLK_HIGH_MIN      600  //ns
#define I2C_FAST_SPIKE_LEN          2
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
uint32_t Hal_I2c_SpeedSet_patch(E_I2cSpeed_t eSpeed);

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

void Hal_I2c_PatchInit(void)
{
    Hal_I2c_SpeedSet = Hal_I2c_SpeedSet_patch;
    
}



/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */


uint32_t Hal_I2c_SpeedSet_patch(E_I2cSpeed_t eSpeed)
{
    uint32_t u32EnStatus = 0;
    uint32_t u32Temp = 0;
    uint32_t u32Pclk;
    double dTotalCnt;
    
    Hal_Sys_ApsClockGet(APS_CLK_GRP_PCLK, &u32Pclk);
    
    // Disable before set
    u32EnStatus = I2C->ENABLE;
    if( u32EnStatus & I2C_ENABLE_ENABLE_Msk )
        _Hal_I2c_Eanble_impl(0);

    u32Temp = I2C->CON & ~(I2C_CON_SPEED_Msk);
    switch(eSpeed)
    {
        case I2C_SPEED_STANDARD:
            dTotalCnt = u32Pclk / I2C_CLK_STD;
            I2C->CON = u32Temp | I2C_CON_SPEED_STD;
            I2C->SS_SCL_HCNT = ((double)I2C_STANDARD_SCLK_HIGH_MIN / (I2C_STANDARD_SCLK_LOW_MIN + I2C_STANDARD_SCLK_HIGH_MIN)) * dTotalCnt;
            I2C->SS_SCL_LCNT = ((double)I2C_STANDARD_SCLK_LOW_MIN  / (I2C_STANDARD_SCLK_LOW_MIN + I2C_STANDARD_SCLK_HIGH_MIN)) * dTotalCnt;
            I2C->FS_SPKLEN   = I2C_STANDARD_SPIKE_LEN;
            break;
        case I2C_SPEED_FAST:
            dTotalCnt = u32Pclk / I2C_CLK_FAST;
            I2C->CON = u32Temp | I2C_CON_SPEED_FAST;
            I2C->FS_SCL_HCNT = ((double)I2C_FAST_SCLK_HIGH_MIN / (I2C_FAST_SCLK_LOW_MIN + I2C_FAST_SCLK_HIGH_MIN)) * dTotalCnt;
            I2C->FS_SCL_LCNT = ((double)I2C_FAST_SCLK_LOW_MIN  / (I2C_FAST_SCLK_LOW_MIN + I2C_FAST_SCLK_HIGH_MIN)) * dTotalCnt;
            I2C->FS_SPKLEN   = I2C_FAST_SPIKE_LEN;
            break;
        //case I2C_SPEED_HIGH:
        default:
            if( u32EnStatus & I2C_ENABLE_ENABLE_Msk )
                _Hal_I2c_Eanble_impl(1);
            return 1;
    }
    
    // Enable if need
    if( u32EnStatus & I2C_ENABLE_ENABLE_Msk )
        _Hal_I2c_Eanble_impl(1);
    return 0;
}
