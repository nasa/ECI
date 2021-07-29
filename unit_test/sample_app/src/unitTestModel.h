/*
 * File: unitTestModel.h
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

#ifndef RTW_HEADER_unitTestModel_h_
#define RTW_HEADER_unitTestModel_h_
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <string.h>
#ifndef unitTestModel_COMMON_INCLUDES_
#define unitTestModel_COMMON_INCLUDES_
#include "eci_app.h"
#include "rtwtypes.h"
#endif /* unitTestModel_COMMON_INCLUDES_ */

#include "eci_app.h" /* CSL Header file */
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#include "unitTestModel_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm) ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val) ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct
{
    outputTlmPkt4_b BusCreator3;
    outputTlmPkt3_b BusCreator2;         /* '<Root>/Bus Creator2' */
    outputTlmPkt2_b BusCreator;          /* '<Root>/Bus Creator' */
    real_T          CFS_GNC_Time;        /* '<Root>/CFS_GNC_Time' */
    boolean_T       RelationalOperator;  /* '<Root>/Relational Operator' */
    boolean_T       Compare;             /* '<S2>/Compare' */
    boolean_T       RelationalOperator1; /* '<Root>/Relational Operator1' */
} B_unitTestModel_T;

/* Block states (default storage) for system '<Root>' */
typedef struct
{
    int32_T  sendOutputTlmPkt2_busSize; /* '<Root>/sendOutputTlmPkt2' */
    int32_T  sendOutputTlmPkt3_busSize; /* '<Root>/sendOutputTlmPkt3' */
    uint16_T DelayInput1_DSTATE;        /* '<S10>/Delay Input1' */
    uint8_T  Output_DSTATE;             /* '<S3>/Output' */
} DW_unitTestModel_T;

/* Constant parameters (default storage) */
typedef struct
{
    /* Expression: event_mask
     * Referenced by: '<Root>/event1'
     */
    uint32_T event1_event_mask;

    /* Expression: event_mask
     * Referenced by: '<Root>/event3'
     */
    uint32_T event3_event_mask;

    /* Expression: event_mask
     * Referenced by: '<Root>/event4'
     */
    uint32_T event4_event_mask;

    /* Expression: event_mask
     * Referenced by: '<Root>/event2'
     */
    uint32_T event2_event_mask;

    /* Expression: event_mask
     * Referenced by: '<Root>/event0'
     */
    uint32_T event0_event_mask;

    /* Pooled Parameter (Expression: )
     * Referenced by:
     *   '<Root>/CFS_FDC_Event'
     *   '<Root>/event0'
     */
    uint8_T pooled8;

    /* Computed Parameter: event1_event_id
     * Referenced by: '<Root>/event1'
     */
    uint8_T event1_event_id;

    /* Computed Parameter: event1_event_type
     * Referenced by: '<Root>/event1'
     */
    uint8_T event1_event_type;

    /* Expression: fmt_string
     * Referenced by: '<Root>/event1'
     */
    uint8_T event1_event_fmtstring[20];

    /* Computed Parameter: event3_event_id
     * Referenced by: '<Root>/event3'
     */
    uint8_T event3_event_id;

    /* Computed Parameter: event3_event_type
     * Referenced by: '<Root>/event3'
     */
    uint8_T event3_event_type;

    /* Expression: fmt_string
     * Referenced by: '<Root>/event3'
     */
    uint8_T event3_event_fmtstring[40];

    /* Computed Parameter: event4_event_id
     * Referenced by: '<Root>/event4'
     */
    uint8_T event4_event_id;

    /* Pooled Parameter (Expression: )
     * Referenced by:
     *   '<Root>/event0'
     *   '<Root>/event4'
     */
    uint8_T pooled9;

    /* Expression: fmt_string
     * Referenced by: '<Root>/event4'
     */
    uint8_T event4_event_fmtstring[31];

    /* Computed Parameter: event2_event_id
     * Referenced by: '<Root>/event2'
     */
    uint8_T event2_event_id;

    /* Computed Parameter: event2_event_type
     * Referenced by: '<Root>/event2'
     */
    uint8_T event2_event_type;

    /* Expression: fmt_string
     * Referenced by: '<Root>/event2'
     */
    uint8_T event2_event_fmtstring[25];

    /* Expression: fmt_string
     * Referenced by: '<Root>/event0'
     */
    uint8_T event0_event_fmtstring[19];
} ConstP_unitTestModel_T;

