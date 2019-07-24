# Simple App Example

The code in this directory presents an example of integrating a set of simple externally-written code into a CFS app. All interfaces that the ECI provides are exercised, so the eci_interface.h included in this example may be used as an reference implementation of that header file.

## Structure

This example is setup as a typical CFS app directory. All code is contained within the `fsw` directory.

Within the `fsw` directory, the `src` directory contains the existing code which needs to be integrated into a CFS environment. These files contain the implementation of the algorithm. 

The files in `fsw/eci_inc` would be written by the engineer integrating this code to provide the necessary information for the ECI to compile with this code. The files `table_validation.c` and `table_validation.h` would likely not exist already (because the environment the external code was written for likely didn't utilize the concept of tables or validation) and could optionally be written if parameter validation is required. Its also possible that the external code may have implemented some form of validation and the code defined here could leverage that if available (if, for example, the signature of the available validation function didn't match the ECI requirements).

The `fsw/for_build` directory contains files needed for compiling the code, including the makefile.

## Building this code

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
3. Copy the entire `simpleECIApp` directory into `cfe/apps`
4. Copy the entire ECI `fsw` directory into `cfe/apps/eci`
5. Run `. ./setvars`
5. Go to `cfe/build/cpu1`
7. Add the line `THE_APPS += simpleECIApp` to the section defining the CFS apps to compile.
8. Add the line `THE_TBLS += simpleECIApp` to the section defining the CFS tables to compile.
7. Run `make config`
8. Run `make`

