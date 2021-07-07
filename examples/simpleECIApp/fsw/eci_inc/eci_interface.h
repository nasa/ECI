/** \file
 * This is a reference interface header file demonstrating the
 * definition of all the ECI interfaces.
 */

#ifndef ECI_INTERFACE_H
#define ECI_INTERFACE_H

/* Include ECI/System headers */
#include "cfe_evs.h"     /** For access to event type/filter macros in defining events */
#include "eci_app.h"     /** For access to ECI data types */

/** Include all external code to be integrated */
#include "external_code.h"     /** Include top-level header for external code */
#include "table_validation.h"  /** Include header file containing definition of 
                                * table validation functions that were defined
                                * for the external code's parameter tables.
                                */

/** Include headers containing msgIDs and perfIDs for this app */
#include "simpleECI_app_msgids.h"
#include "simpleECI_app_perfids.h"

/** Define code revision identifier... can be used for tracking code versions,
 * used in NoOp and Init event messages
 */
#define ECI_APP_REVISION_NUMBER                "1.0"

/** Define Message IDs for ECI packets 
 * Its common for these MIDs to be assigned macros containing the MID's
 * which are defined in a central location (ex msgids.h) for a system 
 * (which helps prevent collisions). They could also just contain the MID's.
 */
#define ECI_FLAG_MID                   SIMPLE_ECI_FLAG_MID
#define ECI_CMD_MID                    SIMPLE_ECI_CMD_MID
#define ECI_PERF_ID                    SIMPLE_ECI_PERF_ID
#define ECI_TICK_MID                   SIMPLE_ECI_TICK_MID
#define ECI_HK_MID                     SIMPLE_ECI_HK_MID
#define ECI_TBL_MANAGE_MID 	           SIMPLE_ECI_TBL_MANAGE_MID
#define ECI_SEND_HK_MID                SIMPLE_ECI_SEND_HK_MID

/* Define the name of this app's Main function. This needs to be consistent
 * with the name provided in the App's makefile to ensure that CFE startup
 * script correctly invokes this app.
 */
#define ECI_APP_MAIN                   sa_AppMain

/* Define the name of this app in several formats for use in event 
 * messages, registering the app with CFE. Its common for CFS 
 * apps to use 2-3 character names, but they can be longer (up to 
 * OS_MAX_API_NAME characters). Here we've used 'SA' for 'Sample App'.
 */
#define ECI_APP_NAME_UPPER             "SA"
#define ECI_APP_NAME_LOWER             "sa"

/* Define the name of the input software bus pipes */
#define ECI_CMD_PIPE_NAME              "SA_CMD_PIPE"
#define ECI_DATA_PIPE_NAME             "SA_DATA_PIPE"


/* Begin definition of parameter tables */

/** If the external code is going to use parameter tables, 
 * ECI_PARAM_TBL_DEFINED must be defined to enable that ECI interface.
 * Its value is unimportant, it merely needs to be defined.
 */
#define ECI_PARAM_TBL_DEFINED

/* Its likely that the external code declares its parameters table,
 * but that those declarations are not visible from its header file. We need
 * to declare externs so that we can use their values here. If the parameter
 * tables are accessible from the header file included above, then this 
 * declaration isn't necessary.
 */
extern convParamStruct_t       convParams;
extern checkParamStruct_t      checkParams;
void * convParamsPtr =  (void *) &convParams;
void * checkParamsPtr = (void *) &checkParams;
/* The following table defines each of the parameter tables needed by the
 * external code for the ECI. 
 */
static ECI_Tbl_t ECI_ParamTable[] = {
  {
    &(convParamsPtr),                       /** Pointer to table */
    "convParams",                           /** Name of table */
    "SA app's conversion parameter table",  /** Table description */
    "convParam.tbl",                        /** Name of file which contains table values */
    sizeof(convParamStruct_t),              /** Size of table (in bytes) */
    &Table_Validation0                      /** (Optional) pointer to table validation function */
  },

  {
    &(checkParamsPtr),
    "checkParams",
    "SA app's check parameter table",
    "checkParams.tbl",
    sizeof(checkParamStruct_t),
    NULL                                    /** NULL if table validation function isn't used */
  },

  { 0, 0, 0, 0, 0 }                         /** The table is null-terminated */
};
/* End definition of parameter tables */

/** Begin definition of state table */
/** Note: This section is commented out because it needs some work*/

/** If the external code is going to use the state table dump functionality, 
 * ECI_STATE_TBL_DEFINED must be defined to enable that ECI interface.
 * Its value is unimportant, it merely needs to be defined.
 */
/*#define ECI_STATE_TBL_DEFINED*/

/** A macro defining the name of the state table for CFE table services is 
 * needed
 */
/*#define ECI_STATE_TABLE_NAME "SA.STATE"*/

/** Its likely that the external code declares its state table,
 * but that those declarations are not visible from its header file. We need
 * to declare externs so that we can use their values here. If the state
 * table is accessible from the header file included above, then this 
 * declaration isn't necessary.
 */
extern stateStruct_t stateData;

/*ECI_TBL_FILEDEF(CSL_TBL_FileDef_State, state_TBL, SA.STATE, SA STATES, sa_state.tbl)*/

/*#define ECI_STATE_TBL state_TBL  */

/** End definition of state table */

/** Begin definition of output messages */

/* Its likely that the external code declares its output data structures,
 * but that those declarations are not visible from its header file. We need
 * to declare externs so that we can use their values here. If the output
 * data structure is accessible from the header file included above, then this 
 * declaration isn't necessary.
 */
extern outputStruct_t outputData;
/* Here we also need to declare this flag which is used to trigger the 
 * output of one of the packets
 */
