/**
 * File:  eci_app_events.h
 * Description:  Event ID specification
 */

#ifndef ECI_APP_EVENTS_H
#define ECI_APP_EVENTS_H

/**
 * .. _`ranges`:
 *
 * ------------------------
 *
 * Event message ID's:
 * -------------------
 *
 * ====================== ============
 * Event                  Range
 * ====================== ============
 * `Informational Codes`_ 101-150
 * `Error Codes`_         151-200
 * `Critical Codes`_      201-210
 * `Debug Codes`_         211-220
 * ====================== ============
 *
 * ------------------------
 */

/**
 * ===================
 * Informational Codes
 * ===================
 * return to `top <ranges_>`_
 */
/**
 * :Description:  ECI initializes successfully event
 * :Text: "Application Initialized, Revision %s", REVISION_NUMBER
 * :Type: Informational
 * :Cause: Application initializes 
 */
#define ECI_INIT_INF_EID                  101

/**
 * :Description:  No-Op event
 * :Text: "No-op command, Revision %s", REVISION_NUMBER
 * :Type: Informational
 * :Cause: No-Op command received
 */
#define ECI_NOOP_INF_EID                  102

/**
 * :Description:  Reset HK counters event
 * :Text: "Reset Housekeeping Counters"
 * :Type: Informational
 * :Cause: Housekeeping counter reset command received
 */
#define ECI_RESET_HK_CTR_INF_EID          103

/**
 * :Description:  Restoring existing data from CDS
 * :Text: "ECI_CDS[%d] already exists in CDS and will be restored", idx
 * :Type: Informational
 * :Cause: CFE_ES_RegisterCDS() returns CFE_ES_CDS_ALREADY_EXISTS
 */
#define ECI_CDS_RESTORE_INF_EID           104

/**
 * :Description:  CDS not available for ECI app use
 * :Text: "CDS not available for ECI_CDS"
 * :Type: Informational
 * :Cause: CFE_ES_RegisterCDS() returns CFE_ES_NOT_IMPLEMENTED
 */
#define ECI_CDS_NOT_AVAIL_INF_EID         105

/**
 * :Description:  CDS registers data from ECI_CDS
 * :Text: "ECI_CDS[%d] registered to CDS", idx
 * :Type: Informational
 * :Cause: CFE_ES_RegisterCDS() returns successful return code
 */
#define ECI_CDS_REGISTER_INF_EID          106

/**
 * ===========
 * Error Codes
 * ===========
 * return to `top <ranges_>`_
 */
/**
 * :Description:  Tlm pkt has mismatched length
 * :Text: "Invalid tlm pkt: ID = 0x%X, Len = %d (Expected = %d)", 
 *  messageID, actualLength, expectedLength
 * :Type: Error
 * :Cause: Telemetry packet received as a reported length which does
 *        agree with expected length
 */
#define ECI_TLM_LEN_ERR_EID               151

/**
 * :Description:  Cmd pkt has mismatched length
 * :Text: "Invalid cmd pkt: ID = 0x%X, Len = %d (Expected = %d)",
 *      messageID, actualLength, expectedLength
 * :Type: Error
 * :Cause: Command packet received as a reported length which does
 *       agree with expected length
 */
#define ECI_CMD_LEN_ERR_EID               152

/**
 * :Description:  Command Queue is full
 * :Text: "Cmd Queue for mid 0x%04X is full. Msg dropped", MsgRcv[idx].MsgStruct->mid
 * :Type: Error
 * :Cause: Command queue for mid has filled-up during currently control cycle and cannot
 *       accept another command message
 */
#define ECI_QFULL_ERR_EID                 153

/**
 * :Description:  Message with unsubscribed mid received 
 * :Text: "Msg with Invalid MID Received on %s: ID = 0x%X", (PipeType ? "DATA PIPE":"CMD PIPE"), mid
 * :Type: Error
 * :Cause: Message with unsubscribed mid received on app SB pipe
 */
#define ECI_UNK_MID_ERR_EID               154

/**
 * :Description:  Error getting parameter table address from cFE table services
 * :Text: "Table Manage Cmd ERROR: Getting ECI Parameter Table Address(idx = %d) with %s, Status = 0x%08X", 
 *      (int)CmdPtr->Payload.Parameter, ECI_ParamTable[CmdPtr->Payload.Parameter].tblname, (int)Result
 * :Type: Error
 * :Cause: After table update, app received error from CFE_TBL_GetAddress() for updated
 *       table data.
 */
