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
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/stats.h"
#include "lwip/dhcp.h"

#include "wlannetif.h"
#include "lwip_helper.h"
#include "network_config.h"
#include "event_loop.h"
#include "wifi_api.h"
#include "wifi_mac_task.h"


extern LWIP_RETDATA sys_sem_t wifi_connected;
extern LWIP_RETDATA sys_sem_t ip_ready;

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
#define IPADDR_TIMEOUT            ((u32_t)0x0000FEA9UL)
#define IP4_ADDR_ISTIMEOUT(addr1) (((*(addr1)).addr & IPADDR_TIMEOUT) == IPADDR_TIMEOUT)


/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
/* NETIF data */
extern LWIP_RETDATA struct netif netif;
extern LWIP_RETDATA bool tcpip_inited;

extern LWIP_RETDATA int wakeup_event_timeouts;
extern LWIP_RETDATA u32_t last_wakeup_time;
extern LWIP_RETDATA u32_t wakeup_event_timeouts_max;

/*****************************************************************************
 * Private functions declarations
 ****************************************************************************/
static uint8_t def_dhcp_retry = 1;
static uint8_t dhcp_retry = 0;


/*****************************************************************************
 * Private functions
 ****************************************************************************/
static void ip_ready_callback_patch(struct netif *netif)
{
    event_msg_t msg = {0};

    if (!ip4_addr_isany(netif_ip4_addr(netif))) {
        sys_sem_signal(&ip_ready);
        char ipaddrstr[16] = {0};
        if (IP4_ADDR_ISTIMEOUT(netif_ip4_addr(netif))) { //DHCP timeout
            dhcp_retry++;

            ipaddr_ntoa_r(&netif->ip_addr, ipaddrstr, sizeof(ipaddrstr));
            printf("DHCP got timeout IP:%s\r\n", ipaddrstr);

            // 1. Retry DHCP mechanism
            if (dhcp_retry <= def_dhcp_retry) {
                printf("DHCP got timeout IP, retry %d\r\n", dhcp_retry);
#if LWIP_DHCP
                dhcp_start(netif);
#endif
            }
            // 2. Wifi disconnect
            else {
                printf("DHCP got Failed\r\n");
#if LWIP_DHCP
                dhcp_release(netif);
                dhcp_stop(netif);
#endif
                wifi_connection_disconnect_ap();
                dhcp_retry = 0;
            }
        }
        else {
            dhcp_retry = 0;
            ipaddr_ntoa_r(&netif->ip_addr, ipaddrstr, sizeof(ipaddrstr));
            printf("DHCP got IP:%s\r\n", ipaddrstr);
            msg.event = WIFI_EVENT_STA_GOT_IP;
            msg.length = 0;
            msg.param = NULL;
            wifi_event_loop_send(&msg);
        }
    }
}

void lwip_tcpip_init_patch(void)
{
	ip4_addr_t ipaddr, netmask, gw;
	volatile s32_t tcpipdone = 0;
    lwip_tcpip_config_t tcpip_config = {{0}, {0}, {0}};

    tcpip_init(tcpip_init_done_cb, (void *) &tcpipdone);
	while (!tcpipdone) {
		sys_msleep(1);
	}

	LWIP_DEBUGF(NETIF_DEBUG, ("LwIP TCPIP thread is initialized.. \n"));

    if (0 != tcpip_config_init(&tcpip_config)) {
        LWIP_DEBUGF(NETIF_DEBUG, ("tcpip config init fail \n"));
        return;
    }

    ipaddr = tcpip_config.sta_ip;
    netmask = tcpip_config.sta_mask;
    gw = tcpip_config.sta_gw;

	/* Add netif interface for OPL2500, wlan interface */
	if (!netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init, tcpip_input)) {
		LWIP_ASSERT("Net interface failed to initialize\r\n", 0);
	}
	netif_set_default(&netif);

#ifdef WIFI_RX_DATA_USE_IPC
    wifi_mac_register_rxcb(&netif, lwip_wlan_sta_input);
#endif

	netif_set_up(&netif);

    //netif_set_link_callback(&netif, lwip_netif_link_irq);
    //netif_set_status_callback(&netif, lwip_netif_status_irq);
}

/*-------------------------------------------------------------------------------------
 * Interface assignment
 *------------------------------------------------------------------------------------*/
void lwip_load_interface_lwip_helper_patch(void)
{
    ip_ready_callback = ip_ready_callback_patch;
    lwip_tcpip_init   = lwip_tcpip_init_patch;
}

