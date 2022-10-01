/****************************************************************************
*
* Copyright (c) 2016 Wi-Fi Alliance
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
* SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
* NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
* USE OR PERFORMANCE OF THIS SOFTWARE.
*
*****************************************************************************/


/*
 * For MADWIFI driver, the TOS to 11E queue mapping as:
 *    0x08, 0x20            ----> WME_AC_BK;
 *    0x28, 0xa0            ----> WMC_AC_VI;
 *    0x30, 0xe0 0x88, 0xb8 ----> WME_AC_VO
 *      here 0x88 for UPSD, will be implemented later
 *    all other/default     ----> WME_AC_BE;
 */

#include "wfa_portall.h"
#include "wfa_stdincs.h"
#include "wfa_types.h"
#include "wfa_main.h"
#include "wfa_tg.h"
#include "wfa_debug.h"
#include "wfa_tlv.h"
#include "wfa_sock.h"
#include "wfa_rsp.h"
#include "wfa_wmmps.h"
#include "wfa_miscs.h"


/*
 * external global thread sync variables
 */
extern tgWMM_t wmm_thr[WFA_THREADS_NUM];
extern int resetsnd;
extern int resetrcv;
extern int newCmdOn;

extern tgStream_t *findStreamProfile(int id);
extern int gxcSockfd;
int vend;
extern int wfaSetProcPriority(int);

typedef struct {
    size_t head;
    size_t tail;
    size_t size;
    tgStream_t *pStream;
} wfa_thr_streams_queue_t;
wfa_thr_streams_queue_t g_wfa_thr_stream_q;

tgStream_t gStreams[WFA_MAX_TRAFFIC_STREAMS];

extern unsigned short wfa_defined_debug;
extern unsigned int recvThr;
extern int tgWMMTestEnable;
extern dutCmdResponse_t gGenericResp;

int num_stops=0;
int num_hello=0;

extern StationProcStatetbl_t stationProcStatetbl[LAST_TEST+1][11];

int nsent;

int runLoop = 0;
int usedThread=0;

int sentTranPkts=0;
BYTE *trafficBuf=NULL;

int sendThrId = 0;

int wfa_thr_stream_default(void)
{
    for (int i=0; i<WFA_MAX_TRAFFIC_STREAMS; i++) {
        if (gStreams[i].sockfd != -1) {
            wSHUTDOWN(gStreams->sockfd, SHUT_WR);
            wCLOSE(gStreams->sockfd);
        }
        wMEMSET(&gStreams[i], 0, sizeof(tgStream_t));
        gStreams[i].sockfd = -1;
    }
    return 0;
}

int wfa_thr_stream_queue_init(void)
{
    g_wfa_thr_stream_q.head = 0;
    g_wfa_thr_stream_q.tail = 0;
    g_wfa_thr_stream_q.size = WFA_MAX_TRAFFIC_STREAMS;
    g_wfa_thr_stream_q.pStream = &gStreams[0];
    return 0;
}

int wfa_thr_stream_queue_is_full(void)
{
    return g_wfa_thr_stream_q.head - g_wfa_thr_stream_q.tail >= g_wfa_thr_stream_q.size;
}

void *wfa_thr_stream_queue_get(void)
{
    tgStream_t *pStream = &g_wfa_thr_stream_q.pStream[g_wfa_thr_stream_q.head & (g_wfa_thr_stream_q.size - 1)];
    g_wfa_thr_stream_q.head++;
    return pStream;
}

int wfa_thr_stream_queue_delete(void)
{
    tgStream_t *pStream;
    
    pStream = &g_wfa_thr_stream_q.pStream[g_wfa_thr_stream_q.tail & (g_wfa_thr_stream_q.size - 1)];
    if (pStream == NULL)
        wEXIT(1);

    if (pStream->sockfd != -1) {
        wSHUTDOWN(pStream->sockfd, SHUT_WR);
        wCLOSE(pStream->sockfd);
        wMEMSET(pStream, 0, sizeof(tgStream_t));
        pStream->sockfd = -1;
    }

    g_wfa_thr_stream_q.tail++;
    return 0;
}

/* this is to stop sending packets by timer       */
void wfa_thr_freertos_tmr_out(void const *arg)
{
    struct timeval af;

    gettimeofday(&af,0);
    DPRINT_INFO(WFA_OUT, "timer fired, stop sending traffic, Exiting at sec %d usec %d\n", (int )af.tv_sec, (int)af.tv_usec);

    /*
     *  After runLoop reset, all sendLong will stop
     */
    runLoop = 0;

    /*
     * all WMM streams also stop
     */
    for(int i=0; i<WFA_THREADS_NUM; i++)
    {
        wmm_thr[i].thr_flag      = 0;
        wmm_thr[i].stop_flag     = 0;
        wmm_thr[i].assigned_flag = 0;
    }
}

