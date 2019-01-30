/*
 * File: unitTestModel.c
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

#include "unitTestModel.h"
#include "unitTestModel_private.h"

const CCSDS_TlmHdr unitTestModel_rtZCCSDS_TlmHdr = {
  {
    0U, 0U }
  ,                                    /* StreamID */

  {
    0U, 0U }
  ,                                    /* Sequence */

  {
    0U, 0U }
  ,                                    /* PktLength */

  {
    0U, 0U, 0U, 0U }
  ,                                    /* TimeSec */

  {
    0U, 0U }
  /* TimeSubsec */
} ;                                    /* CCSDS_TlmHdr ground */

const inputTlmPkt1_b unitTestModel_rtZinputTlmPkt1_b = {
  {
    {
      0U, 0U }
    ,                                  /* StreamID */

    {
      0U, 0U }
    ,                                  /* Sequence */

    {
      0U, 0U }
    ,                                  /* PktLength */

    {
      0U, 0U, 0U, 0U }
    ,                                  /* TimeSec */

    {
      0U, 0U }
    /* TimeSubsec */
  },                                   /* TlmHdr */
  0U,                                  /* inVal1 */
  0U,                                  /* inVal2 */
  0U,                                  /* inVal3 */
  0U                                   /* inVal4 */
} ;                                    /* inputTlmPkt1_b ground */

const inputTlmPkt2_b unitTestModel_rtZinputTlmPkt2_b = {
  {
    {
      0U, 0U }
    ,                                  /* StreamID */

    {
      0U, 0U }
    ,                                  /* Sequence */

    {
      0U, 0U }
    ,                                  /* PktLength */

    {
      0U, 0U, 0U, 0U }
    ,                                  /* TimeSec */

    {
      0U, 0U }
    /* TimeSubsec */
  },                                   /* TlmHdr */
  0U,                                  /* inVal1 */
  0U,                                  /* inVal2 */
  0U                                   /* inVal3 */
} ;                                    /* inputTlmPkt2_b ground */

const inputCmdPkt_b unitTestModel_rtZinputCmdPkt_b = {
  {
    {
      0U, 0U }
    ,                                  /* StreamID */

    {
      0U, 0U }
    ,                                  /* Sequence */

    {
      0U, 0U }
    ,                                  /* PktLength */
    0U,                                /* FcnCode */
    0U                                 /* ChkSum */
  },                                   /* CmdHdr */
  0U                                   /* arg1 */
} ;                                    /* inputCmdPkt_b ground */

const outputTlmPkt2_b unitTestModel_rtZoutputTlmPkt2_b = {
  {
    {
      0U, 0U }
    ,                                  /* StreamID */

    {
      0U, 0U }
    ,                                  /* Sequence */

    {
      0U, 0U }
    ,                                  /* PktLength */

    {
      0U, 0U, 0U, 0U }
    ,                                  /* TimeSec */

    {
      0U, 0U }
    /* TimeSubsec */
  },                                   /* TlmHdr */
  0U,                                  /* outVal1 */
  0U,                                  /* outVal2 */
  0U                                   /* outVal3 */
} ;                                    /* outputTlmPkt2_b ground */

const outputTlmPkt1_b unitTestModel_rtZoutputTlmPkt1_b = {
  {
    {
      0U, 0U }
    ,                                  /* StreamID */

    {
      0U, 0U }
    ,                                  /* Sequence */

    {
      0U, 0U }
    ,                                  /* PktLength */

    {
      0U, 0U, 0U, 0U }
    ,                                  /* TimeSec */

    {
      0U, 0U }
    /* TimeSubsec */
  },                                   /* TlmHdr */
  0U,                                  /* outVal1 */
  0U,                                  /* outVal2 */
  0U,                                  /* outVal3 */
  0U                                   /* outVal4 */
} ;                                    /* outputTlmPkt1_b ground */

const outputTlmPkt3_b unitTestModel_rtZoutputTlmPkt3_b = {
  {
    {
      0U, 0U }
    ,                                  /* StreamID */

    {
      0U, 0U }
    ,                                  /* Sequence */

    {
      0U, 0U }
    ,                                  /* PktLength */

    {
      0U, 0U, 0U, 0U }
    ,                                  /* TimeSec */

    {
      0U, 0U }
    /* TimeSubsec */
  },                                   /* TlmHdr */

  {
    0U,                                /* param1 */
    0U,                                /* param2 */
    0U,                                /* param3 */
    0U,                                /* param4 */
    0U                                 /* param5 */
  }                                    /* vals */
} ;                                    /* outputTlmPkt3_b ground */


