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
#include "uttest.h"
#include "utassert.h"

/* Test includes */
#include "op_test_utils.h"
#include "app_msgids.h"
#include "sgp4Wrapper_test.h"

/* Global test data */
CFE_TIME_SysTime_t UT_Time;
NoArgsCmd_t       TickMsg;
tle_lines_t test_tle_tbl;

/* CFE_TIME_GetTime() Stub */
CFE_TIME_SysTime_t Spoof_CFE_TIME_GetTime(void){
   UtPrintf("Returning time %d %d\n", UT_Time.Seconds, UT_Time.Subseconds);
   return UT_Time;

} /* End of SA_Test_CFE_TIME_GetTime */

void OP_Test_SetTime(uint32 seconds, uint16 subseconds){
	UT_Time.Seconds = seconds;
	UT_Time.Subseconds = subseconds;
}

void OP_Test_Setup(void){

	Ut_CFE_EVS_Reset();
	Ut_CFE_FS_Reset();
	Ut_CFE_TIME_Reset();
	Ut_CFE_TBL_Reset();
	Ut_CFE_SB_Reset();
	Ut_CFE_ES_Reset();
	Ut_OSAPI_Reset();
	Ut_OSFILEAPI_Reset();

    /* Initialize tick message in case user wants to run app during this test */
    CFE_SB_InitMsg(&TickMsg, OP_TICK_MID, sizeof(NoArgsCmd_t), TRUE);

    /* Hook get time so that user can set time received by app during test */
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX, Spoof_CFE_TIME_GetTime);

    /* Add table so app will initalize */
    Ut_CFE_TBL_AddTable("/mram/op_tle.tbl", &test_tle_tbl);

}

void OP_Test_SendTick(void){
	extern Ut_CFE_SB_PipeTableEntry_t  PipeTable[UT_CFE_SB_MAX_PIPES];

    UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&TickMsg), 0);
}

void * OP_Test_RunAppAndGetPkt(int pktNum){

    void* OutPacket;

    /* Send scheduler message to trigger app */
    OP_Test_SendTick();

    /* Run the app code */
    op_AppMain();

    /* Get the output and verify vaid pkt */
    OutPacket = Ut_CFE_SB_FindPacket(OP_PROPSTATE_MID, pktNum);

    if (OutPacket == NULL){
    	UtAssert_True(TRUE, "Return message pointer is not NULL");

    	/* End test otherwise validations which use this data will segfault*/
    	UtAssert_EndTest();
    }

	return (propState_t*) OutPacket;

}

void OP_Test_TearDown(void){
}
