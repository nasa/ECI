/*
 * File: paramTable1.c
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

#include "eci_tbl_if.h"
#include "unitTestModel_types.h"

paramTable1_b UT_param_TBL3 = {1U, 2U, 13U, 4U, 5U};

#define PARAM_TBL3_TBL_NAME_LEN (11) /* sizeof("param_TBL1") */
#if PARAM_TBL3_TBL_NAME_LEN > ECI_PARAM_TBL_MAX_NAME_LEN
#error Table name param_TBL1 must be less than ECI_PARAM_TBL_MAX_NAME_LEN characters
#endif

/*
 ** The macro below identifies:
 **    1) the data structure type to use as the table image format
 **    2) the name of the table to be placed into the CFS Table File Header
 **    3) a brief description of the contents of the file image
 **    4) the desired name of the table image binary file that is CFS compatible
 */
ECI_TBL_FILEDEF(CFE_TBL_FileDef, UT_param_TBL3, unitTestModel.param_TBL3, unitTestModel param_TBL3, paramTable3.tbl)
/*
 * File trailer for generated code.
 *
 * [EOF]
 */
