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
 *  @file hal_system_patch.c
 * 
 * 
 *******************************************************************************/

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */
#include <string.h>
#include "hal_system.h"
#include "svn_rev_patch.h"
#include "ipc.h"
#include "hal_vic.h"
#include "hal_uart.h"
#include "hal_pin.h"
/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

#define APS_SRAM_BYPASS_RD_FF       (SYS_SRAM_BYPASS_APS_SRAM_RD_FF1_BYPASS     | \
                                     SYS_SRAM_BYPASS_APS_SRAM_RD_FF2_BYPASS     | \
                                     SYS_SRAM_BYPASS_SHARE_SRAM_RD_FF1_BYPASS   | \
                                     SYS_SRAM_BYPASS_SHARE_SRAM_RD_FF2_BYPASS   | \
                                     SYS_SRAM_BYPASS_APS_ROM_RD_FF_BYPASS       | \
                                     SYS_SRAM_BYPASS_QSPI_RD_FF_BYPASS          )

#define APS_SRAM_BYPASS_RD_WS2      (SYS_SRAM_BYPASS_APS_SRAM_RD_WS2_BYPASS     | \
                                     SYS_SRAM_BYPASS_SHARE_SRAM_RD_WS2_BYPASS   )

#define APS_SRAM_BYPASS_RD_WS1      (SYS_SRAM_BYPASS_APS_SRAM_RD_WS1_BYPASS     | \
                                     SYS_SRAM_BYPASS_SHARE_SRAM_RD_WS1_BYPASS   | \
                                     SYS_SRAM_BYPASS_APS_ROM_RD_WS_BYPASS       | \
                                     SYS_SRAM_BYPASS_QSPI_RD_WS_BYPASS          )
                                     
#define APS_SRAM_BYPASS_WR_WS       (SYS_SRAM_BYPASS_APS_SRAM_WR_WS_BYPASS      | \
                                     SYS_SRAM_BYPASS_SHARE_SRAM_WR_WS_BYPASS    )
#define APS_SRAM_BYPASS_WR_FF       (SYS_SRAM_BYPASS_APS_SRAM_WR_FF_BYPASS      | \
                                    SYS_SRAM_BYPASS_SHARE_SRAM_WR_FF_BYPASS     )


#define APS_WAIT_STATE_ZERO_VALUE   (APS_SRAM_BYPASS_RD_WS1 | \
                                     APS_SRAM_BYPASS_RD_WS2 | \
                                     APS_SRAM_BYPASS_WR_WS  | \
                                     APS_SRAM_BYPASS_RD_FF  | \
                                     APS_SRAM_BYPASS_WR_FF  )

/* Pipeline bypass number > wait state bypass number */
#define APS_WAIT_STATE_TRANS_VALUE  (APS_SRAM_BYPASS_RD_WS2 | \
                                     APS_SRAM_BYPASS_RD_FF  | \
                                     APS_SRAM_BYPASS_WR_FF  )

#define APS_WAIT_STATE_ONE_VALUE    (APS_SRAM_BYPASS_RD_WS2 | \
                                     APS_SRAM_BYPASS_RD_FF  )


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

void Hal_Sys_WakeupResume_patch(void);
void Hal_Sys_SleepPrepare_patch(void);
uint32_t Hal_Sys_RetRamCtrlBySeq_patch(uint32_t u32RetRamIdxs);
uint32_t Hal_Sys_RetRamTurnOff_patch(uint32_t u32RetRamIdxs);
uint32_t Hal_Sys_RetRamTurnOn_patch(uint32_t u32RetRamIdxs);
void Hal_SysPinMuxM3UartSwitch_patch(void);
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
extern uint32_t g_u32SramUsageBmp;


uint32_t g_u32ApsWaitState = 0;

 
 
 


/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */

void Hal_Sys_PatchInit(void)
{
    Hal_Sys_WakeupResume = Hal_Sys_WakeupResume_patch;
    Hal_Sys_SleepPrepare = Hal_Sys_SleepPrepare_patch;
    Hal_Sys_RetRamCtrlBySeq = Hal_Sys_RetRamCtrlBySeq_patch;
    Hal_Sys_RetRamTurnOn = Hal_Sys_RetRamTurnOn_patch;
    Hal_SysPinMuxM3UartSwitch = Hal_SysPinMuxM3UartSwitch_patch;
}


void Hal_Sys_PatchVerInit(uint32_t u32VerInfoAddr)
{
    S_SW_VERSION_INFO *pVerInfo = (S_SW_VERSION_INFO *)u32VerInfoAddr;
    char *szBuildTime = SVN_TIME_NOW_PATCH;
    
    pVerInfo->sApsPatch.u32Revision = SVN_REVISION_PATCH;
    strcpy(pVerInfo->sApsPatch.szBuildTime, szBuildTime);
}




