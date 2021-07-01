cmake_minimum_required(VERSION 3.01)
add_definitions(-Wfatal-errors)
include(../setup_cfe.cmake)

set_common_paths("${CMAKE_SOURCE_DIR}/..")

install_eci()
install_app_defs("sgp4")

file(GLOB APP_SRC "${ECI_ROOT}/examples/sgp4Prop/*")
file(INSTALL ${APP_SRC} DESTINATION "${CFS_APPS}/op")

