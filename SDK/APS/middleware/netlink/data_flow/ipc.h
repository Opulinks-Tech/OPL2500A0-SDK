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

#ifndef __IPC_H__
#define __IPC_H__

#include <stdio.h>
#include <stdint.h>

#define ENHANCE_IPC

#define IPC_WIFI_DESC
#define IPC_WIFI_TX_DESC_IN_MSQ_RAM
#define IPC_WIFI_RX_DESC_IN_MSQ_RAM
#define IPC_WIFI_TX_BUF_IN_MSQ_RAM
//#define IPC_WIFI_RX_BUF_IN_MSQ_RAM

//#define IPC_WIFI_DESC_DUMP
//#define IPC_WIFI_DESC_CMD
//#define IPC_WIFI_RX_DESC_RESERVE
//#define IPC_WIFI_RX_DESC_RESERVE_LEN    (64)
//#define IPC_WIFI_RX_DESC_SEARCH_PARTIAL

//#define IPC_MSQ
//#define IPC_SUT

#ifdef IPC_MSQ
    #define IPC_LOGI(...)
    #define IPC_LOGE(...)
#else
    #include "msg.h"

    #define IPC_LOGI(...)
    #define IPC_LOGE                ipc_printf
#endif

#ifdef IPC_SUT
    #ifdef IPC_MSQ
        #define IPC_LOGT            printf
    #else
        #define IPC_LOGT(args...)   tracer_cli(LOG_MED_LEVEL, args)
    #endif

    #undef IPC_LOGE
    #define IPC_LOGE            IPC_LOGT
#endif

#ifdef IPC_MSQ
    #define IPC_LOGC            printf
#else
    #define IPC_LOGC(args...)   tracer_cli(LOG_MED_LEVEL, args)
#endif

#if 1
    #define IPC_VAL_INC(var, value, min_value, max_value)   \
    {   \
        var = value + 1;\
        \
        if(var >= max_value)    \
        {   \
            var = min_value;    \
        }   \
    }
#else
    #define IPC_VAL_INC(var, value, min_value, max_value)   \
    {   \
        var = ((value + 1) >= max_value)?(min_value):(value + 1);   \
    }
#endif

#define IPC_DESC_HEADER_ADDR_GET(ptDesc)            ((uint8_t *)(ptDesc->u8aBufAddr + (ptDesc->u8MaxHeaderLen - ptDesc->u8HeaderLen)))
#define IPC_DESC_TOTAL_LEN_GET(ptDesc)              (ptDesc->u8HeaderLen + ptDesc->u32DataLen)

#define IPC_SPARE0_APS_PATCH_FINISHED_BIT           3
#define IPC_SPARE0_APS_PATCH_FINISHED_MSK           (1UL << IPC_SPARE0_APS_PATCH_FINISHED_BIT)
#define IPC_SPARE0_MSQ_INIT_DONE_BIT                4
#define IPC_SPARE0_MSQ_INIT_DONE_MSK                (1UL << IPC_SPARE0_MSQ_INIT_DONE_BIT)
#define IPC_SPARE0_MSQ_DEEP_SLEEP_BIT               5
#define IPC_SPARE0_MSQ_DEEP_SLEEP_MSK               (1UL << IPC_SPARE0_MSQ_DEEP_SLEEP_BIT)
/* Bit 6: For RF access flash. Not implemented */
#define IPC_SPARE0_APS_WARMBOOT_CLK_READY_BIT       7
#define IPC_SPARE0_APS_WARMBOOT_CLK_READY_MSK       (1UL << IPC_SPARE0_APS_WARMBOOT_CLK_READY_BIT)
#define IPC_SPARE0_BOOT_TRACE_DBG_OUT_BIT           8
#define IPC_SPARE0_BOOT_TRACE_DBG_OUT_MSK           (1UL << IPC_SPARE0_BOOT_TRACE_DBG_OUT_BIT)
/* bit 9~10 reserved */
#define IPC_SPARE0_PS_APS_MASTER_BIT                11
#define IPC_SPARE0_PS_APS_MASTER_MASK               (1UL << IPC_SPARE0_PS_APS_MASTER_BIT)
/* 32k clock source selection */
#define IPC_SPARE0_32K_SRC_SEL_BIT                  12
#define IPC_SPARE0_32K_SRC_SEL_MASK                 (1UL << IPC_SPARE0_32K_SRC_SEL_BIT)

#define IPC_SPARE0_FPGA_0_BIT                       16


#if defined(OPL2500_APS) || defined(OPL2500_MSQ)
#define IPC_SHARED_MEM_ADDR         SHARE_RAM_BASE
#define IPC_SHARED_MEM_SIZE         0x00010000
#elif defined(__NL1000_An__)
#define IPC_SHARED_MEM_ADDR         0x80000000
#define IPC_SHARED_MEM_SIZE         0x00010000
#else   // __NL1000_TC__
#define IPC_SHARED_MEM_ADDR         0x00458000
#define IPC_SHARED_MEM_SIZE         0x00008000
#endif

#define IPC_ADDR_ALIGN(addr, byte)  ((((uint32_t)(addr) + byte - 1) & ~(uint32_t)(byte - 1)))
#define IPC_VALUE_ALIGN             IPC_ADDR_ALIGN

#define IPC_DEF_RB_NUM              1
#define IPC_WIFI_APS_TX_RB_NUM      5
#define IPC_BLE_TX_RB_NUM           8

#define IPC_DEF_BUF_NUM             8   // must be power of 2
#define IPC_WIFI_RB_BUF_NUM         32  // must be power of 2

#define IPC_CMD_DATA_SIZE           64 // for common command
#define IPC_EVT_DATA_SIZE           64 // for common event

#define IPC_BLE_CMD_DATA_SIZE       260 // 4-bytes alignment for 258
#define IPC_BLE_EVT_DATA_SIZE       260 // 4-bytes alignment for 257

#define IPC_ENTRY_INFO_SIZE         sizeof(T_IpcEntryInfo)
#define IPC_WIFI_ENTRY_INFO_SIZE    sizeof(T_IpcWifiEntryInfo)

#define IPC_COMM_RB_BUF_NUM         16
#define IPC_CMD_BUF_NUM             8
#define IPC_EVT_BUF_NUM             8
#define IPC_BLE_CMD_BUF_NUM         2
#define IPC_BLE_EVT_BUF_NUM         4
#define IPC_BLE_TX_BUF_NUM          16
#define IPC_BLE_RX_BUF_NUM          4
#define IPC_WIFI_CMD_BUF_NUM        8
#define IPC_WIFI_EVT_BUF_NUM        8

#ifdef IPC_WIFI_DESC
    #define IPC_WIFI_MSQ_TX_DESC_NUM    16
    #define IPC_WIFI_MSQ_RX_DESC_NUM    16
    #define IPC_WIFI_APS_TX_DESC_NUM    16
    #define IPC_WIFI_APS_RX_DESC_NUM    16
#else
    #define IPC_WIFI_MSQ_TX_DESC_NUM    4
    #define IPC_WIFI_MSQ_RX_DESC_NUM    4
    #define IPC_WIFI_APS_TX_DESC_NUM    4
    #define IPC_WIFI_APS_RX_DESC_NUM    4
#endif

