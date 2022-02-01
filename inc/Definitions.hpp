#pragma once 

#include "Eigen/Geometry"
#include <cstdint>

using std::uint8_t;

inline constexpr auto const PI = 3.14159265358979323846;

inline uint8_t const VectorSize = 3;
inline uint8_t const NumOfActuators = 2;
inline uint8_t const LocalStateSize = 6;
inline uint8_t const GlobalStateSize = 7;
inline uint8_t const MeasurementSize = 6;
inline uint8_t const ReflectivityDataRows = 180;
inline int16_t const ReflectivityDataColumns = 288;

typedef Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> EarthCellsMatrix;
typedef Eigen::Vector<float, MeasurementSize> MeasurementVector;
typedef Eigen::Vector<float, GlobalStateSize> GlobalStateVector;
