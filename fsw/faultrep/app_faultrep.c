/* 
** File:
**   $Id: app_faultrep.c 1.1 2016/05/10 15:29:11EDT myang Exp  $
**
** Purpose: Implement the fault reporter class.
**
** Notes
**   1. This is an Application service object which means it has functions
**      that are directly callable from any other object contained in the
**      application. Orginally this object was implemented as a "singleton." 
**      A singleton is implemented by the  Constructor storing a static pointer
**      to the global instance. The "service" functions called by other objects
**      do not pass a reference to the object as the first parameter.  However,
**      in order to allow multiple applications to share a single framework
**      object library the singleton design was abandoned. This requires each
**      object that calls FaultRep to have access to it. In many cases a
**      "higher" level object can monitor the state of lower level objects so
**      this does not creat a terible burdon.
**   2. There are several (uint16) casts that are required to prevent compiler
**      warnings most are due to the compiler assuming a signed integer result
**      for integer-based math operations.
**
** References:
**   1. Core Flight Executive Application Developers Guide.
**   2. The GN&C FSW Framework Programmer's Guide
**
*/

#include "app_faultrep_priv.h"

#include "cfe_psp.h"
#include "cfe_evs.h"
#include "cfe_sb.h"


/*
** Macro Definitions
*/

#define EVS_ID(Offset)   ((uint16)(FaultRepObj->EvsIdBase  + Offset)) 

/*
** Type Definitions
*/

typedef struct
{

   uint16  WordIndex;
   uint16  Mask;

} FaultDetBitStruct;


/* 
** Local Function Prototypes
*/

static bool GetFaultDetIdBit(App_FaultRep_Class*  FaultRepObj,
                                const char*          CallerStr,
                                uint16               FaultDetId,
                                FaultDetBitStruct*   FaultDetBit);
                         
/*
** Function definitions
*/

/******************************************************************************
** Function: App_FaultRep_ClearFaultDetCmd
**
** Notes:
**    1. Must clear both the Software Bus report packet and NewReport.
*/
bool App_FaultRep_ClearFaultDetCmd(      void*  CmdObjPtr, 
                                      const void*  CmdParamPtr)
{

   App_FaultRep_Class* FaultRepObj = (App_FaultRep_Class*)CmdObjPtr;
   App_FaultRep_ClearFaultDetCmdParam*  CmdParam = (App_FaultRep_ClearFaultDetCmdParam*)CmdParamPtr;

   bool            RetStatus = true;
   FaultDetBitStruct  FaultDetBit;

   if (CmdParam->FaultDetId == APP_FAULTREP_SELECT_ALL)
   {

      CFE_PSP_MemSet(&(FaultRepObj->FaultDet.Latched),0,sizeof(FaultRepObj->FaultDet.Latched));

      for (FaultDetBit.WordIndex=0; FaultDetBit.WordIndex < APP_FAULTREP_BITFIELD_WORDS; FaultDetBit.WordIndex++)
      {

         FaultRepObj->NewReport.Data[FaultDetBit.WordIndex] = 0;
         FaultRepObj->SbMsg.Tlm.Data[FaultDetBit.WordIndex] = 0;

      } /* End LatchIndex loop */

   } /* End if select all */

   else 
   {

      RetStatus = GetFaultDetIdBit(FaultRepObj,
                                   "Fault Reporter Rejected Clear Detector Cmd: ",
                                   CmdParam->FaultDetId,
                                   &FaultDetBit);
      
      if (RetStatus == true)
      {

         FaultDetBit.Mask = (uint16)~FaultDetBit.Mask;

         FaultRepObj->FaultDet.Latched[FaultDetBit.WordIndex] &= FaultDetBit.Mask;

         FaultRepObj->NewReport.Data[FaultDetBit.WordIndex] &= FaultDetBit.Mask;
         FaultRepObj->SbMsg.Tlm.Data[FaultDetBit.WordIndex] &= FaultDetBit.Mask;
      
      } /* End if valid ID */

   } /* End if individual ID */


   return RetStatus;


} /* End App_FaultRep_ClearFaultDetCmd() */



