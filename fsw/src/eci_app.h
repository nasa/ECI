/*
 * File: eci_app.h 
 * Description:  Defines the datatypes & macros used by the ECI interface header file.
 */

#ifndef ECI_APP_H
#define ECI_APP_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "cfe.h"
/**
 * Event message defines for use in :c:type:`ECI_Evs`
 */
/** Identifier for Event message with zero data points */
#define ECI_EVENT_0_DATA     0
/** Identifier for Event message with one data points */
#define ECI_EVENT_1_DATA     1
/** Identifier for Event message with two data points */
#define ECI_EVENT_2_DATA     2
/** Identifier for Event message with three data points */
#define ECI_EVENT_3_DATA     3
/** Identifier for Event message with four data points */
#define ECI_EVENT_4_DATA     4
/** Identifier for Event message with five data points */
#define ECI_EVENT_5_DATA     5

/**
 * Definition of an incoming or outgoing message for the software bus.
 * 
 * **Example** (eci_interface.h)
 * ::
 * 
 *   static ECI_Msg_t ECI_MsgSnd[] = { 
 *     { message_id,
 *       pointer_to_object,
 *       sizeof(object),
 *       NULL,
 *       NULL },
 *     { 0, NULL, 0, NULL, NULL }
 *   };
 *
 */
typedef struct {
   /** Message ID */
   CFE_SB_MsgId_t mid;
   /** Input/Output Buffer */
   void           *mptr;
   /** Message Size (including header) */
   size_t         siz;
   /** Location of Cmd Queue Buffer - NULL if Tlm Message */
   void           *qptr;
   /** Pointer to the flag indicating whether to send output */
   boolean*       sendMsg;
} ECI_Msg_t;

/**
 * FDC Reporting Interface Structure
 */
typedef struct ECI_Flag {
  /** Pointer to flag ID - unique ID set by the user */
  uint8*   FlagID;
  /** Pointer to status flag */
  boolean* StatusFlag;
} ECI_Flag_t;

/**
 * Struct to define an EVS event message to be registered.
 *
 * **Example** (eci_interface.h)
 * ::
 * 
 *   static const ECI_Evs_t ECI_Events[] = { 
 *     { ECI_EVENT_0_DATA,
 *       &id,
 *       &error,
 *       &filter,
 *       (boolean*)condition,
 *       "MESSAGE",
 *       0, 0, 0, 0, 0
 *     },
 *     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
 *   }
 * 
 */
typedef struct ECI_Evs {
  /** Event Block describes how many data points  */
  uint8 eventBlock;
  /** Event Id  - unique id set by the user*/
  uint8* eventID;
  /** Event Type - debug, info, error, crit set by user */
  uint8* eventType;
  /** Event Mask - filter set by user */
  uint32* eventMask;
   /** Flag indicating simulink event has occurred */
  boolean* eventFlag;
  /** Msgpoint to send with an event taken from observable signal */     
  uint8* eventMsg;
  /** Location string */
  char* loc;
  /** First data point */
  double* data_1;
  /** Second data point */
  double* data_2;
  /** Third data point */
  double* data_3;
  /** Fourth data point */
  double* data_4;
  /** Fifth data point */
  double* data_5;
} ECI_Evs_t;

/**
 * Table Interface Structure
 *
 * **Example** (eci_interface.h)
 * ::
 *
 *   static ECI_Tbl_t ECI_ParamTable[] = {
 *    {
 *      (void*)&(tle_lines),                 
 *      "OP_TLE_Tbl",                        
 *      "OP app's tle parameter table",      
 *      "tle_tbl.tbl",                       
 *      sizeof(tle_lines_t),                 
 *      &validateTLE                         
 *    },
 *    { 0, 0, 0, 0, 0 }                         
 *   };
 */
typedef struct ECI_Tbl {
    /** Pointer to table  */
    void**  tblptr;
    /** Name of table  */
    char*   tblname;
    /** Description of table  */
    char*   tbldesc;
    /** Filename of table  */
    char*   tblfilename;
    /** Size of table */
    uint32  tblsize;
    /** Table validation func */
    void*   tblvalfunc;   
} ECI_Tbl_t;

/** Critical Data Store Structure */
typedef struct ECI_Cds {
   /** Name of CDS block */
   char*  cdsname;
   /** Size of CDS block */
   size_t cdssiz;
   /** Address of Critical Data  */
   void*  cdsptr;
} ECI_Cds_t;

/** Time Interface Structure */
typedef CFE_TIME_SysTime_t ECI_TimeStamp_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ECI_APP_H */

