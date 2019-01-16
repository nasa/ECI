
# Introduction
This document describes the format of the interface specification that ECI uses to create and initialize a CFS application. 

# Interface Specification
In this interface specification, any element "defined" by the interface header must be available via including the interface header, but does not necessarily need to be defined in that header file specifically. 

## General
1. The interface specification shall be contained with a file named 'eci_interface.h' (known as the "interface header" henceforth), formatted as a C language header file.
2. The interface header shall have standard C include-guard macros.
3. The interface header should include any header files needed to access data types, macros, or other elements of the external code used within the ECI data structures.

## Definitions
1. The interface header shall define a macro "ECI_APP_REVISION_NUMBER" which contains a string containing the version or revision identifier for the external code.
3. The interface header shall define a macro "ECI_CMD_MID" which contains the APID of the command message.
4. The interface header shall define a macro "ECI_PERF_ID" which contains a unique performance ID for this app.
5. The interface header shall define a macro "ECI_TICK_MID" which contains the APID of the scheduler wakeup message.
7. The interface header shall define a macro "ECI_HK_MID" which contains the APID of the generated ECI HK message.
8. The interface header shall define a macro "ECI_APP_NAME_UPPER" which contains a string containing the name of the CFS app in all uppercase. This name shall be less than OS_MAX_API_NAME characters (including null termination).
9. The interface header shall define a macro "ECI_APP_NAME_LOWER" which contains a string containing the name of the CFS app in all lowercase. This name shall be less than OS_MAX_API_NAME characters (including null termination).
10. The interface header shall define a macro "ECI_CMD_PIPE_NAME" which contains a string containing the desired name of the software bus command pipe.
11. The interface header shall define a macro "ECI_DATA_PIPE_NAME" which contains the desired name of the software bus data pipe.
12. The interface header shall define a macro "ECI_APP_MAIN" which contains the name of the ECI main function. This is the name of the function only, not the function signature, and so should only contain characters valid for C function names.

## Calling interface
This section specified the interface through which the ECI will interact with the external code. Each of these macros must be formatted as valid C expressions (ie, they must be terminated by a semicolon) which call the appropriate external code.

1. The interface header shall define a macro "ECI_STEP_FCN" which contains the expression to call to invoke the external code's step function. 
2. The interface header may define a macro "ECI_INIT_FCN" which contains the expression to call to invoke the external code's initialization function. 
3. The interface header may define a macro "ECI_TERM_FCN" which contains the expression to call to invoke the external code's termination function. 

## Tables
1. If the external code is utilizing the ECI table interface, the interface header shall define a macro "ECI_PARAM_TBL_DEFINED". The value assigned to this macro is not important.
2. The external code shall provide a definition of "static ECI_Tbl_t SIL_ParamTable[]" 
2a. Each element of the SIL_ParamTable shall contain:
    - A pointer to an instance of the table structure in the external code
    - A string containing the name of the table to be registered with CFE table services
    - A string containing a description of the table 
    - A string containing the name of the file containing the table initial values
    - An integer indicating the size of the table (in bytes)
2b. The last element of SIL_ParamTable shall contain all zeros (null terminated)

## State Table

## Input Packets
1. The external code shall provide a definition of "static ECI_Msg_t ECI_MsgRcv[]" 
1a. Each element of the SIL_MsgRcv shall contain:
    - An integer defining the APID of the input message
    - A pointer to memory of sufficient size to contain the message
    - An integer containing the length of the message in bytes
    - If the input is a command, a pointer to memory sufficient to hold a queue of size CMD_MSG_QUEUE_SIZE input packets. Otherwise Null
    - Null

## Output Packets
1. The external code shall provide a definition of "static ECI_Msg_t ECI_MsgSnd[]" 
1a. Each element of the SIL_MsgRcv shall contain:
    - An integer defining the APID of the input message
    - A pointer to memory of sufficient size to contain the message
    - An integer containing the length of the message in bytes
    - Null
    - Null if the packet should be sent each step, otherwise a boolean flag which will result in the packet being sent when true
    
## Events
1. If the external code is utilizing the ECI event interface, the interface header shall define a macro "CSL_EVENT_TABLE_DEFINED". The value assigned to this macro is not important.
2. The external code shall provide a definition of "static const ECI_Evs_t ECI_Events[]" 
2a. Each element of the SIL_ParamTable shall contain:
    - An integer containing the number of data items contained within this event between zero and five.
    - An integer containing a unique (to this app) event ID.
    - An integer containing the type of event (per CFE event services definitions)
    - An integer containing the event mask (per CFE event services definitions)
    - A boolean flag which is equal to true when the event should be issued
    - A string containing a fprintf-style format string
    - A string containing a location identifier within the external code
    - Pointers to between zero and five data values to be placed in the event message. Unused values shall be filled with zeros.
    
## Status Flags
1. If the external code is utilizing the ECI table interface, the interface header shall define a macro "ECI_FDC_TABLE_DEFINED". The value assigned to this macro is not important.
2. The external code shall provide a definition of "static const ECI_Fdc_t ECI_Flags[]" 
2a. Each element of the SIL_Fdc shall contain:
    - An integer which is a unique (for this app) identifier for the status flag
    - A pointer to the status flag
3. The interface header shall define a macro "ECI_FDC_MID" which contains the APID of the generated packet containing the status flags.

## CDS
1. If the external code is utilizing the ECI table interface, the interface header shall define a macro "ECI_CDS_TABLE_DEFINED". The value assigned to this macro is not important.
2. The external code shall provide a definition of "static const ECI_CDS_t ECI_CDS[]" 
2a. Each element of the ECI_CDS shall contain:
    - A string containing the name of the CDS block
    - An integer containing the length of the CDS block
    - A pointer to the CDS block

## Time
1. If the external code is utilizing the ECI time interface, the interface header shall define a macro "ECI_STEP_TIMESTAMP_DEFINED". The value assigned to this macro is not important.

