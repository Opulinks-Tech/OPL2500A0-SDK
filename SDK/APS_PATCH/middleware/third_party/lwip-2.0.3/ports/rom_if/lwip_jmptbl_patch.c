/******************************************************************************
*  Copyright 2019, Netlink Communication Corp.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Netlink Communication Corp. (C) 2019
******************************************************************************/

#include "lwip_jmptbl_patch.h"

extern void lwip_load_interface_lwip_helper_patch(void);
extern void lwip_load_interface_dhcp_patch(void);
extern void lwip_memp_patch(void);
extern void lwip_wlannetif_patch(void);
extern void opl_lwip_load_interface_option_init_patch(void);
extern void lwip_load_interface_tcp_in_patch(void);
extern void lwip_load_interface_tcp_patch(void);

void lwip_module_interface_init_patch(void)
{
    lwip_load_interface_lwip_helper_patch();
    lwip_load_interface_tcp_in_patch();
    lwip_load_interface_tcp_patch();
    lwip_load_interface_dhcp_patch();
    #if (REFINE_RX_PROC == 2)
    lwip_memp_patch();
    lwip_wlannetif_patch();
    #endif

    opl_lwip_load_interface_option_init_patch();
}
