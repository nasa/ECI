#ifndef _sa_msg_h_
#define _sa_msg_h_

#include "cfe.h"

/**
 ** \brief No argumentscommand data type
 ** 
 ** For command details see #SA_NOOP_CC, SA_RESET_CC
 */
typedef struct {
   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE];

} SA_NoArgsCmd_t;

/* FDC Config Command */
typedef struct {

   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
   uint16   FaultDetId;      /* Single FD identifier or APP_FAULTREP_SELECT_ALL */
   uint8    Enable;          /* TRUE - Enable a FD; FALSE - Disable a FD        */

} SA_FDCConfigCmd_t;

/* FDC Clear Command */
typedef struct {

   uint8    CmdHeader[CFE_SB_CMD_HDR_SIZE]; 
   uint16   FaultDetId;      /* Single FD identifier or APP_FAULTREP_SELECT_ALL */

} SA_FDCClearCmd_t;

/*
 * HK Telemetry Packet definition (SIL housekeeping)
 */
typedef struct {

   uint8    TlmHeader[CFE_SB_TLM_HDR_SIZE];         /* cFE Software Bus Telemetry Message Header */

   uint8    CmdAcceptCounter;                       /* number of command messages accepted (excluding HK Resets) */
   uint8    CmdErrorCounter;                        /* number of command messages rejected (excluding HK Resets) */

#ifdef ECI_FLAG_TABLE_DEFINED
    uint16   Enabled[5];  /* Fault Reporter Enable status */
    uint16   Latched[5];  /* Fault Reporter Latched status  */
#endif

   uint16   MsgRcvCnt[3];  /* Number of each SIL App-Subscribed Messages Received */

} SA_HkPacket_t;







#endif /* _sa_msg_h_ */

/************************/
/*  End of File Comment */
/************************/
