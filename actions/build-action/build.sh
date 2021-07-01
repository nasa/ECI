#!/bin/sh -l

echo "This action is to build ECI"
../ci/fetchCFE.sh
../ci/buildSGP4Example.sh

