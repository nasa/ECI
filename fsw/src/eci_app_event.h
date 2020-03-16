/**
 * @file
 * @brief This file contains all of the event ids used by the
 *  ECI when sending messages to event services.
 */
/**
 * @defgroup event_ids Event IDs
 * @{
 */

#ifndef ECI_APP_EVENTS_H
#define ECI_APP_EVENTS_H

/**
 * @defgroup info_ids Informational EIDs
 * EIDs for cFE/cFS informational events.
 * @{
 */
/**
 * Description: ECI initializes successfully event<br>
 * Text: "Application Initialized, Revision %s", REVISION_NUMBER<br>
 * Type: Informational<br>
 * Cause: Application initializes<br>
 */
#define ECI_INIT_INF_EID                  101

/**
 * Description:  No-Op event<br>
 * Text: "No-op command, Revision %s", REVISION_NUMBER<br>
 * Type: Informational<br>
 * Cause: No-Op command received<br>
 */
#define ECI_NOOP_INF_EID                  102

/**
 * Description:  Reset HK counters event<br>
 * Text: "Reset Housekeeping Counters"<br>
 * Type: Informational<br>
 * Cause: Housekeeping counter reset command received<br>
 */
#define ECI_RESET_HK_CTR_INF_EID          103

/**
 * Description:  Restoring existing data from CDS<br>
 * Text: "ECI_CDS[%d] already exists in CDS and will be restored", idx<br>
 * Type: Informational<br>
 * Cause: CFE_ES_RegisterCDS() returns CFE_ES_CDS_ALREADY_EXISTS<br>
 */
#define ECI_CDS_RESTORE_INF_EID           104

/**
 * Description:  CDS not available for ECI app use<br>
 * Text: "CDS not available for ECI_CDS"<br>
 * Type: Informational<br>
 * Cause: CFE_ES_RegisterCDS() returns CFE_ES_NOT_IMPLEMENTED<br>
 */
#define ECI_CDS_NOT_AVAIL_INF_EID         105

/**
 * Description:  CDS registers data from ECI_CDS<br>
 * Text: "ECI_CDS[%d] registered to CDS", idx<br>
 * Type: Informational<br>
 * Cause: CFE_ES_RegisterCDS() returns successful return code<br>
 */
#define ECI_CDS_REGISTER_INF_EID          106
/**@}*/

/**@defgroup error_ids Error EIDs
 * EIDs relating to errors during code execution.
 * @{
 */
/**
 * Description:  Tlm pkt has mismatched length<br>
 * Text: "Invalid tlm pkt: ID = 0x%X, Len = %d (Expected = %d)", <br>
 *  messageID, actualLength, expectedLength
 * Type: Error<br>
 * Cause: Telemetry packet received as a reported length which does<br>
 *        agree with expected length
 */
#define ECI_TLM_LEN_ERR_EID               151

/**
 * Description:  Cmd pkt has mismatched length<br>
 * Text: "Invalid cmd pkt: ID = 0x%X, Len = %d (Expected = %d)",<br>
 *      messageID, actualLength, expectedLength
 * Type: Error<br>
 * Cause: Command packet received as a reported length which does<br>
 *       agree with expected length
 */
#define ECI_CMD_LEN_ERR_EID               152

/**
 * Description:  Command Queue is full<br>
 * Text: "Cmd Queue for mid 0x%04X is full. Msg dropped", MsgRcv[idx].MsgStruct->mid<br>
 * Type: Error<br>
 * Cause: Command queue for mid has filled-up during currently control cycle and cannot<br>
 *       accept another command message
 */
#define ECI_QFULL_ERR_EID                 153

/**
 * Description:  Message with unsubscribed mid received <br>
 * Text: "Msg with Invalid MID Received on %s: ID = 0x%X", (PipeType ? "DATA PIPE":"CMD PIPE"), mid<br>
 * Type: Error<br>
 * Cause: Message with unsubscribed mid received on app SB pipe<br>
 */
#define ECI_UNK_MID_ERR_EID               154

/**
 * Description:  Error getting parameter table address from cFE table services<br>
 * Text: "Table Manage Cmd ERROR: Getting ECI Parameter Table Address(idx = %d) with %s, Status = 0x%08X", <br>
 *      (int)CmdPtr->Payload.Parameter, ECI_ParamTable[CmdPtr->Payload.Parameter].tblname, (int)Result
 * Type: Error<br>
 * Cause: After table update, app received error from CFE_TBL_GetAddress() for updated<br>
 *       table data.
 */
