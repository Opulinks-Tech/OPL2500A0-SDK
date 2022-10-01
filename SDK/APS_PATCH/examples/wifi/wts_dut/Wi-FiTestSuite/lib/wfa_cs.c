/****************************************************************************
Copyright (c) 2016 Wi-Fi Alliance.  All Rights Reserved

Permission to use, copy, modify, and/or distribute this software for any purpose with or
without fee is hereby granted, provided that the above copyright notice and this permission
notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

******************************************************************************/

/*
 *   File: wfa_cs.c -- configuration and setup
 *   This file contains all implementation for the dut setup and control
 *   functions, such as network interfaces, ip address and wireless specific
 *   setup with its supplicant.
 *
 *   The current implementation is to show how these functions
 *   should be defined in order to support the Agent Control/Test Manager
 *   control commands. To simplify the current work and avoid any GPL licenses,
 *   the functions mostly invoke shell commands by calling linux system call,
 *   system("<commands>").
 *
 *   It depends on the differnt device and platform, vendors can choice their
 *   own ways to interact its systems, supplicants and process these commands
 *   such as using the native APIs.
 *
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wfa_portall.h"
#include "wfa_debug.h"
#include "wfa_ver.h"
#include "wfa_main.h"
#include "wfa_types.h"
#include "wfa_ca.h"
#include "wfa_tlv.h"
#include "wfa_sock.h"
#include "wfa_tg.h"
#include "wfa_cmds.h"
#include "wfa_rsp.h"
#include "wfa_utils.h"
#ifdef WFA_WMM_PS_EXT
#include "wfa_wmmps.h"
#endif

#include "wifi_api.h"
#include "lwip_helper.h"
#include "ping/ping_sock.h"
#include "wts_dut.h"

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

#define CERTIFICATES_PATH    "/etc/wpa_supplicant"

/* Some device may only support UDP ECHO, activate this line */
//#define WFA_PING_UDP_ECHO_ONLY 1

#define WFA_ENABLED 1

extern unsigned short wfa_defined_debug;
int wfaExecuteCLI(char *CLI);

/* Since the two definitions are used all over the CA function */
char gCmdStr[WFA_CMD_STR_SZ];
extern dutCmdResponse_t gGenericResp;
int wfaTGSetPrio(int sockfd, int tgClass);
void create_apts_msg(int msg, unsigned int txbuf[],int id);

int sret = 0;

extern char e2eResults[];

FILE *e2efp = NULL;
int chk_ret_status()
{
    char *ret = getenv(WFA_RET_ENV);

    if(*ret == '1')
        return WFA_SUCCESS;
    else
        return WFA_FAILURE;
}

/*
 * agtCmdProcGetVersion(): response "ca_get_version" command to controller
 *  input:  cmd --- not used
 *          valLen -- not used
 *  output: parms -- a buffer to store the version info response.
 */
int agtCmdProcGetVersion(int len, BYTE *parms, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *getverResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering agtCmdProcGetVersion ...\n");

    getverResp->status = STATUS_COMPLETE;
    wSTRNCPY(getverResp->cmdru.version, WFA_SYSTEM_VER, WFA_VERNAM_LEN);

    wfaEncodeTLV(WFA_GET_VERSION_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)getverResp, respBuf);

    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaAssociate():
 *    The function is to force the station wireless I/F to re/associate
 *    with the AP.
 */
