/**
 * File:  eci_app_hk.h
 * Description:  Specification for ECI app housekeeping telemetry packet
 */

#ifndef ECI_APP_HK_H
#define ECI_APP_HK_H

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
#include "eci_util_macro.h"
#include "eci_interface.h"

#ifdef ECI_FLAG_TABLE_DEFINED
#include "app_faultrep.h"
#endif /* ECI_FLAG_TABLE_DEFINED */

/************************************************************************
** Type Definitions
*************************************************************************/

/*
 * HK Telemetry Packet definition (SIL housekeeping)
 */
typedef struct {

   uint8    TlmHeader[CFE_SB_TLM_HDR_SIZE];         /* cFE Software Bus Telemetry Message Header */

   uint8    CmdAcceptCounter;                       /* number of command messages accepted (excluding HK Resets) */
   uint8    CmdErrorCounter;                        /* number of command messages rejected (excluding HK Resets) */

#ifdef ECI_FLAG_TABLE_DEFINED
    uint16   Enabled[APP_FAULTREP_BITFIELD_WORDS];  /* Fault Reporter Enable status */
    uint16   Latched[APP_FAULTREP_BITFIELD_WORDS];  /* Fault Reporter Latched status  */
#endif

   uint16   MsgRcvCnt[SIZEOF_ARRAY(ECI_MsgRcv)-1];  /* Number of each SIL App-Subscribed Messages Received */

} ECI_HkPacket_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ECI_APP_HK_H */

/************************/
/*  End of File Comment */
/************************/
