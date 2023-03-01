/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/ethip6.h"
#include "netif/etharp.h"
#include "arch/sys_arch.h"
#include "wlannetif.h"
//#include "netif/ppp_oe.h"
#include <string.h>

#include "wifi_mac_task.h"
#include "controller_wifi.h"
#include "msg.h"
#include "wifi_nvm.h"
#include "sys_common_ctrl.h"


#if (REFINE_RX_PROC == 2)
//#include "wifi_mac_data.h"

extern volatile uint8_t g_u8RxFrameHdrLen;
extern s32 rx_data_frame_proc_for_one_copy(u8 *data, u16 data_len, u8 u8HdrLen, struct pbuf *pBuf);
#endif


/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

#define TRANSPORT_TASK
//#define IF_LOOPBACK
//#define TX_PKT_DUMP
//#define RX_PKT_DUMP

#if defined(LWIP_ROMBUILD)
#include "port/wlannetif_if.h"
#endif

#include "wlannetif.h"

#define TX_TASK_STACKSIZE           (512)
#ifdef LWIP_DEBUG
#define RX_TASK_STACKSIZE           (512*2)
#else
#define RX_TASK_STACKSIZE           (512)
#endif

#define RX_PRIORITY                 (tskIDLE_PRIORITY + 2)//(osPriorityBelowNormal)
#define TX_PRIORITY                 (tskIDLE_PRIORITY + 2)//(osPriorityBelowNormal)

extern LWIP_RETDATA sys_sem_t TxReadySem;
extern LWIP_RETDATA sys_sem_t RxReadySem; /**< RX packet ready semaphore */
extern LWIP_RETDATA sys_sem_t TxCleanSem;
extern LWIP_RETDATA sys_thread_t rx_thread_handle;

extern struct netif netif;

extern char g_sLwipHostName[WLAN_HOST_NAME_LEN + 1];

extern void dump_buffer(char *pbuf, int len, int mode);

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
};

#if (REFINE_RX_PROC == 2)
/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
struct pbuf *low_level_input_patch(struct netif *netif, void *buf, u16_t len)
{
    struct ethernetif *ethernetif = netif->state;
    struct pbuf *p = NULL, *q = NULL;
    //u16_t l = 0;

    extern volatile uint8_t g_u8RxOneCopy;

    LWIP_UNUSED_ARG(ethernetif);

    /* Drop oversized packet */
    if(g_u8RxOneCopy)
    {
        // TODO: refine PBUF_POOL_BUFSIZE in memp_pools[] to handle max. "len" argument (MAC frame without FCS) for max. IP packet
        if (len > (1514 + 46)) {
            LINK_STATS_INC(link.lenerr);
            goto done;
            //return NULL;
        }
    }
    else
    {
        if (len > 1514) {
            LINK_STATS_INC(link.lenerr);
            goto done;
        //return NULL;
        }
    }

    if (!netif || !buf || len <= 0) {
        LINK_STATS_INC(link.err);
        goto done;
    }
        //return NULL;

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    if(g_u8RxOneCopy)
    {
        extern struct pbuf *pbuf_alloc_for_rx_one_copy(pbuf_layer layer, u16_t length, pbuf_type type);

        p = pbuf_alloc_for_rx_one_copy(PBUF_RAW, len, PBUF_POOL);
    }
    else
    {
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    }

    /* We allocate a continous pbuf */
    //p = pbuf_alloc(PBUF_RAW, len, PBUF_RAM);

    if (p != NULL) {

#if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

        /* pbufs allocated from the RAM pool should be non-chained. */
        LWIP_ASSERT("lpc_rx_queue: pbuf is not contiguous (chained)",
        pbuf_clen(p) <= 1);

        /* Copy the data to intermediate buffer. This is required because
           the driver copies all the data to one continuous packet data buffer. */

        if(g_u8RxOneCopy)
        {
            if(rx_data_frame_proc_for_one_copy(buf, len, g_u8RxFrameHdrLen, p) < 0)
            {
                pbuf_free(p);
                p = NULL;
                goto done;
            }
        }
        else
        {
            memcpy(p->payload, buf, len);
        }

        /* We iterate over the pbuf chain until we have read the entire
         * packet into the pbuf. */
        for(q = p; q != NULL; q = q->next) {
          /* Read enough bytes to fill this pbuf in the chain. The
           * available data in the pbuf is given by the q->len
           * variable.
           * This does not necessarily have to be a memcpy, you can also preallocate
           * pbufs for a DMA-enabled MAC and after receiving truncate it to the
           * actually received size. In this case, ensure the tot_len member of the
           * pbuf is the sum of the chained pbuf len members.
           */

            //memcpy((u8_t*)q->payload, (u8_t*)&buf[l], q->len);
            //l = l + q->len;
        }

        // Acknowledge that packet has been read;
        //wifi_mac_rx_queue_first_entry_free();

#if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

        LINK_STATS_INC(link.recv);
    } else {
        /* drop packet(); */
        //wifi_mac_rx_queue_first_entry_free();
        LINK_STATS_INC(link.recv_memerr);
        LINK_STATS_INC(link.drop);
    }

done:
    if(g_u8RxOneCopy)
    {
        g_u8RxOneCopy = 0;
        wifi_mac_rx_queue_first_entry_free();
    }

    return p;
}
#endif //#if (REFINE_RX_PROC == 2)

void lwip_wlannetif_patch(void)
{
    #if (REFINE_RX_PROC == 2)
    low_level_input_adpt = low_level_input_patch;
    #endif
}

