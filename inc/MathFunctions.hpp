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
Eigen::Quaternionf quaternionExponent(Eigen::Vector3f angularVelocity);

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
Eigen::Matrix<float, 3, 3> skew(Eigen::Vector3f vector);

/**
 * Convert degrees to rads
 */
inline double deg2rad(double degrees) {
    return PI * degrees / 180;
}

/**
 * Convert rads to degrees
 */
inline double rad2deg(double rads) {
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
DecimalDate date2decimal(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

/**
 * Transforms Cartesian to Spherical coordinates
 * @param vectorCartesian vector in Cartesian coordinates (x, y, z) in radians
 * @return vector in Spherical coordinates (azimuth, elevation, radius) in radians
 */
Eigen::Vector3f cartesianToSpherical(Eigen::Vector3f vectorCartesian);

/**
 * Transforms Spherical to Cartesian coordinates
 * @param vectorSpherical vector in Spherical coordinates (azimuth, elevation, radius) in radians
 * @return vector in Cartesian coordinates (x, y, z) in radians
 */
Eigen::Vector3f sphericalToCartesian(Eigen::Vector3f vectorSpherical);

Eigen::Vector3d eci_to_ecef(Eigen::Vector3d vec_eci, double gstime);

Eigen::Vector3d ecef_to_llh(Eigen::Vector3d uvw);


