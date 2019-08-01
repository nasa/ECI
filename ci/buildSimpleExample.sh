#!/usr/bin/env bash

# intended to be run from root of repo

# enable exit on error when in CI environment
if [[ "$CI" == true ]]; then 
	set -e 
fi

# copy app files to CFS apps dir
mkdir ./cFE/apps/simpleECIApp
cp -r ./examples/simpleECIApp/* ./cFE/apps/simpleECIApp/
# copy eci source code to CFS apps dir 
mkdir ./cFE/apps/eci
mkdir ./cFE/apps/eci/fsw
cp -r ./fsw/* ./cFE/apps/eci/fsw/
# setup environment for compiling
cd ./cFE
# ensure CFS builds new app we added
sed -i '44a THE_APPS += simpleECIApp' ./build/cpu1/Makefile
sed -i '50a THE_TBLS += simpleECIApp' ./build/cpu1/Makefile

sed -i '5a CFE_APP, /cf/apps/simpleECIApp.so,          sa_AppMain,     SA_APP,       90,   8192, 0x0, 0;' ./build/cpu1/exe/cfe_es_startup.scr
sed -i '26a #include "simpleECI_app_msgids.h"' ./apps/sch_lab/fsw/platform_inc/sch_lab_sched_tab.h
sed -i '74a      { SIMPLE_ECI_TICK_MID,   1, 0 },' ./apps/sch_lab/fsw/platform_inc/sch_lab_sched_tab.h

. ./setvars.sh
cd ./build/cpu1

# compile
make clean
make config
make