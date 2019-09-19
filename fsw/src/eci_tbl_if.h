/**
 * @file
 * @brief Interface implementation between app-specified Simulink-generated code and cFE table services
 */

#ifndef ECI_TBL_IF_H
#define ECI_TBL_IF_H

#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "eci_app_cfg.h"

/**@defgroup table_file FSW Table File Builder
 * How to build a table file
 * @{
 */
#ifndef ECI_PARAM_TBL_MAX_NAME_LEN
/**
 * Maximum parameter table length. If this variable isn't defined
 *  it will be set to :c:macro:`CFE_TBL_MAX_NAME_LENGTH`.
 */
#define ECI_PARAM_TBL_MAX_NAME_LEN    CFE_TBL_MAX_NAME_LENGTH
#endif /* ECI_PARAM_TBL_MAX_NAME_LEN */

/**
 * Function to create table files via the elf2cfetbl tool. This should be called
 *  at the end of the parameter table header file to be later compiled with elf2cfetbl.
 *  This macro uses expansion to resolve it's names so there should be no commented
 *  strings as parameters.
 *
 * @param StructName Data structure type to be used as the table image format<br/>
 * @param ObjName Name of the object to be placed in the parameter table<br/>
 * @param TblName Name of the table registered with :c:type:`ECI_Tbl_t.tblname`<br/>
 * @param Desc Quick description of the table<br/>
 * @param Filename What to name the file<br/>
 * @returns Static declaration of CFE_TBL_FileDef_t to be used with elf2cfetbl<br/>
 */
#define ECI_TBL_FILEDEF(StructName, ObjName, TblName, Desc, Filename) \
        static CFE_TBL_FileDef_t StructName __attribute__((__used__)) = \
        {#ObjName, #TblName, #Desc, #Filename, sizeof(ObjName)};

/** @} */
#endif /* ECI_TBL_IF_H */
