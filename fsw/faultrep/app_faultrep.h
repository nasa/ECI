/** \file
** 
** \brief  Define a class that provides a mechanism for objects to report faults
**
** $Id: app_faultrep.h 1.1 2016/05/10 15:29:12EDT myang Exp  $
**
** \note
**   -# Architecturally this class serves as an Interface. The Report could
**      be viewed as the Entity object being managed by the App_FaultRep
**      interface. Because of the simplicity of the Report it was defined
**      in this file rather than in a separate file.
**   -# App_FaultRep_Constructor() must be called prior to any other App_FaultRep_ functions
**   -# A higher level object (typically the Application's "main" file) must
**      register App_FaultRep_GenTlmMsg() with TlmGen.
**   -# The telemetry generation requires that an even number of fault
**      detection points be defined. This file requires the number of fault
**      detection points to be a multiple of 16 so the telemetry generation
**      restriction should always be satisfied.
**   -# The actual fault detection IDs are not known until the GN&C
**      application is integrated. 
**   -# Typically multiple detection points are "ORed" together for a single
**      fault correction monitor point. This object assumes the combining
**      of fault detection points is accomplished outside of this object.
**   -# The ReportMode flag has the following definitions
**      - APP_FAULTREP_NEW_REPORT - The ID notifications for the current control cycle
**        are copied into the message.
**      - APP_FAULTREP_MERGE_REPORT - The ID notifications for the current control cycle
**        are merged(logically ORed) with the message
**   -# Each Application can customized the following macros:
**      - APP_FAULTREP_ID_MAX
**
** References:
**   -# Core Flight Executive Application Developers Guide.
**   -# The GN&C FSW Framework Programmer's Guide
**
*/

/** 
** @addtogroup framework_gnc
** @{
*/

#ifndef  _app_faultrep_
#define  _app_faultrep_

/*
** Include Files
*/

#include "common_types.h"
#include "cfe_sb.h"

/*
** Macro Definitions
*/


#ifdef MISSION_FAULTREP_ID_MAX
   #define APP_FAULTREP_ID_MAX MISSION_FAULTREP_ID_MAX
#else
   #define APP_FAULTREP_ID_MAX 80
#endif

/*
** Event Service 
*/

#define  APP_FAULTREP_EVS_INV_DETECTOR_ID      0
#define  APP_FAULTREP_EVS_CONFIG_CMD_ERR       1

#define  APP_FAULTREP_EVS_MSG_CNT              2


/*
** The following macros define all possible fault detection points. Applications
** must use these definitions when using package functions that require a
** FaultDetId.
*/

#define  APP_FAULTREP_SELECT_ALL  0xFFFF  /* Used by functions to select all IDs */
                                          /* Must not be a valid detector ID     */

/*
** Define constants that are based on the total number of fault detection
** points:
**
** - APP_FAULTREP_ID_MAX         = Total number of IDs (1..LIMIT). This must
**                                 be a multiple of APP_FAULTREP_BITS_PER_WORD.
**
** - APP_FAULTREP_BITFIELD_WORDS = Number of words that are used to hold
**                                 bit information
*/

#define APP_FAULTREP_BITS_PER_WORD    16
#define APP_FAULTREP_BITFIELD_WORDS   (APP_FAULTREP_ID_MAX/APP_FAULTREP_BITS_PER_WORD)

#if (APP_FAULTREP_ID_MAX % APP_FAULTREP_BITS_PER_WORD) != 0
   #error APP_FAULTREP_ID_MAX must be a multiple of APP_FAULTREP_BITS_PER_WORD
#endif

/*
** Type definitions
*/


/*
** Report types used by App_FaultRep_SendTlmMsg()
*/

typedef enum
{

   APP_FAULTREP_NEW_REPORT,   /**< Output a new report                     */
   APP_FAULTREP_MERGE_REPORT  /**< Output a new report OR'ed with previous */

} App_FaultRep_TlmMode;



/*
** Report packet to be monitored by another Application
*/

typedef struct
{

   uint16  Data[APP_FAULTREP_BITFIELD_WORDS];  /* Bit packed status */

} App_FaultRep_Data;


typedef struct
{

   uint8          Hdr[CFE_SB_TLM_HDR_SIZE];

   App_FaultRep_Data  Tlm;

} App_FaultRep_SbMsg;


/*
** Data structures for the Fault Reporter Status
**
** - A single bit is used for each fault detection point for the enable/disable
**   configuration and for latched status.
**
** - A latched bit is set when an enabled fault detector notifies App_FaultRep of 
**   an error. The bit remains set until a command is received to clear the bit.
**
*/

typedef struct
{

   uint16   IdLimit;
   
   uint16   BitfieldWords;
   uint16   BitfieldRemMask;

   uint16   Enabled[APP_FAULTREP_BITFIELD_WORDS];   /* 0 = Disabled, 1 = Enabled */
   uint16   Latched[APP_FAULTREP_BITFIELD_WORDS];   /* 0 = No Error, 1 = Error   */

} App_FaultRep_FaultDet;


/*
** Fault Reporter Class Definition
*/

typedef struct
{

   uint16                 EvsIdBase;
   App_FaultRep_TlmMode   TlmMode;
   App_FaultRep_FaultDet  FaultDet;
   App_FaultRep_Data      NewReport;  /* Collected between SendTlmMsg() calls */
   App_FaultRep_SbMsg     SbMsg;      /* Last SB message sent                 */

} App_FaultRep_Class;


/*
** Exported Functions
*/


/**
** \brief  Report that a fault detector failed
**
** \note
**   -# A reference to a App_FaultRep object is not required
**      to avoid all Application objects requiring access
**      to a global object.
**   -# Limit checking is performed on the FaultDetId but this type
**      of error should only occur during integration.
**
** \param[in,out]  FaultRepObj  Pointer to a App_FaultRep object
** \param[in]      FaultDetId   Identifier of the fault detector
**
** \returns
** \retcode void \endcode
** \endreturns
*/
void App_FaultRep_FaultDetFailed(App_FaultRep_Class*  FaultRepObj,
                                 uint16               FaultDetId);



/**
** \brief  Set the telemetry reporting mode.
**
** \note
**   -# If fault detectors could not execute during the current control
**      cycle then ReportMode should be set to APP_FAULTREP_MERGE_REPORT which 
**      allows consecutive counters in an external monitoring appication to
**      continue to count for a detected fault.
**
** \param[in,out]  FaultRepObj  Pointer to a App_FaultRep object
** \param[in]      TlmMode      Set reporting mode to new or merged
**
** \returns
** \retcode void \endcode
** \endreturns
*/
void App_FaultRep_SetTlmMode(App_FaultRep_Class*  FaultRepObj,
                             App_FaultRep_TlmMode TlmMode);



#endif  /* _app_faultrep_ */
/** @} */
