/**
 * File: eci_app.h 
 * Description:  Defines the datatypes & macros used by the ECI interface header file.
 */

#ifndef ECI_APP_H
#define ECI_APP_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*************************************************************************
 **
 ** Include section
 **
 **************************************************************************/

#include "cfe.h"

/************************************************************************
** Macro Definitions
*************************************************************************/

#define ECI_EVENT_0_DATA     0  /* Identifier for Event message with zero data points */
#define ECI_EVENT_1_DATA     1  /* Identifier for Event message with one data points */
#define ECI_EVENT_2_DATA     2  /* Identifier for Event message with two data points */
#define ECI_EVENT_3_DATA     3  /* Identifier for Event message with three data points */
#define ECI_EVENT_4_DATA     4  /* Identifier for Event message with four data points */
#define ECI_EVENT_5_DATA     5  /* Identifier for Event message with five data points */

/************************************************************************
** Type Definitions
*************************************************************************/

/* ECI_MsgRcv and ECI_MsgSnd Interface Structure */
typedef struct {
   CFE_SB_MsgId_t mid;     /* Message ID */
   void           *mptr;   /* Input/Output Buffer */
   size_t         siz;     /* Message Size (including header) */
   void           *qptr;   /* Location of Cmd Queue Buffer - NULL if Tlm Message */
   boolean*       sendMsg; /* Pointer to Flag indicating whether to send 
                                Output Buffer Msg on SB - Don't Care for Input Messages */
} ECI_Msg_t;

/* FDC Reporting Interface Structure */
typedef struct {
  uint8*   FlagID;     /* Pointer to Flag Id  - unique id set by the user */
  boolean* StatusFlag; /* Pointer to status flag */
} ECI_Flag_t;

/* EVS Interface Structure */
typedef struct {
  uint8 eventBlock;   /* Event Block describes how many data points  */
  uint8* eventID;     /* Event Id  - unique id set by the user*/
  uint8* eventType;   /* Event Type - debug, info, error, crit set by user */
  uint32* eventMask;  /* Event Mask - filter set by user */
  boolean* eventFlag; /* Flag indicating simulink event has occurred */
  uint8* eventMsg;    /* Msgpoint to send with an event taken from observable signal */     
  char* loc;          /* Location string */
  double* data_1;     /* First data point */
  double* data_2;     /* Second data point */
  double* data_3;     /* Third data point */
  double* data_4;     /* Fourth data point */
  double* data_5;     /* Fifth data point */
} ECI_Evs_t;

/* Table Interface Structure */
typedef struct{
    void**  tblptr;       /* Pointer to table  */
    char*   tblname;      /* Name of table  */
    char*   tbldesc;      /* Description of table  */
    char*   tblfilename;  /* Filename of table  */
    uint32  tblsize;      /* Size of table */
    void*   tblvalfunc;   /* Table validation func */
}ECI_Tbl_t;

/* Critical Data Store Structure */
typedef struct
{
   char*  cdsname;         /* Name of CDS block */
   size_t cdssiz;          /* Size of CDS block */
   void*  cdsptr;          /* Address of Critical Data  */
}ECI_Cds_t;

/* Time Interface Structure */
typedef CFE_TIME_SysTime_t ECI_TimeStamp_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ECI_APP_H */

/************************/
/*  End of File Comment */
/************************/