#define IPC_WIFI_MSQ_TX_ENTRY_NUM       IPC_WIFI_MSQ_TX_DESC_NUM
#define IPC_WIFI_MSQ_RX_ENTRY_NUM       IPC_WIFI_MSQ_RX_DESC_NUM
#define IPC_WIFI_APS_TX_ENTRY_NUM       IPC_WIFI_APS_TX_DESC_NUM
#define IPC_WIFI_APS_RX_ENTRY_NUM       IPC_WIFI_APS_RX_DESC_NUM

// number_of_ipc_entry >= number_of_wifi_entry
#define IPC_MSQ_TX_ENTRY_NUM            IPC_WIFI_MSQ_TX_ENTRY_NUM
#define IPC_MSQ_RX_ENTRY_NUM            IPC_WIFI_MSQ_RX_ENTRY_NUM
#define IPC_APS_TX_ENTRY_NUM            IPC_WIFI_APS_TX_ENTRY_NUM
#define IPC_APS_RX_ENTRY_NUM            IPC_WIFI_APS_RX_ENTRY_NUM

#ifdef IPC_SUT
#define IPC_SUT_BUF_SIZE                sizeof(T_IpcSutBuf) //(IPC_SUT_DATA_SIZE + sizeof(uint32_t))
#define IPC_SUT_BUF_NUM                 4
#define IPC_SUT_DATA_SIZE               IPC_VALUE_ALIGN(800 + IPC_ENTRY_INFO_SIZE, 4)
#endif

// For message from M0 to M3
#define IPC_M0_MSG_BUF_NUM              8

// For message from M3 to M0
#define IPC_M3_MSG_BUF_NUM              8

// For WiFi BSS Info
#define IPC_WIFI_BSS_INFO_START         g_u32IpcWifiBssInfoAddr

// For DBG
#define IPC_DBG_TRX_PARAM_START         g_u32IpcWifiDbgParamAddr

// For WiFi STA Info
#define IPC_WIFI_MIB_START              g_u32IpcWifiMibAddr

// For PS module ps_conf
#define IPC_PS_CONF_START               g_u32IpcPsConfAddr

// For Sleep IO config
#define IPC_SLEEP_IO_START              g_u32IpcSleepIoCfgAddr

// For SW Version info
#define IPC_SW_VER_INFO_START           g_u32IpcSwVerInfoAddr

#ifdef IPC_SUT

    #define IPC_CMD_SEND                ipc_dummy_send
    #define IPC_CMD_COUNT_GET           ipc_dummy_count_get_1

    #define IPC_EVT_SEND                ipc_dummy_send
    #define IPC_EVT_READ                ipc_dummy_get_2
    #define IPC_EVT_READ_DONE           ipc_dummy_2
    #define IPC_EVT_COUNT_GET           ipc_dummy_count_get_1

    #define IPC_BLE_CMD_SEND            ipc_dummy_send
    #define IPC_BLE_CMD_COUNT_GET       ipc_dummy_count_get_1

    #define IPC_BLE_EVT_SEND            ipc_dummy_send
    #define IPC_BLE_EVT_READ            ipc_dummy_get_2
    #define IPC_BLE_EVT_READ_DONE       ipc_dummy_2
    #define IPC_BLE_EVT_COUNT_GET       ipc_dummy_count_get_1

    #define IPC_BLE_TX_WRITE            ipc_dummy_get_1
    #define IPC_BLE_TX_WRITE_DONE       ipc_dummy_1
    #define IPC_BLE_TX_READ             ipc_dummy_get_1
    #define IPC_BLE_TX_READ_DONE        ipc_dummy_1
    #define IPC_BLE_TX_NEXT_DATA_EXIST  ipc_dummy_exist
    #define IPC_BLE_TX_COUNT_GET        ipc_dummy_count_get_2

    #define IPC_BLE_RX_WRITE            ipc_dummy_get_1
    #define IPC_BLE_RX_WRITE_DONE       ipc_dummy_1
    #define IPC_BLE_RX_READ             ipc_dummy_get_1
    #define IPC_BLE_RX_READ_DONE        ipc_dummy_1
    #define IPC_BLE_RX_COUNT_GET        ipc_dummy_count_get_2

    #define IPC_WIFI_CMD_SEND           ipc_dummy_send
    #define IPC_WIFI_CMD_COUNT_GET      ipc_dummy_count_get_1

    #define IPC_WIFI_EVT_SEND           ipc_dummy_send
    #define IPC_WIFI_EVT_COUNT_GET      ipc_dummy_count_get_1
#else //#ifdef IPC_SUT
    #define IPC_CMD_SEND                ipc_cmd_send
    #define IPC_CMD_COUNT_GET           ipc_cmd_count_get

    #define IPC_EVT_SEND                ipc_evt_send
    #define IPC_EVT_READ                ipc_evt_read
    #define IPC_EVT_READ_DONE           ipc_evt_read_done
    #define IPC_EVT_COUNT_GET           ipc_evt_count_get

    #define IPC_BLE_CMD_SEND            ipc_ble_cmd_send
    #define IPC_BLE_CMD_COUNT_GET       ipc_ble_cmd_count_get

    #define IPC_BLE_EVT_SEND            ipc_ble_evt_send
    #define IPC_BLE_EVT_READ            ipc_ble_evt_read
    #define IPC_BLE_EVT_READ_DONE       ipc_ble_evt_read_done
    #define IPC_BLE_EVT_COUNT_GET       ipc_ble_evt_count_get

    #define IPC_BLE_TX_WRITE            ipc_ble_tx_write
    #define IPC_BLE_TX_WRITE_DONE       ipc_ble_tx_write_done
    #define IPC_BLE_TX_READ             ipc_ble_tx_read
    #define IPC_BLE_TX_READ_DONE        ipc_ble_tx_read_done
    #define IPC_BLE_TX_NEXT_DATA_EXIST  ipc_ble_tx_next_data_exist
    #define IPC_BLE_TX_COUNT_GET        ipc_ble_tx_count_get

    #define IPC_BLE_RX_WRITE            ipc_ble_rx_write
    #define IPC_BLE_RX_WRITE_DONE       ipc_ble_rx_write_done
    #define IPC_BLE_RX_READ             ipc_ble_rx_read
    #define IPC_BLE_RX_READ_DONE        ipc_ble_rx_read_done
    #define IPC_BLE_RX_COUNT_GET        ipc_ble_rx_count_get

    #define IPC_WIFI_CMD_SEND           ipc_wifi_cmd_send
    #define IPC_WIFI_CMD_COUNT_GET      ipc_wifi_cmd_count_get

    #define IPC_WIFI_EVT_SEND           ipc_wifi_evt_send
    #define IPC_WIFI_EVT_COUNT_GET      ipc_wifi_evt_count_get
#endif //#ifdef IPC_SUT

#define ipc_wifi_msq_tx_set             ipc_wifi_msq_tx_write
#define ipc_wifi_msq_tx_set_done        ipc_wifi_msq_tx_write_done
#define ipc_wifi_msq_tx_get             ipc_wifi_msq_tx_read
#define ipc_wifi_msq_tx_get_done        ipc_wifi_msq_tx_read_done

#define ipc_wifi_msq_rx_set             ipc_wifi_msq_rx_write
#define ipc_wifi_msq_rx_set_done        ipc_wifi_msq_rx_write_done
#define ipc_wifi_msq_rx_get             ipc_wifi_msq_rx_read
#define ipc_wifi_msq_rx_get_done        ipc_wifi_msq_rx_read_done

