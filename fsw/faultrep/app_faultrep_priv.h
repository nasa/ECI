/*
** 
** Purpose:  Define a class that provides a mechanism for objects to report faults
**
** $Id: app_faultrep_priv.h 1.1 2016/05/10 15:29:12EDT myang Exp  $
**
** Notes:
**   1. This is a private header. See app_faultrep.h's notes for public
**      interface details.
**
** References:
**   1. Core Flight Executive Application Developers Guide.
**   2. The GN&C FSW Framework Programmer's Guide
**
*/

#ifndef  _app_faultrep_priv_
#define  _app_faultrep_priv_

/*
** Include Files
*/

#include "app_faultrep.h"

/*
** Type definitions
*/


/*
** Fault Reporter Ground Commands
*/

typedef struct
{

   uint16   FaultDetId;      /* Single identifier or APP_FAULTREP_SELECT_ALL */

} App_FaultRep_ClearFaultDetCmdParam;


typedef struct
{

   uint16   FaultDetId;      /* Single FD identifier or APP_FAULTREP_SELECT_ALL */
   bool  Enable;          /* TRUE - Enable a FD; FALSE - Disable a FD        */

} App_FaultRep_ConfigFaultDetCmdParam;


/*
** Exported Functions
*/

/*
** Purpose: Clear the latched status flag and the status flag in the report
**          packet for a single fault detector ID or for all of the IDs.
**
** Notes:
**   1. The function signature must comply with the GNC framework's
**      command interface standard (See cmdmsg.h).
**   2. If the command parameter fault ID is set to APP_FAULTREP_SELECT_ALL then
**      all fault detector IDs are affected otherwise it'sinterpreted as a
**      single ID.
**
** Return:
**   - TRUE  - Command accepted purpose achieved.
**   - FALSE - Command rejected: An event message is issued describing the
**             cause of the failure.
*/
bool App_FaultRep_ClearFaultDetCmd(      void* CmdObjPtr,  /**< Pointer to an instance of a App_FaultRep_Class */
                                      const void* CmdParamPtr /**< Pointer to a App_FaultRep_ClearCmd structure.  */
                                     );



/*
** Purpose:  Configure a fault detector to be enabled or disabled.
**
** Notes:
**   1. The function signature must comply with the GNC framework's
**      command interface standard (See cmdmsg.h).
**   2. If the command parameter fault ID is set to APP_FAULTREP_SELECT_ALL then
**      all fault detector IDs are affected otherwise it'sinterpreted as a
**      single ID. The EnableFlag is defined as:
**      - TRUE  - Enable the fault detector
**      - FALSE - Disable the fault detector
**
** Return:
**   - TRUE  - Command accepted purpose achieved.
**   - FALSE - Command rejected: An event message is issued describing the
**             cause of the failure.
*/
bool App_FaultRep_ConfigFaultDetCmd(      void* CmdObjPtr,  /**< Pointer to an instance of a App_FaultRep_Class */
                                       const void* CmdParamPtr /**< Pointer to a App_FaultRep_ConfigCmd structure. */
                                      );


/*
** Purpose:  Initialize the App_FaultRep object.
**
** Notes:
**   1. This function must be called prior to any other App_FaultRep_ functions
**   2. This is private because only one App_FaultRep needs to exist for an 
**      application and it is automatically created by the framework.
**   3. The App_FaultRep object is initialized to a known state. If the
**      App_FaultRep state should be preserved across some event then
**      the object managing this object should control when the
**      constructor routine is called. The telemetry reporting mode
**      default is APP_FAULTREP_NEW_REPORT.
**
*/
void App_FaultRep_Constructor(App_FaultRep_Class*  FaultRepObjParam, /**< Pointer to a App_FaultRep object */
                              uint16               FaultIdCnt,       /**< Number of fault detector IDs used (not an index, but a count) */
                              uint16*              EvsIdBase         /**< Starting event message ID        */
                             );



/*
** Purpose:  Update the fault report telemetry message.
**
** Notes:
**   1. This function MUST only be called once for each Application control
**      cycle. It MUST be called after all fault detectors have executed
**      and prior to ground command processing.
**   2. An Applicaiton can use App_FaultRep_SetTlmMode to change the behavior 
**      of this function. See App_FaultRep_SetTlmModes prologue.
**   3. This function signature must comply with TlmGen's telemetry
**      function interface definition (See tlmgen.h).
**   4. TlmGen manages the packet's time stamp.
**
*/
void App_FaultRep_GenTlmMsg(void*            TlmObjPtr, /**< Pointer to an instance of a App_FaultRep_Class */
                            CFE_SB_MsgPtr_t  TlmMsgPtr  /**< Pointer to App_FaultRep's telemetry message.   */
                           );



#endif  /* _app_faultrep_priv_ */
