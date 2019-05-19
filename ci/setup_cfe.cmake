
# Setup variables for root directories
get_filename_component(ECI_ROOT "${CI_DIR}/.." ABSOLUTE)
get_filename_component(CFE_ROOT "${ECI_ROOT}/cfe" ABSOLUTE)
get_filename_component(CFE_APPS "${CFE_ROOT}/apps" ABSOLUTE)

get_filename_component(ECI_DIR "${CFE_APPS}/eci" ABSOLUTE)
get_filename_component(APP_DIR "${CFE_APPS}/${APP_NAME}" ABSOLUTE)

# Clean out cfe directory
file(REMOVE_RECURSE ${ECI_DIR})
file(REMOVE_RECURSE ${APP_DIR})
file(GLOB CFE_DEFS "${CFE_ROOT}/*_defs")
file(REMOVE_RECURSE ${CFE_DEFS})

set(CMAKE_INSTALL_MESSAGE NEVER)
# Install the configurations for building the app
file(INSTALL "sch_lab_sched_tab.h" DESTINATION "${CFE_APPS}/sch_lab/fsw/platform_inc")
file(INSTALL "${APP_NAME}_defs" DESTINATION "${CFE_ROOT}")
file(COPY "${CI_DIR}/common_defs/" DESTINATION "${CFE_ROOT}/${APP_NAME}_defs")

# Install ECI src
file(INSTALL "${ECI_ROOT}/fsw" DESTINATION "${CFE_ROOT}/apps/eci")

# Add the ability to set the app/table destination in the cfe cmake
set(CFE_ARCH_CMAKE "${CFE_ROOT}/cfe/cmake/arch_build.cmake")
execute_process(COMMAND sed -i "99c\      install(TARGETS \${APP_NAME} DESTINATION \${TGT}/\${INSTALL_SUBDIR}/\${APP_SUBDIR})" ${CFE_ARCH_CMAKE})
execute_process(COMMAND sed -i "171c\      install(FILES \${TABLE_DESTDIR}/\${TBLWE}.tbl DESTINATION \${TGT}/\${INSTALL_SUBDIR}/\${TBL_SUBDIR})" ${CFE_ARCH_CMAKE})
