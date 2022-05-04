cmake_minimum_required(VERSION 3.13)
project(aflplusplus)

set(CMAKE_CXX_STANDARD 17)

# Check for unsupported combinations
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # are we using the special AFL instrumented compiler?
  string(REGEX MATCH "afl-clang-lto\\+\\+$" AFL ${CMAKE_CXX_COMPILER})
else()
  message(FATAL_ERROR "You should use the AFL-instrumented compiler (with LTO)")
endif()

include_directories(../inc
        ../lib/etl/include
        ../lib/eigen
        ../lib/SGP4
        ../lib/geomag)

add_library(afl OBJECT STATIC
        ../src/MathFunctions.cpp
        ../lib/geomag/Geomag.cpp
        ../src/SatelliteModel.cpp
        ../src/MEKF.cpp
        ../src/Albedo.cpp
        ../src/Bdot.cpp
        ../lib/SGP4/sgp4.cpp
        ../src/Wahba.cpp
        ../src/CSSCompensation.cpp
        ../src/Pointing.cpp
        ../src/PointingTarget.cpp
        ../src/NadirPointing.cpp
        ../src/SunPointing.cpp
        ../src/PointingStrategy.cpp
        ../src/MagnetorquerPlusRW.cpp
        ../src/MagnetorquerOnly.cpp)

add_executable(aflplusplus tests/ToTest.cpp
        $<TARGET_OBJECTS:afl>)

add_compile_definitions(EIGEN_STACK_ALLOCATION_LIMIT=0)