#define ipc_wifi_aps_tx_set             ipc_wifi_aps_tx_write
#define ipc_wifi_aps_tx_set_done        ipc_wifi_aps_tx_write_done
#define ipc_wifi_aps_tx_get             ipc_wifi_aps_tx_read
#define ipc_wifi_aps_tx_get_done        ipc_wifi_aps_tx_read_done

#define ipc_wifi_aps_rx_set             ipc_wifi_aps_rx_write
#define ipc_wifi_aps_rx_set_done        ipc_wifi_aps_rx_write_done
#define ipc_wifi_aps_rx_get             ipc_wifi_aps_rx_read
#define ipc_wifi_aps_rx_get_done        ipc_wifi_aps_rx_read_done


#define IPC_RB_RESERVE(rb, buf) \
{ \
    buf = rb->pdwaBuf[*(rb->pdwWrite) & rb->dwMask]; \
}

#define IPC_RB_RESERVE_DONE(rb) \
{ \
    *(rb->pdwWrite) += 1; \
}

#define IPC_RB_PROCESS(rb, buf) \
{ \
    buf = rb->pdwaBuf[*(rb->pdwRead) & rb->dwMask]; \
}

#define IPC_RB_PROCESS_DONE(rb) \
{ \
    *(rb->pdwRead) += 1; \
}

#define IPC_RB_PROCESS_GET(rb, buf, idx) \
{ \
    buf = rb->pdwaBuf[(*(rb->pdwRead) + idx) & rb->dwMask]; \
}

#define IPC_RB_FULL(rb) \
    (((uint32_t)(*(rb->pdwWrite) - *(rb->pdwRead))) >= rb->dwNum)

#define IPC_RB_EMPTY(rb) \
    (*(rb->pdwWrite) == *(rb->pdwRead))

#define IPC_RB_COUNT(rb) \
    (((uint32_t)(*(rb->pdwWrite) - *(rb->pdwRead))) & rb->dwMask)


// must be 4-byte aligned
#define IPC_WIFI_MSQ_TX_MEM_BLK_SIZE        528
#define IPC_WIFI_MSQ_RX_MEM_BLK_SIZE        528
#define IPC_WIFI_APS_TX_MEM_BLK_SIZE        528
#define IPC_WIFI_APS_RX_MEM_BLK_SIZE        528

// must be 4-byte aligned
#define IPC_WIFI_MSQ_RX_LAST_MEM_BLK_SIZE   3844
#define IPC_WIFI_APS_RX_LAST_MEM_BLK_SIZE   3844

// max header length must be 4-byte aligned and smaller than block size
#define IPC_WIFI_MSQ_TX_MAX_HEADER_LEN      0
#define IPC_WIFI_MSQ_RX_MAX_HEADER_LEN      0
#define IPC_WIFI_APS_TX_MAX_HEADER_LEN      0
#define IPC_WIFI_APS_RX_MAX_HEADER_LEN      0


#ifdef IPC_MSQ
    #include "trx_generic.h"
#else

// DMA TX
#define TRX_RG_RD_ADDR_SFT                    0
#define TRX_RG_RD_ADDR_MSK                    (0XFFFFFFFF << TRX_RG_RD_ADDR_SFT)

#define TRX_RG_WR_ADDR_SFT                    0
#define TRX_RG_WR_ADDR_MSK                    (0XFFFFFFFF << TRX_RG_WR_ADDR_SFT)

#define TRX_RG_CMD_SET_INT_SFT                0
#define TRX_RG_CMD_SET_INT_MSK                (0X1 << TRX_RG_CMD_SET_INT_SFT)
#define TRX_RG_CMD_LAST_SFT                   1
#define TRX_RG_CMD_LAST_MSK                   (0X1 << TRX_RG_CMD_LAST_SFT)
#define TRX_RG_CMD_INT_FLG_SFT                2
#define TRX_RG_CMD_INT_FLG_MSK                (0X1 << TRX_RG_CMD_INT_FLG_SFT)
#define TRX_RG_CMD_NXT_ADDR_SFT               4
#define TRX_RG_CMD_NXT_ADDR_MSK               (0XFFFFFFFU << TRX_RG_CMD_NXT_ADDR_SFT)

#define TRX_RG_DAT_LEN_SFT                    0
#define TRX_RG_DAT_LEN_MSK                    (0XFFFF << TRX_RG_DAT_LEN_SFT)
#define TRX_TX_BUF_RDY_SFT                    16
#define TRX_TX_BUF_RDY_MSK                    (0X1 << TRX_TX_BUF_RDY_SFT)
#define TRX_RG_DMA_EN_P_SFT                   30
#define TRX_RG_DMA_EN_P_MSK                   (0X1 << TRX_RG_DMA_EN_P_SFT)
#define TRX_RG_FLUSH_P_SFT                    31
#define TRX_RG_FLUSH_P_MSK                    (0X1U << TRX_RG_FLUSH_P_SFT)

// DMA RX
#define TRX_RX_MSQ_BUF_ADDR_SFT               0
#define TRX_RX_MSQ_BUF_ADDR_MSK               (0XFFFFFFFF << TRX_RX_MSQ_BUF_ADDR_SFT)

#define TRX_RX_MSQ_CMD_LAST_SFT               1
#define TRX_RX_MSQ_CMD_LAST_MSK               (0X1 << TRX_RX_MSQ_CMD_LAST_SFT)
#define TRX_RX_MSQ_CMD_NXT_ADDR_SFT           4
#define TRX_RX_MSQ_CMD_NXT_ADDR_MSK           (0XFFFFFFFU << TRX_RX_MSQ_CMD_NXT_ADDR_SFT)

#define TRX_RX_MSQ_DAT_LEN_SFT                0
#define TRX_RX_MSQ_DAT_LEN_MSK                (0XFFFF << TRX_RX_MSQ_DAT_LEN_SFT)
#define TRX_RX_MSQ_BUF_RDY_SFT                16
#define TRX_RX_MSQ_BUF_RDY_MSK                (0X1 << TRX_RX_MSQ_BUF_RDY_SFT)
#define TRX_RX_MSQ_HEAD_SFT                   17
#define TRX_RX_MSQ_HEAD_MSK                   (0X1 << TRX_RX_MSQ_HEAD_SFT)

typedef struct
{
    uint32_t      u32RdAddr;      // Address that TRX will read data then send to PHY for TX
    uint32_t      u32WrAddr;      // Not used
    uint32_t      u32CmdCtrl;
    uint32_t      u32DatLen;
} S_TRX_DMA_TX;

typedef struct
{
    uint32_t      u32Reserved;
    uint32_t      u32WrAddr;      // Address that TRX will write data into
    uint32_t      u32CmdCtrl;
    uint32_t      u32DatLen;
} S_TRX_DMA_RX;

#endif //#ifdef IPC_MSQ


