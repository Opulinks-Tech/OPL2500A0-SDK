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
*  hal_i2c.c
*
*  Project:
*  --------
*  OPL2500 series
*
*  Description:
*  ------------
*  This source file defines the functions of I2C .
*  Ref. document is << DesignWare DW_apb_i2c Databook >>
*
*  Author:
*  -------
*  Chung-Chun Wang
******************************************************************************/

/***********************
Head Block of The File
***********************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "opulinks.h"
#include "hal_system.h"
#include "hal_vic.h"
#include "hal_i2c.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous



/********************************************
Declaration of data structure
********************************************/
// Sec 3: structure, uniou, enum, linked list...


typedef uint32_t (*T_Hal_I2c_Eanble)(uint8_t u8Enable);

/********************************************
Declaration of Global Variables & Functions
********************************************/
// Sec 4: declaration of global  variable
/* Internal */
T_Hal_I2c_Eanble _Hal_I2c_Eanble    = _Hal_I2c_Eanble_impl;

/* Comom */
T_Hal_I2c_DeInit     Hal_I2c_DeInit     = Hal_I2c_DeInit_impl;
T_Hal_I2c_SpeedGet   Hal_I2c_SpeedGet   = Hal_I2c_SpeedGet_impl;
T_Hal_I2c_SpeedSet   Hal_I2c_SpeedSet   = Hal_I2c_SpeedSet_impl;
T_Hal_I2c_UpdateClk  Hal_I2c_UpdateClk  = Hal_I2c_UpdateClk_impl;
T_Hal_I2c_IntHandler Hal_I2c_IntHandler = Hal_I2c_IntHandler_impl;

/* Master mode relative */
T_Hal_I2c_TargetAddrSet          Hal_I2c_TargetAddrSet          = Hal_I2c_TargetAddrSet_impl;
T_Hal_I2c_WaitForMasterCompleted Hal_I2c_WaitForMasterCompleted = Hal_I2c_WaitForMasterCompleted_impl;
T_Hal_I2c_MasterReceive          Hal_I2c_MasterReceive          = Hal_I2c_MasterReceive_impl;
T_Hal_I2c_MasterReceive_Ex       Hal_I2c_MasterReceive_Ex       = Hal_I2c_MasterReceive_Ex_impl;
T_Hal_I2c_MasterTrasmit          Hal_I2c_MasterTrasmit          = Hal_I2c_MasterTrasmit_impl;
T_Hal_I2c_MasterTrasmit_Ex       Hal_I2c_MasterTrasmit_Ex       = Hal_I2c_MasterTrasmit_Ex_impl;
T_Hal_I2c_MasterInit             Hal_I2c_MasterInit             = Hal_I2c_MasterInit_impl;

/* Slave mode relative */
T_Hal_I2c_SlaveIntTxCallBack        g_tHalI2cIntTxCallback = NULL;
T_Hal_I2c_SlaveIntRxCallBack        g_tHalI2cIntRxCallback = NULL;
T_Hal_I2c_SlaveReceive              Hal_I2c_SlaveReceive                = Hal_I2c_SlaveReceive_impl;
T_Hal_I2c_SlaveTrasmit              Hal_I2c_SlaveTrasmit                = Hal_I2c_SlaveTrasmit_impl;
T_Hal_I2c_SlaveInit                 Hal_I2c_SlaveInit                   = Hal_I2c_SlaveInit_impl;
T_Hal_I2c_SlaveIntTxEn              Hal_I2c_SlaveIntTxEn                = Hal_I2c_SlaveIntTxEn_impl;
T_Hal_I2c_SlaveIntRxEn              Hal_I2c_SlaveIntRxEn                = Hal_I2c_SlaveIntRxEn_impl;
T_Hal_I2c_SlaveIntTxCallBackFuncSet Hal_I2c_SlaveIntTxCallBackFuncSet   = Hal_I2c_SlaveIntTxCallBackFuncSet_impl;
T_Hal_I2c_SlaveIntRxCallBackFuncSet Hal_I2c_SlaveIntRxCallBackFuncSet   = Hal_I2c_SlaveIntRxCallBackFuncSet_impl;

// Sec 5: declaration of global function prototype


