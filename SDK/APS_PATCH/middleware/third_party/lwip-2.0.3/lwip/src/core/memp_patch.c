/**
 * @file
 * Dynamic pool memory manager
 *
 * lwIP has dedicated pools for many structures (netconn, protocol control blocks,
 * packet buffers, ...). All these pools are managed here.
 *
 * @defgroup mempool Memory pools
 * @ingroup infrastructure
 * Custom memory pools

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

#include "lwip/memp.h"
#include "lwip/sys.h"
#include "lwip/stats.h"

#include <string.h>

/* Make sure we include everything we need for size calculation required by memp_std.h */
#include "lwip/pbuf.h"
#include "lwip/raw.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/ip4_frag.h"
#include "lwip/netbuf.h"
#include "lwip/api.h"
#include "lwip/priv/tcpip_priv.h"
#include "lwip/priv/api_msg.h"
#include "lwip/sockets.h"
#include "lwip/netifapi.h"
#include "lwip/etharp.h"
#include "lwip/igmp.h"
#include "lwip/timeouts.h"
/* needed by default MEMP_NUM_SYS_TIMEOUT */
#include "netif/ppp/ppp_opts.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "lwip/priv/nd6_priv.h"
#include "lwip/ip6_frag.h"
#include "lwip/mld6.h"

#define LWIP_MEMPOOL(name,num,size,desc) LWIP_MEMPOOL_DECLARE(name,num,size,desc)
//#include "lwip/priv/memp_std.h"

/*
const struct memp_desc* const memp_pools[MEMP_MAX] = {
#define LWIP_MEMPOOL(name,num,size,desc) &memp_ ## name,
#include "lwip/priv/memp_std.h"
};
*/
extern const struct memp_desc* const memp_pools[MEMP_MAX];

#ifdef LWIP_HOOK_FILENAME
#include LWIP_HOOK_FILENAME
#endif

#if MEMP_MEM_MALLOC && MEMP_OVERFLOW_CHECK >= 2
#undef MEMP_OVERFLOW_CHECK
/* MEMP_OVERFLOW_CHECK >= 2 does not work with MEMP_MEM_MALLOC, use 1 instead */
#define MEMP_OVERFLOW_CHECK 1
#endif

//#if defined(LWIP_ROMBUILD)
#include "memp_if.h"
//#endif


#if (REFINE_RX_PROC == 2)
#define MEMP_NUM_NETBUF_PATCH           24 //48
#define PBUF_POOL_SIZE_PATCH            20 //16 //28
#define MEMP_NUM_TCP_SEG_PATCH          32
//#define MEMP_NUM_TCPIP_MSG_INPKT_PATCH  24 //48

LWIP_MEMPOOL(NETBUF_PATCH, MEMP_NUM_NETBUF_PATCH, sizeof(struct netbuf), "NETBUF_PATCH")
LWIP_MEMPOOL(PBUF_POOL_PATCH, PBUF_POOL_SIZE_PATCH, (MEMP_ALIGN_SIZE(sizeof(struct pbuf)) + MEMP_ALIGN_SIZE(PBUF_POOL_BUFSIZE + 64)), "PBUF_POOL_PATCH")
LWIP_MEMPOOL(TCP_SEG_PATCH, MEMP_NUM_TCP_SEG_PATCH, sizeof(struct tcp_seg), "TCP_SEG_PATCH")
//LWIP_MEMPOOL(TCPIP_MSG_INPKT_PATCH, MEMP_NUM_TCPIP_MSG_INPKT_PATCH, sizeof(struct tcpip_msg), "TCPIP_MSG_INPKT_PATCH")


/**
 * Initializes lwIP built-in pools.
 * Related functions: memp_malloc, memp_free
 *
 * Carves out memp_memory into linked lists for each pool-type.
 */
