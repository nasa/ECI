cmake_minimum_required(VERSION 2.6.4)
project(CFS_OP C)

# Project includes
include_directories(fsw/src)
include_directories(fsw/eci_inc)
include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)

# ECI includes
include_directories(../eci/fsw/src)
include_directories(../eci/fsw/faultrep)

aux_source_directory(fsw/eci_inc WRAPPER_SRC)
aux_source_directory(fsw/src APP_SRC_FILES)

# Add the ECI source files to the project
aux_source_directory(../eci/fsw/src ECI_SRC)

# Add OP FSW tables
aux_source_directory(fsw/tables OP_TABLES)

# Create the app module
add_cfe_app(op ${APP_SRC_FILES} ${WRAPPER_SRC} ${ECI_SRC})
add_cfe_tables(op ${OP_TABLES})

target_link_libraries(op m)
