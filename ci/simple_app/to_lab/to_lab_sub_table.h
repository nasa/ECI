/************************************************************************
**
**      GSC-18128-1, "Core Flight Executive Version 6.6"
**
**      Copyright (c) 2006-2019 United States Government as represented by
**      the Administrator of the National Aeronautics and Space Administration.
**      All Rights Reserved.
**
**      Licensed under the Apache License, Version 2.0 (the "License");
**      you may not use this file except in compliance with the License.
**      You may obtain a copy of the License at
**
**        http://www.apache.org/licenses/LICENSE-2.0
**
**      Unless required by applicable law or agreed to in writing, software
**      distributed under the License is distributed on an "AS IS" BASIS,
**      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**      See the License for the specific language governing permissions and
**      limitations under the License.
**
** File: to_sub_table.h
**
** Purpose: 
**  Define TO Lab CPU specific subscription table 
**
** Notes:
**
** $Log: to_lab_sub_table.h  $
** Revision 1.3 2014/07/16 14:44:45GMT-05:00 acudmore 
** Member renamed from to_sub_table.h to to_lab_sub_table.h in project c:/MKSDATA/MKS-REPOSITORY/CFS-REPOSITORY/to_lab/fsw/platform_inc/project.pj.
** Revision 1.2 2014/07/16 14:44:45ACT acudmore 
** Updated TO_LAB subscription table header file ( comments, clean up )
**
*************************************************************************/

/* 
** Add the proper include file for the message IDs below
*/
#include "cfe_msgids.h"

/*
** Common CFS app includes below are commented out
*/
#include "ci_lab_msgids.h"

#if 0
#include "hs_msgids.h"
#include "fm_msgids.h"
#include "sc_msgids.h"
#include "ds_msgids.h"
#include "lc_msgids.h"
#endif

static TO_subscription_t  TO_SubTable[] =
{
            /* CFS App Subscriptions */
            {TO_LAB_HK_TLM_MID,     {0,0},  4},
            {TO_LAB_DATA_TYPES_MID, {0,0},  4},
            {CI_LAB_HK_TLM_MID,     {0,0},  4},

#if 0
            /* Add these if needed */
            {HS_HK_TLM_MID,         {0,0},  4},
            {FM_HK_TLM_MID,         {0,0},  4},
            {SC_HK_TLM_MID,         {0,0},  4},
            {DS_HK_TLM_MID,         {0,0},  4},
            {LC_HK_TLM_MID,         {0,0},  4},
#endif
            /* cFE Core subsciptions */
            {CFE_ES_HK_TLM_MID,     {0,0},  4},
            {CFE_EVS_HK_TLM_MID,    {0,0},  4},
            {CFE_SB_HK_TLM_MID,     {0,0},  4},
            {CFE_TBL_HK_TLM_MID,    {0,0},  4},
            {CFE_TIME_HK_TLM_MID,   {0,0},  4},
            {CFE_TIME_DIAG_TLM_MID, {0,0},  4},
            {CFE_SB_STATS_TLM_MID,  {0,0},  4},
            {CFE_TBL_REG_TLM_MID,   {0,0},  4},
            {CFE_EVS_EVENT_MSG_MID, {0,0},  32},
            {CFE_ES_SHELL_TLM_MID,  {0,0},  32},
            {CFE_ES_APP_TLM_MID,    {0,0},   4},
            {CFE_ES_MEMSTATS_TLM_MID,{0,0},  4},

            {TO_UNUSED,              {0,0},  0},
            {TO_UNUSED,              {0,0},  0},
            {TO_UNUSED,              {0,0},  0}
};

/************************
** End of File Comment ** 
************************/
