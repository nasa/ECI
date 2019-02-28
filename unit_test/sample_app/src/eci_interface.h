/*
 * File: eci_interface.h
 *
 * Code generated for Simulink model 'unitTestModel'.
 *
 * Model version                  : 1.18
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Thu Sep 27 22:02:27 2018
 *
 * Target selection: cfs_ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_eci_interface_h_
#define RTW_HEADER_eci_interface_h_
#include "eci_app.h"
#include "eci_app_cfg.h"
#include "app_msgids.h"
#include "app_perfids.h"
#include "unitTestModel.h"             /* Model's header file */


/* Code Revision Identifier */
#define ECI_APP_REVISION_NUMBER                "unittest"

/*
 * Auto generated eci_interface.h for model: unitTestModel
 *
 *
 * Description:
 *
 * For more information:
 *   o Simulink Coder User's Guide
 *   o Embedded Coder User's Guide
 *   o CFS Target User's Guide
 *
 */
#define ECI_PARAM_TBL_DEFINED                   1

paramTable1_b *param_TBL1 ;
paramTable2_b *param_TBL2 ;
int32 Table_Validation0(void* TblPtr);
int32 Table_Validation1(void* TblPtr);
static ECI_Tbl_t ECI_ParamTable[] = {
  {
    &(param_TBL1),
    "param_TBL1",
    "unitTestModel app's param_TBL1 table",
    "param_TBL1.tbl",
    sizeof(paramTable1_b),
    &Table_Validation0
  },

  {
    &(param_TBL2),
    "param_TBL2",
    "unitTestModel app's param_TBL2 table",
    "param_TBL2.tbl",
    sizeof(paramTable2_b),
    &Table_Validation1
  },

  { 0, 0, 0, 0, 0 }
};

#define MODEL_NAME_LEN                 (14)
#if MODEL_NAME_LEN > OS_MAX_API_NAME
#error Model name unitTestModel must be less than CFE_TBL_MAX_NAME_LENGTH characters
#endif

#define ECI_FLAG_MID                   UNITTESTMODEL_FDC_MID
#define ECI_CMD_MID                    UNITTESTMODEL_CMD_MID
#define ECI_PERF_ID                    UNITTESTMODEL_PERF_ID
#define ECI_TICK_MID                   UNITTESTMODEL_TICK_MID
#define ECI_HK_MID                     UNITTESTMODEL_HK_MID
#define ECI_APP_MAIN                   unittestmodel_AppMain
#define ECI_APP_NAME_UPPER             "UNITTESTMODEL"
#define ECI_APP_NAME_LOWER             "unittestmodel"


#define ECI_CMD_PIPE_NAME              "UNITTESTMODEL_CMD_PIPE"
#define ECI_DATA_PIPE_NAME             "UNITTESTMODEL_DATA_PIPE"

/* Begin CSC_SL_SEND_Messages */

/* Place each output signal that is a bus into the Send Table */
static ECI_Msg_t ECI_MsgSnd[] = {
  { OUTPUTTLMPKT1_B_OUTPUTTLMPKT1_S_MID, &outputTlmPkt1_s, sizeof
    (outputTlmPkt1_b), NULL, NULL },

  { OUTPUTTLMPKT2_B_OUTPUTTLMPKT2_S_MID, &outputTlmPkt2_s, sizeof
    (outputTlmPkt2_b), NULL, &cmsgFlag_unitTestModel_117 },

  { OUTPUTTLMPKT3_B_OUTPUTTLMPKT3_S_MID, &outputTlmPkt3_s, sizeof
    (outputTlmPkt3_b), NULL, &cmsgFlag_unitTestModel_126 },

  { OUTPUTTLMPKT4_B_OUTPUTTLMPKT4_S_MID, &outputTlmPkt4_s, sizeof
    (outputTlmPkt4_b), NULL, &cmsgFlag_unitTestModel_126 },

  { 0, NULL, 0, NULL, NULL }
};

/* End CSC_SL_SEND_Messages */

/* Begin CSC_SL_RECV_Messages */
/* Specify Queue sizes for Command Messages */
static inputCmdPkt_b inputCmdPkt_s_queue[ECI_CMD_MSG_QUEUE_SIZE];