/******************************************************************************
** Function: App_FaultRep_ConfigFaultDetCmd
**
** Notes:
**    None
*/
bool App_FaultRep_ConfigFaultDetCmd(      void*  CmdObjPtr, 
                                       const void*  CmdParamPtr)
{

   App_FaultRep_Class* FaultRepObj = (App_FaultRep_Class*)CmdObjPtr;
   App_FaultRep_ConfigFaultDetCmdParam*  CmdParam = (App_FaultRep_ConfigFaultDetCmdParam*)CmdParamPtr;

   bool  RetStatus = true;

   FaultDetBitStruct  FaultDetBit;

   if (CmdParam->Enable == true || CmdParam->Enable == false)
   {

      if (CmdParam->FaultDetId == APP_FAULTREP_SELECT_ALL) 
      {
         
         if (CmdParam->Enable)
         {
            for (FaultDetBit.WordIndex=0; FaultDetBit.WordIndex < FaultRepObj->FaultDet.BitfieldWords; FaultDetBit.WordIndex++)
               FaultRepObj->FaultDet.Enabled[FaultDetBit.WordIndex] = 0xFFFF;

            if (FaultRepObj->FaultDet.BitfieldWords < APP_FAULTREP_BITFIELD_WORDS)
               FaultRepObj->FaultDet.Enabled[FaultRepObj->FaultDet.BitfieldWords] = FaultRepObj->FaultDet.BitfieldRemMask;

         }
         else
         {
            CFE_PSP_MemSet(&(FaultRepObj->FaultDet.Enabled),0,sizeof(FaultRepObj->FaultDet.Enabled));
         }

         
      } /* End if select all */
      
      else 
      {
         
         RetStatus = GetFaultDetIdBit(FaultRepObj,
                                      "Fault Reporter Reject Config Detector Cmd:",
                                      CmdParam->FaultDetId,
                                      &FaultDetBit);
         
         if (RetStatus == true)
         {
            
            if (CmdParam->Enable)
               FaultRepObj->FaultDet.Enabled[FaultDetBit.WordIndex] |= FaultDetBit.Mask;
            
            else
               FaultRepObj->FaultDet.Enabled[FaultDetBit.WordIndex] &= ~FaultDetBit.Mask;
            
         } /* End if valid ID */
         
      } /* End if individual ID */
   } /* End if valid boolean range */
   else
   {

      CFE_EVS_SendEvent (EVS_ID(APP_FAULTREP_EVS_CONFIG_CMD_ERR),
                         CFE_EVS_EventType_ERROR,
                         "Fault Reporter Reject Config Detector Cmd: Invalid enable value %d",
                         CmdParam->Enable);

      RetStatus = false;

   } /* End if invalid boolean raange */
  

   return RetStatus;


} /* End App_FaultRep_ConfigFaultDetCmd() */


/******************************************************************************
** Function: App_FaultRep_Constructor
**
** Notes:
**    None
*/
void App_FaultRep_Constructor(App_FaultRep_Class*  FaultRepObj,
                              uint16               FaultIdCnt,
                              uint16*              EvsIdBase)
{

   uint16 RemBitCnt, i;

   /*
   ** Clear entire check structure which disables all detectors and
   ** clears their latch flags.
   */
   
   CFE_PSP_MemSet(FaultRepObj,0,sizeof(App_FaultRep_Class));

   FaultRepObj->FaultDet.IdLimit = FaultIdCnt;
   FaultRepObj->FaultDet.BitfieldWords = (uint16)(FaultIdCnt / APP_FAULTREP_BITS_PER_WORD);
   
   RemBitCnt = (uint16)(FaultIdCnt % APP_FAULTREP_BITS_PER_WORD);
   for (i=0; i < RemBitCnt; i++)
   {
      FaultRepObj->FaultDet.BitfieldRemMask |= 1 << i;
   }

   FaultRepObj->TlmMode = APP_FAULTREP_NEW_REPORT;
      
   FaultRepObj->EvsIdBase = *EvsIdBase;
   *EvsIdBase += APP_FAULTREP_EVS_MSG_CNT;

} /* End App_FaultRep_Constructor() */



