#include <stdio.h>
/* Test framework includes */
#include "uttest.h"

/* Include test declarations */
#include "op_test_utils.h"
#include "tle_validation_test.h"

void OP_App_Test_AddTestCases(void) {
	UtTest_Add(OP_Test_digitToInt, OP_Test_Setup,
              OP_Test_TearDown, "OP_Test_digitToInt: digitToInt function correctly converts digits");

	UtTest_Add(OP_Test_computeTLEChecksum, OP_Test_Setup,
              OP_Test_TearDown, "OP_Test_computeTLEChecksum: Checksum for TLE is computed correctly");
              
	UtTest_Add(OP_Test_validateTLE, OP_Test_Setup,
              OP_Test_TearDown, "OP_Test_validateTLE: TLE is validated correctly");

	UtTest_Add(OP_Test_tableValidate, OP_Test_Setup,
              OP_Test_TearDown, "OP_Test_tableValidate: TLE table validates");
  UtTest_Add(OP_Test_getTime, OP_Test_Setup,
              OP_Test_TearDown, "OP_Test_getTime: OP app gets time"); 

	UtTest_Add(OP_Test_propagateOrbit, OP_Test_Setup,
              OP_Test_TearDown, "OP_Test_propagateOrbit: OP app propagates orbit");	
              
} /* End of OP_App_Test_AddTestCases */


/* Nonstandard main() name to avoid collision with main() in 
 * external code. Linker flag used to specify this function as
 * entry point.
 */
/*int main(void)
{
    OP_App_Test_AddTestCases();
    return UtTest_Run();
}*/ 

/* End of main() */

/************************/
/*  End of File Comment */
/************************/

