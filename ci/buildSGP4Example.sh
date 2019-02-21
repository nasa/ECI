#!/usr/bin/env bash

# intended to be run from root of repo

# enable exit on error when in CI environment
if [[ "$CI" == true ]]; then 
	set -e 
fi

# copy app files to CFS apps dir
mkdir ./cFE/apps/op
cp -r ./examples/sgp4Prop/* ./cFE/apps/op/
# copy eci source code to CFS apps dir 
mkdir ./cFE/apps/eci
mkdir ./cFE/apps/eci/fsw
cp -r ./fsw/* ./cFE/apps/eci/fsw/
# debugging
ls ./cFE/apps/op/
# setup environment for compiling
cd ./cFE
. ./setvars.sh
cd ./build/cpu1
# ensure CFS builds new app we added
sed -i '44a THE_APPS += op' Makefile
sed -i '50a THE_TBLS += op' Makefile
# configure the app to run when CFS starts
sed -i '5a CFE_APP, /cf/apps/op.so,          op_AppMain,     OP_APP,       90,   8192, 0x0, 0;' ./exe/cfe_es_startup.scr
sed -i '74a { OP_TICK_MID,   1, 0 },' ./inc/sch_lab_sched_tab.h

# compile
make clean
make config
make