#include "cfe.h"
#include "cfe_tbl_msg.h"

/* UT Test includes */
#include "sa_test_utils.h"
#include "utlist.h"
#include "ut_cfe_tbl_hooks.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_sb_hooks.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_time_stubs.h"

#define ECI_FLAG_TABLE_DEFINED 

/* ECI includes */
#include "eci_app_event.h"
#include "eci_app_msgdefs.h"

#include "app_faultrep.h"

/* Sample App includes */
#include "sa_msg.h"
#include "app_msgids.h"
#include "buses.h"

/* Pipe IDs */
#define  CMDPIPE  0
#define  DATAPIPE  1

/* ut_assert structures and definitions for SB msg */
#define UT_CFE_SB_MAX_PIPES    32

typedef struct {
    char                PipeName[OS_MAX_API_NAME];
    UtListHead_t        MsgQueue;
    boolean             InUse;
} Ut_CFE_SB_PipeTableEntry_t;

extern UtListHead_t                MsgQueue;
extern Ut_CFE_SB_PipeTableEntry_t  PipeTable[UT_CFE_SB_MAX_PIPES];

/* Global Table Data */
extern paramTable1_b UT_param_TBL1;
extern paramTable2_b UT_param_TBL2;
extern paramTable1_b UT_param_TBL3;
extern paramTable2_b UT_param_TBL4;
extern paramTable1_b UT_param_TBL5;
extern paramTable2_b UT_param_TBL6;

void SA_TestHelpers_stub_tables() {
   Ut_CFE_TBL_AddTable(CAT_TBL_PATH("param_TBL1.tbl"), &UT_param_TBL1);
   Ut_CFE_TBL_AddTable(CAT_TBL_PATH("param_TBL2.tbl"), &UT_param_TBL2);
}

/*
 * Test will Place a message on the command pipe with an unrecognized message ID.
 */
void SA_AppMain_Test_ECI_1001_1050(void) {

   SA_HkPacket_t       *HkPacket;
   SA_NoArgsCmd_t       UT_CmdPacket;
   SA_NoArgsCmd_t       UT_HkReqMsg;

   SA_TestHelpers_stub_tables();

   CFE_SB_InitMsg(&UT_CmdPacket, UNITTESTMODEL_CMD_MID + 5,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   CFE_SB_InitMsg(&UT_HkReqMsg, SA_SEND_HK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   /* Message with unrecognized Message ID placed on Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_CmdPacket, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_CmdPacket), 0);

   /* HK Request Message placed on Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HkReqMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HkReqMsg), 0);

   unittestmodel_AppMain();

   HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,1);

   /* Verifies that Error Event message sent and command accepted/error counters updated */
   UtAssert_True(HkPacket->CmdErrorCounter == 1, "Command Rejected Counter Incremented");
   UtAssert_True(HkPacket->CmdAcceptCounter == 0, "Command Accepted Counter NOT Incremented");
   UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_INV_MID_ERR_EID, CFE_EVS_ERROR, "Invalid MID Received on Command Pipe: ID = 0x1807") == 1, "Error Event Message Outputted");

} /* End of SA_AppMain_Test_ECI_1001_1050 */

/*
 * Test will Place a message on the data pipe with an unrecognized message ID.
 */
void SA_AppMain_Test_ECI_1010(void) {

   outputTlmPkt2_b   UT_outputPkt;
   SA_NoArgsCmd_t    UT_CmdPacket;
   
   SA_TestHelpers_stub_tables();

   CFE_SB_InitMsg(&UT_outputPkt, INPUTTLMPKT1_B_INPUTTLMPKT1_S_MID + 5, sizeof(outputTlmPkt2_b), TRUE);

   CFE_SB_InitMsg(&UT_CmdPacket, UNITTESTMODEL_TICK_MID, sizeof(SA_NoArgsCmd_t), TRUE);

   /* Data Message with Invalid Message ID placed on data pipe */
   UtList_Add(&PipeTable[DATAPIPE].MsgQueue, &UT_outputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_outputPkt), 0);

   /* Wakeup message placed on command pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_CmdPacket, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_CmdPacket), 0);

   unittestmodel_AppMain();

   /* Verify that error event message outputted */
   UtAssert_True(Ut_CFE_EVS_EventSent(ECI_UNK_MID_ERR_EID, CFE_EVS_ERROR, "Msg with Invalid MID Received on DATA PIPE: ID = 0x80A"), "Error Event Message Outputted");

} /* End of SA_AppMain_Test_ECI_1010 */

/*
 * Test will Place messages with mismatched message lengths on the command & data pipes.
 */
