cmake_minimum_required(VERSION 3.13)
project(adcs-onboard-software)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/inc
        ${PROJECT_SOURCE_DIR}/lib/etl/include
        ${PROJECT_SOURCE_DIR}/lib/eigen
        ${PROJECT_SOURCE_DIR}/lib/SGP4
        ${PROJECT_SOURCE_DIR}/lib/geomag)

add_library(adcs OBJECT
        src/MathFunctions.cpp
        lib/geomag/Geomag.cpp
        src/SatelliteModel.cpp
        src/MEKF.cpp
        src/Albedo.cpp
        src/Bdot.cpp
        lib/SGP4/sgp4.cpp
        src/Wahba.cpp
        src/CSSCompensation.cpp
        src/Pointing.cpp
        src/PointingTarget.cpp
        src/NadirPointing.cpp
        src/SunPointing.cpp
        src/PointingStrategy.cpp
        src/MagnetorquerPlusRW.cpp
        src/MagnetorquerOnly.cpp
        src/Eclipse.cpp
        src/SunPosition.cpp
        src/EnvironmentalModel.cpp
        src/OrbitalParameters.cpp)

add_executable(adcs-onboard-software main.cpp
        $<TARGET_OBJECTS:adcs>)

add_compile_definitions(EIGEN_STACK_ALLOCATION_LIMIT=0)

IF (EXISTS "${PROJECT_SOURCE_DIR}/lib/Catch2/CMakeLists.txt")
    # Gather all the .cpp files corresponding to tests
    file(GLOB test_main_SRC "test/*.cpp")
    file(GLOB test_SRC "test/**/*.cpp")

    add_subdirectory(lib/Catch2)
    add_executable(tests $<TARGET_OBJECTS:adcs>
            ${test_main_SRC}
            ${test_SRC})
    target_link_libraries(tests Catch2::Catch2)
ENDIF ()
