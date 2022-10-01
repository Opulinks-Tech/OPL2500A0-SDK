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

#include "controller_wifi.h"
#include "lwip/netif.h"
#include "iperf.h"
#include "iperf_cli.h"
#include "lwip/ip4_addr.h"

#include "opulinks_log.h"

static const char *TAG="iperf_cli";

const long kKilo_to_Unit = 1024;
const long kMega_to_Unit = 1024 * 1024;
const long kGiga_to_Unit = 1024 * 1024 * 1024;

const long kkilo_to_Unit = 1000;
const long kmega_to_Unit = 1000 * 1000;
const long kgiga_to_Unit = 1000 * 1000 * 1000;

/* -------------------------------------------------------------------
 * byte_atoi
 *
 * Given a string of form #x where # is a number and x is a format
 * character listed below, this returns the interpreted integer.
 * Gg, Mm, Kk are giga, mega, kilo respectively
 * ------------------------------------------------------------------- */
static int byte_atoi( const char *inString ) {
    double theNum;
    char suffix = '\0';

    if (inString == NULL) {
        return 0;
    }

    /* scan the number and any suffices */
    sscanf( inString, "%lf%c", &theNum, &suffix );

    /* convert according to [Gg Mm Kk] */
    switch ( suffix ) {
        case 'G':  theNum *= kGiga_to_Unit;  break;
        case 'M':  theNum *= kMega_to_Unit;  break;
        case 'K':  theNum *= kKilo_to_Unit;  break;
        case 'g':  theNum *= kgiga_to_Unit;  break;
        case 'm':  theNum *= kmega_to_Unit;  break;
        case 'k':  theNum *= kkilo_to_Unit;  break;
        default: break;
    }
    return theNum;
} /* end byte_atof */

static void cmd_helper(void)
{
    printf("Client/Server:\r\n");
    printf("    -a, --abort              abort iperf program\r\n");
    printf("    -i, --interval  #        seconds between periodic bandwidth reports (default 3 secs)\r\n");
    printf("    -p, --port      #        server port to listen on/connect to (default port 5001)\r\n");
    printf("    -u, --udp                use UDP rather than TCP\r\n");
    printf("    -s, --server             run in server mode\r\n");
    printf("Client specific:\r\n");
    printf("    -c, --client        <host>       run in client mode, connecting to <host>\r\n");
    printf("    -d, --dualtest                   Do a bidirectional test simultaneously\r\n");
    printf("    -n, --num           #[kmgKMG]    number of bytes to transmit (instead of -t)\r\n");
    printf("    -r, --random length #            the data length is random\r\n");
    printf("    -f, --fix length    #            fix the data length by user specify(16-1432)\r\n");
    printf("    -t, --time          #            time in seconds to transmit for (default 30 secs)\r\n");
}

int cmd_iperf(int argc, char** argv)
{
    int i;
    iperf_cfg_t cfg;

    memset(&cfg, 0, sizeof(cfg));

    cfg.sport = IPERF_DEFAULT_PORT;
    cfg.dport = IPERF_DEFAULT_PORT;
    cfg.interval = IPERF_DEFAULT_INTERVAL;
    cfg.time = IPERF_DEFAULT_TIME;
    
    for (i=0; i<argc; i++) {
        if (!strcmp("-a", argv[i])) {
            iperf_stop();
            return 0;
        }
        
        if (!strcmp("-c", argv[i])) {
            cfg.dip = ipaddr_addr(argv[i+1]);
            if (cfg.dip !=0) {
                cfg.flag |= IPERF_FLAG_CLIENT;
            }
            
            struct netif *iface = netif_find("st1");
            cfg.sip = iface->ip_addr.u_addr.ip4.addr;
        }

        if (!strcmp("-s", argv[i])) {
            if ((cfg.flag & IPERF_FLAG_CLIENT)) {
                LOGI(TAG, "wrong input parameter.");
                return -1;
            }
            cfg.flag |= IPERF_FLAG_SERVER;
            struct netif *iface = netif_find("st1");
            cfg.sip = iface->ip_addr.u_addr.ip4.addr;
        }

        if (!strcmp("-d", argv[i])) {
            cfg.flag |= IPERF_FLAG_DUAL;
            struct netif *iface = netif_find("st1");
            cfg.sip = iface->ip_addr.u_addr.ip4.addr;
        }

        if (!strcmp("-u", argv[i])) {
            cfg.flag |= IPERF_FLAG_UDP;
        }

        if (!strcmp("-p", argv[i])) {
            if (cfg.flag & IPERF_FLAG_SERVER) {
                cfg.sport = atoi(argv[i+1]);
                cfg.dport = IPERF_DEFAULT_PORT;
            } else {
                cfg.sport = IPERF_DEFAULT_PORT;
                cfg.dport = atoi(argv[i+1]);
            }
        }

        if (!strcmp("-i", argv[i])) {
            cfg.interval = atoi(argv[i+1]);
            if (cfg.interval <= 0) {
                cfg.interval = IPERF_DEFAULT_INTERVAL;
            }
        }

        if (!strcmp("-t", argv[i])) {
            cfg.time = atoi(argv[i+1]);
            if (cfg.time <= cfg.interval) {
                cfg.time = cfg.interval;
            }
        }

        if (!strcmp("-n", argv[i])) {
            cfg.mAmount = byte_atoi(argv[i+1]);
            cfg.time = 0;
        }

        if (!strcmp("-r", argv[i])) {
            cfg.random = true;
            cfg.fix_len = 0;
        }

        if (!strcmp("-f", argv[i])) {
            cfg.fix_len = atoi(argv[i+1]);
            cfg.random = false;
        }

        if (!strcmp("-h", argv[i])) {
            cmd_helper();
            return 0;
        }
    }
    
    if (!(cfg.flag & IPERF_FLAG_UDP)) {
        cfg.flag |= IPERF_FLAG_TCP;
    }

    printf("\r\n");
    LOGI(TAG, "sip=%d.%d.%d.%d:%d interval=%d time=%d amount=%d",
            cfg.sip&0xFF, (cfg.sip>>8)&0xFF, (cfg.sip>>16)&0xFF, (cfg.sip>>24)&0xFF, cfg.sport,
            cfg.interval, cfg.time, cfg.mAmount);

    iperf_start(&cfg);

    return 0;
}