void SA_AppMain_Test_ECI_1020_1030_1050(void) {

   SA_NoArgsCmd_t       UT_CmdPacket[2];
   SA_NoArgsCmd_t       UT_TickMsg;
   SA_NoArgsCmd_t       UT_TickMsgValid;
   SA_NoArgsCmd_t       UT_HK_Req_MsgErr;
   SA_NoArgsCmd_t       UT_HK_Req_Msg;
   SA_FDCConfigCmd_t    UT_FDCConfigCmdPkt;
   SA_FDCClearCmd_t     UT_FDCClearCmdPkt;
   inputCmdPkt_b        UT_InputPkt;
   CFE_TBL_NotifyCmd_t  UT_TblManageCmd;

   SA_HkPacket_t       *HkPacket;

   inputTlmPkt1_b       UT_InputPkt1;
   inputTlmPkt2_b       UT_InputPkt2;

   SA_TestHelpers_stub_tables();

   CFE_SB_InitMsg(&UT_CmdPacket[0], UNITTESTMODEL_CMD_MID,
                   sizeof(SA_NoArgsCmd_t)+1, FALSE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_CmdPacket[0], 0);

   CFE_SB_InitMsg(&UT_CmdPacket[1], UNITTESTMODEL_CMD_MID,
                   sizeof(SA_NoArgsCmd_t)+1, FALSE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_CmdPacket[1], 1);

   CFE_SB_InitMsg(&UT_FDCConfigCmdPkt, UNITTESTMODEL_CMD_MID,
                   sizeof(SA_FDCConfigCmd_t)+1, FALSE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_FDCConfigCmdPkt, ECI_FAULTREP_CONFIG_CC);

   CFE_SB_InitMsg(&UT_FDCClearCmdPkt, UNITTESTMODEL_CMD_MID,
                   sizeof(SA_FDCClearCmd_t)+1, FALSE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_FDCClearCmdPkt, ECI_FAULTREP_CLEAR_CC);

   CFE_SB_InitMsg(&UT_InputPkt, INPUTCMDPKT_B_INPUTCMDPKT_S_MID,
                  sizeof(inputCmdPkt_b)+1, FALSE);

   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_InputPkt, 102);

   CFE_SB_InitMsg(&UT_HK_Req_MsgErr, SA_SEND_HK_MID,
                  sizeof(SA_NoArgsCmd_t)+1, FALSE);

   CFE_SB_InitMsg(&UT_TickMsg, UNITTESTMODEL_TICK_MID,
                  sizeof(SA_NoArgsCmd_t)+1, FALSE);

   CFE_SB_InitMsg(&UT_TickMsgValid, UNITTESTMODEL_TICK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   CFE_SB_InitMsg(&UT_TblManageCmd, SIL_TBL_MANAGE_MID,
                  sizeof(CFE_TBL_NotifyCmd_t)+1, FALSE);

   CFE_SB_InitMsg(&UT_HK_Req_Msg, SA_SEND_HK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   CFE_SB_InitMsg(&UT_InputPkt1, INPUTTLMPKT1_B_INPUTTLMPKT1_S_MID,
                  sizeof(inputTlmPkt1_b)+1, FALSE);

   CFE_SB_InitMsg(&UT_InputPkt2, INPUTTLMPKT2_B_INPUTTLMPKT2_S_MID,
                  sizeof(inputTlmPkt2_b)+1, FALSE);

   /* Length Mismatched No-Op Command Message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_CmdPacket[0], CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_CmdPacket[0]) - 1, 0);

   /* Length Mismatched HK Reset Message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_CmdPacket[1], CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_CmdPacket[1]) - 1, 0);

   /* Length Mismatched Config FDC Command Message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_FDCConfigCmdPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_FDCConfigCmdPkt) - 1, 0);

   /* Length Mismatched Clear FDC Command Message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_FDCClearCmdPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_FDCClearCmdPkt) - 1, 0);

   /* Length Mismatched Auto-coded Command Message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt,     CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt) - 1, 0);

   /* Length Mismatched Table Management Message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TblManageCmd, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TblManageCmd) - 1, 0);

   /* Length Mismatched Wakeup Message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg) - 1, 0);

   /* Length Mismatched HK Request Message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HK_Req_MsgErr, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HK_Req_MsgErr) - 1, 0);

   /* Length Mismatched Data Message (1 of 2) */
   UtList_Add(&PipeTable[DATAPIPE].MsgQueue, &UT_InputPkt1, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt1) - 1, 0);

   /* Length Mismatched Data Message (2 of 2) */
   UtList_Add(&PipeTable[DATAPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2) - 1, 0);

   /* Valid Wakeup Message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsgValid, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsgValid), 0);

   /* Valid HK Request Message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HK_Req_Msg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HK_Req_Msg), 0);

   unittestmodel_AppMain();

   HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,1);

   /* Verify that Command Counters updated and Error Event messages sent */
   UtAssert_True(HkPacket->CmdErrorCounter == 5, "Command Rejected Counter Incremented");
   UtAssert_True(HkPacket->CmdAcceptCounter == 0, "Command Accepted Counter NOT Incremented");

   UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_CMD_LEN_ERR_EID, CFE_EVS_ERROR, "Invalid cmd pkt: ID = ") == 8, "Command Event Error Counter Incremented");
   UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_TLM_LEN_ERR_EID, CFE_EVS_ERROR, "Invalid tlm pkt: ID = ") == 2, "Telemetry Event Error Counter Incremented");
  
} /* End of SA_AppMain_Test_ECI_1020_1030_1050 */

/*
 * Test Of Nominal ECI App Initialization.
 */
