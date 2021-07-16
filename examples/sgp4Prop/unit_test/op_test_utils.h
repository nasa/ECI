#ifndef OP_TEST_UTILS_H
#define OP_TEST_UTILS_H

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
#include "cfe.h"
#include "utlist.h"

/* File Specific Includes */
#include "eci_app_cfg.h"
#include "unit_test/op_test_utils.h"
#include "fsw/platform_inc/op_app_msgids.h"
#include "fsw/eci_inc/sgp4Wrapper_test.h"

#define CAT_TBL_PATH(PATH) PARAM_TBL_PATH_PREFIX PATH

/* Setup message framework */
#define UT_CFE_SB_MAX_PIPES 32
/* Pipe IDs */
#define CMDPIPE  0
#define DATAPIPE 1

typedef struct
{
    uint8 CmdHeader[CFE_SB_CMD_HDR_SIZE];
} NoArgsCmd_t;

typedef struct
{
    char         PipeName[OS_MAX_API_NAME];
    UtListHead_t MsgQueue;
    boolean      InUse;
} Ut_CFE_SB_PipeTableEntry_t;

CFE_TIME_SysTime_t Spoof_CFE_TIME_GetTime(void);
void               OP_Test_SetTime(uint32 seconds, uint16 subseconds);
void               OP_Test_Setup(void);
void               OP_Test_SendTick(void);
void               OP_Test_TearDown(void);

#endif
