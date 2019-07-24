# Onboard Propagator Example

The code in this directory presents an example of integrating an implementation of the SGP4 propagator into a CFS application. 

This code is intended as a realistic example of using ECI to integrate code into the CFS environment. The SGP4 propagator was chosen as it represents an algorithm which could feaibly be needed onboard a spacecraft. No warranty or gaurantee of the flight-worthiness of the propagator or the code in this example is given or implied.

## App Interfaces

The OP app defined by this example uses one parameter table (containing the TLE to propagate), one output packet, and uses the ECI-provided FSW time as the time to propagate to.

The format of the parameter table is:
| Name   | DataType | Dimensions | Description             |
| ------ | -------- | ---------- | ----------------------- |
| Line 1 | char     |  70        | First line of the TLE   |
| Line 2 | char     |  70        | Second line of the TLE  |

The format of the output packet is:
| Name   | DataType | Dimensions | Description                  |
| ------ | -------- | ---------- | ---------------------------- |
| r      | double   |  3         | Propagated position (km)     |
| v      | double   |  3         | Propagated velocity (km/sec) |
| t      | double   |  1         | Propagation timestamp (sec)  |
    
## Directory Structure

This example is setup as a typical CFS app directory. All code is contained within the `fsw` directory.

Within the `fsw` directory, the `src` directory contains the implementation of the SGP4 propagator. Please review the readme included there for additional details. 

The files in `fsw/eci_inc` were written to integrate the propagator code. The propagtor implementation was not suitable for direct integration via the ECI, so a thin wrapper was needed to provide the data structures and function interfaces for the SIL to call. As part of that wrapper, a table validation function was also defined to check the uploaded TLE.

The `fsw/for_build` directory contains files needed for compiling the code, including the makefile.

## Building/integrating this app

To compile this app:

1. Download a copy of the CFS from the github repo: 
```
git clone https://github.com/nasa/cFE
```
2. The CFS repo uses submodules to include the repos for the individual apps. Initalize and update all submodules:
```
cd cFE
git submodule update --init --recursive
```
3. Copy the entire `sgp4Prop` directory into `cfe/apps` and rename it to `op`
4. Copy the entire ECI `fsw` directory into `cfe/apps/eci`
5. Run `. ./setvars`
5. Go to `cfe/build/cpu1`
7. Add the line `THE_APPS += op` to the section defining the CFS apps to compile.
8. Add the line `THE_TBLS += op` to the section defining the CFS tables to compile.
7. Run `make config`
8. Run `make`