void SA_AppMain_Test_INIT_1001_TBL_1002(void) {

   int i;

   SA_NoArgsCmd_t       UT_HK_Req_Msg;
   inputCmdPkt_b        UT_InputPkt;
   SA_NoArgsCmd_t       UT_TickMsg;

   outputTlmPkt3_b      *UT_outputTlmPkt3;
   outputTlmPkt4_b      *UT_outputTlmPkt4;

   SA_HkPacket_t       *UT_HkPacket;

   SA_TestHelpers_stub_tables();

   CFE_SB_InitMsg(&UT_InputPkt, UNITTESTMODEL_CMD_MID,
                   sizeof(inputCmdPkt_b), TRUE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_InputPkt, 102);

   UT_InputPkt.arg1 = 1;

   CFE_SB_InitMsg(&UT_TickMsg, UNITTESTMODEL_TICK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   CFE_SB_InitMsg(&UT_HK_Req_Msg, SA_SEND_HK_MID,
                   sizeof(SA_NoArgsCmd_t), TRUE);

   /* Placed Valid HK Request Message onto Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HK_Req_Msg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HK_Req_Msg), 0);

   /* Placed Auto-generated command onto Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);

   /* Placed Wake-up Message onto Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);

   unittestmodel_AppMain();

   UT_HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,1);
   UT_outputTlmPkt3 = Ut_CFE_SB_FindPacket(OUTPUTTLMPKT3_B_OUTPUTTLMPKT3_S_MID,1);
   UT_outputTlmPkt4 = Ut_CFE_SB_FindPacket(OUTPUTTLMPKT4_B_OUTPUTTLMPKT4_S_MID,1);

   /* Verify that HK Telemetry Initialized Correctly at Startup */
   UtAssert_True(UT_HkPacket->CmdErrorCounter  == 0, "HK Command Rejected Counter initialized to ZERO");
   UtAssert_True(UT_HkPacket->CmdAcceptCounter == 0, "HK Command Accepted Counter initialized to ZERO");

   for (i = 0; i < APP_FAULTREP_BITFIELD_WORDS; i++)
   {
      UtAssert_True(UT_HkPacket->Enabled[i] == 0, "FDC Enabled Clear");
      UtAssert_True(UT_HkPacket->Latched[i] == 0, "FDC Latched Clear");
   }

   for (i = 0; i < 3; i++)
   {
      UtAssert_True(UT_HkPacket->MsgRcvCnt[i] == 0, "Msg Counter Clear");
   }

   /* Verified that parameter tables loaded with default table data from files */
   UtAssert_True(UT_outputTlmPkt3->vals.param1 == UT_param_TBL1.param1, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param2 == UT_param_TBL1.param2, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param3 == UT_param_TBL1.param3, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param4 == UT_param_TBL1.param4, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param5 == UT_param_TBL1.param5, "Initial Table Data Loaded");

   UtAssert_True(UT_outputTlmPkt4->vals.param1 == UT_param_TBL2.param1, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param2 == UT_param_TBL2.param2, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param3 == UT_param_TBL2.param3, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param4 == UT_param_TBL2.param4, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param5 == UT_param_TBL2.param5, "Initial Table Data Loaded");

   /* Check for Event Indicating that Initialization Failed */
   UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_INIT_INF_EID, CFE_EVS_INFORMATION, "") == 1, "ECI_INIT_INF_EID Event Counter Incremented");

} /* End of SA_AppMain_Test_INIT_1001() */

/*
 * Test Of SIL table management.
 */
void SA_AppMain_Test_TBL_1010(void) {

   CFE_TBL_NotifyCmd_t  UT_TblManageCmd_0;
   CFE_TBL_NotifyCmd_t  UT_TblManageCmd_1;
   CFE_TBL_NotifyCmd_t  UT_TblManageCmd_2;
   CFE_TBL_NotifyCmd_t  UT_TblManageCmd_3;

   inputCmdPkt_b        UT_InputPkt;
   SA_NoArgsCmd_t       UT_TickMsg;

   outputTlmPkt3_b      *UT_outputTlmPkt3;
   outputTlmPkt4_b      *UT_outputTlmPkt4;

   CFE_SB_InitMsg(&UT_TblManageCmd_0, SIL_TBL_MANAGE_MID,
                  sizeof(CFE_TBL_NotifyCmd_t), TRUE);

   UT_TblManageCmd_0.Payload.Parameter = 0;

   CFE_SB_InitMsg(&UT_TblManageCmd_1, SIL_TBL_MANAGE_MID,
                  sizeof(CFE_TBL_NotifyCmd_t), TRUE);

   UT_TblManageCmd_1.Payload.Parameter = 1;

   CFE_SB_InitMsg(&UT_TblManageCmd_2, SIL_TBL_MANAGE_MID,
                  sizeof(CFE_TBL_NotifyCmd_t), TRUE);

   UT_TblManageCmd_2.Payload.Parameter = 0;

   CFE_SB_InitMsg(&UT_TblManageCmd_3, SIL_TBL_MANAGE_MID,
                  sizeof(CFE_TBL_NotifyCmd_t), TRUE);

   UT_TblManageCmd_3.Payload.Parameter = 1;

   CFE_SB_InitMsg(&UT_InputPkt, UNITTESTMODEL_CMD_MID,
                   sizeof(inputCmdPkt_b), TRUE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_InputPkt, 102);

   UT_InputPkt.arg1 = 1;

   CFE_SB_InitMsg(&UT_TickMsg, UNITTESTMODEL_TICK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   /* Place Valid Table management messages on command message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TblManageCmd_0, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TblManageCmd_0), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TblManageCmd_1, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TblManageCmd_1), 0);
   /* Place auto-generated command to output active tbl contents in telemetry */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   /* Place Wakeup message onto command message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);

   /* Using CFE_TBL_Manage() stub which indicates table update is pending - Place Valid Table management messages on command message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TblManageCmd_2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TblManageCmd_2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TblManageCmd_3, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TblManageCmd_3), 0);
   /* Place auto-generated command to output active tbl contents in telemetry */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   /* Place Wakeup message onto command message */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);

   Ut_CFE_TBL_SetFunctionHook(UT_CFE_TBL_MANAGE_INDEX, SA_Test_CFE_TBL_Manage);
   SA_Test_Iteration_Reset();

   SA_TestHelpers_stub_tables();

   unittestmodel_AppMain();

   UT_outputTlmPkt3 = Ut_CFE_SB_FindPacket(OUTPUTTLMPKT3_B_OUTPUTTLMPKT3_S_MID,1);
   UT_outputTlmPkt4 = Ut_CFE_SB_FindPacket(OUTPUTTLMPKT4_B_OUTPUTTLMPKT4_S_MID,1);

   /* Verified that parameter tables loaded with default table data from files */
   UtAssert_True(UT_outputTlmPkt3->vals.param1 == UT_param_TBL1.param1, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param2 == UT_param_TBL1.param2, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param3 == UT_param_TBL1.param3, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param4 == UT_param_TBL1.param4, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param5 == UT_param_TBL1.param5, "Initial Table Data Loaded");

   UtAssert_True(UT_outputTlmPkt4->vals.param1 == UT_param_TBL2.param1, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param2 == UT_param_TBL2.param2, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param3 == UT_param_TBL2.param3, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param4 == UT_param_TBL2.param4, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param5 == UT_param_TBL2.param5, "Initial Table Data Loaded");

   UT_outputTlmPkt3 = Ut_CFE_SB_FindPacket(OUTPUTTLMPKT3_B_OUTPUTTLMPKT3_S_MID,2);
   UT_outputTlmPkt4 = Ut_CFE_SB_FindPacket(OUTPUTTLMPKT4_B_OUTPUTTLMPKT4_S_MID,2);

   /* Verified that parameter tables Updated */
   UtAssert_True(UT_outputTlmPkt3->vals.param1 == UT_param_TBL5.param1, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param2 == UT_param_TBL5.param2, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param3 == UT_param_TBL5.param3, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param4 == UT_param_TBL5.param4, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt3->vals.param5 == UT_param_TBL5.param5, "Initial Table Data Loaded");

   UtAssert_True(UT_outputTlmPkt4->vals.param1 == UT_param_TBL6.param1, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param2 == UT_param_TBL6.param2, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param3 == UT_param_TBL6.param3, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param4 == UT_param_TBL6.param4, "Initial Table Data Loaded");
   UtAssert_True(UT_outputTlmPkt4->vals.param5 == UT_param_TBL6.param5, "Initial Table Data Loaded");

} /* End of SA_AppMain_Test_TBL_1010 */

