#!/bin/bash
set -e
ROOT_DIR="${0%/*}"/../..
cd $ROOT_DIR

SGP4_SCRIPT_DIR=${PWD}/ci/sgp4
DEFS_DIR=$SGP4_SCRIPT_DIR/sgp4_defs

CFE_DIR=${PWD}/cfe
CFE_APPS=$CFE_DIR/apps
SCH_DIR=$CFE_APPS/sch_lab
ECI_DIR=$CFE_APPS/eci
APP_DIR=$CFE_APPS/op

rm -rf $ECI_DIR
rm -rf $SGP4_DIR
rm -rf $CFE_DIR/*_defs

mkdir -p $ECI_DIR
mkdir -p $APP_DIR

# Add the feature to change our table/apps dir in the install dir
CFE_CMAKE=$CFE_DIR/cfe/cmake/arch_build.cmake
sed -i "99c\      install(TARGETS \${APP_NAME} DESTINATION \${TGT}/\${INSTALL_SUBDIR}/\${APP_SUBDIR})" $CFE_CMAKE
sed -i "171c\      install(FILES \${TABLE_DESTDIR}/\${TBLWE}.tbl DESTINATION \${TGT}/\${INSTALL_SUBDIR}/\${TBL_SUBDIR})" $CFE_CMAKE

cp $SGP4_SCRIPT_DIR/sch_lab_sched_tab.h $SCH_DIR/fsw/platform_inc/
cp -r $DEFS_DIR $CFE_DIR

cp -r examples/sgp4Prop/* $APP_DIR
cp -r fsw $ECI_DIR