const outputTlmPkt4_b unitTestModel_rtZoutputTlmPkt4_b = {
  {
    {
      0U, 0U }
    ,                                  /* StreamID */

    {
      0U, 0U }
    ,                                  /* Sequence */

    {
      0U, 0U }
    ,                                  /* PktLength */

    {
      0U, 0U, 0U, 0U }
    ,                                  /* TimeSec */

    {
      0U, 0U }
    /* TimeSubsec */
  },                                   /* TlmHdr */

  {
    0U,                                /* param1 */
    0U,                                /* param2 */
    0U,                                /* param3 */
    0U,                                /* param4 */
    0U                                 /* param5 */
  }                                    /* vals */
} ;                                    /* outputTlmPkt4_b ground */






/* Exported block states */
real_T evData_unitTestModel_122[5];    /* '<Root>/event1' */
real_T evData_unitTestModel_154[5];    /* '<Root>/event3' */
real_T evData_unitTestModel_155[5];    /* '<Root>/event4' */
real_T evData_unitTestModel_152[5];    /* '<Root>/event2' */
boolean_T cmsgFlag_unitTestModel_117;  /* '<Root>/sendOutputTlmPkt2' */
boolean_T cmsgFlag_unitTestModel_126;  /* '<Root>/sendOutputTlmPkt3' */
boolean_T fdcFlag_121;                 /* '<Root>/CFS_FDC_Event' */
boolean_T evFlag_unitTestModel_122;    /* '<Root>/event1' */
boolean_T evFlag_unitTestModel_154;    /* '<Root>/event3' */
boolean_T evFlag_unitTestModel_155;    /* '<Root>/event4' */
boolean_T evFlag_unitTestModel_152;    /* '<Root>/event2' */
boolean_T evFlag_unitTestModel_150;    /* '<Root>/event0' */

/* Exported data definition */

/* Definition of data with custom storage class cfsCmdMessage */
inputCmdPkt_b inputCmdPkt_s;

/* Definition of data with custom storage class cfsTlmMessage */
inputTlmPkt1_b inputTlmPkt1_s;
inputTlmPkt2_b inputTlmPkt2_s;
outputTlmPkt1_b outputTlmPkt1_s;
outputTlmPkt2_b outputTlmPkt2_s;
outputTlmPkt3_b outputTlmPkt3_s;
outputTlmPkt4_b outputTlmPkt4_s;

/* Block signals (default storage) */
B_unitTestModel_T unitTestModel_B;

/* Block states (default storage) */
DW_unitTestModel_T unitTestModel_DW;

/* Real-time model */
RT_MODEL_unitTestModel_T unitTestModel_M_;
RT_MODEL_unitTestModel_T *const unitTestModel_M = &unitTestModel_M_;
real_T rt_modd_snf(real_T u0, real_T u1)
{
  real_T y;
  boolean_T yEq;
  real_T q;
  y = u0;
  if (rtIsNaN(u0) || rtIsInf(u0) || (rtIsNaN(u1) || rtIsInf(u1))) {
    if (u1 != 0.0) {
      y = (rtNaN);
    }
  } else if (u0 == 0.0) {
    y = u1 * 0.0;
  } else {
    if (u1 != 0.0) {
      y = fmod(u0, u1);
      yEq = (y == 0.0);
      if ((!yEq) && (u1 > floor(u1))) {
        q = fabs(u0 / u1);
        yEq = (fabs(q - floor(q + 0.5)) <= DBL_EPSILON * q);
      }

      if (yEq) {
        y = u1 * 0.0;
      } else {
        if ((u0 < 0.0) != (u1 < 0.0)) {
          y += u1;
        }
      }
    }
  }

  return y;
}

