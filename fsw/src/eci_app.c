/**
 * @file
 * @brief Interface implementation between app-specified Simulink-generated code and cFE
 */

/*************************************************************************
** Includes
*************************************************************************/

/* CFE headers */
#include "cfe.h"
#include "cfe_tbl_msg.h"

/* Std-Lib headers */
#include <string.h>
#include <math.h>

/* Config-independent ECI headers */
#include "eci_app_cfg.h"
#include "eci_app.h"
#include "eci_util_macro.h"
#include "eci_app_event.h"

/* External-code interface definition */
#include "eci_interface.h"

/* Config-dependent ECI headers */
/* The headers included here must be included after eci_interface.h
 * which contains configuration macros which affect the definitions 
 * in the following header files.
 */
#include "eci_app_msgdefs.h"
#include "eci_app_hk.h"

/* Only include status reporting definitions if enabled */
#ifdef ECI_FLAG_TABLE_DEFINED
#include "app_faultrep_priv.h"
#endif

/* Define ECI Parameter Table Elements if App uses Parameter Tables */
#ifdef ECI_PARAM_TBL_DEFINED

#ifndef PARAM_TBL_PATH_PREFIX
#error "Parameter Table File Directory Path (PARAM_TBL_PATH_PREFIX) Not Defined!"
#endif

#endif /*ECI_PARAM_TBL_DEFINED */

/************************************************************************
** Macro Definitions
*************************************************************************/

#define CMD_SEQUENCE_MASK              0x3FFF   /* Sequence number mask for CCSDS Header Sequence Field */

/* Define Default command code range beginning */
#ifndef ECI_FUNC_CODE_START
#define ECI_FUNC_CODE_START  100
#endif

/* Define Default command code range ending */
#ifndef ECI_FUNC_CODE_END
#define ECI_FUNC_CODE_END  110
#endif

/* Pre-processor error if command code range invalid */
#if ECI_FUNC_CODE_END < ECI_FUNC_CODE_START
#error "Invalid Function Code Range"
/*
TODO:  Protection for reserved command code values?
#elif ECI_FUNC_CODE_START <= ECI_FAULTREP_CLEAR_CC
#error "Function Code Range Conflicts with Reserved Values"
*/
#endif

/* Command code for internal cFE table management notification command */
#define ECI_TBL_MANAGE_CC              12

/************************************************************************
** Type/Enum Definitions
*************************************************************************/

/* Enumeration for Either Command or Data SB Pipe */
typedef enum
{
    CMDPIPE = 0,     /* Command Pipe */
    DATAPIPE = 1     /* Data Pipe */
    
}SB_PipeType_t;

/* Message Validity & Status Check Enums */
typedef enum {

   BUFFERED,      /* Message On Input/Output Buffer */
   QUEUED,        /* Message Queued */
   QUEUEFULL,     /* Message Queue Full */
   QUEUEEMPTY,    /* Message Queue Empty */

} ECI_MsgControl_t; 

/* Message Length Verification Enums */
typedef enum { 

   EQUAL,            /* Message Lengths Equal  */
   LESS_THAN_EQUAL   /* Received Message Length Less than Expected */

} ECI_MsgLengthVerify_t; 

/* Internal Structure Definition for Telemetry/Command Message Processing */
typedef struct {
    const ECI_Msg_t*       MsgStruct;  /* Pointer to ECI_MsgRcv in eci_interface.h  */

    int                    ecnt;       /* Message error counter */
    int                    qhd;        /* Index into first element in cmd queue */
    int                    qtl;        /* Index into last element in cmd queue */
    int                    qcnt;       /* Number of messages in cmd queue */
    boolean                qexists;    /* Flag verifying existence of cmd queue */
    uint16                 seq;        /* Sequence number for command msgs */
} ECI_InternalMsg_t;

/*
 * ECI app internal data definition
 */
typedef struct {

   ECI_HkPacket_t HkPacket;       /* Housekeeping telemetry packet */

#ifdef ECI_FLAG_TABLE_DEFINED
   App_FaultRep_SbMsg  FDCPacket; /*  FDC Telemetry Packet */   
   App_FaultRep_Class  FaultRep;  /*  Fault Reporter Object */
#endif

   CFE_SB_MsgPtr_t MsgPtr;        /*  Operational data (not reported in housekeeping). */  
   CFE_SB_MsgPtr_t DataMsgPtr;    /*  Data Pipe (not reported in housekeeping). */
   CFE_SB_PipeId_t CmdPipe;       /*  Software Command Pipe Id */  
   CFE_SB_PipeId_t DataPipe;      /*  Software Data Pipe Id */
   uint32 RunStatus;              /*  RunStatus variable used in the main processing loop */

#ifdef ECI_CDS_TABLE_DEFINED
   CFE_ES_CDSHandle_t CDSHandle[SIZEOF_ARRAY(ECI_CdsTable) - 1];  /*  Handle to CDS memory block */
   boolean            CDS_Avail;  /* Flag indicating whether CDS is available */
#endif

#ifdef ECI_FLAG_TABLE_DEFINED
   uint16  EVSBaseId;
#endif

#ifdef ECI_EVENT_TABLE_DEFINED
   CFE_EVS_BinFilter_t EventFilters[SIZEOF_ARRAY(ECI_Events) - 1];   /* Event Filter Tables */
#endif

}ECI_AppData_t;

/************************************************************************
 ** Static Data
 ************************************************************************/

/* Define ECI Parameter Table Elements if App uses Parameter Tables */
#ifdef ECI_PARAM_TBL_DEFINED
/*FIXME: Move to AppData?? */
static CFE_TBL_Handle_t TableHandle_Param[SIZEOF_ARRAY(ECI_ParamTable) - 1];                  /* Handles for ECI Parameter Table. */
static char             TblFullPathParam[SIZEOF_ARRAY(ECI_ParamTable) - 1][OS_MAX_PATH_LEN];  /* Full Path to Table Default Data Files */

#endif /*ECI_PARAM_TBL_DEFINED */


/* Define ECI State Table Elements if App uses State Table */
#ifdef ECI_STATE_TBL_DEFINED

#ifndef STATE_TBL_PATH_PREFIX
#error "State Table File Directory Path Not Defined!"
#endif
/*FIXME: Move to AppData?? */
static CFE_TBL_Handle_t TableHandle_State;                                          /* Handle for ECI State Table. */
static char             TblFullPathState[OS_MAX_PATH_LEN] = STATE_TBL_PATH_PREFIX;  /* Full Path to Table Default Data File */

#endif

/* If event tables defined, will define flags for determining 
   if event should be issued during the current 
   wake-up based on value after previous wake-up  */
#ifdef ECI_EVENT_TABLE_DEFINED
/*FIXME: Move to AppData?? */
static uint16 prevEventFlag[SIZEOF_ARRAY(ECI_Events) - 1];
#endif

/* Internal structures for Telemetry Messages */
/*FIXME: Move to AppData?? */
static ECI_InternalMsg_t MsgRcv[SIZEOF_ARRAY(ECI_MsgRcv) - 1];

/* ECI App Internal Data */
static ECI_AppData_t ECI_AppData;

/*************************************************************************
**
** Exported data
**
**************************************************************************/
/* Global Step TimeStamp Data */
ECI_TimeStamp_t ECI_Step_TimeStamp;


/*************************************************************************
**
** Exported Function Prototype
**
**************************************************************************/
void ECI_APP_MAIN(void);

/*************************************************************************
**
** Local Functions
**
**************************************************************************/

/*****************************************************************//**
 * Register Events for this app.
 *
 * \return CFE_SUCCESS if registration was successful.
 ********************************************************************/
