/*
 * File:  eci_app_msgdefs.h
 * Description:  Specification for ECI app command codes
 */

#ifndef ECI_APP_MSGDEFS_H
#define ECI_APP_MSGDEFS_H

/**
 * :Description:  No-Op Command
 * :Command Verification:  HK command counter increments and event# ECI_NOOP_INF_EID issued
 * :Error Condition:  None
 */
#define ECI_NOOP_CC                    0   

/**
 * :Description:  HK Counters Reset
 * :Command Verification:  HK command, error, and Msg counters reset to zero, event#
 *                         ECI_RESET_HK_CTR_INF_EID issued
 * :Error Condition:  None
 */
#define ECI_RESET_HK_COUNTER_CC        1   

#ifdef ECI_FLAG_TABLE_DEFINED

/**
 * :Description:  Configures Fault Reporter if FDC reporting enabled
 * :Command Verification:  FDC enabled bitfield in HK telemetry set
 *                         accordingly and command counter incremented
 * :Error Condition:  Invalid FDC ID specified in command
 */
#define ECI_FAULTREP_CONFIG_CC         2

/**
 * :Description:  Clears Fault Reporter if FDC reporting enabled
 * :Command Verification:  FDC latched bitfield in HK telemetry and status
 *                         bitfield in FDC telemetry set accordingly and
 *                         command counter incremented
 * :Error Condition:  Invalid FDC ID specified in command
 */
#define ECI_FAULTREP_CLEAR_CC          3

#endif /* ECI_FLAG_TABLE_DEFINED */

#endif /* ECI_APP_MSGDEFS_H */