void Hal_Sys_WaitStateSet(APS_WAIT_STATE u32WaitState)
{
    /*
    Case 1: Low Freq. (WS=0) -> High Freq. (WS=1)
            wait state bypass dis -> pipe bypass dis
    Case 2: High Freq. (WS=1) -> Low Freq. (WS=0)
            pipe bypass en -> wait state bypass en
    */
    
    if (g_u32ApsWaitState == u32WaitState)
        return;
    
    if (g_u32ApsWaitState < u32WaitState)
    {   /* Case 1:  WS=0 -> WS=1 */
        SYS->SRAM_BYPASS = APS_WAIT_STATE_TRANS_VALUE;
        __ISB();
        __DSB();
        SYS->SRAM_BYPASS = APS_WAIT_STATE_ONE_VALUE;
        __ISB();
        __DSB();
    }
    else
    {   /* Case 2:  WS=1 -> WS=0 */
        SYS->SRAM_BYPASS = APS_WAIT_STATE_TRANS_VALUE;
        __ISB();
        __DSB();
        SYS->SRAM_BYPASS = APS_WAIT_STATE_ZERO_VALUE;
        __ISB();
        __DSB();
    }
    g_u32ApsWaitState = u32WaitState;
}

uint32_t Hal_Sys_WaitStateGet(void)
{
    return g_u32ApsWaitState;
}

uint32_t Hal_Sys_WaitStateCalc(uint32_t u32SysClock)
{
    /* APS max frequency w/o wait state: 99.82 MHz */
    if (u32SysClock > 99000000)
        return 1;
    return 0;
}




/*
 *************************************************************************
 *                          Private Functions
 *************************************************************************
 */

void Hal_Sys_WakeupResume_patch(void)
{
    Hal_Sys_RetRamTurnOn(RET_RAM_SCRT);
    Hal_Vic_WakeupResume();
}

void Hal_Sys_SleepPrepare_patch(void)
{
    Hal_Sys_PtatEn(0);          // disable HPBG
    AOS->RSTS = 0xFFFFFFFF;     // clear all boot status
    
    //HAL_SYS_FAST_STARTUP_EN(1); 
    /* Don't enable fast start here.
     * It will affect XTAL clock.
     * MSQ will ENABLE it after swtich to RC */
    Hal_Uart_SleepPrepare();
    Hal_Sys_SleepClkSetup();
    Hal_Vic_SleepStore();

    Hal_Sys_RetRamTurnOff(RET_RAM_SCRT);
}

uint32_t Hal_Sys_RetRamCtrlBySeq_patch(uint32_t u32RetRamIdxs)
{
    uint32_t u32Value;
    if( u32RetRamIdxs & (~RET_RAM_BIT_Msk) )
        return 1;
    u32Value = AOS->SRAM_PSD;
    u32Value &= ~(u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos);   /* Set SEL = 0 */
    AOS->SRAM_PSD = u32Value;
    return 0;
}

uint32_t Hal_Sys_RetRamTurnOff_patch(uint32_t u32RetRamIdxs)
{
    uint32_t u32Value;
    // Check Indexes are all in legal region
    if( u32RetRamIdxs & (~RET_RAM_BIT_Msk) )
        return 1;
    
    /* Turn off: ISO -> PSD */
    
    /* Isolation */
    u32Value = AOS->SRAM_ISO_EN;
    u32Value |= u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos;      /* Set SEL = 1 */
    u32Value |= u32RetRamIdxs << RET_RAM_FW_CTRL_BIT_Pos;       /* Set CTRL = 1 */
    AOS->SRAM_ISO_EN = u32Value;
    
    /* PSD */
    u32Value = AOS->SRAM_PSD;
    u32Value |= u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos;      /* Set SEL = 1 */
    u32Value &= ~(u32RetRamIdxs << RET_RAM_FW_CTRL_BIT_Pos);    /* Set CTRL = 0 */
    AOS->SRAM_PSD = u32Value;

    return 0;
}
uint32_t Hal_Sys_RetRamTurnOn_patch(uint32_t u32RetRamIdxs)
{
    uint32_t u32Value;
    // Check Indexes are all in legal region
    if( u32RetRamIdxs & (~RET_RAM_BIT_Msk) )
        return 1;
    
    /* Turn on: PSD -> ISO */
    /* PSD */
    u32Value = AOS->SRAM_PSD;
    u32Value |= u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos;      /* Set SEL = 1 */
    u32Value |= u32RetRamIdxs << RET_RAM_FW_CTRL_BIT_Pos;       /* Set CTRL = 1 */
    AOS->SRAM_PSD = u32Value;
    
    /* Isolation */
    u32Value = AOS->SRAM_ISO_EN;
    u32Value &= ~(u32RetRamIdxs << RET_RAM_CTRL_SRC_SEL_Pos);   /* Set SEL = 0 */
    u32Value &= ~(u32RetRamIdxs << RET_RAM_FW_CTRL_BIT_Pos);    /* Set CTRL = 0 */
    AOS->SRAM_ISO_EN = u32Value;
    
    return 0;
}

void Hal_SysPinMuxM3UartSwitch_patch(void)
{
// APS_dbg_uart
    // IO2(RX), IO0(TX)
    Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_IO2 | PIN_INMODE_IO2_PULL_UP);
    Hal_Pin_Config(PIN_TYPE_APS_UART_TXD_IO0);
    
//// UART1
//    // IO5(TX), IO1(RX)
//    Hal_Pin_Config(PIN_TYPE_APS_UART_RXD_IO1 | PIN_INMODE_IO1_PULL_UP);
//    Hal_Pin_Config(PIN_TYPE_APS_UART_TXD_IO5);
}
