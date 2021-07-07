#!/usr/bin/env bash

# intended to be run from root of repo

# enable exit on error when in CI environment
if [[ "$CI" == true ]]; then 
	set -e 
fi

# copy app files to CFS apps dir
mkdir ./cFS/apps/simpleECIApp
cp -r ./examples/simpleECIApp/* ./cFS/apps/simpleECIApp/
# copy eci source code to CFS apps dir 
mkdir ./cFS/apps/eci
mkdir ./cFS/apps/eci/fsw
cp -r ./fsw/* ./cFS/apps/eci/fsw/
# setup environment for compiling
cd ./cFS
# ensure CFS builds new app we added
sed -i '106a SET(TGT1_APPLIST ci_lab to_lab sch_lab simpleECIApp)' ./sample_defs/targets.cmake
sed -i '5a CFE_APP, /cf/simpleECIApp.so,          sa_AppMain,     SA_APP,       90,   8192, 0x0, 0;' ./sample_defs/cpu1_cfe_es_startup.scr

# compile
make prep
make