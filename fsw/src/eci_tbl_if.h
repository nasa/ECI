/**
 * File:  eci_tbl_if.h
 * Description:  Interface implementation between app-specified Simulink-generated code and cFE table services
 */

#ifndef ECI_TBL_IF_H
#define ECI_TBL_IF_H

/*************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "eci_app_cfg.h"

/************************************************************************
** Macro Definitions
*************************************************************************/

/* Default Maximum Parameter Table Name Length  */
#ifndef ECI_PARAM_TBL_MAX_NAME_LEN
#define ECI_PARAM_TBL_MAX_NAME_LEN    CFE_TBL_MAX_NAME_LENGTH
#endif /* ECI_PARAM_TBL_MAX_NAME_LEN */

/* Macro Definition so that ECI can define CFE_TBL_FileDef_t for Creating Table Files via elf2cfetbl */

#define ECI_TBL_FILEDEF(StructName, ObjName, TblName, Desc, Filename) \
        static CFE_TBL_FileDef_t StructName __attribute__((__used__)) = \
        {#ObjName, #TblName, #Desc, #Filename, sizeof(ObjName)};

#endif /* ECI_TBL_IF_H */

/************************/
/*  End of File Comment */
/************************/

