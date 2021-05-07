cmake_minimum_required(VERSION 3.13)
project(adcs-onboard-software)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/inc
        ${PROJECT_SOURCE_DIR}/lib/eigen)

add_library(math-lib OBJECT
        src/mathFunctions.cpp)

add_library(satellite-model OBJECT
        src/SatelliteModel.cpp)

add_executable(adcs-onboard-software main.cpp
        $<TARGET_OBJECTS:math-lib>
        $<TARGET_OBJECTS:satellite-model>
        )

target_link_libraries(adcs-onboard-software eigen)
add_subdirectory(lib/eigen)

IF (EXISTS "${PROJECT_SOURCE_DIR}/lib/Catch2/CMakeLists.txt")
    # Gather all the .cpp files corresponding to tests
    file(GLOB test_main_SRC "test/*.cpp")
    file(GLOB test_SRC "test/**/*.cpp")

    add_subdirectory(lib/Catch2)
    add_executable(tests $<TARGET_OBJECTS:math-lib> ${test_main_SRC} ${test_SRC})
    target_link_libraries(tests Catch2::Catch2)
ENDIF ()