/* Model step function */
void unitTestModel_step(void)
{
  uint16_T rtb_SeqCnt;
  uint8_T rtb_ExtractedBits;
  real_T tmp;

  /* S-Function (sfix_bitop): '<S11>/ExtractSeqCnt' incorporates:
   *  ArithShift: '<S15>/ShiftUpperBits'
   *  DataTypeConversion: '<S15>/Data Type Conversion8'
   *  Inport: '<Root>/inputCmdPkt'
   *  S-Function (sfix_bitop): '<S15>/ANDStreamBytes'
   */
  rtb_SeqCnt = (uint16_T)((inputCmdPkt_s.CmdHdr.Sequence[1] << 8 |
    inputCmdPkt_s.CmdHdr.Sequence[0]) & 16383);

  /* Switch: '<S8>/Switch' incorporates:
   *  Inport: '<Root>/inputCmdPkt'
   *  RelationalOperator: '<S10>/FixPt Relational Operator'
   *  UnitDelay: '<S10>/Delay Input1'
   *
   * Block description for '<S10>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if (rtb_SeqCnt != unitTestModel_DW.DelayInput1_DSTATE) {
    rtb_ExtractedBits = inputCmdPkt_s.arg1;
  } else {
    rtb_ExtractedBits = 0U;
  }

  /* End of Switch: '<S8>/Switch' */

  /* S-Function (sfix_bitop): '<S18>/ExtractBits' */
  rtb_ExtractedBits = (uint8_T)(rtb_ExtractedBits & 127);

  /* BusCreator: '<Root>/Bus Creator' incorporates:
   *  Constant: '<Root>/TlmHdr2'
   *  UnitDelay: '<S3>/Output'
   */
  unitTestModel_B.BusCreator.TlmHdr = unitTestModel_rtZCCSDS_TlmHdr;
  unitTestModel_B.BusCreator.outVal1 = unitTestModel_DW.Output_DSTATE;

  /* DataTypeConversion: '<Root>/Data Type Conversion' incorporates:
   *  Constant: '<Root>/Constant11'
   *  Math: '<Root>/Mod'
   *  Rounding: '<Root>/Floor'
   *  S-Function (cfs_gnc_time): '<Root>/CFS_GNC_Time1'
   */
  tmp = floor(rt_modd_snf(floor(((real_T)ECI_Step_TimeStamp.Seconds + ((real_T)
    ECI_Step_TimeStamp.Subseconds/4294967296.0))), 255.0));
  if (rtIsNaN(tmp) || rtIsInf(tmp)) {
    tmp = 0.0;
  } else {
    tmp = fmod(tmp, 256.0);
  }

  /* BusCreator: '<Root>/Bus Creator' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   */
  unitTestModel_B.BusCreator.outVal2 = (uint8_T)(tmp < 0.0 ? (int32_T)(uint8_T)
    -(int8_T)(uint8_T)-tmp : (int32_T)(uint8_T)tmp);
  unitTestModel_B.BusCreator.outVal3 = rtb_ExtractedBits;

  /* RelationalOperator: '<Root>/Relational Operator' incorporates:
   *  Constant: '<Root>/Constant'
   *  Inport: '<Root>/inputTlmPkt2'
   */
  unitTestModel_B.RelationalOperator = (inputTlmPkt2_s.inVal3 > (*param_TBL2).
    param3);

  /* S-Function (cfs_conditional_msg): '<Root>/sendOutputTlmPkt2' */
  cmsgFlag_unitTestModel_117 = unitTestModel_B.RelationalOperator;
  if (cmsgFlag_unitTestModel_117) {
    outputTlmPkt2_s = unitTestModel_B.BusCreator;
  }

  /* RelationalOperator: '<S2>/Compare' incorporates:
   *  Constant: '<S2>/Constant'
   */
  unitTestModel_B.Compare = (rtb_ExtractedBits == 1);

  /* BusCreator: '<Root>/Bus Creator2' incorporates:
   *  Constant: '<Root>/Constant2'
   *  Constant: '<Root>/TlmHdr3'
   */
  unitTestModel_B.BusCreator2.TlmHdr = unitTestModel_rtZCCSDS_TlmHdr;
  unitTestModel_B.BusCreator2.vals = (*param_TBL1);

  unitTestModel_B.BusCreator3.TlmHdr = unitTestModel_rtZCCSDS_TlmHdr;
  unitTestModel_B.BusCreator3.vals = (*param_TBL2);

  /* S-Function (cfs_conditional_msg): '<Root>/sendOutputTlmPkt3' */
  cmsgFlag_unitTestModel_126 = unitTestModel_B.Compare;
  if (cmsgFlag_unitTestModel_126) {
    outputTlmPkt3_s = unitTestModel_B.BusCreator2;
    outputTlmPkt4_s = unitTestModel_B.BusCreator3;
  }

  /* RelationalOperator: '<Root>/Relational Operator1' incorporates:
   *  Inport: '<Root>/inputTlmPkt2'
   */
  unitTestModel_B.RelationalOperator1 = (inputTlmPkt2_s.inVal1 >
    inputTlmPkt2_s.inVal2);

  /* S-Function (cfs_fdc): '<Root>/CFS_FDC_Event' */
  fdcFlag_121 = unitTestModel_B.RelationalOperator1;

  /* S-Function (cfs_gnc_time): '<Root>/CFS_GNC_Time' */
  unitTestModel_B.CFS_GNC_Time = ((real_T)ECI_Step_TimeStamp.Seconds + ((real_T)
    ECI_Step_TimeStamp.Subseconds/4294967296.0));

  evData_unitTestModel_122[0] = unitTestModel_B.CFS_GNC_Time;

  /* S-Function (cfs_event): '<Root>/event1' incorporates:
   *  Constant: '<Root>/sendAlways1'
   */
  evFlag_unitTestModel_122 = true;

  /* S-Function (cfs_event): '<Root>/event3' incorporates:
   *  Constant: '<Root>/Constant1'
   *  Constant: '<Root>/Constant3'
   *  Constant: '<Root>/Constant4'
   *  Constant: '<Root>/sendAlways3'
   */
  evFlag_unitTestModel_154 = true;
  evData_unitTestModel_154[0] = 2.0;
  evData_unitTestModel_154[1] = 2.0;

  /* S-Function (cfs_event): '<Root>/event4' incorporates:
   *  Constant: '<Root>/Constant10'
   *  Constant: '<Root>/Constant7'
   *  Constant: '<Root>/Constant8'
   *  Constant: '<Root>/Constant9'
   *  Constant: '<Root>/sendAlways4'
   */
  evFlag_unitTestModel_155 = true;
  evData_unitTestModel_155[0] = 1.0;
  evData_unitTestModel_155[1] = 3.0;
  evData_unitTestModel_155[2] = 3.0;

  /* S-Function (cfs_event): '<Root>/event2' incorporates:
   *  Constant: '<Root>/Constant5'
   *  Constant: '<Root>/Constant6'
   *  Constant: '<Root>/sendAlways2'
   */
  evFlag_unitTestModel_152 = true;
  evData_unitTestModel_152[0] = 2.0;

  /* S-Function (cfs_event): '<Root>/event0' incorporates:
   *  Constant: '<Root>/sendAlways0'
   */
  evFlag_unitTestModel_150 = true;

  /* Switch: '<S21>/FixPt Switch' incorporates:
   *  Constant: '<S20>/FixPt Constant'
   *  Sum: '<S20>/FixPt Sum1'
   *  UnitDelay: '<S3>/Output'
   */
  unitTestModel_DW.Output_DSTATE++;

  /* BusCreator: '<Root>/Bus Creator1' incorporates:
   *  Constant: '<Root>/TlmHdr1'
   *  Inport: '<Root>/inputTlmPkt1'
   */
  outputTlmPkt1_s.TlmHdr = unitTestModel_rtZCCSDS_TlmHdr;
  outputTlmPkt1_s.outVal1 = inputTlmPkt1_s.inVal1;
  outputTlmPkt1_s.outVal2 = inputTlmPkt1_s.inVal2;
  outputTlmPkt1_s.outVal3 = inputTlmPkt1_s.inVal3;
  outputTlmPkt1_s.outVal4 = inputTlmPkt1_s.inVal4;

  /* Update for UnitDelay: '<S10>/Delay Input1'
   *
   * Block description for '<S10>/Delay Input1':
   *
   *  Store in Global RAM
   */
  unitTestModel_DW.DelayInput1_DSTATE = rtb_SeqCnt;
}

