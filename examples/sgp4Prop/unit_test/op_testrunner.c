/* Test framework includes */
#include "uttest.h"

/* Include test declarations */
#include "op_test_utils.h"
#include "tle_validation_test.h"

void OP_App_Test_AddTestCases(void) {

	UtTest_Add(OP_Test_digitToInt, OP_Test_Setup,
              OP_Test_TearDown, "digitToInt function correctly converts digits");

	UtTest_Add(OP_Test_computeTLEChecksum, OP_Test_Setup,
              OP_Test_TearDown, "Checksum for TLE is computed correctly");
              
	UtTest_Add(OP_Test_validateTLEChecksum, OP_Test_Setup,
              OP_Test_TearDown, "TLE checksum is validated correctly");

	UtTest_Add(OP_Test_tableValidate, OP_Test_Setup,
              OP_Test_TearDown, "TLE table validates");

	UtTest_Add(OP_Test_propagateOrbit, OP_Test_Setup,
              OP_Test_TearDown, "OP app propagates orbit");

              
} /* End of OP_App_Test_AddTestCases */


/* Nonstandard main() name to avoid collision with main() in 
 * external code. Linker flag used to specify this function as
 * entry point.
 */
int test_main(void)
{
    OP_App_Test_AddTestCases();

    return(UtTest_Run());

} /* End of main() */

/************************/
/*  End of File Comment */
/************************/