/* Real-time Model Data Structure */
struct tag_RTM_unitTestModel_T
{
    const char_T *volatile errorStatus;
};

/* Block signals (default storage) */
extern B_unitTestModel_T unitTestModel_B;

/* Block states (default storage) */
extern DW_unitTestModel_T unitTestModel_DW;

/* External data declarations for dependent source files */
extern const CCSDS_TlmHdr    unitTestModel_rtZCCSDS_TlmHdr;    /* CCSDS_TlmHdr ground */
extern const inputTlmPkt1_b  unitTestModel_rtZinputTlmPkt1_b;  /* inputTlmPkt1_b ground */
extern const inputTlmPkt2_b  unitTestModel_rtZinputTlmPkt2_b;  /* inputTlmPkt2_b ground */
extern const inputCmdPkt_b   unitTestModel_rtZinputCmdPkt_b;   /* inputCmdPkt_b ground */
extern const outputTlmPkt2_b unitTestModel_rtZoutputTlmPkt2_b; /* outputTlmPkt2_b ground */
extern const outputTlmPkt1_b unitTestModel_rtZoutputTlmPkt1_b; /* outputTlmPkt1_b ground */
extern const outputTlmPkt3_b unitTestModel_rtZoutputTlmPkt3_b; /* outputTlmPkt3_b ground */

/* Constant parameters (default storage) */
extern const ConstP_unitTestModel_T unitTestModel_ConstP;

/*
 * Exported States
 *
 * Note: Exported states are block states with an exported global
 * storage class designation.  Code generation will declare the memory for these
 * states and exports their symbols.
 *
 */
extern real_T    evData_unitTestModel_122[5]; /* '<Root>/event1' */
extern real_T    evData_unitTestModel_154[5]; /* '<Root>/event3' */
extern real_T    evData_unitTestModel_155[5]; /* '<Root>/event4' */
extern real_T    evData_unitTestModel_152[5]; /* '<Root>/event2' */
extern boolean_T cmsgFlag_unitTestModel_117;  /* '<Root>/sendOutputTlmPkt2' */
extern boolean_T cmsgFlag_unitTestModel_126;  /* '<Root>/sendOutputTlmPkt3' */
extern boolean_T fdcFlag_121;                 /* '<Root>/CFS_FDC_Event' */
extern boolean_T evFlag_unitTestModel_122;    /* '<Root>/event1' */
extern boolean_T evFlag_unitTestModel_154;    /* '<Root>/event3' */
extern boolean_T evFlag_unitTestModel_155;    /* '<Root>/event4' */
extern boolean_T evFlag_unitTestModel_152;    /* '<Root>/event2' */
extern boolean_T evFlag_unitTestModel_150;    /* '<Root>/event0' */

/* Model entry point functions */
extern void unitTestModel_initialize(void);
extern void unitTestModel_step(void);
extern void unitTestModel_terminate(void);

/* Exported data declaration */

/* Declaration of data with custom storage class cfsCmdMessage */
extern inputCmdPkt_b inputCmdPkt_s;

/* Declaration of data with custom storage class cfsTlmMessage */
extern inputTlmPkt1_b  inputTlmPkt1_s;
extern inputTlmPkt2_b  inputTlmPkt2_s;
extern outputTlmPkt1_b outputTlmPkt1_s;
extern outputTlmPkt2_b outputTlmPkt2_s;
extern outputTlmPkt3_b outputTlmPkt3_s;
extern outputTlmPkt4_b outputTlmPkt4_s;

