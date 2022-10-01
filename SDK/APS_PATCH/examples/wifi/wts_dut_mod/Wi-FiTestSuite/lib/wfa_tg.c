
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
 *    File: wfa_tg.c
 *    Library functions for traffic generator.
 *    They are shared with both TC and DUT agent.
 */
//#include <sys/time.h>
//#include <time.h>
//#include <signal.h>
//#include <fcntl.h>
//#include <netdb.h>
//#include <pthread.h>
#include <math.h>

#include "wfa_portall.h"
#include "wfa_stdincs.h"
#include "wfa_debug.h"
#include "wfa_ver.h"
#include "wfa_main.h"
#include "wfa_tlv.h"
#include "wfa_tg.h"
#include "wfa_cmds.h"
#include "wfa_sock.h"
#include "wfa_rsp.h"
#include "wfa_wmmps.h"
#include "wfa_miscs.h"

#include "wfa_thr.h"

extern int gRegSec;

int adj_latency;

#define system(a) dd

extern int wfaTrafficSendTo(int, char *, int, struct sockaddr *);
extern int wfaTrafficRecv(int, char *, struct sockaddr *);
extern void wfaSendPing(tgPingStart_t *staPing, float *interval, int streamid);
extern int wfaStopPing(dutCmdResponse_t *stpResp, int streamid);
extern unsigned short wfa_defined_debug;

extern tgWMM_t wmm_thr[];

extern double min_rttime;
extern double gtgPktRTDelay;
extern void int2BuffBigEndian(int val, char *buf);
extern int bigEndianBuff2Int(char *buff);

static int streamId = 0;
static int sentTranPkts = 0;

extern int usedThread;
extern int runLoop;
extern int sendThrId;

extern dutCmdResponse_t gGenericResp;

static int  tableDscpToTos[15] [2] = {
{0,0},{8,32},{10,40},{14,56},{18,72},{22,88},{24,96},{28,112},{30,120},{34,136},{36,144},{38,152},{40,160},{46,184},{48,192}};


/* Some devices may only support UDP ECHO and do not have ICMP level ping */
// #define WFA_PING_UDP_ECHO_ONLY     1

/*
 * findStreamProfile(): search existing stream profile by stream id
 * input: id - stream id;
 * return: matched stream profile
 */
tgStream_t *findStreamProfile(int id)
{
    int i;
    tgStream_t *myStream = gStreams;

    for(i = 0; i< WFA_MAX_TRAFFIC_STREAMS; i++)
    {
        if(myStream->id == id)
            return myStream;

        myStream++;
    }

    return NULL;
}

tgProfile_t *findTGProfile(int streamId)
{
    volatile int i;
    tgStream_t *myStream = gStreams;

    for(i = 0; i< WFA_MAX_TRAFFIC_STREAMS; i++)
    {
       if(myStream->id == streamId)
          return &(myStream->profile);

       myStream++;
    }

    return NULL;
}


int convertDscpToTos(int dscp)// return >=0 as TOS, otherwise error.
{
    int i =0;

    for (i=0; i< WFA_DSCP_TABLE_SIZE; i++)
    {
        if ( tableDscpToTos[i][0] == dscp)
            return tableDscpToTos[i][1];
    }
    return  -1;
}

static int wfa_gc_assign_job_to_thread(int streamid)
{
    for (int i = 0; i<WFA_THREADS_NUM; i++) {
        if (wmm_thr[i].assigned_flag == false) {
            wmm_thr[i].thr_flag = streamid;
            wmm_thr[i].assigned_flag = true;
            return i;
        }
    }

    return -1;
}

/*
 * wfaTGSendPing(): Instruct Traffic Generator to send ping packets
 * destination    : IP address to send the pings.
 * frameSize      : Frame Size in bytes.
 * frameRate      : Number of pings to send each second.
 * duration       : Number of seconds to ping. 0 = continuous.
 */
