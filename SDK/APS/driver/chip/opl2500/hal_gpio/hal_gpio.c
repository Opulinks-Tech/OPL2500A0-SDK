/******************************************************************************
*  Copyright 2017 - 2018, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2018
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  hal_gpio.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This include file defines the proto-types of GPIO functions
*
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File 
#include "opulinks.h"
#include "hal_gpio.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable

/** The MAX index(excluded) for current IO package */
uint32_t g_u32Hal_IoPkgNum;
S_PIN_MUX_CACHE g_sPinmuxCache;
S_SLEEP_IO_CFG *g_psHal_Gpio_SleepIoCfg = NULL;

// GPIO module
T_Gpio_CallBack g_taHalVicGpioCallBack[GPIO_IDX_NUM] = {0, };

T_Hal_Gpio_IntInit          Hal_Gpio_IntInit            = Hal_Gpio_IntInit_impl;
T_Hal_Gpio_IntDeInit        Hal_Gpio_IntDeInit          = Hal_Gpio_IntDeInit_impl;
T_Hal_Gpio_IntEn            Hal_Gpio_IntEn              = Hal_Gpio_IntEn_impl;
T_Hal_Gpio_IntClear         Hal_Gpio_IntClear           = Hal_Gpio_IntClear_impl;
T_Hal_Gpio_IntMask          Hal_Gpio_IntMask            = Hal_Gpio_IntMask_impl;
T_Hal_Gpio_IntTypeSel       Hal_Gpio_IntTypeSel         = Hal_Gpio_IntTypeSel_impl;
T_Hal_Gpio_IntInv           Hal_Gpio_IntInv             = Hal_Gpio_IntInv_impl;
T_Hal_Gpio_IntStatRead      Hal_Gpio_IntStatRead        = Hal_Gpio_IntStatRead_impl;
T_Hal_Gpio_CallBackFuncSet  Hal_Gpio_CallBackFuncSet    = Hal_Gpio_CallBackFuncSet_impl;

T_Hal_Gpio_VicInit          Hal_Gpio_VicInit            = Hal_Gpio_VicInit_impl;
T_Hal_Gpio_Input            Hal_Gpio_Input              = Hal_Gpio_Input_impl;
T_Hal_Gpio_Output           Hal_Gpio_Output             = Hal_Gpio_Output_impl;
T_Hal_Gpio_Direction        Hal_Gpio_Direction          = Hal_Gpio_Direction_impl;
T_Hal_Gpio_Pinmux           Hal_Gpio_Pinmux             = Hal_Gpio_Pinmux_impl;

// MmFactor relative
T_Hal_Gpio_MmFactorResumeByGpioIntEn       Hal_Gpio_MmFactorResumeByGpioIntEn           = Hal_Gpio_MmFactorResumeByGpioIntEn_impl;
T_Hal_Gpio_MmFactorResumeByGpioIntStatRead Hal_Gpio_MmFactorResumeByGpioIntStatRead     = Hal_Gpio_MmFactorResumeByGpioIntStatRead_impl;

/* Sleep IO */
T_Hal_Gpio_SleepIoUsrCtrlSet    Hal_Gpio_SleepIoUsrCtrlSet      = Hal_Gpio_SleepIoUsrCtrlSet_impl;
T_Hal_Gpio_SleepIoUsrCtrlGet    Hal_Gpio_SleepIoUsrCtrlGet      = Hal_Gpio_SleepIoUsrCtrlGet_impl;
T_Hal_Gpio_SleepIoAutoCtrlSet   Hal_Gpio_SleepIoAutoCtrlSet     = Hal_Gpio_SleepIoAutoCtrlSet_impl;
T_Hal_Gpio_SleepIoAutoCtrlGet   Hal_Gpio_SleepIoAutoCtrlGet     = Hal_Gpio_SleepIoAutoCtrlGet_impl;


T_Hal_Gpio_StoreRunModePinmux       Hal_Gpio_StoreRunModePinmux     = Hal_Gpio_StoreRunModePinmux_impl;
T_Hal_Gpio_RestoreRunModePinmux     Hal_Gpio_RestoreRunModePinmux   = Hal_Gpio_RestoreRunModePinmux_impl;
T_Hal_Gpio_SleepIoApply             Hal_Gpio_SleepIoApply           = Hal_Gpio_SleepIoApply_impl;
// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable

// Sec 7: declaration of static function prototype