/*
 * wfaTGSetPrio(): This depends on the network interface card.
 *               So you might want to remap according to the driver
 *               provided.
 *               The current implementation is to set the TOS/DSCP bits
 *               in the IP header
 */
int wfaTGSetPrio(int sockfd, int tgUserPriority)
{
    int tosval;

    socklen_t size = sizeof(tosval);
    wGETSOFD(sockfd, IPPROTO_IP, IP_TOS, &tosval, &size);

    switch(tgUserPriority)
    {
    case TG_WMM_AC_BK:   // user priority "1"
        /*Change this value to the ported device*/
        tosval = TOS_BK;
        break;

    case TG_WMM_AC_VI:   // user priority "5"
        /*Change this value to the ported device*/
        tosval = TOS_VI;
        break;

    case TG_WMM_AC_UAPSD:
        tosval = 0x88;
        break;

    case TG_WMM_AC_VO:   // user priority "6"
        /*Change this value to the ported device*/
        tosval = TOS_VO;
        break;

    case TG_WMM_AC_BE:   // user priority "0"
        tosval = TOS_BE;
        break;

    /* For WMM-AC Program User Priority Defintions */
    case TG_WMM_AC_UP0:
        tosval = 0x00;
        break;

    case TG_WMM_AC_UP1:
        tosval = 0x20;
        break;

    case TG_WMM_AC_UP2:
        tosval = 0x40;
        break;

    case TG_WMM_AC_UP3:
        tosval = 0x60;
        break;

    case TG_WMM_AC_UP4:
        tosval = 0x80;
        break;
    case TG_WMM_AC_UP5:
        tosval = 0xa0;
        break;

    case TG_WMM_AC_UP6:
        tosval = 0xc0;
        break;

    case TG_WMM_AC_UP7:
        tosval = 0xe0;
        break;

    default:
        tosval = 0x00;
        /* default */
        ;
    }

    if ( sockfd > 0)
    {
        if(wSETSOCKOPT ( sockfd, IPPROTO_IP, IP_TOS, &tosval, sizeof(tosval)) != 0)
        {
            DPRINT_ERR(WFA_ERR, "wfaTGSetPrio: Failed to set IP_TOS\n");
        }
    }
    else
    {
        DPRINT_INFO(WFA_OUT, "wfaTGSetPrio::socket closed\n");
    }
    return (tosval == 0xE0)?0xD8:tosval;
}

/*
 * wfaSetThreadPrio():
 *    Set thread priorities
 *    It is an optional experiment if you decide not necessary.
 */
void wfaSetThreadPrio(int tid, int userPriority)
{
//    struct sched_param tschedParam;
//    pthread_attr_t tattr;

//    wPT_ATTR_INIT(&tattr);
//    //wPT_ATTR_SETSCH(&tattr, SCHED_RR);

//    switch(userPriority)
//    {
//    case TG_WMM_AC_BK:
//        tschedParam.sched_priority = -1;
//        break;
//    case TG_WMM_AC_VI:
//        tschedParam.sched_priority = 19-1;
//        break;
//    case TG_WMM_AC_VO:
//        tschedParam.sched_priority = 19;
//        break;
//    case TG_WMM_AC_BE:
//        tschedParam.sched_priority = 0;
//    default:
//        /* default */
//        ;
//    }

//    wPT_ATTR_SETSCHPARAM(&tattr, &tschedParam);
}

/*
 * collects the traffic statistics from other threads and
 * sends the collected information to CA
 */