/* Real-time Model object */
extern RT_MODEL_unitTestModel_T *const unitTestModel_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S11>/AddPktLenOffset' : Unused code path elimination
 * Block '<S11>/Data Type Conversion1' : Unused code path elimination
 * Block '<S11>/Data Type Conversion2' : Unused code path elimination
 * Block '<S11>/Data Type Conversion3' : Unused code path elimination
 * Block '<S11>/Data Type Conversion5' : Unused code path elimination
 * Block '<S11>/Data Type Conversion7' : Unused code path elimination
 * Block '<S11>/ExtractAPID' : Unused code path elimination
 * Block '<S11>/ExtractCCSDSVersion' : Unused code path elimination
 * Block '<S11>/ExtractPktTypeFlag' : Unused code path elimination
 * Block '<S11>/ExtractSegFlag' : Unused code path elimination
 * Block '<S13>/Compare' : Unused code path elimination
 * Block '<S13>/Constant' : Unused code path elimination
 * Block '<S11>/LengthZeroSwitch' : Unused code path elimination
 * Block '<S11>/PktLenOffset' : Unused code path elimination
 * Block '<S14>/ANDStreamBytes' : Unused code path elimination
 * Block '<S14>/Data Type Conversion8' : Unused code path elimination
 * Block '<S14>/ShiftUpperBits' : Unused code path elimination
 * Block '<S16>/ANDStreamBytes' : Unused code path elimination
 * Block '<S16>/Data Type Conversion8' : Unused code path elimination
 * Block '<S16>/ShiftUpperBits' : Unused code path elimination
 * Block '<S11>/SecondaryHeaderFlag' : Unused code path elimination
 * Block '<S11>/ShiftCCSDSVer' : Unused code path elimination
 * Block '<S11>/ShiftPktTypeFlag' : Unused code path elimination
 * Block '<S11>/ShiftSecHdrFlag' : Unused code path elimination
 * Block '<S11>/ShiftSegFlag' : Unused code path elimination
 * Block '<S9>/Data Type Conversion1' : Unused code path elimination
 * Block '<S17>/ExtractBits' : Unused code path elimination
 * Block '<S17>/ShiftBits' : Unused code path elimination
 * Block '<S19>/ExtractBits' : Unused code path elimination
 * Block '<S19>/ShiftBits' : Unused code path elimination
 * Block '<S3>/Data Type Propagation' : Unused code path elimination
 * Block '<S20>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S21>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<S11>/Data Type Conversion6' : Eliminate redundant data type
 * conversion Block '<S9>/Data Type Conversion5' : Eliminate redundant data type
 * conversion Block '<S18>/ShiftBits' : Eliminated trivial shift
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'unitTestModel'
 * '<S1>'   : 'unitTestModel/CmdPreProcess'
 * '<S2>'   : 'unitTestModel/Compare To Constant'
 * '<S3>'   : 'unitTestModel/Counter Limited'
 * '<S4>'   : 'unitTestModel/NameChange'
 * '<S5>'   : 'unitTestModel/NameChange1'
 * '<S6>'   : 'unitTestModel/NameChange2'
 * '<S7>'   : 'unitTestModel/NameChange3'
 * '<S8>'   : 'unitTestModel/CmdPreProcess/CmdOrNoCmd'
 * '<S9>'   : 'unitTestModel/CmdPreProcess/ExtractCCSDSCmdSecHeader'
 * '<S10>'  : 'unitTestModel/CmdPreProcess/CmdOrNoCmd/DetectChange'
 * '<S11>'  : 'unitTestModel/CmdPreProcess/CmdOrNoCmd/ExtractCCSDSPriHeader'
 * '<S12>'  : 'unitTestModel/CmdPreProcess/CmdOrNoCmd/NoCmd'
 * '<S13>'  :
 * 'unitTestModel/CmdPreProcess/CmdOrNoCmd/ExtractCCSDSPriHeader/IfLenZero'
 * '<S14>'  :
 * 'unitTestModel/CmdPreProcess/CmdOrNoCmd/ExtractCCSDSPriHeader/RecombineLength'
 * '<S15>'  :
 * 'unitTestModel/CmdPreProcess/CmdOrNoCmd/ExtractCCSDSPriHeader/RecombineSequence'
 * '<S16>'  :
 * 'unitTestModel/CmdPreProcess/CmdOrNoCmd/ExtractCCSDSPriHeader/RecombineStreamID'
 * '<S17>'  :
 * 'unitTestModel/CmdPreProcess/ExtractCCSDSCmdSecHeader/ExtractChecksumBits'
 * '<S18>'  :
 * 'unitTestModel/CmdPreProcess/ExtractCCSDSCmdSecHeader/ExtractFcnCodeBits'
 * '<S19>'  :
 * 'unitTestModel/CmdPreProcess/ExtractCCSDSCmdSecHeader/ExtractReservedBits'
 * '<S20>'  : 'unitTestModel/Counter Limited/Increment Real World'
 * '<S21>'  : 'unitTestModel/Counter Limited/Wrap To Zero'
 */
#endif /* RTW_HEADER_unitTestModel_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
