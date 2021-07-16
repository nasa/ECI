#ifndef ECI_INTERFACE_H
#define ECI_INTERFACE_H

/* Include ECI/System headers */
#include "cfe_evs.h"     /* For access to event type/filter macros in defining events */
#include "eci_app.h"     /* For access to ECI data types */
#include "eci_app_cfg.h" /* For access to config parameters (including queue sizes)*/

/* Include top-level header for external code */
#include "sgp4Wrapper.h"

/* Include headers containing mission and platform-specific configs for this app */
#include "op_app_msgids.h"
#include "op_app_perfids.h"

/* Define code revision identifier... can be used for tracking code versions,
 * used in NoOp and Init event messages */
#define ECI_APP_REVISION_NUMBER "1.0"

/* Define Message IDs for ECI packets
 * The macros OP_* are expected to be defined by the system integrator.
 */
#define ECI_FLAG_MID OP_FLAG_MID
#define ECI_CMD_MID  OP_CMD_MID
#define ECI_PERF_ID  OP_PERF_ID
#define ECI_TICK_MID OP_TICK_MID
#define ECI_HK_MID   OP_HK_MID

/* Define the name of this app's Main function. This needs to be consistent
 * with the name provided in the App's makefile to ensure that CFE startup
 * script correctly invokes this app.
 */
#define ECI_APP_MAIN op_AppMain

/* Define the name of this app in several formats for use in event
 * messages, registering the app with CFE. Here we've used 'OP' for
 * 'Onboard Propagator'.
 */
#define ECI_APP_NAME_UPPER "op"
#define ECI_APP_NAME_LOWER "op"

/* Define the name of the input software bus pipes */
#define ECI_CMD_PIPE_NAME  "OP_CMD_PIPE"
#define ECI_DATA_PIPE_NAME "OP_DATA_PIPE"

/* Begin definition of parameter tables */

/* If the external code is going to use parameter tables,
 * ECI_PARAM_TBL_DEFINED must be defined to enable that ECI interface.
 * Its value is unimportant, it merely needs to be defined.
 */
#define ECI_PARAM_TBL_DEFINED 1

/* The following table defines each of the parameter tables needed by the
 * external code for the ECI.
 */
static ECI_Tbl_t ECI_ParamTable[] = {
    {
        (void *)&(tle_lines),           /* Pointer to begining of TLE line1 */
        "OP_TLE_Tbl",                   /* Name of table */
        "OP app's tle parameter table", /* Table description */
        "tle_tbl.tbl",                  /* Name of file which contains table values */
        sizeof(tle_lines_t),            /* Size of table (in bytes) */
        &validateTLE                    /* (Optional) pointer to table validation function */
    },

    {0, 0, 0, 0, 0} /* The table is null-terminated */
};

/* End definition of parameter tables */

/* Begin definition of output messages */

/* Place each output packet into the Send Table */
static ECI_Msg_t ECI_MsgSnd[] = {
    {OP_PROPSTATE_MID,    /* ApID of message */
     &propState,          /* Pointer to output packet structure */
     sizeof(propState_t), /* Size of output packet */
     NULL,                /* Unused for output packets */
     NULL},               /* Flag indicating when packet should be output */

    {0, NULL, 0, NULL, NULL} /* The table is null-terminated */
};
/* End definition of output messages */

/* Begin definition of input messages */

/* No input messages are used for this code*/
static ECI_Msg_t ECI_MsgRcv[] = {

    {0, NULL, 0, NULL, NULL} /* The table is null-terminated */
};
/* End definition of input messages */

/* Begin definition of events */

/* If the external code is going to use events, ECI_PARAM_TBL_DEFINED
 * must be defined to enable that ECI interface. Its value is unimportant,
 * it merely needs to be defined.
 */
#define ECI_EVENT_TABLE_DEFINED 1

/* Defined so that we can pass pointer to them to match ECI interface */
uint8               line1FailEventID = 1;
extern line_check_t line1Check;
uint8               line2FailEventID = 2;
extern line_check_t line2Check;
uint8               timeOutOfRangeID = 3;
uint8               errorEventType   = CFE_EVS_ERROR;
uint32              filterOneEvent   = CFE_EVS_FIRST_ONE_STOP;

/* Create array of structures with error type, pointer to observable signal location,
   the location comment, and the event message */
static const ECI_Evs_t ECI_Events[] = {
    {ECI_EVENT_2_DATA,                   /* Macro defining type of ECI event,
                                          * in this case, event with two data points*/
     &line1FailEventID, &errorEventType, /* Type of event */
     &filterOneEvent,                    /* Event mask - Ask CFE to filter all but the first
                                          * instance of this event since the checksumPassed flag
                                          * will retain its failing status until the next time
                                          * validation is run, which would result in constantly
                                          * sending this event
                                          */
     (boolean *)&line1Check.failed,      /* Pointer to flag */
     "Checksum for line 1 failed to validate, computed %.0f does not match expected %.0f", /* fprintf-style format
                                                                                              string */
     "validateTLE()",                /* (Optional) Location in code where event originated*/
     (double *)&line1Check.computed, /* Data values (not used here) */
     (double *)&line1Check.expected, 0, 0, 0},

    {ECI_EVENT_2_DATA, &line2FailEventID, &errorEventType, &filterOneEvent, (boolean *)&line2Check.failed,
     "Checksum for line 2 failed to validate, computed %.0f does not match expected %.0f", /* fprintf-style format
                                                                                              string */
     "validateTLE()", (double *)&line2Check.computed, /* Data values (not used here) */
     (double *)&line2Check.expected, 0, 0, 0},

    {ECI_EVENT_2_DATA, &timeOutOfRangeID, &errorEventType, &filterOneEvent, &propState.invalid,
     "FSW time (%f) outside TLE range (%f)", "propagate()", (double *)&propState.t, /* FSW Time */
     (double *)&tle.epoch,                                                          /* TLE.epoch */
     0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} /* The table is null-terminated */
};

/* End definition of events */

/* If the external code is going to use ECI-provided time,
 * ECI_STEP_TIMESTAMP_DEFINED must be defined to enable that ECI interface.
 * Its value is unimportant, it merely needs to be defined.
 */
#define ECI_STEP_TIMESTAMP_DEFINED 1

/* Begin definition of external code functions */
#define ECI_STEP_FCN propagate();
/* Init and Term functions not needed for this app */

/* End definition of external code functions */

#endif
