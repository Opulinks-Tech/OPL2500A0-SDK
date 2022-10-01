/* *****************************************************************************
 *  Copyright 2017-2021, Opulinks Technology Ltd.
 *  ---------------------------------------------------------------------------
 *  Statement:
 *  ----------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of Opulinks Technology Ltd. (C) 2021
 *
 *******************************************************************************
 *
 *  @file wfa_thr.h
 * 
 *  @brief 
 *  
 ******************************************************************************/

#ifndef _WFA_THR_H_
#define _WFA_THR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *                          Include files
 *************************************************************************
 */


/*
 *************************************************************************
 *                          Definitions and Macros
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Typedefs and Structures
 *************************************************************************
 */

/*
 *************************************************************************
 *                          Public Variables
 *************************************************************************
 */
extern tgStream_t gStreams[];

/*
 *************************************************************************
 *                          Public Functions
 *************************************************************************
 */
int wfa_thr_stream_default(void);
int wfa_thr_stream_queue_init(void);
int wfa_thr_stream_queue_is_full(void);
void *wfa_thr_stream_queue_get(void);
int wfa_thr_stream_queue_delete(void);

#ifdef __cplusplus
}
#endif
#endif /* _WFA_THR_H_ */
