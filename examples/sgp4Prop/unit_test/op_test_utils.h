#ifndef OP_TEST_UTILS_H
#define OP_TEST_UTILS_H

#include "cfe.h"
#include "utlist.h"

/* Setup message framework */
#define UT_CFE_SB_MAX_PIPES 32
/* Pipe IDs */
#define  CMDPIPE   0
#define  DATAPIPE  1

typedef struct {
    uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];
} NoArgsCmd_t;

typedef struct {
    char                PipeName[OS_MAX_API_NAME];
    UtListHead_t        MsgQueue;
    boolean             InUse;
} Ut_CFE_SB_PipeTableEntry_t;

CFE_TIME_SysTime_t Spoof_CFE_TIME_GetTime(void);
void OP_Test_SetTime(uint32 seconds, uint16 subseconds);
void OP_Test_Setup(void);
void OP_Test_SendTick(void);
void OP_Test_TearDown(void);


#endif