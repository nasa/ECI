#!/bin/sh -l

echo "This action is to build ECI"
sudo apt-get update
sudo apt-get install gcc-multilib
. ./ci/fetchCFE.sh
. ./ci/buildSGP4Example.sh

