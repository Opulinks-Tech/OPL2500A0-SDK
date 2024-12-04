/**
 * @file
 * Transmission Control Protocol, incoming traffic
 *
 * The input processing functions of the TCP layer.
 *
 * These functions are generally called in the order (ip_input() ->)
 * tcp_input() -> * tcp_process() -> tcp_receive() (-> application).
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

#if LWIP_TCP /* don't build if not configured for use in lwipopts.h */

#include "lwip/priv/tcp_priv.h"
#include "lwip/def.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/inet_chksum.h"
#include "lwip/stats.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#if LWIP_ND6_TCP_REACHABILITY_HINTS
#include "lwip/nd6.h"
#endif /* LWIP_ND6_TCP_REACHABILITY_HINTS */

#if defined(LWIP_ROMBUILD)
#include "tcp_if.h"
#endif

/** Initial CWND calculation as defined RFC 2581 */
#define LWIP_TCP_CALC_INITIAL_CWND(mss) LWIP_MIN((4U * (mss)), LWIP_MAX((2U * (mss)), 4380U));

/* These variables are global to all functions involved in the input
   processing of TCP segments. They are set by the tcp_input()
   function. */
extern LWIP_RETDATA struct tcp_seg inseg;
extern LWIP_RETDATA struct tcp_hdr *tcphdr;
extern LWIP_RETDATA u16_t tcphdr_optlen;
extern LWIP_RETDATA u16_t tcphdr_opt1len;
extern LWIP_RETDATA u8_t *tcphdr_opt2;
extern LWIP_RETDATA u16_t tcp_optidx;
extern LWIP_RETDATA u32_t seqno, ackno;
extern LWIP_RETDATA tcpwnd_size_t recv_acked;
extern LWIP_RETDATA u16_t tcplen;
extern LWIP_RETDATA u8_t flags;

extern LWIP_RETDATA u8_t recv_flags;
extern LWIP_RETDATA struct pbuf *recv_data;

extern LWIP_RETDATA struct tcp_pcb *tcp_input_pcb;

/**
 * Implements the TCP state machine. Called by tcp_input. In some
 * states tcp_receive() is called to receive data. The tcp_seg
 * argument will be freed by the caller (tcp_input()) unless the
 * recv_data pointer in the pcb is set.
 *
 * @param pcb the tcp_pcb for which a segment arrived
 *
 * @note the segment which arrived is saved in global variables, therefore only the pcb
 *       involved is passed as a parameter to this function
 */