/*
 * Test Of SIL No-Op Command.
 */
void SA_AppMain_Test_ECI_1040_CMD_1001_SB_1001_NoOpCmd(void) {

   SA_NoArgsCmd_t       UT_CmdPacket;
   SA_NoArgsCmd_t       UT_HK_Req_Msg;

   SA_HkPacket_t       *HkPacket;

   SA_TestHelpers_stub_tables();

   CFE_SB_InitMsg(&UT_CmdPacket, UNITTESTMODEL_CMD_MID,
                   sizeof(SA_NoArgsCmd_t), TRUE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_CmdPacket, ECI_NOOP_CC);

   CFE_SB_InitMsg(&UT_HK_Req_Msg, SA_SEND_HK_MID,
                   sizeof(SA_NoArgsCmd_t), TRUE);

   /* Place SIL No-Op Command onto Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_CmdPacket, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_CmdPacket), 0);

   /* Place HK Request Message onto Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HK_Req_Msg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HK_Req_Msg), 0);

   unittestmodel_AppMain();

   HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,1);

   /* Verify that Command Counter Increments and Informational Event Message Outputted */
   UtAssert_True(HkPacket->CmdErrorCounter == 0, "Command Rejected Counter NOT Incremented");
   UtAssert_True(HkPacket->CmdAcceptCounter == 1, "Command Accepted Counter Incremented");
   UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_NOOP_INF_EID, CFE_EVS_INFORMATION, "No-op command") == 1, "Informational No-Op Event Message Sent");
    
} /* End of SA_AppMain_Test_ECI_1040_CMD_1001_SB_1001_NoOpCmd */

/*
 * Test Of SIL HK Counter Reset Command.
 */
void SA_AppMain_Test_SB_1001_CMD_1010_ResetHkCtrsCmd(void) {

   SA_HkPacket_t       *HkPacket;    
   SA_NoArgsCmd_t       UT_CmdPacket;
   SA_NoArgsCmd_t       UT_NoOpPacket;
   SA_NoArgsCmd_t       UT_NoOpPacketErr;
   SA_NoArgsCmd_t       UT_HK_Req_Msg;
   SA_NoArgsCmd_t       UT_TickMsgValid;

   inputCmdPkt_b        UT_inputCmdPkt;
   inputTlmPkt1_b       UT_inputTlmPkt1;
   inputTlmPkt2_b       UT_inputTlmPkt2;

   SA_TestHelpers_stub_tables();

   CFE_SB_InitMsg(&UT_CmdPacket, UNITTESTMODEL_CMD_MID,
                   sizeof(SA_NoArgsCmd_t), TRUE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_CmdPacket, ECI_RESET_HK_COUNTER_CC);

   CFE_SB_InitMsg(&UT_NoOpPacket, UNITTESTMODEL_CMD_MID,
                   sizeof(SA_NoArgsCmd_t), TRUE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_NoOpPacket, ECI_NOOP_CC);

   CFE_SB_InitMsg(&UT_NoOpPacketErr, UNITTESTMODEL_CMD_MID,
                   sizeof(SA_NoArgsCmd_t) + 1, FALSE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_NoOpPacketErr, ECI_NOOP_CC);

   CFE_SB_InitMsg(&UT_inputCmdPkt, INPUTCMDPKT_B_INPUTCMDPKT_S_MID,
                  sizeof(inputCmdPkt_b), FALSE);

   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_inputCmdPkt, 102);

   CFE_SB_InitMsg(&UT_inputTlmPkt1, INPUTTLMPKT1_B_INPUTTLMPKT1_S_MID,
                  sizeof(inputTlmPkt1_b), TRUE);

   CFE_SB_InitMsg(&UT_inputTlmPkt2, INPUTTLMPKT2_B_INPUTTLMPKT2_S_MID,
                  sizeof(inputTlmPkt2_b), TRUE);

   CFE_SB_InitMsg(&UT_HK_Req_Msg, SA_SEND_HK_MID,
                   sizeof(SA_NoArgsCmd_t), TRUE);

   CFE_SB_InitMsg(&UT_TickMsgValid, UNITTESTMODEL_TICK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   /* Place 3 Valid no-Op commands on command pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_NoOpPacket, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_NoOpPacket), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_NoOpPacket, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_NoOpPacket), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_NoOpPacket, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_NoOpPacket), 0);

   /* Place 3 Invalid no-Op commands on command pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_NoOpPacketErr, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_NoOpPacket), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_NoOpPacketErr, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_NoOpPacket), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_NoOpPacketErr, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_NoOpPacket), 0);

   /* Place Autogenerated commands & data messages on Command & Data Pipes */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_inputCmdPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_inputCmdPkt), 0);
   UtList_Add(&PipeTable[DATAPIPE].MsgQueue, &UT_inputTlmPkt1, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_inputTlmPkt1), 0);
   UtList_Add(&PipeTable[DATAPIPE].MsgQueue, &UT_inputTlmPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_inputTlmPkt2), 0);

   /* Place Wakeup Message on Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsgValid, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsgValid), 0);

   /* Place HK Request Message on Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HK_Req_Msg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HK_Req_Msg), 0);

   /* Place HK Counter Reset Command on Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_CmdPacket, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_CmdPacket), 0);

   /* Place HK Request Message on Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HK_Req_Msg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HK_Req_Msg), 0);
    
   unittestmodel_AppMain();
    
   HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,1);

   /* Verify that HK Counters were Increment Initially */
   UtAssert_True(HkPacket->CmdErrorCounter == 3, "Initial Command Rejected Counter Incremented");
   UtAssert_True(HkPacket->CmdAcceptCounter == 4, "Initial Command Accepted Counter Incremented");

   UtAssert_True(HkPacket->MsgRcvCnt[0] == 1, "Initial SIL HK Message 0 Counter Incremented");
   UtAssert_True(HkPacket->MsgRcvCnt[1] == 1, "Initial SIL HK Message 1 Counter Incremented");
   UtAssert_True(HkPacket->MsgRcvCnt[2] == 1, "Initial SIL HK Message 2 Counter Incremented");

   HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,2);

   /* Verify that HK Counters were Reset and Event Message sent */

   UtAssert_True(HkPacket->CmdErrorCounter == 0, "Command Rejected Counter Reset");
   UtAssert_True(HkPacket->CmdAcceptCounter == 0, "Command Accepted Counter Reset");

   UtAssert_True(HkPacket->MsgRcvCnt[0] == 0, "SIL HK Message 0 Counter Reset");
   UtAssert_True(HkPacket->MsgRcvCnt[1] == 0, "SIL HK Message 1 Counter Reset");
   UtAssert_True(HkPacket->MsgRcvCnt[2] == 0, "SIL HK Message 2 Counter Reset");

   UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_RESET_HK_CTR_INF_EID, CFE_EVS_INFORMATION, "Reset Housekeeping Counters") == 1, "Information Event For HK Counter Reset Sent");

   /* Check that HK Telmetry Received */
   UtAssert_True(Ut_CFE_SB_GetMsgCount(UNITTESTMODEL_HK_MID) == 2, "HK Telemetry Packets Received because of two HK request Messages");

} /* End of SA_AppMain_Test_SB_1001_CMD_1010_ResetHkCtrsCmd */

