#!/bin/bash
set -e
cd "${0%/*}"/..

CFE_DIR=cfe

make -C $CFE_DIR distclean
make -C $CFE_DIR install
