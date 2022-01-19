#pragma once

#include "Eigen/Geometry"
#include <algorithm>

#include "Definitions.hpp"

inline constexpr int16_t ReflectivityDataRows = 180;
inline constexpr int16_t ReflectivityDataColumns = 288;

inline constexpr auto const EMR = 6371.01e3;

namespace albedo {
/**
 * Transforms radians to TOMS reflectivity matrix indices
 * @param theta reflectivity data point's latitude
 * @param phi reflectivity data point's longitude
 * @return vector containing TOMS reflectivity matrix indices
 */
    inline Eigen::Vector<int16_t, 2> radiansToIndices(const float theta, const float phi) {
        float dx = 2 * PI / ReflectivityDataColumns;
        float dy = PI / ReflectivityDataRows;

        int16_t i = static_cast<int16_t>(round((PI - dy / 2 - phi) / dy));
        int16_t j = static_cast<int16_t>(round((theta + PI - dx / 2) / dx));

        i = std::max(static_cast<int16_t>(0), i);
        j = std::max(static_cast<int16_t>(0), j);

        return {i, j};
    }

/**
 * Transforms TOMS reflectivity matrix indices to radians
 * @param i TOMS reflectivity matrix index
 * @param j TOMS reflectivity matrix index
 * @return vector containing reflectivity data point's latitude and longitude
 */
    inline Eigen::Vector2f indicesToRadians(const int16_t i, const int16_t j) {
        float dx = 2 * PI / ReflectivityDataColumns;
        float dy = PI / ReflectivityDataRows;

        float phi = PI - dy / 2 - i * dy;
        float theta = j * dx - PI + dx / 2;

        return {theta, phi};
    }

/**
 * Calculates the area of a cell with indices i, j
 * @param i index i
 * @param j index j
 * @return area of a cell with indices i, j
 */
    float calculateCellArea(const int16_t i, const int16_t j);

/**
 * Calculates the angle between two grid index pairs
 * @param loopI reflectivity grid index i
 * @param loopJ reflectivity grid index j
 * @param sunIndexI sun grid index i
 * @param sunIndexJ sun grid index j
 * @return angle between two grid index pairs
 */
    float gridAngle(const int16_t loopI, const int16_t loopJ, const int16_t sunIndexI, const int16_t sunIndexJ);
}

/**
 * Calculates the sunlight reflected off the Earth's surface
 * @param satellite satellite position in ECEF frame
 * @param sunPosition sun position in ECEF frame
 * @param reflectivityData Earth surface reflectivity data from TOMS project
 * @return sunlight reflected off the Earth's surface
 */
Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns>
calculateAlbedo(const Eigen::Vector3f &satellite, const Eigen::Vector3f &sunPosition,
                const Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> &reflectivityData);