#define IPC_DMA_TX_RD_ADDR_GET(Dma)         ((Dma.u32RdAddr & TRX_RG_RD_ADDR_MSK) >> TRX_RG_RD_ADDR_SFT)
#define IPC_DMA_TX_RD_ADDR_SET(Dma, addr)   (Dma.u32RdAddr = ((uint32_t)(addr) & TRX_RG_RD_ADDR_MSK))
#define IPC_DMA_TX_WR_ADDR_GET(Dma)         ((Dma.u32WrAddr & TRX_RG_WR_ADDR_MSK) >> TRX_RG_WR_ADDR_SFT)
#define IPC_DMA_TX_WR_ADDR_SET(Dma, addr)   (Dma.u32WrAddr = ((uint32_t)(addr) & TRX_RG_WR_ADDR_MSK))
#define IPC_DMA_TX_CMD_LAST_GET(Dma)        ((Dma.u32CmdCtrl & TRX_RG_CMD_LAST_MSK) >> TRX_RG_CMD_LAST_SFT)
#define IPC_DMA_TX_CMD_LAST_SET(Dma)        (Dma.u32CmdCtrl |= TRX_RG_CMD_LAST_MSK)
#define IPC_DMA_TX_CMD_LAST_CLR(Dma)        (Dma.u32CmdCtrl &= ~TRX_RG_CMD_LAST_MSK)
#define IPC_DMA_TX_NXT_ADDR_GET(Dma)        (Dma.u32CmdCtrl & TRX_RG_CMD_NXT_ADDR_MSK)
#define IPC_DMA_TX_NXT_ADDR_SET(Dma, addr)  (Dma.u32CmdCtrl = ((uint32_t)(addr) & TRX_RG_CMD_NXT_ADDR_MSK) | (Dma.u32CmdCtrl & ~TRX_RG_CMD_NXT_ADDR_MSK))
#define IPC_DMA_TX_DAT_LEN_GET(Dma)         ((Dma.u32DatLen & TRX_RG_DAT_LEN_MSK) >> TRX_RG_DAT_LEN_SFT)
#define IPC_DMA_TX_DAT_LEN_SET(Dma, len)    (Dma.u32DatLen = (((len) << TRX_RG_DAT_LEN_SFT) & TRX_RG_DAT_LEN_MSK) | (Dma.u32DatLen & ~TRX_RG_DAT_LEN_MSK))
#define IPC_DMA_TX_BUF_RDY_GET(Dma)         ((Dma.u32DatLen & TRX_TX_BUF_RDY_MSK) >> TRX_TX_BUF_RDY_SFT)
#define IPC_DMA_TX_BUF_RDY_SET(Dma)         (Dma.u32DatLen |= TRX_TX_BUF_RDY_MSK)
#define IPC_DMA_TX_BUF_RDY_CLR(Dma)         (Dma.u32DatLen &= ~TRX_TX_BUF_RDY_MSK)

#define IPC_DMA_RX_WR_ADDR_GET(Dma)         ((Dma.u32WrAddr & TRX_RX_MSQ_BUF_ADDR_MSK) >> TRX_RX_MSQ_BUF_ADDR_SFT)
#define IPC_DMA_RX_WR_ADDR_SET(Dma, addr)   (Dma.u32WrAddr = ((uint32_t)(addr) & TRX_RX_MSQ_BUF_ADDR_MSK))
#define IPC_DMA_RX_CMD_LAST_GET(Dma)        ((Dma.u32CmdCtrl & TRX_RX_MSQ_CMD_LAST_MSK) >> TRX_RX_MSQ_CMD_LAST_SFT)
#define IPC_DMA_RX_CMD_LAST_SET(Dma)        (Dma.u32CmdCtrl |= TRX_RX_MSQ_CMD_LAST_MSK)
#define IPC_DMA_RX_CMD_LAST_CLR(Dma)        (Dma.u32CmdCtrl &= ~TRX_RX_MSQ_CMD_LAST_MSK)
#define IPC_DMA_RX_NXT_ADDR_GET(Dma)        (Dma.u32CmdCtrl & TRX_RX_MSQ_CMD_NXT_ADDR_MSK)
#define IPC_DMA_RX_NXT_ADDR_SET(Dma, addr)  (Dma.u32CmdCtrl = ((uint32_t)(addr) & TRX_RX_MSQ_CMD_NXT_ADDR_MSK) | (Dma.u32CmdCtrl & ~TRX_RX_MSQ_CMD_NXT_ADDR_MSK))
#define IPC_DMA_RX_DAT_LEN_GET(Dma)         ((Dma.u32DatLen & TRX_RX_MSQ_DAT_LEN_MSK) >> TRX_RX_MSQ_DAT_LEN_SFT)
#define IPC_DMA_RX_DAT_LEN_SET(Dma, len)    (Dma.u32DatLen = (((len) << TRX_RX_MSQ_DAT_LEN_SFT) & TRX_RX_MSQ_DAT_LEN_MSK) | (Dma.u32DatLen & ~TRX_RX_MSQ_DAT_LEN_MSK))
#define IPC_DMA_RX_BUF_RDY_GET(Dma)         ((Dma.u32DatLen & TRX_RX_MSQ_BUF_RDY_MSK) >> TRX_RX_MSQ_BUF_RDY_SFT)
#define IPC_DMA_RX_BUF_RDY_SET(Dma)         (Dma.u32DatLen |= TRX_RX_MSQ_BUF_RDY_MSK)
#define IPC_DMA_RX_BUF_RDY_CLR(Dma)         (Dma.u32DatLen &= ~TRX_RX_MSQ_BUF_RDY_MSK)


#pragma anon_unions

typedef int (*T_IpcHandleFp)(uint32_t dwType, uint32_t dwIdx, void *pBuf, uint32_t dwBufSize);

typedef enum
{
    IPC_TYPE_CMD = 0,
    IPC_TYPE_EVT,

    IPC_TYPE_COMMON_END = IPC_TYPE_EVT,

    IPC_TYPE_BLE_CMD,
    IPC_TYPE_BLE_EVT,
    IPC_TYPE_BLE_TX,
    IPC_TYPE_BLE_RX,

    IPC_TYPE_BLE_END = IPC_TYPE_BLE_RX,

    IPC_TYPE_WIFI_CMD,
    IPC_TYPE_WIFI_EVT,
    IPC_TYPE_WIFI_MSQ_TX,
    IPC_TYPE_WIFI_MSQ_RX,
    IPC_TYPE_WIFI_APS_TX,
    IPC_TYPE_WIFI_APS_RX,

    IPC_TYPE_WIFI_END = IPC_TYPE_WIFI_APS_RX,

    IPC_TYPE_M0_MSG,
    IPC_TYPE_M3_MSG,

    IPC_TYPE_MAX,

    #ifdef IPC_SUT
    IPC_TYPE_PROC_ENABLE,
    IPC_TYPE_PROC_DISABLE,
    IPC_TYPE_HANDLE_CLEAR,
    IPC_TYPE_HANDLE_RESTORE,
    IPC_TYPE_MEM_WRITE,
    IPC_TYPE_INT_TEST,
    #endif
} T_IpcType;

typedef enum
{
    IPC_RET_DONE = 0,
    IPC_RET_ONGOING,

    IPC_RET_MAX
} T_IpcRetCode;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[IPC_CMD_DATA_SIZE];
} T_IpcCmdBuf;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[IPC_EVT_DATA_SIZE];
} T_IpcEvtBuf;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[IPC_BLE_CMD_DATA_SIZE];
} T_IpcBleCmdBuf;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[IPC_BLE_EVT_DATA_SIZE];
} T_IpcBleEvtBuf;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[];
} T_IpcWifiCmdBuf;

typedef struct
{
    uint32_t dwType;
    uint8_t baData[];
} T_IpcWifiEvtBuf;

