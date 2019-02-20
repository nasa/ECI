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
. ./setvars.sh
cd ./build/cpu1
# ensure CFS builds new app we added
sed -i '44a THE_APPS += simpleECIApp' Makefile
sed -i '50a THE_TBLS += simpleECIApp' Makefile
# compile
make clean
make config
make