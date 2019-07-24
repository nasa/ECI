/**
 * File:  eci_app_cfg.h
 * Description:  Contains all of the configurable options (defined as macros) which 
 * change how the ECI interacts with the cFS and external code.  
 */
 
#ifndef ECI_APP_CFG_H
#define ECI_APP_CFG_H

/************************************************************************
** Includes
*************************************************************************/

#include "cfe.h"

/************************************************************************
** Macro Definitions
*************************************************************************/
/*
 * Table File Path Settings
 */
#define PARAM_TBL_PATH_PREFIX "/cf/apps/"
#define STATE_TBL_PATH_PREFIX "/cf/apps/"

/*
 * App start-up configuration settings
 */

#define ECI_STARTUP_SYNC_TIMEOUT       65535                   /* Time Out (Secs) for Syncing ECI apps with cFS */

/*
 * Command configuration settings
 */
 
#define ECI_NO_DATA_CMD_MSG_LENGTH     CFE_SB_CMD_HDR_SIZE     /* Length of a no data parameter command message */

#define ECI_FUNC_CODE_START            100                     /* Start of valid Function code values  */

#define ECI_FUNC_CODE_END              110                     /* End of valid function code values */

#define ECI_CMD_MSG_QUEUE_SIZE         25                      /* Command Message Max Queue Size */

#define ECI_MAX_CMD_SEQUENCE_NUMBER    16383                   /* Maximum sequence number - Sequence numbers are 14-bits */

#endif /* ECI_APP_CFG_H */

/************************/
/*  End of File Comment */
/************************/
