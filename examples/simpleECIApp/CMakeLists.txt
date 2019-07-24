cmake_minimum_required(VERSION 2.6.4)
project(CFS_ECI_EXAMPLE C)

include_directories(fsw/src)
include_directories(fsw/eci_inc)

aux_source_directory(fsw/src APP_SRC_FILES)

# Create the app module
add_cfe_app(simpleECIApp ${APP_SRC_FILES})

