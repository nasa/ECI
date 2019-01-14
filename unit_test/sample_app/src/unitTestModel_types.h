/*
 * File: unitTestModel_types.h
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

#ifndef RTW_HEADER_unitTestModel_types_h_
#define RTW_HEADER_unitTestModel_types_h_
#include "rtwtypes.h"
#include "buses.h"
#include "paramTable2_b.h"
#ifndef DEFINED_TYPEDEF_FOR_CFEEventType_
#define DEFINED_TYPEDEF_FOR_CFEEventType_

typedef enum {
  CFEEventType_Debug = 1,              /* Default value */
  CFEEventType_Info,
  CFEEventType_Error,
  CFEEventType_Critical
} CFEEventType;

#endif

#ifndef DEFINED_TYPEDEF_FOR_CFEEventMask_
#define DEFINED_TYPEDEF_FOR_CFEEventMask_

typedef enum {
  CFEEventMask_NoFilter = 0,           /* Default value */
  CFEEventMask_First1 = 65535,
  CFEEventMask_First2 = 65534,
  CFEEventMask_First4 = 65532,
  CFEEventMask_First8 = 65528,
  CFEEventMask_First16 = 65520,
  CFEEventMask_First32 = 65504,
  CFEEventMask_First64 = 65472,
  CFEEventMask_EveryOther = 1,
  CFEEventMask_EveryTwo,
  CFEEventMask_EveryFour
} CFEEventMask;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_unitTestModel_T RT_MODEL_unitTestModel_T;

#endif                                 /* RTW_HEADER_unitTestModel_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
