function(set_common_paths CI_DIR)
    # Setup variables for root directories
    set(CI_DIR ${CI_DIR} PARENT_SCOPE)
    get_filename_component(ECI_ROOT "${CI_DIR}/../../.." ABSOLUTE CACHE)
    get_filename_component(CFS_ROOT "${CMAKE_CURRENT_BINARY_DIR}/cfs" ABSOLUTE CACHE)
    get_filename_component(CFS_APPS "${CFS_ROOT}/apps" ABSOLUTE CACHE)
endfunction(set_common_paths)

function(install_eci)
    create_cfe()
    get_filename_component(ECI_DIR "${CFS_APPS}/eci" ABSOLUTE)

    # Clean out cfe directory
    file(REMOVE_RECURSE ${ECI_DIR})
    file(GLOB CFS_DEFS "${CFS_ROOT}/*_defs")
    if(${CFS_DEFS})
        file(REMOVE_RECURSE ${CFS_DEFS})
    endif(${CFS_DEFS})

    set(CMAKE_INSTALL_MESSAGE NEVER)
    # Install the configurations for building the app
    file(COPY "${CFS_ROOT}/cfe/cmake/Makefile.sample" DESTINATION "${CFS_ROOT}")
    file(RENAME "${CFS_ROOT}/Makefile.sample" "${CFS_ROOT}/Makefile")

    # Install ECI src
    file(INSTALL "${ECI_ROOT}/fsw" DESTINATION "${CFS_APPS}/eci")
endfunction(install_eci)

function(install_app_defs APP_NAME)
    get_filename_component(APP_DIR "${CFS_APPS}/${APP_NAME}" ABSOLUTE)
    file(REMOVE_RECURSE ${APP_DIR})
    file(COPY "${CI_DIR}/common_defs/" DESTINATION "${CFS_ROOT}/sample_defs")
endfunction(install_app_defs)

function(create_cfe)
    if(NOT EXISTS ${CFS_ROOT})
        execute_process(COMMAND git clone -v https://github.com/nasa/cFS.git ${CFS_ROOT})
        execute_process(COMMAND git -C ${CFS_ROOT} --work-tree=${CFS_ROOT} checkout main)
        execute_process(COMMAND git -C ${CFS_ROOT} submodule update --init --recursive)
    endif()
endfunction(create_cfe)
