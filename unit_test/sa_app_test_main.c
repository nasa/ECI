#include "uttest.h"
#include "sa_test_utils.h"
#include "sa_app_test.h"

void SA_App_Test_AddTestCases(void)
{

    UtTest_Add(SA_AppMain_Test_ECI_1001_1050, SA_Test_Setup, SA_Test_TearDown,
               "ECI receives a message on the command pipe with an unrecognized message ID");

    UtTest_Add(SA_AppMain_Test_ECI_1010, SA_Test_Setup, SA_Test_TearDown,
               "ECI receives a message on the data pipe with an unrecognized message ID");

    UtTest_Add(SA_AppMain_Test_ECI_1020_1030_1050, SA_Test_Setup, SA_Test_TearDown,
               "ECI receives messages on the command and data pipe with mismatched message lengths");

    UtTest_Add(SA_AppMain_Test_INIT_1001_TBL_1002, SA_Test_Setup, SA_Test_TearDown,
               "Test of Nominal ECI App Initialization");

    UtTest_Add(SA_AppMain_Test_TBL_1010, SA_Test_Setup, SA_Test_TearDown, "Test of ECI table management");

    UtTest_Add(SA_AppMain_Test_ECI_1040_CMD_1001_SB_1001_NoOpCmd, SA_Test_Setup, SA_Test_TearDown,
               "Test of ECI No_Op Command");

    UtTest_Add(SA_AppMain_Test_SB_1001_CMD_1010_ResetHkCtrsCmd, SA_Test_Setup, SA_Test_TearDown,
               "Test of ECI HK Counter Reset Command");

    UtTest_Add(SA_AppMain_Test_CMD_1050_1051_1052_1053, SA_Test_Setup, SA_Test_TearDown, "Test of ECI Command Queue");

    UtTest_Add(SA_AppMain_Test_CMD_1060, SA_Test_Setup, SA_Test_TearDown,
               "Test of Command Message with Func Code outside range of valid values");

    UtTest_Add(SA_AppMain_Test_TIME_1001, SA_Test_Setup, SA_Test_TearDown, "Test of ECI Time Fetching");

    UtTest_Add(SA_AppMain_Test_FDC_1001_1030, SA_Test_Setup, SA_Test_TearDown, "Test of ECI FDC Handling");

    UtTest_Add(SA_AppMain_Test_INIT_1012, SA_Test_Setup, SA_Test_TearDown,
               "Test of ECI Init when Table Fails to Load table values at startup");

    UtTest_Add(SA_AppMain_Test_Wakeup, SA_Test_Setup, SA_Test_TearDown, "Test of ECI Wakeup");

} /* End of SA_App_Test_AddTestCases */