#define ECI_MANAGE_TBL_GETADDR_ERR_EID    155

/**
 * :Description:  Received internal table management notification command from cFE table services
 *              with invalid index parameter
 * :Text: "Table Manage Cmd ERROR: ECI Parameter Table idx (idx = %d) out of valid range (0 to %d)", 
 *      (int)CmdPtr->Payload.Parameter, (int)(SIZEOF_ARRAY(ECI_ParamTable) - 1)
 * :Type: Error
 * :Cause: cFE table services sent an internal table management notification command with an invalid
 *       index parameter for the ECI_ParamTable[] array.
 */
#define ECI_MANAGE_TBL_PARAM_IDX_ERR_EID  156

/**
 * :Description:  Error subscribing to the ECI_CMD_MID
 * :Text: "Error Subscribing to ECI_CMD_MID 0x%04X, Status = 0x%08X", ECI_CMD_MID, status
 * :Type: Error
 * :Cause: CFE_SB_Subscribe() returns an error when subscribing to ECI_CMD_MID
 */
#define ECI_SUB_CMD_MID_ERR_EID           157

/**
 * :Description:  Error subscribing to ECI_SEND_HK_MID
 * :Text: "ERROR Subscribing to ECI_SEND_HK_MID 0x%04X, Status = 0x%08X", ECI_SEND_HK_MID, status
 * :Type: Error
 * :Cause: CFE_SB_Subscribe() returns an error when subscribing to ECI_SEND_HK_MID
 */
#define ECI_SUB_SEND_HK_MSG_ERR_EID       158

/**
 * :Description:  Error subscribing to ECI_TBL_MANAGE_MID
 * :Text: "ERROR Subscribing to ECI_TBL_MANAGE_MID 0x%04X, Status = 0x%08X", 
 *      ECI_TBL_MANAGE_MID, status
 * :Type: Error
 * :Cause: CFE_SB_Subscribe() returns an error when subscribing to ECI_TBL_MANAGE_MID
 */
#define ECI_SUB_TBL_MANAGE_MSG_ERR_EID    159

/**
 * :Description:  Error subscribing to ECI_TICK_MID
 * :Text: "Error Subscribing to ECI_TICK_MID 0x%04X, Status = 0x%08X", ECI_TICK_MID, status
 * :Type: Error
 * :Cause: CFE_SB_Subscribe() returns an error when subscribing to ECI_TICK_MID
 */
#define ECI_SUB_TICK_MSG_ERR_EID          160

/**
 * :Description:  MID in ECI_MsgRcv[idx] is a reserved value
 * :Text: "Msg ID for CMD MID 0x%04X in ECI_MsgRcv[%d] is reserved value", MsgRcv[idx].MsgStruct->mid, idx
 * :Type: Error
 * :Cause:  MID in ECI_MsgRcv[idx] equals ECI_SEND_HK_MID, ECI_TBL_MANAGE_MID, or ECI_TICK_MID
 */
#define ECI_CMD_MSGID_RESERVE_ERR_EID     161

/**
 * :Description:  Error subscribing to Command message in ECI_MsgRcv[idx]
 * :Text: "Error Subscribing to CMD MID 0x%04X in ECI_MsgRcv[%d] on CMD Pipe, Status = 0x%08X", 
 *      MsgRcv[idx].MsgStruct->mid, idx, status
 * :Type: Error
 * :Cause: CFE_SB_Subscribe() returns an error when subscribing to tlm mid in ECI_MsgRcv[idx]
 */
#define ECI_CMD_SUBSCRIBE_ERR_EID         162

/**
 * :Description:  Error subscribing to telemetry message in ECI_MsgRcv[idx]
 * :Text: "Error Subscribing to MsgID 0x%04X in ECI_MsgRcv[%d] on Data Pipe, Status = 0x%08X",
 *     MsgRcv[idx].MsgStruct->mid, idx, status
 * :Type:  Error
 * :Cause: CFE_SB_Subscribe() returns an error when subscribing to cmd mid in ECI_MsgRcv[idx]
 */
#define ECI_TLM_SUBSCRIBE_ERR_EID         163

/**
 * :Description:  Error registering table ECI_ParamTable[idx] with cFE table services
 * :Text: "ERROR Registering ECI_ParamTable[%d], Status = 0x%08X", idx, (int)param_status
 * :Type: Error
 * :Cause: CFE_TBL_Register() returns an error for ECI_ParamTable[idx]
 */
