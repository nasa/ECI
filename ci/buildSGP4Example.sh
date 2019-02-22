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

# make any code changes needed to integrate this app
# ensure CFS builds new app we added
sed -i '44a THE_APPS += op' ./build/cpu1/Makefile
sed -i '50a THE_TBLS += op' ./build/cpu1/Makefile
# configure the app to run when CFS starts
sed -i '5a CFE_APP, /cf/apps/op.so,          op_AppMain,     OP_APP,       90,   8192, 0x0, 0;' ./build/cpu1/exe/cfe_es_startup.scr
sed -i '26a #include "op_app_msgids.h"' ./apps/sch_lab/fsw/platform_inc/sch_lab_sched_tab.h
sed -i '74a      { OP_TICK_MID,   1, 0 },' ./apps/sch_lab/fsw/platform_inc/sch_lab_sched_tab.h
# update makefile to include math library
sed -i '42s/.*/\t$(COMPILER) -m32 -shared -o $@ $(OBJS) -lm/' ./psp/fsw/pc-linux/make/link-rules.mak
# Note: this should be a temporary fix until its determined how to
# add a library via a supported mechanism

# prepare environment
. ./setvars.sh
cd ./build/cpu1

# compile
make clean
make config
make