/***************************************************
Declaration of static Global Variables &  Functions
***************************************************/
// Sec 6: declaration of static global  variable
uint32_t u32_I2c_Fs_Spike_Max = 50;
uint32_t u32_I2c_Hs_Spike_Max = 10; 
uint32_t u32_I2c_Std_SCL_Low = 5000;//4700;
uint32_t u32_I2c_Std_SCL_High = 5000;//4000;
uint32_t u32_I2c_Fs_SCL_Low = 1500;//1300;
uint32_t u32_I2c_Fs_SCL_High = 1500;// 600;
uint32_t u32_I2c_Hs_SCL_Low = 120;
uint32_t u32_I2c_Hs_SCL_High = 60;

// Sec 7: declaration of static function prototype


/***********
C Functions
***********/
// Sec 8: C Functions
/*************************************************************************
* FUNCTION:
*  _Hal_I2c_Eanble
* 
* DESCRIPTION:
*   1. Enable/Disable I2C
* 
* CALLS
* 
* PARAMETERS
*   1. u8Enable: 1 for enable/0 for disable
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t _Hal_I2c_Eanble_impl(uint8_t u8Enable)
{
    uint32_t u32Count = 0;
    uint32_t u32DisableTimeout = 10*( SystemCoreClockGet()/I2C_CLK_SUPPORT_MAX );    

    if(u8Enable)
    {
        // Enable
        I2C->ENABLE |= I2C_ENABLE_ENABLE;
    }else{
        // Disable, ref to << 3.8.3 Disabling DW_apb_i2c >>
        I2C->ENABLE &= ~I2C_ENABLE_ENABLE_Msk;
        
        while( I2C->ENABLE_STATUS & I2C_ENABLE_STATUS_IC_EN_Msk )
        {
            if(u32Count++ > u32DisableTimeout)
                return 1;
        }
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  _Hal_I2c_WaitForMasterCompleted
* 
* DESCRIPTION:
*   1. Wait for master mode no activity
* 
* CALLS
* 
* PARAMETERS
*   None
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_WaitForMasterCompleted_impl(void)
{
    uint32_t u32Count = 0;

    while(I2C->STATUS & I2C_STATUS_MST_ACTIVITY)
    {
        if(u32Count > I2C_TIMEOUT_COUNT_MAX)
            return 1;
        u32Count++;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SpeedGet
* 
* DESCRIPTION:
*   1. Get I2C speed.
* 
* CALLS
* 
* PARAMETERS
*   None
* 
* RETURNS
*   refer to E_I2cSpeed_t
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
E_I2cSpeed_t Hal_I2c_SpeedGet_impl(void)
{
    switch(I2C->CON & I2C_CON_SPEED_Msk)
    {
        case I2C_CON_SPEED_STD : return I2C_SPEED_STANDARD;
        case I2C_CON_SPEED_FAST: return I2C_SPEED_FAST;
        //case I2C_CON_SPEED_HIGH: return I2C_SPEED_HIGH;
    }
    return I2C_SPEED_STANDARD;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SpeedSet
* 
* DESCRIPTION:
*   1. Setup I2C speed.
* 
* CALLS
* 
* PARAMETERS
*   1. eSpeed : Standard(100K)/Fast(400K)/High speed(3.4M) mode. refer to E_I2cSpeed_t
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SpeedSet_impl(E_I2cSpeed_t eSpeed)
{
    uint32_t u32EnStatus = 0;
    uint32_t u32Temp = 0;

    // Disable before set
    u32EnStatus = I2C->ENABLE;
    if( u32EnStatus & I2C_ENABLE_ENABLE_Msk )
        _Hal_I2c_Eanble(0);

    u32Temp = I2C->CON & ~(I2C_CON_SPEED_Msk);
    switch(eSpeed)
    {
        case I2C_SPEED_STANDARD:
            I2C->CON = u32Temp | I2C_CON_SPEED_STD;
            I2C->SS_SCL_HCNT = u32_I2c_Std_SCL_High * SYSYEM_TICK_PER_US / US_TO_NS;
            I2C->SS_SCL_LCNT = u32_I2c_Std_SCL_Low  * SYSYEM_TICK_PER_US / US_TO_NS;
            I2C->FS_SPKLEN   = u32_I2c_Fs_Spike_Max * SYSYEM_TICK_PER_US / US_TO_NS;
            break;
        case I2C_SPEED_FAST:
            I2C->CON = u32Temp | I2C_CON_SPEED_FAST;
            I2C->FS_SCL_HCNT = u32_I2c_Fs_SCL_High  * SYSYEM_TICK_PER_US / US_TO_NS;
            I2C->FS_SCL_LCNT = u32_I2c_Fs_SCL_Low   * SYSYEM_TICK_PER_US / US_TO_NS;
            I2C->FS_SPKLEN   = u32_I2c_Fs_Spike_Max * SYSYEM_TICK_PER_US / US_TO_NS;
            break;
        //case I2C_SPEED_HIGH:
        default:
            if( u32EnStatus & I2C_ENABLE_ENABLE_Msk )
                _Hal_I2c_Eanble(1);
            return 1;
    }
    
    // Enable if need
    if( u32EnStatus & I2C_ENABLE_ENABLE_Msk )
        _Hal_I2c_Eanble(1);
    return 0;
}


/*************************************************************************
* FUNCTION:
*  Hal_I2c_UpdateClk
* 
* DESCRIPTION:
*   1. Update I2C clock when system clock updated
* 
* CALLS
* 
* PARAMETERS
*   
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_UpdateClk_impl(void)
{
    Hal_I2c_SpeedSet( Hal_I2c_SpeedGet() );
    return 0;
}

/*************************************************************************
* FUNCTION:
*   Hal_I2c_IntHandler
*
* DESCRIPTION:
*   the handler of I2C
*
* PARAMETERS
*   none
*
* RETURNS
*   none
*
*************************************************************************/
void Hal_I2c_IntHandler_impl(void)
{
    uint32_t ulIrqClr;
    uint8_t u8Data;

    // Master read cmd => slave tx
	if (I2C->INTR_STAT & I2C_INTR_STAT_RD_REQ)
    {
        if (g_tHalI2cIntTxCallback != 0)
            g_tHalI2cIntTxCallback();
        
        // clear IRQ
        ulIrqClr = I2C->CLR_RD_REQ;
    }
    
    // Master wrtie cmd => slave rx
    if (I2C->INTR_STAT & I2C_INTR_STAT_RX_FULL)
    {
        Hal_I2c_SlaveReceive(&u8Data, 1);
        if (g_tHalI2cIntRxCallback != 0)
            g_tHalI2cIntRxCallback(u8Data);
        
        // automatically clear IRQ
    }
//    
//    // tx abort
//	if (I2C->INTR_STAT & I2C_INTR_STAT_TX_ABRT)
//    {
//        // clear IRQ
//        ulIrqClr = I2C->CLR_TX_ABRT;
//    }
    
    (void)ulIrqClr;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_TargetAddrSet
* 
* DESCRIPTION:
*   1. Setup master's target address
* 
* CALLS
* 
* PARAMETERS
*   1. u16TargetAddr : Target address
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_TargetAddrSet_impl(uint16_t u16TargetAddr)
{
    uint32_t u32EnStatus = 0;
    
    // Wait for previous action complete, generate START bit
    if( Hal_I2c_WaitForMasterCompleted() )
        return 1;

    // Disable before set
    u32EnStatus = I2C->ENABLE;
    if( u32EnStatus & I2C_ENABLE_ENABLE_Msk )
        _Hal_I2c_Eanble(0);
    
    I2C->TAR &= ~I2C_TAR_TAR_Msk;
    I2C->TAR |= (u16TargetAddr & I2C_TAR_TAR_Msk);
    
    // Enable if need
    if( u32EnStatus & I2C_ENABLE_ENABLE_Msk )
        _Hal_I2c_Eanble(1);
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_MasterTrasmit
* 
* DESCRIPTION:
*   1. Master transmit, used before Hal_I2c_TargetAddrSet
* 
* CALLS
* 
* PARAMETERS
*   1. pu8Data      : Data to transmit
*   2. u32Length    : Data length
*   3. u8NeedStopBit: Need Stop bit when finished
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_MasterTrasmit_impl(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit)
{
    uint32_t u32Count = 0;
    uint32_t u32Temp = 0;
    uint32_t u32Idx = 0;

    // Wait for previous action complete, generate START bit
    if( Hal_I2c_WaitForMasterCompleted() )
        return 1;

    // Transmit data
    while(u32Idx < u32Length)
    {
        u32Temp = (uint32_t)pu8Data[u32Idx] | I2C_DATA_CMD_WRITE;

        // Check last data
        if(u32Idx == u32Length-1)
        {
            //Check for stop bit
            if(u8NeedStopBit)
                u32Temp |= I2C_DATA_CMD_STOP;
        }
        // Wait for TX not full
        while( !(I2C->STATUS & I2C_STATUS_TNFN) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
                return 1;
            u32Count++;
        }
        
        // Send data
        I2C->DATA_CMD = u32Temp;
        u32Idx++;
    }
    return 0;
}

uint32_t Hal_I2c_MasterTrasmit_Ex_impl(uint8_t *pu8Data, uint32_t u32Length, E_I2cTxCmd_t eCmd, E_I2cTxRestart_t eRestart, E_I2cTxStopBit_t eStopBit)
{
    uint32_t u32Count = 0;
    uint32_t u32Temp = 0;
    uint32_t u32Idx = 0;

     // Transmit data
    while(u32Idx < u32Length)
    {
        u32Temp = (uint32_t)pu8Data[u32Idx];

        // Check first data
        if(u32Idx == 0)
        {
            if( eCmd == I2C_TX_CMD_READ )
                u32Temp |= I2C_DATA_CMD_READ;
            else
                u32Temp |= I2C_DATA_CMD_WRITE;
            
            if( eRestart == I2C_TX_RESTART_NEED )
                u32Temp |= I2C_DATA_CMD_RESTART;
        }

        // Check last data
        if(u32Idx == u32Length-1)
        {
            if( eStopBit == I2C_TX_STOP_BIT_NEED )
                u32Temp |= I2C_DATA_CMD_STOP;
        }

        // Wait for TX not full
        while( !(I2C->STATUS & I2C_STATUS_TNFN) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
                return 1;
            u32Count++;
        }
        
        // Send data
        I2C->DATA_CMD = u32Temp;
        u32Idx++;
    }
    return 0;
}
/*************************************************************************
* FUNCTION:
*  Hal_I2c_MasterRecieve
* 
* DESCRIPTION:
*   1. Master receive, used before Hal_I2c_TargetAddrSet
* 
* CALLS
* 
* PARAMETERS
*   1. pu8Data      : Data buff to receive
*   2. u32Length    : Data length
*   3. u8NeedStopBit: Need Stop bit when finished
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_MasterReceive_impl(uint8_t *pu8Data, uint32_t u32Length, uint8_t u8NeedStopBit)
{
    uint32_t u32Count = 0;
    uint32_t u32Temp = 0;
    uint32_t u32Idx = 0;

    // Wait for previous action complete, generate START bit
    if( Hal_I2c_WaitForMasterCompleted() )
        return 1;
    
    while(u32Idx < u32Length)
    {
        u32Temp = I2C_DATA_CMD_READ;

        // Check STOP bit
        if(u32Idx == u32Length-1)
        {
            //Last data, check for stop bit
            if(u8NeedStopBit)
                u32Temp |= I2C_DATA_CMD_STOP;
        }
        I2C->DATA_CMD = u32Temp;
        
        // Wait for RX not empty
        while( !(I2C->STATUS & I2C_STATUS_RFNE) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
                return 1;
            u32Count++;
        }
        // Get data
        pu8Data[u32Idx] = (uint8_t)(I2C->DATA_CMD & I2C_DATA_CMD_DAT_Msk);
        u32Idx++;
    }
    return 0;
}

uint32_t Hal_I2c_MasterReceive_Ex_impl(uint8_t *pu8Data)
{
    uint32_t u32Count = 0;

    // Wait for RX not empty
    while( !(I2C->STATUS & I2C_STATUS_RFNE) )
    {
        if(u32Count > I2C_TIMEOUT_COUNT_MAX)
            return 1;
        u32Count++;
    }
    // Get data
    *pu8Data = (uint8_t)(I2C->DATA_CMD & I2C_DATA_CMD_DAT_Msk);
    return 0;
}


void Hal_I2c_DeInit_impl(void)
{
    // Reset module
    Hal_Sys_ApsModuleRst(APS_RST_I2C);
    // Enable clock of module
    Hal_Sys_ApsModuleClkEn(DISABLE, APS_CLK_I2C);
    
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_MasterInit
* 
* DESCRIPTION:
*   1. Initial I2c as master mode
* 
* CALLS
* 
* PARAMETERS
*   1. eAddrMode: 7/10bit Address mode. refer to E_I2cAddrMode_t
*   2. eSpeed   : Standard(100K)/Fast(400K)/High speed(3.4M) mode. refer to E_I2cSpeed_t
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_MasterInit_impl(E_I2cAddrMode_t eAddrMode, E_I2cSpeed_t eSpeed)
{
    // Enable clock of module
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_I2C);
    // Reset module
    Hal_Sys_ApsModuleRst(APS_RST_I2C);
    
    // ---> Disable first, to set registers 
    _Hal_I2c_Eanble( 0 );
    
    // Master Mode, and restart enable
    I2C->CON = I2C_CON_MASTER_MODE | I2C_CON_SLAVE_DISABLE | I2C_CON_RESTART_EN;
    
    // Address Bits
    if(eAddrMode == I2C_10BIT)
    {
        // 10-Bit Master
        I2C->CON |= I2C_CON_MASTER_10BIT;
    }else{
        // 7-Bit Master
        I2C->CON |= I2C_CON_MASTER_07BIT;
    }
    
    // Set FIFO
    I2C->RX_TL = 0;
    I2C->TX_TL = 0;
    
    // Speed
    Hal_I2c_SpeedSet( eSpeed );

//    // Mask all interrupt, handle Tx abort
//    I2C->INTR_MASK = I2C_INTR_MASK_TX_ABRT;
    
    // New for v.2.01a, clear IC_TX_CMD_BLOCK
    I2C->ENABLE = 0;

    // <--- Enable modules
    _Hal_I2c_Eanble( 1 );
    
    /* TODO: Interrrupt enable control in init parameter */
    //Hal_Vic_IntInit(I2C_IRQn, VIC_TRIG_LEVEL_HIGH, IRQ_PRIORITY_I2C);
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveTrasmit
* 
* DESCRIPTION:
*   1. Slave transmit, see <3.8.1.2> Slave-Transmitter Operation for a Single Byte
* 
* CALLS
* 
* PARAMETERS
*   1. pu8Data      : Data to transmit
*   2. u32Length    : Data length
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SlaveTrasmit_impl(uint8_t *pu8Data, uint32_t u32Length)
{
    uint32_t u32Count = 0;
    uint32_t u32Idx = 0;

    while( !(I2C->RAW_INTR_STAT & I2C_RAW_INTR_STAT_RD_REQ_Msk) )
    {
        if(u32Count > I2C_TIMEOUT_COUNT_MAX)
            return 1;
        u32Count++;
    }

    // Transmit data
    while(u32Idx < u32Length)
    {
        // Wait for TX not full
        u32Count = 0;
        while( !(I2C->STATUS & I2C_STATUS_TNFN) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
                return 1;
            u32Count++;
        }
        
        // Send data
        I2C->DATA_CMD = pu8Data[u32Idx] | I2C_DATA_CMD_WRITE;
        u32Idx++;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveReceive
* 
* DESCRIPTION:
*   1. Slave receive
* 
* CALLS
* 
* PARAMETERS
*   1. pu8Data      : Data buff to receive
*   2. u32Length    : Data length
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SlaveReceive_impl(uint8_t *pu8Data, uint32_t u32Length)
{
    uint32_t u32Count = 0;
    uint32_t u32Idx = 0;
    
    while(u32Idx < u32Length)
    {
        // Wait for RX not empty
        while( !(I2C->STATUS & I2C_STATUS_RFNE) )
        {
            if(u32Count > I2C_TIMEOUT_COUNT_MAX)
                return 1;
            u32Count++;
        }
        // Get data
        pu8Data[u32Idx] = (uint8_t)(I2C->DATA_CMD & I2C_DATA_CMD_DAT_Msk);
        u32Idx++;
    }
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveInit
* 
* DESCRIPTION:
*   1. Initial I2c as slave mode
* 
* CALLS
* 
* PARAMETERS
*   1. eAddrMode      : 7/10bit Address mode. refer to E_I2cAddrMode_t
*   2. u16SlaveAddr   : Slave address
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SlaveInit_impl(E_I2cAddrMode_t eAddrMode, uint16_t u16SlaveAddr)
{
    // Enable clock of module
    Hal_Sys_ApsModuleClkEn(ENABLE, APS_CLK_I2C);
    // Reset module
    Hal_Sys_ApsModuleRst(APS_RST_I2C);
    
    // ---> Disable first, to set registers 
    _Hal_I2c_Eanble( 0 );
    
    // Slave Mode, and restart enable
    I2C->CON = I2C_CON_RESTART_EN;
    
    // Address Bits
    if(eAddrMode == I2C_10BIT)
    {
        // 10-Bit Master
        I2C->CON |= I2C_CON_SLAVE_10BIT;
    }
    
    // Set FIFO
    I2C->RX_TL = 0;
    I2C->TX_TL = 0;
    
    // Speed, recommend set to max speed available 
    Hal_I2c_SpeedSet( I2C_SPEED_FAST );

    // Mask all interrupt
    I2C->INTR_MASK = 0;
    
    // Slave address
    I2C->SAR = u16SlaveAddr & I2C_SAR_IC_SAR_Msk;
    
    // New for v.2.01a, clear IC_TX_CMD_BLOCK
    I2C->ENABLE = 0;

    // <--- Enable modules
    _Hal_I2c_Eanble( 1 );
    
    // Enable interrupt
    Hal_Vic_IntInit(I2C_IRQn, VIC_TRIG_LEVEL_HIGH, IRQ_PRIORITY_I2C);
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveIntTxEn
* 
* DESCRIPTION:
*   1. enable / disable the slave Tx interrupt
* 
* CALLS
* 
* PARAMETERS
*   1. u8Enable: 1 for enable/0 for disable
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SlaveIntTxEn_impl(uint8_t u8Enable)
{
    // 3.8.1.2 Slave-Transmitter Operation for a Single Byte
    if(u8Enable)
    {
        I2C->INTR_MASK |= ( I2C_INTR_MASK_RD_REQ | I2C_INTR_MASK_TX_ABRT );
    }
    else
    {
        I2C->INTR_MASK &= ~( I2C_INTR_MASK_RD_REQ | I2C_INTR_MASK_TX_ABRT );
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveIntRxEn
* 
* DESCRIPTION:
*   1. enable / disable the slave Rx interrupt
* 
* CALLS
* 
* PARAMETERS
*   1. u8Enable: 1 for enable/0 for disable
* 
* RETURNS
*   0: setting complete
*   1: error 
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
uint32_t Hal_I2c_SlaveIntRxEn_impl(uint8_t u8Enable)
{
    // 3.8.1.3 Slave-Receiver Operation for a Single Byte
    if(u8Enable)
    {
        I2C->INTR_MASK |= I2C_INTR_MASK_RX_FULL;
    }
    else
    {
        I2C->INTR_MASK &= ~I2C_INTR_MASK_RX_FULL;
    }
    
    return 0;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveIntTxCallBackFuncSet
* 
* DESCRIPTION:
*   1. set the slave Tx callback fucntion
* 
* CALLS
* 
* PARAMETERS
*   1. tFunc: the callback function for slave Tx
* 
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_I2c_SlaveIntTxCallBackFuncSet_impl(T_Hal_I2c_SlaveIntTxCallBack tFunc)
{
    g_tHalI2cIntTxCallback = tFunc;
}

/*************************************************************************
* FUNCTION:
*  Hal_I2c_SlaveIntRxCallBackFuncSet
* 
* DESCRIPTION:
*   1. set the slave Rx callback fucntion
* 
* CALLS
* 
* PARAMETERS
*   1. tFunc: the callback function for slave Rx
* 
* RETURNS
* 
* GLOBALS AFFECTED
* 
*************************************************************************/
void Hal_I2c_SlaveIntRxCallBackFuncSet_impl(T_Hal_I2c_SlaveIntRxCallBack tFunc)
{
    g_tHalI2cIntRxCallback = tFunc;
}