extern bool dataUpdated;

/** Place each output packet into the Send Table */
static ECI_Msg_t ECI_MsgSnd[] = {
  { 0x0AA5,                        /** ApID of message */
    &outputData,                   /** Pointer to output packet structure */
    sizeof(outputStruct_t),        /** Size of output packet */
    NULL,                          /** Unused for output packets */
    &dataUpdated },                /** Flag indicating when packet should be output, NULL if every cycle */

  { 0, NULL, 0, NULL, NULL }       /** The table is null-terminated */
};

/* End definition of output messages */

/* Begin definition of input messages */

/* Its likely that the external code declares its input data structures,
 * but that those declarations are not visible from its header file. We need
 * to declare externs so that we can use their values here. If the input
 * data structure is accessible from the header file included above, then this 
 * declaration isn't necessary.
 */
extern inputStruct_t  inputData;
extern inputCmdPkt_t  input_CmdPkt;

/* The ECI provides the capability to queue received command messages 
 * in case multiple are received in a given cycle (which is more than 
 * the external code can process, since it only runs once each cycle.
 * The buffer must be declared.
 */
static inputCmdPkt_t inputCmdPkt_queue[ECI_CMD_MSG_QUEUE_SIZE];

/* Place each input packet into the Receive Table */
static ECI_Msg_t ECI_MsgRcv[] = {
  { 0x1AA1,                   /** ApID of message */
    &input_CmdPkt,            /** Pointer to input packet structure */
    sizeof(inputCmdPkt_t),    /** Size of input packet */
    &inputCmdPkt_queue[0],    /** Pointer to queue for command packets */
    NULL },                   /** Unused for input packets */
  { 0x0AA6,
    &inputData,
    sizeof(inputStruct_t),
    NULL,                      /** Null for telemetry packets */
    NULL },                    /** Unused for input packets */

  { 0, NULL, 0, NULL, NULL }   /** The table is null-terminated */
};

/* End definition of input messages */

/* Begin definition of events */

/** If the external code is going to use events, ECI_PARAM_TBL_DEFINED 
 * must be defined to enable that ECI interface. Its value is unimportant, 
 * it merely needs to be defined.
 */
#define ECI_EVENT_TABLE_DEFINED

/* Its likely that the external code declares its flags, but that those 
 * declarations are not visible from its header file. We need
 * to declare externs so that we can use their values here. If the flaag 
 * is accessible from the header file included above, then this declaration 
 * isn't necessary.
 */
extern bool isOverThresh;

/** Create array of structures with error type, pointer to observable signal location,
   the location comment, and the event message */
static const ECI_Evs_t ECI_Events[] = {
  { ECI_EVENT_2_DATA,                          /** Macro defining type of ECI event, 
                                                * in this case, event with 2 data points*/
    &(uint8){1},                                         /* ID for this event, unqiue to this app */
    &(uint8){CFE_EVS_EventType_INFORMATION},                       /* Type of event */
    &(uint32){CFE_EVS_NO_FILTER},                         /* Event mask */
    &isOverThresh,                             /** Pointer to flag */
    "Value was over threshold! (%d > %d)",     /** fprintf-style format string */
    "external code run()",                     /** (Optional) Location in code where event originated*/
    (double*)&outputData.converted_Value,      /** First data value */
    &checkParams.checkThresh,                  /** Second data value */
    0,                                         /** Zeros for unused data points */
    0,
    0
  },

  { ECI_EVENT_0_DATA,                          /** Macro defining type of ECI event, 
                                                ** in this case, event with no data points*/
    &(uint8){2},
    &(uint8){CFE_EVS_EventType_INFORMATION},
    &(uint32){CFE_EVS_NO_FILTER}, 
    (bool*)&input_CmdPkt.processData,
    "Got command to process data!",
    "external code run()",
    0,                                         /** Zeros for unused data points */
    0,
    0,
    0,
    0
  },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }       /** The table is null-terminated */
};

/* End definition of events */

/* Begin definition of flags */

/** If the external code is going to use status flags, ECI_FLAG_TABLE_DEFINED 
 * must be defined to enable that ECI interface. Its value is unimportant, 
 * it merely needs to be defined.
 */
#define ECI_FLAG_TABLE_DEFINED

/* FIXME: why is this needed??? */
#define ECI_FLAG_MAX_ID                 1

/* Create array of Flag structures with id and flag */
static const ECI_Flag_t ECI_Flags[] = {
  { &(uint8){1},                        /** ID of this flag (unqiue to this app) */
    (bool *)&isOverThresh            /** Flag to be telemetered */
  },

  { 0, 0 }                              /** The table is null-terminated */
};
/* End definition of flags */

/* If the external code is going to use ECI-provided time, 
 * ECI_STEP_TIMESTAMP_DEFINED must be defined to enable that ECI interface. 
 * Its value is unimportant, it merely needs to be defined.
 */
#define ECI_STEP_TIMESTAMP_DEFINED

/* Begin definition of external code functions */

/* Begin CDS definition */
#define ECI_CDS_TABLE_DEFINED          1

/* Create array of structures with data to be managed by CDS */
static const ECI_Cds_t ECI_CdsTable[] = {
  { "stateData",
    sizeof(stateStruct_t),
    &stateData,
  },
  
  { NULL, 0, NULL }
};

/* End CDS definition */

/* model initialization function */
#define ECI_INIT_FCN                 setup();

/* step function.  Single rate (non-reusable interface) */
#define ECI_STEP_FCN                 run();

/* Get model outputs here */
#define ECI_TERM_FCN                 teardown();

/* End definition of external code functions */

#endif                                 /* ECI_INTERFACE_H */
