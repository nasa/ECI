#!/usr/bin/env bash

# intended to be run from root of repo

# enable exit on error when in CI environment
if [[ "$CI" == true ]]; then 
	set -e 
fi

# copy app files to CFS apps dir
mkdir ./cFS/apps/op
cp -r ./examples/sgp4Prop/* ./cFS/apps/op/
# copy eci source code to CFS apps dir 
mkdir ./cFS/apps/eci
mkdir ./cFS/apps/eci/fsw
cp -r ./fsw/* ./cFS/apps/eci/fsw/
# debugging
ls ./cFS/apps/op/
# setup environment for compiling
cd ./cFS

# make any code changes needed to integrate this app
# ensure CFS builds new app we added
sed -i '105a SET(cpu1_APPLIST ci_lab to_lab sch_lab op)' ./sample_defs/targets.cmake
# configure the app to run when CFS starts
sed -i '6a CFE_APP, op, op_AppMain,     OP_APP,       90,   8192, 0x0, 0;' ./sample_defs/cpu1_cfe_es_startup.scr
# Note: this should be a temporary fix until its determined how to
# add a library via a supported mechanism

# compile
make SIMULATION=native prep
make