static int32 register_events(void) {

   int32 status = CFE_SUCCESS;

#ifdef ECI_EVENT_TABLE_DEFINED

   /* Add on Auto-generated additional events */
   int i;

   /* Loop through app-specified event messages */
   for (i = 0; i < SIZEOF_ARRAY(ECI_Events) - 1 ; i++) {

      /* Set up Filter for Event */
      ECI_AppData.EventFilters[i].EventID =  (int16)(*(ECI_Events[i].eventID));
      ECI_AppData.EventFilters[i].Mask    = (uint16)(*(ECI_Events[i].eventMask));

      /* Initialize the previous Event Flag so that changes can be detected  */
      prevEventFlag[i]=0;

   } /* End for-loop */

   /* Register event filter table. */
   status = CFE_EVS_Register(ECI_AppData.EventFilters, (SIZEOF_ARRAY(ECI_Events) - 1), CFE_EVS_BINARY_FILTER);

#else

   /* If no app-specified event messages */
   status = CFE_EVS_Register(NULL, 0, 0);

#endif /* ECI_EVENT_TABLE_DEFINED */

   return (status);

} /* End of register_events() */

/*****************************************************************//**
 * Initializes/subscribes software bus messages.
 *
 * \return int32 - return state of CFE_SB_Subscribe()
 ********************************************************************/
static int32 sb_init(void)
{
   unsigned int idx;
   int status;

   /* Subscribe to app-specific ECI_CMD Msg */
   status = CFE_SB_Subscribe(ECI_CMD_MID, ECI_AppData.CmdPipe);

   /* Exits sb_init() for subscribe fails */
   if (status < CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(ECI_SUB_CMD_MID_ERR_EID, CFE_EVS_ERROR,
                       "Error Subscribing to ECI_CMD_MID 0x%04X, Status = 0x%08X", 
                        ECI_CMD_MID, status);
      return status;
   } /* End if-statement */

   /* Subscribe to app-specific house keeping command */ 
   status = CFE_SB_Subscribe(ECI_SEND_HK_MID, ECI_AppData.CmdPipe);

   /* Exits sb_init() for subscribe fails */
   if (status < CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(ECI_SUB_SEND_HK_MSG_ERR_EID, CFE_EVS_ERROR,
                       "ERROR Subscribing to ECI_SEND_HK_MID 0x%04X, Status = 0x%08X", 
                        ECI_SEND_HK_MID, status);
      return status;
   } /* End if-statement */

   /* Subscribe to cFE Table Update Notification command */ 
   status = CFE_SB_Subscribe(ECI_TBL_MANAGE_MID, ECI_AppData.CmdPipe);

   /* Exits sb_init() for subscribe fails */
   if (status < CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(ECI_SUB_TBL_MANAGE_MSG_ERR_EID, CFE_EVS_ERROR,
                       "ERROR Subscribing to ECI_TBL_MANAGE_MID 0x%04X, Status = 0x%08X", 
                        ECI_TBL_MANAGE_MID, status);
      return status;
   } /* End if-statement */

   /* Subscribe to Wake-up Message */
   status = CFE_SB_Subscribe(ECI_TICK_MID, ECI_AppData.CmdPipe);

   /* Exits sb_init() for subscribe fails */
   if (status < CFE_SUCCESS)
   {
      CFE_EVS_SendEvent(ECI_SUB_TICK_MSG_ERR_EID, CFE_EVS_ERROR,
                       "Error Subscribing to ECI_TICK_MID 0x%04X, Status = 0x%08X",
                        ECI_TICK_MID, status);
      return status;
   } /* End if-statement */
   
   /* Initialize MsgRcv Structure */
   CFE_PSP_MemSet(MsgRcv, 0, sizeof(ECI_InternalMsg_t) * (SIZEOF_ARRAY(ECI_MsgRcv) - 1));

   /* Setup Cmd/Tlm Msgs In */
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_MsgRcv) - 1; idx++)
   {
      /* Links to ECI_MsgRcv[] in eci_interface.h */
      MsgRcv[idx].MsgStruct = &ECI_MsgRcv[idx];

      /* Initialize the cmd queue */
      MsgRcv[idx].qcnt  = 0;
      MsgRcv[idx].qtl = 1;
      MsgRcv[idx].qhd = 1;
      MsgRcv[idx].seq = 0;
      MsgRcv[idx].qexists = FALSE;
        
      /* Identify messages with queue (all received cmd messages have queue) */
      if (CCSDS_SID_TYPE(MsgRcv[idx].MsgStruct->mid) == CCSDS_CMD)
      {
         /* Returns error if reserved mid used */
         /* FIXME: update macros*/
         if (MsgRcv[idx].MsgStruct->mid == ECI_SEND_HK_MID || 
             MsgRcv[idx].MsgStruct->mid == ECI_TBL_MANAGE_MID || 
             MsgRcv[idx].MsgStruct->mid == ECI_TICK_MID)
         {
            CFE_EVS_SendEvent(ECI_CMD_MSGID_RESERVE_ERR_EID, CFE_EVS_ERROR,
                             "Msg ID for CMD MID 0x%04X in ECI_MsgRcv[%d] is reserved value", 
                              MsgRcv[idx].MsgStruct->mid, idx);
            return (CFE_SUCCESS - 1);
         } /* End if-statement */

         /* Otherwise subscribe to command message */
         status = CFE_SB_Subscribe(MsgRcv[idx].MsgStruct->mid, ECI_AppData.CmdPipe);

         /* Exits sb_init() for subscribe fails */
         if (status < CFE_SUCCESS)
         {
            CFE_EVS_SendEvent(ECI_CMD_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                             "Error Subscribing to CMD MID 0x%04X in ECI_MsgRcv[%d] on CMD Pipe, Status = 0x%08X", 
                              MsgRcv[idx].MsgStruct->mid, idx, status);
            return status;

         } /* End if-statement */

         /* Cmd queue Status Verified */
         MsgRcv[idx].qexists = TRUE;

         /* Initialization cmd queue */
         CFE_PSP_MemSet(MsgRcv[idx].MsgStruct->qptr, 0, ECI_CMD_MSG_QUEUE_SIZE * MsgRcv[idx].MsgStruct->siz);

      } else { /* Subscribe to Messages in ECI_MsgRcv which are Telemetry Messages */

         /* Subscribe to telemetry message */
         status = CFE_SB_Subscribe(MsgRcv[idx].MsgStruct->mid, ECI_AppData.DataPipe);
      
         /* Exits sb_init() for subscribe fails */
         if (status < CFE_SUCCESS)
         {
            CFE_EVS_SendEvent(ECI_TLM_SUBSCRIBE_ERR_EID, CFE_EVS_ERROR,
                             "Error Subscribing to MsgID 0x%04X in ECI_MsgRcv[%d] on Data Pipe, Status = 0x%08X",
                              MsgRcv[idx].MsgStruct->mid, idx, status);
                                  
            return status;
         } /* End if-statement */
      } /* End if-else statement */
   } /* End for-loop */
    
   /* Init cFE Msg Outputted */
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_MsgSnd) - 1; idx++)
   {
      CFE_SB_InitMsg(ECI_MsgSnd[idx].mptr, ECI_MsgSnd[idx].mid, ECI_MsgSnd[idx].siz, TRUE);
   } /* End for-loop */

   return CFE_SUCCESS;

} /* End of sb_init() */

/*****************************************************************//**
 * Registers and loads app-specified parameter tables.
 *
 * \return int32 - Status of #CFE_TBL_Register.
 ********************************************************************/