/******************************************************************************
** Function: App_FaultRep_FaultDetFailed
**
** Notes:
**    1. Errors in this functions should only occur during integration. No
**       return status is provided because the caller always expects the call
**       to be successful.
*/
void App_FaultRep_FaultDetFailed(App_FaultRep_Class*  FaultRepObj,
                                 uint16 FaultDetId)
{

   bool            ValidFaultDetId;
   FaultDetBitStruct  FaultDetBit;

      
   ValidFaultDetId = GetFaultDetIdBit(FaultRepObj,
                                      "Fault Reporter Rejected Detector Failure:",
                                      FaultDetId,
                                      &FaultDetBit);
      
   if (ValidFaultDetId == true)
   {

      if (FaultRepObj->FaultDet.Enabled[FaultDetBit.WordIndex] & FaultDetBit.Mask)
      {
         
         FaultRepObj->FaultDet.Latched[FaultDetBit.WordIndex] |= FaultDetBit.Mask;
            
         FaultRepObj->NewReport.Data[FaultDetBit.WordIndex] |= FaultDetBit.Mask;
            
      } /* End if enabled */
         
   } /* End if valid detector ID */


} /* End App_FaultRep_FaultDetFailed() */


/******************************************************************************
** Function: App_FaultRep_GenTlmMsg
**
** Notes:
**    1. Logic assumes FAULTDET_REPORT_MODE has two states.
**
*/
void App_FaultRep_GenTlmMsg(void* TlmObjPtr, CFE_SB_MsgPtr_t TlmMsgPtr)
{

   uint16  i;
   App_FaultRep_Class* FaultRepObj = (App_FaultRep_Class*)TlmObjPtr;
   App_FaultRep_SbMsg* FaultRepMsg = (App_FaultRep_SbMsg*)TlmMsgPtr;

   /*
   ** Generate the FD report packet
   ** - Merge or copy NewTlm into the telemetry packet
   ** - Clear NewTlm for the next control cycle
   */

   if (FaultRepObj->TlmMode == APP_FAULTREP_MERGE_REPORT)
   {

      for (i=0; i < APP_FAULTREP_BITFIELD_WORDS; i++)
         FaultRepMsg->Tlm.Data[i] |= FaultRepObj->NewReport.Data[i];

   } /* End if APP_FAULTREP_MERGE_REPORT */
   else 
   {

      CFE_PSP_MemCpy(&(FaultRepMsg->Tlm),&(FaultRepObj->NewReport),sizeof(App_FaultRep_Data));

   } /* End if APP_FAULTREP_NEW_REPORT */

   CFE_PSP_MemSet(&(FaultRepObj->NewReport),0,sizeof(App_FaultRep_Data));


} /* End App_FaultRep_GenTlmMsg() */


/******************************************************************************
** Function: App_FaultRep_SetTlmMode
**
** Notes:
**    None
**
*/
void App_FaultRep_SetTlmMode(App_FaultRep_Class*   FaultRepObj,
                             App_FaultRep_TlmMode  TlmMode)
{


   FaultRepObj->TlmMode = TlmMode;


} /* End App_FaultRep_SetTlmMode() */



/******************************************************************************
** Function: GetFaultDetIdBit
**
** Notes:
**    1. If the ID is invalid (too big) then an event message is sent.
**
*/
static bool GetFaultDetIdBit(App_FaultRep_Class*  FaultRepObj,
                                const char*          CallerStr,
                                uint16               FaultDetId,
                                FaultDetBitStruct*   FaultDetBit)
{

   bool  RetStatus = true;


   if (FaultDetId < FaultRepObj->FaultDet.IdLimit)
   {
   
      FaultDetBit->WordIndex = (uint16)(FaultDetId/APP_FAULTREP_BITS_PER_WORD);
      FaultDetBit->Mask = (uint16)(1 << (uint16)(FaultDetId % APP_FAULTREP_BITS_PER_WORD));
   
   }
   else
   {

      RetStatus = false;
      CFE_EVS_SendEvent (EVS_ID(APP_FAULTREP_EVS_INV_DETECTOR_ID),
                         CFE_EVS_EventType_ERROR,
                         "%s Invalid fault ID %d (Max ID = %d)",
                         CallerStr,
                         FaultDetId,
                         FaultRepObj->FaultDet.IdLimit-1);
   }


   return RetStatus;


} /* End GetFaultIdBit() */


/* end of file */