#define ECI_MANAGE_TBL_GETADDR_ERR_EID    155

/**
 * Description:  Received internal table management notification command from cFE table services<br>
 *              with invalid index parameter
 * Text: "Table Manage Cmd ERROR: ECI Parameter Table idx (idx = %d) out of valid range (0 to %d)", <br>
 *      (int)CmdPtr->Payload.Parameter, (int)(SIZEOF_ARRAY(ECI_ParamTable) - 1)
 * Type: Error<br>
 * Cause: cFE table services sent an internal table management notification command with an invalid<br>
 *       index parameter for the ECI_ParamTable[] array.
 */
#define ECI_MANAGE_TBL_PARAM_IDX_ERR_EID  156

/**
 * Description:  Error subscribing to the ECI_CMD_MID<br>
 * Text: "Error Subscribing to ECI_CMD_MID 0x%04X, Status = 0x%08X", ECI_CMD_MID, status<br>
 * Type: Error<br>
 * Cause: CFE_SB_Subscribe() returns an error when subscribing to ECI_CMD_MID<br>
 */
#define ECI_SUB_CMD_MID_ERR_EID           157

/**
 * Description:  Error subscribing to ECI_SEND_HK_MID<br>
 * Text: "ERROR Subscribing to ECI_SEND_HK_MID 0x%04X, Status = 0x%08X", ECI_SEND_HK_MID, status<br>
 * Type: Error<br>
 * Cause: CFE_SB_Subscribe() returns an error when subscribing to ECI_SEND_HK_MID<br>
 */
#define ECI_SUB_SEND_HK_MSG_ERR_EID       158

/**
 * Description:  Error subscribing to ECI_TBL_MANAGE_MID<br>
 * Text: "ERROR Subscribing to ECI_TBL_MANAGE_MID 0x%04X, Status = 0x%08X", <br>
 *      ECI_TBL_MANAGE_MID, status
 * Type: Error<br>
 * Cause: CFE_SB_Subscribe() returns an error when subscribing to ECI_TBL_MANAGE_MID<br>
 */
#define ECI_SUB_TBL_MANAGE_MSG_ERR_EID    159

/**
 * Description:  Error subscribing to ECI_TICK_MID<br>
 * Text: "Error Subscribing to ECI_TICK_MID 0x%04X, Status = 0x%08X", ECI_TICK_MID, status<br>
 * Type: Error<br>
 * Cause: CFE_SB_Subscribe() returns an error when subscribing to ECI_TICK_MID<br>
 */
#define ECI_SUB_TICK_MSG_ERR_EID          160

/**
 * Description:  MID in ECI_MsgRcv[idx] is a reserved value<br>
 * Text: "Msg ID for CMD MID 0x%04X in ECI_MsgRcv[%d] is reserved value", MsgRcv[idx].MsgStruct->mid, idx<br>
 * Type: Error<br>
 * Cause:  MID in ECI_MsgRcv[idx] equals ECI_SEND_HK_MID, ECI_TBL_MANAGE_MID, or ECI_TICK_MID<br>
 */
#define ECI_CMD_MSGID_RESERVE_ERR_EID     161

/**
 * Description:  Error subscribing to Command message in ECI_MsgRcv[idx]<br>
 * Text: "Error Subscribing to CMD MID 0x%04X in ECI_MsgRcv[%d] on CMD Pipe, Status = 0x%08X", <br>
 *      MsgRcv[idx].MsgStruct->mid, idx, status
 * Type: Error<br>
 * Cause: CFE_SB_Subscribe() returns an error when subscribing to tlm mid in ECI_MsgRcv[idx]<br>
 */
#define ECI_CMD_SUBSCRIBE_ERR_EID         162

/**
 * Description:  Error subscribing to telemetry message in ECI_MsgRcv[idx]<br>
 * Text: "Error Subscribing to MsgID 0x%04X in ECI_MsgRcv[%d] on Data Pipe, Status = 0x%08X",<br>
 *     MsgRcv[idx].MsgStruct->mid, idx, status
 * Type:  Error<br>
 * Cause: CFE_SB_Subscribe() returns an error when subscribing to cmd mid in ECI_MsgRcv[idx]<br>
 */
#define ECI_TLM_SUBSCRIBE_ERR_EID         163

