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

#include "unitTestModel_types.h"
#include "eci_tbl_if.h"

paramTable1_b UT_param_TBL5 = {2U, 1U, 1U, 1U, 1U};

#define PARAM_TBL5_TBL_NAME_LEN (11) /* sizeof("param_TBL1") */
#if PARAM_TBL5_TBL_NAME_LEN > ECI_PARAM_TBL_MAX_NAME_LEN
#error Table name param_TBL5 must be less than ECI_PARAM_TBL_MAX_NAME_LEN characters
#endif

/*
 ** The macro below identifies:
 **    1) the data structure type to use as the table image format
 **    2) the name of the table to be placed into the CFS Table File Header
 **    3) a brief description of the contents of the file image
 **    4) the desired name of the table image binary file that is CFS compatible
 */
ECI_TBL_FILEDEF(CFE_TBL_FileDef, UT_param_TBL5, unitTestModel.param_TBL5, unitTestModel param_TBL5, paramTable5.tbl)
/*
 * File trailer for generated code.
 *
 * [EOF]
 */