typedef struct
{
    void *pRb;
    uint32_t dwRbSize;
    uint32_t dwRbNum;
    uint32_t dwWriteAddr;
    uint32_t dwReadAddr;
    uint32_t dwBufStartAddr;
    uint32_t dwBufSize;
    uint32_t dwBufNum;
    T_IpcHandleFp fpHandle;
} T_IpcCb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_COMM_RB_BUF_NUM];
} T_IpcCommRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_DEF_BUF_NUM];
} T_IpcDefRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_BLE_TX_BUF_NUM];
} T_IpcBleTxRb;

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_WIFI_RB_BUF_NUM];
} T_IpcWifiRb;

typedef struct
{
    uint32_t u32MsqTxDescPoolAddr;
    uint32_t u32MsqRxDescPoolAddr;

    uint32_t u32ApsTxDescPoolAddr;
    uint32_t u32ApsRxDescPoolAddr;

    uint32_t u32MsqTxMemPoolAddr;
    uint32_t u32MsqRxMemPoolAddr;

    uint32_t u32ApsTxMemPoolAddr;
    uint32_t u32ApsRxMemPoolAddr;
} T_IpcWifiDescInfo;

typedef struct
{
    uint8_t u8Used;
    uint8_t u8aPadding[3];
} T_IpcWifiEntryInfo;

typedef struct
{
    uint8_t u8Processed;
    uint8_t u8aPadding[3];
} T_IpcEntryInfo;

typedef enum
{
    IPC_WIFI_DESC_IDX_MSQ_TX = 0,
    IPC_WIFI_DESC_IDX_MSQ_RX,
    IPC_WIFI_DESC_IDX_APS_TX,
    IPC_WIFI_DESC_IDX_APS_RX,

    IPC_WIFI_DESC_IDX_MAX
} T_IpcWifiDescIdx;

typedef enum
{
    IPC_WIFI_ENTRY_IDX_MSQ_TX = 0,
    IPC_WIFI_ENTRY_IDX_MSQ_RX,
    IPC_WIFI_ENTRY_IDX_APS_TX,
    IPC_WIFI_ENTRY_IDX_APS_RX,

    IPC_WIFI_ENTRY_IDX_MAX
} T_IpcWifiEntryIdx;

typedef struct S_IpcWifiDesc
{
    // dma cmd
    union
    {
        S_TRX_DMA_TX tDmaTx;
        S_TRX_DMA_RX tDmaRx;
    };

    struct S_IpcWifiDesc *ptNext;   // read-only after initialization
    uint8_t *u8aBufAddr;            // read-only after initialization
    uint16_t u16BufSize;            // read-only after initialization
    uint16_t u16DescNum;

    uint8_t *u8aDataAddr;
    uint32_t u32DataLen;

    uint8_t u8MaxHeaderLen;
    uint8_t u8HeaderLen;            // Do not write directly. Update it with ipc_wifi_header_len_update
    uint8_t u8DataOffset;           // For RX data
    uint8_t bUsed:1;
    uint8_t bIsFirst:1;
    uint8_t bIsLast:1;
    uint8_t bMoreDesc:1;
    uint8_t bReserved:4;

    uint8_t u8SeqNo;
    uint8_t u8aPadding[7];

    // 16-bytes aligned and sizeof(T_IpcWifiDesc) must be mutiple of 16
} T_IpcWifiDesc;

typedef struct
{
    T_IpcWifiDesc *taDescPool;
    uint16_t u16DescNum;
    uint16_t u16SearchStart;
    uint8_t *u8aMemPool;
    uint16_t u16MemBlkSize;
    uint16_t u16LastMemBlkSize;
    uint8_t u8CurrSeqNo;
    uint8_t u8MaxHeaderLen;
} T_IpcWifiDescCb;

typedef struct
{
    uint8_t *u8aPool;
    uint16_t u16Num;
    uint16_t u16Size;
    uint16_t u16SearchStart;
} T_IpcWifiEntryCb;

typedef struct
{
    void *pEntry;
} T_IpcDefEntry;

// internal
typedef void (*T_IpcRbProcFp)(void *pRb);
typedef void *(*T_IpcRbReadWriteBufGetFp)(void *pRb);
typedef void (*T_IpcRbInitFp)(uint32_t u32Idx, T_IpcCb *ptIpcCb);
typedef uint32_t (*T_IpcRbCountGetFp)(void *pRb, uint32_t *pdwNum);
typedef void *(*T_IpcRbReadBufGetByIdxFp)(void *pRb, uint32_t dwIdx);
typedef void *(*T_IpcRbWriteBufGetByIdxFp)(void *pRb, uint32_t dwIdx);
typedef void (*T_IpcRbWriteDoneFp)(void *pRb, uint8_t bInd);
typedef uint8_t (*T_IpcRbCheckFp)(void *pRb);
typedef void (*T_IpcSetFp)(uint8_t bMode);
typedef void (*T_IpcRbWriteDoneByNumFp)(void *pRb, uint8_t u8Ind, uint32_t u32Num);
typedef void (*T_IpcRbReadDoneByNumFp)(void *pRb, uint32_t u32Num);
typedef void (*T_IpcEntryInfoSetFp)(uint8_t u8IsWrite, void *pRb, void *pEntry);
typedef T_IpcEntryInfo *(*T_IpcEntryInfoGetFp)(void *pRb, void *pEntry);
typedef int (*T_IpcPrintfFp)(char *sFmt, ...);

// wifi descriptor
//typedef uint32_t (*T_IpcWifiDescInitFp)(uint32_t u32StartAddr);
typedef void (*T_IpcWifiDescInitFp)(uint8_t u8Idx);
typedef T_IpcWifiDesc *(*T_IpcWifiRxDescAllocMaxFp)(T_IpcWifiDescCb *ptDescCb, uint32_t u32Limit);
typedef T_IpcWifiDesc *(*T_IpcWifiDescAllocFp)(T_IpcWifiDescCb *ptDescCb, uint32_t u32DataLen);
typedef int (*T_IpcWifiDescCheckFp)(T_IpcWifiDescCb *ptDescCb, T_IpcWifiDesc *ptDesc);
typedef int (*T_IpcWifiDescFreeFp)(T_IpcWifiDescCb *ptDescCb, T_IpcWifiDesc *ptDesc);
typedef int (*T_IpcWifiTxDescShrinkIntFp)(T_IpcWifiDescCb *ptDescCb, T_IpcWifiDesc *ptDesc, uint32_t u32DataLen);
typedef int (*T_IpcWifiTxDescUpdateIntFp)(T_IpcWifiDescCb *ptDescCb, T_IpcWifiDesc *ptDesc);
typedef int (*T_IpcWifiRxDescUpdateIntFp)(T_IpcWifiDescCb *ptDescCb, T_IpcWifiDesc *ptDesc, uint32_t u32DataLen, uint8_t u8FreeRemainDesc, T_IpcWifiDesc **pptRemainDesc);
typedef uint16_t (*T_IpcWifiFirstDescAvailSizeGetFp)(T_IpcWifiDesc *ptDesc);
typedef uint8_t *(*T_IpcWifiDataAddrGetFp)(uint8_t u8IsRx, T_IpcWifiDesc *ptDesc);
typedef uint32_t (*T_IpcWifiDataLenGetFp)(T_IpcWifiDescCb *ptDescCb, T_IpcWifiDesc *ptDesc);
typedef int (*T_IpcWifiDataOffsetUpdateFp)(T_IpcWifiDescCb *ptDescCb, T_IpcWifiDesc *ptDesc, uint8_t u8Offset);
typedef void (*T_IpcWifiDescDumpFp)(T_IpcWifiDescCb *ptDescCb);
typedef int (*T_IpcWifiHeaderLenUpdate)(uint8_t u8IsRx, T_IpcWifiDesc *ptDesc, uint8_t u8Len);
typedef int (*T_IpcWifiTxRxHeaderLenUpdate)(T_IpcWifiDesc *ptDesc, uint8_t u8Len);