/**
 * Description:  Error registering table ECI_ParamTable[idx] with cFE table services<br>
 * Text: "ERROR Registering ECI_ParamTable[%d], Status = 0x%08X", idx, (int)param_status<br>
 * Type: Error<br>
 * Cause: CFE_TBL_Register() returns an error for ECI_ParamTable[idx]<br>
 */
#define ECI_PARAM_TBL_REG_ERR_EID         164

/**
 * Description:  Error loading table ECI_ParamTable[idx] with default table file data<br>
 * Text: "ERROR Loading ECI_ParamTable[%d] (%s), Status = 0x%08X",<br>
 *      idx, ECI_ParamTable[idx].tblname, (int)status
 * Type: Error<br>
 * Cause: CFE_TBL_Load() returns an error for ECI_ParamTable[idx]<br>
 */
#define ECI_PARAM_TBL_LOAD_ERR_EID        165

/**
 * Description: Error getting address for ECI_ParamTable[idx] after table data load<br>
 * Text: "ERROR Getting Address from cFE Table Services from ECI_ParamTable[%d] (%s), Status = 0x%08X",<br>
 *       idx, ECI_ParamTable[idx].tblname, (int)status
 * Type: Error<br>
 * Cause: CFE_TBL_GetAddress() returns error for ECI_ParamTable[idx] after table load during initialization<br>
 */
#define ECI_PARAM_TBL_GETADDR_ERR_EID     166

/**
 * Description:  Error managing ECI_ParamTable[idx] after table data load<br>
 * Text: "ERROR Managing ECI_ParamTable[%d] (%s), Status = 0x%08X", <br>
 *      idx, ECI_ParamTable[idx].tblname, (int)status
 * Type: Error<br>
 * Cause: CFE_TBL_Manage() returns error for ECI_ParamTable[idx] after table load during initialization<br>
 */
#define ECI_PARAM_TBL_MANAGE_ERR_EID      167

/**
 * Description:  Application received cmd msg with an invalid command code<br>
 * Text: ECI_INV_CMD_CODE_ERR_EID, CFE_EVS_ERROR, "Invalid Command Code: ID = 0x%X, CC = %d", messageID, commandCode<br>
 * Type: Error<br>
 * Cause: Application received a cmd message with a valid messageID, but with a command code that was not within<br>
 *      the valid range (ECI_FUNC_CODE_START - ECI_FUNC_CODE_END)
 */
#define ECI_INV_CMD_CODE_ERR_EID          168

/**
 * Description:  Application had an error reading the command pipe<br>
 * Text: "SB Command Pipe Read Error, ECI App will Exit."<br>
 * Type: Error<br>
 * Cause: CFE_SB_RcvMsg() returns an error while pending forever on the command pipe<br>
 */
#define ECI_CMD_PIPE_READ_ERR_EID             170

/**
 * Description:  Application had an error saving data back to CDS<br>
 * Text: "Error saving data in ECI_CDS[%d] to CDS", idx<br>
 * Type: Error<br>
 * Cause: CFE_ES_CopyToCDS() returns an error when writing data to CDS<br>
 */
#define ECI_CDS_COPYTOCDS_ERR_EID             171

/**
 * Description:  Application had an error restoring data from CDS<br>
 * Text: "ECI_CDS[%d] failed to restore from CDS, Status = 0x%08X", idx, status<br>
 * Type: Error<br>
 * Cause: CFE_ES_RestoreFromCDS() returns an error when restoring data from CDS<br>
 */
#define ECI_CDS_RESTORE_ERR_EID               172

/**
 * Description:  Application error during SIL initialization<br>
 * Text: "ECI Initialization Failed, ECI App Will Exit"<br>
 * Type: Error<br>
 * Cause: app_init() returns an error during initialization<br>
 */
#define ECI_INIT_FAIL_ERR_EID                173
/**@}*/

/**@defgroup crit_ids Critical EIDs
 * EIDs relating to events that would immediately halt program execution.
 * @{
 */
/**@}*/

/**@defgroup debug_ids Debug EIDs
 * EIDs that relate to useful debugging messages.
 * @{
 */
/**
 * Description:  Application has a command/telemetry message buffered or queued<br>
 * Text: "Mid 0x%04X is %s", ((status == QUEUED) ? "QUEUED":"BUFFERED"), MsgRcv[idx].MsgStruct->mid<br>
 * Type: DEBUG<br>
 * Cause: Application has a command/telemetry message buffered or queued<br>
 */
#define ECI_MSG_MANAGE_DBG_EID            211
/**@}*/

#endif  /* ECI_APP_EVENTS_H */
/**@}*/

