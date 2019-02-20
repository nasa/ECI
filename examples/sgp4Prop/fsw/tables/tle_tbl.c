#include "sgp4Wrapper.h"
#include "eci_tbl_if.h"

tle_lines_t tle_lines_tbl = {
  "1 00005U 58002B   00179.78495062  .00000023  00000-0  28098-4 0  4753",
  "2 00005  34.2682 348.7242 1859667 331.7664  19.3264 10.82419157413667",
} ;

/*
 ** The macro below identifies:
 **    1) the data structure type to use as the table image format
 **    2) the name of the table to be placed into the CFS Table File Header
 **    3) a brief description of the contents of the file image
 **    4) the desired name of the table image binary file that is CFS compatible
 */
ECI_TBL_FILEDEF(CFE_TBL_FileDef, tle_lines_tbl, "OP_APP.OP_TLE_Tbl",
                "OP app TLE table", tle_tbl.tbl )
/*
 * File trailer for generated code.
 *
 * [EOF]
 */