int wfaTGSendPing(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    float interval;      /* it could be subseconds/100s minisecond */
    tgPingStart_t *staPing = (tgPingStart_t *)caCmdBuf;
    dutCmdResponse_t *spresp = &gGenericResp;

    spresp->status = STATUS_COMPLETE;

    DPRINT_INFO(WFA_OUT, "Entering wfaTGSendPing ...\n");

    if (staPing->frameSize == 0)
        staPing->frameSize = 64;

    if (staPing->frameRate == 0)
        staPing->frameRate = 1;

    interval = (float) 1/staPing->frameRate;

    if (staPing->duration == 0)
        staPing->duration = 86400;

    if (staPing->iptype == 1) {
        DPRINT_INFO(WFA_OUT, "    iptype is IPv4\n");
    }
    else if (staPing->iptype == 2) {
        DPRINT_WARNING(WFA_OUT, "    IPv6 Not supported yet\n");
        spresp->status = STATUS_ERROR;
        goto done;
    }
    else {
        DPRINT_INFO(WFA_OUT, "    default iptype is IPv4\n");
    }

    spresp->streamId = ++streamId;

    DPRINT_INFO(WFA_OUT, "    framesize %u framerate %f interval %f streamID %d duration %d\n",
                staPing->frameSize, staPing->frameRate, interval, streamId,staPing->duration);

    wfaSendPing(staPing, &interval, streamId);

done:
    wfaEncodeTLV(WFA_TRAFFIC_SEND_PING_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)spresp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * tgStopPing(): Instruct Traffic Generator to stop ping packets
 *
 */
int wfaTGStopPing(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    int streamid = *(int *)(caCmdBuf);
    dutCmdResponse_t *stpResp = &gGenericResp;

    stpResp->status = STATUS_COMPLETE;

    DPRINT_INFO(WFA_OUT, "Entering wfaTGStopPing ...\n");

    DPRINT_INFO(WFA_OUT, "    the stream id is %d\n", streamid);

    wfaStopPing(stpResp, streamid);

    wfaEncodeTLV(WFA_TRAFFIC_STOP_PING_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)stpResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaTGConfig: store the traffic profile setting that will be used to
 *           instruct traffic generation.
 * input: cmd -- not used
 * response: send success back to controller
 * return: success or fail
 * Note: the profile storage is a global space.
 */
int wfaTGConfig(int len, BYTE *caCmdBuf, int *respLen, BYTE *respBuf)
{
    int ret = WFA_FAILURE;
    tgStream_t *myStream = NULL;
    dutCmdResponse_t *confResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaTGConfig ...\n");

    if (wfa_thr_stream_queue_is_full() == true) {
        wfa_thr_stream_queue_delete();
        myStream = wfa_thr_stream_queue_get();
    }
    else {
        myStream = wfa_thr_stream_queue_get();
    }

    if (myStream == NULL)
        wEXIT(1);

    wMEMCPY(&myStream->profile, caCmdBuf, sizeof(tgProfile_t));

    myStream->id = ++streamId; /* the id start from 1 */

    DPRINT_INFO(WFA_OUT, "    streamId %u\n", streamId);
    DPRINT_INFO(WFA_OUT, "    profile %i direction %i dest ip %s dport %i source %s sport %i\n", 
                myStream->profile.profile, myStream->profile.direction, myStream->profile.dipaddr, 
                myStream->profile.dport, myStream->profile.sipaddr, myStream->profile.sport);
    DPRINT_INFO(WFA_OUT, "    rate %i duration %i size %i class %i delay %i\n",
                myStream->profile.rate, myStream->profile.duration, myStream->profile.pksize, myStream->profile.trafficClass, 
                myStream->profile.startdelay);

    confResp->status = STATUS_COMPLETE;
    confResp->streamId = myStream->id;
    wfaEncodeTLV(WFA_TRAFFIC_AGENT_CONFIG_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)confResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return ret;
}

/* RecvStart: instruct traffic generator to start receiving
 *                 based on a profile
 * input:      cmd -- not used
 * response:   inform controller for "running"
 * return:     success or failed
 */
int wfaTGRecvStart(int len, BYTE *parms, int *respLen, BYTE *respBuf)
{
    int status = STATUS_COMPLETE, i;
    int numStreams = len/4;
    int streamid;
    int ret;
    tgProfile_t *theProfile;
    tgStream_t *myStream;

    DPRINT_INFO(WFA_OUT, "Entering tgRecvStart ...\n");

    /*
     * The function wfaSetProcPriority called here is to enhance the real-time
     * performance for packet receiving. It is only for tuning and optional
     * to implement
     */

    for(i=0; i<numStreams; i++)
    {
        wMEMCPY(&streamid, parms+(4*i), 4); /* changed from 2 to 4, bug reported by n.ojanen */
        myStream = findStreamProfile(streamid);
        if(myStream == NULL)
        {
            DPRINT_ERR(WFA_OUT, "    myStream == NULL\n");
            status = STATUS_INVALID;
            goto fail;
        }

        theProfile = &myStream->profile;
        if(theProfile == NULL)
        {
            DPRINT_ERR(WFA_OUT, "    theProfile == NULL\n");
            status = STATUS_INVALID;
            goto fail;
        }

        /* calculate the frame interval which is used to derive its jitter */
        if(theProfile->rate != 0 && theProfile->rate < 5000)
            myStream->fmInterval = 1000000/theProfile->rate; /* in ms */
        else
            myStream->fmInterval = 0;

        if(theProfile->direction != DIRECT_RECV)
        {
            DPRINT_ERR(WFA_OUT, "    theProfile->direction != DIRECT_RECV\n");
            status = STATUS_INVALID;
            goto fail;
        }

        wMEMSET(&myStream->stats, 0, sizeof(tgStats_t));

        // mark the stream active
        myStream->state = WFA_STREAM_ACTIVE;

        switch(theProfile->profile)
        {
            case PROF_TRANSC:
            case PROF_CALI_RTD:  /* Calibrate roundtrip delay */
            case PROF_MCAST:
            case PROF_FILE_TX:
            case PROF_IPTV:
                ret = wfa_gc_assign_job_to_thread(streamid);
                if (ret < WFA_SUCCESS) {
                    DPRINT_ERR(WFA_OUT, "    assign job failed\n");
                    goto fail;
                }

                DPRINT_INFO(WFA_OUT, "Recv Start in thread %i for streamid %i\n", ret, streamid);

                wMEMSET(&myStream->stats, 0, sizeof(tgStats_t));
                osSemaphoreRelease(wmm_thr[ret].semaphore);
                break;

            case PROF_UAPSD:
                DPRINT_WARNING(WFA_OUT, "PROF_UAPSD Not supported yet\n");
                break;
            default:
                break;
        }
    }

fail:
    /* encode a TLV for response for "complete/error ..." */
    wfaEncodeTLV(WFA_TRAFFIC_AGENT_RECV_START_RESP_TLV, sizeof(int), (BYTE *)&status, respBuf);
    *respLen = WFA_TLV_HDR_LEN + sizeof(int);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * tgRecvStop: instruct traffic generator to stop receiving based on a profile
 * input:      cmd -- not used
 * response:   inform controller for "complete"
 * return:     success or failed
 */
int wfaTGRecvStop(int len, BYTE *parms, int *respLen, BYTE *respBuf)
{
    int status = STATUS_COMPLETE, i;
    int numStreams = len/4;
    unsigned int streamid;
    tgProfile_t *theProfile;
    tgStream_t *myStream = NULL;
    dutCmdResponse_t *statResp = &gGenericResp;

    int id_cnt = 0;

    DPRINT_INFO(WFA_OUT, "Entering tgRecvStop ...\n");

    /* in case that send-stream not done yet, an optional delay */
//    while(sendThrId != 0)
//        sleep(1);

    /*
     * After finishing the receiving command, it should lower itself back to
     * normal level. It is optional implementation if it is not called
     * while it starts receiving for raising priority level.
     */
    for(i=0; i<numStreams; i++)
    {
        wMEMCPY(&streamid, parms+(4*i), 4);
        DPRINT_INFO(WFA_OUT, "    stop stream id %i\n", streamid);
        myStream = findStreamProfile(streamid);
        if(myStream == NULL)
        {
            status = STATUS_INVALID;
            wfaEncodeTLV(WFA_TRAFFIC_AGENT_RECV_STOP_RESP_TLV, 4, (BYTE *)&status, respBuf);
            *respLen = WFA_TLV_HDR_LEN + 4;
            DPRINT_INFO(WFA_OUT, "    stream table empty\n");
            continue;
        }

        theProfile = &myStream->profile;
        if(theProfile == NULL)
        {
            status = STATUS_INVALID;
            wfaEncodeTLV(WFA_TRAFFIC_AGENT_RECV_STOP_RESP_TLV, 4, (BYTE *)&status, respBuf);
            *respLen = WFA_TLV_HDR_LEN + 4;

            return WFA_SUCCESS;
        }

        if(theProfile->direction != DIRECT_RECV)
        {
            status = STATUS_INVALID;
            wfaEncodeTLV(WFA_TRAFFIC_AGENT_RECV_STOP_RESP_TLV, 4, (BYTE *)&status, respBuf);
            *respLen = WFA_TLV_HDR_LEN + 4;

            return WFA_SUCCESS;
        }

        /* reset its flags , close sockets */
        switch(theProfile->profile)
        {
            case PROF_TRANSC:
            case PROF_CALI_RTD:
            case PROF_MCAST:
            case PROF_FILE_TX:
            case PROF_IPTV:
                if(myStream->sockfd != -1)
                {
                    wSHUTDOWN(myStream->sockfd, SHUT_WR);
                    wCLOSE(myStream->sockfd);
                    myStream->sockfd = -1;
                }
                break;
            case PROF_UAPSD:
                break;
        }

        /* encode a TLV for response for "complete/error ..." */
        statResp->status = STATUS_COMPLETE;
        statResp->streamId = streamid;

        DPRINT_INFO(WFA_OUT, "    stream Id %u rx %u total %llu\n", 
                    streamid, myStream->stats.rxFrames, myStream->stats.rxPayloadBytes);

        wMEMCPY(&statResp->cmdru.stats, &myStream->stats, sizeof(tgStats_t));
        id_cnt++;

        // Not empty it but require to reset the entire table before test starts.
        //wMEMSET(myStream, 0, sizeof(tgStream_t));
    }

    // mark the stream inactive
    myStream->state = WFA_STREAM_INACTIVE;

    usedThread = 0; //Vincent Tai, TODO

    wfaEncodeTLV(WFA_TRAFFIC_AGENT_RECV_STOP_RESP_TLV, id_cnt * sizeof(dutCmdResponse_t), (uint8_t *)statResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + numStreams * sizeof(dutCmdResponse_t);

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * wfaTGSendStart: instruct traffic generator to start sending based on a profile
 * input:      cmd -- not used
 * response:   inform controller for "running"
 * return:     success or failed
 */
int wfaTGSendStart(int len, BYTE *parms, int *respLen, BYTE *respBuf)
{
    int i=0, streamid = 0;
    int numStreams = len/4;
    int ret;

    tgProfile_t *theProfile;
    tgStream_t  *myStream;

    dutCmdResponse_t *staSendResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering tgSendStart ...\n");

    for(i=0; i<numStreams; i++)
    {
        wMEMCPY(&streamid, parms+(4*i), 4);
        DPRINT_INFO(WFA_OUT, "    start stream id %i\n", streamid);

        myStream = findStreamProfile(streamid);
        if(myStream == NULL)
        {
            goto fail;
        }

        if(myStream->state == WFA_STREAM_ACTIVE) {
            goto fail;
        }

        theProfile = &myStream->profile;
        if(theProfile == NULL)
        {
            DPRINT_ERR(WFA_OUT, "    profile is not found\n");
            goto fail;
        }

        if(theProfile->direction != DIRECT_SEND)
        {
            DPRINT_ERR(WFA_OUT, "    dirction is not send\n");
            goto fail;
        }

        switch(theProfile->profile)
        {
            case PROF_FILE_TX:
            case PROF_IPTV:
                ret = wfa_gc_assign_job_to_thread(streamid);
                if (ret < WFA_SUCCESS) {
                    goto fail;
                }

                DPRINT_INFO(WFA_OUT, "    Send Start in thread %i for streamid %i\n", ret, streamid);

                wMEMSET(&myStream->stats, 0, sizeof(tgStats_t));
                myStream->state = WFA_STREAM_ACTIVE;
                osSemaphoreRelease(wmm_thr[ret].semaphore);
                break;
            case PROF_MCAST:
                printf("PROF_MCAST Not supported yet\n");
                break;
            case PROF_TRANSC:
                printf("PROF_TRANSC Not supported yet\n");
                break;
            case PROF_CALI_RTD:
                printf("PROF_CALI_RTD Not supported yet\n");
                break;
            case PROF_UAPSD:
                printf("PROF_UAPSD Not supported yet\n");
            break;
        } /* switch  */
    }/*  for */

    *respLen = 0;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;

fail:
    staSendResp->status = STATUS_INVALID;
    wfaEncodeTLV(WFA_TRAFFIC_AGENT_SEND_RESP_TLV, 4, (BYTE *)&staSendResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    return WFA_SUCCESS;
}

int wfaTGReset(int len, BYTE *parms, int *respLen, BYTE *respBuf)
{
    dutCmdResponse_t *resetResp = &gGenericResp;

    DPRINT_INFO(WFA_OUT, "Entering wfaTGReset ...\n");

    wfa_thr_stream_default();

    runLoop = 0;

    usedThread = 0;

    streamId = 0;

    /* encode a TLV for response for "complete ..." */
    resetResp->status = STATUS_COMPLETE;
    wfaEncodeTLV(WFA_TRAFFIC_AGENT_RESET_RESP_TLV, 4, (BYTE *)resetResp, respBuf);
    *respLen = WFA_TLV_HDR_LEN + 4;

    DPRINT_INFO(WFA_OUT, "Completing ...\n");

    return WFA_SUCCESS;
}

/*
 * calculate the sleep time for different frame rate
 * It should be done according the device
 * This is just one way to adjust the packet delivery speed. If you find
 * you device does not meet the test requirements, you MUST re-adjust
 * the method.
 */

/* The HZ value could be found in the build header file */
/* 100 -> 10ms, 1000 -> 1ms , etc                       */
#define WFA_KERNEL_MIN_TIMER_RES   100   /* HZ Value for 10 ms */

void wfaTxSleepTime(int profile, int rate, int *sleepTime, int *throttledRate)
{
    *sleepTime=0;     /* in microseconds */
    /* calculate the sleep time based on frame rate */

    /*
     * Framerate is still required for Multicast traffic
     * Sleep and hold for a timeout.
     *
     * For WMM traffic, the framerate must also need for VO and VI.
     * the framerate 500, OS may not handle it precisely.
     */
    switch(profile)
    {
    /*
     * make it a fix rate
     * according to test plan, it requires ~80kbps which is around 50 frames/s
     * For other cases which may want to run experiments for very high rate,
     * the change should accommodate the requirement.
     */
    case PROF_MCAST:
        if(rate < 500 && rate >= 50)
        {
            *sleepTime = 100000;  /* sleep for 100 ms */
            *throttledRate = WFA_MCAST_FRATE;
        }
        else
        {
            *sleepTime = 100000;
            *throttledRate = rate;
        }
#if 0
        *throttledRate = WFA_MCAST_FRATE;
#endif
        break;

    /*
     * Vendor must find ways to better adjust the speed for their own device
     */
    case PROF_IPTV:
    case PROF_FILE_TX:
        if(rate >=50 || rate == 0)
        {
            /*
             * this sleepTime indeed is now being used for time period
             * to send packets in the throttled Rate.
             * The idea here is that in each fixed 20 minisecond period,
             * The device will send rate/50 (rate = packets / second),
             * then go sleep for rest of time.
             */
            *sleepTime = 20000; /* fixed 20 miniseconds */
            *throttledRate = (rate?rate:10000)/50;
            printf("Hi Sleep time %i, throttledRate %i\n", *sleepTime, *throttledRate);
        }
        else if(rate == 0)
        {
            *sleepTime = 20000; /* fixed 20 miniseconds */
            *throttledRate = (rate?rate:10000)/50;
            printf("Hi Sleep time %i, throttledRate %i\n", *sleepTime, *throttledRate);
        }
        else if (rate > 0 && rate <= 50) /* typically for voice */
        {
            *throttledRate = 1;
            *sleepTime = 1000*1000/rate;
        }
        break;
    default:
        DPRINT_ERR(WFA_ERR, "Incorrect profile\n");
    }
}

#define WFA_TIME_DIFF(before, after, rtime, dtime) \
             dtime = rtime + (after.tv_sec*1000000 + after.tv_usec) - (before.tv_sec*1000000 + before.tv_usec);

void buzz_time(int delay)
{
    struct timeval now, stop;
    int diff;
    int remain_time = 0;

    //wGETTIMEOFDAY(&stop, 0);
    gettimeofday(&stop, NULL);

    stop.tv_usec += delay;
    if(stop.tv_usec > 1000000)
    {
        stop.tv_usec -=1000000;
        stop.tv_sec +=1;
    }

    do
    {
        wGETTIMEOFDAY(&now, 0);
        WFA_TIME_DIFF(now, stop, remain_time, diff);
    }
    while(diff>0);
}

/**************************************************/
/* the actually functions to send/receive packets */
/**************************************************/

/* This is going to be a blocking SEND till it finishes */
int wfaSendLongFile(int mySockfd, int streamid, BYTE *aRespBuf, int *aRespLen)
{
    tgProfile_t           *theProf = NULL;
    tgStream_t            *myStream = NULL;
    struct sockaddr_in    toAddr;
    char                  *packBuf;
    int  packLen;
    int  bytesSent;
    dutCmdResponse_t sendResp;
    int sleepTime = 0;
    int throttledRate = 0;
    struct timeval before, after,af;
    int difftime = 0, counter = 0;
    struct timeval stime;
    int act_sleep_time;
    gettimeofday(&af,0);

    DPRINT_INFO(WFA_OUT, "Entering sendLongFile %i\n", streamid);

    /* find the profile */
    myStream = findStreamProfile(streamid);
    if(myStream == NULL)
    {
        return WFA_FAILURE;
    }

    theProf = &myStream->profile;

    if(theProf == NULL)
    {
        return WFA_FAILURE;
    }

    /* If RATE is 0 which means to send as much as possible, the frame size set to max UDP length */
    if(theProf->rate == 0)
	{
		if(theProf->hti == 0 && theProf->pksize)
			packLen = theProf->pksize;
		else
			packLen = MAX_UDP_LEN;
	}
    else
        packLen = theProf->pksize;

    /* allocate a buf */
    packBuf = (char *)malloc(packLen+1);
    wMEMSET(packBuf, 0, packLen);

    /* fill in the header */
    wSTRNCPY(packBuf, "1345678", sizeof(tgHeader_t));

    /* initialize the destination address */
    wMEMSET(&toAddr, 0, sizeof(toAddr));
    toAddr.sin_family = AF_INET;
    toAddr.sin_addr.s_addr = inet_addr(theProf->dipaddr);
    toAddr.sin_port = htons(theProf->dport);

    /* if a frame rate and duration are defined, then we know
     * interval for each packet and how many packets it needs to
     * send.
     */
    if(theProf->duration != 0)
    {
        printf("duration %i\n", theProf->duration);

        /*
         *  use this to decide periodical interval sleep time and frames to send
         *  int the each interval.
         *  Each device should adopt a own algorithm for better performance
         */
        wfaTxSleepTime(theProf->profile, theProf->rate, &sleepTime, &throttledRate);
        /*
         * alright, we need to raise the priority level of the process
         * to improve the real-time performance of packet sending.
         * Since this is for tuning purpose, it is optional implementation.
         */

        act_sleep_time = sleepTime - adj_latency;
        if (act_sleep_time <= 0)
            act_sleep_time = sleepTime;

        printf("sleep time %i act_sleep_time %i\n", sleepTime, act_sleep_time);

        runLoop=1;
        while(runLoop)
        {
            counter++;
            /* fill in the counter */
            int2BuffBigEndian(counter, &((tgHeader_t *)packBuf)->hdr[8]);

            /*
             * the following code is only used to slow down
             * over fast traffic flooding the buffer and cause
             * packet drop or the other end not able to receive due to
             * some limitations, purely for experiment purpose.
             * each implementation needs some fine tune to it.
             */
            if(counter ==1)
            {
                wGETTIMEOFDAY(&before, NULL);

                before.tv_usec += sleepTime;
                if(before.tv_usec > 1000000)
                {
                    before.tv_usec -= 1000000;
                    before.tv_sec +=1;
                }
            }

            if(throttledRate != 0)
            {
                if(counter%throttledRate == 0)
                {
                    wGETTIMEOFDAY(&after, NULL);
                    difftime = wfa_itime_diff(&after, &before);

                    if(difftime > adj_latency)
                    {
                        // too much time left, go sleep
                        wUSLEEP(difftime-adj_latency);

                        wGETTIMEOFDAY(&after, NULL);
                        difftime = wfa_itime_diff(&after, &before);
                    }

                    // burn the rest to absort latency
                    if(difftime >0)
                        buzz_time(difftime);

                    before.tv_usec += sleepTime;
                    if(before.tv_usec > 1000000)
                    {
                        before.tv_usec -= 1000000;
                        before.tv_sec +=1;
                    }
                }
            } // otherwise, it floods

            /*
             * Fill the timestamp to the header.
             */
            wGETTIMEOFDAY(&stime, NULL);

            int2BuffBigEndian(stime.tv_sec, &((tgHeader_t *)packBuf)->hdr[12]);
            int2BuffBigEndian(stime.tv_usec, &((tgHeader_t *)packBuf)->hdr[16]);

            bytesSent = wfaTrafficSendTo(mySockfd, packBuf, packLen,
                                         (struct sockaddr *)&toAddr);

            if(bytesSent != -1)
            {
                myStream->stats.txPayloadBytes += bytesSent;
                myStream->stats.txFrames++ ;
            }
            else
            {
                int errsv = errno;
                switch(errsv)
                {
                case EAGAIN:
                case ENOBUFS:
                    DPRINT_ERR(WFA_ERR, "send error\n");
                    wUSLEEP(1000);             /* hold for 1 ms */
                    counter-- ;
                    myStream->stats.txFrames--;
                    break;
                case ECONNRESET:
                    runLoop = 0;
                    break;
                case EPIPE:
                    runLoop = 0;
                    break;
                default:
                    perror("sendto: ");
                    DPRINT_ERR(WFA_ERR, "Packet sent error\n");
                }
            }

        }


        /*
         * lower back to an original level if the process is raised previously
         * It is optional.
         */
    }
    else /* invalid parameters */
    {
        /* encode a TLV for response for "invalid ..." */
        sendResp.status = STATUS_INVALID;
        wfaEncodeTLV(WFA_TRAFFIC_AGENT_SEND_RESP_TLV, 4,
                     (BYTE *)&sendResp, (BYTE *)aRespBuf);

        /* done here */
        *aRespLen = WFA_TLV_HDR_LEN + 4;

        return DONE;
    }

//    gtgSend = 0;

    /* free the buffer */
    wFREE(packBuf);

    //printf("done sending long\n");
    /* return statistics */
    sendResp.status = STATUS_COMPLETE;
    sendResp.streamId = myStream->id;
    wMEMCPY(&sendResp.cmdru.stats, &myStream->stats, sizeof(tgStats_t));

#if 0
    DPRINT_INFO(WFA_OUT, "stream Id %u tx %u total %llu\n", myStream->id, myStream->stats.txFrames, myStream->stats.txPayloadBytes);
#endif

    wfaEncodeTLV(WFA_TRAFFIC_AGENT_SEND_RESP_TLV, sizeof(dutCmdResponse_t),
                 (BYTE *)&sendResp, (BYTE *)aRespBuf);

    *aRespLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    return DONE;
}

/* this only sends one packet a time */
int wfaSendShortFile(int mySockfd, int streamid, BYTE *sendBuf, int pksize, BYTE *aRespBuf, int *aRespLen)
{
    BYTE *packBuf = sendBuf;
    struct sockaddr_in toAddr;
    tgProfile_t *theProf;
    tgStream_t *myStream;
    int packLen, bytesSent=-1;
    dutCmdResponse_t sendResp;

    if(mySockfd == -1)
    {
        /* stop */
//        gtgTransac = 0;
//        gtgRecv = 0;
//        gtgSend = 0;
        printf("stop short traffic\n");

        myStream = findStreamProfile(streamid);
        if(myStream != NULL)
        {
            sendResp.status = STATUS_COMPLETE;
            sendResp.streamId = streamid;
            wMEMCPY(&sendResp.cmdru.stats, &myStream->stats, sizeof(tgStats_t));

            wfaEncodeTLV(WFA_TRAFFIC_AGENT_SEND_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)&sendResp, aRespBuf);

            *aRespLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);
        }

        return DONE;
    }

    /* find the profile */
    myStream = findStreamProfile(streamid);

    if(myStream == NULL)
    {
        return WFA_FAILURE;
    }

    theProf = &myStream->profile;
    if(theProf == NULL)
    {
        return WFA_FAILURE;
    }

    if(pksize == 0)
        packLen = theProf->pksize;
    else
        packLen = pksize;

    wMEMSET(&toAddr, 0, sizeof(toAddr));
    toAddr.sin_family = AF_INET;
    toAddr.sin_addr.s_addr = inet_addr(theProf->sipaddr);
    toAddr.sin_port = htons(theProf->sport);

//    if(gtgRecv && gtgTransac)
//    {
//        toAddr.sin_addr.s_addr = inet_addr(theProf->sipaddr);
//        toAddr.sin_port = htons(theProf->sport);
//    }
//    else if(gtgSend && gtgTransac)
//    {
//        toAddr.sin_addr.s_addr = inet_addr(theProf->dipaddr);
//        toAddr.sin_port = htons(theProf->dport);
//    }

    int2BuffBigEndian(myStream->stats.txFrames, &((tgHeader_t *)packBuf)->hdr[8]);

    if(mySockfd != -1)
        bytesSent = wfaTrafficSendTo(mySockfd, (char *)packBuf, packLen, (struct sockaddr *)&toAddr);

    if(bytesSent != -1)
    {
        myStream->stats.txFrames++;
        myStream->stats.txPayloadBytes += bytesSent;
    }
    else
    {
        int errsv = errno;
        switch(errsv)
        {
        case EAGAIN:
        case ENOBUFS:
            DPRINT_ERR(WFA_ERR, "send error\n");
            wUSLEEP(1000);             /* hold for 1 ms */
            myStream->stats.txFrames--;
            break;
        default:
            ;;
        }
    }

    sentTranPkts++;

    return WFA_SUCCESS;
}

/* always receive from a specified IP address and Port */
int wfaRecvFile(int mySockfd, int streamid, char *recvBuf)
{
    /* how many packets are received */
    char *packBuf = recvBuf;
    struct sockaddr_in fromAddr;
    tgProfile_t *theProf;
    tgStream_t *myStream;
    int bytesRecvd;
    int lostPkts;

    /* find the profile */
    myStream = findStreamProfile(streamid);
    if(myStream == NULL)
    {
        return WFA_ERROR;
    }

    theProf = &myStream->profile;
    if(theProf == NULL)
    {
        return WFA_ERROR;
    }

    wMEMSET(packBuf, 0, MAX_UDP_LEN);

    wMEMSET(&fromAddr, 0, sizeof(fromAddr));
    fromAddr.sin_family = AF_INET;
    fromAddr.sin_addr.s_addr = inet_addr(theProf->dipaddr);
    fromAddr.sin_port = htons(theProf->dport);

//    if(gtgRecv && gtgTransac)
//    {
//        fromAddr.sin_addr.s_addr = inet_addr(theProf->sipaddr);
//        fromAddr.sin_port = htons(theProf->sport);
//    }
//    else if(gtgSend && gtgTransac)
//    {
//        fromAddr.sin_addr.s_addr = inet_addr(theProf->dipaddr);
//        fromAddr.sin_port = htons(theProf->dport);
//    }

    /* it is always to receive at least one packet, in case more in the
       queue, just pick them up.
     */
    bytesRecvd = wfaTrafficRecv(mySockfd, packBuf, (struct sockaddr *)&fromAddr);
    if(bytesRecvd != -1)
    {
        myStream->stats.rxFrames++;
        myStream->stats.rxPayloadBytes +=bytesRecvd;

        /*
         *  Get the lost packet count
         */
        lostPkts = bigEndianBuff2Int(&((tgHeader_t *)packBuf)->hdr[8]) - 1 - myStream->lastPktSN;
        myStream->stats.lostPkts += lostPkts;
        myStream->lastPktSN = bigEndianBuff2Int(&((tgHeader_t *)packBuf)->hdr[8]);
    }
//    else
//    {
//        DPRINT_ERR(WFA_OUT, "receive error\n");
//    }
    return (bytesRecvd);
}

//  new add-on code to process limite bitrate data push
//

void wfaSleepMilsec(int milSec)
{
    if (milSec >0)
//    usleep(milSec * 1000);
    osDelay(milSec);
}

/* send limite bitrate data only
  Condition under Win7:
   payload <= 1000
   rate    <= 3500


*/
int wfaSendBitrateData(int mySockfd, int streamId, BYTE *pRespBuf, int *pRespLen)
{
    tgProfile_t           *theProf = findTGProfile(streamId);
    tgStream_t            *myStream =findStreamProfile(streamId);
    int                   ret = WFA_SUCCESS;
    struct sockaddr_in    toAddr;
    char                  *packBuf=NULL;
    int                   packLen, bytesSent, rate;
    int                   nOverTimeCount = 0, nDuration=0, nOverSend=0;
    unsigned long long int sleepTotal=0, extraTimeSpendOnSending=0;   /* sleep mil-sec per sending  */
    int                   counter = 0, i, iSleep;     /*  frame data sending count */
    long                  difftime;
    dutCmdResponse_t      *sendResp = &gGenericResp;
    struct timeval        before, after, stime;

    printf("wfaSendBitrateData entering ...\n");
    /* error check section  */
    if ( (mySockfd < 0) || (streamId < 0) || ( pRespBuf == NULL)
            || ( pRespLen == NULL) )
    {
        DPRINT_INFO(WFA_OUT, "wfaSendBitrateData pass-in parameter err mySockfd=%i streamId=%i pRespBuf=%p pRespLen=%p\n",
                    mySockfd, streamId, pRespBuf, pRespLen );
        ret= WFA_FAILURE;
        goto errcleanup;
    }

    if ( theProf == NULL || myStream == NULL)
    {
        DPRINT_INFO(WFA_OUT, "wfaSendBitrateData parameter err in NULL pt theProf=%p myStream=%p \n",
                    theProf, myStream);
        ret= WFA_FAILURE;
        goto errcleanup;
    }
    if (theProf->rate == 0 || theProf->duration == 0)
    {  /*  rate == 0 means unlimited to push data out which is not this routine to do */
        DPRINT_INFO(WFA_OUT, "wfaSendBitrateData usage error, bitrate=%i or duration=%i \n" ,
            theProf->rate, theProf->duration);
        ret= WFA_FAILURE;
        goto errcleanup;
    }

    if ((theProf->rate > 3500) || (theProf->pksize > 1000))
    {
        printf("wfaSendBitrateData Warning, must set total streams rate<=10000 and payload<=1000; stream bitrate may not meet\n");
    }

    /* calculate bitrate asked */
    if ( (rate = theProf->pksize * theProf->rate * 8) > WFA_SEND_FIX_BITRATE_MAX)
    {
        DPRINT_INFO(WFA_OUT, "wfaSendBitrateData over birate can do in the routine, req bitrate=%u \n",rate);
        ret= WFA_FAILURE;
        goto errcleanup;
    }

    packLen = theProf->pksize;
    packBuf = (char *)malloc(packLen+4);
    if ( packBuf == NULL)
    {
        DPRINT_INFO(WFA_OUT, "wfaSendBitrateData malloc err \n");
        ret= WFA_FAILURE;
        goto errcleanup;
    }
    memset(packBuf, 0, packLen + 4);
    /* fill in the header */
    wSTRNCPY(packBuf, "1345678", sizeof(tgHeader_t));

    /*  initialize the destination address  */
    memset(&toAddr, 0, sizeof(toAddr));
    toAddr.sin_family = AF_INET;
    toAddr.sin_addr.s_addr = inet_addr(theProf->dipaddr);
    toAddr.sin_port = htons(theProf->dport);

    runLoop=1; /* global defined share with thread routine, should remove it later  */
    while ( runLoop)
    {
        iSleep = 1;
        gettimeofday(&before, NULL);
        /* send data per second loop */
        for ( i=0; i<= (theProf->rate); i++)
        {
            counter++;
            //iSleep++;
            /* fill in the counter */
            int2BuffBigEndian(counter, &((tgHeader_t *)packBuf)->hdr[8]);
            /*
            * Fill the timestamp to the header.
            */
            wGETTIMEOFDAY(&stime, NULL);
            int2BuffBigEndian(stime.tv_sec, &((tgHeader_t *)packBuf)->hdr[12]);
            int2BuffBigEndian(stime.tv_usec, &((tgHeader_t *)packBuf)->hdr[16]);

            bytesSent = wfaTrafficSendTo(mySockfd, packBuf, packLen, (struct sockaddr *)&toAddr);

            if(bytesSent != -1)
            {
               myStream->stats.txPayloadBytes += bytesSent;
               myStream->stats.txFrames++ ;
            }
            else
            {
               counter--;
               wfaSleepMilsec(1);

               sleepTotal = sleepTotal + (unsigned long long int) 1;
               //printf("ERR counter=%i i=%i; send busy, sleep %i MilSec then send\n", counter, i, sleepTimePerSent);
               i--;
            }
            /*  sleep per batch sending */
            if ( i == ((int)(theProf->rate/50)) * iSleep)
            {
              wfaSleepMilsec(10);
              sleepTotal = sleepTotal + (unsigned long long int) 10;
              iSleep++;
            }


        }// for loop per second sending
        nDuration++;

        /*calculate second rest part need to sleep  */
        gettimeofday(&after, NULL);
        difftime = wfa_itime_diff(&before, &after);
        if ( difftime < 0 || difftime >= 1000000 )
        {/* over time used, no sleep, go back to send */
            nOverTimeCount++;
            if ( difftime > 1000000)
                extraTimeSpendOnSending += (difftime - 1000000);
            wfaSleepMilsec(1);
            sleepTotal++;
            continue;
        }

        /* difftime < 1 sec case, use sleep to catchup time as 1 sec per sending  */
        /*  check with accumulated extra time spend on previous sending, difftime < 1 sec case */
        if (extraTimeSpendOnSending > 0)
        {
            if ( extraTimeSpendOnSending > 1000000 - difftime )
            {/* reduce sleep time to catch up over all on time sending   */
                extraTimeSpendOnSending = (extraTimeSpendOnSending - (1000000 - difftime));
                wfaSleepMilsec(5);
                sleepTotal = sleepTotal + (unsigned long long int) 5;
                continue;
            }
            else
            {  /* usec used to   */
                difftime =(unsigned long)( difftime - extraTimeSpendOnSending);
                extraTimeSpendOnSending = 0;
            }
        }

        difftime = difftime/1000; // convert to mil-sec

        if(1000 - difftime > 0)
        {/*  only sleep if there is extrac time with in the second did not spend on sending */
            wfaSleepMilsec(1000 - difftime);
            sleepTotal = sleepTotal + (unsigned long long int)(1000 - difftime);
        }

        if ( theProf->duration < nDuration)
        {
            nOverSend++;
        }

    }// while loop

    if (packBuf) free(packBuf);
    /* return statistics */
    sendResp->status = STATUS_COMPLETE;
    sendResp->streamId = myStream->id;

    memcpy(&sendResp->cmdru.stats, &myStream->stats, sizeof(tgStats_t));

    wfaEncodeTLV(WFA_TRAFFIC_AGENT_SEND_RESP_TLV, sizeof(dutCmdResponse_t), (BYTE *)&sendResp, (BYTE *)pRespBuf);
    *pRespLen = WFA_TLV_HDR_LEN + sizeof(dutCmdResponse_t);

    extraTimeSpendOnSending = extraTimeSpendOnSending/1000;

    printf("***\n");
    printf("wfaSendBitrateData streamId %u\n", streamId);
    printf("pRespLen %u\n", *pRespLen);
    printf("Count=%i txFrames=%i totalByteSent=%i sleepTotal=%llu milSec extraTimeSpendOnSending=%llu\n",
            counter, (myStream->stats.txFrames),(unsigned int) (myStream->stats.txPayloadBytes), sleepTotal,extraTimeSpendOnSending);
    printf("nOverTimeCount=%d nOverSend=%i rate=%d nDuration=%d iSleep=%d\n", nOverTimeCount, nOverSend, theProf->rate , nDuration, iSleep);
    printf("***\n");

    wfaSleepMilsec(1000);

    return ret;

errcleanup:
    /* encode a TLV for response for "invalid ..." */
    if (packBuf) free(packBuf);

    sendResp->status = STATUS_INVALID;
    wfaEncodeTLV(WFA_TRAFFIC_AGENT_SEND_RESP_TLV, 4, (BYTE *)&sendResp, (BYTE *)pRespBuf);
    *pRespLen = WFA_TLV_HDR_LEN + 4;

    return ret;
}/*  wfaSendBitrateData  */