static int32 param_table_register(void) {

   int32  param_status = CFE_SUCCESS;

#ifdef ECI_PARAM_TBL_DEFINED

   unsigned int idx;

   /* Loop through all the app-specified parameter tables */
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_ParamTable) - 1; idx++)
   {
      TableHandle_Param[idx] = CFE_TBL_BAD_TABLE_HANDLE;

      /* Sets up Table Data File Path name */
      strcpy(TblFullPathParam[idx], PARAM_TBL_PATH_PREFIX);
      strncat(TblFullPathParam[idx], ECI_ParamTable[idx].tblfilename, OS_MAX_PATH_LEN - sizeof(PARAM_TBL_PATH_PREFIX) - 1);

      /* Register parameter table with cFE table services */
      param_status = CFE_TBL_Register(&TableHandle_Param[idx],
                                      ECI_ParamTable[idx].tblname,
                                      ECI_ParamTable[idx].tblsize,
                                     (CFE_TBL_OPT_DBL_BUFFER | CFE_TBL_OPT_LOAD_DUMP),
                                      ECI_ParamTable[idx].tblvalfunc);

      /* Event message if table registration returns error */
      if (param_status != CFE_SUCCESS) {
         CFE_EVS_SendEvent(ECI_PARAM_TBL_REG_ERR_EID, CFE_EVS_ERROR,
         "ERROR Registering ECI_ParamTable[%d], Status = 0x%08X", idx, (int)param_status);
      } else {
         /* cFE table services will notify app if parameter table update */
         CFE_TBL_NotifyByMessage(TableHandle_Param[idx], ECI_TBL_MANAGE_MID, ECI_TBL_MANAGE_CC, idx);
      } /* End if-else statement */

   } /* End for-loop */

   return(param_status);

#else

   /* If app does not specify any parameter table */
   CFE_ES_WriteToSysLog(ECI_APP_NAME_UPPER ": App Does Not Register Param Table\n");
   return(param_status);

#endif /* ECI_PARAM_TBL_DEFINED */

} /* End of param_table_register() */

/*****************************************************************//**
 * Load data to app-specified parameter table(s).
 *
 * \return Status of #CFE_TBL_Load, #CFE_TBL_Manage or
 *         #CFE_TBL_GetAddress.
 ********************************************************************/
static int32 load_param_table(void) {
   int32 status = CFE_SUCCESS;

#ifdef ECI_PARAM_TBL_DEFINED

   unsigned int idx;

   /* Loop through all the app-specified parameter tables */
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_ParamTable) - 1; idx++)
   {
      /*
       * Load file table data to app-specified parameter tables
       */
      status = CFE_TBL_Load(TableHandle_Param[idx],
                            CFE_TBL_SRC_FILE,
                           (const void *)TblFullPathParam[idx]);

      /* Table Load returns error */
      if (status != CFE_SUCCESS) {
         CFE_EVS_SendEvent(ECI_PARAM_TBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                          "ERROR Loading ECI_ParamTable[%d] (%s), Status = 0x%08X",
                           idx, ECI_ParamTable[idx].tblname, (int)status);
                              
         break; /* Exit and Return an Error */

      } /* End if statement */

      /* 
       * Manage the table - this routine calls the table update routine
       * to copy the table from the working buffer to the active buffer.
       */ 
      status = CFE_TBL_Manage(TableHandle_Param[idx]);

      /* On first call, CFE_TBL_Manage will take no action, as the first time a 
       * table load is done, it does not require management. Thus, we support
       * using this function separately in the future.
       */
      if (status == CFE_SUCCESS || status == CFE_TBL_INFO_UPDATED) {
         /* Get address of tables active buffer and lock against updates */
         status = CFE_TBL_GetAddress(ECI_ParamTable[idx].tblptr, TableHandle_Param[idx]);

         /* Release active buffer for future updates */
         CFE_TBL_ReleaseAddress(TableHandle_Param[idx]);

         /* This is the required status to take this action */
         if (status >= CFE_SUCCESS) {
            status = CFE_SUCCESS;
         } else {
            CFE_EVS_SendEvent(ECI_PARAM_TBL_GETADDR_ERR_EID, CFE_EVS_ERROR,
                              "ERROR Getting Address from cFE Table Services from ECI_ParamTable[%d] (%s), Status = 0x%08X",
                              idx, ECI_ParamTable[idx].tblname, (int)status);

            break;   /* Exit function and return error */
         } /* End if-else statement */

      } else {

         CFE_EVS_SendEvent(ECI_PARAM_TBL_MANAGE_ERR_EID, CFE_EVS_ERROR, 
                          "ERROR Managing ECI_ParamTable[%d] (%s), Status = 0x%08X", 
                           idx, ECI_ParamTable[idx].tblname, (int)status);

      } /* End if-else statement */

   } /* End for-loop */

#else
   CFE_ES_WriteToSysLog(ECI_APP_NAME_UPPER ": App Does Not Have a Registered Param Table to Load\n");
#endif

   return status;

} /* End of load_param_table() */

/*****************************************************************//**
 * Registers and loads state table
 *
 * \return int32 Status of CFE_TBL_Register() or CFE_TBL_Load().
 ********************************************************************/
static int32 state_table_register(void) {
   int32  state_status = CFE_SUCCESS;

#ifdef ECI_STATE_TBL_DEFINED
   TableHandle_State = CFE_TBL_BAD_TABLE_HANDLE;

   /* Sets up Table Data File Path name */
   strncat(TblFullPathState, ECI_TBL_FileDef_State.TgtFilename, OS_MAX_PATH_LEN - sizeof(STATE_TBL_PATH_PREFIX) - 1);

   /* Register State table as a DUMP ONLY TABLE where the active buffer is user provided */
   state_status = CFE_TBL_Register(&TableHandle_State,
                                   "STATE",
                                    ECI_TBL_FileDef_State.ObjectSize,
                                    CFE_TBL_OPT_DUMP_ONLY | CFE_TBL_OPT_USR_DEF_ADDR, 
                                    NULL);

   /* State table registration returns error */
   if (state_status != CFE_SUCCESS) {
      CFE_EVS_SendEvent(ECI_STATE_TBL_REG_ERR_EID, CFE_EVS_ERROR,
                       "ERROR Registering ECI STATE Table, Status = 0x%08X",
                        state_status);
   } else {
      /*
       * Perform a basic load in which the user address is provided - this tells CFE the address 
       * of the dump only table.
       */
      state_status = CFE_TBL_Load(TableHandle_State, CFE_TBL_SRC_ADDRESS, &ECI_STATE_TBL);

      if (state_status != CFE_SUCCESS) {
         CFE_EVS_SendEvent(ECI_STATE_TBL_LOAD_ERR_EID, CFE_EVS_ERROR,
                          "ERROR LOADING ECI STATE Table, Status = 0x%08X",
                           state_status);
      } /* End if statement */

   } /* End if-else statement */

#else
   CFE_ES_WriteToSysLog(ECI_APP_NAME_UPPER ": App Does Not Register State Table\n");
#endif /* ECI_STATE_TBL_DEFINED */

   return(state_status);

} /* End of state_table_register() */

/*****************************************************************//**
 * Initialize parameter and state tables
 *
 * \return Status of param_table_register(), state_table_register(), 
 *         and load_param_table()
 ********************************************************************/
static int32 table_init(void) {

   int32  param_status = CFE_SUCCESS;
   int32  state_status = CFE_SUCCESS;
   int32  status = CFE_SUCCESS;

   /* Register parameter tables */
   param_status = param_table_register();

   /* If parameter table succesfully registered */
   if (param_status == CFE_SUCCESS) {
      param_status = load_param_table();
   } /* End if statement */

   /* Register state table */
   state_status = state_table_register();

   /* 
    * Setup proper return. Return any error status
    * that may have occured. Individual calls above
    * provide user with detailed error information.
     */
   if (param_status != CFE_SUCCESS) {
      status = param_status;
   } else if (state_status != CFE_SUCCESS) {
      status = state_status;
   } /* End if-else if statement */

   /* 
    * If this point is reached, 
    * registers and loads were successful.
    */
   return status;

} /* End of table_init() */

/*****************************************************************//**
 * Initialize Critical Data Store (if available)
 *
 * \return Status of CFE_ES_RegisterCDS and/or CFE_ES_RestoreFromCDS
 ********************************************************************/
