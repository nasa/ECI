/**
 * @file
 * @brief Specification for ECI app housekeeping telemetry packet
 */

#ifndef ECI_APP_HK_H
#define ECI_APP_HK_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "cfe.h"
#include "eci_util_macro.h"
#include "eci_interface.h"

#ifdef ECI_FLAG_TABLE_DEFINED
#include "app_faultrep.h"
#endif /* ECI_FLAG_TABLE_DEFINED */

/**
 * HK Telemetry Packet definition (SIL housekeeping)
 */
typedef struct ECI_HkPacket_t {
    /** cFE Software Bus Telemetry Message Header */
    uint8    TlmHeader[CFE_SB_TLM_HDR_SIZE];
    /** number of command messages accepted (excluding HK Resets) */
    uint8    CmdAcceptCounter;
    /** number of command messages rejected (excluding HK Resets) */
    uint8    CmdErrorCounter;
#ifdef ECI_FLAG_TABLE_DEFINED
    /** Fault Reporter Enable status */
    uint16   Enabled[APP_FAULTREP_BITFIELD_WORDS];
    /** Fault Reporter Latched status  */
    uint16   Latched[APP_FAULTREP_BITFIELD_WORDS];
#endif
    /** Number of each SIL App-Subscribed Messages Received */
    uint16   MsgRcvCnt[SIZEOF_ARRAY(ECI_MsgRcv)-1];
} ECI_HkPacket_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ECI_APP_HK_H */
