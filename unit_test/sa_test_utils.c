/* UT Test includes */
#include "cfe.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_time_stubs.h"
#include "ut_osfileapi_stubs.h"

#include "ut_osapi_stubs.h"

#include "paramTable1_b.h"
#include "paramTable2_b.h"
#include "sa_test_utils.h"

/* Global Data */
extern paramTable1_b UT_param_TBL1;
extern paramTable2_b UT_param_TBL2;
extern paramTable1_b UT_param_TBL3;
extern paramTable2_b UT_param_TBL4;
extern paramTable1_b UT_param_TBL5;
extern paramTable2_b UT_param_TBL6;

CFE_SB_MsgId_t MsgIdStop;
boolean        RcvMsgStop;

static CFE_TIME_SysTime_t UT_Time;

static int UT_Iteration = 0;

uint32 UT_Increment;

paramTable1_b *UT_param_ptr1;
paramTable2_b *UT_param_ptr2;

/*
 * Setup function run before each unit test.  Initializes test
 * environment to default state.
 */
void SA_Test_Setup(void)
{

    /*CFE_PSP_MemSet(&GPSIO_AppData, 0, sizeof(GPSIO_AppData_t));*/

    Ut_CFE_EVS_Reset();
    Ut_CFE_FS_Reset();
    Ut_CFE_TIME_Reset();
    Ut_CFE_TBL_Reset();
    Ut_CFE_SB_Reset();
    Ut_CFE_ES_Reset();
    Ut_OSAPI_Reset();
    Ut_OSFILEAPI_Reset();

} /* End of SA_Test_Setup() */

/*
 * Tear down function run after each unit test.
 */
void SA_Test_TearDown(void) {} /* End of SA_Test_TearDown */

/* Reset Iterations for SA_Test_CFE_TBL_Manage */
void SA_Test_Iteration_Reset(void)
{

    UT_Iteration = 0;

    Ut_CFE_TBL_AddTable(CAT_TBL_PATH("param_TBL3.tbl"), &UT_param_TBL3);
    Ut_CFE_TBL_AddTable(CAT_TBL_PATH("param_TBL4.tbl"), &UT_param_TBL4);
    Ut_CFE_TBL_AddTable(CAT_TBL_PATH("param_TBL5.tbl"), &UT_param_TBL5);
    Ut_CFE_TBL_AddTable(CAT_TBL_PATH("param_TBL6.tbl"), &UT_param_TBL6);

} /* End of SA_Test_Iteration_Reset */

/* Reset parameters for SA_Test_CFE_SB_RcvMsg */
void set_SA_Test_CFE_SB_RcvMsg(CFE_SB_MsgId_t MsgId)
{

    MsgIdStop  = MsgId;
    RcvMsgStop = FALSE;

} /* End of set_SA_Test_CFE_SB_RcvMsg */

/* When checking for messages on the data pipe, it will grab messages up until
 * MsgIdStop received */
int32 SA_Test_CFE_SB_RcvMsg(CFE_SB_MsgPtr_t *BufPtr, CFE_SB_PipeId_t PipeId, int32 TimeOut)
{

    int32 RetStat;

    if (RcvMsgStop && PipeId == 1)
    {
        if (TimeOut == CFE_SB_POLL)
            RetStat = CFE_SB_NO_MESSAGE;
        else
        {
            RetStat = CFE_SB_TIME_OUT;
        }

        RcvMsgStop = FALSE;
    }
    else
    {
        RetStat = Ut_CFE_SB_RcvMsgHook(BufPtr, PipeId, TimeOut);

        if (Ut_CFE_SB_GetMsgIdHook(*BufPtr) == MsgIdStop)
        {
            RcvMsgStop = TRUE;
        }
    }

    return RetStat;

} /* End of SA_Test_CFE_SB_RcvMsg */

/* CFE_TBL_Manage Stub */
int32 SA_Test_CFE_TBL_Manage(CFE_TBL_Handle_t TblHandle)
{

    if (UT_Iteration == 0 || UT_Iteration == 1)
    {
        UT_Iteration++;
        return CFE_SUCCESS;
    }

    /* Table Validation will Fail */
    if (UT_Iteration == 2 || UT_Iteration == 3)
    {
        if (TblHandle == 0)
        {
            Ut_CFE_TBL_LoadHook(TblHandle, CFE_TBL_SRC_FILE, CAT_TBL_PATH("param_TBL3.tbl"));
        }
        else
        {
            Ut_CFE_TBL_LoadHook(TblHandle, CFE_TBL_SRC_FILE, CAT_TBL_PATH("param_TBL4.tbl"));
        }
    }

    /* Table Validation will Pass */
    if (UT_Iteration == 4 || UT_Iteration == 5)
    {
        if (TblHandle == 0)
        {
            Ut_CFE_TBL_LoadHook(TblHandle, CFE_TBL_SRC_FILE, CAT_TBL_PATH("param_TBL5.tbl"));
        }
        else
        {
            Ut_CFE_TBL_LoadHook(TblHandle, CFE_TBL_SRC_FILE, CAT_TBL_PATH("param_TBL6.tbl"));
        }
    }

    UT_Iteration++;
    return CFE_SUCCESS;

} /* End of SA_Test_CFE_TBL_Manage */

/* Will set cFE Time */
void SA_Test_set_time(uint32 Sec, uint32 Subsec, uint32 increment)
{

    UT_Time.Seconds    = Sec;
    UT_Time.Subseconds = Subsec;

    UT_Increment = increment;

} /* End of SA_Test_set_time */

/* CFE_TIME_GetTime() Stub */
CFE_TIME_SysTime_t SA_Test_CFE_TIME_GetTime(void)
{
    CFE_TIME_SysTime_t Ret_Time;

    Ret_Time = UT_Time;

    UT_Time.Seconds += UT_Increment;

    return Ret_Time;

} /* End of SA_Test_CFE_TIME_GetTime */

/* CFE_ES_ExitApp() Stub */
void SA_Test_CFE_ES_ExitApp(uint32 ExitStatus)
{

    CFE_EVS_SendEvent(900, CFE_EVS_INFORMATION, "UT App Exits");

} /* End of SA_Test_CFE_ES_ExitApp */

/************************/
/*  End of File Comment */
/************************/