static int32 cds_init(void) {

   int32  status = CFE_SUCCESS;

#ifdef ECI_CDS_TABLE_DEFINED

   int idx;

   ECI_AppData.CDS_Avail = TRUE;

   /* Register CDS data */
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_CdsTable) - 1; idx++)
   {
      status = CFE_ES_RegisterCDS(&(ECI_AppData.CDSHandle[idx]), ECI_CdsTable[idx].cdssiz, ECI_CdsTable[idx].cdsname);

      /* If CDS data already exists, will be restored from CDS */
      if (status == CFE_ES_CDS_ALREADY_EXISTS)
      {
         CFE_EVS_SendEvent(ECI_CDS_RESTORE_INF_EID, CFE_EVS_INFORMATION, "ECI_CdsTable[%d] already exists in CDS and will be restored", idx);
         status = CFE_ES_RestoreFromCDS(ECI_CdsTable[idx].cdsptr, ECI_AppData.CDSHandle[idx]);

         /* If there is an error restoring from CDS */
         if (status < CFE_SUCCESS)
         {
            CFE_EVS_SendEvent(ECI_CDS_RESTORE_ERR_EID, CFE_EVS_ERROR, "ECI_CdsTable[%d] failed to restore from CDS, Status = 0x%08X", idx, (int)status);
         } /* End if-statement */

      } else if (status == CFE_ES_NOT_IMPLEMENTED) {  /* If CDS is unavailable */

         ECI_AppData.CDS_Avail = FALSE;
         CFE_EVS_SendEvent(ECI_CDS_NOT_AVAIL_INF_EID, CFE_EVS_INFORMATION, "CDS not available for ECI_CdsTable");
         break;

      } else {

         CFE_EVS_SendEvent(ECI_CDS_REGISTER_INF_EID, CFE_EVS_INFORMATION, "ECI_CdsTable[%d] registered to CDS", idx);

      } /* End if-else statement */
   } /* End for-statement */

   if (status >= CFE_SUCCESS || ECI_AppData.CDS_Avail == FALSE)
      status = CFE_SUCCESS;

#endif /* ECI_CDS_TABLE_DEFINED */

   return (status);

} /* End of cds_init() */

/*********************************************************************
 * Initializes cFE software bus pipes, events, messages, tables, fault
 * reporter and app-specified initializations
 *
 * \return CFE_SUCCESS if initialization was successful.
 ********************************************************************/
static int32 app_init(void)
{
   int32 status;
   uint32 resetType;

   CFE_PSP_MemSet(&ECI_AppData, 0, sizeof(ECI_AppData_t));

   /* Determine Reset Type */
   status = CFE_ES_GetResetType(&resetType);

   /* Setup the RunStatus variable */
   ECI_AppData.RunStatus = CFE_ES_APP_RUN;

   /* Initialize housekeeping packet (clear user data area).*/
   CFE_SB_InitMsg(&ECI_AppData.HkPacket, ECI_HK_MID, sizeof(ECI_HkPacket_t), TRUE);

#ifdef ECI_FLAG_TABLE_DEFINED 
   /* Initialize FDC packet */
   CFE_SB_InitMsg(&ECI_AppData.FDCPacket, ECI_FLAG_MID, sizeof(App_FaultRep_SbMsg), TRUE);
#endif

   /* Create Software Bus command message pipe. */
   status = CFE_SB_CreatePipe(&ECI_AppData.CmdPipe, 32, ECI_CMD_PIPE_NAME);

   /* If pipe creation returns an error */
   if (status != CFE_SUCCESS)
   {
      /* Could use an event at this point */
      CFE_ES_WriteToSysLog(ECI_APP_NAME_UPPER ": Error Creating SB Pipe, Status = 0x%08X\n", (int)status);
      return (status);
   } /* End if statement */

   /* Create Software Bus data message pipe. */
   status = CFE_SB_CreatePipe(&ECI_AppData.DataPipe, 32, ECI_DATA_PIPE_NAME);

   /* If pipe creation returns an error */
   if (status != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog(ECI_APP_NAME_UPPER ": Error Creating Data SB Pipe, Status = 0x%08X\n", (int)status);
      return (status);
   } /* End if statement */

#ifdef ECI_FLAG_TABLE_DEFINED 
   /* Initialize Fault Reporter */
   App_FaultRep_Constructor(&ECI_AppData.FaultRep, ECI_FLAG_MAX_ID, &ECI_AppData.EVSBaseId);
#endif

   /* Register the events */
   status = register_events();

   /* If event registration returns an error */
   if (status != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog(ECI_APP_NAME_UPPER ": Error Registering Events, Status = 0x%08X\n", (int)status);
      return (status);
   } /* End if statement */

   /* Init the Software Bus Messages */
   status = sb_init();

   /* If software bus message initialization returns an error */
   if (status != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog(ECI_APP_NAME_UPPER ": Error Initializing Software Buses and SB Messages, Status = 0x%08X\n", (int)status);
      return (status);
   } /* End if statement */

   /* Register param & state tables with cFE and load default data */
   status = table_init();

   /* If table initialization returns an error */
   if (status != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog(ECI_APP_NAME_UPPER ": Error Initializing Tables, Status = 0x%08X\n", (int)status);
      return (status);
   } /* End if statement */

   /* Initialize model if the user has defined an init function*/
#ifdef ECI_INIT_FCN
   ECI_INIT_FCN;
#endif

   /* Initialize CDS */
   status = cds_init();

   /* If cds initialization returns an error */
   if (status != CFE_SUCCESS)
   {
      CFE_ES_WriteToSysLog(ECI_APP_NAME_UPPER ": Error Initializing CDS, Status = 0x%08X\n", (int)status);
      return (status);
   } /* End if statement */

   /* Application startup event message. */
   CFE_EVS_SendEvent(ECI_INIT_INF_EID, CFE_EVS_INFORMATION, "Application Initialized, Revision %s", ECI_APP_REVISION_NUMBER);

   return (status);

} /* End of app_init() */

/*****************************************************************//**
 * Verifies msg packet length (cmd or tlm).
 *
 * \param[in] messageID = Message ID software bus message packet.
 * \param[in] actualLength = Actual length of packet.
 * \param[in] expectedLength = Expected length of packet.
 * \param[in] cmdMsgTypeCheck = specify type of check to be done (See ECI_MsgLengthVerify_t)
 *
 * \retval TRUE  = The message packet passes message length check.
 * \retval FALSE = The message packet failes message length check.
 ********************************************************************/
static boolean verify_msg_length(CFE_SB_MsgId_t messageID, uint16 actualLength, uint16 expectedLength, int cmdMsgTypeCheck) {

   boolean tlmFail,cmdFail, result = TRUE;
   int msgType;

   tlmFail = FALSE;
   cmdFail = FALSE;

   msgType = CCSDS_SID_TYPE(messageID);

   /*
    * Verify the command packet length using specified evaluation.
    */
   if (cmdMsgTypeCheck == LESS_THAN_EQUAL) {
      if ( msgType == CCSDS_TLM && actualLength > expectedLength) {
         tlmFail = TRUE;
      } /* End if statement */

      if ( msgType == CCSDS_CMD && actualLength > expectedLength) {
         cmdFail = TRUE;
      } /* End if statement */

   } else { /* DEFAULT is a check for not EQUAL */   
      if ( msgType == CCSDS_TLM && actualLength != expectedLength) {
         tlmFail = TRUE;
      } /* End if statement */

      if ( msgType == CCSDS_CMD && actualLength != expectedLength) {
         cmdFail = TRUE;
      } /* End if statement */
   } /* End if-else statement */

   /* Telemetry message length check failed */
   if (tlmFail) {
      CFE_EVS_SendEvent(ECI_TLM_LEN_ERR_EID, CFE_EVS_ERROR,
         "Invalid tlm pkt: ID = 0x%X, Len = %d (Expected = %d)",
         messageID, actualLength, expectedLength);
      result = FALSE;
   } /* End if statement */

   /* Command message length check failed */
   if (cmdFail) {
      CFE_EVS_SendEvent(ECI_CMD_LEN_ERR_EID, CFE_EVS_ERROR,
         "Invalid cmd pkt: ID = 0x%X, Len = %d (Expected = %d)",
         messageID, actualLength, expectedLength);
      result = FALSE;
   } /* End if statement */

   return (result);

} /* verify_msg_length() */