/* Model initialize function */
void unitTestModel_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize error status */
  rtmSetErrorStatus(unitTestModel_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &unitTestModel_B), 0,
                sizeof(B_unitTestModel_T));

  /* custom signals */
  outputTlmPkt4_s = unitTestModel_rtZoutputTlmPkt4_b;
  outputTlmPkt3_s = unitTestModel_rtZoutputTlmPkt3_b;
  outputTlmPkt2_s = unitTestModel_rtZoutputTlmPkt2_b;
  outputTlmPkt1_s = unitTestModel_rtZoutputTlmPkt1_b;

  /* states (dwork) */
  (void) memset((void *)&unitTestModel_DW, 0,
                sizeof(DW_unitTestModel_T));

  /* exported global states */
  (void) memset(&evData_unitTestModel_122, 0,
                5U*sizeof(real_T));
  (void) memset(&evData_unitTestModel_154, 0,
                5U*sizeof(real_T));
  (void) memset(&evData_unitTestModel_155, 0,
                5U*sizeof(real_T));
  (void) memset(&evData_unitTestModel_152, 0,
                5U*sizeof(real_T));
  cmsgFlag_unitTestModel_117 = false;
  cmsgFlag_unitTestModel_126 = false;
  fdcFlag_121 = false;
  evFlag_unitTestModel_122 = false;
  evFlag_unitTestModel_154 = false;
  evFlag_unitTestModel_155 = false;
  evFlag_unitTestModel_152 = false;
  evFlag_unitTestModel_150 = false;

  /* external inputs */
  inputTlmPkt1_s = unitTestModel_rtZinputTlmPkt1_b;
  inputTlmPkt2_s = unitTestModel_rtZinputTlmPkt2_b;
  inputCmdPkt_s = unitTestModel_rtZinputCmdPkt_b;

  /* InitializeConditions for S-Function (cfs_conditional_msg): '<Root>/sendOutputTlmPkt2' */
  cmsgFlag_unitTestModel_117 = false;

  /* InitializeConditions for S-Function (cfs_conditional_msg): '<Root>/sendOutputTlmPkt3' */
  cmsgFlag_unitTestModel_126 = false;
}

/* Model terminate function */
void unitTestModel_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
