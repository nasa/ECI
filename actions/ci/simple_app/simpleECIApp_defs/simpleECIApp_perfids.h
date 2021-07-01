/*
** File: cfe_perfids.h
**
** Purpose: This file contains the cFE performance IDs
**
**      Copyright (c) 2004-2012, United States government as represented by the 
**      administrator of the National Aeronautics Space Administration.  
**      All rights reserved. This software(cFE) was created at NASA's Goddard 
**      Space Flight Center pursuant to government contracts.
**
**      This is governed by the NASA Open Source Agreement and may be used,
**      distributed and modified only pursuant to the terms of that agreement. 
**
** Design Notes:
**   Each performance id is used to identify something that needs to be
**   measured.  Performance ids are limited to the range of 0 to
**   CFE_PLATFORM_ES_PERF_MAX_IDS - 1.  Any performance ids outside of this range
**   will be ignored and will be flagged as an error.  Note that
**   performance ids 0-31 are reserved for the cFE Core.
**
** References:
**
*/

#ifndef _cfe_perfids_
#define _cfe_perfids_


#define CFE_MISSION_ES_PERF_EXIT_BIT            31      /**< \brief bit (31) is reserved by the perf utilities */

/** \name cFE Performance Monitor IDs (Reserved IDs 0-31) */
/** \{ */
#define CFE_MISSION_ES_MAIN_PERF_ID             1    /**< \brief Performance ID for Executive Services Task */
#define CFE_MISSION_EVS_MAIN_PERF_ID            2    /**< \brief Performance ID for Events Services Task */
#define CFE_MISSION_TBL_MAIN_PERF_ID            3    /**< \brief Performance ID for Table Services Task */
#define CFE_MISSION_SB_MAIN_PERF_ID             4    /**< \brief Performance ID for Software Bus Services Task */
#define CFE_MISSION_SB_MSG_LIM_PERF_ID          5    /**< \brief Performance ID for Software Bus Msg Limit Errors */
#define CFE_MISSION_SB_PIPE_OFLOW_PERF_ID       27   /**< \brief Performance ID for Software Bus Pipe Overflow Errors */


#define CFE_MISSION_TIME_MAIN_PERF_ID           6    /**< \brief Performance ID for Time Services Task */
#define CFE_MISSION_TIME_TONE1HZISR_PERF_ID     7    /**< \brief Performance ID for 1 Hz Tone ISR */
#define CFE_MISSION_TIME_LOCAL1HZISR_PERF_ID    8    /**< \brief Performance ID for 1 Hz Local ISR */

#define CFE_MISSION_TIME_SENDMET_PERF_ID        9    /**< \brief Performance ID for Time ToneSendMET */
#define CFE_MISSION_TIME_LOCAL1HZTASK_PERF_ID   10   /**< \brief Performance ID for 1 Hz Local Task */
#define CFE_MISSION_TIME_TONE1HZTASK_PERF_ID    11   /**< \brief Performance ID for 1 Hz Tone Task */


/** \} */


#endif   /* _cfe_perfids_ */