/*****************************************************************//**
 * Increments command accept counter and outputs no-op event
 ********************************************************************/
static void no_op_cmd(void) {

   CFE_EVS_SendEvent(ECI_NOOP_INF_EID, CFE_EVS_INFORMATION, "No-op command, Revision %s", ECI_APP_REVISION_NUMBER);
   ECI_AppData.HkPacket.CmdAcceptCounter++;

} /* End of no_op_cmd() */

/*****************************************************************//**
 * Resets HK counters
 ********************************************************************/
static void reset_hk_counters(void) {

   int idx;

   ECI_AppData.HkPacket.CmdAcceptCounter = 0;
   ECI_AppData.HkPacket.CmdErrorCounter = 0;

   /* Loops through and resets all message receive counters */
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_MsgRcv) - 1; idx++)
   {
      ECI_AppData.HkPacket.MsgRcvCnt[idx] = 0;
   } /* End for-loop */

} /* End of reset_hk_counters() */

/*****************************************************************//**
 * Ground command to reset HK counters.
 ********************************************************************/
static void reset_hk_counter_cmd(void) {

   reset_hk_counters();

   CFE_EVS_SendEvent(ECI_RESET_HK_CTR_INF_EID, CFE_EVS_INFORMATION, "Reset Housekeeping Counters");

} /* End of reset_hk_counter_cmd() */

/******************************************************************//**
 * Add data to ring buffer
 *
 * Pre-requisite:  Assumes that space available on ring buffer
 *
 * \param[in, out] qtl = Tail of the ring buffer
 * \param[in, out] qcnt = Number of items in the ring buffer
 * \param[in] data = data to be added to the ring buffer.
 * \param[in] data_size = size of the data
 * \param[in] q = ring buffer pointer
 * \param[in] qsize = total length of the ring buffer
 *
 *********************************************************************/
static void add_to_ring_buffer(int* qtl, int* qcnt, void* data, size_t data_size, char* q, int qsize) {

   char* qdes;

   /* Specify the destination in the queue*/
   qdes = q + (*qtl - 1) * data_size;

   /* Copy new data into queue */
   CFE_PSP_MemCpy(qdes, data, data_size);

   /* Increment tail counter */
   (*qtl)++;

   /* Increment message counter */
   (*qcnt)++;

   /* Check qtl for wrap over */
   if (*qtl > qsize) {
      *qtl = 1;
   } /* End if statement */

} /* add_to_ring_buffer() */

/******************************************************************//**
 * Enqueue CMD messages.
 *
 * \param[in] msg = Pointer to command message data
 * \param[in] idx = Index into global ECI_MsgRcv that contains pointer to queue
 *
 * Pre-requisite:
 *      1)  'msg' is a command message
 *      2)  'idx' is a valid index value
 *
 * \return #ECI_MsgControl_t
 *********************************************************************/
static ECI_MsgControl_t enqueue_cmd_msg(const CFE_SB_MsgPtr_t msg, const unsigned int idx) {

   /* Command Queue cannot hold any more messages */
   if (MsgRcv[idx].qcnt >= ECI_CMD_MSG_QUEUE_SIZE){

      return QUEUEFULL;

   } else { /* Command message added to Queue */

      add_to_ring_buffer(&(MsgRcv[idx].qtl), &(MsgRcv[idx].qcnt), msg, MsgRcv[idx].MsgStruct->siz, MsgRcv[idx].MsgStruct->qptr, ECI_CMD_MSG_QUEUE_SIZE);

      return QUEUED;

   } /* End if-else statement */

} /* end of enqueue_cmd_msg() */

/******************************************************************//**
 * Manage the counters/queues for TLM and CMD messages.
 *
 * \param[in] msg = Pointer to software bus message
 * \param[in] idx = Index into global MsgRcv that contains queue/buffer
 *
 * \return #ECI_MsgControl_t
 *********************************************************************/
static ECI_MsgControl_t msg_manage(const CFE_SB_MsgPtr_t msg, const unsigned int idx) {

   /* Buffers Telemetry Message Data */
   if (MsgRcv[idx].qexists == FALSE) {

      CFE_PSP_MemCpy(MsgRcv[idx].MsgStruct->mptr, msg, MsgRcv[idx].MsgStruct->siz);

      return BUFFERED;

   } else { /* Queue Command Message Data */

      return enqueue_cmd_msg(msg, idx);

   } /* End if-else statement */

} /* end of msg_manage() */

/*********************************************************************
 * Process any incoming messages if we are expecting it.
 *
 * \param[in] msg = Pointer to the software bus message.
 * \param[in] mid = Message id of software bus message.
 * \param[in] ActualLength = Reported length of software bus message
 * \param[in] PipeType = Indicates whether software bus message is command or data pipe
 *
 ********************************************************************/
static void rcv_msg(const CFE_SB_MsgPtr_t msg, CFE_SB_MsgId_t mid, uint16 ActualLength, SB_PipeType_t PipeType) {

   int idx;

   ECI_MsgControl_t  status;

   /* Flag indicating that msg id received is subscribed */
   boolean IDFound = FALSE;

   /* Verify cmd messages should be received */
   boolean CmdMsgRcv = (CCSDS_SID_TYPE(mid) == CCSDS_CMD);

   /* Loop through to see if message matches mid */
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_MsgRcv) - 1; idx++) {

      /* Message matches a receive message ID */
      if (MsgRcv[idx].MsgStruct->mid == mid) {

         IDFound = TRUE;

         /* Verify Length of Received Message */
         if (verify_msg_length(mid, ActualLength, MsgRcv[idx].MsgStruct->siz, EQUAL)) {

            /* Attempts to buffer or queue software bus message */
            status = msg_manage(msg,idx);

            switch (status)
            {
               case QUEUEFULL:

                  CFE_EVS_SendEvent(ECI_QFULL_ERR_EID, CFE_EVS_ERROR, "Cmd Queue for mid 0x%04X is full. Msg dropped", MsgRcv[idx].MsgStruct->mid);
                  MsgRcv[idx].ecnt++;
                  ECI_AppData.HkPacket.CmdErrorCounter++;
                  break;

               case QUEUED:
                  ECI_AppData.HkPacket.CmdAcceptCounter++; 
               case BUFFERED:
               default:
                  CFE_EVS_SendEvent(ECI_MSG_MANAGE_DBG_EID, CFE_EVS_DEBUG, "Mid 0x%04X is %s", MsgRcv[idx].MsgStruct->mid, ((status == QUEUED) ? "QUEUED":"BUFFERED"));
                  ECI_AppData.HkPacket.MsgRcvCnt[idx]++;
                  break;
            } /* End switch statement */
         } else {
            /* Message Received is a Command Message */
            if (CmdMsgRcv)
            {
               /* Command message length failed */
               ECI_AppData.HkPacket.CmdErrorCounter++;
            } /* End if statement */

            MsgRcv[idx].ecnt++;
         } /* End if-else statement */
      }
   }
 
   /* Message did not have a known app ID */
   if (!IDFound) {

      /* Message received is a command message */
      if (CmdMsgRcv) {

         ECI_AppData.HkPacket.CmdErrorCounter++;

      } /* End if statement */

      /* Send Event */
      CFE_EVS_SendEvent(ECI_UNK_MID_ERR_EID, CFE_EVS_ERROR, "Msg with Invalid MID Received on %s: ID = 0x%X", (PipeType ? "DATA PIPE":"CMD PIPE"), mid);
   } /* End if statement */

} /* End of rcv_msg() */