/*
 * Enough autogenerated commands will be received so that command queue will overflow
 */
void SA_AppMain_Test_CMD_1050_1051_1052_1053(void) {
    
   inputCmdPkt_b        UT_InputPkt;
   inputCmdPkt_b        UT_InputPkt2;
   SA_NoArgsCmd_t       UT_HK_Req_Msg;
   SA_NoArgsCmd_t       UT_TickMsg;

   SA_HkPacket_t       *HkPacket;

   SA_TestHelpers_stub_tables();

   CFE_SB_InitMsg(&UT_InputPkt, UNITTESTMODEL_CMD_MID,
                   sizeof(inputCmdPkt_b), TRUE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_InputPkt, 100);

   UT_InputPkt.arg1 = 1;

   CFE_SB_InitMsg(&UT_InputPkt2, UNITTESTMODEL_CMD_MID,
                   sizeof(inputCmdPkt_b), TRUE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_InputPkt2, 110);

   UT_InputPkt2.arg1 = 1;

   CFE_SB_InitMsg(&UT_HK_Req_Msg, SA_SEND_HK_MID,
                   sizeof(SA_NoArgsCmd_t), TRUE);

   CFE_SB_InitMsg(&UT_TickMsg, UNITTESTMODEL_TICK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   /* Will place 28 auto-generated commands onto the command pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);

   /* Will place 30 Wakeup messages onto Command Pipe  */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_TickMsg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_TickMsg), 0);

   /* Will place HK Request Message onto Command Pipe  */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HK_Req_Msg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HK_Req_Msg), 0);

   unittestmodel_AppMain();

   HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,1);

   /* Verify that Command & Error Counter Incremented Correctly, Commands Executed, and Error Event Messages sent */

   UtAssert_True(HkPacket->CmdErrorCounter == 3, "Command Rejected Counter Incremented Correctly for Command Messages that overflow Msg Queue");
   UtAssert_True(HkPacket->CmdAcceptCounter == 25, "Command Accepted Counter Incremented Correctly for Command Messages that are Queued");

   UtAssert_True(Ut_CFE_SB_GetMsgCount(OUTPUTTLMPKT3_B_OUTPUTTLMPKT3_S_MID) == 25, "SIL-Defined Command Executed 1 of 2");
   UtAssert_True(Ut_CFE_SB_GetMsgCount(OUTPUTTLMPKT4_B_OUTPUTTLMPKT4_S_MID) == 25, "SIL-Defined Command Executed 2 of 2");

   UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_QFULL_ERR_EID, CFE_EVS_ERROR, "Cmd Queue for mid 0x1802 is full. Msg dropped") == 3, "Queue Full Error Event Counter Incremented Correctly");

} /* End of SA_AppMain_Test_CMD_1050_1051_1052_1053 */

/*
 * Autogenerated commands with function codes outside the range of valid values will be placed on Command Pipe
 */
