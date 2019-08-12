cmake_minimum_required(VERSION 2.6.4)
project(CFS_ECI_EXAMPLE C)

include_directories(fsw/src)
include_directories(fsw/eci_inc)
include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)

aux_source_directory(fsw/src APP_SRC_FILES)
aux_source_directory(fsw/eci_inc WRAPPER_SRC)

# ECI includes
include_directories(../eci/fsw/src)
include_directories(../eci/fsw/faultrep)

# Add the ECI source files to the project
aux_source_directory(../eci/fsw/src ECI_SRC)
aux_source_directory(../eci/fsw/faultrep ECI_FAULT)

# Add the tables
aux_source_directory(fsw/tables SA_TABLES)

# Create the app module
add_cfe_app(simpleECIApp ${APP_SRC_FILES} ${WRAPPER_SRC} ${ECI_SRC} ${ECI_FAULT})
add_cfe_tables(sa ${SA_TABLES})
