#!/bin/bash
set -e
cd "${0%/*}"/..

CFE_APPS=cfe/apps

ECI_DIR=$CFE_APPS/eci
SGP4_DIR=$CFE_APPS/op

SCH_LAB_DIR=sch_lab_sgp4
DEFS_DIR=sgp4_defs

rm -rf $ECI_DIR
rm -rf $SGP4_DIR
rm -rf $CFE_APPS/sch_lab
rm -rf cfe/$DEFS_DIR

mkdir -p $ECI_DIR
mkdir -p $SGP4_DIR
mkdir -p $CFE_APPS/sch_lab

CFE_CMAKE=cfe/cfe/cmake/arch_build.cmake
sed -i "99c\      install(TARGETS \${APP_NAME} DESTINATION \${TGT}/\${INSTALL_SUBDIR}/\${APP_SUBDIR})" $CFE_CMAKE
sed -i "171c\      install(FILES \${TABLE_DESTDIR}/\${TBLWE}.tbl DESTINATION \${TGT}/\${INSTALL_SUBDIR}/\${TBL_SUBDIR})" $CFE_CMAKE

cp -r examples/sgp4Prop/* $SGP4_DIR
cp -r fsw $ECI_DIR

cp -r ci/$SCH_LAB_DIR/* $CFE_APPS/sch_lab
cp -r ci/$DEFS_DIR cfe/$DEFS_DIR

