/**
 * @file
 * @brief Defines the datatypes & macros used by the ECI interface header file.
 */

#ifndef ECI_APP_H
#define ECI_APP_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


#include "cfe.h"
#include "eci_app_cfg.h" /* Needed for queue length macros used by eci_interface.h */

/**@defgroup eci_interface ECI Interface
 * This section contains information on how to properly construct
 *  the main eci_interface.h file.
 */

/**@defgroup eci_event ECI Events
 * @ingroup eci_interface
 * Details on how to specify ECI events.
 * @{
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
/**@}*/

/**@defgroup eci_msg ECI Software Bus messages
 * @ingroup eci_interface
 * Definition of an incoming or outgoing message for the software bus.
 * @{
 */
/**
 * Element of the message array which defines a single message to be
 *  sent over the software bus.
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
   bool*       sendMsg;
} ECI_Msg_t;
/**@}*/

/** FDC Reporting Interface Structure */
typedef struct {
  /** Pointer to flag ID - unique ID set by the user */
  uint8*   FlagID;
  /** Pointer to status flag */
  bool* StatusFlag;
} ECI_Flag_t;

/**@ingroup eci_event
 * Struct to define an EVS event message to be registered.
 */
typedef struct {
  /** Event Block describes how many data points  */
  uint8 eventBlock;
  /** Event Id  - unique id set by the user*/
  uint8* eventID;
  /** Event Type - debug, info, error, crit set by user */
  uint8* eventType;
  /** Event Mask - filter set by user */
  uint32* eventMask;
   /** Flag indicating simulink event has occurred */
  bool* eventFlag;
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

/**@defgroup eci_table ECI Table
 * @ingroup eci_interface
 * How to define a FSW table
 * @{
 */
/** Table Interface Structure */
typedef struct {
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
/**@}*/

/** Critical Data Store Structure */
typedef struct {
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
