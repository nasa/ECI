#include "tle_validation_test.h"

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
    UtPrintf("Line1 Failed: %d, computed: %d, expected: %d\n", line1Check.failed, line1Check.computed, line1Check.expected);
    UtPrintf("Line2 Failed: %d, computed: %d, expected: %d\n", line2Check.failed, line2Check.computed, line2Check.expected);

    /* Example with correct checksum */
    strcpy(tle_lines.line1, "1 27843U 03031D   14297.79709235  .00000190  00000-0  10887-3 0   703");
    strcpy(tle_lines.line2, "2 27843  98.7177 303.6579 0008799 278.1894 134.1004 14.20414232586768");
    UtAssert_True(validateTLE(&tle_lines) >= 0 , "Detected correct checksum");
    UtPrintf("Line1 Failed: %d, computed: %d, expected: %d\n", line1Check.failed, line1Check.computed, line1Check.expected);
    UtPrintf("Line2 Failed: %d, computed: %d, expected: %d\n", line2Check.failed, line2Check.computed, line2Check.expected);

}

void OP_Test_tableValidate(){
/* Validates that the OP app loads its table on startup*/

    /* Initalize a param table with a test TLE */
    
    extern line_check_t   line1Check;
    extern line_check_t   line2Check;
    extern tle_lines_t    test_tle_tbl;

    /* TEME example from test_TLEs.data */
    strncpy(test_tle_tbl.line1, "1 00005U 58002B   00179.78495062  .00000023  00000-0  28098-4 0  4753", 69);
    test_tle_tbl.line1[69] = 0;
    strncpy(test_tle_tbl.line2, "2 00005  34.2682 348.7242 1859667 331.7664  19.3264 10.82419157413667", 69);
    test_tle_tbl.line2[69] = 0;

    /* Create and place Wakeup message onto command message */
    OP_Test_SendTick();

    /* Run the code */
    op_AppMain();

    /* Verify event indicating table validation failing was
     * not sent
     */
    UtAssert_True(Ut_CFE_EVS_GetEventCount(1, CFE_EVS_ERROR, "") == 0, "Event for TLE Line1 failing validation did not increment");
    UtAssert_True(Ut_CFE_EVS_GetEventCount(2, CFE_EVS_ERROR, "") == 0, "Event for TLE Line2 failing validation did not increment");
    UtPrintf("Line1 Failed: %d, computed: %d, expected: %d\n", line1Check.failed, line1Check.computed, line1Check.expected);
    UtPrintf("Line2 Failed: %d, computed: %d, expected: %d\n", line2Check.failed, line2Check.computed, line2Check.expected);
    //UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_PARAM_TBL_LOAD_ERR_EID, CFE_EVS_ERROR, "") == 0, "ECI_PARAM_TBL_LOAD_ERR_EID Event Counter did not increment");

}

void OP_Test_getTime(){

    propState_t* OutPacket;

    OP_Test_SetTime(0, 0);

    /* Run the code */
    OutPacket = OP_Test_RunAppAndGetPkt(1);

   /* Verify app got the time that was set */
    UtAssert_DoubleCmpAbs(OutPacket->t,     0,             0.1,  "Propagation time matches");

    OP_Test_SetTime(120, 0);

    /* Run app again */
    OutPacket = OP_Test_RunAppAndGetPkt(2);

   /* Verify app got the time that was set */
    UtAssert_DoubleCmpAbs(OutPacket->t,     2,             0.1,  "Propagation time matches");

}


void OP_Test_propagateOrbit(){
/* Validates that the OP app propagates an orbit for a given TLE
 * and known results.
 */

    extern tle_lines_t    test_tle_tbl;
    propState_t* OutPacket;

    /* Set table contents to TLE example from test_TLEs.data */
    strncpy(test_tle_tbl.line1, "1 00005U 58002B   00179.78495062  .00000023  00000-0  28098-4 0  4753", 69);
    test_tle_tbl.line1[69] = 0;
    strncpy(test_tle_tbl.line2, "2 00005  34.2682 348.7242 1859667 331.7664  19.3264 10.82419157413667", 69);
    test_tle_tbl.line2[69] = 0;

    /* Epoch = Day 179.78495062, 2000 = */
    uint32 epoch_sec = 0;

    /* Override the time to the TLE epoch*/
    OP_Test_SetTime(epoch_sec, 0);

    /* Run the code and get output */
    OutPacket = OP_Test_RunAppAndGetPkt(1);

    /* Verify table validated by ensuring no error events generated */
    UtAssert_True(Ut_CFE_EVS_GetEventCount(1, CFE_EVS_ERROR, "") == 0, "Event for TLE Line1 failing validation did not increment");
    UtAssert_True(Ut_CFE_EVS_GetEventCount(2, CFE_EVS_ERROR, "") == 0, "Event for TLE Line2 failing validation did not increment");

    /* Check values from test_output.data */
    UtAssert_DoubleCmpAbs(OutPacket->r[0],  7022.46529266, 0.01, "X component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->r[1], -1400.08296755, 0.01, "Y component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->r[2],  0.03995155,    0.01, "Z component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[0],  1.893841015,   0.1,  "X component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[1],  6.405893759,   0.1,  "Y component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[2],  4.534807250,   0.1,  "Z component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->t,     0,             0.1,  "Propagation time matches");

    /* Override the time to 360min past the TLE epoch*/
    OP_Test_SetTime(epoch_sec + 360 * 60, 0);

    /* Create and place Wakeup message onto command message */
    OutPacket = OP_Test_RunAppAndGetPkt(2);

    /* Verify table validated */
    UtAssert_True(Ut_CFE_EVS_GetEventCount(1, CFE_EVS_ERROR, "") == 0, "Event for TLE Line1 failing validation did not increment");
    UtAssert_True(Ut_CFE_EVS_GetEventCount(2, CFE_EVS_ERROR, "") == 0, "Event for TLE Line2 failing validation did not increment");

    /* Check values from test_output.data */
    UtAssert_DoubleCmpAbs(OutPacket->r[0], -7154.03120202, 0.01, "X component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->r[1], -3783.17682504, 0.01, "Y component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->r[2], -3536.19412294, 0.01, "Z component of propagated position matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[0],  4.741887409,   0.1,  "X component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[1], -4.151817765,   0.1,  "Y component of propagated velocity matches");
    UtAssert_DoubleCmpAbs(OutPacket->v[2], -2.093935425,   0.1,  "Z component of propagated velocity matches");
    UtPrintf("OutPacket->t: %f\n", OutPacket->t);
    UtPrintf("OutPacket->r: %f %f %f\n", OutPacket->r[0], OutPacket->r[1], OutPacket->r[2]);
    UtAssert_DoubleCmpAbs(OutPacket->t,     360,           0.1,  "Propagation time matches");

}
