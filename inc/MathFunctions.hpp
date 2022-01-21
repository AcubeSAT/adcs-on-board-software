#pragma once

#include "Eigen/Geometry"
#include "Definitions.hpp"

/**
 * Calculates the product of two quaternions
 * @param quaternion1 quaternion that expresses a rotation
 * @param quaternion2 quaternion that expresses another rotation
 * @return quaternion equivalent to applying the two rotations in sequence
 */
Eigen::Quaternionf quaternionProduct(const Eigen::Quaternionf &quaternion1, const Eigen::Quaternionf &quaternion2);

/**
 * Calculates the exponential map of the rotational velocity
 * @param angularVelocity rotational velocity
 * @return quaternion that expresses rotation equal to rotational velocity (angularVelocity) in seconds
 */
Eigen::Quaternionf quaternionExponent(const Eigen::Vector3f angularVelocity);

/**
 * Rotates 3D vector by quaternion.
 * This function implements v' = q_conj * v * q
 * To implement v' = q * v * q_conj just switch the order of the cross products.
 * @param quaternion quaternion that expresses a rotation from a frame to another
 * @param vector vector to be rotated from a frame to another
 * @return rotated vector
 */
Eigen::Vector3f rotateVector(const Eigen::Quaternionf &quaternion, const Eigen::Vector3f &vector);

/**
 * Maps a 3-dimensional vector to its corresponding skew-symmetric matrix
 * @param vector 3-dimensional vector
 * @return skew-symmetric matrix
 */
Eigen::Matrix<float, 3, 3> skew(const Eigen::Vector3f vector);

/**
 * Convert degrees to rads
 */
inline double deg2rad(const double degrees) {
    return PI * degrees / 180;
}

/**
 * Convert rads to degrees
 */
inline double rad2deg(const double rads) {
    return 180 * rads / PI;
}

/**
 * This is a date format consists of 2 parts:
 * The integer part of the number is the year.
 * The floating point part is the fraction of the year for the specified date.
 */
typedef double DecimalDate;

/**
 * Converts date input parameters to decimal date format
 * STEPS:
 * Date = YYYY MM DD HH MM SS
 * 1. Store each month's days
 * 2. Calculate if the current year is a leap year
 * 3. Calculate the input day number
 * 4. Calculate the input hour number
 * 5. Fraction = Input Number Calculated / Total Number
 * 6. Decimal Date = YYYY + Fraction
 */
DecimalDate date2decimal(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t minute, const uint8_t second);

/**
 * Transforms Cartesian to Spherical coordinates
 * @param vectorCartesian vector in Cartesian coordinates (x, y, z) in radians
 * @return vector in Spherical coordinates (azimuth, elevation, radius) in radians
 */
Eigen::Vector3f cartesianToSpherical(const Eigen::Vector3f vectorCartesian);

/**
 * Transforms Spherical to Cartesian coordinates
 * @param vectorSpherical vector in Spherical coordinates (azimuth, elevation, radius) in radians
 * @return vector in Cartesian coordinates (x, y, z) in radians
 */
Eigen::Vector3f sphericalToCartesian(const Eigen::Vector3f vectorSpherical);

/**
 * Clamps the values of an Eigen vector between an upper and lower bound
 * @param vector vector to be clamped
 * @param lowerLimit lower bound
 * @param upperLimit upper bound
 * @return clamped vector
 */
inline Eigen::Vector3f
clamp(Eigen::Vector3f vector, const Eigen::Vector3f &lowerLimit, const Eigen::Vector3f &upperLimit) {
    for (int i = 0; i < 3; i++) {
        vector(i) = std::max(lowerLimit(i), std::min(vector(i), upperLimit(i)));
    }

    return vector;
}

/**
 * Clamps a value between an upper and lower bound
 * @param number number to be clamped
 * @param lowerLimit lower bound
 * @param upperLimit upper bound
 * @return clamped vector
 */
template<typename T>
T clamp(T number, const T lowerLimit, const T upperLimit) {
    return std::max(lowerLimit, std::min(number, upperLimit));
}

/**
 * Clamps a value in case it is lower than a limit
 * @param number number to be clamped
 * @return clamped number 
 */
template<typename T>
T clampDown(T number, const T limit) {
    if (number < limit) {
        number = limit;
    }

    return number;
}

/**
 * Clamps a value in case it is upper than a limit
 * @param number number to be clamped
 * @return clamped number 
 */
template<typename T>
T clampUp(T number, const T limit) {
    if (number > limit) {
        number = limit;
    }

    return number;
}