// wifi entry
typedef uint32_t (*T_IpcWifiEntryInitFp)(uint32_t u32StartAddr);
typedef T_IpcWifiEntryInfo *(*T_IpcIntWifiEntryInfoGetFp)(void *pEntry, uint16_t u16EntrySize);
typedef void *(*T_IpcIntWifiEntryAllocFp)(T_IpcWifiEntryCb *ptEntryCb);
typedef int (*T_IpcIntWifiEntryFreeFp)(T_IpcWifiEntryCb *ptEntryCb, void *pEntry);
typedef void (*T_IpcWifiEntryDumpFp)(T_IpcWifiEntryCb *ptEntryCb);

// external
typedef void (*T_IpcCommonFp)(void);
typedef void (*T_IpcPeerReadyFp)(uint8_t bReady, uint8_t bRsp);
typedef int (*T_IpcSendFp)(uint32_t dwType, void *pData, uint32_t dwDataLen);
typedef uint32_t (*T_IpcCountGetFp)(uint32_t *pdwTotal);
typedef void *(*T_IpcReadWriteBufGetFp)(void);
typedef void *(*T_IpcBleReadWriteBufGetFp)(uint32_t dwHandle);
typedef void (*T_IpcBleReadWriteDoneFp)(uint32_t dwHandle);
typedef uint8_t (*T_IpcBleNextDataExistFp)(uint32_t dwHandle);
typedef uint32_t (*T_IpcBleCountGetFp)(uint32_t dwHandle, uint32_t *pdwTotal);
typedef uint32_t (*T_IpcWifiCountGetFp)(void);
typedef void (*T_IpcRbSetFp)(uint32_t dwType);
typedef void (*T_IpcDumpFp)(uint8_t bMode);
typedef void *(*T_IpcNextBufGetFp)(uint32_t u32Idx);
typedef void (*T_IpcApsTxResetFp)(uint32_t u32Handle);
typedef void *(*T_IpcApsTxReadWriteBufGetFp)(uint32_t u32Handle);
typedef void (*T_IpcApsTxReadWriteDoneFp)(uint32_t u32Handle);
typedef uint8_t (*T_IpcApsTxNextDataExistFp)(uint32_t u32Handle);
typedef uint32_t (*T_IpcApsTxCountGetFp)(uint32_t u32Handle);
typedef void *(*T_IpcApsTxNextBufGetFp)(uint32_t u32Handle, uint32_t u32Idx);
typedef int (*T_IpcApsTxCompleteFp)(uint32_t u32Handle, void *pWifiEntry);
typedef void (*T_IpcApsTxLockFp)(uint32_t u32Handle);
typedef void (*T_IpcApsTxUnlockFp)(uint32_t u32Handle);

// wifi descriptor
typedef T_IpcWifiDesc *(*T_IpcWifiApsMsqRxDescAllocMaxFp)(uint32_t u32Limit);
typedef T_IpcWifiDesc *(*T_IpcWifiTxRxDescAllocFp)(uint32_t u32DataLen);
typedef int (*T_IpcWifiTxRxDescFreeFp)(T_IpcWifiDesc *ptDesc);
typedef int (*T_IpcWifiRxDescUpdateFp)(T_IpcWifiDesc *ptDesc, uint32_t u32DataLen, uint8_t u8FreeRemainDesc, T_IpcWifiDesc **pptRemainDesc);
typedef int (*T_IpcWifiTxDescShrinkFp)(T_IpcWifiDesc *ptDesc, uint32_t u32DataLen);
typedef uint8_t *(*T_IpcWifiTxDataAddrGetFp)(T_IpcWifiDesc *ptDesc);
typedef uint8_t *(*T_IpcWifiRxDataAddrGetFp)(T_IpcWifiDesc *ptDesc);
//typedef uint32_t (*T_IpcWifiTxDataLenGetFp)(T_IpcWifiDesc *ptDesc);
typedef uint32_t (*T_IpcWifiRxDataLenGetFp)(T_IpcWifiDesc *ptDesc);
typedef int (*T_IpcWifiTxDataOffsetUpdateFp)(T_IpcWifiDesc *ptDesc, uint8_t u8Offset);
typedef int (*T_IpcWifiRxDataOffsetUpdateFp)(T_IpcWifiDesc *ptDesc, uint8_t u8Offset);
typedef void (*T_IpcCliCmdFp)(int argc, char *argv[]);

// wifi entry
typedef void *(*T_IpcWifiEntryAllocFp)(void);
typedef int (*T_IpcWifiEntryFreeFp)(void *pEntry);

extern uint32_t g_u32IpcWifiBssInfoAddr;
extern uint32_t g_u32IpcWifiDbgParamAddr;
extern uint32_t g_u32IpcWifiMibAddr;
extern uint32_t g_u32IpcPsConfAddr;
extern uint32_t g_u32IpcSleepIoCfgAddr;
extern uint32_t g_u32IpcSwVerInfoAddr;

extern T_IpcCommonFp ipc_init;
extern T_IpcPeerReadyFp ipc_peer_ready;
extern T_IpcCommonFp ipc_proc;
extern T_IpcDumpFp ipc_dump;
extern T_IpcSetFp ipc_enable;

// rf command
extern T_IpcSendFp ipc_cmd_send;
extern T_IpcCountGetFp ipc_cmd_count_get;

// rf event
extern T_IpcSendFp ipc_evt_send;
extern T_IpcReadWriteBufGetFp ipc_evt_read;
extern T_IpcCommonFp ipc_evt_read_done;
extern T_IpcCountGetFp ipc_evt_count_get;

// ble command
extern T_IpcSendFp ipc_ble_cmd_send;
extern T_IpcCountGetFp ipc_ble_cmd_count_get;

// ble event
extern T_IpcSendFp ipc_ble_evt_send;
extern T_IpcReadWriteBufGetFp ipc_ble_evt_read;
extern T_IpcCommonFp ipc_ble_evt_read_done;
extern T_IpcCountGetFp ipc_ble_evt_count_get;

// ble tx
extern T_IpcBleReadWriteBufGetFp ipc_ble_tx_write;
extern T_IpcBleReadWriteDoneFp ipc_ble_tx_write_done;
extern T_IpcBleNextDataExistFp ipc_ble_tx_next_data_exist;
extern T_IpcBleReadWriteBufGetFp ipc_ble_tx_read;
extern T_IpcBleReadWriteDoneFp ipc_ble_tx_read_done;
extern T_IpcBleCountGetFp ipc_ble_tx_count_get;

// ble rx
extern T_IpcBleReadWriteBufGetFp ipc_ble_rx_write;
extern T_IpcBleReadWriteDoneFp ipc_ble_rx_write_done;
extern T_IpcBleReadWriteBufGetFp ipc_ble_rx_read;
extern T_IpcBleReadWriteDoneFp ipc_ble_rx_read_done;
extern T_IpcBleCountGetFp ipc_ble_rx_count_get;

// wifi command
extern T_IpcSendFp ipc_wifi_cmd_send;
extern T_IpcCountGetFp ipc_wifi_cmd_count_get;