/* Place each input signal that is a bus into the Receive Table */
static ECI_Msg_t ECI_MsgRcv[] = {
  { INPUTCMDPKT_B_INPUTCMDPKT_S_MID, &inputCmdPkt_s, sizeof(inputCmdPkt_b),
    &inputCmdPkt_s_queue[0], NULL },

  { INPUTTLMPKT1_B_INPUTTLMPKT1_S_MID, &inputTlmPkt1_s, sizeof(inputTlmPkt1_b),
    NULL, NULL },

  { INPUTTLMPKT2_B_INPUTTLMPKT2_S_MID, &inputTlmPkt2_s, sizeof(inputTlmPkt2_b),
    NULL, NULL },

  { 0, NULL, 0, NULL, NULL }
};

/* End CSC_SL_RECV_Messages */

/* Create array of structures with error type, pointer to observable signal location,
   the location comment, and the event message */
#define ECI_EVENT_TABLE_DEFINED        1

static const ECI_Evs_t ECI_Events[] = {
  /* Event for block: unitTestModel/event1 */
  { EVENT_MESSAGE_1_DATA,
    &unitTestModel_ConstP.event1_event_id,
    &unitTestModel_ConstP.event1_event_type,
    &unitTestModel_ConstP.event1_event_mask,
    &evFlag_unitTestModel_122,
    unitTestModel_ConstP.event1_event_fmtstring,
    "unitTestModel/event1",
    &evData_unitTestModel_122[0],
    0,
    0,
    0,
    0
  },

  /* Event for block: unitTestModel/event3 */
  { EVENT_MESSAGE_3_DATA,
    &unitTestModel_ConstP.event3_event_id,
    &unitTestModel_ConstP.event3_event_type,
    &unitTestModel_ConstP.event3_event_mask,
    &evFlag_unitTestModel_154,
    unitTestModel_ConstP.event3_event_fmtstring,
    "unitTestModel/event3",
    &evData_unitTestModel_154[0],
    &evData_unitTestModel_154[1],
    &evData_unitTestModel_154[2],
    0,
    0
  },

  /* Event for block: unitTestModel/event4 */
  { EVENT_MESSAGE_4_DATA,
    &unitTestModel_ConstP.event4_event_id,
    &unitTestModel_ConstP.pooled9,
    &unitTestModel_ConstP.event4_event_mask,
    &evFlag_unitTestModel_155,
    unitTestModel_ConstP.event4_event_fmtstring,
    "unitTestModel/event4",
    &evData_unitTestModel_155[0],
    &evData_unitTestModel_155[1],
    &evData_unitTestModel_155[2],
    &evData_unitTestModel_155[3],
    0
  },

  /* Event for block: unitTestModel/event2 */
  { EVENT_MESSAGE_2_DATA,
    &unitTestModel_ConstP.event2_event_id,
    &unitTestModel_ConstP.event2_event_type,
    &unitTestModel_ConstP.event2_event_mask,
    &evFlag_unitTestModel_152,
    unitTestModel_ConstP.event2_event_fmtstring,
    "unitTestModel/event2",
    &evData_unitTestModel_152[0],
    &evData_unitTestModel_152[1],
    0,
    0,
    0
  },

  /* Event for block: unitTestModel/event0 */
  { EVENT_MESSAGE_0_DATA,
    &unitTestModel_ConstP.pooled8,
    &unitTestModel_ConstP.pooled9,
    &unitTestModel_ConstP.event0_event_mask,
    &evFlag_unitTestModel_150,
    unitTestModel_ConstP.event0_event_fmtstring,
    "unitTestModel/event0",
    0,
    0,
    0,
    0,
    0
  },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

/* Create array of FDC structures with id and flag */
#define ECI_FLAG_TABLE_DEFINED          1
#define ECI_FLAG_MAX_ID                 1

static const ECI_Flag_t ECI_Flags[] = {
  /* Event for block: unitTestModel/CFS_FDC_Event */
  { &unitTestModel_ConstP.pooled8,
    &fdcFlag_121
  },

  { 0, 0 }
};

/* GNC time is used in model code */
#define ECI_STEP_TIMESTAMP_DEFINED

/* model initialization function */
#define ECI_INIT_FCN \
 unitTestModel_initialize();

/* step function.  Single rate (non-reusable interface) */
#define ECI_STEP_FCN \
 unitTestModel_step();

/* Get model outputs here */
#define ECI_TERM_FCN                 unitTestModel_terminate();
#endif                                 /* RTW_HEADER_eci_interface_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
