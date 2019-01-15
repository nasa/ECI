/* UT framework includes */
#include "uttest.h"
#include "utassert.h"
#include "common_types.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_sb_hooks.h"

/* Test includes */
#include "tle_validation_test.h"
#include "op_test_utils.h"

/* Code-under-test includes */
#include "sgp4Wrapper_test.h"
#include "app_msgids.h"
#include "eci_app.h"
#include "eci_app_event.h"

/* Provide prototypes */
CFE_TIME_SysTime_t Spoof_CFE_TIME_GetTime(void);
void op_AppMain(void);

/* Setup message framework */
#define UT_CFE_SB_MAX_PIPES 32
/* Pipe IDs */
#define  CMDPIPE   0
#define  DATAPIPE  1

typedef struct {
    char                PipeName[OS_MAX_API_NAME];
    UtListHead_t        MsgQueue;
    boolean             InUse;
} Ut_CFE_SB_PipeTableEntry_t;

typedef struct {
    uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];
} NoArgsCmd_t;

extern UtListHead_t                MsgQueue;
extern Ut_CFE_SB_PipeTableEntry_t  PipeTable[UT_CFE_SB_MAX_PIPES];



void OP_Test_digitToInt(void) {
/* Validates that the digitToInt() function returns the 
 * correct integer for each digit 0 - 9.
 */
    
    UtAssert_True(digitToInt('0') == 0, "Digit 0 converted correctly");
    UtAssert_True(digitToInt('1') == 1, "Digit 1 converted correctly");
    UtAssert_True(digitToInt('2') == 2, "Digit 2 converted correctly");
    UtAssert_True(digitToInt('3') == 3, "Digit 3 converted correctly");
    UtAssert_True(digitToInt('4') == 4, "Digit 4 converted correctly");
    UtAssert_True(digitToInt('5') == 5, "Digit 5 converted correctly");
    UtAssert_True(digitToInt('6') == 6, "Digit 6 converted correctly");
    UtAssert_True(digitToInt('7') == 7, "Digit 7 converted correctly");
    UtAssert_True(digitToInt('8') == 8, "Digit 8 converted correctly");
    UtAssert_True(digitToInt('9') == 9, "Digit 9 converted correctly");

}

void OP_Test_computeTLEChecksum(void) {
/* Validates that the computeTLEChecksum() function returns 
 * the correct checksum for several example TLE lines. 
 * this example is derived from:
 * http://www.stltracker.com/resources/tle 
 * which contains several example TLE's with known-good 
 * checksums.
 */
     
    /* Line 1 */
    char line1[] = "1 27843U 03031D   14297.79709235  .00000190  00000-0  10887-3 0   703";
    UtAssert_True(computeTLEChecksum(line1) == digitToInt(line1[68]), "TLE Line 1 checksum computed correctly");
    
    /* Line 2 */
    char line2[] = "2 27843  98.7177 303.6579 0008799 278.1894 134.1004 14.20414232586768";
    UtAssert_True(computeTLEChecksum(line2) == digitToInt(line2[68]), "TLE Line 2 checksum computed correctly");
    
}

void OP_Test_validateTLE(void) {
/* Validates that the validateTLE() function returns a 
 * non-negative value if the validated. The examples 
 * shown here are also from
 * http://www.stltracker.com/resources/tle 
 * which contains several example TLE's with known-good 
 * checksums.
 */

    tle_lines_t tle_lines;
    extern line_check_t line1Check;
    extern line_check_t line2Check;

    /* Modified checksum from example above to make checksum incorrect 
     * Modified from 3 (original) to 5 (for intentional failure)*/
    strcpy(tle_lines.line1, "1 27843U 03031D   14297.79709235  .00000190  00000-0  10887-3 0   705");
    strcpy(tle_lines.line2, "2 27843  98.7177 303.6579 0008799 278.1894 134.1004 14.20414232586768");
    UtAssert_True(validateTLE(&tle_lines) < 0, "Detected incorrect checksum");
    UtPrintf("Line1 Passed: %d, computed: %d, expected: %d\n", line1Check.passed, line1Check.computed, line1Check.expected);
    UtPrintf("Line2 Passed: %d, computed: %d, expected: %d\n", line2Check.passed, line2Check.computed, line2Check.expected);

    /* Example with correct checksum */
    strcpy(tle_lines.line1, "1 27843U 03031D   14297.79709235  .00000190  00000-0  10887-3 0   703");
    strcpy(tle_lines.line2, "2 27843  98.7177 303.6579 0008799 278.1894 134.1004 14.20414232586768");
    UtAssert_True(validateTLE(&tle_lines) >= 0 , "Detected correct checksum");
    UtPrintf("Line1 Passed: %d, computed: %d, expected: %d\n", line1Check.passed, line1Check.computed, line1Check.expected);
    UtPrintf("Line2 Passed: %d, computed: %d, expected: %d\n", line2Check.passed, line2Check.computed, line2Check.expected);

}

