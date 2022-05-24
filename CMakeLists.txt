cmake_minimum_required(VERSION 3.15)
project(adcs-onboard-software C CXX ASM)
include(ExternalProject)

set(
    BUILD_KIND SUPERBUILD CACHE STRING 
    "Select build kind: ATSAM, NATIVE or SUPERBUILD"
)


if ( ${BUILD_KIND} MATCHES "NATIVE" )
    include(cmake/Native.cmake)
elseif ( ${BUILD_KIND} MATCHES "ATSAM" )
    include(cmake/ATSAM.cmake)
elseif ( ${BUILD_KIND} MATCHES "SUPERBUILD" )
    # This superbuild contains the two separate builds as external
    # projects. These projects have their root in this file, but
    # are exposed by CMake according to the -DBUILD_KIND argument.
    ExternalProject_Add (native
        DEPENDS ${DEPENDENCIES}
        SOURCE_DIR ${PROJECT_SOURCE_DIR}
        CMAKE_ARGS -DBUILD_KIND=NATIVE ${EXTRA_CMAKE_ARGS}
        INSTALL_COMMAND ""
        BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/native
    )

    ExternalProject_Add (atsam
        DEPENDS ${DEPENDENCIES}
        SOURCE_DIR ${PROJECT_SOURCE_DIR}
        CMAKE_ARGS -DBUILD_KIND=ATSAM ${EXTRA_CMAKE_ARGS}
        INSTALL_COMMAND ""
        BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/atsam
    )
else ()
    message ( FATAL_ERROR "Unknown build kind: ${BUILD_KIND}" )
endif () 

set (DEPENDENCIES)
set (EXTRA_CMAKE_ARGS)