void SA_AppMain_Test_CMD_1060(void) {

   inputCmdPkt_b        UT_InputPkt1;
   inputCmdPkt_b        UT_InputPkt2;
   SA_NoArgsCmd_t       UT_HK_Req_Msg;

   SA_HkPacket_t       *HkPacket;

   SA_TestHelpers_stub_tables();

   CFE_SB_InitMsg(&UT_InputPkt1, UNITTESTMODEL_CMD_MID,
                   sizeof(inputCmdPkt_b), TRUE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_InputPkt1, 99);

   CFE_SB_InitMsg(&UT_InputPkt2, UNITTESTMODEL_CMD_MID,
                   sizeof(inputCmdPkt_b), TRUE);
   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&UT_InputPkt2, 111);

   CFE_SB_InitMsg(&UT_HK_Req_Msg, SA_SEND_HK_MID,
                   sizeof(SA_NoArgsCmd_t), TRUE);

   /* Autogenerated command message with function codes outside valid range of 100-110 placed on command pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt1, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt1), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_InputPkt2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputPkt2), 0);

   /* Place HK Request Message on Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HK_Req_Msg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HK_Req_Msg), 0);

   unittestmodel_AppMain();

   HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,1);

   /* Verify that command error counter incremented, commands not executed and Error Event Message outputted */
   UtAssert_True(HkPacket->CmdErrorCounter == 2, "Command Rejected Counter Incremented Correctly");
   UtAssert_True(HkPacket->CmdAcceptCounter == 0, "Command Accepted Counter Correctly Not Increment");

   UtAssert_True(Ut_CFE_SB_GetMsgCount(OUTPUTTLMPKT3_B_OUTPUTTLMPKT3_S_MID) == 0, "SIL-Defined Command Correctly NOT Executed 1 of 2");
   UtAssert_True(Ut_CFE_SB_GetMsgCount(OUTPUTTLMPKT4_B_OUTPUTTLMPKT4_S_MID) == 0, "SIL-Defined Command Correctly NOT Executed 2 of 2");

   UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_INV_CMD_CODE_ERR_EID, CFE_EVS_ERROR, "Invalid Command Code: ID = 0x1802, CC = ") == 2, "Function Code Error Event Counter Incremented Correctly");

} /* End of SA_AppMain_Test_CMD_1060 */

/*
 * Will Verify that SIL will fetch cFE time after wake-up signal received.
 */
void SA_AppMain_Test_TIME_1001(void) {

   SA_NoArgsCmd_t    UT_Tick;

   SA_TestHelpers_stub_tables();

   /* Initialize Time */
   SA_Test_set_time(1000, 0x80000000, 1);
   Ut_CFE_TIME_SetFunctionHook(UT_CFE_TIME_GETTIME_INDEX, SA_Test_CFE_TIME_GetTime);

   /* Place Wake-up Message on Command Pipe */
   CFE_SB_InitMsg(&UT_Tick, UNITTESTMODEL_TICK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_Tick, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_Tick), 0);

   /* Execute SIL App */
   unittestmodel_AppMain();

   /* Verify that cFE time fetched */
   UtAssert_True(Ut_CFE_EVS_GetEventCount(15, CFE_EVS_DEBUG, "Loc: unitTestModel/event1, Time is 1000.500000") == 1, "SIL Time Event Counter Incremented");

} /* End of SA_AppMain_Test_TIME_1001 */

/*
 * Will Verify SIL FDC Handling
 */