// wifi event
extern T_IpcSendFp ipc_wifi_evt_send;
extern T_IpcCountGetFp ipc_wifi_evt_count_get;

// wifi msq tx
extern T_IpcCommonFp ipc_wifi_msq_tx_reset;
extern T_IpcReadWriteBufGetFp ipc_wifi_msq_tx_write;
extern T_IpcCommonFp ipc_wifi_msq_tx_write_done;
extern T_IpcReadWriteBufGetFp ipc_wifi_msq_tx_read;
extern T_IpcCommonFp ipc_wifi_msq_tx_read_done;
extern T_IpcWifiCountGetFp ipc_wifi_msq_tx_count_get;

// wifi msq rx
extern T_IpcCommonFp ipc_wifi_msq_rx_reset;
extern T_IpcReadWriteBufGetFp ipc_wifi_msq_rx_write;
extern T_IpcCommonFp ipc_wifi_msq_rx_write_done;
extern T_IpcReadWriteBufGetFp ipc_wifi_msq_rx_read;
extern T_IpcCommonFp ipc_wifi_msq_rx_read_done;
extern T_IpcWifiCountGetFp ipc_wifi_msq_rx_count_get;

// wifi aps tx
extern T_IpcApsTxResetFp ipc_wifi_aps_tx_reset;

#if ((defined IPC_SUT) || (!defined IPC_MSQ))
extern T_IpcApsTxReadWriteBufGetFp ipc_wifi_aps_tx_write;
extern T_IpcApsTxReadWriteDoneFp ipc_wifi_aps_tx_write_done;
extern T_IpcApsTxCompleteFp ipc_wifi_aps_tx_complete;
#endif

extern T_IpcApsTxReadWriteBufGetFp ipc_wifi_aps_tx_read;
extern T_IpcApsTxReadWriteDoneFp ipc_wifi_aps_tx_read_done;
extern T_IpcApsTxCountGetFp ipc_wifi_aps_tx_count_get;

// wiff aps rx
extern T_IpcCommonFp ipc_wifi_aps_rx_reset;
extern T_IpcReadWriteBufGetFp ipc_wifi_aps_rx_write;
extern T_IpcCommonFp ipc_wifi_aps_rx_write_done;
extern T_IpcReadWriteBufGetFp ipc_wifi_aps_rx_read;
extern T_IpcCommonFp ipc_wifi_aps_rx_read_done;
extern T_IpcWifiCountGetFp ipc_wifi_aps_rx_count_get;

extern T_IpcReadWriteBufGetFp ipc_m0_msg_write;
extern T_IpcCommonFp ipc_m0_msg_write_done;
extern T_IpcReadWriteBufGetFp ipc_m3_msg_read;
extern T_IpcCommonFp ipc_m3_msg_read_done;

#if ((defined IPC_SUT) || (!defined IPC_MSQ))
extern T_IpcReadWriteBufGetFp ipc_m3_msg_write;
extern T_IpcCommonFp ipc_m3_msg_write_done;
extern T_IpcReadWriteBufGetFp ipc_m0_msg_read;
extern T_IpcCommonFp ipc_m0_msg_read_done;
#ifdef IPC_MSQ
#else
extern T_IpcApsTxLockFp ipc_wifi_aps_tx_lock;
extern T_IpcApsTxUnlockFp ipc_wifi_aps_tx_unlock;

extern T_IpcCommonFp ipc_m3_msg_lock;
extern T_IpcCommonFp ipc_m3_msg_unlock;

extern T_IpcCommonFp ipc_wifi_aps_tx_entry_lock;
extern T_IpcCommonFp ipc_wifi_aps_tx_entry_unlock;
extern T_IpcCommonFp ipc_wifi_aps_tx_desc_lock;
extern T_IpcCommonFp ipc_wifi_aps_tx_desc_unlock;
#endif
#endif

extern T_IpcCliCmdFp ipc_cli_cmd;
extern T_IpcPrintfFp ipc_printf;

// wifi descriptor
extern T_IpcWifiApsMsqRxDescAllocMaxFp ipc_wifi_msq_rx_desc_alloc_max;
extern T_IpcWifiApsMsqRxDescAllocMaxFp ipc_wifi_aps_rx_desc_alloc_max;
extern T_IpcWifiTxRxDescAllocFp ipc_wifi_msq_tx_desc_alloc;
//extern T_IpcWifiTxRxDescAllocFp ipc_wifi_msq_rx_desc_alloc;
extern T_IpcWifiTxRxDescAllocFp ipc_wifi_aps_tx_desc_alloc;
//extern T_IpcWifiTxRxDescAllocFp ipc_wifi_aps_rx_desc_alloc;
extern T_IpcWifiTxRxDescFreeFp ipc_wifi_msq_tx_desc_free;
extern T_IpcWifiTxRxDescFreeFp ipc_wifi_msq_rx_desc_free;
extern T_IpcWifiTxRxDescFreeFp ipc_wifi_aps_tx_desc_free;
extern T_IpcWifiTxRxDescFreeFp ipc_wifi_aps_rx_desc_free;
extern T_IpcWifiTxDescShrinkFp ipc_wifi_msq_tx_desc_shrink;
extern T_IpcWifiTxDescShrinkFp ipc_wifi_aps_tx_desc_shrink;
extern T_IpcWifiRxDescUpdateFp ipc_wifi_msq_rx_desc_update;
extern T_IpcWifiRxDescUpdateFp ipc_wifi_aps_rx_desc_update;
extern T_IpcWifiTxDataAddrGetFp ipc_wifi_tx_data_addr_get;
extern T_IpcWifiRxDataAddrGetFp ipc_wifi_rx_data_addr_get;

#if 0
extern T_IpcWifiTxDataAddrGetFp ipc_wifi_msq_tx_data_addr_get;
extern T_IpcWifiRxDataAddrGetFp ipc_wifi_msq_rx_data_addr_get;
extern T_IpcWifiTxDataAddrGetFp ipc_wifi_aps_tx_data_addr_get;
extern T_IpcWifiRxDataAddrGetFp ipc_wifi_aps_rx_data_addr_get;
#endif

//extern T_IpcWifiTxDataLenGetFp ipc_wifi_msq_tx_data_len_get;
//extern T_IpcWifiRxDataLenGetFp ipc_wifi_msq_rx_data_len_get;
//extern T_IpcWifiTxDataLenGetFp ipc_wifi_aps_tx_data_len_get;
//extern T_IpcWifiRxDataLenGetFp ipc_wifi_aps_rx_data_len_get;

#if 0
extern T_IpcWifiTxDataOffsetUpdateFp ipc_wifi_msq_tx_start_offset_update;
extern T_IpcWifiRxDataOffsetUpdateFp ipc_wifi_msq_rx_start_offset_update;
extern T_IpcWifiTxDataOffsetUpdateFp ipc_wifi_aps_tx_start_offset_update;
extern T_IpcWifiRxDataOffsetUpdateFp ipc_wifi_aps_rx_start_offset_update;
extern T_IpcWifiTxDataOffsetUpdateFp ipc_wifi_msq_tx_end_offset_update;
extern T_IpcWifiRxDataOffsetUpdateFp ipc_wifi_msq_rx_end_offset_update;
extern T_IpcWifiTxDataOffsetUpdateFp ipc_wifi_aps_tx_end_offset_update;
extern T_IpcWifiRxDataOffsetUpdateFp ipc_wifi_aps_rx_end_offset_update;
#endif

