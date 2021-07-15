/*
 * File: buses.h
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

#ifndef RTW_HEADER_buses_h_
#define RTW_HEADER_buses_h_
#include "paramTable1_b.h"
#include "paramTable2_b.h"
#include "rtwtypes.h"

typedef struct
{
    /* Description */
    uint8_T StreamID[2];

    /* Description */
    uint8_T Sequence[2];

    /* Description */
    uint8_T PktLength[2];

    /* Description */
    uint8_T TimeSec[4];

    /* Description */
    uint8_T TimeSubsec[2];
} CCSDS_TlmHdr;

typedef struct
{
    /* Description */
    uint8_T StreamID[2];

    /* Description */
    uint8_T Sequence[2];

    /* Description */
    uint8_T PktLength[2];

    /* Description */
    uint8_T FcnCode;

    /* Description */
    uint8_T ChkSum;
} CCSDS_CmdHdr;

typedef struct
{
    /* Description */
    CCSDS_TlmHdr TlmHdr;

    /* Description */
    uint8_T inVal1;

    /* Description */
    uint8_T inVal2;

    /* Description */
    uint8_T inVal3;

    /* Description */
    uint8_T inVal4;
} inputTlmPkt1_b;

typedef struct
{
    /* Description */
    CCSDS_TlmHdr TlmHdr;

    /* Description */
    uint8_T inVal1;

    /* Description */
    uint8_T inVal2;

    /* Description */
    uint8_T inVal3;
} inputTlmPkt2_b;

typedef struct
{
    /* Description */
    CCSDS_CmdHdr CmdHdr;

    /* Description */
    uint8_T arg1;
} inputCmdPkt_b;

typedef struct
{
    /* Description */
    CCSDS_TlmHdr TlmHdr;

    /* Description */
    uint8_T outVal1;

    /* Description */
    uint8_T outVal2;

    /* Description */
    uint8_T outVal3;
} outputTlmPkt2_b;

typedef struct
{
    /* Description */
    CCSDS_TlmHdr TlmHdr;

    /* Description */
    uint8_T outVal1;

    /* Description */
    uint8_T outVal2;

    /* Description */
    uint8_T outVal3;

    /* Description */
    uint8_T outVal4;
} outputTlmPkt1_b;

typedef struct
{
    /* Description */
    CCSDS_TlmHdr TlmHdr;

    /* Description */
    paramTable1_b vals;
} outputTlmPkt3_b;

typedef struct
{
    /* Description */
    CCSDS_TlmHdr TlmHdr;

    /* Description */
    paramTable2_b vals;
} outputTlmPkt4_b;

#endif /* RTW_HEADER_buses_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