void SA_AppMain_Test_FDC_1001_1030(void) {

#ifdef ECI_FLAG_TABLE_DEFINED

   App_FaultRep_SbMsg   *FdcPacket;
   SA_HkPacket_t       *HkPacket;

   inputTlmPkt2_b    inputTlm2[2];
   SA_FDCConfigCmd_t FDCConfigCmd[4];
   SA_FDCClearCmd_t  FDCClearCmd[2];

   SA_NoArgsCmd_t    UT_HKRequest, UT_Tick;

   SA_TestHelpers_stub_tables();

   set_SA_Test_CFE_SB_RcvMsg(INPUTTLMPKT2_B_INPUTTLMPKT2_S_MID);
   Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_RCVMSG_INDEX, SA_Test_CFE_SB_RcvMsg);

   /* Set conditions in Input Data Telemetry to SET FDC */
   CFE_SB_InitMsg(&inputTlm2[0], INPUTTLMPKT2_B_INPUTTLMPKT2_S_MID,
                  sizeof(inputTlmPkt2_b), TRUE);

   inputTlm2[0].inVal1 = 10;
   inputTlm2[0].inVal2 = 8;

   /* Set conditions in Input Data Telemetry to UNSET FDC */
   CFE_SB_InitMsg(&inputTlm2[1], INPUTTLMPKT2_B_INPUTTLMPKT2_S_MID,
                  sizeof(inputTlmPkt2_b), TRUE);

   inputTlm2[1].inVal1 = 8;
   inputTlm2[1].inVal2 = 10;

   /* Set the FDC Config & Clear Cmds */
  
   /* Valid FDC Config Command */
   CFE_SB_InitMsg(&FDCConfigCmd[0], UNITTESTMODEL_CMD_MID,
                  sizeof(SA_FDCConfigCmd_t), TRUE);

   /* Invalid FDC Config Command with invalid FDC ID */
   CFE_SB_InitMsg(&FDCConfigCmd[1], UNITTESTMODEL_CMD_MID,
                  sizeof(SA_FDCConfigCmd_t), TRUE);

   /* Invalid FDC Config Command with invalid FDC Config State */
   CFE_SB_InitMsg(&FDCConfigCmd[2], UNITTESTMODEL_CMD_MID,
                  sizeof(SA_FDCConfigCmd_t), TRUE);

   /* Invalid FDC Config Command with invalid FDC Config State */
   CFE_SB_InitMsg(&FDCConfigCmd[3], UNITTESTMODEL_CMD_MID,
                  sizeof(SA_FDCConfigCmd_t), TRUE);

   /* Valid FDC Clear Command*/
   CFE_SB_InitMsg(&FDCClearCmd[0], UNITTESTMODEL_CMD_MID,
                  sizeof(SA_FDCClearCmd_t), TRUE);

   /* Invalid FDC Clear Command with invalid FDC ID */
   CFE_SB_InitMsg(&FDCClearCmd[1], UNITTESTMODEL_CMD_MID,
                  sizeof(SA_FDCClearCmd_t), TRUE);

   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&FDCConfigCmd[0], ECI_FAULTREP_CONFIG_CC);

   FDCConfigCmd[0].FaultDetId = 0;
   FDCConfigCmd[0].Enable = TRUE;

   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&FDCConfigCmd[1], ECI_FAULTREP_CONFIG_CC);

   FDCConfigCmd[1].FaultDetId = 2;
   FDCConfigCmd[1].Enable = TRUE;

   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&FDCConfigCmd[2], ECI_FAULTREP_CONFIG_CC);

   FDCConfigCmd[2].FaultDetId = 0;
   FDCConfigCmd[2].Enable = 2;

   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&FDCConfigCmd[3], ECI_FAULTREP_CONFIG_CC);

   FDCConfigCmd[3].FaultDetId = 0;
   FDCConfigCmd[3].Enable = FALSE;

   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&FDCClearCmd[0], ECI_FAULTREP_CLEAR_CC);

   FDCClearCmd[0].FaultDetId = 0;

   CFE_SB_SetCmdCode((CFE_SB_MsgPtr_t)&FDCClearCmd[1], ECI_FAULTREP_CLEAR_CC);
   FDCClearCmd[1].FaultDetId = 2;

   /* Set Wake-up Message on Command Pipe */
   CFE_SB_InitMsg(&UT_Tick, UNITTESTMODEL_TICK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   /* Set HK Request Message on Command Pipe */
   CFE_SB_InitMsg(&UT_HKRequest, SA_SEND_HK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   /* Place Valid FDC Config Message enabling FDC ID 0 */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &FDCConfigCmd[0], CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&FDCConfigCmd[0]), 0);

   /* Place Invalid FDC Config Message enabling FDC ID 2 */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &FDCConfigCmd[1], CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&FDCConfigCmd[1]), 0);

   /* Place Invalid FDC Config Message enabling FDC ID 0 with value 2 */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &FDCConfigCmd[2], CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&FDCConfigCmd[2]), 0);

   /* Place Wakeup message onto Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_Tick, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_Tick), 0);

   /* Place HK Request Message onto command pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HKRequest, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HKRequest), 0);

   /* Place valid FDC Clear message clearing FDC ID 0 */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &FDCClearCmd[0], CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&FDCClearCmd[0]), 0);

   /* Place invalid FDC Clear message clearing FDC ID 2 */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &FDCClearCmd[1], CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&FDCClearCmd[1]), 0);

   /* Place HK Request Message onto command pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HKRequest, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HKRequest), 0);

   /* Place Valid FDC Config Message Disabling FDC ID 0 */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &FDCConfigCmd[3], CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&FDCConfigCmd[3]), 0);

   /* Place Wakeup message onto Command Pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_Tick, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_Tick), 0);

   /* Place HK Request Message onto command pipe */
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HKRequest, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HKRequest), 0);

   /* Place data message onto data pipe which will trigger FDC */
   UtList_Add(&PipeTable[DATAPIPE].MsgQueue, &inputTlm2[0], CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&inputTlm2[0]), 0);

   /* Place data message onto data pipe which will unset FDC */
   UtList_Add(&PipeTable[DATAPIPE].MsgQueue, &inputTlm2[1], CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&inputTlm2[1]), 0);

   /* Execute SIL App */
   unittestmodel_AppMain();

   UtAssert_True(1, "ECI_FDC_TABLE_DEFINED is set");

   HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,1);
   FdcPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_FDC_MID,1);

   /* Verify that FDC Config Commands are being validated */
   UtAssert_True(HkPacket->CmdErrorCounter  == 2, "Command Rejected Counter Incremented for two invalid FDC Config Command");
   UtAssert_True(HkPacket->CmdAcceptCounter == 1, "Command Accepted Counter Incremented for valid FDC Config Command");

   /* Verify that FDC State Set Correctly */
   UtAssert_True(HkPacket->Enabled[0] == 1, "FDC Enabled State Set Due to Valid FDC Config Command");
   UtAssert_True(HkPacket->Latched[0] == 1, "FDC Latched State Set due to FDC Condition Met via input telemetry message");
   UtAssert_True(FdcPacket->Tlm.Data[0] == 1, "FDC Tlm Set due to FDC Condition Met via input telemetry message");

   HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,2);

   /* Verify that FDC Clear Commands are being validated */
   UtAssert_True(HkPacket->CmdErrorCounter  == 3, "Command Rejected Counter Incremented for Invalid FDC Clear Command");
   UtAssert_True(HkPacket->CmdAcceptCounter == 2, "Command Accepted Counter Increment for Valid FDC Clear Command");

   /* Verify that FDC State Set Correctly */
   UtAssert_True(HkPacket->Enabled[0] == 1, "FDC Enabled State Remains Unchanged");
   UtAssert_True(HkPacket->Latched[0] == 0, "FDC Latched State cleared due to FDC Clear Command");

   HkPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_HK_MID,3);
   FdcPacket = Ut_CFE_SB_FindPacket(UNITTESTMODEL_FDC_MID,2);

   /* Verify that FDC Config Commands to disable is being validated and executed correctly */
   UtAssert_True(HkPacket->CmdErrorCounter  == 3, "Command Rejected Counter Unchanged");
   UtAssert_True(HkPacket->CmdAcceptCounter == 3, "Command Accepted Counter Increment for Valid FDC Config Command");

   UtAssert_True(HkPacket->Enabled[0] == 0, "FDC Enabled State Reset from Previous Valid FDC Config Command");
   UtAssert_True(HkPacket->Latched[0] == 0, "FDC Latched State cleared due to FDC condition not being met via input telemetry message and the previous FDC clear command");
   UtAssert_True(FdcPacket->Tlm.Data[0] == 0, "FDC Tlm Set due to FDC Condition not being met via input telemetry message");

   UtAssert_True(Ut_CFE_EVS_GetEventCount(APP_FAULTREP_EVS_INV_DETECTOR_ID, CFE_EVS_ERROR, "Fault Reporter Reject Config Detector Cmd: Invalid fault ID 2 (Max ID = 0)") == 1, "Error event received due to FDC Config command with invalid FDC IDs");
   UtAssert_True(Ut_CFE_EVS_GetEventCount(APP_FAULTREP_EVS_INV_DETECTOR_ID, CFE_EVS_ERROR, "Fault Reporter Rejected Clear Detector Cmd:  Invalid fault ID 2 (Max ID = 0)") == 1, "Error event received due to FDC Clear command with invalid FDC IDs");
   UtAssert_True(Ut_CFE_EVS_GetEventCount(APP_FAULTREP_EVS_CONFIG_CMD_ERR, CFE_EVS_ERROR, "Fault Reporter Reject Config Detector Cmd: Invalid enable value 2") == 1, "Error event received due to FDC Config commands with invalid FDC Enable State");