#ifdef IPC_WIFI_DESC_DUMP
extern T_IpcCommonFp ipc_wifi_msq_tx_desc_dump;
extern T_IpcCommonFp ipc_wifi_msq_rx_desc_dump;
extern T_IpcCommonFp ipc_wifi_aps_tx_desc_dump;
extern T_IpcCommonFp ipc_wifi_aps_rx_desc_dump;
#endif

// wifi entry
extern T_IpcCommonFp ipc_wifi_entry_addr_dump;
extern T_IpcWifiEntryAllocFp ipc_wifi_msq_tx_entry_alloc;
extern T_IpcWifiEntryAllocFp ipc_wifi_msq_rx_entry_alloc;
extern T_IpcWifiEntryAllocFp ipc_wifi_aps_tx_entry_alloc;
extern T_IpcWifiEntryAllocFp ipc_wifi_aps_rx_entry_alloc;
extern T_IpcWifiEntryFreeFp ipc_wifi_msq_tx_entry_free;
extern T_IpcWifiEntryFreeFp ipc_wifi_msq_rx_entry_free;
extern T_IpcWifiEntryFreeFp ipc_wifi_aps_tx_entry_free;
extern T_IpcWifiEntryFreeFp ipc_wifi_aps_rx_entry_free;

//extern T_IpcCommonFp ipc_wifi_desc_addr_dump;

#ifdef IPC_WIFI_DESC_DUMP
extern T_IpcCommonFp ipc_wifi_msq_tx_entry_dump;
extern T_IpcCommonFp ipc_wifi_msq_rx_entry_dump;
extern T_IpcCommonFp ipc_wifi_aps_tx_entry_dump;
extern T_IpcCommonFp ipc_wifi_aps_rx_entry_dump;
#endif


extern T_IpcRbProcFp ipc_rb_reset;
extern T_IpcRbSetFp ipc_rb_reset_by_type;
extern T_IpcRbInitFp ipc_rb_init;
extern T_IpcRbReadWriteBufGetFp ipc_rb_write;
extern T_IpcRbWriteDoneFp ipc_rb_write_done;
extern T_IpcRbReadWriteBufGetFp ipc_rb_read;
extern T_IpcRbProcFp ipc_rb_read_done;
extern T_IpcRbCountGetFp ipc_rb_count_get;
extern T_IpcRbReadBufGetByIdxFp ipc_rb_read_by_idx;
extern T_IpcRbCheckFp ipc_is_enabled;
extern T_IpcSetFp ipc_enable;
extern T_IpcEntryInfoGetFp ipc_entry_info_get;


// wifi descriptor
extern uint32_t g_u32IpcDescSize;
extern T_IpcWifiDescInitFp ipc_wifi_desc_init;
extern T_IpcWifiRxDescAllocMaxFp ipc_wifi_rx_desc_alloc_max;
extern T_IpcWifiDescAllocFp ipc_wifi_tx_desc_alloc;
//extern T_IpcWifiDescAllocFp ipc_wifi_rx_desc_alloc;
extern T_IpcWifiDescCheckFp ipc_wifi_desc_check;
extern T_IpcWifiDescFreeFp ipc_wifi_desc_free;
extern T_IpcWifiTxDescShrinkIntFp ipc_wifi_tx_desc_shrink;
//extern T_IpcWifiTxDescUpdateIntFp ipc_wifi_tx_desc_update;
extern T_IpcWifiRxDescUpdateIntFp ipc_wifi_rx_desc_update;
extern T_IpcWifiDataAddrGetFp ipc_wifi_data_addr_get;
extern T_IpcWifiDataLenGetFp ipc_wifi_tx_data_len_get;
extern T_IpcWifiDataLenGetFp ipc_wifi_rx_data_len_get;
//extern T_IpcWifiFirstDescAvailSizeGetFp ipc_wifi_tx_first_desc_avail_size_get;

extern T_IpcWifiHeaderLenUpdate ipc_wifi_header_len_update;
extern T_IpcWifiTxRxHeaderLenUpdate ipc_wifi_tx_header_len_update;
extern T_IpcWifiTxRxHeaderLenUpdate ipc_wifi_rx_header_len_update;


#if 0
extern T_IpcWifiDataOffsetUpdateFp ipc_wifi_tx_start_offset_update;
extern T_IpcWifiDataOffsetUpdateFp ipc_wifi_rx_start_offset_update;
extern T_IpcWifiDataOffsetUpdateFp ipc_wifi_tx_end_offset_update;
extern T_IpcWifiDataOffsetUpdateFp ipc_wifi_rx_end_offset_update;
#endif

#ifdef IPC_WIFI_DESC_DUMP
extern T_IpcWifiDescDumpFp ipc_wifi_desc_dump;
#endif

// wifi entry
extern T_IpcWifiEntryInitFp ipc_wifi_entry_init;
extern T_IpcIntWifiEntryInfoGetFp ipc_wifi_entry_info_get;
extern T_IpcIntWifiEntryAllocFp ipc_wifi_entry_alloc;
extern T_IpcIntWifiEntryFreeFp ipc_wifi_entry_free;

#ifdef IPC_WIFI_DESC_DUMP
extern T_IpcWifiEntryDumpFp ipc_wifi_entry_dump;
#endif

void Ipc_PreInit(void);
void ipc_cli_cmd_impl(int argc, char *argv[]);

#ifdef IPC_SUT

typedef struct
{
    uint32_t dwNum;
    uint32_t dwMask;
    uint32_t dwBufSize;
    uint32_t *pdwWrite;
    uint32_t *pdwRead;
    uint32_t *pdwaBuf[IPC_SUT_BUF_NUM];
} T_IpcSutRb;

typedef struct
{
    uint32_t dwType;
    uint32_t dwIdx;
    uint8_t baData[IPC_SUT_DATA_SIZE];
} T_IpcSutBuf;

void ipc_sut_proc(void);

uint8_t ipc_dummy_exist(uint32_t dwHandle);
int ipc_dummy_send(uint32_t dwType, void *pData, uint32_t dwDataLen);
void ipc_dummy_1(uint32_t dwHandle);
void ipc_dummy_2(void);
void *ipc_dummy_get_1(uint32_t dwHandle);
void *ipc_dummy_get_2(void);
uint32_t ipc_dummy_count_get_1(uint32_t *pdwTotal);
uint32_t ipc_dummy_count_get_2(uint32_t dwHandle, uint32_t *pdwTotal);

#ifdef IPC_MSQ
void ipc_sut_eint_2_proc(void);
#else
int ipc_sut_1_1(uint32_t dwType, uint32_t dwCount);
int ipc_sut_1_2(uint32_t dwType, uint32_t dwCount);
int ipc_sut_1_3(uint32_t dwType, uint32_t dwCount);
int ipc_sut_2_1(uint32_t dwType, uint32_t dwCount);
int ipc_sut_2_2(uint32_t dwType, uint32_t dwCount);
int ipc_sut_2_3(uint32_t dwType, uint32_t dwCount);
int ipc_sut_3_1(uint32_t dwType, uint32_t dwCount);
int ipc_sut_3_2(uint32_t dwType, uint32_t dwCount);
int ipc_sut_3_3(uint32_t dwType, uint32_t dwCount);
int ipc_sut_4_1(uint32_t dwType, uint32_t dwCount);
#endif

#endif //#ifdef IPC_SUT

#include "ipc_patch.h"
#endif //#ifndef __IPC_H__

