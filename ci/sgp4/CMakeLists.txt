cmake_minimum_required(VERSION 3.10)

set(APP_NAME "sgp4")

get_filename_component(SETUP_DIR "${CMAKE_SOURCE_DIR}" ABSOLUTE)
get_filename_component(CI_DIR "${CMAKE_SOURCE_DIR}/.." ABSOLUTE)

include(../setup_cfe.cmake)

file(GLOB APP_SRC "../../examples/sgp4Prop/*")
file(INSTALL ${APP_SRC} DESTINATION "../../cfe/apps/op")