void memp_init_patch(void)
{
  u16_t i;

  /* for every pool: */
  for (i = 0; i < LWIP_ARRAYSIZE(memp_pools); i++) {
    switch(i)
    {
        case MEMP_NETBUF:
            memp_init_pool(&memp_NETBUF_PATCH);
            
            #if LWIP_STATS && MEMP_STATS
            lwip_stats.memp[MEMP_NETBUF] = memp_NETBUF_PATCH.stats;
            #endif
            
            break;
    
        case MEMP_PBUF_POOL:
            memp_init_pool(&memp_PBUF_POOL_PATCH);
             
            #if LWIP_STATS && MEMP_STATS
            lwip_stats.memp[MEMP_PBUF_POOL] = memp_PBUF_POOL_PATCH.stats;
            #endif
            
            break;

        case MEMP_TCP_SEG:
            memp_init_pool(&memp_TCP_SEG_PATCH);
             
            #if LWIP_STATS && MEMP_STATS
            lwip_stats.memp[MEMP_TCP_SEG] = memp_TCP_SEG_PATCH.stats;
            #endif
            
            break;
/*
        case MEMP_TCPIP_MSG_INPKT:
            memp_init_pool(&memp_TCPIP_MSG_INPKT_PATCH);
             
            #if LWIP_STATS && MEMP_STATS
            lwip_stats.memp[MEMP_TCPIP_MSG_INPKT] = memp_TCPIP_MSG_INPKT_PATCH.stats;
            #endif
            
            break;
*/
        default:
            memp_init_pool(memp_pools[i]);
        
            #if LWIP_STATS && MEMP_STATS
            lwip_stats.memp[i] = memp_pools[i]->stats;
            #endif
            
            break;
    }
  }

#if MEMP_OVERFLOW_CHECK >= 2
  /* check everything a first time to see if it worked */
  memp_overflow_check_all();
#endif /* MEMP_OVERFLOW_CHECK >= 2 */
}

/**
 * Get an element from a specific pool.
 *
 * @param type the pool to get an element from
 *
 * @return a pointer to the allocated memory or a NULL pointer on error
 */
void *memp_malloc_patch(memp_t type)
{
  void *memp;
  LWIP_ERROR("memp_malloc: type < MEMP_MAX", (type < MEMP_MAX), return NULL;);

#if MEMP_OVERFLOW_CHECK >= 2
  memp_overflow_check_all();
#endif /* MEMP_OVERFLOW_CHECK >= 2 */

#if !MEMP_OVERFLOW_CHECK

  switch(type)
  {
  case MEMP_NETBUF:
      memp = do_memp_malloc_pool(&memp_NETBUF_PATCH);
      break;

  case MEMP_PBUF_POOL:
      memp = do_memp_malloc_pool(&memp_PBUF_POOL_PATCH);
      break;

    case MEMP_TCP_SEG:
      memp = do_memp_malloc_pool(&memp_TCP_SEG_PATCH);
      break;
/*
  case MEMP_TCPIP_MSG_INPKT:
      memp = do_memp_malloc_pool(&memp_TCPIP_MSG_INPKT_PATCH);
      break;
*/
  default:
      memp = do_memp_malloc_pool(memp_pools[type]);
      break;
  }
  
#else
  memp = do_memp_malloc_pool_fn(memp_pools[type], file, line);
#endif

  return memp;
}

/**
 * Put an element back into its pool.
 *
 * @param type the pool where to put mem
 * @param mem the memp element to free
 */
void memp_free_patch(memp_t type, void *mem)
{
#ifdef LWIP_HOOK_MEMP_AVAILABLE
  struct memp *old_first;
#endif

  LWIP_ERROR("memp_free: type < MEMP_MAX", (type < MEMP_MAX), return;);

  if (mem == NULL) {
    return;
  }

#if MEMP_OVERFLOW_CHECK >= 2
  memp_overflow_check_all();
#endif /* MEMP_OVERFLOW_CHECK >= 2 */

#ifdef LWIP_HOOK_MEMP_AVAILABLE
  old_first = *memp_pools[type]->tab;
#endif

  switch(type)
  {
  case MEMP_NETBUF:
      do_memp_free_pool(&memp_NETBUF_PATCH, mem);
      break;

  case MEMP_PBUF_POOL:
      do_memp_free_pool(&memp_PBUF_POOL_PATCH, mem);
      break;

  case MEMP_TCP_SEG:
      do_memp_free_pool(&memp_TCP_SEG_PATCH, mem);
      break;
/*
  case MEMP_TCPIP_MSG_INPKT:
      do_memp_free_pool(&memp_TCPIP_MSG_INPKT_PATCH, mem);
      break;
*/
  default:
      do_memp_free_pool(memp_pools[type], mem);
      break;
  }

#ifdef LWIP_HOOK_MEMP_AVAILABLE
  if (old_first == NULL) {
    LWIP_HOOK_MEMP_AVAILABLE(type);
  }
#endif
}
#endif //#if (REFINE_RX_PROC == 2)

void lwip_memp_patch(void)
{
    #if (REFINE_RX_PROC == 2)
    memp_init_adpt                  = memp_init_patch;
    memp_malloc_adpt                = memp_malloc_patch;
    memp_free_adpt                  = memp_free_patch;
    #endif
}

