/**
 * @file
 * @brief Contains all of the configurable options (defined as macros) which 
 * change how the ECI interacts with the cFS and external code.  
 */
 
#ifndef ECI_APP_CFG_H
#define ECI_APP_CFG_H

#include "cfe.h"

/**
 * @defgroup table_settings Table File Path Settings
 * Configurations for where in the runtime environment
 *  the ECI app will look for it's FSW tables. This setting
 *  will be dependent on your cFE build environement.
 * @{
 */
/** Path to FSW parameter tables */
#define PARAM_TBL_PATH_PREFIX "/cf/"
/** Path to FSW state tables */
#define STATE_TBL_PATH_PREFIX "/cf/"
/**@}*/

/**
 * @defgroup general_settings General Settings
 * @{
 */
/** Time Out (Secs) for syncing ECI apps with cFS */
#define ECI_STARTUP_SYNC_TIMEOUT       65535

/** 
 * Length of a no data parameter command message
 */
#define ECI_NO_DATA_CMD_MSG_LENGTH     CFE_SB_CMD_HDR_SIZE
/** Start of valid function code values */
#define ECI_FUNC_CODE_START            100
/** End of valid function code values */
#define ECI_FUNC_CODE_END              110
/** Command Message Max Queue Size */
#define ECI_CMD_MSG_QUEUE_SIZE         25
/** Maximum sequence number (14 bits) */
#define ECI_MAX_CMD_SEQUENCE_NUMBER    16383
/**@}*/

#endif  /* ECI_APP_CFG_H */