int wfaStaAssociate(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCommand_t *assoc = (dutCommand_t *)caCmdBuf;
    dutCmdResponse_t *staAssocResp = &gGenericResp;
    int ret;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaAssociate ...\n");

    staAssocResp->status = STATUS_COMPLETE;

    uint32_t ssid_len = wSTRLEN(assoc->cmdsu.assoc.ssid);
    if (ssid_len == 0) {
        DPRINT_ERR(WFA_OUT, "The length of SSID is Zero.\n");
        staAssocResp->status = STATUS_ERROR;
        goto done;
    }

    if (assoc->cmdsu.assoc.wps == 1) {
        DPRINT_ERR(WFA_OUT, "WPS Not supported yet.\n");
        staAssocResp->status = STATUS_ERROR;
        goto done;
    }

    wifi_config_t wifi_config;
    wifi_get_config(WIFI_MODE_STA, &wifi_config);

    if (!wSTRNCMP((char *)wifi_config.sta_config.ssid, assoc->cmdsu.assoc.ssid, ssid_len)) {
        ret = wifi_connection_connect(&wifi_config);
        if (ret != 0) {
            DPRINT_ERR(WFA_OUT, "API ret error %d.\n", ret);
            staAssocResp->status = STATUS_ERROR;
            goto done;
        }
    }
    else {
        DPRINT_ERR(WFA_OUT, "Wrong SSID input before setting.\n");
        staAssocResp->status = STATUS_ERROR;
        goto done;
    }

done:
    /*
     * Then report back to control PC for completion.
     * This does not have failed/error status. The result only tells
     * a completion.
     */
    staAssocResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_ASSOCIATE_RESP_TLV, 4, (BYTE *)staAssocResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaReAssociate():
 *    The function is to force the station wireless I/F to re/associate
 *    with the AP.
 */
int wfaStaReAssociate(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    dutCommand_t *assoc = (dutCommand_t *)caCmdBuf;
    dutCmdResponse_t *staAssocResp = &gGenericResp;
    int ret;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaAssociate ...\n");

    staAssocResp->status = STATUS_COMPLETE;

    wifi_config_t wifi_config;
    wifi_get_config(WIFI_MODE_STA, &wifi_config);
    ret = wifi_connection_connect(&wifi_config);
    if (ret != 0) {
        DPRINT_ERR(WFA_OUT, "API ret error %d.\n", ret);
        staAssocResp->status = STATUS_ERROR;
    }

    /*
     * Then report back to control PC for completion.
     * This does not have failed/error status. The result only tells
     * a completion.
     */
    wfaEncodeTLV(WFA_STA_ASSOCIATE_RESP_TLV, 4, (BYTE *)staAssocResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaIsConnected():
 *    The function is to check whether the station's wireless I/F has
 *    already connected to an AP.
 */
int wfaStaIsConnected(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    dutCommand_t *connStat = (dutCommand_t *)caCmdBuf;
    dutCmdResponse_t *staConnectResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering isConnected ...\n");

    if(wts_dut_is_wifi_connected())
        staConnectResp->cmdru.connected = 1;
    else
        staConnectResp->cmdru.connected = 0;

    /*
     * Report back the status: Complete or Failed.
     */
    staConnectResp->status = STATUS_COMPLETE;

    wfaEncodeTLV(WFA_STA_IS_CONNECTED_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)staConnectResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaGetIpConfig():
 * This function is to retriev the ip info including
 *     1. dhcp enable
 *     2. ip address
 *     3. mask
 *     4. primary-dns
 *     5. secondary-dns
 *
 *     The current implementation is to use a script to find these information
 *     and store them in a file.
 */
int wfaStaGetIpConfig(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    dutCommand_t *getIpConf = (dutCommand_t *)caCmdBuf;
    dutCmdResponse_t *ipconfigResp = &gGenericResp;
    struct netif *p_netif = netif_find("st1");

    DPRINT_INFO(WFA_OUT, "Entering wfaStaGetIpConfig ...\n");

    caStaGetIpConfigResp_t *ifinfo = &ipconfigResp->cmdru.getIfconfig;

    ifinfo->isDhcp = 1; // TODO

    wSTRNCPY(ifinfo->ipaddr, ip4addr_ntoa(&p_netif->ip_addr.u_addr.ip4), 15);
    ifinfo->ipaddr[15]='\0';


    wSTRNCPY(ifinfo->mask, ip4addr_ntoa(&p_netif->netmask.u_addr.ip4), 15);
    ifinfo->mask[15]='\0';

    wSTRNCPY(ifinfo->dns[0], ip4addr_ntoa(&p_netif->gw.u_addr.ip4), 15);

    /*
     * Report back the results
     */
    ipconfigResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_GET_IP_CONFIG_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)ipconfigResp, respBuf);

    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "%i %i %s %s %s %s %i\n", ipconfigResp->status,
                ifinfo->isDhcp, ifinfo->ipaddr, ifinfo->mask,
                ifinfo->dns[0], ifinfo->dns[1], *respLen);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaSetIpConfig():
 *   The function is to set the ip configuration to a wireless I/F.
 *   1. IP address
 *   2. Mac address
 *   3. default gateway
 *   4. dns nameserver (pri and sec).
 */
int wfaStaSetIpConfig(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCommand_t *setIpConf = (dutCommand_t *)caCmdBuf;
    caStaSetIpConfig_t *ipconfig = &setIpConf->cmdsu.ipconfig;
    dutCmdResponse_t *staSetIpResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetIpConfig ...\n");

    if (ipconfig->isDhcp == 0) { // Disable DHCP
        lwip_net_start(WIFI_MODE_STA);
    }
    else { //TODO
        DPRINT_WARNING(WFA_OUT, "Static IP Not supported yet.\n");
    }

    /*
     * report status
     */
    staSetIpResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_IP_CONFIG_RESP_TLV, 4, (BYTE *)staSetIpResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaVerifyIpConnection():
 * The function is to verify if the station has IP connection with an AP by
 * send ICMP/pings to the AP.
 */
int wfaStaVerifyIpConnection(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    dutCommand_t *verip = (dutCommand_t *)caCmdBuf;
    dutCmdResponse_t *verifyIpResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaVerifyIpConnection ...\n");

    DPRINT_WARNING(WFA_OUT, "wfaStaVerifyIpConnection Not supported yet.\n");

#if 0
#ifndef WFA_PING_UDP_ECHO_ONLY
//    char strout[64], *pcnt;
//    FILE *tmpfile;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaVerifyIpConnection ...\n");

    /* set timeout value in case not set */
    if(verip->cmdsu.verifyIp.timeout <= 0)
    {
        verip->cmdsu.verifyIp.timeout = 10;
    }

#if 0
    /* execute the ping command  and pipe the result to a tmp file */
    sprintf(gCmdStr, "ping %s -c 3 -W %u | grep loss | cut -f3 -d, 1>& /tmp/pingout.txt", verip->cmdsu.verifyIp.dipaddr, verip->cmdsu.verifyIp.timeout);
    sret = system(gCmdStr);

    /* scan/check the output */
    tmpfile = fopen("/tmp/pingout.txt", "r+");
    if(tmpfile == NULL)
    {
        verifyIpResp->status = STATUS_ERROR;
        wfaEncodeTLV(WFA_STA_VERIFY_IP_CONNECTION_RESP_TLV, 4, (BYTE *)verifyIpResp, respBuf);
        *respLen = WFA_TLV_HDR_LEN + 4;

        DPRINT_ERR(WFA_ERR, "file open failed\n");
        return WFA_FAILURE;
    }

    verifyIpResp->status = STATUS_COMPLETE;
    if(fscanf(tmpfile, "%s", strout) == EOF)
        verifyIpResp->cmdru.connected = 0;
    else
    {
        pcnt = strtok(strout, "%");

        /* if the loss rate is 100%, not able to connect */
        if(atoi(pcnt) == 100)
            verifyIpResp->cmdru.connected = 0;
        else
            verifyIpResp->cmdru.connected = 1;
    }

    fclose(tmpfile);
#endif
    verifyIpResp->status = STATUS_COMPLETE;
#else
    int btSockfd;
    struct pollfd fds[2];
    int timeout = 2000;
    char anyBuf[64];
    struct sockaddr_in toAddr;
    int done = 1, cnt = 0, ret, nbytes;

    verifyIpResp->status = STATUS_COMPLETE;
    verifyIpResp->cmdru.connected = 0;

    btSockfd = wfaCreateUDPSock("127.0.0.1", WFA_UDP_ECHO_PORT);

    if(btSockfd == -1)
    {
        verifyIpResp->status = STATUS_ERROR;
        wfaEncodeTLV(WFA_STA_VERIFY_IP_CONNECTION_RESP_TLV, 4, (BYTE *)verifyIpResp, respBuf);
        *respLen = WFA_TLV_HDR_LEN + 4;
        return WFA_FAILURE;;
    }

    toAddr.sin_family = AF_INET;
    toAddr.sin_addr.s_addr = inet_addr(verip->cmdsu.verifyIp.dipaddr);
    toAddr.sin_port = htons(WFA_UDP_ECHO_PORT);

    while(done)
    {
        wfaTrafficSendTo(btSockfd, (char *)anyBuf, 64, (struct sockaddr *)&toAddr);
        cnt++;

        fds[0].fd = btSockfd;
        fds[0].events = POLLIN | POLLOUT;

        ret = poll(fds, 1, timeout);
        switch(ret)
        {
        case 0:
            /* it is time out, count a packet lost*/
            break;
        case -1:
        /* it is an error */
        default:
        {
            switch(fds[0].revents)
            {
            case POLLIN:
            case POLLPRI:
            case POLLOUT:
                nbytes = wfaTrafficRecv(btSockfd, (char *)anyBuf, (struct sockaddr *)&toAddr);
                if(nbytes != 0)
                    verifyIpResp->cmdru.connected = 1;
                done = 0;
                break;
            default:
                /* errors but not care */
                ;
            }
        }
        }
        if(cnt == 3)
        {
            done = 0;
        }
    }

#endif
#endif

    wfaEncodeTLV(WFA_STA_VERIFY_IP_CONNECTION_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)verifyIpResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaGetMacAddress()
 *    This function is to retrieve the MAC address of a wireless I/F.
 */
int wfaStaGetMacAddress(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    dutCommand_t *getMac = (dutCommand_t *)caCmdBuf;
    dutCmdResponse_t *getmacResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaGetMacAddress ...\n");

    //TODO
    extern int ctrl_wifi_sta_mac_address_get(uint8_t *p_mac);
    uint8_t sta_mac[6];
    ctrl_wifi_sta_mac_address_get(&sta_mac[0]);
    sprintf(getmacResp->cmdru.mac, MACSTR, MAC2STR(sta_mac));

    getmacResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_GET_MAC_ADDRESS_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)getmacResp, respBuf);

    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaGetStats():
 * The function is to retrieve the statistics of the I/F's layer 2 txFrames,
 * rxFrames, txMulticast, rxMulticast, fcsErrors/crc, and txRetries.
 * Currently there is not definition how to use these info.
 */
int wfaStaGetStats(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *statsResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaGetStats ...\n");

    statsResp->status = STATUS_ERROR;
    wfaEncodeTLV(WFA_STA_GET_STATS_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)statsResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaSetEncryption():
 *   The function is to set the wireless interface with WEP or none.
 *
 *   Since WEP is optional test, current function is only used for
 *   resetting the Security to NONE/Plaintext (OPEN). To test WEP,
 *   this function should be replaced by the next one (wfaSetEncryption1())
 *
 *   Input parameters:
 *     1. I/F
 *     2. ssid
 *     3. encpType - wep or none
 *     Optional:
 *     4. key1
 *     5. key2
 *     6. key3
 *     7. key4
 *     8. activeKey Index
 */

int wfaSetEncryption1(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetEncryption_t *setEncryp = (caStaSetEncryption_t *)caCmdBuf;
    dutCmdResponse_t *setEncrypResp = &gGenericResp;

#if 0
    /*
     * disable the network first
     */
    sprintf(gCmdStr, "wpa_cli -i %s disable_network 0", setEncryp->intf);
    sret = system(gCmdStr);

    /*
     * set SSID
     */
    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 ssid '\"%s\"'", setEncryp->intf, setEncryp->ssid);
    sret = system(gCmdStr);

    /*
     * Tell the supplicant for infrastructure mode (1)
     */
    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 mode 0", setEncryp->intf);
    sret = system(gCmdStr);

    /*
     * set Key management to NONE (NO WPA) for plaintext or WEP
     */
    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 key_mgmt NONE", setEncryp->intf);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s enable_network 0", setEncryp->intf);
    sret = system(gCmdStr);
#endif
    setEncrypResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_ENCRYPTION_RESP_TLV, 4, (BYTE *)setEncrypResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

/*
 *  Since WEP is optional, this function could be used to replace
 *  wfaSetEncryption() if necessary.
 */
int wfaSetEncryption(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    caStaSetEncryption_t *setEncryp = (caStaSetEncryption_t *)caCmdBuf;
    dutCmdResponse_t *setEncrypResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaSetEncryption ...\n");

    if (setEncryp->encpType == 1) { //WEP
        DPRINT_WARNING(WFA_OUT, "WEP Not supported yet.\n");
    }
    else {
        uint32_t ssid_len = wSTRLEN(setEncryp->ssid);
        if (ssid_len == 0) {
            DPRINT_ERR(WFA_OUT, "The length of SSID is Zero.\n");
            setEncrypResp->status = STATUS_ERROR;
            goto done;
        }

        wifi_config_t wifi_config;
        wifi_get_config(WIFI_MODE_STA, &wifi_config);
        wMEMSET(&wifi_config, 0, sizeof(wifi_config_t));
        wSTRNCPY((char *)wifi_config.sta_config.ssid, setEncryp->ssid, ssid_len);
        wifi_config.sta_config.ssid_length = ssid_len;

        wifi_set_config(WIFI_MODE_STA, &wifi_config);

        setEncrypResp->status = STATUS_COMPLETE;
    }

done:
    wfaEncodeTLV(WFA_STA_SET_ENCRYPTION_RESP_TLV, 4, (BYTE *)setEncrypResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

int wfaStaSetSecurity(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetSecurity ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaSetSecurity Not supported yet\n");

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaSetEapTLS():
 *   This is to set
 *   1. ssid
 *   2. encrypType - tkip or aes-ccmp
 *   3. keyManagementType - wpa or wpa2
 *   4. trustedRootCA
 *   5. clientCertificate
 */
int wfaStaSetEapTLS(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetEapTLS_t *setTLS = (caStaSetEapTLS_t *)caCmdBuf;
//    char *ifname = setTLS->intf;
    dutCmdResponse_t *setEapTlsResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetEapTLS ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaSetEapTLS Not supported yet\n");

    setEapTlsResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_EAPTLS_RESP_TLV, 4, (BYTE *)setEapTlsResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * The function is to set
 *   1. ssid
 *   2. passPhrase
 *   3. keyMangementType - wpa/wpa2
 *   4. encrypType - tkip or aes-ccmp
 */
int wfaStaSetPSK(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    caStaSetPSK_t *setPSK = (caStaSetPSK_t *)caCmdBuf;
    dutCmdResponse_t *setPskResp = &gGenericResp;

    uint32_t ssid_len = wSTRLEN(setPSK->ssid);
    uint32_t pass_len = wSTRLEN((char *)setPSK->passphrase);
    wifi_config_t wifi_config;

    setPskResp->status = STATUS_COMPLETE;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetPSK ...\n");

    if (ssid_len == 0) {
        setPskResp->status = STATUS_ERROR;
        DPRINT_ERR(WFA_OUT, "The length of SSID is Zero.\n");
        goto done;
    }

    DPRINT_INFO(WFA_OUT, "ssid is %.*s\n", ssid_len, setPSK->ssid);

    wifi_get_config(WIFI_MODE_STA, &wifi_config);
    wMEMSET(&wifi_config, 0, sizeof(wifi_config_t));
    wSTRNCPY((char *)wifi_config.sta_config.ssid, setPSK->ssid, ssid_len);
    wifi_config.sta_config.ssid_length = ssid_len;

    if (pass_len != 0) {
        DPRINT_INFO(WFA_OUT, "passPhrase is %.*s\n", pass_len, setPSK->passphrase);

        if (!strncasecmp(setPSK->keyMgmtType, "wpa", 3)) {
            wSTRNCPY((char *)wifi_config.sta_config.password, (char *)setPSK->passphrase, pass_len);
            wifi_config.sta_config.password_length = pass_len;
        }
        else if (!strncasecmp(setPSK->keyMgmtType, "wpa2", 4)) {
            wSTRNCPY((char *)wifi_config.sta_config.password, (char *)setPSK->passphrase, pass_len);
            wifi_config.sta_config.password_length = pass_len;
        }
        else if (!strncasecmp(setPSK->keyMgmtType, "wpa-psk", 7)) {
            setPskResp->status = STATUS_ERROR;
            DPRINT_ERR(WFA_OUT, "No supported wpa-psk.\n");
            goto done;
        }
        else if (!strncasecmp(setPSK->keyMgmtType, "wpa2-psk", 8)) {
            setPskResp->status = STATUS_ERROR;
            DPRINT_ERR(WFA_OUT, "No supported wpa2-psk.\n");
            goto done;
        }
        else if (!strncasecmp(setPSK->keyMgmtType, "wpa2-ft", 7)) {
            setPskResp->status = STATUS_ERROR;
            DPRINT_ERR(WFA_OUT, "No supported wpa2-ft.\n");
            goto done;
        }
        else if (!strncasecmp(setPSK->keyMgmtType, "wpa3", 4)) {
            setPskResp->status = STATUS_ERROR;
            DPRINT_ERR(WFA_OUT, "No supported wpa3.\n");
            goto done;
        }
        else {
            setPskResp->status = STATUS_ERROR;
            DPRINT_ERR(WFA_OUT, "No specified keyMgmtType.\n");
            goto done;
        }
    }

    if (setPSK->encpType == 2) {
        DPRINT_INFO(WFA_OUT, "encpType is tkip\n");
    }
    else if (setPSK->encpType == 3) {
        DPRINT_INFO(WFA_OUT, "encpType is aes-ccmp\n");
    }
    else {
        setPskResp->status = STATUS_ERROR;
        DPRINT_ERR(WFA_OUT, "No specified encpType.\n");
        goto done;
    }

    /* Optional Arguments */
    DPRINT_INFO(WFA_OUT, "optional: PMF is %d\n", setPSK->pmf);
    DPRINT_INFO(WFA_OUT, "optional: micAlg is %.*s\n", wSTRLEN(setPSK->micAlg), setPSK->micAlg);

    wifi_set_config(WIFI_MODE_STA, &wifi_config);

done:
    wfaEncodeTLV(WFA_STA_SET_PSK_RESP_TLV, 4, (BYTE *)setPskResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaGetInfo():
 * Get vendor specific information in name/value pair by a wireless I/F.
 */
int wfaStaGetInfo(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    dutCommand_t *getInfo = (dutCommand_t *)caCmdBuf;

    /*
     * Normally this is called to retrieve the vendor information
     * from a interface, no implement yet
     */
    sprintf(infoResp.cmdru.info, "interface,%s,vendor,XXX,cardtype,802.11a/b/g", getInfo->intf);

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_GET_INFO_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}

/*
 * wfaStaSetEapTTLS():
 *   This is to set
 *   1. ssid
 *   2. username
 *   3. passwd
 *   4. encrypType - tkip or aes-ccmp
 *   5. keyManagementType - wpa or wpa2
 *   6. trustedRootCA
 */
int wfaStaSetEapTTLS(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetEapTTLS_t *setTTLS = (caStaSetEapTTLS_t *)caCmdBuf;
//    char *ifname = setTTLS->intf;
    dutCmdResponse_t *setEapTtlsResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetEapTTLS ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaSetEapTTLS Not supported yet\n");

    setEapTtlsResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_EAPTTLS_RESP_TLV, 4, (BYTE *)setEapTtlsResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaSetEapSIM():
 *   This is to set
 *   1. ssid
 *   2. user name
 *   3. passwd
 *   4. encrypType - tkip or aes-ccmp
 *   5. keyMangementType - wpa or wpa2
 */
int wfaStaSetEapSIM(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetEapSIM_t *setSIM = (caStaSetEapSIM_t *)caCmdBuf;
//    char *ifname = setSIM->intf;
    dutCmdResponse_t *setEapSimResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetEapSIM ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaSetEapSIM Not supported yet\n");

    setEapSimResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_EAPSIM_RESP_TLV, 4, (BYTE *)setEapSimResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaSetPEAP()
 *   This is to set
 *   1. ssid
 *   2. user name
 *   3. passwd
 *   4. encryType - tkip or aes-ccmp
 *   5. keyMgmtType - wpa or wpa2
 *   6. trustedRootCA
 *   7. innerEAP
 *   8. peapVersion
 */
int wfaStaSetPEAP(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetEapPEAP_t *setPEAP = (caStaSetEapPEAP_t *)caCmdBuf;
//    char *ifname = setPEAP->intf;
    dutCmdResponse_t *setPeapResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetPEAP ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaSetPEAP Not supported yet\n");

    setPeapResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_PEAP_RESP_TLV, 4, (BYTE *)setPeapResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaSetUAPSD()
 *    This is to set
 *    1. acBE
 *    2. acBK
 *    3. acVI
 *    4. acVO
 */
int wfaStaSetUAPSD(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *setUAPSDResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetUAPSD ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaSetUAPSD Not supported yet\n");

#if 0 /* used for only one specific device, need to update to reflect yours */
    caStaSetUAPSD_t *setUAPSD = (caStaSetUAPSD_t *)caCmdBuf;
    char *ifname = setUAPSD->intf;
    char tmpStr[10];
    char line[100];
    char *pathl="/etc/Wireless/RT61STA";
    BYTE acBE=1;
    BYTE acBK=1;
    BYTE acVO=1;
    BYTE acVI=1;
    BYTE APSDCapable;
    FILE *pipe;

    /*
     * A series of setting need to be done before doing WMM-PS
     * Additional steps of configuration may be needed.
     */

    /*
     * bring down the interface
     */
    sprintf(gCmdStr, "ifconfig %s down",ifname);
    sret = system(gCmdStr);
    /*
     * Unload the Driver
     */
    sprintf(gCmdStr, "rmmod rt61");
    sret = system(gCmdStr);
#ifndef WFA_WMM_AC
    if(setUAPSD->acBE != 1)
        acBE=setUAPSD->acBE = 0;
    if(setUAPSD->acBK != 1)
        acBK=setUAPSD->acBK = 0;
    if(setUAPSD->acVO != 1)
        acVO=setUAPSD->acVO = 0;
    if(setUAPSD->acVI != 1)
        acVI=setUAPSD->acVI = 0;
#else
    acBE=setUAPSD->acBE;
    acBK=setUAPSD->acBK;
    acVO=setUAPSD->acVO;
    acVI=setUAPSD->acVI;
#endif

    APSDCapable = acBE||acBK||acVO||acVI;
    /*
     * set other AC parameters
     */

    sprintf(tmpStr,"%d;%d;%d;%d",setUAPSD->acBE,setUAPSD->acBK,setUAPSD->acVI,setUAPSD->acVO);
    sprintf(gCmdStr, "sed -e \"s/APSDCapable=.*/APSDCapable=%d/g\" -e \"s/APSDAC=.*/APSDAC=%s/g\" %s/rt61sta.dat >/tmp/wfa_tmp",APSDCapable,tmpStr,pathl);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "mv /tmp/wfa_tmp %s/rt61sta.dat",pathl);
    sret = system(gCmdStr);
    pipe = popen("uname -r", "r");
    /* Read into line the output of uname*/
    fscanf(pipe,"%s",line);
    pclose(pipe);

    /*
     * load the Driver
     */
    sprintf(gCmdStr, "insmod /lib/modules/%s/extra/rt61.ko",line);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "ifconfig %s up",ifname);
    sret = system(gCmdStr);
#endif

    setUAPSDResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_UAPSD_RESP_TLV, 4, (BYTE *)setUAPSDResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

int wfaDeviceGetInfo(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    dutCommand_t *dutCmd = (dutCommand_t *)caCmdBuf;
//    caDevInfo_t *devInfo = &dutCmd->cmdsu.dev;
    dutCmdResponse_t *infoResp = &gGenericResp;

    /*a vendor can fill in the proper info or anything non-disclosure */
    caDeviceGetInfoResp_t dinfo = {"Netlink", "OPL2500", WFA_SYSTEM_VER, "SDK_v0.1"};

    DPRINT_INFO(WFA_OUT, "Entering wfaDeviceGetInfo ...\n");

    memcpy(&infoResp->cmdru.devInfo, &dinfo, sizeof(caDeviceGetInfoResp_t));

    infoResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_DEVICE_GET_INFO_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;

}

/*
 * This funciton is to retrieve a list of interfaces and return
 * the list back to Agent control.
 * ********************************************************************
 * Note: We intend to make this WLAN interface name as a hardcode name.
 * Therefore, for a particular device, you should know and change the name
 * for that device while doing porting. The MACRO "WFA_STAUT_IF" is defined in
 * the file "inc/wfa_ca.h". If the device OS is not linux-like, this most
 * likely is hardcoded just for CAPI command responses.
 * *******************************************************************
 *
 */
int wfaDeviceListIF(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *infoResp = &gGenericResp;
    dutCommand_t *ifList = (dutCommand_t *)caCmdBuf;
    caDeviceListIFResp_t *ifListResp = &infoResp->cmdru.ifList;

    DPRINT_INFO(WFA_OUT, "Entering wfaDeviceListIF ...\n");

    switch(ifList->cmdsu.iftype)
    {
    case IF_80211:
        infoResp->status = STATUS_COMPLETE;
        ifListResp->iftype = IF_80211;
        strcpy(ifListResp->ifs[0], WFA_STAUT_IF);
        strcpy(ifListResp->ifs[1], "NULL");
        strcpy(ifListResp->ifs[2], "NULL");
        break;
    case IF_ETH:
        infoResp->status = STATUS_COMPLETE;
        ifListResp->iftype = IF_ETH;
        strcpy(ifListResp->ifs[0], "eth0");
        strcpy(ifListResp->ifs[1], "NULL");
        strcpy(ifListResp->ifs[2], "NULL");
        break;
    default:
    {
        infoResp->status = STATUS_ERROR;
        wfaEncodeTLV(WFA_DEVICE_LIST_IF_RESP_TLV, 4, (BYTE *)infoResp, respBuf);
        *respLen = WFA_TLV_HDR_LEN + 4;

        return WFA_SUCCESS;
    }
    }

    wfaEncodeTLV(WFA_DEVICE_LIST_IF_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    return WFA_SUCCESS;
}

int wfaStaDebugSet(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *debugResp = &gGenericResp;
    dutCommand_t *debugSet = (dutCommand_t *)caCmdBuf;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaDebugSet ...\n");

    if(debugSet->cmdsu.dbg.state == 1) /* enable */
        wfa_defined_debug |= debugSet->cmdsu.dbg.level;
    else
        wfa_defined_debug = (~debugSet->cmdsu.dbg.level & wfa_defined_debug);

    debugResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_GET_INFO_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)debugResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);


    return WFA_SUCCESS;
}


/*
 *   wfaStaGetBSSID():
 *     This function is to retrieve BSSID of a specific wireless I/F.
 */
int wfaStaGetBSSID(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *bssidResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaGetBSSID ...\n");

    if (wts_dut_is_wifi_connected()) {
        //TODO
        extern int ctrl_wifi_bssid_get(uint8_t *p_bssid);
        uint8_t bssid[6];
        ctrl_wifi_bssid_get(&bssid[0]);
        sprintf(bssidResp->cmdru.bssid, MACSTR, MAC2STR(bssid));
    }
    else {
        wSTRCPY(bssidResp->cmdru.bssid, "00:00:00:00:00:00");
    }

    bssidResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_GET_BSSID_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)bssidResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaStaSetIBSS()
 *    This is to set
 *    1. ssid
 *    2. channel
 *    3. encrypType - none or wep
 *    optional
 *    4. key1
 *    5. key2
 *    6. key3
 *    7. key4
 *    8. activeIndex - 1, 2, 3, or 4
 */
int wfaStaSetIBSS(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetIBSS_t *setIBSS = (caStaSetIBSS_t *)caCmdBuf;
    dutCmdResponse_t *setIbssResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetIBSS ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaSetIBSS Not supported yet\n");

    setIbssResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_IBSS_RESP_TLV, 4, (BYTE *)setIbssResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 *  wfaSetMode():
 *  The function is to set the wireless interface with a given mode (possible
 *  adhoc)
 *  Input parameters:
 *    1. I/F
 *    2. ssid
 *    3. mode adhoc or managed
 *    4. encType
 *    5. channel
 *    6. key(s)
 *    7. active  key
 */
int wfaStaSetMode(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetMode_t *setmode = (caStaSetMode_t *)caCmdBuf;
    dutCmdResponse_t *SetModeResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetMode ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaSetMode Not supported yet\n");

    SetModeResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_MODE_RESP_TLV, 4, (BYTE *)SetModeResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

int wfaStaSetPwrSave(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetPwrSave_t *setps = (caStaSetPwrSave_t *)caCmdBuf;
    dutCmdResponse_t *SetPSResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetPwrSave ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaSetPwrSave Not supported yet\n");

    SetPSResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_PWRSAVE_RESP_TLV, 4, (BYTE *)SetPSResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

int wfaStaUpload(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    caStaUpload_t *upload = &((dutCommand_t *)caCmdBuf)->cmdsu.upload;
    dutCmdResponse_t *upLoadResp = &gGenericResp;
//    caStaUploadResp_t *upld = &upLoadResp->cmdru.uld;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaUpload ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaUpload Not supported yet\n");

    if(upload->type == WFA_UPLOAD_VHSO_RPT)
    {
#if 0
        int rbytes;
        /*
         * if asked for the first packet, always to open the file
         */
        if(upload->next == 1)
        {
            if(e2efp != NULL)
            {
                fclose(e2efp);
                e2efp = NULL;
            }

            e2efp = fopen(e2eResults, "r");
        }

        if(e2efp == NULL)
        {
            upLoadResp->status = STATUS_ERROR;
            wfaEncodeTLV(WFA_STA_UPLOAD_RESP_TLV, 4, (BYTE *)upLoadResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + 4;
            return WFA_FAILURE;
        }

        rbytes = fread(upld->bytes, 1, 256, e2efp);

        if(rbytes < 256)
        {
            /*
             * this means no more bytes after this read
             */
            upld->seqnum = 0;
            fclose(e2efp);
            e2efp=NULL;
        }
        else
        {
            upld->seqnum = upload->next;
        }

        upld->nbytes = rbytes;
#endif
        upLoadResp->status = STATUS_COMPLETE;
        wfaEncodeTLV(WFA_STA_UPLOAD_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)upLoadResp, respBuf);
        *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
    }
    else
    {
        upLoadResp->status = STATUS_ERROR;
        wfaEncodeTLV(WFA_STA_UPLOAD_RESP_TLV, 4, (BYTE *)upLoadResp, respBuf);
        *respLen = WFA_TLV_HDR_LEN + 4;
    }

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}
/*
 * wfaStaSetWMM()
 *  TO be ported on a specific plaform for the DUT
 *  This is to set the WMM related parameters at the DUT.
 *  Currently the function is used for GROUPS WMM-AC and WMM general configuration for setting RTS Threshhold, Fragmentation threshold and wmm (ON/OFF)
 *  It is expected that this function will set all the WMM related parametrs for a particular GROUP .
 */
int wfaStaSetWMM(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetWMM ...\n");

    DPRINT_INFO(WFA_OUT, "wfaStaSetWMM Not supported yet\n");

#ifdef WFA_WMM_AC
    caStaSetWMM_t *setwmm = (caStaSetWMM_t *)caCmdBuf;
    char *ifname = setwmm->intf;
    dutCmdResponse_t *setwmmResp = &gGenericResp;

    switch(setwmm->group)
    {
    case GROUP_WMMAC:
        if (setwmm->send_trig)
        {
            int Sockfd;
            struct sockaddr_in psToAddr;
            unsigned int TxMsg[512];

            Sockfd = wfaCreateUDPSock(setwmm->dipaddr, 12346);
            memset(&psToAddr, 0, sizeof(psToAddr));
            psToAddr.sin_family = AF_INET;
            psToAddr.sin_addr.s_addr = inet_addr(setwmm->dipaddr);
            psToAddr.sin_port = htons(12346);


            switch (setwmm->trig_ac)
            {
            case WMMAC_AC_VO:
                wfaTGSetPrio(Sockfd, 7);
                create_apts_msg(APTS_CK_VO, TxMsg, 0);
                printf("\r\nSending AC_VO trigger packet\n");
                break;

            case WMMAC_AC_VI:
                wfaTGSetPrio(Sockfd, 5);
                create_apts_msg(APTS_CK_VI, TxMsg, 0);
                printf("\r\nSending AC_VI trigger packet\n");
                break;

            case WMMAC_AC_BK:
                wfaTGSetPrio(Sockfd, 2);
                create_apts_msg(APTS_CK_BK, TxMsg, 0);
                printf("\r\nSending AC_BK trigger packet\n");
                break;

            default:
            case WMMAC_AC_BE:
                wfaTGSetPrio(Sockfd, 0);
                create_apts_msg(APTS_CK_BE, TxMsg, 0);
                printf("\r\nSending AC_BE trigger packet\n");
                break;
            }

            sendto(Sockfd, TxMsg, 256, 0, (struct sockaddr *)&psToAddr,
                   sizeof(struct sockaddr));
            close(Sockfd);
            usleep(1000000);
        }
        else if (setwmm->action == WMMAC_ADDTS)
        {
            printf("ADDTS AC PARAMS: dialog id: %d, TID: %d, "
                   "DIRECTION: %d, PSB: %d, UP: %d, INFOACK: %d BURST SIZE DEF: %d"
                   "Fixed %d, MSDU Size: %d, Max MSDU Size %d, "
                   "MIN SERVICE INTERVAL: %d, MAX SERVICE INTERVAL: %d, "
                   "INACTIVITY: %d, SUSPENSION %d, SERVICE START TIME: %d, "
                   "MIN DATARATE: %d, MEAN DATA RATE: %d, PEAK DATA RATE: %d, "
                   "BURSTSIZE or MSDU Aggreg: %d, DELAY BOUND: %d, PHYRATE: %d, SPLUSBW: %f, "
                   "MEDIUM TIME: %d, ACCESSCAT: %d\n",
                   setwmm->actions.addts.dialog_token,
                   setwmm->actions.addts.tspec.tsinfo.TID,
                   setwmm->actions.addts.tspec.tsinfo.direction,
                   setwmm->actions.addts.tspec.tsinfo.PSB,
                   setwmm->actions.addts.tspec.tsinfo.UP,
                   setwmm->actions.addts.tspec.tsinfo.infoAck,
                   setwmm->actions.addts.tspec.tsinfo.bstSzDef,
                   setwmm->actions.addts.tspec.Fixed,
                   setwmm->actions.addts.tspec.size,
                   setwmm->actions.addts.tspec.maxsize,
                   setwmm->actions.addts.tspec.min_srvc,
                   setwmm->actions.addts.tspec.max_srvc,
                   setwmm->actions.addts.tspec.inactivity,
                   setwmm->actions.addts.tspec.suspension,
                   setwmm->actions.addts.tspec.srvc_strt_tim,
                   setwmm->actions.addts.tspec.mindatarate,
                   setwmm->actions.addts.tspec.meandatarate,
                   setwmm->actions.addts.tspec.peakdatarate,
                   setwmm->actions.addts.tspec.burstsize,
                   setwmm->actions.addts.tspec.delaybound,
                   setwmm->actions.addts.tspec.PHYrate,
                   setwmm->actions.addts.tspec.sba,
                   setwmm->actions.addts.tspec.medium_time,
                   setwmm->actions.addts.accesscat);

            //tspec should be set here.

            sret = system(gCmdStr);
        }
        else if (setwmm->action == WMMAC_DELTS)
        {
            // send del tspec
        }

        setwmmResp->status = STATUS_COMPLETE;
        break;

    case GROUP_WMMCONF:
    #if 0
        sprintf(gCmdStr, "iwconfig %s rts %d",
                ifname,setwmm->actions.config.rts_thr);

        sret = system(gCmdStr);
        sprintf(gCmdStr, "iwconfig %s frag %d",
                ifname,setwmm->actions.config.frag_thr);

        sret = system(gCmdStr);
        sprintf(gCmdStr, "iwpriv %s wmmcfg %d",
                ifname, setwmm->actions.config.wmm);

        sret = system(gCmdStr);
        setwmmResp->status = STATUS_COMPLETE;
    #endif
        break;

    default:
        DPRINT_ERR(WFA_ERR, "The group %d is not supported\n",setwmm->group);
        setwmmResp->status = STATUS_ERROR;
        break;

    }

    wfaEncodeTLV(WFA_STA_SET_WMM_RESP_TLV, 4, (BYTE *)setwmmResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;
#endif

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

int wfaStaSendNeigReq(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *sendNeigReqResp = &gGenericResp;

    /*
     *  run your device to send NEIGREQ
     */

    sendNeigReqResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SEND_NEIGREQ_RESP_TLV, 4, (BYTE *)sendNeigReqResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

int wfaStaSetEapFAST(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetEapFAST_t *setFAST= (caStaSetEapFAST_t *)caCmdBuf;
//    char *ifname = setFAST->intf;
    dutCmdResponse_t *setEapFastResp = &gGenericResp;

#if 0
#ifdef WFA_NEW_CLI_FORMAT
    sprintf(gCmdStr, "wfa_set_eapfast %s %s %s %s %s %s", ifname, setFAST->ssid, setFAST->username,
            setFAST->passwd, setFAST->pacFileName,
            setFAST->innerEAP);
    sret = system(gCmdStr);
#else

    sprintf(gCmdStr, "wpa_cli -i %s disable_network 0", ifname);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 ssid '\"%s\"'", ifname, setFAST->ssid);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 identity '\"%s\"'", ifname, setFAST->username);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 password '\"%s\"'", ifname, setFAST->passwd);
    sret = system(gCmdStr);

    if(strcasecmp(setFAST->keyMgmtType, "wpa2-sha256") == 0)
    {
    }
    else if(strcasecmp(setFAST->keyMgmtType, "wpa2-eap") == 0)
    {
    }
    else if(strcasecmp(setFAST->keyMgmtType, "wpa2-ft") == 0)
    {

    }
    else if(strcasecmp(setFAST->keyMgmtType, "wpa") == 0)
    {
        sprintf(gCmdStr, "wpa_cli -i %s set_network 0 key_mgmt WPA-EAP", ifname);
    }
    else if(strcasecmp(setFAST->keyMgmtType, "wpa2") == 0)
    {
        // take all and device to pick one which is supported.
    }
    else
    {
        // ??
    }
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 eap FAST", ifname);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 pac_file '\"%s/%s\"'", ifname, CERTIFICATES_PATH,     setFAST->pacFileName);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 anonymous_identity '\"anonymous\"'", ifname);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 phase1 '\"fast_provisioning=1\"'", ifname);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 phase2 '\"auth=%s\"'", ifname,setFAST->innerEAP);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s enable_network 0", ifname);
    sret = system(gCmdStr);
#endif
#endif
    setEapFastResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_EAPFAST_RESP_TLV, 4, (BYTE *)setEapFastResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

int wfaStaSetEapAKA(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetEapAKA_t *setAKA= (caStaSetEapAKA_t *)caCmdBuf;
//    char *ifname = setAKA->intf;
    dutCmdResponse_t *setEapAkaResp = &gGenericResp;
#if 0
#ifdef WFA_NEW_CLI_FORMAT
    sprintf(gCmdStr, "wfa_set_eapaka %s %s %s %s", ifname, setAKA->ssid, setAKA->username, setAKA->passwd);
    sret = system(gCmdStr);
#else

    sprintf(gCmdStr, "wpa_cli -i %s disable_network 0", ifname);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 ssid '\"%s\"'", ifname, setAKA->ssid);
    sret = system(gCmdStr);

    if(strcasecmp(setAKA->keyMgmtType, "wpa2-sha256") == 0)
    {
    }
    else if(strcasecmp(setAKA->keyMgmtType, "wpa2-eap") == 0)
    {
    }
    else if(strcasecmp(setAKA->keyMgmtType, "wpa2-ft") == 0)
    {

    }
    else if(strcasecmp(setAKA->keyMgmtType, "wpa") == 0)
    {
        sprintf(gCmdStr, "wpa_cli -i %s set_network 0 key_mgmt WPA-EAP", ifname);
    }
    else if(strcasecmp(setAKA->keyMgmtType, "wpa2") == 0)
    {
        // take all and device to pick one which is supported.
    }
    else
    {
        // ??
    }
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 proto WPA2", ifname);
    sret = system(gCmdStr);
    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 proto CCMP", ifname);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 eap AKA", ifname);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 phase1 \"result_ind=1\"", ifname);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 identity '\"%s\"'", ifname, setAKA->username);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s set_network 0 password '\"%s\"'", ifname, setAKA->passwd);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "wpa_cli -i %s enable_network 0", ifname);
    sret = system(gCmdStr);
#endif
#endif
    setEapAkaResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_EAPAKA_RESP_TLV, 4, (BYTE *)setEapAkaResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

int wfaStaSetSystime(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
//    caStaSetSystime_t *systime = (caStaSetSystime_t *)caCmdBuf;
    dutCmdResponse_t *setSystimeResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaStaSetSystime ...\n");
#if 0
    sprintf(gCmdStr, "date %d-%d-%d",systime->month,systime->date,systime->year);
    sret = system(gCmdStr);

    sprintf(gCmdStr, "time %d:%d:%d", systime->hours,systime->minutes,systime->seconds);
    sret = system(gCmdStr);
#endif
    setSystimeResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_SYSTIME_RESP_TLV, 4, (BYTE *)setSystimeResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

#ifdef WFA_STA_TB

int wfaStaPresetParams(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *PresetParamsResp = &gGenericResp;
    caStaPresetParameters_t *presetParams = (caStaPresetParameters_t *)caCmdBuf;
    BYTE presetDone = 1;
    int st = 0;
   char cmdStr[128];
   char string[256];
   FILE *tmpfd = NULL;
   long val;
   char *endptr;

    DPRINT_INFO(WFA_OUT, "Inside wfaStaPresetParameters function ...\n");

   if (presetParams->supplicant == eWpaSupplicant)
   {
	st = access("/tmp/processid.txt", F_OK);
	if (st != -1)
	{
	    st = remove("/tmp/processid.txt");
	}

	sprintf(cmdStr, "/usr/local/sbin/findprocess.sh %s /tmp/processid.txt\n", "wpa_supplicant");
	st = system(cmdStr);

	tmpfd = fopen("/tmp/processid.txt", "r+");
	if (tmpfd == NULL)
	{
	    DPRINT_ERR(WFA_ERR, "process id file not exist\n");
	    return WFA_FAILURE;
	}

	for (;;)
	{
	    if (fgets(string, 256, tmpfd) == NULL)
	        break;

	    errno = 0;
	    val = strtol(string, &endptr, 10);
	    if (errno != 0 && val == 0)
	    {
		DPRINT_ERR(WFA_ERR, "strtol error\n");
		return WFA_FAILURE;
	    }

	    if (endptr == string)
	    {
		DPRINT_ERR(WFA_ERR, "No wpa_supplicant instance was found\n");
	    }

	    presetDone = 1;
	}
   }

    if(presetParams->wmmFlag)
    {
        st = wfaExecuteCLI(gCmdStr);
        switch(st)
        {
        case 0:
            presetDone = 1;
            break;
        case 1:
            presetDone = 0;
            break;
        case 2:
            presetDone = 0;
            break;
        }
    }

    if(presetParams->modeFlag != 0)
    {
        switch(presetParams->wirelessMode)
        {
        default:
            printf("other mode does not need to support\n");
        }

        st = wfaExecuteCLI(gCmdStr);
        switch(st)
        {
        case 0:
            presetDone = 1;
            break;
        case 1:
            presetDone = 0;
        case 2:
            presetDone = 0;
            break;
        }
    }


    if(presetParams->psFlag)
    {

        printf("%s\n", gCmdStr);
        sret = system(gCmdStr);
    }

    /************the followings are used for Voice Enterprise **************/
    if(presetParams->program == PROG_TYPE_VENT)
    {
        if(presetParams->ftoa == eEnable)
        {
            // enable Fast BSS Transition Over the Air
        }
        else
        {
            // disable Fast BSS Transition Over the Air
        }

        if(presetParams->ftds == eEnable)
        {
            // enable Fast BSS Transition Over the DS
        }
        else
        {
            // disable Fast BSS Transition Over the DS
        }

        if(presetParams->activescan == eEnable)
        {
            // Enable Active Scan on STA
        }
        else
        {
            // disable Active Scan on STA
        }
    }

    /************the followings are used for Wi-Fi Display *************/
    if(presetParams->program == PROG_TYPE_WFD)
    {

        if(presetParams->tdlsFlag)
        {
            // enable / disable tdls based on tdls
        }
        if(presetParams->wfdDevTypeFlag)
        {
            // set WFD device type to source/sink/dual based on wfdDevType
        }
        if(presetParams->wfdUibcGenFlag)
        {
            // enable / disable the feature
        }
        if(presetParams->wfdUibcHidFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdUiInputFlag)
        {
            // set the UI input as mentioned
        }
        if(presetParams->wfdHdcpFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdFrameSkipFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdAvChangeFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdStandByFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdInVideoFlag)
        {
            // select the input vide as protecteed or non-protetcted or protected audio
            // or unprotected audio etc.
        }

        if(presetParams->wfdVideoFmatFlag)
        {
            // set the video format as requested

            //switch(presetParams->wfdVideoFmt )
            //{
            // case e640x480p60:
            //   ;
            // default:
            // set the mandatory
            // }
        }
        if(presetParams->wfdAudioFmatFlag)
        {
            // set the Audio format as requested

            //switch(presetParams->wfdAudioFmt )
            //{
            // case eMandatoryAudioMode:
            //	;
            // case eDefaultAudioMode:
            //  ;

            // default:
            // set the mandatory
            // }
        }

        if(presetParams->wfdI2cFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdVideoRecoveryFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdPrefDisplayFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdServiceDiscoveryFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfd3dVideoFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdMultiTxStreamFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdTimeSyncFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdEDIDFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdUIBCPrepareFlag)
        {
            // Provdes information to start valid WFD session to check UIBC operation.
        }
        if(presetParams->wfdCoupledCapFlag)
        {
            // enable / disable feature
        }
        if(presetParams->wfdOptionalFeatureFlag)
        {
            // disable all program specific optional features
        }
        if(presetParams->wfdSessionAvailFlag)
        {
            // enable / disable session available bit
        }
        if(presetParams->wfdDeviceDiscoverabilityFlag)
        {
            // enable / disable feature
        }
    }

   if(presetParams->program == PROG_TYPE_WFDS)
   {

	   if(presetParams->wfdsType == eAcceptPD)
	   {
	      // preset to accept PD request
		 if (presetParams->wfdsConnectionCapabilityFlag == 1)
		 {
		 	// use  presetParams->wfdsConnectionCapability and set role accordingly
		 }

	   }
	   if(presetParams->wfdsType == eRejectPD)
	   {
	      // preset to Reject PD request
	   }
	   if(presetParams->wfdsType == eIgnorePD)
	   {
	      // preset to Ignore PD request
	   }
	   if(presetParams->wfdsType == eRejectSession)
	   {
	      // preset to reject Session request
	   }

   }

   if (presetDone)
   {
      PresetParamsResp->status = STATUS_COMPLETE;
   }
   else
   {
      PresetParamsResp->status = STATUS_INVALID;
   }

    wfaEncodeTLV(WFA_STA_PRESET_PARAMETERS_RESP_TLV, 4, (BYTE *)PresetParamsResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

int wfaStaSet11n(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *v11nParamsResp = &gGenericResp;

    v11nParamsResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, 4, (BYTE *)v11nParamsResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;
    return WFA_SUCCESS;
}
int wfaStaSetWireless(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *staWirelessResp = &gGenericResp;

    staWirelessResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_WIRELESS_RESP_TLV, 4, (BYTE *)staWirelessResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;
    return WFA_SUCCESS;
}

int wfaStaSendADDBA(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *staSendADDBAResp = &gGenericResp;

    wfaEncodeTLV(WFA_STA_SET_SEND_ADDBA_RESP_TLV, 4, (BYTE *)staSendADDBAResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;
    return WFA_SUCCESS;
}

int wfaStaSetRIFS(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *staSetRIFSResp = &gGenericResp;

    wfaEncodeTLV(WFA_STA_SET_RIFS_TEST_RESP_TLV, 4, (BYTE *)staSetRIFSResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

int wfaStaSendCoExistMGMT(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *staSendMGMTResp = &gGenericResp;

    wfaEncodeTLV(WFA_STA_SEND_COEXIST_MGMT_RESP_TLV, 4, (BYTE *)staSendMGMTResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;

}

int wfaStaResetDefault(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    caStaResetDefault_t *reset = (caStaResetDefault_t *)caCmdBuf;
    dutCmdResponse_t *ResetResp = &gGenericResp;


    // need to make your own command available for this, here is only an example
    sprintf(gCmdStr, "myresetdefault %s program %s", reset->intf, reset->prog);
    sret = system(gCmdStr);

    ResetResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_RESET_DEFAULT_RESP_TLV, 4, (BYTE *)ResetResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

#else

int wfaStaTestBedCmd(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *staCmdResp = &gGenericResp;

    wfaEncodeTLV(WFA_STA_DISCONNECT_RESP_TLV, 4, (BYTE *)staCmdResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}
#endif

/*
 * This is used to send a frame or action frame
 */
int wfaStaDevSendFrame(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCommand_t *cmd = (dutCommand_t *)caCmdBuf;
    /* uncomment it if needed */
    // char *ifname = cmd->intf;
    dutCmdResponse_t *devSendResp = &gGenericResp;
    caStaDevSendFrame_t *sf = &cmd->cmdsu.sf;

    DPRINT_INFO(WFA_OUT, "Inside wfaStaDevSendFrame function ...\n");
    /* processing the frame */

    switch(sf->program)
    {
    case PROG_TYPE_PMF:
    {
        pmfFrame_t *pmf = &sf->frameType.pmf;
        switch(pmf->eFrameName)
        {
        case PMF_TYPE_DISASSOC:
        {
            /* use the protected to set what type of key to send */

        }
        break;
        case PMF_TYPE_DEAUTH:
        {

        }
        break;
        case PMF_TYPE_SAQUERY:
        {

        }
        break;
        case PMF_TYPE_AUTH:
        {
        }
        break;
        case PMF_TYPE_ASSOCREQ:
        {
        }
        break;
        case PMF_TYPE_REASSOCREQ:
        {
        }
        break;
        }
    }
    break;
    case PROG_TYPE_TDLS:
    {
        tdlsFrame_t *tdls = &sf->frameType.tdls;
        switch(tdls->eFrameName)
        {
        case TDLS_TYPE_DISCOVERY:
            /* use the peer mac address to send the frame */
            break;
        case TDLS_TYPE_SETUP:
            break;
        case TDLS_TYPE_TEARDOWN:
            break;
        case TDLS_TYPE_CHANNELSWITCH:
            break;
        case TDLS_TYPE_NULLFRAME:
            break;
        }
    }
    break;
    case PROG_TYPE_VENT:
    {
        ventFrame_t *vent = &sf->frameType.vent;
        switch(vent->type)
        {
        case VENT_TYPE_NEIGREQ:
            break;
        case VENT_TYPE_TRANSMGMT:
            break;
        }
    }
    break;
    case PROG_TYPE_WFD:
    {
        wfdFrame_t *wfd = &sf->frameType.wfd;
        switch(wfd->eframe)
        {
        case WFD_FRAME_PRBREQ:
        {
            /* send probe req */
        }
        break;

        case WFD_FRAME_PRBREQ_TDLS_REQ:
        {
            /* send tunneled tdls probe req  */
        }
        break;

        case WFD_FRAME_11V_TIMING_MSR_REQ:
        {
            /* send 11v timing mearurement request */
        }
        break;

        case WFD_FRAME_RTSP:
        {
            /* send WFD RTSP messages*/
            // fetch the type of RTSP message and send it.
            switch(wfd->eRtspMsgType)
            {
            case WFD_RTSP_PAUSE:
                break;
            case WFD_RTSP_PLAY:
                //send RTSP PLAY
                break;
            case WFD_RTSP_TEARDOWN:
                //send RTSP TEARDOWN
                break;
            case WFD_RTSP_TRIG_PAUSE:
                //send RTSP TRIGGER PAUSE
                break;
            case WFD_RTSP_TRIG_PLAY:
                //send RTSP TRIGGER PLAY
                break;
            case WFD_RTSP_TRIG_TEARDOWN:
                //send RTSP TRIGGER TEARDOWN
                break;
            case WFD_RTSP_SET_PARAMETER:
                //send RTSP SET PARAMETER
                if (wfd->eSetParams == WFD_CAP_UIBC_KEYBOARD)
                {
                    //send RTSP SET PARAMETER message for UIBC keyboard
                }
                if (wfd->eSetParams == WFD_CAP_UIBC_MOUSE)
                {
                    //send RTSP SET PARAMETER message for UIBC Mouse
                }
                else if (wfd->eSetParams == WFD_CAP_RE_NEGO)
                {
                    //send RTSP SET PARAMETER message Capability re-negotiation
                }
                else if (wfd->eSetParams == WFD_STANDBY)
                {
                    //send RTSP SET PARAMETER message for standby
                }
                else if (wfd->eSetParams == WFD_UIBC_SETTINGS_ENABLE)
                {
                    //send RTSP SET PARAMETER message for UIBC settings enable
                }
                else if (wfd->eSetParams == WFD_UIBC_SETTINGS_DISABLE)
                {
                    //send RTSP SET PARAMETER message for UIBC settings disable
                }
                else if (wfd->eSetParams == WFD_ROUTE_AUDIO)
                {
                    //send RTSP SET PARAMETER message for route audio
                }
                else if (wfd->eSetParams == WFD_3D_VIDEOPARAM)
                {
                    //send RTSP SET PARAMETER message for 3D video parameters
                }
                else if (wfd->eSetParams == WFD_2D_VIDEOPARAM)
                {
                    //send RTSP SET PARAMETER message for 2D video parameters
                }
                break;
            }
        }
        break;
        }
    }
    break;
    /* not need to support HS2 release 1, due to very short time period  */
    case PROG_TYPE_HS2_R2:
    {
        /* type of frames */
        hs2Frame_t *hs2 = &sf->frameType.hs2_r2;
        switch(hs2->eframe)
        {
        case HS2_FRAME_ANQPQuery:
        {

        }
        break;
        case HS2_FRAME_DLSRequest:
        {

        }
        break;
        case HS2_FRAME_GARPReq:
        {

        }
        break;
        case HS2_FRAME_GARPRes:
        {
        }
        break;
        case HS2_FRAME_NeighAdv:
        {
        }
        case HS2_FRAME_ARPProbe:
        {
        }
        case HS2_FRAME_ARPAnnounce:
        {

        }
        break;
        case HS2_FRAME_NeighSolicitReq:
        {

        }
        break;
        case HS2_FRAME_ARPReply:
        {

        }
        break;
        }

        }/*  PROG_TYPE_HS2-R2  */
    case PROG_TYPE_GEN:
    {
        /* General frames */
    }


    }
    devSendResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_DEV_SEND_FRAME_RESP_TLV, 4, (BYTE *)devSendResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

/*
 * This is used to set a temporary MAC address of an interface
 */
int wfaStaSetMacAddr(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    // Uncomment it if needed
    //dutCommand_t *cmd = (dutCommand_t *)caCmdBuf;
    // char *ifname = cmd->intf;
    dutCmdResponse_t *staCmdResp = &gGenericResp;
    // Uncomment it if needed
    //char *macaddr = &cmd->cmdsu.macaddr[0];

    wfaEncodeTLV(WFA_STA_SET_MAC_ADDRESS_RESP_TLV, 4, (BYTE *)staCmdResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}


int wfaStaDisconnect(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    //dutCommand_t *disc = (dutCommand_t *)caCmdBuf;
    //char *intf = disc->intf;
    dutCmdResponse_t *staDiscResp = &gGenericResp;

    // stop the supplicant

    staDiscResp->status = STATUS_COMPLETE;

    wfaEncodeTLV(WFA_STA_DISCONNECT_RESP_TLV, 4, (BYTE *)staDiscResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

/* Execute CLI, read the status from Environment variable */
int wfaExecuteCLI(char *CLI)
{
#if 0
    char *retstr;

    sret = system(CLI);

    retstr = getenv("WFA_CLI_STATUS");
    printf("cli status %s\n", retstr);
    return atoi(retstr);
#else
    return 0;
#endif
}

/* Supporting Functions */
typedef struct {
    void    *ping_h;
    uint32_t sent;
    uint32_t replies;
}wfa_cs_ping_t;
wfa_cs_ping_t g_wfa_cs_ping = {0};

static void wfa_cs_on_ping_success(void *hdl, void *args)
{
    uint8_t ttl;
    uint16_t seqno;
    uint32_t elapsed_time, recv_len;
    ip_addr_t target_addr;
    ping_get_profile(hdl, PING_PROF_SEQNO, &seqno, sizeof(seqno));
    ping_get_profile(hdl, PING_PROF_TTL, &ttl, sizeof(ttl));
    ping_get_profile(hdl, PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    ping_get_profile(hdl, PING_PROF_SIZE, &recv_len, sizeof(recv_len));
    ping_get_profile(hdl, PING_PROF_TIMEGAP, &elapsed_time, sizeof(elapsed_time));
    DPRINT_INFO(WFA_OUT, "%d bytes from %s seq=%d ttl=%d time=%d ms\r\n",
              recv_len, ipaddr_ntoa((ip_addr_t*)&target_addr), seqno, ttl, elapsed_time);
}

static void wfa_cs_on_ping_timeout(void *hdl, void *args)
{
    uint16_t seqno;
    ip_addr_t target_addr;
    ping_get_profile(hdl, PING_PROF_SEQNO, &seqno, sizeof(seqno));
    ping_get_profile(hdl, PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    DPRINT_INFO(WFA_OUT, "From %s seq=%d timeout\r\n", ipaddr_ntoa((ip_addr_t*)&target_addr), seqno);
}

static void wfa_cs_on_ping_end(void *hdl, void *args)
{
    ip_addr_t target_addr;
    uint32_t transmitted;
    uint32_t received;
    uint32_t total_time_ms;
    ping_get_profile(hdl, PING_PROF_REQUEST, &transmitted, sizeof(transmitted));
    ping_get_profile(hdl, PING_PROF_REPLY, &received, sizeof(received));
    ping_get_profile(hdl, PING_PROF_IPADDR, &target_addr, sizeof(target_addr));
    ping_get_profile(hdl, PING_PROF_DURATION, &total_time_ms, sizeof(total_time_ms));
    double loss = (double)((1 - ((float)received) / transmitted) * 100);

    g_wfa_cs_ping.sent    = transmitted;
    g_wfa_cs_ping.replies = received;

    if (IP_IS_V4(&target_addr))
        DPRINT_INFO(WFA_OUT, "--- %s ping statistics ---\r\n", inet_ntoa(*ip_2_ip4(&target_addr)));

    DPRINT_INFO(WFA_OUT, "%d transmitted, %d received, %u lost (%.2f%% loss), average time %u ms\r\n",
              transmitted, received, (transmitted - received), loss, (total_time_ms / received));

    // delete the ping sessions, so that we clean up all resources and can create a new ping session
    // we don't have to call delete function in the callback, instead we can call delete function from other tasks
    ping_delete_session(hdl);
}

void wfaSendPing(tgPingStart_t *staPing, float *interval, int streamid)
{
    if (g_wfa_cs_ping.ping_h != NULL)
        return;

    ping_config_t config = PING_DEFAULT_CONFIG();
    ping_callbacks_t ping_cbs = {0};

    config.target_addr.u_addr.ip4.addr = ipaddr_addr(staPing->dipaddr);
    config.target_addr.type = IPADDR_TYPE_V4;
    config.count       = staPing->frameRate * staPing->duration;
    config.data_size   = staPing->frameSize;
    config.timeout_ms  = (uint32_t)((float)*interval*1000);
    config.interval_ms = config.timeout_ms;

    ping_cbs.cb_args = NULL;
    ping_cbs.on_ping_success = wfa_cs_on_ping_success;
    ping_cbs.on_ping_timeout = wfa_cs_on_ping_timeout;
    ping_cbs.on_ping_end     = wfa_cs_on_ping_end;

    ping_new_session(&config, &ping_cbs, &g_wfa_cs_ping.ping_h);
    ping_start(g_wfa_cs_ping.ping_h);
}

int wfaStopPing(dutCmdResponse_t *stpResp, int streamid)
{
    ping_stop(g_wfa_cs_ping.ping_h);

    g_wfa_cs_ping.ping_h = NULL;

    osDelay(300);

    stpResp->cmdru.pingStp.sendCnt    = g_wfa_cs_ping.sent;
    stpResp->cmdru.pingStp.repliedCnt = g_wfa_cs_ping.replies;

    DPRINT_INFO(WFA_OUT, "sent %u replies %u\n", stpResp->cmdru.pingStp.sendCnt, stpResp->cmdru.pingStp.repliedCnt);

    return WFA_SUCCESS;
}

/*
 * wfaStaGetP2pDevAddress():
 */
int wfaStaGetP2pDevAddress(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* dutCommand_t *getInfo = (dutCommand_t *)caCmdBuf; */

    printf("\n Entry wfaStaGetP2pDevAddress... ");

    // Fetch the device ID and store into infoResp->cmdru.devid
    //strcpy(infoResp->cmdru.devid, str);
    strcpy(&infoResp.cmdru.devid[0], "ABCDEFGH");

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_GET_DEV_ADDRESS_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}



/*
 * wfaStaSetP2p():
 */
int wfaStaSetP2p(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* caStaSetP2p_t *getStaSetP2p = (caStaSetP2p_t *)caCmdBuf; uncomment and use it*/

    printf("\n Entry wfaStaSetP2p... ");

    // Implement the function and this does not return any thing back.

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_SETP2P_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}
/*
 * wfaStaP2pConnect():
 */
int wfaStaP2pConnect(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* caStaP2pConnect_t *getStaP2pConnect = (caStaP2pConnect_t *)caCmdBuf; uncomment and use it */

    printf("\n Entry wfaStaP2pConnect... ");

    // Implement the function and does not return anything.


    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_CONNECT_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}

/*
 * wfaStaStartAutoGo():
 */
int wfaStaStartAutoGo(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    //caStaStartAutoGo_t *getStaStartAutoGo = (caStaStartAutoGo_t *)caCmdBuf;

    printf("\n Entry wfaStaStartAutoGo... ");

    // Fetch the group ID and store into 	infoResp->cmdru.grpid
    strcpy(&infoResp.cmdru.grpid[0], "ABCDEFGH");

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_START_AUTO_GO_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}




/*
 * wfaStaP2pStartGrpFormation():
 */
int wfaStaP2pStartGrpFormation(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;

    printf("\n Entry wfaStaP2pStartGrpFormation... ");

    strcpy(infoResp.cmdru.grpFormInfo.result, "CLIENT");
    strcpy(infoResp.cmdru.grpFormInfo.grpId, "AA:BB:CC:DD:EE:FF_DIRECT-SSID");


    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_START_GRP_FORMATION_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}


/*
 * wfaStaP2pDissolve():
 */
int wfaStaP2pDissolve(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;

    printf("\n Entry wfaStaP2pDissolve... ");

    // Implement the function and this does not return any thing back.

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_DISSOLVE_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}

/*
 * wfaStaSendP2pInvReq():
 */
int wfaStaSendP2pInvReq(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* caStaSendP2pInvReq_t *getStaP2pInvReq= (caStaSendP2pInvReq_t *)caCmdBuf; */

    printf("\n Entry wfaStaSendP2pInvReq... ");

    // Implement the function and this does not return any thing back.

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_SEND_INV_REQ_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}


/*
 * wfaStaAcceptP2pInvReq():
 */
int wfaStaAcceptP2pInvReq(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* uncomment and use it
     * caStaAcceptP2pInvReq_t *getStaP2pInvReq= (caStaAcceptP2pInvReq_t *)caCmdBuf;
     */

    printf("\n Entry wfaStaAcceptP2pInvReq... ");

    // Implement the function and this does not return any thing back.

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_ACCEPT_INV_REQ_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}


/*
 * wfaStaSendP2pProvDisReq():
 */
int wfaStaSendP2pProvDisReq(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* uncomment and use it
     * caStaSendP2pProvDisReq_t *getStaP2pProvDisReq= (caStaSendP2pProvDisReq_t *)caCmdBuf;
     */

    printf("\n Entry wfaStaSendP2pProvDisReq... ");

    // Implement the function and this does not return any thing back.

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_SEND_PROV_DIS_REQ_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}

/*
 * wfaStaSetWpsPbc():
 */
int wfaStaSetWpsPbc(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* uncomment and use it
     * caStaSetWpsPbc_t *getStaSetWpsPbc= (caStaSetWpsPbc_t *)caCmdBuf;
     */

    printf("\n Entry wfaStaSetWpsPbc... ");

    // Implement the function and this does not return any thing back.

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_WPS_SETWPS_PBC_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}

/*
 * wfaStaWpsReadPin():
 */
int wfaStaWpsReadPin(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* uncomment and use it
     * caStaWpsReadPin_t *getStaWpsReadPin= (caStaWpsReadPin_t *)caCmdBuf;
     */

    printf("\n Entry wfaStaWpsReadPin... ");

    // Fetch the device PIN and put in 	infoResp->cmdru.wpsPin
    //strcpy(infoResp->cmdru.wpsPin, "12345678");
    strcpy(&infoResp.cmdru.wpsPin[0], "1234456");


    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_WPS_READ_PIN_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}



/*
 * wfaStaWpsReadLabel():
 */
int wfaStaWpsReadLabel(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;

    printf("\n Entry wfaStaWpsReadLabel... ");

    // Fetch the device Label and put in	infoResp->cmdru.wpsPin
    //strcpy(infoResp->cmdru.wpsPin, "12345678");
    strcpy(&infoResp.cmdru.wpsPin[0], "1234456");


    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_WPS_READ_PIN_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}


/*
 * wfaStaWpsEnterPin():
 */
int wfaStaWpsEnterPin(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* uncomment and use it
     * caStaWpsEnterPin_t *getStaWpsEnterPin= (caStaWpsEnterPin_t *)caCmdBuf;
     */

    printf("\n Entry wfaStaWpsEnterPin... ");

    // Implement the function and this does not return any thing back.


    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_WPS_ENTER_PIN_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}


/*
 * wfaStaGetPsk():
 */
int wfaStaGetPsk(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* caStaGetPsk_t *getStaGetPsk= (caStaGetPsk_t *)caCmdBuf; uncomment and use it */

    printf("\n Entry wfaStaGetPsk... ");


    // Fetch the device PP and SSID  and put in 	infoResp->cmdru.pskInfo
    strcpy(&infoResp.cmdru.pskInfo.passPhrase[0], "1234456");
    strcpy(&infoResp.cmdru.pskInfo.ssid[0], "WIFI_DIRECT");


    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_GET_PSK_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}

/*
 * wfaStaP2pReset():
 */
int wfaStaP2pReset(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* dutCommand_t *getStaP2pReset= (dutCommand_t *)caCmdBuf; */

    printf("\n Entry wfaStaP2pReset... ");
    // Implement the function and this does not return any thing back.

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_RESET_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}



/*
 * wfaStaGetP2pIpConfig():
 */
int wfaStaGetP2pIpConfig(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* caStaGetP2pIpConfig_t *staGetP2pIpConfig= (caStaGetP2pIpConfig_t *)caCmdBuf; */

    caStaGetIpConfigResp_t *ifinfo = &(infoResp.cmdru.getIfconfig);

    printf("\n Entry wfaStaGetP2pIpConfig... ");

    ifinfo->isDhcp =0;
    strcpy(&(ifinfo->ipaddr[0]), "192.165.100.111");
    strcpy(&(ifinfo->mask[0]), "255.255.255.0");
    strcpy(&(ifinfo->dns[0][0]), "192.165.100.1");
    strcpy(&(ifinfo->mac[0]), "ba:ba:ba:ba:ba:ba");

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_GET_IP_CONFIG_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}




/*
 * wfaStaSendServiceDiscoveryReq():
 */
int wfaStaSendServiceDiscoveryReq(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;

    printf("\n Entry wfaStaSendServiceDiscoveryReq... ");
    // Implement the function and this does not return any thing back.


    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_SEND_SERVICE_DISCOVERY_REQ_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}



/*
 * wfaStaSendP2pPresenceReq():
 */
int wfaStaSendP2pPresenceReq(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_SEND_PRESENCE_REQ_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}

/*
 * wfaStaSetSleepReq():
 */
int wfaStaSetSleepReq(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* caStaSetSleep_t *staSetSleepReq= (caStaSetSleep_t *)caCmdBuf; */

    printf("\n Entry wfaStaSetSleepReq... ");
    // Implement the function and this does not return any thing back.


    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_SET_SLEEP_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN +4;

    return WFA_SUCCESS;
}

/*
 * wfaStaSetOpportunisticPsReq():
 */
int wfaStaSetOpportunisticPsReq(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;

    printf("\n Entry wfaStaSetOpportunisticPsReq... ");
    // Implement the function and this does not return any thing back.


    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_SET_OPPORTUNISTIC_PS_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}
#ifndef WFA_STA_TB
/*
 * wfaStaPresetParams():
 */

int wfaStaPresetParams(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;

    DPRINT_INFO(WFA_OUT, "Inside wfaStaPresetParameters function ...\n");

    // Implement the function and its sub commands
    infoResp.status = STATUS_COMPLETE;

    wfaEncodeTLV(WFA_STA_PRESET_PARAMETERS_RESP_TLV, 4, (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}
int wfaStaSet11n(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{

    dutCmdResponse_t infoResp;
    dutCmdResponse_t *v11nParamsResp = &infoResp;

#ifdef WFA_11N_SUPPORT_ONLY

    caSta11n_t * v11nParams = (caSta11n_t *)caCmdBuf;

    int st =0; // SUCCESS

    DPRINT_INFO(WFA_OUT, "Inside wfaStaSet11n function....\n");

    if(v11nParams->addba_reject != 0xFF && v11nParams->addba_reject < 2)
    {
        // implement the funciton
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_addba_reject failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->ampdu != 0xFF && v11nParams->ampdu < 2)
    {
        // implement the funciton

        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_ampdu failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->amsdu != 0xFF && v11nParams->amsdu < 2)
    {
        // implement the funciton
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_amsdu failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->greenfield != 0xFF && v11nParams->greenfield < 2)
    {
        // implement the funciton
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "_set_greenfield failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->mcs32!= 0xFF && v11nParams->mcs32 < 2 && v11nParams->mcs_fixedrate[0] != '\0')
    {
        // implement the funciton
        //st = wfaExecuteCLI(gCmdStr);
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_mcs failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }
    else if (v11nParams->mcs32!= 0xFF && v11nParams->mcs32 < 2 && v11nParams->mcs_fixedrate[0] == '\0')
    {
        // implement the funciton
        //st = wfaExecuteCLI(gCmdStr);
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_mcs32 failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }
    else if (v11nParams->mcs32 == 0xFF && v11nParams->mcs_fixedrate[0] != '\0')
    {
        // implement the funciton
        //st = wfaExecuteCLI(gCmdStr);
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_mcs32 failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->rifs_test != 0xFF && v11nParams->rifs_test < 2)
    {
        // implement the funciton
        //st = wfaExecuteCLI(gCmdStr);
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_rifs_test failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->sgi20 != 0xFF && v11nParams->sgi20 < 2)
    {
        // implement the funciton
        //st = wfaExecuteCLI(gCmdStr);
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_sgi20 failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->smps != 0xFFFF)
    {
        if(v11nParams->smps == 0)
        {
            // implement the funciton
            //st = wfaExecuteCLI(gCmdStr);
        }
        else if(v11nParams->smps == 1)
        {
            // implement the funciton
            //st = wfaExecuteCLI(gCmdStr);
            ;
        }
        else if(v11nParams->smps == 2)
        {
            // implement the funciton
            //st = wfaExecuteCLI(gCmdStr);
            ;
        }
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_smps failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->stbc_rx != 0xFFFF)
    {
        // implement the funciton
        //st = wfaExecuteCLI(gCmdStr);
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_stbc_rx failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->width[0] != '\0')
    {
        // implement the funciton
        //st = wfaExecuteCLI(gCmdStr);
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_11n_channel_width failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->_40_intolerant != 0xFF && v11nParams->_40_intolerant < 2)
    {
        // implement the funciton
        //st = wfaExecuteCLI(gCmdStr);
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_40_intolerant failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

    if(v11nParams->txsp_stream != 0 && v11nParams->txsp_stream <4)
    {
        // implement the funciton
        //st = wfaExecuteCLI(gCmdStr);
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_txsp_stream failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }

    }

    if(v11nParams->rxsp_stream != 0 && v11nParams->rxsp_stream < 4)
    {
        // implement the funciton
        //st = wfaExecuteCLI(gCmdStr);
        if(st != 0)
        {
            v11nParamsResp->status = STATUS_ERROR;
            strcpy(v11nParamsResp->cmdru.info, "set_rxsp_stream failed");
            wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)v11nParamsResp, respBuf);
            *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
            return FALSE;
        }
    }

#endif

    v11nParamsResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_11N_RESP_TLV, 4, (BYTE *)v11nParamsResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;
    return WFA_SUCCESS;
}
#endif
/*
 * wfaStaAddArpTableEntry():
 */
int wfaStaAddArpTableEntry(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* caStaAddARPTableEntry_t *staAddARPTableEntry= (caStaAddARPTableEntry_t *)caCmdBuf; uncomment and use it */

    printf("\n Entry wfastaAddARPTableEntry... ");
    // Implement the function and this does not return any thing back.

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_ADD_ARP_TABLE_ENTRY_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}

/*
 * wfaStaBlockICMPResponse():
 */
int wfaStaBlockICMPResponse(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    /* caStaBlockICMPResponse_t *staAddARPTableEntry= (caStaBlockICMPResponse_t *)caCmdBuf; uncomment and use it */

    printf("\n Entry wfaStaBlockICMPResponse... ");
    // Implement the function and this does not return any thing back.

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_P2P_BLOCK_ICMP_RESPONSE_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}

/*
 * wfaStaSetRadio():
 */

int wfaStaSetRadio(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCommand_t *setRadio = (dutCommand_t *)caCmdBuf;
    dutCmdResponse_t *staCmdResp = &gGenericResp;
    caStaSetRadio_t *sr = &setRadio->cmdsu.sr;

    if(sr->mode == WFA_OFF)
    {
        // turn radio off
    }
    else
    {
        // always turn the radio on
    }

    staCmdResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_RADIO_RESP_TLV, 4, (BYTE *)staCmdResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

/*
 * wfaStaSetRFeature():
 */

int wfaStaSetRFeature(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCommand_t *dutCmd = (dutCommand_t *)caCmdBuf;
    caStaRFeat_t *rfeat = &dutCmd->cmdsu.rfeat;
    dutCmdResponse_t *caResp = &gGenericResp;

    if(strcasecmp(rfeat->prog, "tdls") == 0)
    {


    }

    caResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_SET_RFEATURE_RESP_TLV, 4, (BYTE *)caResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

/*
 * wfaStaStartWfdConnection():
 */
int wfaStaStartWfdConnection(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    //caStaStartWfdConn_t *staStartWfdConn= (caStaStartWfdConn_t *)caCmdBuf; //uncomment and use it

    printf("\n Entry wfaStaStartWfdConnection... ");


    // Fetch the GrpId and WFD session and return
    strcpy(&infoResp.cmdru.wfdConnInfo.wfdSessionId[0], "1234567890");
    strcpy(&infoResp.cmdru.wfdConnInfo.p2pGrpId[0], "WIFI_DISPLAY");
    strcpy(&infoResp.cmdru.wfdConnInfo.result[0], "GO");

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_START_WFD_CONNECTION_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}
/*
 * wfaStaCliCommand():
 */

int wfaStaCliCommand(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    char cmdName[32];
    char *pcmdStr=NULL, *str;
    int  st = 1;
    char CmdStr[WFA_CMD_STR_SZ];
//    FILE *wfaCliFd;
//    char wfaCliBuff[64];
//    char retstr[256];
    int CmdReturnFlag =0;
//    char tmp[256];
    //FILE * sh_pipe;
    caStaCliCmdResp_t infoResp;

    printf("\nEntry wfaStaCliCommand; command Received: %s\n",caCmdBuf);
    memcpy(cmdName, strtok_r((char *)caCmdBuf, ",", (char **)&pcmdStr), 32);
    sprintf(CmdStr, "%s",cmdName);

    for(;;)
    {
        // construct CLI standard cmd string
        str = strtok_r(NULL, ",", &pcmdStr);
        if(str == NULL || str[0] == '\0')
            break;
        else
        {
            sprintf(CmdStr, "%s /%s",CmdStr,str);
            str = strtok_r(NULL, ",", &pcmdStr);
            sprintf(CmdStr, "%s %s",CmdStr,str);
        }
    }



#if 0
    // check the return process
    wfaCliFd=fopen("/etc/WfaEndpoint/wfa_cli.txt","r");
    if(wfaCliFd!= NULL)
    {
        while(fgets(wfaCliBuff, 64, wfaCliFd) != NULL)
        {
            //printf("\nLine read from CLI file : %s",wfaCliBuff);
            if(ferror(wfaCliFd))
                break;

            str=strtok(wfaCliBuff,"-");
            if(strcmp(str,cmdName) == 0)
            {
                str=strtok(NULL,",");
                if (str != NULL)
                {
                    if(strcmp(str,"TRUE") == 0)
                        CmdReturnFlag =1;
                }
                else
                    printf("ERR wfa_cli.txt, inside line format not end with , or missing TRUE/FALSE\n");
                break;
            }
        }
        fclose(wfaCliFd);
    }
    else
    {
        printf("/etc/WfaEndpoint/wfa_cli.txt is not exist\n");
        goto cleanup;
    }

    //printf("\n Command Return Flag : %d",CmdReturnFlag);
    memset(&retstr[0],'\0',255);
    memset(&tmp[0],'\0',255);
    sprintf(gCmdStr, "%s",  CmdStr);
    printf("\nCLI Command -- %s\n", gCmdStr);

    sh_pipe = popen(gCmdStr,"r");
    if(!sh_pipe)
    {
        printf ("Error in opening pipe\n");
        goto cleanup;
    }

    sleep(5);
    //tmp_val=getdelim(&retstr,255,"\n",sh_pipe);
    if (fgets(&retstr[0], 255, sh_pipe) == NULL)
    {
        printf("Getting NULL string in popen return\n");
        goto cleanup;
    }
    else
        printf("popen return str=%s\n",retstr);

    sleep(2);
    if(pclose(sh_pipe) == -1)
    {
        printf("Error in closing shell cmd pipe\n");
        goto cleanup;
    }
    sleep(2);

    // find status first in output
    str = strtok_r((char *)retstr, "-", (char **)&pcmdStr);
    if (str != NULL)
    {
        memset(tmp, 0, 10);
        memcpy(tmp, str,  2);
        printf("cli status=%s\n",tmp);
        if(strlen(tmp) > 0)
            st = atoi(tmp);
        else printf("Missing status code\n");
    }
    else
    {
        printf("wfaStaCliCommand no return code found\n");
    }
#endif

    infoResp.resFlag=CmdReturnFlag;

//cleanup:

    switch(st)
    {
    case 0:
        infoResp.status = STATUS_COMPLETE;
        if (CmdReturnFlag)
        {
            if((pcmdStr != NULL) && (strlen(pcmdStr) > 0) )
            {
                memset(&(infoResp.result[0]),'\0',WFA_CLI_CMD_RESP_LEN-1);
                strncpy(&infoResp.result[0], pcmdStr ,(strlen(pcmdStr) < WFA_CLI_CMD_RESP_LEN ) ? strlen(pcmdStr) : (WFA_CLI_CMD_RESP_LEN-2) );
                printf("Return CLI result string to CA=%s\n", &(infoResp.result[0]));
            }
            else
            {
                strcpy(&infoResp.result[0], "No return string found\n");
            }
        }
        break;
    case 1:
        infoResp.status = STATUS_ERROR;
        break;
    case 2:
        infoResp.status = STATUS_INVALID;
        break;
    }

    wfaEncodeTLV(WFA_STA_CLI_CMD_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    printf("Exit from wfaStaCliCommand\n");
    return TRUE;

}
/*
 * wfaStaConnectGoStartWfd():
 */

int wfaStaConnectGoStartWfd(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
//  caStaConnectGoStartWfd_t *staConnecGoStartWfd= (caStaConnectGoStartWfd_t *)caCmdBuf; //uncomment and use it

    printf("\n Entry wfaStaConnectGoStartWfd... ");

    // connect the specified GO and then establish the wfd session

    // Fetch WFD session and return
    strcpy(&infoResp.cmdru.wfdConnInfo.wfdSessionId[0], "1234567890");

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_CONNECT_GO_START_WFD_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}

/*
 * wfaStaGenerateEvent():
 */

int wfaStaGenerateEvent(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    caStaGenEvent_t *staGenerateEvent= (caStaGenEvent_t *)caCmdBuf; //uncomment and use it
    caWfdStaGenEvent_t *wfdGenEvent;

    printf("\n Entry wfaStaGenerateEvent... ");


    // Geneate the specified action and return with complete/error.
    if(staGenerateEvent->program == PROG_TYPE_WFD)
    {
        wfdGenEvent = &staGenerateEvent->wfdEvent;
        if(wfdGenEvent ->type == eUibcGen)
        {
        }
        else if(wfdGenEvent ->type == eUibcHid)
        {
        }
        else if(wfdGenEvent ->type == eFrameSkip)
        {

        }
        else if(wfdGenEvent ->type == eI2cRead)
        {
        }
        else if(wfdGenEvent ->type == eI2cWrite)
        {
        }
        else if(wfdGenEvent ->type == eInputContent)
        {
        }
        else if(wfdGenEvent ->type == eIdrReq)
        {
        }
    }

    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_GENERATE_EVENT_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}




/*
 * wfaStaReinvokeWfdSession():
 */

int wfaStaReinvokeWfdSession(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
//  caStaReinvokeWfdSession_t *staReinvokeSession= (caStaReinvokeWfdSession_t *)caCmdBuf; //uncomment and use it

    printf("\n Entry wfaStaReinvokeWfdSession... ");

    // Reinvoke the WFD session by accepting the p2p invitation   or sending p2p invitation


    infoResp.status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_STA_REINVOKE_WFD_SESSION_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

    return WFA_SUCCESS;
}


int wfaStaGetParameter(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t infoResp;
    caStaGetParameter_t *staGetParam= (caStaGetParameter_t *)caCmdBuf; //uncomment and use it


    caStaGetParameterResp_t *paramList = &infoResp.cmdru.getParamValue;

    printf("\n Entry wfaStaGetParameter... ");

    // Check the program type
    if(staGetParam->program == PROG_TYPE_WFD)
    {
        if(staGetParam->getParamValue == eDiscoveredDevList )
        {
            // Get the discovered devices, make space seperated list and return, check list is not bigger than 128 bytes.
            paramList->getParamType = eDiscoveredDevList;
            strcpy((char *)&paramList->devList, "11:22:33:44:55:66 22:33:44:55:66:77 33:44:55:66:77:88");
        }
    }

	if(staGetParam->program == PROG_TYPE_WFDS)
	{

		if(staGetParam->getParamValue == eDiscoveredDevList )
		{
			// Get the discovered devices, make space seperated list and return, check list is not bigger than 128 bytes.
			paramList->getParamType = eDiscoveredDevList;
			strcpy((char *)&paramList->devList, "11:22:33:44:55:66 22:33:44:55:66:77 33:44:55:66:77:88");

		}
		if(staGetParam->getParamValue == eOpenPorts)
		{
			// Run the port checker tool
			// Get all the open ports and make space seperated list and return, check list is not bigger than 128 bytes.
			paramList->getParamType = eOpenPorts;
			strcpy((char *)&paramList->devList, "22 139 445 68 9700");

		}

	}
	if(staGetParam->program == PROG_TYPE_NAN)
   	{
      if(staGetParam->getParamValue == eMasterPref )
      {
          // Get the master preference of the device and return the value
          paramList->getParamType = eMasterPref;
          strcpy((char *)&paramList->masterPref, "0xff");
      }
    }

	infoResp.status = STATUS_COMPLETE;
	wfaEncodeTLV(WFA_STA_GET_PARAMETER_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
	*respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

   return WFA_SUCCESS;
}


int wfaStaNfcAction(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{

	dutCmdResponse_t infoResp;
	caStaNfcAction_t *getStaNfcAction = (caStaNfcAction_t *)caCmdBuf;  //uncomment and use it

	 printf("\n Entry wfaStaNfcAction... ");

	if(getStaNfcAction->nfcOperation == eNfcHandOver)
	{
		printf("\n NfcAction - HandOver... ");

	}
	else if(getStaNfcAction->nfcOperation == eNfcReadTag)
	{
		printf("\n NfcAction - Read Tag... ");

	}
	else if(getStaNfcAction->nfcOperation == eNfcWriteSelect)
	{
		printf("\n NfcAction - Write Select... ");

	}
	else if(getStaNfcAction->nfcOperation == eNfcWriteConfig)
	{
		printf("\n NfcAction - Write Config... ");

	}
	else if(getStaNfcAction->nfcOperation == eNfcWritePasswd)
	{
		printf("\n NfcAction - Write Password... ");

	}
	else if(getStaNfcAction->nfcOperation == eNfcWpsHandOver)
	{
		printf("\n NfcAction - WPS Handover... ");

	}

	 // Fetch the device mode and put in	 infoResp->cmdru.p2presult
	 //strcpy(infoResp->cmdru.p2presult, "GO");

	 // Fetch the device grp id and put in	 infoResp->cmdru.grpid
	 //strcpy(infoResp->cmdru.grpid, "AA:BB:CC:DD:EE:FF_DIRECT-SSID");

	 strcpy(infoResp.cmdru.staNfcAction.result, "CLIENT");
	 strcpy(infoResp.cmdru.staNfcAction.grpId, "AA:BB:CC:DD:EE:FF_DIRECT-SSID");
	 infoResp.cmdru.staNfcAction.peerRole = 1;




	infoResp.status = STATUS_COMPLETE;
	wfaEncodeTLV(WFA_STA_NFC_ACTION_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
	*respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

   return WFA_SUCCESS;
}

int wfaStaExecAction(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{

	dutCmdResponse_t infoResp;
	caStaExecAction_t *staExecAction = (caStaExecAction_t *)caCmdBuf;  //comment if not used

	 printf("\n Entry wfaStaExecAction... ");

	if(staExecAction->prog == PROG_TYPE_NAN)
	{
		// Perform necessary configurations and actions
		// return the MAC address conditionally as per CAPI specification
	}

	infoResp.status = STATUS_COMPLETE;
	wfaEncodeTLV(WFA_STA_EXEC_ACTION_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
	*respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

   return WFA_SUCCESS;
}

int wfaStaInvokeCommand(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{

	dutCmdResponse_t infoResp;
	caStaInvokeCmd_t *staInvokeCmd = (caStaInvokeCmd_t *)caCmdBuf;  //uncomment and use it

	 printf("\n Entry wfaStaInvokeCommand... ");


	 // based on the command type , invoke API or complete the required procedures
	 // return the  defined parameters based on the command that is received ( example response below)

	if(staInvokeCmd->cmdType == ePrimitiveCmdType && staInvokeCmd->InvokeCmds.primtiveType.PrimType == eCmdPrimTypeAdvt )
	{
		 infoResp.cmdru.staInvokeCmd.invokeCmdRspType = eCmdPrimTypeAdvt;
		 infoResp.cmdru.staInvokeCmd.invokeCmdResp.advRsp.numServInfo = 1;
		 strcpy(infoResp.cmdru.staInvokeCmd.invokeCmdResp.advRsp.servAdvInfo[0].servName,"org.wi-fi.wfds.send.rx");
		 infoResp.cmdru.staInvokeCmd.invokeCmdResp.advRsp.servAdvInfo[0].advtID = 0x0000f;
		 strcpy(infoResp.cmdru.staInvokeCmd.invokeCmdResp.advRsp.servAdvInfo[0].serviceMac,"ab:cd:ef:gh:ij:kl");
	}
	else if (staInvokeCmd->cmdType == ePrimitiveCmdType && staInvokeCmd->InvokeCmds.primtiveType.PrimType == eCmdPrimTypeSeek)
	{
		infoResp.cmdru.staInvokeCmd.invokeCmdRspType = eCmdPrimTypeSeek;
		infoResp.cmdru.staInvokeCmd.invokeCmdResp.seekRsp.searchID = 0x000ff;
	}
	else if (staInvokeCmd->cmdType == ePrimitiveCmdType && staInvokeCmd->InvokeCmds.primtiveType.PrimType == eCmdPrimTypeConnSession)
	{
		infoResp.cmdru.staInvokeCmd.invokeCmdRspType = eCmdPrimTypeConnSession;
		infoResp.cmdru.staInvokeCmd.invokeCmdResp.connSessResp.sessionID = 0x000ff;
		strcpy(infoResp.cmdru.staInvokeCmd.invokeCmdResp.connSessResp.result,"GO");
		strcpy(infoResp.cmdru.staInvokeCmd.invokeCmdResp.connSessResp.grpId,"DIRECT-AB WFADUT");

	}
	infoResp.status = STATUS_COMPLETE;
	wfaEncodeTLV(WFA_STA_INVOKE_CMD_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
	*respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

   return WFA_SUCCESS;
}


int wfaStaManageService(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{

	dutCmdResponse_t infoResp;
	//caStaMngServ_t *staMngServ = (caStaMngServ_t *)caCmdBuf;  //uncomment and use it

	 printf("\n Entry wfaStaManageService... ");

	// based on the manage service type , invoke API's or complete the required procedures
	// return the  defined parameters based on the command that is received ( example response below)
	strcpy(infoResp.cmdru.staManageServ.result, "CLIENT");
	strcpy(infoResp.cmdru.staManageServ.grpId, "AA:BB:CC:DD:EE:FF_DIRECT-SSID");
    infoResp.cmdru.staManageServ.sessionID = 0x000ff;

	infoResp.status = STATUS_COMPLETE;
	wfaEncodeTLV(WFA_STA_MANAGE_SERVICE_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
	*respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

   return WFA_SUCCESS;
}



int wfaStaGetEvents(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{

	dutCmdResponse_t infoResp;
	caStaGetEvents_t *staGetEvents = (caStaGetEvents_t *)caCmdBuf;  //uncomment and use it

	 printf("\n Entry wfaStaGetEvents... ");

	 if(staGetEvents->program == PROG_TYPE_NAN)
	{
		// Get all the events from the Log file or stored events
		// return the  received/recorded event details - eventName, remoteInstanceID, localInstanceID, mac
	}

	// Get all the event from the Log file or stored events
	// return the  received/recorded events as space seperated list   ( example response below)
	strcpy(infoResp.cmdru.staGetEvents.result, "SearchResult SearchTerminated AdvertiseStatus SessionRequest ConnectStatus SessionStatus PortStatus");

	infoResp.status = STATUS_COMPLETE;
	wfaEncodeTLV(WFA_STA_GET_EVENTS_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
	*respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

   return WFA_SUCCESS;
}

int wfaStaGetEventDetails(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{

	dutCmdResponse_t infoResp;
	caStaGetEventDetails_t *getStaGetEventDetails = (caStaGetEventDetails_t *)caCmdBuf;  //uncomment and use it

	 printf("\n Entry wfaStaGetEventDetails... ");


	 // based on the Requested Event type
	 // return the latest corresponding evnet detailed parameters  ( example response below)

	if(getStaGetEventDetails->eventId== eSearchResult )
	{
		// fetch from log file or event history for the search result event and return the parameters
		infoResp.cmdru.staGetEventDetails.eventID= eSearchResult;

		infoResp.cmdru.staGetEventDetails.getEventDetails.searchResult.searchID = 0x00abcd;
		strcpy(infoResp.cmdru.staGetEventDetails.getEventDetails.searchResult.serviceMac,"ab:cd:ef:gh:ij:kl");
		infoResp.cmdru.staGetEventDetails.getEventDetails.searchResult.advID = 0x00dcba;
		strcpy(infoResp.cmdru.staGetEventDetails.getEventDetails.searchResult.serviceName,"org.wi-fi.wfds.send.rx");

		infoResp.cmdru.staGetEventDetails.getEventDetails.searchResult.serviceStatus = eServiceAvilable;
	}
	else if (getStaGetEventDetails->eventId == eSearchTerminated)
	{		// fetch from log file or event history for the search terminated event and return the parameters
		infoResp.cmdru.staGetEventDetails.eventID= eSearchTerminated;
		infoResp.cmdru.staGetEventDetails.getEventDetails.searchTerminated.searchID = 0x00abcd;
	}
	else if (getStaGetEventDetails->eventId == eAdvertiseStatus)
	{// fetch from log file or event history for the Advertise Status event and return the parameters
		infoResp.cmdru.staGetEventDetails.eventID= eAdvertiseStatus;
		infoResp.cmdru.staGetEventDetails.getEventDetails.advStatus.advID = 0x00dcba;

		infoResp.cmdru.staGetEventDetails.getEventDetails.advStatus.status = eAdvertised;
	}
	else if (getStaGetEventDetails->eventId == eSessionRequest)
	{// fetch from log file or event history for the session request event and return the parameters
		infoResp.cmdru.staGetEventDetails.eventID= eSessionRequest;
		infoResp.cmdru.staGetEventDetails.getEventDetails.sessionReq.advID = 0x00dcba;
		strcpy(infoResp.cmdru.staGetEventDetails.getEventDetails.sessionReq.sessionMac,"ab:cd:ef:gh:ij:kl");
		infoResp.cmdru.staGetEventDetails.getEventDetails.sessionReq.sessionID = 0x00baba;
	}
	else if (getStaGetEventDetails->eventId ==eSessionStatus )
	{// fetch from log file or event history for the session status event and return the parameters
		infoResp.cmdru.staGetEventDetails.eventID= eSessionStatus;
		infoResp.cmdru.staGetEventDetails.getEventDetails.sessionStatus.sessionID = 0x00baba;
		strcpy(infoResp.cmdru.staGetEventDetails.getEventDetails.sessionStatus.sessionMac,"ab:cd:ef:gh:ij:kl");
		infoResp.cmdru.staGetEventDetails.getEventDetails.sessionStatus.state = eSessionStateOpen;
	}
	else if (getStaGetEventDetails->eventId == eConnectStatus)
	{
		infoResp.cmdru.staGetEventDetails.eventID= eConnectStatus;
		infoResp.cmdru.staGetEventDetails.getEventDetails.connStatus.sessionID = 0x00baba;
		strcpy(infoResp.cmdru.staGetEventDetails.getEventDetails.connStatus.sessionMac,"ab:cd:ef:gh:ij:kl");
		infoResp.cmdru.staGetEventDetails.getEventDetails.connStatus.status = eGroupFormationComplete;

	}
	else if (getStaGetEventDetails->eventId == ePortStatus)
	{
		infoResp.cmdru.staGetEventDetails.eventID= ePortStatus;
		infoResp.cmdru.staGetEventDetails.getEventDetails.portStatus.sessionID = 0x00baba;
		strcpy(infoResp.cmdru.staGetEventDetails.getEventDetails.portStatus.sessionMac,"ab:cd:ef:gh:ij:kl");
		infoResp.cmdru.staGetEventDetails.getEventDetails.portStatus.port = 1009;
		infoResp.cmdru.staGetEventDetails.getEventDetails.portStatus.status = eLocalPortAllowed;
	}



	infoResp.status = STATUS_COMPLETE;
	wfaEncodeTLV(WFA_STA_GET_EVENT_DETAILS_RESP_TLV, sizeof(infoResp), (BYTE *)&infoResp, respBuf);
	*respLen = WFA_TLV_HDR_LEN + sizeof(infoResp);

   return WFA_SUCCESS;
}




