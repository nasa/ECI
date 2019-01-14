# intended to be run from root of repo
# exit on error
set -e
# setup environment for compiling
cd ./cFE
. ./setvars.sh
# compile
cd ../unit_test
make clean
make all
# run tests
make run
make gcov