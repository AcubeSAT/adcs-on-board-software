SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# Project settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
include(cmake/common.cmake)

# ADCS source files
add_library(attitude OBJECT ${ADCS_SOURCES})

add_executable(attitude_example main.cpp
    $<TARGET_OBJECTS:attitude>)

# Catch2 unit testing
IF (EXISTS "${PROJECT_SOURCE_DIR}/lib/Catch2/CMakeLists.txt")
    file(GLOB_RECURSE test_SRC "test/*.cpp")

    add_subdirectory(lib/Catch2)
    add_executable(tests $<TARGET_OBJECTS:attitude> ${test_SRC})
    target_link_libraries(tests Catch2::Catch2)
ENDIF ()