/*****************************************************************//**
 * Outputs to housekeeping message to software bus.
 ********************************************************************/
static void housekeeping_cmd(void) {

#ifdef ECI_FLAG_TABLE_DEFINED
   CFE_PSP_MemCpy(ECI_AppData.HkPacket.Enabled, ECI_AppData.FaultRep.FaultDet.Enabled, sizeof(uint16) * APP_FAULTREP_BITFIELD_WORDS);
   CFE_PSP_MemCpy(ECI_AppData.HkPacket.Latched, ECI_AppData.FaultRep.FaultDet.Latched, sizeof(uint16) * APP_FAULTREP_BITFIELD_WORDS);
#endif

   /* Send housekeeping telemetry packet */
   CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) & ECI_AppData.HkPacket);
   CFE_SB_SendMsg((CFE_SB_Msg_t *) & ECI_AppData.HkPacket);

} /* End of housekeeping_cmd() */

/********************************************************************
 * Manage Parameter Table if notified by cFE table services of update
 *
 * \param[in] MessagePtr = Pointer to command message from cFE table services
 *                         Has parameter with index to table in ECI_ParamTable[]
 *********************************************************************/
static void table_manage_cmd(CFE_SB_MsgPtr_t MessagePtr)
{

#ifdef ECI_PARAM_TBL_DEFINED
   int32                Result;
   CFE_TBL_NotifyCmd_t  *CmdPtr;

   CmdPtr = ((CFE_TBL_NotifyCmd_t*)MessagePtr);

   /* Checks if command paramter is a valid index parameter */
   if (CmdPtr->Payload.Parameter < (SIZEOF_ARRAY(ECI_ParamTable) - 1))
   {
      /* Release table data pointer */
      CFE_TBL_ReleaseAddress(TableHandle_Param[CmdPtr->Payload.Parameter]);

      /* Allow cFE to manage table */
      CFE_TBL_Manage(TableHandle_Param[CmdPtr->Payload.Parameter]);

      /* If an update occurred ... copy data into parameters structure */
      Result = CFE_TBL_GetAddress(ECI_ParamTable[CmdPtr->Payload.Parameter].tblptr, TableHandle_Param[CmdPtr->Payload.Parameter]);

      /* Get address of tables active buffer and lock against updates */
      if (Result < CFE_SUCCESS)
      {
         CFE_EVS_SendEvent(ECI_MANAGE_TBL_GETADDR_ERR_EID, CFE_EVS_ERROR, 
                           "Table Manage Cmd ERROR: Getting ECI Parameter Table Address(idx = %d) with %s, Status = 0x%08X", 
                          (int)CmdPtr->Payload.Parameter, ECI_ParamTable[CmdPtr->Payload.Parameter].tblname, (int)Result);
      } /* End if statement */
   }
   else
   {
      CFE_EVS_SendEvent(ECI_MANAGE_TBL_PARAM_IDX_ERR_EID, CFE_EVS_ERROR, 
                        "Table Manage Cmd ERROR: ECI Parameter Table idx (idx = %d) out of valid range (0 to %d)", 
                       (int)CmdPtr->Payload.Parameter, (int)(SIZEOF_ARRAY(ECI_ParamTable) - 1));
   } /* End if-else statement */

#endif /* ECI_PARAM_TBL_DEFINED */

} /* End table_manage_cmd() */

/*****************************************************************//**
 * Manage state table if state table present
 ********************************************************************/
static void manage_state_tables(void) {

#ifdef ECI_STATE_TBL_DEFINED
   /*
    * State tables are dump only - so manage tables has to be called 
    * in order to synchronize the table being dumped (if its pending).
    */
   if (TableHandle_State != CFE_TBL_BAD_TABLE_HANDLE)
   {
      CFE_TBL_Manage(TableHandle_State);
   } /* End if statement */

#endif /* ECI_STATE_TBL_DEFINED */

} /* manage_state_tables() */

/********************************************************************
 * Remove data from ring buffer
 *
 * Pre-requisite:  Assumes that data is available on ring buffer
 *
 * \param[in, out] qhd= Head of the ring buffer
 * \param[in, out] qcnt = Number of items in the ring buffer
 * \param[out] data = data to be removed from the ring buffer.
 * \param[in] data_size = size of the data
 * \param[in] q = ring buffer pointer
 * \param[in, out] qsize = total length of the ring buffer
 *********************************************************************/
static void remove_from_ring_buffer( int* qhd, int* qcnt, void* data, size_t data_size, char* q, int qsize) {

   char* qsrc;
    
   /* Specify the source location */
   qsrc = q + (*qhd - 1) * data_size;
   
   /* Copy over the data */
   CFE_PSP_MemCpy(data, qsrc, data_size);

   /* Increment the head of the list integer */
   (*qhd)++;

   /* Check for integer wrap */
   if (*qhd > qsize) {
      *qhd = 1;
   } /* End if statement */

   /* Decrement queue count */
   (*qcnt)--;

} /* remove_from_ring_buffer() */

/********************************************************************
 * Increments the buffered cmd msg seq. number for command message MsgRcv[idx]
 * Prerequisites:  1) MsgRcv[idx].imd is command message (Based on mid)
 *                 2) New cmd message placed on input buffer
 *********************************************************************/
static void increment_cmd_msg_sequence(unsigned int idx) {

   uint16 sequence;
   char* sqptr;

   /* Pointer to location of sequence number in cmd message */
   sqptr = (char*)MsgRcv[idx].MsgStruct->mptr + sizeof(uint16);

   /* Current sequence counter value */
   sequence = MsgRcv[idx].seq;

   if (sequence < ECI_MAX_CMD_SEQUENCE_NUMBER) {
      /* Increment the sequence number */
      sequence++;
   } else {
      /* Sequence number wraps around to 0 */
      sequence = 0;
   } /* End if-else statement */

   /* Copy back the sequence number to command message header */
   CFE_PSP_MemCpy(sqptr, &sequence, sizeof(uint16));

   /* Store the new current value of the sequence number */
   MsgRcv[idx].seq = sequence;

} /* End of increment_cmd_msg_sequence() */

/******************************************************************//**
 * Dequeues command messages
 *
 * \param[in] idx = Index into global ECI_MsgRcv that contains pointer to buffer
 *
 * Pre-requisite:
 *      1)  'idx' is a valid index value that points to an array with a queue
 *
 *********************************************************************/
static ECI_MsgControl_t dequeue_cmd_msg(const unsigned int idx) {

   /* Messages are in queue and need to be removed, sequence number updated and placed in buffer */
   if (MsgRcv[idx].qcnt > 0) {

      remove_from_ring_buffer(&(MsgRcv[idx].qhd), &(MsgRcv[idx].qcnt), MsgRcv[idx].MsgStruct->mptr, MsgRcv[idx].MsgStruct->siz, MsgRcv[idx].MsgStruct->qptr, ECI_CMD_MSG_QUEUE_SIZE);
      increment_cmd_msg_sequence(idx);

      return BUFFERED;

   } else {

      return QUEUEEMPTY;

   } /* End if-else statement */

} /* end of dequeue_cmd_msg() */

/********************************************************************
 * Executes application events based on interface event flags
 *********************************************************************/
