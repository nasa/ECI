# intended to be run from root of repo
# exit on error
set -e
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
# compile
make clean
make config
make