void  wfaSentStatsResp(int sock)
{
    int i, total=0, outputLen;
    char *poutputBuf;

    tgStream_t       *pStreams;
//    dutCmdResponse_t *prespBuff;
    dutCmdResponse_t *ptr;

    pStreams  = gStreams;
    for(i = 0; i < WFA_MAX_TRAFFIC_STREAMS; i++) {
        if((pStreams->id != 0) && (pStreams->profile.direction == DIRECT_SEND) && (pStreams->state == WFA_STREAM_ACTIVE)) {
            total++;
        }
        pStreams++;
    }

    poutputBuf = malloc(total * sizeof(dutCmdResponse_t) + WFA_TLV_HDR_LEN);
    if (poutputBuf == NULL)
        wEXIT(1);
    wMEMSET(poutputBuf, 0, total * sizeof(dutCmdResponse_t) + WFA_TLV_HDR_LEN);

//    prespBuff = malloc(total * sizeof(dutCmdResponse_t));
//    if (prespBuff == NULL)
//        wEXIT(1);
//    wMEMSET(prespBuff, 0, total * sizeof(dutCmdResponse_t));

    ptr = (dutCmdResponse_t *)&poutputBuf[4];
    pStreams  = gStreams;
    for(i = 0; i < WFA_MAX_TRAFFIC_STREAMS; i++)
    {
        if((pStreams->id != 0) && (pStreams->profile.direction == DIRECT_SEND) && (pStreams->state == WFA_STREAM_ACTIVE))
        {
            ptr->status = STATUS_COMPLETE;
            ptr->streamId = pStreams->id;

            wMEMCPY(&ptr->cmdru.stats, &pStreams->stats, sizeof(tgStats_t));

            pStreams->state = WFA_STREAM_INACTIVE;

            ptr++;
        }
        pStreams++;
    }

//    printf("txFrames %u rxFrames %u txPayloadBytes %llu rxPayloadBytes %llu\n", 
//           first->cmdru.stats.txFrames,
//           first->cmdru.stats.rxFrames,
//           first->cmdru.stats.txPayloadBytes,
//           first->cmdru.stats.rxPayloadBytes);

    uint16_t *ptlv_header;
    ptlv_header = (uint16_t *)&poutputBuf[0];
    ((wfaTLV *)ptlv_header)->tag = WFA_TRAFFIC_AGENT_SEND_RESP_TLV;
    ((wfaTLV *)ptlv_header)->len = total * sizeof(dutCmdResponse_t);

//    wfaEncodeTLV(WFA_TRAFFIC_AGENT_SEND_RESP_TLV, total * sizeof(dutCmdResponse_t), (BYTE *)prespBuff, (BYTE *)poutputBuf);

    outputLen = WFA_TLV_HDR_LEN + total * sizeof(dutCmdResponse_t);
    printf("stats outputLen %u\n", outputLen);

#if 0
    for(i = 0; i< pkLen; i++)
        printf("%x ", buff[i]);

    printf("\n");
#endif

    if(wfaCtrlSend(sock, (BYTE *)poutputBuf, outputLen) != outputLen)
    {
        DPRINT_WARNING(WFA_WNG, "wfaCtrlSend Error\n");
    }

    free(poutputBuf);
//    free(prespBuff);
    poutputBuf = NULL;
//    prespBuff  = NULL;

    return;
}

static int wfa_thr_init_sock(int myId, tgProfile_t *myProfile)
{
    int mySock = -1;

    mySock = wfaCreateUDPSock(myProfile->sipaddr, myProfile->sport);
    if (mySock < 0) {
        DPRINT_ERR(WFA_OUT, "wfa_wmm_thread %u SEND ERROR failed create UDP socket! \n", myId);
        return -1;
    }

    mySock = wfaConnectUDPPeer(mySock, myProfile->dipaddr, myProfile->dport);

    return mySock;
}

static int wfa_thr_set_timeout(int myId, tgWMM_t *my_wmm, tgProfile_t *myProfile)
{
    if(myProfile->maxcnt == 0)
    {
        osTimerStart(my_wmm->freertos_tmr, myProfile->duration * 1000);
        printf("wfa_wmm_thread %u SEND set stop alarm for %d sec \n", myId, myProfile->duration);
    }
    return 0;
}

static int wfa_thr_do_sending(int myId, tgWMM_t *my_wmm, tgProfile_t *myProfile, int myStreamId, int mySock, int respLen)
{
    int iOptVal, iOptLen;
    dutCmdResponse_t *respBuf;

    respBuf = malloc(sizeof(dutCmdResponse_t));
    if (respBuf == NULL)
        wEXIT(1);

    wMEMSET(respBuf, 0, sizeof(dutCmdResponse_t));

    if (myProfile->profile == PROF_FILE_TX) {
        getsockopt(mySock, SOL_SOCKET, SO_SNDBUF, (char *)&iOptVal, (socklen_t *)&iOptLen);
        iOptVal = iOptVal * 16;
        setsockopt(mySock, SOL_SOCKET, SO_SNDBUF, (char *)&iOptVal, (socklen_t )iOptLen);

        if ( (myProfile->rate != 0 ) /* WFA_SEND_FIX_BITRATE_MAX_FRAME_RATE)*/ &&
           (myProfile->pksize * myProfile->rate * 8 < WFA_SEND_FIX_BITRATE_MAX) &&
           (myProfile->trafficClass != TG_WMM_AC_VO)  )
            wfaSendBitrateData(mySock, myStreamId, (uint8_t *)respBuf, &respLen);
        else
        {
            wfaSendLongFile(mySock, myStreamId, (uint8_t *)respBuf, &respLen);
        }

        if(mySock != -1)
        {
            wCLOSE(mySock);
            mySock = -1;
        }
    }

    wFREE(respBuf);
    respBuf = NULL;

    return 0;
}