static void event_signal(void) {

#ifdef ECI_EVENT_TABLE_DEFINED

   uint16 idx;
   uint16 evs_id;
   uint8 evs_type;

   /* Cycle through events and see if one has been flagged */ 
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_Events) - 1; idx++) {

      /* If a signal increments, an event occurred*/
      if ((*(ECI_Events[idx].eventFlag) != prevEventFlag[idx]) && *(ECI_Events[idx].eventFlag) != 0) {

         /* Setup up the EVS_ID so that all messages are filterable */
         evs_id = *(ECI_Events[idx].eventID);

         /* Set up the event type */
         evs_type = *(ECI_Events[idx].eventType);
	  
         /* Determine number of supporting pieces of data to send out message */
         switch (ECI_Events[idx].eventBlock) {
            case ECI_EVENT_0_DATA:
               CFE_EVS_SendEvent(evs_id, evs_type,(char*)(ECI_Events[idx].eventMsg),  ECI_Events[idx].loc);
               break; 
            case ECI_EVENT_1_DATA:
               CFE_EVS_SendEvent(evs_id, evs_type,(char*)(ECI_Events[idx].eventMsg), ECI_Events[idx].loc, 
                                *(ECI_Events[idx].data_1));
               break; 
            case ECI_EVENT_2_DATA:
               CFE_EVS_SendEvent(evs_id, evs_type,(char*)(ECI_Events[idx].eventMsg), ECI_Events[idx].loc,
                               *(ECI_Events),
                               *(ECI_Events[idx].data_2));
               break; 
            case ECI_EVENT_3_DATA:
               CFE_EVS_SendEvent(evs_id, evs_type,(char*)(ECI_Events[idx].eventMsg), ECI_Events[idx].loc,
                               *(ECI_Events[idx].data_1), 
                               *(ECI_Events[idx].data_2), 
                               *(ECI_Events[idx].data_3));
               break; 
            case ECI_EVENT_4_DATA:
               CFE_EVS_SendEvent(evs_id, evs_type,(char*)(ECI_Events[idx].eventMsg), ECI_Events[idx].loc,
                               *(ECI_Events[idx].data_1), 
                               *(ECI_Events[idx].data_2), 
                               *(ECI_Events[idx].data_3),
                               *(ECI_Events[idx].data_4));
               break; 
            case ECI_EVENT_5_DATA:
               CFE_EVS_SendEvent(evs_id, evs_type,(char*)(ECI_Events[idx].eventMsg), ECI_Events[idx].loc,
                               *(ECI_Events[idx].data_1), 
                               *(ECI_Events[idx].data_2), 
                               *(ECI_Events[idx].data_3),
                               *(ECI_Events[idx].data_4),
                               *(ECI_Events[idx].data_5));
               break; 
            default:
               break;

         } /* End Switch Statement */


      } /* End If Statement */

      prevEventFlag[idx]=*(ECI_Events[idx].eventFlag);

   } /* End For Loop */

#endif /* ECI_EVENT_TABLE_DEFINED */

} /* End of event_signal() */

/******************************************************************//**
 * Executes Fault Reporting based on interface status flags
 *********************************************************************/
static void fdc_signal(void) {

#ifdef ECI_FLAG_TABLE_DEFINED
   uint16      idx;
   uint16   flag_id;

   /* Cycle through Flags and see if one has been set */ 
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_Flags) - 1; idx++) {

      if (*(ECI_Flags[idx].StatusFlag) != 0) {
         /* Setup up the EVS_ID so that all messages are filterable */
         flag_id = (*(ECI_Flags[idx].FlagID));

         /* Notify FDC */
         App_FaultRep_FaultDetFailed(&ECI_AppData.FaultRep, flag_id);
      } /* End of if statement */

   } /* End For Loop */

#endif /* ECI_FLAG_TABLE_DEFINED */

} /* End of fdc_signal() */

/********************************************************************
 * Outputs SB messages listed in ECI_MsgSnd on to the Software Bus.
 ********************************************************************/
static void send_msg(void) {

   int idx;

   /* Send out Messages */
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_MsgSnd) - 1; idx++) {

      /* Applies time stamp if telemetry message */
      if (CCSDS_SID_TYPE(ECI_MsgSnd[idx].mid) == CCSDS_TLM) {
         CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) ECI_MsgSnd[idx].mptr);
      } /* End if statement */

      CFE_SB_SetMsgId((CFE_SB_Msg_t *) ECI_MsgSnd[idx].mptr, ECI_MsgSnd[idx].mid);

      CFE_SB_SetTotalMsgLength((CFE_SB_Msg_t *) ECI_MsgSnd[idx].mptr, ECI_MsgSnd[idx].siz);

      /* Will place message onto software bus if app-specified enables output or if
         set to always output*/
      if (ECI_MsgSnd[idx].sendMsg == NULL || *(ECI_MsgSnd[idx].sendMsg))
      {
         CFE_SB_SendMsg((CFE_SB_Msg_t *) ECI_MsgSnd[idx].mptr);
      } /* End if statement */

   } /* End for-loop */

} /* End of send_msg() */

/*******************************************************************
 * Places FDC Packet on software bus if fault reporting enabled
 ********************************************************************/
static void fdc_pkt_gen(void) {

#ifdef ECI_FLAG_TABLE_DEFINED

   App_FaultRep_GenTlmMsg((void*)&ECI_AppData.FaultRep, (CFE_SB_MsgPtr_t)&ECI_AppData.FDCPacket);

   /* Send FDC telemetry packet */
   CFE_SB_TimeStampMsg((CFE_SB_Msg_t *) &ECI_AppData.FDCPacket);
   CFE_SB_SendMsg((CFE_SB_Msg_t *) &ECI_AppData.FDCPacket);

#endif /* ECI_FLAG_TABLE_DEFINED */

} /* End of fdc_pkt_gen() */

/*******************************************************************
 * Update CDS if available
 ********************************************************************/
static void cds_update(void) {

#ifdef ECI_CDS_TABLE_DEFINED

   /* Exits if CDS is unavailable */
   if (ECI_AppData.CDS_Avail)
      return;

   int   idx;
   int32 status;

   /* Saves all specified CDS data */
   for (idx = 0; idx < SIZEOF_ARRAY(ECI_CdsTable) - 1; idx++)
   {
      status = CFE_ES_CopyToCDS(ECI_AppData.CDSHandle[idx], ECI_CdsTable[idx].cdsptr);

      if (status < CFE_SUCCESS){

         CFE_EVS_SendEvent(ECI_CDS_COPYTOCDS_ERR_EID, CFE_EVS_ERROR, "Error saving data in ECI_CdsTable[%d] to CDS", idx);

      } /* End if-statement */
   } /* End for-loop */

#endif /* ECI_CDS_TABLE_DEFINED */

} /* End of cds_update() */

/********************************************************************
 * 1) Get step timestamp
 * 2) Manages state table
 * 3) Dequeues command messages
 * 4) Executes app-specified step function
 * 5) Execute Event Message processing
 * 6) Execute Fault Reporting
 * 7) Send messages specified in ECI_MsgSnd[]
 * 8) Send FDC messages
 * 9) Update CDS
 ********************************************************************/
static void do_step(void) {

   int i;

#ifdef ECI_STEP_TIMESTAMP_DEFINED
   ECI_Step_TimeStamp = CFE_TIME_GetTime();
#endif /* ECI_STEP_TIMESTAMP_DEFINED */

   /* Manage tables */
   manage_state_tables();

   /* Dequeue Cmd Messages */
   for (i = 0; i < SIZEOF_ARRAY(ECI_MsgRcv) - 1; i++)
   {

      if (MsgRcv[i].qexists)
         dequeue_cmd_msg (i);

   } /* End for-loop */

   /* Call model function step */
   ECI_STEP_FCN;

   /* Check for internal event triggers */
   event_signal();

   /* Check for internal FDC triggers */
   fdc_signal();

   /* Send Telemetry Messages */
   send_msg();

   /* Send FDC Telemetry Message */
   fdc_pkt_gen();

   /* Update CDS */
   cds_update();

   return;

} /* End of do_step() */

/*****************************************************************//**
 * Processes command pipe messages.
 *
 * \param[in] msg = Pointer to a software bus message.
 ********************************************************************/
