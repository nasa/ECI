#ifndef _sa_test_utils_
#define _sa_test_utils_

/* UT Test includes */
#include "cfe.h"
#include "eci_app_cfg.h"

#define CAT_TBL_PATH(PATH) PARAM_TBL_PATH_PREFIX PATH

void SA_Test_Setup(void);
void SA_Test_TearDown(void);

void SA_Test_CFE_ES_ExitApp(uint32 ExitStatus);

void SA_Test_set_time(uint32 Sec, uint32 Subsec, uint32 increment);

CFE_TIME_SysTime_t SA_Test_CFE_TIME_GetTime(void);

void SA_Test_Iteration_Reset(void);

int32 SA_Test_CFE_TBL_Manage(CFE_TBL_Handle_t TblHandle);

void set_SA_Test_CFE_SB_RcvMsg(CFE_SB_MsgId_t MsgId);

int32 SA_Test_CFE_SB_RcvMsg(CFE_SB_MsgPtr_t *BufPtr, CFE_SB_PipeId_t PipeId, int32 TimeOut);

#endif /* End of _sa_test_utils_ */

/************************/
/*  End of File Comment */
/************************/
