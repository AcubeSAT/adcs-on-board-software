#pragma once

#include "Eigen/Geometry"
#include <cstdint>

using std::uint8_t;

inline constexpr double const PI = 3.14159265358979323846;
inline constexpr uint8_t NumberOfWahbaLoops = 2;
inline constexpr uint8_t VectorSize = 3;
inline constexpr uint8_t NumOfActuators = 2;
inline constexpr uint8_t LocalStateSize = 6;
inline constexpr uint8_t GlobalStateSize = 7;
inline constexpr uint8_t MeasurementSize = 6;
inline constexpr uint8_t ReflectivityDataRows = 180;
inline constexpr uint16_t ReflectivityDataColumns = 288;

typedef Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> EarthCellsMatrix;
typedef Eigen::Vector<float, MeasurementSize> MeasurementVector;
typedef Eigen::Vector<float, GlobalStateSize> GlobalStateVector;