static void app_pipe(const CFE_SB_MsgPtr_t msg) {

   CFE_SB_MsgId_t messageID;
   uint16 commandCode;
   uint16 ActualLength;

   /* Obtain the message ID */
   messageID = CFE_SB_GetMsgId(msg);
   ActualLength = CFE_SB_GetTotalMsgLength(msg);

   /* Execute based on message Id */
   switch (messageID) {

      /*
       * Process ECI App Commands
       */
      case ECI_CMD_MID:

         /* Obtain command code */           
         commandCode = CFE_SB_GetCmdCode(msg);

         switch (commandCode) {

            /* No-op Cmd */
            case ECI_NOOP_CC:
               if (!verify_msg_length(messageID, ActualLength, ECI_NO_DATA_CMD_MSG_LENGTH, EQUAL))
                  ECI_AppData.HkPacket.CmdErrorCounter++;
               else
                  no_op_cmd();

               break;

            /* HK Reset Cmd */
            case ECI_RESET_HK_COUNTER_CC:
               if (!verify_msg_length(messageID, ActualLength,ECI_NO_DATA_CMD_MSG_LENGTH,EQUAL))
                  ECI_AppData.HkPacket.CmdErrorCounter++;
               else
                  reset_hk_counter_cmd();

               break;

#ifdef ECI_FLAG_TABLE_DEFINED
            /* Config Fault Detection Cmd */
            case ECI_FAULTREP_CONFIG_CC:
               if (!verify_msg_length(messageID, ActualLength, (CFE_SB_CMD_HDR_SIZE + sizeof(App_FaultRep_ConfigFaultDetCmdParam)),EQUAL))
                  ECI_AppData.HkPacket.CmdErrorCounter++;
               else if (App_FaultRep_ConfigFaultDetCmd(&ECI_AppData.FaultRep, CFE_SB_GetUserData(msg)))
                  ECI_AppData.HkPacket.CmdAcceptCounter++;
               else
                  ECI_AppData.HkPacket.CmdErrorCounter++;

               break;

            /* Clear Fault Detection Data Cmd */
            case ECI_FAULTREP_CLEAR_CC:
               if (!verify_msg_length(messageID, ActualLength, (CFE_SB_CMD_HDR_SIZE + sizeof(App_FaultRep_ClearFaultDetCmdParam)),EQUAL))
                  ECI_AppData.HkPacket.CmdErrorCounter++;
               else if (App_FaultRep_ClearFaultDetCmd(&ECI_AppData.FaultRep, CFE_SB_GetUserData(msg)))
                  ECI_AppData.HkPacket.CmdAcceptCounter++; 
               else
                  ECI_AppData.HkPacket.CmdErrorCounter++;

               break;
#endif /* ECI_FLAG_TABLE_DEFINED */
            /* Cmd for Auto-Generated Code */
            default:
               if (commandCode <= ECI_FUNC_CODE_END && commandCode >= ECI_FUNC_CODE_START)
                  rcv_msg(msg, ECI_CMD_MID, ActualLength, CMDPIPE); /* Receive message prior to execution */
               else {
                  CFE_EVS_SendEvent(ECI_INV_CMD_CODE_ERR_EID, CFE_EVS_ERROR,
                                   "Invalid Command Code: ID = 0x%X, CC = %d",
                                    messageID, commandCode);
                  ECI_AppData.HkPacket.CmdErrorCounter++;
               } /* End if-else statement */

                break;
         } /* End Switch Statement */

         break;

      /* Scheduler requesting HK */
      case ECI_SEND_HK_MID:
         if (verify_msg_length(messageID, ActualLength, CFE_SB_CMD_HDR_SIZE, EQUAL))
         {
            housekeeping_cmd();
         } /* End if statement */

            break;

      /* Table management command */
      case ECI_TBL_MANAGE_MID:
         if (verify_msg_length(messageID, ActualLength, sizeof(CFE_TBL_NotifyCmd_t),EQUAL))
            table_manage_cmd(msg);

         break;

      /*
       * TICK Processing (trigger step function call)
       */
      case ECI_TICK_MID:

         if (verify_msg_length(messageID, ActualLength,ECI_NO_DATA_CMD_MSG_LENGTH,EQUAL))
         {
            while (CFE_SB_RcvMsg(&ECI_AppData.DataMsgPtr, ECI_AppData.DataPipe, CFE_SB_POLL) >= CFE_SUCCESS)
            {
               rcv_msg(ECI_AppData.DataMsgPtr, CFE_SB_GetMsgId(ECI_AppData.DataMsgPtr), CFE_SB_GetTotalMsgLength(ECI_AppData.DataMsgPtr), DATAPIPE);
            } /* End while-loop */

            do_step(); 
         } /* End if statement */

         break;

      default:
         rcv_msg(msg, messageID, ActualLength, CMDPIPE); /* Receive message prior to execution */
         break;

   } /* End switch-statement */

} /* app_pipe() */

/*****************************************************************//**
 * Application entry point and main process loop.
 ********************************************************************/
void ECI_APP_MAIN(void) {
   int32 status;

   /* Register the Application with Executive Services */
   CFE_ES_RegisterApp();

   /* Create the first Performance Log entry */
   CFE_ES_PerfLogEntry(ECI_PERF_ID);

   /*
    * Perform application specific initialization
    * If the initialization fails, set the RunStatus to CFE_ES_APP_ERROR
    * and the App will not enter the RunLoop.
    */
   status = app_init();

   /* If no errors were detected during initialization, then wait for everyone to start */
   if (status == CFE_SUCCESS) {
      CFE_ES_WaitForStartupSync (ECI_STARTUP_SYNC_TIMEOUT);
   } else {
      CFE_EVS_SendEvent(ECI_INIT_FAIL_ERR_EID, CFE_EVS_ERROR, "ECI Initialization Failed, ECI App Will Exit");
      /* App will exit for error during initialization */
      ECI_AppData.RunStatus = CFE_ES_APP_ERROR;
   } /* End if-else statement */

   /*
    * Application Main Loop. Call CFE_ES_RunLoop to check for changes
    * in the Application's status. If there is a request to kill this
    * App, it will be passed in through the RunLoop call.
    */
   while (CFE_ES_RunLoop(&ECI_AppData.RunStatus) == TRUE) {

      /* Performance Log Exit Stamp. */
      CFE_ES_PerfLogExit(ECI_PERF_ID);

      /* Pend on the arrival of the next Software Bus message. */
      status = CFE_SB_RcvMsg(&ECI_AppData.MsgPtr, ECI_AppData.CmdPipe, CFE_SB_PEND_FOREVER);

      /* Performance Log Entry Stamp. */
      CFE_ES_PerfLogEntry(ECI_PERF_ID);

      /* Check the return status from the Software Bus. */
      if (status == CFE_SUCCESS) {

         /*
          * Process Software Bus message. If there are fatal errors
          * in command processing, or the "exit" command is received,
          * the command can alter the global RunStatus variable to 
          * exit the main event loop.
          */
         app_pipe(ECI_AppData.MsgPtr);

      } else {

         CFE_EVS_SendEvent(ECI_CMD_PIPE_READ_ERR_EID, CFE_EVS_ERROR, "SB Command Pipe Read Error, ECI App will Exit.");

         ECI_AppData.RunStatus = CFE_ES_APP_ERROR;
      } /* End if-else statement */

   }   /* end while-loop */

  /* Call the user-provided term function if defined */
#ifdef ECI_TERM_FCN
   ECI_TERM_FCN;
#endif

   /* Performance Log Exit Stamp. */
   CFE_ES_PerfLogExit(ECI_PERF_ID);

   /* Exit the Application. */
   CFE_ES_ExitApp(ECI_AppData.RunStatus);

} /* End of ECI_APP_MAIN() */

/************************/
/*  End of File Comment */
/************************/
