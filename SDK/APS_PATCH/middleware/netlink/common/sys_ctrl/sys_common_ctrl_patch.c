/******************************************************************************
*  Copyright 2017 - 2023, Opulinks Technology Ltd.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Opulinks Technology Ltd. (C) 2023
******************************************************************************/
#include "sys_common_ctrl.h"
#include "sys_cfg.h"

int sys_set_tca(uint8_t enable)
{
    int ret;
    T_TcaCfg tca_cfg = {enable,5};

    ret = sys_cfg_tca_set((void *)&tca_cfg);

    if( ret )
        return 0;
    else
        return -1;
}

void sys_common_ctrl_func_patch(void)
{
    //base_mac_addr_src_get_cfg     =     base_mac_addr_src_get_cfg_impl;
    //base_mac_addr_src_set_cfg     =     base_mac_addr_src_set_cfg_impl;
    //base_nvm_mac_addr_src_read    =     base_nvm_mac_addr_src_read_impl;
    //base_nvm_mac_addr_src_write   =     base_nvm_mac_addr_src_write_impl;
    //get_rf_power_level            =     get_rf_power_level_impl;
    //set_rf_power_level            =     set_rf_power_level_impl;
    //get_dhcp_arp_check            =     get_dhcp_arp_check_impl;
    //get_dhcp_arp_check_from_fim   =     get_dhcp_arp_check_from_fim_impl;
    //set_dhcp_arp_check            =     set_dhcp_arp_check_impl;
    //set_dhcp_interval_retry_times =     set_dhcp_interval_retry_times_impl; //I0064  0000978
    //set_ble_bd_addr               =     set_ble_bd_addr_impl;
    //get_ble_bd_addr               =     get_ble_bd_addr_impl;
}