/***********
C Functions
***********/
// Sec 8: C Functions
uint32_t Hal_Gpio_IntInit_impl(E_GpioIdx_t eIdx, E_VIC_INT_TRIG_TYPE eTrigType, T_Gpio_CallBack tFunc)
{
    S_VIC_TRIG_CTRL *psTrigCtrl = (S_VIC_TRIG_CTRL *)&eTrigType;
    uint32_t u32Idx;
    uint32_t u32TypeBitIdx;
    
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    g_taHalVicGpioCallBack[eIdx] = tFunc;
    u32TypeBitIdx = GPIO_IO_IDX_TO_TYPE_BIT_IDX(eIdx);
    u32Idx = GPIO_IO_IDX_TO_BIT_IDX(eIdx);
    if (eIdx < GPIO_CTRL_REG_HIGH_START)
    {   /* IO0 ~ IO31 */
        /* Masked IRQ first */
        BIT_SET(VIC->CTRLC_GINT_MSK_L, u32Idx, 1);
        /* Inverter */
        BIT_SET(VIC->CTRL7_GINT_INV_L, u32Idx, psTrigCtrl->u8InvEn);
        /* Disable */
        BIT_SET(VIC->CTRL8_GINT_DIS_L, u32Idx, 0);
        /* Set trigger type */
        if (eIdx < GPIO_INT_TYPE1_IO_START)
            GPIO_SET_INT_TYPE_BITS(VIC->CTRL9_GINT_TYPE0, u32TypeBitIdx, psTrigCtrl->u8Type);
        else
            GPIO_SET_INT_TYPE_BITS(VIC->CTRLA_GINT_TYPE1, u32TypeBitIdx, psTrigCtrl->u8Type);
        /* Clear */
        VIC->CTRLB_GINT_CLR_L = 1 << u32Idx;
        /* Mask */
        BIT_SET(VIC->CTRLC_GINT_MSK_L, u32Idx, 0);
    }
    else
    {   /* IO32 ~ IOMAX */
        /* Masked IRQ first */
        BIT_SET(VIC->CTRL15_GINT_MSK_H, u32Idx, 1);
        /* Inverter */
        BIT_SET(VIC->CTRL10_GINT_INV_H, u32Idx, psTrigCtrl->u8InvEn);
        /* Disable */
        BIT_SET(VIC->CTRL11_GINT_DIS_H, u32Idx, 0);
        /* Set trigger type */
        GPIO_SET_INT_TYPE_BITS(VIC->CTRL12_GINT_TYPE2, u32TypeBitIdx, psTrigCtrl->u8Type);
        /* Clear */
        VIC->CTRL14_GINT_CLR_H = 1 << u32Idx;
        /* Mask */
        BIT_SET(VIC->CTRL15_GINT_MSK_H, u32Idx, 0);
    }
    
    return RESULT_SUCCESS;
    
}

