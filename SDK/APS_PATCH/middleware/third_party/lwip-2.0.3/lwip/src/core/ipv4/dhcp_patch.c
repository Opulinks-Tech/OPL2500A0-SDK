/**
 * @file
 * Dynamic Host Configuration Protocol client
 *
 * @defgroup dhcp4 DHCPv4
 * @ingroup ip4
 * DHCP (IPv4) related functions
 * This is a DHCP client for the lwIP TCP/IP stack. It aims to conform
 * with RFC 2131 and RFC 2132.
 *
 * @todo:
 * - Support for interfaces other than Ethernet (SLIP, PPP, ...)
 *
 * Options:
 * @ref DHCP_COARSE_TIMER_SECS (recommended 60 which is a minute)
 * @ref DHCP_FINE_TIMER_MSECS (recommended 500 which equals TCP coarse timer)
 *
 * dhcp_start() starts a DHCP client instance which
 * configures the interface by obtaining an IP address lease and maintaining it.
 *
 * Use dhcp_release() to end the lease and use dhcp_stop()
 * to remove the DHCP client.
 *
 * @see netifapi_dhcp4
 */

/*
 * Copyright (c) 2001-2004 Leon Woestenberg <leon.woestenberg@gmx.net>
 * Copyright (c) 2001-2004 Axon Digital Design B.V., The Netherlands.
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
 * The Swedish Institute of Computer Science and Adam Dunkels
 * are specifically granted permission to redistribute this
 * source code.
 *
 * Author: Leon Woestenberg <leon.woestenberg@gmx.net>
 *
 */

#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_DHCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/stats.h"
#include "lwip/mem.h"
#include "lwip/udp.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/def.h"
#include "lwip/dhcp.h"
#include "lwip/autoip.h"
#include "lwip/dns.h"
#include "lwip/etharp.h"
#include "lwip/prot/dhcp.h"

#include <string.h>

#if defined(LWIP_ROMBUILD)
#include "ipv4/dhcp_if.h"
#endif

int g_t0_renewal_cnt = 3;

static void dhcp_t0_timeout(struct netif *netif)
{
  struct dhcp *dhcp = netif_dhcp_data(netif);

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_STATE, ("dhcp_t0_timeout()\n"));
  if ((dhcp->state == DHCP_STATE_REQUESTING) || (dhcp->state == DHCP_STATE_BOUND) ||
      (dhcp->state == DHCP_STATE_RENEWING)) {
    /* just retry to renew - note that the rebind timer (t2) will
     * eventually time-out if renew tries fail. */
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE,
                ("dhcp_t0_timeout(): try renew\n"));
    /* This slightly different to RFC2131: DHCPREQUEST will be sent from state
       DHCP_STATE_RENEWING, not DHCP_STATE_BOUND */
    dhcp_renew(netif);
  }
}

/**
 * The DHCP timer that checks for lease renewal/rebind timeouts.
 * Must be called once a minute (see @ref DHCP_COARSE_TIMER_SECS).
 */
void
dhcp_coarse_tmr_patch(void)
{
  struct netif *netif = netif_list;
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp_coarse_tmr()\n"));
  /* iterate through all network interfaces */
  while (netif != NULL) {
    /* only act on DHCP configured interfaces */
    struct dhcp *dhcp = netif_dhcp_data(netif);

    if ((dhcp != NULL) && (dhcp->state != DHCP_STATE_OFF)) {

       if (dhcp->t0_timeout == 1) {//dhcp lease time 60s case
         if (dhcp->lease_used > g_t0_renewal_cnt ) {
#if OPL_DYN_UPDATE_MTU
           dhcp_restore_mtu(netif);
#endif
           /* this clients' lease time has expired */
           dhcp_release(netif);
           dhcp_discover(netif);

         } else if (++dhcp->lease_used >= dhcp->t0_timeout) {
           LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_coarse_tmr(): t0 timeout 111\n"));
#if OPL_DYN_UPDATE_MTU
           dhcp_restore_mtu(netif);
#endif
           dhcp_t0_timeout(netif);
         }

         netif = netif->next;
         continue;
       }

      /* compare lease time to expire timeout */
      if (dhcp->t0_timeout && (++dhcp->lease_used == dhcp->t0_timeout)) {
#if OPL_DYN_UPDATE_MTU
        dhcp_restore_mtu(netif);
#endif
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_coarse_tmr(): t0 timeout\n"));
        /* this clients' lease time has expired */
        dhcp_release(netif);
        dhcp_discover(netif);
      /* timer is active (non zero), and triggers (zeroes) now? */
      } else if (dhcp->t2_rebind_time && (dhcp->t2_rebind_time-- == 1)) {
#if OPL_DYN_UPDATE_MTU
        dhcp_restore_mtu(netif);
#endif
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_coarse_tmr(): t2 timeout\n"));
        /* this clients' rebind timeout triggered */
        dhcp_t2_timeout(netif);
      /* timer is active (non zero), and triggers (zeroes) now */
      } else if (dhcp->t1_renew_time && (dhcp->t1_renew_time-- == 1)) {
#if OPL_DYN_UPDATE_MTU
        dhcp_restore_mtu(netif);
#endif
        LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_coarse_tmr(): t1 timeout\n"));
        /* this clients' renewal timeout triggered */
        dhcp_t1_timeout(netif);
      }
      LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp_coarse_tmr(): mtu %d\n", netif->mtu));
    }
    /* proceed to next netif */
    netif = netif->next;
  }
}

void lwip_load_interface_dhcp_patch(void)
{
    dhcp_coarse_tmr_adpt   = dhcp_coarse_tmr_patch;
}

#endif /* LWIP_IPV4 && LWIP_DHCP */
