

#include "table_validation.h"

/* CFE table services (and the ECI) supports the use of a table validation 
 * function. The external code may or may not provide one, depending on its 
 * design. The declaration of the function to be used as the table validation
 * function must be accessible. We declare one below. Note that all table 
 * validation functions must have the signature shown below for compatibility
 * with CFE table services and the ECI.
 */
int32_t Table_Validation0(void* TblPtr){
    int32_t retStat = 0;
    
    return retStat;
}