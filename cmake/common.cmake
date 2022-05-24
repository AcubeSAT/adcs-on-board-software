# ADCS includes
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/inc/Attitude)
include_directories(${PROJECT_SOURCE_DIR}/lib/etl/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/eigen)
include_directories(${PROJECT_SOURCE_DIR}/lib/SGP4)
include_directories(${PROJECT_SOURCE_DIR}/lib/geomag)

# ADCS source files
set(ADCS_SOURCES
    lib/geomag/Geomag.cpp
    lib/geomag/GeomagVector.c
    lib/SGP4/sgp4.cpp
    src/Attitude/Albedo.cpp
    src/Attitude/Bdot.cpp
    src/Attitude/CSSCompensation.cpp
    src/Attitude/EnvironmentalModel.cpp
    src/Attitude/MagnetorquerOnly.cpp
    src/Attitude/MagnetorquerPlusRW.cpp
    src/Attitude/MathFunctions.cpp
    src/Attitude/MEKF.cpp
    src/Attitude/NadirPointing.cpp
    src/Attitude/OrbitalParameters.cpp
    src/Attitude/Pointing.cpp
    src/Attitude/PointingStrategy.cpp
    src/Attitude/PointingTarget.cpp
    src/Attitude/SatelliteModel.cpp
    src/Attitude/SunPointing.cpp
    src/Attitude/Wahba.cpp
)

# Compile definitions
add_compile_definitions(EIGEN_STACK_ALLOCATION_LIMIT=0)