
#ifndef TLE_VALIDATION_TEST_H
#define TLE_VALIDATION_TEST_H

/* UT framework includes */
#include "common_types.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "utassert.h"
#include "uttest.h"

/* Test includes */
#include "op_test_utils.h"

/* Code-under-test includes */
#include "eci_app.h"
#include "eci_app_event.h"
#include "fsw/eci_inc/sgp4Wrapper_test.h"
#include "fsw/platform_inc/op_app_msgids.h"

/* Provide prototypes */
void op_AppMain(void);

void OP_Test_digitToInt(void);
void OP_Test_computeTLEChecksum(void);
void OP_Test_validateTLE(void);
void OP_Test_tableValidate(void);
void OP_Test_getTime(void);
void OP_Test_propagateOrbit(void);

#endif
