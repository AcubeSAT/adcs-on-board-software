#pragma once

#include "Eigen/Geometry"
#include <cstdint>

using std::uint8_t;

inline constexpr double const PI = 3.14159265358979323846;
inline constexpr double const axisOfEarth = 6378137.0;
inline constexpr double const flattening = 1.0 / 298.257223563;
inline constexpr double const eccentricitySquared = (2.0 - flattening) * flattening;

inline constexpr uint8_t const VectorSize = 3;
inline constexpr uint8_t const NumOfActuators = 2;
inline constexpr uint8_t const LocalStateSize = 6;
inline constexpr uint8_t const GlobalStateSize = 7;
inline constexpr uint8_t const MeasurementSize = 6;
inline constexpr uint8_t const ReflectivityDataRows = 180;
inline constexpr uint16_t const ReflectivityDataColumns = 288;

typedef Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> EarthCellsMatrix;
typedef Eigen::Vector<float, MeasurementSize> MeasurementVector;
typedef Eigen::Vector<float, GlobalStateSize> GlobalStateVector;