static int wfa_thr_do_receiving(int myId, tgWMM_t *my_wmm, tgProfile_t *myProfile, int myStreamId, int mySock, int respLen)
{
    int iOptVal, iOptLen;
    int nbytes = 0;
    char *precvBuf = NULL;

    struct timeval tmout;

    precvBuf = malloc(MAX_RCV_BUF_LEN+1);
    if (precvBuf == NULL)
        wEXIT(1);

    wMEMSET(precvBuf, 0, MAX_RCV_BUF_LEN+1);

    tgStream_t *myStream = findStreamProfile(myStreamId);

    if (myProfile->profile == PROF_IPTV) {

        getsockopt(myStream->sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&iOptVal, (socklen_t *)&iOptLen);
        iOptVal = iOptVal * 10;
        setsockopt(myStream->sockfd, SOL_SOCKET, SO_RCVBUF, (char *)&iOptVal, (socklen_t )iOptLen);

        /* set timeout for blocking receive */
        tmout.tv_sec = 0;
        tmout.tv_usec = 200000;   /* set the receive time out to 200 ms */
        setsockopt(myStream->sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tmout, (socklen_t) sizeof(tmout));

        for(;;)
        {
            nbytes = wfaRecvFile(myStream->sockfd, myStreamId, (char *)precvBuf);
            if(nbytes <= 0)
            {
                /* due to timeout */
                if(myStream->sockfd >= 0 )
                    continue;

                break;
            }
        }

        my_wmm->thr_flag = 0;
    }

    free(precvBuf);
    precvBuf = NULL;

    return 0;
}

void wfa_wmm_thread_freertos(void *thr_param)
{
    int myId = ((tgThrData_t *)thr_param)->tid;
    int myStreamId;
    int respLen = 0;
    tgStream_t  *myStream = NULL;
    tgProfile_t *myProfile;

    tgWMM_t *my_wmm = &wmm_thr[myId];

    osSemaphoreWait(my_wmm->semaphore, osWaitForever);

    DPRINT_INFO(WFA_OUT, "wfa_wmm_thread %u ready\n", myId);

    while (1) {
        DPRINT_INFO(WFA_OUT, "wfa_wmm_thread %u wait semaphore\n", myId);
        osSemaphoreWait(my_wmm->semaphore, osWaitForever);
        DPRINT_INFO(WFA_DUT, "wfa_wmm_thread %u running\n", myId);

        myStreamId = my_wmm->thr_flag;

        myStream = findStreamProfile(myStreamId);
        myProfile = &myStream->profile;
        if(myProfile == NULL || myStreamId == 0) {
            DPRINT_WARNING(WFA_OUT, "wfa_wmm_thread %u myProfile == NULL || myStreamId == 0\n", myId);
            continue;
        }

        DPRINT_INFO(WFA_OUT, "wfa_wmm_thread %u myStreamId %u myStream %p myProfile %p\n", myId, myStreamId, myStream, myProfile);

        sendThrId = myId;

        switch(myProfile->direction) {
            case DIRECT_SEND:
                myStream->sockfd = wfa_thr_init_sock(myId, myProfile);
                wfa_thr_set_timeout(myId, my_wmm, myProfile);
                wfa_thr_do_sending(myId, my_wmm, myProfile, myStreamId, myStream->sockfd, respLen);

                wSLEEP(1);

                if(myId == sendThrId)
                {
                    DPRINT_INFO(WFA_OUT, "wfa_wmm_thread %u ready report stats...\n", myId);
                    wfaSentStatsResp(gxcSockfd);
                    DPRINT_INFO(WFA_OUT, "wfa_wmm_thread %u done stats ...\n", myId);
                    sendThrId = 0;
                }

                break;
            case DIRECT_RECV:
                myStream->sockfd = wfaCreateUDPSock(myProfile->dipaddr, myProfile->dport);
                wfa_thr_do_receiving(myId, my_wmm, myProfile, myStreamId, myStream->sockfd, respLen);
                DPRINT_INFO(WFA_OUT, "wfa_wmm_thread %u Break out doing...\n", myId);
                break;
            default:
                break;
        }

        my_wmm->thr_flag = 0;
        my_wmm->assigned_flag = false;
    }
}

void * wfa_wmm_thread(void *thr_param)
{
    return NULL;
}
