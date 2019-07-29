#include "external_code.h"
#include "eci_tbl_if.h"

/* Empty table for initialization */
convParamStruct_t conv_param_tbl = {};

/*
 ** The macro below identifies:
 **    1) the data structure type to use as the table image format
 **    2) the name of the table to be placed into the CFS Table File Header
 **    3) a brief description of the contents of the file image
 **    4) the desired name of the table image binary file that is CFS compatible
 */
ECI_TBL_FILEDEF(CFE_TBL_FileDef, conv_param_tbl, SA_APP.convParams,
                Empty conv_param table, convParam.tbl)