static err_t tcp_process_patch(struct tcp_pcb *pcb)
{
  struct tcp_seg *rseg;
  u8_t acceptable = 0;
  err_t err;

  err = ERR_OK;

  /* Process incoming RST segments. */
  if (flags & TCP_RST) {
    /* First, determine if the reset is acceptable. */
    if (pcb->state == SYN_SENT) {
      /* "In the SYN-SENT state (a RST received in response to an initial SYN),
          the RST is acceptable if the ACK field acknowledges the SYN." */
      if (ackno == pcb->snd_nxt) {
        acceptable = 1;
      }
    } else {
      /* "In all states except SYN-SENT, all reset (RST) segments are validated
          by checking their SEQ-fields." */
      if (seqno == pcb->rcv_nxt) {
        acceptable = 1;
      } else  if (TCP_SEQ_BETWEEN(seqno, pcb->rcv_nxt,
                                  pcb->rcv_nxt + pcb->rcv_wnd)) {
        /* If the sequence number is inside the window, we only send an ACK
           and wait for a re-send with matching sequence number.
           This violates RFC 793, but is required to protection against
           CVE-2004-0230 (RST spoofing attack). */
        tcp_ack_now(pcb);
      }
    }

    if (acceptable) {
      //LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_process: Connection RESET\n"));
      printf("tcp_process: Connection RESET\n");
      LWIP_ASSERT("tcp_input: pcb->state != CLOSED", pcb->state != CLOSED);
      recv_flags |= TF_RESET;
      pcb->flags &= ~TF_ACK_DELAY;
      return ERR_RST;
    } else {
      printf("tcp_process: unacceptable reset seqno %"U32_F" rcv_nxt %"U32_F"\n",seqno, pcb->rcv_nxt);

      //LWIP_DEBUGF(TCP_INPUT_DEBUG, ("tcp_process: unacceptable reset seqno %"U32_F" rcv_nxt %"U32_F"\n",
       //seqno, pcb->rcv_nxt));
      LWIP_DEBUGF(TCP_DEBUG, ("tcp_process: unacceptable reset seqno %"U32_F" rcv_nxt %"U32_F"\n",
       seqno, pcb->rcv_nxt));
      return ERR_OK;
    }
  }

  if ((flags & TCP_SYN) && (pcb->state != SYN_SENT && pcb->state != SYN_RCVD)) {
    /* Cope with new connection attempt after remote end crashed */
    tcp_ack_now(pcb);
    return ERR_OK;
  }

  if ((pcb->flags & TF_RXCLOSED) == 0) {
    /* Update the PCB (in)activity timer unless rx is closed (see tcp_shutdown) */
    pcb->tmr = tcp_ticks;
  }
  pcb->keep_cnt_sent = 0;

  tcp_parseopt(pcb);

  /* Do different things depending on the TCP state. */
  switch (pcb->state) {
  case SYN_SENT:
    LWIP_DEBUGF(TCP_INPUT_DEBUG, ("SYN-SENT: ackno %"U32_F" pcb->snd_nxt %"U32_F" unacked %"U32_F"\n", ackno,
     pcb->snd_nxt, lwip_ntohl(pcb->unacked->tcphdr->seqno)));
    /* received SYN ACK with expected sequence number? */
    if ((flags & TCP_ACK) && (flags & TCP_SYN)
        && (ackno == pcb->lastack + 1)) {
      pcb->rcv_nxt = seqno + 1;
      pcb->rcv_ann_right_edge = pcb->rcv_nxt;
      pcb->lastack = ackno;
      pcb->snd_wnd = tcphdr->wnd;
      pcb->snd_wnd_max = pcb->snd_wnd;
      pcb->snd_wl1 = seqno - 1; /* initialise to seqno - 1 to force window update */
      pcb->state = ESTABLISHED;

#if TCP_CALCULATE_EFF_SEND_MSS
      pcb->mss = tcp_eff_send_mss(pcb->mss, &pcb->local_ip, &pcb->remote_ip);
#endif /* TCP_CALCULATE_EFF_SEND_MSS */

      pcb->cwnd = LWIP_TCP_CALC_INITIAL_CWND(pcb->mss);
      LWIP_DEBUGF(TCP_CWND_DEBUG, ("tcp_process (SENT): cwnd %"TCPWNDSIZE_F
                                   " ssthresh %"TCPWNDSIZE_F"\n",
                                   pcb->cwnd, pcb->ssthresh));
      LWIP_ASSERT("pcb->snd_queuelen > 0", (pcb->snd_queuelen > 0));
      --pcb->snd_queuelen;
      LWIP_DEBUGF(TCP_QLEN_DEBUG, ("tcp_process: SYN-SENT --queuelen %"TCPWNDSIZE_F"\n", (tcpwnd_size_t)pcb->snd_queuelen));
      rseg = pcb->unacked;
      if (rseg == NULL) {
        /* might happen if tcp_output fails in tcp_rexmit_rto()
           in which case the segment is on the unsent list */
        rseg = pcb->unsent;
        LWIP_ASSERT("no segment to free", rseg != NULL);
        pcb->unsent = rseg->next;
      } else {
        pcb->unacked = rseg->next;
      }
      tcp_seg_free(rseg);

      /* If there's nothing left to acknowledge, stop the retransmit
         timer, otherwise reset it to start again */
      if (pcb->unacked == NULL) {
        pcb->rtime = -1;
      } else {
        pcb->rtime = 0;
        pcb->nrtx = 0;
      }

      /* Call the user specified function to call when successfully
       * connected. */
      TCP_EVENT_CONNECTED(pcb, ERR_OK, err);
      if (err == ERR_ABRT) {
        return ERR_ABRT;
      }
      tcp_ack_now(pcb);
    }
    /* received ACK? possibly a half-open connection */
    else if (flags & TCP_ACK) {
      /* send a RST to bring the other side in a non-synchronized state. */
      tcp_rst(ackno, seqno + tcplen, ip_current_dest_addr(),
        ip_current_src_addr(), tcphdr->dest, tcphdr->src);
      /* Resend SYN immediately (don't wait for rto timeout) to establish
        connection faster, but do not send more SYNs than we otherwise would
        have, or we might get caught in a loop on loopback interfaces. */
      if (pcb->nrtx < TCP_SYNMAXRTX) {
        pcb->rtime = 0;
        tcp_rexmit_rto(pcb);
      }
    }
    break;
  case SYN_RCVD:
    if (flags & TCP_ACK) {
      /* expected ACK number? */
      if (TCP_SEQ_BETWEEN(ackno, pcb->lastack+1, pcb->snd_nxt)) {
        pcb->state = ESTABLISHED;
        LWIP_DEBUGF(TCP_DEBUG, ("TCP connection established %"U16_F" -> %"U16_F".\n", inseg.tcphdr->src, inseg.tcphdr->dest));
#if LWIP_CALLBACK_API || TCP_LISTEN_BACKLOG
#if LWIP_CALLBACK_API
        LWIP_ASSERT("pcb->listener->accept != NULL",
          (pcb->listener == NULL) || (pcb->listener->accept != NULL));
#endif
        if (pcb->listener == NULL) {
          /* listen pcb might be closed by now */
          err = ERR_VAL;
        } else
#endif /* LWIP_CALLBACK_API || TCP_LISTEN_BACKLOG */
        {
          tcp_backlog_accepted(pcb);
          /* Call the accept function. */
          TCP_EVENT_ACCEPT(pcb->listener, pcb, pcb->callback_arg, ERR_OK, err);
        }
        if (err != ERR_OK) {
          /* If the accept function returns with an error, we abort
           * the connection. */
          /* Already aborted? */
          if (err != ERR_ABRT) {
            tcp_abort(pcb);
          }
          return ERR_ABRT;
        }
        /* If there was any data contained within this ACK,
         * we'd better pass it on to the application as well. */
        tcp_receive(pcb);

        /* Prevent ACK for SYN to generate a sent event */
        if (recv_acked != 0) {
          recv_acked--;
        }

        pcb->cwnd = LWIP_TCP_CALC_INITIAL_CWND(pcb->mss);
        LWIP_DEBUGF(TCP_CWND_DEBUG, ("tcp_process (SYN_RCVD): cwnd %"TCPWNDSIZE_F
                                     " ssthresh %"TCPWNDSIZE_F"\n",
                                     pcb->cwnd, pcb->ssthresh));

        if (recv_flags & TF_GOT_FIN) {
          tcp_ack_now(pcb);
          pcb->state = CLOSE_WAIT;
        }
      } else {
        /* incorrect ACK number, send RST */
        tcp_rst(ackno, seqno + tcplen, ip_current_dest_addr(),
          ip_current_src_addr(), tcphdr->dest, tcphdr->src);
      }
    } else if ((flags & TCP_SYN) && (seqno == pcb->rcv_nxt - 1)) {
      /* Looks like another copy of the SYN - retransmit our SYN-ACK */
      tcp_rexmit(pcb);
    }
    break;
  case CLOSE_WAIT:
    /* FALLTHROUGH */
  case ESTABLISHED:
    tcp_receive(pcb);
    if (recv_flags & TF_GOT_FIN) { /* passive close */
      tcp_ack_now(pcb);
      pcb->state = CLOSE_WAIT;
    }
    break;
  case FIN_WAIT_1:
    tcp_receive(pcb);
    if (recv_flags & TF_GOT_FIN) {
      if ((flags & TCP_ACK) && (ackno == pcb->snd_nxt) &&
          pcb->unsent == NULL) {
        LWIP_DEBUGF(TCP_DEBUG,
          ("TCP connection closed: FIN_WAIT_1 %"U16_F" -> %"U16_F".\n", inseg.tcphdr->src, inseg.tcphdr->dest));
        tcp_ack_now(pcb);
        tcp_pcb_purge(pcb);
        TCP_RMV_ACTIVE(pcb);
        pcb->state = TIME_WAIT;
        TCP_REG(&tcp_tw_pcbs, pcb);
      } else {
        tcp_ack_now(pcb);
        pcb->state = CLOSING;
      }
    } else if ((flags & TCP_ACK) && (ackno == pcb->snd_nxt) &&
               pcb->unsent == NULL) {
      pcb->state = FIN_WAIT_2;
    }
    break;
  case FIN_WAIT_2:
    tcp_receive(pcb);
    if (recv_flags & TF_GOT_FIN) {
      LWIP_DEBUGF(TCP_DEBUG, ("TCP connection closed: FIN_WAIT_2 %"U16_F" -> %"U16_F".\n", inseg.tcphdr->src, inseg.tcphdr->dest));
      tcp_ack_now(pcb);
      tcp_pcb_purge(pcb);
      TCP_RMV_ACTIVE(pcb);
      pcb->state = TIME_WAIT;
      TCP_REG(&tcp_tw_pcbs, pcb);
    }
    break;
  case CLOSING:
    tcp_receive(pcb);
    if ((flags & TCP_ACK) && ackno == pcb->snd_nxt && pcb->unsent == NULL) {
      LWIP_DEBUGF(TCP_DEBUG, ("TCP connection closed: CLOSING %"U16_F" -> %"U16_F".\n", inseg.tcphdr->src, inseg.tcphdr->dest));
      tcp_pcb_purge(pcb);
      TCP_RMV_ACTIVE(pcb);
      pcb->state = TIME_WAIT;
      TCP_REG(&tcp_tw_pcbs, pcb);
    }
    break;
  case LAST_ACK:
    tcp_receive(pcb);
    if ((flags & TCP_ACK) && ackno == pcb->snd_nxt && pcb->unsent == NULL) {
      LWIP_DEBUGF(TCP_DEBUG, ("TCP connection closed: LAST_ACK %"U16_F" -> %"U16_F".\n", inseg.tcphdr->src, inseg.tcphdr->dest));
      /* bugfix #21699: don't set pcb->state to CLOSED here or we risk leaking segments */
      recv_flags |= TF_CLOSED;
    }
    break;
  default:
    break;
  }
  return ERR_OK;
}

void lwip_load_interface_tcp_in_patch(void)
{
    tcp_process_adpt       = tcp_process_patch;
}


#endif /* LWIP_TCP */
