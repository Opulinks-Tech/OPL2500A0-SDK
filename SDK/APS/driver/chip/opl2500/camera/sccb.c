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
 * @file sccb.c
 * 
 * @brief SCCB interface
 * 
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include "sccb.h"
#include "hal_i2c.h"
#include "cmsis_os.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */
 
#define _EN_SCCB_WRITE_CHECK        0


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

T_SCCB_WriteRegs SCCB_WriteRegs = SCCB_WriteRegs_impl;
T_SCCB_Read SCCB_Read = SCCB_Read_impl;
T_SCCB_Write SCCB_Write = SCCB_Write_impl;
T_SCCB_Init SCCB_Init = SCCB_Init_impl;
T_SCCB_DeInit SCCB_DeInit = SCCB_DeInit_impl;

/*
 *************************************************************************
 *                          Private Variables
 *************************************************************************
 */

uint32_t u32SccbWriteCheck = _EN_SCCB_WRITE_CHECK;
uint32_t g_Sccb_u32WRegDelayMs = 0;
 



/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
/*
  0=I2C_SPEED_STANDARD, 100K
  1=I2C_SPEED_FAST      400K
*/
void SCCB_Init_impl(uint8_t u8SlaveAddr, E_I2cSpeed_t eSpeed, uint32_t u32WriteRegsDelayMs)
{
    Hal_I2c_MasterInit(I2C_07BIT, eSpeed);
    Hal_I2c_TargetAddrSet(u8SlaveAddr);
    g_Sccb_u32WRegDelayMs = u32WriteRegsDelayMs;
}

void SCCB_DeInit_impl(void)
{
    Hal_I2c_DeInit();
}

/**
 * @brief SCCB write register
 * @param u8Reg[in] Register address to be written
 * @param u8Value[in] The value to be written to register
 * @return Writing status
 * @retval 1: Fail
 * @retval 0: Success
 */
uint8_t SCCB_Write_impl(uint8_t u8Reg, uint8_t u8Value)
{
    uint8_t u8aCmd[2] = {u8Reg, u8Value};
    uint8_t u8Ret;
    u8Ret = Hal_I2c_MasterTrasmit(u8aCmd, 2, 1);
    if (I2C->CLR_TX_ABRT & I2C_CLR_TX_ABRT_CLR_TX_ABRT)
    {
        return 2;
    }
    if (u8Ret)
        return u8Ret;
    
    if (u32SccbWriteCheck)
    {
        uint8_t u8ReadBack = 0xFF;
        u8Ret = SCCB_Read(u8Reg, &u8ReadBack);
        tracer_drct_printf("[%02X]=%02X (%02X)", u8Reg, u8ReadBack, u8Value);
        if (u8Ret || (u8Value != u8ReadBack))
        {
            tracer_drct_printf("!\n");
        }
        else
        {
            tracer_drct_printf("\n");
        }
    }
    else
        return 0;
    
    return u8Ret;
}
/*
 * @brief SCCB read register
 * @param u8Reg[in] Register address to be written
 * @param pu8Value[out] The pointer to store reading value
 * @return Writing status
 * @retval 1: Fail
 * @retval 0: Success
*/
uint8_t SCCB_Read_impl(uint8_t u8Reg, uint8_t *pu8Value)
{
    uint8_t u8Ret = 1;

    u8Ret = Hal_I2c_MasterTrasmit(&u8Reg, 1, 1);
    if (I2C->CLR_TX_ABRT & I2C_CLR_TX_ABRT_CLR_TX_ABRT)
    {
        return 2;
    }
    if (u8Ret)
        return u8Ret;
    u8Ret = Hal_I2c_MasterReceive(pu8Value, 1, 1);    
    if (I2C->CLR_TX_ABRT & I2C_CLR_TX_ABRT_CLR_TX_ABRT)
    {
        return 2;
    }
    return u8Ret;
}


uint8_t SCCB_WriteRegs_impl(const uint8_t *pu8aRegPairss, uint32_t u32NumPairs)
{
    uint32_t u32Idx;
    uint8_t u8Ret;
    
    for (u32Idx=0; u32Idx<u32NumPairs; u32Idx++, pu8aRegPairss+=2)
    {
        if (g_Sccb_u32WRegDelayMs)
            osDelay(g_Sccb_u32WRegDelayMs);
        u8Ret = SCCB_Write(*pu8aRegPairss, *(pu8aRegPairss+1));
        if (u8Ret)
            break;
    }
    
    return u8Ret;
}


/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

