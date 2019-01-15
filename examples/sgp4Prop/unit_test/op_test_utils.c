/* Test framework includes */
#include "cfe.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_osapi_stubs.h"
#include "ut_osfileapi_stubs.h"

/* Test includes */
#include "op_test_utils.h"


/* Global test data */
CFE_TIME_SysTime_t UT_Time;


/* CFE_TIME_GetTime() Stub */
CFE_TIME_SysTime_t Spoof_CFE_TIME_GetTime(void){

   return UT_Time;

} /* End of SA_Test_CFE_TIME_GetTime */

void OP_Test_Setup(void){

	Ut_CFE_EVS_Reset();
	Ut_CFE_FS_Reset();
	Ut_CFE_TIME_Reset();
	Ut_CFE_TBL_Reset();
	Ut_CFE_SB_Reset();
	Ut_CFE_ES_Reset();
	Ut_OSAPI_Reset();
	Ut_OSFILEAPI_Reset();

}

void OP_Test_TearDown(void){
}