void OP_Test_tableValidate(){
/* Validates that the OP app loads its table on startup*/

    /* Initalize a param table with a test TLE */
    
    tle_lines_t test_tle_tbl;
    extern line_check_t line1Check;
    extern line_check_t line2Check;

    /* TEME example from test_TLEs.data */
    strncpy(test_tle_tbl.line1, "1 00005U 58002B   00179.78495062  .00000023  00000-0  28098-4 0  4753", 69);
    test_tle_tbl.line1[69] = 0;
    strncpy(test_tle_tbl.line2, "2 00005  34.2682 348.7242 1859667 331.7664  19.3264 10.82419157413667", 69);
    test_tle_tbl.line2[69] = 0;

    Ut_CFE_TBL_AddTable("/mram/op_tle.tbl", &test_tle_tbl);

    /* Create and place Wakeup message onto command message */
    NoArgsCmd_t       TickMsg;
    CFE_SB_InitMsg(&TickMsg, OP_TICK_MID, sizeof(NoArgsCmd_t), TRUE);
    UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&TickMsg), 0);

    /* Run the code */
    op_AppMain();

    /* Verify event indicating table validation failing was
     * not sent
     */
    UtAssert_True(Ut_CFE_EVS_GetEventCount(1, CFE_EVS_ERROR, "") == 0, "Event for TLE Line1 failing validation did not increment");
    UtAssert_True(Ut_CFE_EVS_GetEventCount(2, CFE_EVS_ERROR, "") == 0, "Event for TLE Line2 failing validation did not increment");
    UtPrintf("Line1 Passed: %d, computed: %d, expected: %d\n", line1Check.passed, line1Check.computed, line1Check.expected);
    UtPrintf("Line2 Passed: %d, computed: %d, expected: %d\n", line2Check.passed, line2Check.computed, line2Check.expected);
    //UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_PARAM_TBL_LOAD_ERR_EID, CFE_EVS_ERROR, "") == 0, "ECI_PARAM_TBL_LOAD_ERR_EID Event Counter did not increment");

}

void OP_Test_propagateOrbit(){
/* Validates that the OP app propagates an orbit for a given TLE
 * and known results.
 */

    /* Initalize a param table with a test TLE */
    tle_lines_t test_tle_tbl;
    /* TEME example from test_TLEs.data */
    strncpy(test_tle_tbl.line1, "1 00005U 58002B   00179.78495062  .00000023  00000-0  28098-4 0  4753", 69);
    test_tle_tbl.line1[69] = 0;
    strncpy(test_tle_tbl.line2, "2 00005  34.2682 348.7242 1859667 331.7664  19.3264 10.82419157413667", 69);
    test_tle_tbl.line2[69] = 0;

    Ut_CFE_TBL_AddTable("/mram/op_tle.tbl", &test_tle_tbl);
    /* Epoch = Day 179.78495062, 2000 = */
    uint32 epoch_sec = 0;

    /* Override the time to the TLE epoch*/
    extern CFE_TIME_SysTime_t UT_Time;
    UT_Time.Seconds = epoch_sec;
    UT_Time.Subseconds = 0;
    Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX, Spoof_CFE_TIME_GetTime);

    /* Create and place Wakeup message onto command message */
    NoArgsCmd_t       TickMsg;
    CFE_SB_InitMsg(&TickMsg, OP_TICK_MID, sizeof(NoArgsCmd_t), TRUE);
    UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&TickMsg), 0);

    /* Run the code */
    op_AppMain();

    /* Verify table validated */
    UtAssert_True(Ut_CFE_EVS_GetEventCount(1, CFE_EVS_ERROR, "") == 0, "Event for TLE Line1 failing validation did not increment");
    UtAssert_True(Ut_CFE_EVS_GetEventCount(2, CFE_EVS_ERROR, "") == 0, "Event for TLE Line2 failing validation did not increment");

    /* Get the output and verify the propagation */
    propState_t* OutPacket = Ut_CFE_SB_FindPacket(OP_PROPSTATE_MID,1);
    /* Check values from test_output.data */
    UtAssert_DoubleCmpAbs(OutPacket->r[0],  7022.46529266, 0.01, "X component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->r[1], -1400.08296755, 0.01, "Y component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->r[2],  0.03995155,    0.01, "Z component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[0],  1.893841015,   0.1,  "X component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[1],  6.405893759,   0.1,  "Y component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[2],  4.534807250,   0.1,  "Z component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->t,     0,             0.1,  "Propagation time matches");

    /* Override the time to 360min past the TLE epoch*/
    UT_Time.Seconds = epoch_sec + 360 * 60;
    UT_Time.Subseconds = 0;

    /* Create and place Wakeup message onto command message */
    UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&TickMsg), 0);

    /* Run the code */
    op_AppMain();

    /* Verify table validated */
    UtAssert_True(Ut_CFE_EVS_GetEventCount(1, CFE_EVS_ERROR, "") == 0, "Event for TLE Line1 failing validation did not increment");
    UtAssert_True(Ut_CFE_EVS_GetEventCount(2, CFE_EVS_ERROR, "") == 0, "Event for TLE Line2 failing validation did not increment");

    /* Get the output and verify the propagation */
    OutPacket = Ut_CFE_SB_FindPacket(OP_PROPSTATE_MID,1);
    /* Check values from test_output.data */
    UtAssert_DoubleCmpAbs(OutPacket->r[0], -7154.03120202, 0.01, "X component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->r[1], -3783.17682504, 0.01, "Y component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->r[2], -3536.19412294, 0.01, "Z component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[0],  4.741887409,   0.1,  "X component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[1], -4.151817765,   0.1,  "Y component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[2], -2.093935425,   0.1,  "Z component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->t,     360,           0.1,  "Propagation time matches");

}