#define ECI_PARAM_TBL_REG_ERR_EID         164

/**
 * :Description:  Error loading table ECI_ParamTable[idx] with default table file data
 * :Text: "ERROR Loading ECI_ParamTable[%d] (%s), Status = 0x%08X",
 *      idx, ECI_ParamTable[idx].tblname, (int)status
 * :Type: Error
 * :Cause: CFE_TBL_Load() returns an error for ECI_ParamTable[idx]
 */
#define ECI_PARAM_TBL_LOAD_ERR_EID        165

/**
 * :Description: Error getting address for ECI_ParamTable[idx] after table data load
 * :Text: "ERROR Getting Address from cFE Table Services from ECI_ParamTable[%d] (%s), Status = 0x%08X",
 *       idx, ECI_ParamTable[idx].tblname, (int)status
 * :Type: Error
 * :Cause: CFE_TBL_GetAddress() returns error for ECI_ParamTable[idx] after table load during initialization
 */
#define ECI_PARAM_TBL_GETADDR_ERR_EID     166

/**
 * :Description:  Error managing ECI_ParamTable[idx] after table data load
 * :Text: "ERROR Managing ECI_ParamTable[%d] (%s), Status = 0x%08X", 
 *      idx, ECI_ParamTable[idx].tblname, (int)status
 * :Type: Error
 * :Cause: CFE_TBL_Manage() returns error for ECI_ParamTable[idx] after table load during initialization
 */
#define ECI_PARAM_TBL_MANAGE_ERR_EID      167

/**
 * :Description:  Application received cmd msg with an invalid command code
 * :Text: ECI_INV_CMD_CODE_ERR_EID, CFE_EVS_ERROR, "Invalid Command Code: ID = 0x%X, CC = %d", messageID, commandCode
 * :Type: Error
 * :Cause: Application received a cmd message with a valid messageID, but with a command code that was not within
 *      the valid range (ECI_FUNC_CODE_START - ECI_FUNC_CODE_END)
 */
#define ECI_INV_CMD_CODE_ERR_EID          168

/**
 * :Description:  Application received a message on command pipe with an unrecognized MID
 * :Text: "Invalid MID Received on Command Pipe: ID = 0x%X", messageID
 * :Type: Error
 * :Cause: Application received a message on command pipe with an unrecognized MID
 */
#define ECI_INV_MID_ERR_EID               169

/**
 * :Description:  Application had an error reading the command pipe
 * :Text: "SB Command Pipe Read Error, ECI App will Exit."
 * :Type: Error
 * :Cause: CFE_SB_RcvMsg() returns an error while pending forever on the command pipe
 */
#define ECI_CMD_PIPE_READ_ERR_EID             170

/**
 * :Description:  Application had an error saving data back to CDS
 * :Text: "Error saving data in ECI_CDS[%d] to CDS", idx
 * :Type: Error
 * :Cause: CFE_ES_CopyToCDS() returns an error when writing data to CDS
 */
#define ECI_CDS_COPYTOCDS_ERR_EID             171

/**
 * :Description:  Application had an error restoring data from CDS
 * :Text: "ECI_CDS[%d] failed to restore from CDS, Status = 0x%08X", idx, status
 * :Type: Error
 * :Cause: CFE_ES_RestoreFromCDS() returns an error when restoring data from CDS
 */
#define ECI_CDS_RESTORE_ERR_EID               172

/**
 * :Description:  Application error during SIL initialization
 * :Text: "ECI Initialization Failed, ECI App Will Exit"
 * :Type: Error
 * :Cause: app_init() returns an error during initialization
 */
#define ECI_INIT_FAIL_ERR_EID                173

/**
 * ==============
 * Critical Codes
 * ==============
 * return to `top <ranges_>`_
 */

/**
 * ===========
 * DEBUG CODES
 * ===========
 * return to `top <ranges_>`_
 */
/**
 * :Description:  Application has a command/telemetry message buffered or queued
 * :Text: "Mid 0x%04X is %s", ((status == QUEUED) ? "QUEUED":"BUFFERED"), MsgRcv[idx].MsgStruct->mid
 * :Type: DEBUG
 * :Cause: Application has a command/telemetry message buffered or queued
 */
#define ECI_MSG_MANAGE_DBG_EID            211

#endif  /* ECI_APP_EVENTS_H */