#else

   UtAssert_True(0, "ECI_FLAG_TABLE_DEFINED NOT set - Cannot Test");

#endif

} /* End of SA_AppMain_Test_FDC_1001_1030 */

/* Test of SIL initialization when Table fails to load table values at startup. */
void SA_AppMain_Test_INIT_1012(void) {

   Ut_CFE_ES_SetFunctionHook(UT_CFE_ES_EXITAPP_INDEX, SA_Test_CFE_ES_ExitApp);

   unittestmodel_AppMain();

   Ut_CFE_TBL_AddTable(CAT_TBL_PATH("param_TBL1.tbl"), &UT_param_TBL1);

   unittestmodel_AppMain();

   Ut_CFE_TBL_ClearTables();

   Ut_CFE_TBL_AddTable(CAT_TBL_PATH("param_TBL2.tbl"), &UT_param_TBL2);

   unittestmodel_AppMain();

   /* Check for Event Indicating that Table Load Failed */
   UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_PARAM_TBL_LOAD_ERR_EID, CFE_EVS_ERROR, "") == 3, "ECI_PARAM_TBL_LOAD_ERR_EID Event Counter Incremented");

   /* Check for Event Indicating that Initialization Failed */
   UtAssert_True(Ut_CFE_EVS_GetEventCount(ECI_INIT_FAIL_ERR_EID, CFE_EVS_ERROR, "") == 3, "ECI_INIT_FAIL_ERR_EID Event Counter Incremented");

   /* Check for Event Indicating that Initialization Failed */
   UtAssert_True(Ut_CFE_EVS_GetEventCount(900, CFE_EVS_INFORMATION, "") == 3, "App Exits as Expected");  

} /* End of SA_AppMain_Test_INIT_1012() */

void SA_AppMain_Test_Wakeup(void) {

   inputTlmPkt2_b       UT_InputTlmPkt2_1;
   inputTlmPkt2_b       UT_InputTlmPkt2_2;

   SA_NoArgsCmd_t       UT_Tick;
   SA_NoArgsCmd_t       UT_HK_Req_Msg;

   SA_TestHelpers_stub_tables();

   set_SA_Test_CFE_SB_RcvMsg(INPUTTLMPKT2_B_INPUTTLMPKT2_S_MID);
   Ut_CFE_SB_SetFunctionHook(UT_CFE_SB_RCVMSG_INDEX, SA_Test_CFE_SB_RcvMsg);

   /* Create inputTlmPkt2_b which will set the outputTlmPkt2 data telemetry Output */
   CFE_SB_InitMsg(&UT_InputTlmPkt2_1, INPUTTLMPKT2_B_INPUTTLMPKT2_S_MID,
                   sizeof(inputTlmPkt2_b), TRUE);

   UT_InputTlmPkt2_1.inVal3 = 12;

   /* Create inputTlmPkt2_b which will set the outputTlmPkt2 data telemetry to NOT Output */
   CFE_SB_InitMsg(&UT_InputTlmPkt2_2, INPUTTLMPKT2_B_INPUTTLMPKT2_S_MID,
                   sizeof(inputTlmPkt2_b), TRUE);

   UT_InputTlmPkt2_2.inVal1 = 8;
   UT_InputTlmPkt2_2.inVal2 = 7;
   UT_InputTlmPkt2_2.inVal3 = 7;

   CFE_SB_InitMsg(&UT_HK_Req_Msg, SA_SEND_HK_MID,
                   sizeof(SA_NoArgsCmd_t), TRUE);

   /* Set Wake-up Message on Command Pipe */
   CFE_SB_InitMsg(&UT_Tick, UNITTESTMODEL_TICK_MID,
                  sizeof(SA_NoArgsCmd_t), TRUE);

   UtList_Add(&PipeTable[DATAPIPE].MsgQueue, &UT_InputTlmPkt2_1, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputTlmPkt2_1), 0);
   UtList_Add(&PipeTable[DATAPIPE].MsgQueue, &UT_InputTlmPkt2_2, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_InputTlmPkt2_1), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_Tick, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_Tick), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HK_Req_Msg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HK_Req_Msg), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_Tick, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_Tick), 0);
   UtList_Add(&PipeTable[CMDPIPE].MsgQueue, &UT_HK_Req_Msg, CFE_SB_GetTotalMsgLength((CFE_SB_MsgPtr_t)&UT_HK_Req_Msg), 0);

   unittestmodel_AppMain();

   UtAssert_True(Ut_CFE_SB_GetMsgCount(OUTPUTTLMPKT1_B_OUTPUTTLMPKT1_S_MID) == 2, "SIL Wakeup Results in correct output Telemetry placed on SB 1 of 1");
   UtAssert_True(Ut_CFE_SB_GetMsgCount(OUTPUTTLMPKT2_B_OUTPUTTLMPKT2_S_MID) == 1, "SIL Wakeup Results in correct output Telemetry placed on SB 2 of 2");

   UtAssert_True(Ut_CFE_SB_GetMsgCount(UNITTESTMODEL_FDC_MID) == 2, "SIL Outputs 2 FDC Packets because of 2 received Wake-up signals");

} /* End of SA_AppMain_Test_Wakeup */

/************************/
/*  End of File Comment */
/************************/
