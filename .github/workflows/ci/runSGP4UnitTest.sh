#!/usr/bin/env bash

# intended to be run from root of repo

# enable exit on error when in CI environment
if [[ "$CI" == true ]]; then 
	set -e 
fi

# setup environment for compiling
cd ./cfs
. ./setvars.sh
# compile
cd ../examples/sgp4Prop/unit_test
make clean
make all
# run tests
make run
make gcov
