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

#ifndef __SYS_CFG_H__
#define __SYS_CFG_H__


typedef enum
{
    /* Note: Updated in patch file! */
    SYS_CFG_CLK_22_MHZ = 0,
    SYS_CFG_CLK_44_MHZ,
    SYS_CFG_CLK_78_MHZ,  /* Not recommand */
    SYS_CFG_CLK_81_MHZ,  /* Not recommand */
    SYS_CFG_CLK_84_MHZ,  /* Not recommand */
    SYS_CFG_CLK_87_MHZ,  /* Not recommand */
    SYS_CFG_CLK_90_MHZ,  /* Not recommand */
    SYS_CFG_CLK_93_MHZ,  /* Not recommand */
    SYS_CFG_CLK_97_MHZ,  /* Not recommand */
    SYS_CFG_CLK_101_MHZ, /* Not recommand */
    SYS_CFG_CLK_106_MHZ, /* Not recommand */
    SYS_CFG_CLK_110_MHZ, /* Not recommand */
    SYS_CFG_CLK_116_MHZ, /* Not recommand */
    SYS_CFG_CLK_122_MHZ, /* Not recommand */
    SYS_CFG_CLK_128_MHZ, /* Not recommand */
    SYS_CFG_CLK_135_MHZ, /* Not recommand */
    SYS_CFG_CLK_143_MHZ, /* Not recommand */
    SYS_CFG_CLK_176_MHZ,

    SYS_CFG_CLK_MAX
} T_SysCfgClkIdx;

typedef struct
{
    uint8_t u8HighPwrStatus;
} T_RfCfg;

typedef struct
{
    uint8_t u8WifiOnly;
} T_WifiCfg;

typedef struct
{
    uint8_t u8Enable;
    uint8_t u8DataLen;
    uint8_t u8aData[2];
} T_TsCfg;

typedef enum
{
    SYS_CFG_AUXADC_VAL_GET = 0,

    SYS_CFG_AUXADC_MAX
} T_AuxAdcType;

typedef struct
{
    uint8_t u8MaxRetry;
    uint8_t u8HighPriority;
} T_AuxAdcCfg;

typedef struct
{
    uint8_t u8Type;
    uint8_t u8Src;
    uint8_t u8GpioIdx;
    uint8_t u8AdcAlwaysOn;

    uint32_t u32RepeatCnt;
    uint32_t u32PuEnDelay_Us;
} T_AuxAdcReq;

typedef struct
{
    uint8_t u8Type;
    uint8_t u8aRsv[3];
    uint32_t u32Value;
} T_AuxAdcRsp;

// internal
typedef int (*T_SysCfgCommFp)(void);
typedef int (*T_SysCfgM0SendFp)(uint8_t u8Type, void *pMsg, uint32_t u32Len);
typedef int (*T_SysCfgClkSrcGetFp)(uint32_t u32ClkIdx, uint32_t *pu32ClkSrc);

// external
typedef int (*T_SysCfgInitFp)(void *pCfg);
typedef int (*T_SysCfgSetFp)(void *pCfg, uint8_t u8SetM0);
typedef int (*T_SysCfgGetFp)(void *pCfg);
typedef int (*T_SysCfgClkSetFp)(T_SysCfgClkIdx tClkIdx);
typedef int (*T_SysCfgM0SetFp)(void *pCfg);
typedef int (*T_SysCfgReqFp)(void *pReq, void *pRsp);
typedef int (*T_SysCfgRspFp)(uint8_t u8Statue, void *pRsp);


extern T_SysCfgCommFp sys_cfg_init;
extern T_SysCfgInitFp sys_cfg_rf_init;
extern T_SysCfgSetFp sys_cfg_rf_set;
extern T_SysCfgGetFp sys_cfg_rf_get;
extern T_SysCfgSetFp sys_cfg_wifi_set;
extern T_SysCfgGetFp sys_cfg_wifi_get;
extern T_SysCfgCommFp sys_cfg_rsp_done;
extern T_SysCfgCommFp sys_cfg_clk_init;
extern T_SysCfgClkSetFp sys_cfg_clk_set;
extern T_SysCfgM0SetFp sys_cfg_auxadc_set;
extern T_SysCfgReqFp sys_cfg_auxadc_req;
extern T_SysCfgRspFp sys_cfg_auxadc_rsp;

int sys_cfg_lock_impl(void);
int sys_cfg_unlock_impl(void);
int sys_cfg_m0_send_impl(uint8_t u8Type, void *pMsg, uint32_t u32Len);
int sys_cfg_rf_set_impl(void *pCfg, uint8_t u8SetM0);
int sys_cfg_rf_get_impl(void *pCfg);
int sys_cfg_rf_init_impl(void *pCfg);
int sys_cfg_init_impl(void);
int sys_cfg_rsp_wait_impl(void);
int sys_cfg_rsp_done_impl(void);
int sys_cfg_ts_enable_impl(void *pCfg);
int sys_cfg_clk_init_impl(void);
int sys_cfg_clk_src_get_impl(uint32_t u32ClkIdx, uint32_t *pu32ClkSrc);
int sys_cfg_clk_set_impl(T_SysCfgClkIdx tClkIdx);
int sys_cfg_auxadc_set_impl(void *pCfg);
int sys_cfg_auxadc_req_impl(void *pReq, void *pRsp);
int sys_cfg_auxadc_rsp_impl(uint8_t u8Status, void *pRsp);

#include "sys_cfg_patch.h"
#endif //#ifndef __SYS_CFG_H__