uint32_t Hal_Gpio_IntDeInit_impl(E_GpioIdx_t eIdx)
{
    uint32_t u32Idx;
    uint32_t u32TypeBitIdx;
    
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    g_taHalVicGpioCallBack[eIdx] = NULL;
    u32TypeBitIdx = GPIO_IO_IDX_TO_TYPE_BIT_IDX(eIdx);
    u32Idx = GPIO_IO_IDX_TO_BIT_IDX(eIdx);
    if (eIdx < GPIO_CTRL_REG_HIGH_START)
    {   /* IO0 ~ IO31 */
        /* Masked IRQ first */
        BIT_SET(VIC->CTRLC_GINT_MSK_L, u32Idx, 1);
        /* Clear */
        VIC->CTRLB_GINT_CLR_L = 1 << u32Idx;
        /* Set trigger type */
        u32TypeBitIdx = GPIO_IO_IDX_TO_TYPE_BIT_IDX(eIdx);
        if (eIdx < GPIO_INT_TYPE1_IO_START)
            GPIO_SET_INT_TYPE_BITS(VIC->CTRL9_GINT_TYPE0, u32TypeBitIdx, 0);
        else
            GPIO_SET_INT_TYPE_BITS(VIC->CTRLA_GINT_TYPE1, u32TypeBitIdx, 0);
        /* Disable */
        BIT_SET(VIC->CTRL8_GINT_DIS_L, u32Idx, 1);
        /* Inverter */
        BIT_SET(VIC->CTRL7_GINT_INV_L, u32Idx, 0);
    }
    else
    {   /* IO32 ~ IOMAX */
        /* Masked IRQ first */
        BIT_SET(VIC->CTRL15_GINT_MSK_H, u32Idx, 1);
        /* Clear */
        VIC->CTRL14_GINT_CLR_H = 1 << u32Idx;
        /* Set trigger type */
        GPIO_SET_INT_TYPE_BITS(VIC->CTRL12_GINT_TYPE2, u32TypeBitIdx, 0);
        /* Disable */
        BIT_SET(VIC->CTRL11_GINT_DIS_H, u32Idx, 1);
        /* Inverter */
        BIT_SET(VIC->CTRL10_GINT_INV_H, u32Idx, 0);
    }
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Gpio_IntEn
 *        Enable or disable gpio interrupt signal
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @param u8Enable[in] ::ENABLE or ::DISABLE
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Gpio_IntEn_impl(E_GpioIdx_t eIdx, uint8_t u8Enable)
{
    uint32_t u32Idx;
    
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    u32Idx = GPIO_IO_IDX_TO_BIT_IDX(eIdx);
    if (eIdx < GPIO_CTRL_REG_HIGH_START)
    {   /* IO0 ~ IO31 */
        BIT_SET(VIC->CTRL8_GINT_DIS_L, u32Idx, (u8Enable == ENABLE)?0:1);
    }
    else
    {   /* IO32 ~ MAX */
        BIT_SET(VIC->CTRL11_GINT_DIS_H, u32Idx, (u8Enable == ENABLE)?0:1);
    }

    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Gpio_IntClear
 *        Clear gpio interrupt
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Gpio_IntClear_impl(E_GpioIdx_t eIdx)
{
    uint32_t u32Idx;
    
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    u32Idx = GPIO_IO_IDX_TO_BIT_IDX(eIdx);
    if (eIdx < GPIO_CTRL_REG_HIGH_START)
    {   /* IO0 ~ IO31 */
        VIC->CTRLB_GINT_CLR_L = 1 << u32Idx;
    }
    else
    {   /* IO32 ~ MAX */
        VIC->CTRL14_GINT_CLR_H = 1 << u32Idx;
    }
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Gpio_IntMask
 *        Mask gpio interrupt
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @param u8Enable[in] ::ENABLE or ::DISABLE
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Gpio_IntMask_impl(E_GpioIdx_t eIdx, uint8_t u8Enable)
{
    uint32_t u32Idx;
    
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    u32Idx = GPIO_IO_IDX_TO_BIT_IDX(eIdx);
    if (eIdx < GPIO_CTRL_REG_HIGH_START)
    {   /* IO0 ~ IO31 */
        BIT_SET(VIC->CTRLC_GINT_MSK_L, u32Idx, (u8Enable == ENABLE)?1:0);
    }
    else
    {   /* IO32 ~ MAX */
        BIT_SET(VIC->CTRL15_GINT_MSK_H, u32Idx, (u8Enable == ENABLE)?1:0);
    }

    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Gpio_IntTypeSel
 *        Select gpio interrupt type
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @param eType[in] Type of interrupt. refert to E_ItrType_t
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Gpio_IntTypeSel_impl(E_GpioIdx_t eIdx, E_ItrType_t eType)
{
    uint32_t u32TypeBitIdx;
    
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    u32TypeBitIdx = GPIO_IO_IDX_TO_TYPE_BIT_IDX(eIdx);
    if (eIdx < GPIO_INT_TYPE1_IO_START)
    {   /* Type 0: IO0 ~ IO15 */
        GPIO_SET_INT_TYPE_BITS(VIC->CTRL9_GINT_TYPE0, u32TypeBitIdx, eType);
    }
    else if (eIdx < GPIO_INT_TYPE2_IO_START)
    {   /* Type 1: IO16 ~ IO31 */
        GPIO_SET_INT_TYPE_BITS(VIC->CTRLA_GINT_TYPE1, u32TypeBitIdx, eType);
    }
    else
    {   /* Type 2: IO32 ~ MAX */
        GPIO_SET_INT_TYPE_BITS(VIC->CTRL12_GINT_TYPE2, u32TypeBitIdx, eType);
    }
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Gpio_IntInv
 *        Invert gpio interrupt signal
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @param u8Enable[in] ::ENABLE or ::DISABLE
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Gpio_IntInv_impl(E_GpioIdx_t eIdx, uint8_t u8Enable)
{
    uint32_t u32Idx;
    
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    u32Idx = GPIO_IO_IDX_TO_BIT_IDX(eIdx);
    if (eIdx < GPIO_CTRL_REG_HIGH_START)
    {   /* IO0 ~ IO31 */
        BIT_SET(VIC->CTRL7_GINT_INV_L, u32Idx, (u8Enable == ENABLE)?1:0);
    }
    else
    {   /* IO32 ~ MAX */
        BIT_SET(VIC->CTRL10_GINT_INV_H, u32Idx, (u8Enable == ENABLE)?1:0);
    }
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Gpio_IntStatus
 *        Return gpio interrupt status
 * @return gpio interrupt status. Bit position as the GPIO status.
 *          e.g. Bit 0 means IO0 status.
 */
uint64_t Hal_Gpio_IntStatRead_impl(void)
{
    return ((uint64_t)VIC->CTRL16_GINT_STS_H << 32) | (uint64_t)VIC->CTRLD_GINT_STS_L;
}

/**
 * @brief Hal_Gpio_CallBackFuncSet
 *        Setup GPIO callback function
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @param tFunc[in] Callback function
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Gpio_CallBackFuncSet_impl(E_GpioIdx_t eIdx, T_Gpio_CallBack tFunc)
{
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    g_taHalVicGpioCallBack[eIdx] = tFunc;
    return RESULT_SUCCESS;
}



 
/**
 * @brief Set user control pin setting in sleep mode
 *
 *        In OPL2500, if the IO is input by GPIO or peripheral, it will be disable in sleep mode.
 *        If the IO is GPIO output, it will keep output setting in sleep mode.
 *        If the IO is peripheral output, it might be enable high/low or disable according to the peripheral.
 *        Besides the auto configuration, uesr can use this function to assign IO pin as output high/low or disable in sleep mode.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param eSleepIoCfg [in] To configure the pin setting in sleep mode
 *          SLEEP_USER_CTRL_OFF: Using auto control.
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Configuration status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoUsrCtrlSet_impl(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG eSleepIoCfg)
{
    uint64_t u64IoBit = (1ULL << eIdx);
    S_SLEEP_CFG_DEF sSleepDef;
    
    if ((eIdx >= GPIO_IDX_NUM) || (g_psHal_Gpio_SleepIoCfg == NULL))
        return SLEEP_IO_FAIL;
    
    sSleepDef.u32Value = (uint32_t)eSleepIoCfg;
    
    if (sSleepDef.u8IoCtl)
        g_psHal_Gpio_SleepIoCfg->u64UsrCtrl |= u64IoBit;
    else
        g_psHal_Gpio_SleepIoCfg->u64UsrCtrl &= ~u64IoBit;
    
    if (sSleepDef.u8OutEn)
        g_psHal_Gpio_SleepIoCfg->u64UsrOutEn |= u64IoBit;
    else
        g_psHal_Gpio_SleepIoCfg->u64UsrOutEn &= ~u64IoBit;
    
    if (sSleepDef.u8OutLvl)
        g_psHal_Gpio_SleepIoCfg->u64UsrOutLvl |= u64IoBit;
    else
        g_psHal_Gpio_SleepIoCfg->u64UsrOutLvl &= ~u64IoBit;
    
    return SLEEP_IO_SUCCESS;
}


/**
 * @brief Get user control pin setting in sleep mode
 *
 *        In OPL2500, if the IO is input by GPIO or peripheral, it will be disable in sleep mode.
 *        If the IO is GPIO output, it will keep output setting in sleep mode.
 *        If the IO is peripheral output, it might be enable high/low or disable according to the peripheral.
 *        Besides the auto configuration, uesr can use this function to assign IO pin as output high/low or disable in sleep mode.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param peSleepIoCfg [out] Read pin user control setting in sleep mode
 *          SLEEP_USER_CTRL_OFF: Using auto control.
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Read control status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoUsrCtrlGet_impl(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG *peSleepIoCfg)
{
    uint64_t u64IoBit = (1ULL << eIdx);
    
    if ((eIdx >= GPIO_IDX_NUM) || (g_psHal_Gpio_SleepIoCfg == NULL))
        return SLEEP_IO_FAIL;
    
    if (!(g_psHal_Gpio_SleepIoCfg->u64UsrCtrl & u64IoBit))
    {   /* user control is off */
        *peSleepIoCfg = SLEEP_USER_CTRL_OFF;
    }
    else if (!(g_psHal_Gpio_SleepIoCfg->u64UsrOutEn & u64IoBit))
    {
        *peSleepIoCfg = SLEEP_OUTPUT_DISABLE;
    }
    else if (g_psHal_Gpio_SleepIoCfg->u64UsrOutLvl & u64IoBit)
    {
        *peSleepIoCfg = SLEEP_OUTPUT_ENABLE_HIGH;
    }
    else
    {
        *peSleepIoCfg = SLEEP_OUTPUT_ENABLE_LOW;
    }
    return SLEEP_IO_SUCCESS;
}

/**
 * @brief Set auto control pin setting in sleep mode
 * 
 * @warning Only called by Hal driver. Don't call this function in user layer.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param peSleepIoCfg [out] Read pin user control setting in sleep mode
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Read control status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoAutoCtrlSet_impl(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG eSleepIoCfg)
{
    uint64_t u64IoBit = (1ULL << eIdx);
    S_SLEEP_CFG_DEF sSleepDef;

    if ((eIdx >= GPIO_IDX_NUM) || (g_psHal_Gpio_SleepIoCfg == NULL))
        return SLEEP_IO_FAIL;
    
    sSleepDef.u32Value = (uint32_t)eSleepIoCfg;
    
    if (sSleepDef.u8OutEn)
        g_psHal_Gpio_SleepIoCfg->u64AutoOutEn |= u64IoBit;
    else
        g_psHal_Gpio_SleepIoCfg->u64AutoOutEn &= ~u64IoBit;
    
    if (sSleepDef.u8OutLvl)
        g_psHal_Gpio_SleepIoCfg->u64AutoOutLvl |= u64IoBit;
    else
        g_psHal_Gpio_SleepIoCfg->u64AutoOutLvl &= ~u64IoBit;
    
    return SLEEP_IO_SUCCESS;
}

/**
 * @brief Read auto control pin setting in sleep mode
 *
 *        In OPL2500, if the IO is input by GPIO or peripheral, it will be disable in sleep mode.
 *        If the IO is GPIO output, it will keep output setting in sleep mode.
 *        If the IO is peripheral output, it might be enable high/low or disable according to the peripheral.
 *        Besides the auto configuration, uesr can use this function to assign IO pin as output high/low or disable in sleep mode.
 *
 * @param eIdx [in] The IO index. GPIO_IDX_00, GPIO_IDX_01, and etc
 * @param peSleepIoCfg [out] Read pin user control setting in sleep mode
 *          SLEEP_OUTPUT_DISABLE: Using user contorl, force the pin disable output.
 *          SLEEP_OUTPUT_ENABLE_HIGH: Using user control, force the pin enable output and pull high.
 *          SLEEP_OUTPUT_ENABLE_LOW: Using user control, force the pin enable output and pull low.
 * @return Read control status
 * @retval SLEEP_IO_SUCCESS     Set success
 * @retval SLEEP_IO_FAIL        Set fail
 */
E_SLEEP_IO_STATUS Hal_Gpio_SleepIoAutoCtrlGet_impl(E_GpioIdx_t eIdx, E_SLEEP_IO_CFG *peSleepIoCfg)
{
    uint64_t u64IoBit = (1ULL << eIdx);
    
    if ((eIdx >= GPIO_IDX_NUM) || (g_psHal_Gpio_SleepIoCfg == NULL))
        return SLEEP_IO_FAIL;
    
    if (!(g_psHal_Gpio_SleepIoCfg->u64AutoOutEn & u64IoBit))
    {
        *peSleepIoCfg = SLEEP_OUTPUT_DISABLE;
    }
    else if (g_psHal_Gpio_SleepIoCfg->u64AutoOutLvl & u64IoBit)
    {
        *peSleepIoCfg = SLEEP_OUTPUT_ENABLE_HIGH;
    }
    else
    {
        *peSleepIoCfg = SLEEP_OUTPUT_ENABLE_LOW;
    }
    return SLEEP_IO_SUCCESS;
}


/**
 * @brief Hal_Gpio_VicInit
 *        Inital of all GPIO interrupt setting.
 */
void Hal_Gpio_VicInit_impl(void)
{
    Hal_Vic_IntInit(GPIO_IRQn, VIC_TRIG_LEVEL_HIGH, IRQ_PRIORITY_GPIO);
}

/**
 * @brief Hal_Gpio_Input
 *        get the input value
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @param tLevel[in] the output level
 * @return Input level ::E_GpioLevel_t
 * @retval ::GPIO_LEVEL_LOW     Input low level
 * @retval ::GPIO_LEVEL_HIGH    Input high level
 */
E_GpioLevel_t Hal_Gpio_Input_impl(E_GpioIdx_t eIdx)
{
    if (eIdx >= GPIO_SIP_START_IDX)
    {   /* Cannot get SiP IO input level */
        return GPIO_LEVEL_LOW;
    }
    return (E_GpioLevel_t)REG_BB_READ(PIN->GPI, eIdx);
}

/**
 * @brief Hal_Gpio_Output
 *        set the output level
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @param tLevel[in] the output level
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Gpio_Output_impl(E_GpioIdx_t eIdx, E_GpioLevel_t tLevel)
{
    if (eIdx >= GPIO_IDX_NUM)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    if (eIdx >= GPIO_SIP_START_IDX)
    {   /* SiP IO */
        GPIO_CTRL_SIP_IO_GPO_SET(eIdx, 1);
        return RESULT_SUCCESS;
    }
    
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    if (tLevel == GPIO_LEVEL_LOW)
        REG_BB_WRITE(PIN->GPO_LOW, eIdx, 1);
    else
        REG_BB_WRITE(PIN->GPO_HIGH, eIdx, 1);
    
    return RESULT_SUCCESS;
}

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
uint32_t Hal_Gpio_Direction_impl(E_GpioIdx_t eIdx, E_GpioDirection_t eDirection)
{   
    if (eIdx >= GPIO_IDX_NUM)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    if (eIdx >= GPIO_SIP_START_IDX)
    {   /* SiP IO */
        GPIO_CTRL_SIP_IO_IE_SET(eIdx, 0);       /* Disable input first to avoid unexpected input signal */
        GPIO_CTRL_SIP_IO_DIR_SET(eIdx, (eDirection==GPIO_INPUT)?1:0);
        if (eDirection == GPIO_INPUT)
            GPIO_CTRL_SIP_IO_IE_SET(eIdx, 1);
    }
    
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    REG_BB_WRITE(PIN->PD_IE, eIdx, 0);    /* Disable input first to avoid unexpected input signal */
    REG_BB_WRITE(PIN->PD_DIR, eIdx, (eDirection==GPIO_INPUT)?1:0);
    if (eDirection == GPIO_INPUT)
        REG_BB_WRITE(PIN->PD_IE, eIdx, 1);
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Gpio_Pinmux
 *        set the pinmux to input or output
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @param eDirection[in] the GPIO direction
 * @param eLevel[in] the output level
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Gpio_Pinmux_impl(E_GpioIdx_t eIdx, E_GpioDirection_t eDirection, E_GpioLevel_t eLevel)
{
    uint32_t u32Ret = RESULT_SUCCESS;
    if (eIdx >= GPIO_IDX_NUM)
        return RESULT_FAIL;
    
    if (eIdx >= GPIO_SIP_START_IDX)
    {   /* SiP IO */
        GPIO_CTRL_SIP_IO_IE_SET(eIdx, 0);       /* Disable input first to avoid unexpected input signal */
        if (eDirection == GPIO_OUTPUT)
        {   /* Output */           
            GPIO_CTRL_SIP_IO_PE_SET(eIdx, 0);       /* pull-up / pull-down disable */
            GPIO_CTRL_SIP_IO_GPO_SET(eIdx, eLevel);
            GPIO_CTRL_SIP_IO_DIR_SET(eIdx, 0);
            GPIO_CTRL_SIP_IO_PDOC_SET(eIdx, 0);
            GPIO_CTRL_SIP_IO_PDOV_SET(eIdx, 0);
        }
        else
        {   /* Input */
            switch (eLevel)
            {
                case GPIO_LEVEL_FLOATING:
                    GPIO_CTRL_SIP_IO_PE_SET(eIdx, 0);   /* pull-up / pull-down disable */
                    break;
                case GPIO_LEVEL_LOW:
                case GPIO_LEVEL_HIGH:
                    if (((GPIO_IO_PULL_UP_PAD & (1ULL << eIdx)) > 0) ^ (eLevel == GPIO_LEVEL_HIGH))
                    {   /* Mismatch */
                        u32Ret = RESULT_FAIL;
                    }
                    else
                    {
                        GPIO_CTRL_SIP_IO_PE_SET(eIdx, 1);
                    }
                    break;
                default:
                    GPIO_CTRL_SIP_IO_PE_SET(eIdx, 0);
                    u32Ret = RESULT_FAIL;
            }
            GPIO_CTRL_SIP_IO_DIR_SET(eIdx, 1);
            GPIO_CTRL_SIP_IO_PDOC_SET(eIdx, 0);
            GPIO_CTRL_SIP_IO_PDOV_SET(eIdx, 0);
            GPIO_CTRL_SIP_IO_IE_SET(eIdx, 1);       /* input enable */
        }
        return u32Ret;
    }
    
    if ((eIdx >= g_u32Hal_IoPkgNum) && (eIdx < GPIO_SIP_START_IDX))
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    
    REG_BB_WRITE(PIN->PD_IE, eIdx, 0);       /* Disable input first to avoid unexpected input signal */
    
    
    // output level
    if (eDirection == GPIO_OUTPUT)
    {   /* Output */
        REG_BB_WRITE(PIN->PD_PE, eIdx, 0);
        if (eLevel == GPIO_LEVEL_LOW)
            REG_BB_WRITE(PIN->GPO_LOW, eIdx, 1);
        else
            REG_BB_WRITE(PIN->GPO_HIGH, eIdx, 1);
        REG_BB_WRITE(PIN->PD_DIR, eIdx, 0);
        REG_BB_WRITE(PIN->PDOC_MODE, eIdx, 0);
        REG_BB_WRITE(PIN->PDOV_MODE, eIdx, 0);
    }
    else
    {   /* Input */
        switch (eLevel)
        {
            case GPIO_LEVEL_FLOATING:
                REG_BB_WRITE(PIN->PD_PE, eIdx, 0);
                break;
            case GPIO_LEVEL_LOW:
            case GPIO_LEVEL_HIGH:
                if (((GPIO_IO_PULL_UP_PAD & (1ULL << eIdx)) > 0) ^ (eLevel == GPIO_LEVEL_HIGH))
                {   /* Mismatch */
                    u32Ret = RESULT_FAIL;
                }
                else
                {
                    REG_BB_WRITE(PIN->PD_PE, eIdx, 1);
                }
                break;
            default:
                REG_BB_WRITE(PIN->PD_PE, eIdx, 0);
                u32Ret = RESULT_FAIL;
        }
        REG_BB_WRITE(PIN->PD_DIR, eIdx, 1);
        REG_BB_WRITE(PIN->PDOC_MODE, eIdx, 0);
        REG_BB_WRITE(PIN->PDOV_MODE, eIdx, 0);
        REG_BB_WRITE(PIN->PD_IE, eIdx, 1);
    }
    
    Hal_Gpio_SleepIoAutoCtrlSet(eIdx, SLEEP_OUTPUT_DISABLE);
    
    return u32Ret;
}


/**
 * @brief Hal_Gpio_MmFactorResumeByGpioIntEn
 *          gpio interrupt notify clock modulator at full speed enable.
 *          Used with Hal_Sys_ApsClkMmFactorSet
 * @param eIdx[in] Index of GPIO. Refer to E_GpioIdx_t
 *                  GPIO_IDX_00, GPIO_IDX_01, ...
 *                  GPIO_IDX_37, GPIO_IDX_38
 *                  Need to be less than IO package support Index number.
 *                  The IO supported number is stored in g_u32Hal_IoPkgNum.
 * @param eDirection[in] the GPIO direction
 * @param u8Enable[in] ::ENABLE or ::DISABLE
 * @return Setup status ::E_RESULT_COMMON
 * @retval ::RESULT_SUCCESS Setup success
 * @retval ::RESULT_FAIL Setup fail
 */
uint32_t Hal_Gpio_MmFactorResumeByGpioIntEn_impl(E_GpioIdx_t eIdx, uint8_t u8Enable)
{
    uint32_t u32Idx;
    
    u32Idx = GPIO_IO_IDX_TO_BIT_IDX(eIdx);
    if (eIdx >= g_u32Hal_IoPkgNum)
    {   /* Not supported IO */
        return RESULT_FAIL;
    }
    else if (eIdx >= GPIO_CTRL_REG_HIGH_START)
    {   /* IO32 ~ MAX */
        BIT_SET(VIC->CTRL17_GINT_MMF_H, u32Idx, (u8Enable==ENABLE)?1:0);
    }
    else
    {   /* IO0 ~ IO31 */
        BIT_SET(VIC->CTRLE_GINT_MMF_L, u32Idx, (u8Enable==ENABLE)?1:0);
    }
    
    return RESULT_SUCCESS;
}

/**
 * @brief Hal_Gpio_MmFactorResumeByGpioIntStatRead
 *          which gpio interrupt will notify clock modulator at full speed.
 *          Used with Hal_Sys_ApsClkMmFactorSet
 * @return GPIO index bit map. The bit position represent the bit
 *          e.g. Bit 0 means IO0
 *               1 as enable and 0 as disable.
 */
uint64_t Hal_Gpio_MmFactorResumeByGpioIntStatRead_impl(void)
{
    return ((uint64_t)VIC->CTRL17_GINT_MMF_H << 32) | (uint64_t)VIC->CTRLE_GINT_MMF_L;
}

/**
 * @brief Hal_Gpio_Pre_Init
 *          Inital of functuin pointers
 * @param u32SleepIoAddr [in] The sleep IO address which is in share ram
 */
void Hal_Gpio_PreInit(uint32_t u32SleepIoAddr)
{
    #if 0
    E_GpioIdx_t u32IoPkgNum[] = {GPIO_IDX_38, GPIO_IDX_34, GPIO_IDX_27, GPIO_IDX_20};
    uint8_t u8PkgDet = (SYS->BOOT_STS & SYS_BOOT_STS_PKG_DET_Msk) >> SYS_BOOT_STS_PKG_DET_Pos;
    #endif
    
    g_u32Hal_IoPkgNum = GPIO_IDX_38;
    
    g_psHal_Gpio_SleepIoCfg = (S_SLEEP_IO_CFG *)u32SleepIoAddr;
    g_psHal_Gpio_SleepIoCfg->u64UsrCtrl       = 0;
    g_psHal_Gpio_SleepIoCfg->u64UsrOutEn      = 0;
    g_psHal_Gpio_SleepIoCfg->u64UsrOutLvl     = 0;
    g_psHal_Gpio_SleepIoCfg->u64AutoOutEn     = 0;
    g_psHal_Gpio_SleepIoCfg->u64AutoOutLvl    = 0;
}






/*********************** Only run when APS is PS master ***********************************************/
/**
 * @brief Store run mode pinmux before entering sleep
 */
void Hal_Gpio_StoreRunModePinmux_impl(void)
{
    g_sPinmuxCache.PD_IE[0]         = PIN->PD_IE[0];
    g_sPinmuxCache.PD_IE[1]         = PIN->PD_IE[1];
    g_sPinmuxCache.PD_PE[0]         = PIN->PD_PE[0];
    g_sPinmuxCache.PD_PE[1]         = PIN->PD_PE[1];
    g_sPinmuxCache.PD_O_INV[0]      = PIN->PD_O_INV[0];
    g_sPinmuxCache.PD_O_INV[1]      = PIN->PD_O_INV[1];
    g_sPinmuxCache.PD_DS[0]         = PIN->PD_DS[0];
    g_sPinmuxCache.PD_DS[1]         = PIN->PD_DS[1];
    g_sPinmuxCache.PD_I_INV[0]      = PIN->PD_I_INV[0];
    g_sPinmuxCache.PD_I_INV[1]      = PIN->PD_I_INV[1];
    g_sPinmuxCache.PD_DIR[0]        = PIN->PD_DIR[0];
    g_sPinmuxCache.PD_DIR[1]        = PIN->PD_DIR[1];
    g_sPinmuxCache.PDOV_MODE[0]     = PIN->PDOV_MODE[0];
    g_sPinmuxCache.PDOV_MODE[1]     = PIN->PDOV_MODE[1];
    g_sPinmuxCache.PD_OEN_INV[0]    = PIN->PD_OEN_INV[0];
    g_sPinmuxCache.PD_OEN_INV[1]    = PIN->PD_OEN_INV[1];
    g_sPinmuxCache.PDOC_MODE[0]     = PIN->PDOC_MODE[0];
    g_sPinmuxCache.PDOC_MODE[1]     = PIN->PDOC_MODE[1];
    g_sPinmuxCache.GPO_ST[0]        = PIN->GPO_ST[0];
    g_sPinmuxCache.GPO_ST[1]        = PIN->GPO_ST[1];
    g_sPinmuxCache.PSRAM_CONTROL_1  = PIN->PSRAM_CONTROL_1;
    g_sPinmuxCache.PSRAM_CONTROL_2  = PIN->PSRAM_CONTROL_2;
    g_sPinmuxCache.PSRAM_CONTROL_3  = PIN->PSRAM_CONTROL_3;
}

/**
 * @brief After wakeup, restore run mode pinmux
 */
void Hal_Gpio_RestoreRunModePinmux_impl(void)
{   
    /* Disable IE to avoid unexpected interrupt when setup */
    PIN->PD_IE[0]           = 0;
    PIN->PD_IE[1]           = 0;
    
    /* Output */
    PIN->PD_DS[0]           = g_sPinmuxCache.PD_DS[0];
    PIN->PD_DS[1]           = g_sPinmuxCache.PD_DS[1];
    
    /* Output - Value */
    PIN->PD_I_INV[0]        = g_sPinmuxCache.PD_I_INV[0];
    PIN->PD_I_INV[1]        = g_sPinmuxCache.PD_I_INV[1];
    PIN->PDOV_MODE[0]       = g_sPinmuxCache.PDOV_MODE[0];
    PIN->PDOV_MODE[1]       = g_sPinmuxCache.PDOV_MODE[1];
    PIN->GPO_HIGH[0]        = g_sPinmuxCache.GPO_ST[0];
    PIN->GPO_HIGH[1]        = g_sPinmuxCache.GPO_ST[1];
    PIN->GPO_LOW[0]         = ~g_sPinmuxCache.GPO_ST[0];
    PIN->GPO_LOW[1]         = ~g_sPinmuxCache.GPO_ST[1];
    PIN->PSRAM_CONTROL_2    = g_sPinmuxCache.PSRAM_CONTROL_2;
    
    /* Output - Control */
    PIN->PD_OEN_INV[0]      = g_sPinmuxCache.PD_OEN_INV[0];
    PIN->PD_OEN_INV[1]      = g_sPinmuxCache.PD_OEN_INV[1];
    PIN->PD_DIR[0]          = g_sPinmuxCache.PD_DIR[0];
    PIN->PD_DIR[1]          = g_sPinmuxCache.PD_DIR[1];
    PIN->PDOC_MODE[0]       = g_sPinmuxCache.PDOC_MODE[0];
    PIN->PDOC_MODE[1]       = g_sPinmuxCache.PDOC_MODE[1];
    PIN->PSRAM_CONTROL_3    = g_sPinmuxCache.PSRAM_CONTROL_3;
    
    /* Input */
    PIN->PD_PE[0]           = g_sPinmuxCache.PD_PE[0];
    PIN->PD_PE[1]           = g_sPinmuxCache.PD_PE[1];
    PIN->PD_O_INV[0]        = g_sPinmuxCache.PD_O_INV[0];
    PIN->PD_O_INV[1]        = g_sPinmuxCache.PD_O_INV[1];
    PIN->PD_IE[0]           = g_sPinmuxCache.PD_IE[0];
    PIN->PD_IE[1]           = g_sPinmuxCache.PD_IE[1];
    PIN->PSRAM_CONTROL_1    = g_sPinmuxCache.PSRAM_CONTROL_1;
}


/**
 * @brief Apply sleep IO auto and user control
 */
void Hal_Gpio_SleepIoApply_impl(void)
{
    uint32_t u32SipIoOutEn, u32SipIoLevel, u32SipUsrOutEn;
    uint64_t u64GpioOutEn, u64GpioLevel, u64UsrOutEn;
    S_SLEEP_IO_CFG *psCfg = g_psHal_Gpio_SleepIoCfg;
    
    /******* SiP IO *******/
    /* GPIO output setting */
    u32SipIoOutEn = ~((PIN->PSRAM_CONTROL_3 >> PIN_PSRAM_CONTROL_3_PDOC_PSRAM_39_44_Pos) | (PIN->PSRAM_CONTROL_2 >> PIN_PSRAM_CONTROL_2_PD_DIR_PSRAM_39_44_Pos));
    u32SipIoLevel = (PIN->PSRAM_CONTROL_2 >> PIN_PSRAM_CONTROL_2_GPO_PSRAM_39_44_Pos) & u32SipIoOutEn;
            
    /* Plus auto control */
    u32SipIoOutEn |= psCfg->u64AutoOutEn >> GPIO_SIP_START_IDX;
    u32SipIoLevel |= (psCfg->u64AutoOutLvl & psCfg->u64AutoOutEn) >> GPIO_SIP_START_IDX;
    
    /* Force User control IO */
    u32SipUsrOutEn = (psCfg->u64UsrOutEn & psCfg->u64UsrCtrl) >> GPIO_SIP_START_IDX;
    u32SipIoOutEn = (u32SipIoOutEn & ~(psCfg->u64UsrCtrl >> GPIO_SIP_START_IDX)) | u32SipUsrOutEn;
    u32SipIoLevel = (u32SipIoLevel & ~(psCfg->u64UsrCtrl >> GPIO_SIP_START_IDX)) | ((psCfg->u64UsrOutLvl >> GPIO_SIP_START_IDX) & u32SipUsrOutEn);

    /* Apply to PIN register */
    PIN->PSRAM_CONTROL_1 = 0;
    PIN->PSRAM_CONTROL_2 = ((u32SipIoLevel & GPIO_SIP_REG_BITS_MSK) << PIN_PSRAM_CONTROL_2_GPO_PSRAM_39_44_Pos) | ((~u32SipIoOutEn & GPIO_SIP_REG_BITS_MSK) << PIN_PSRAM_CONTROL_2_PD_DIR_PSRAM_39_44_Pos);
    PIN->PSRAM_CONTROL_3 = 0;
    
    
    /******* External IO **************/
    /* Disable all input pin except wakeup IO */
    PIN->PD_IE[0] &= ~VIC->CTRL8_GINT_DIS_L;
    PIN->PD_IE[1] &= ~VIC->CTRL11_GINT_DIS_H;
    PIN->PD_PE[0] &= ~VIC->CTRL8_GINT_DIS_L;
    PIN->PD_PE[1] &= ~VIC->CTRL11_GINT_DIS_H;
    
    /* GPIO output setting */
    u64GpioOutEn = ~( (uint64_t)PIN->PDOC_MODE[0] | (uint64_t)PIN->PD_DIR[0] | ((uint64_t)PIN->PDOC_MODE[1] << 32) | ((uint64_t)PIN->PD_DIR[1] << 32) );
    u64GpioLevel = ( (uint64_t)PIN->GPO_ST[0] | ((uint64_t)PIN->GPO_ST[1] << 32) ) & u64GpioOutEn;
    
    /* Plus auto control */
    u64GpioOutEn |= psCfg->u64AutoOutEn;
    u64GpioLevel |= psCfg->u64AutoOutLvl & psCfg->u64AutoOutEn;
    
    /* Force User control IO */
    u64UsrOutEn = psCfg->u64UsrOutEn & psCfg->u64UsrCtrl;
    u64GpioOutEn = ((u64GpioOutEn & ~psCfg->u64UsrCtrl) | u64UsrOutEn) & GPIO_REG_BITS_MSK;
    u64GpioLevel = ((u64GpioLevel & ~psCfg->u64UsrCtrl) | (psCfg->u64UsrOutLvl & u64UsrOutEn)) & GPIO_REG_BITS_MSK;
         
    /* Apply to PIN register */
    PIN->GPO_HIGH[0]     = (uint32_t)u64GpioLevel;   /* Set output high level */
    PIN->GPO_HIGH[1]     = u64GpioLevel >> 32;
    PIN->GPO_LOW[0]      = ~(uint32_t)u64GpioLevel;  /* Set output low level */
    PIN->GPO_LOW[1]      = ~(u64GpioLevel >> 32);
    
    PIN->PD_DS[0] = 0;           /* Set all IO to minimum driving strength */
    PIN->PD_DS[1] = 0;
    PIN->PDOV_MODE[0] = 0;       /* Set value source to GPIO */    
    PIN->PDOV_MODE[1] = 0;
    
    PIN->PD_DIR[0] = ~(uint32_t)u64GpioOutEn;    /* Set output enable/disable. Low active */
    PIN->PD_DIR[1] = ~(u64GpioOutEn >> 32);
    
    PIN->PDOC_MODE[0] = 0;       /* Set control to GPIO */
    PIN->PDOC_MODE[1] = 0;
}
