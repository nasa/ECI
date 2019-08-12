/*
**    File:
**
*/

#include "cfe.h"
#include "paramTable1_b.h"
#include "paramTable2_b.h"

#define PC_TBL_VALIDATION_MSG	   601
#define PC_TBL_VALIDATION_MSG2	602

int32 Table_Validation0(void* TblPtr)
{
   paramTable1_b* paramTable1 = (paramTable1_b*) TblPtr;

   if (paramTable1->param3 > 10)
   {
      CFE_EVS_SendEvent(PC_TBL_VALIDATION_MSG2, CFE_EVS_ERROR, "paramTable1->param3 (%d) > 10", paramTable1->param3);

      return (CFE_SUCCESS - 1);

   }

	CFE_EVS_SendEvent(PC_TBL_VALIDATION_MSG, CFE_EVS_INFORMATION, "Table Validation Function 0");

	return CFE_SUCCESS;

} /* End of Table_Validation0 */

int32 Table_Validation1(void* TblPtr)
{

   paramTable2_b* paramTable2 = (paramTable2_b*) TblPtr;

   if (paramTable2->param3 > 10)
   {
      CFE_EVS_SendEvent(PC_TBL_VALIDATION_MSG2, CFE_EVS_ERROR, "paramTable2->param3 (%d) > 10", paramTable2->param3);

      return (CFE_SUCCESS - 1);

   }

	CFE_EVS_SendEvent(PC_TBL_VALIDATION_MSG, CFE_EVS_INFORMATION, "Table Validation Function 1");

	return CFE_SUCCESS;

} /* End of Table_Validation1 */



