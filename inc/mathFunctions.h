

#ifndef MAIN_TEST_MATH_FUNCTIONS_H
#define MAIN_TEST_MATH_FUNCTIONS_H

#include "Eigen/Geometry"

using namespace Eigen;

/**
 * Calculates the product of two quaternions
 * @param quaternion1 quaternion that expresses a rotation
 * @param quaternion2 quaternion that expresses another rotation
 * @return quaternion equivalent to applying the two rotations in sequence
 */
Quaternionf quaternionProduct(const Quaternionf &quaternion1, const Quaternionf &quaternion2);

/**
 * Calculates the exponential map of the rotational velocity
 * @param angularVelocity rotational velocity
 * @return quaternion that expresses rotation equal to rotational velocity (angularVelocity) in seconds
 */
Quaternionf quaternionExponent(Vector3f angularVelocity);

/**
 * Rotates 3D vector by quaternion.
 * This function implements v' = q_conj * v * q
 * To implement v' = q * v * q_conj just switch the order of the cross products.
 * @param quaternion quaternion that expresses a rotation from a frame to another
 * @param vector vector to be rotated from a frame to another
 * @return rotated vector
 */
Vector3f rotateVector(const Quaternionf &quaternion, const Vector3f &vector);

/**
 * Maps a 3-dimensional vector to its corresponding skew-symmetric matrix
 * @param vector 3-dimensional vector
 * @return skew-symmetric matrix
 */
Matrix<float, 3, 3> skew(Vector3f vector);

/**
 * Convert degrees to rads
 */
inline double deg2rad(double degrees) {
    return M_PI * degrees / 180;
}

/**
 * Convert rads to degrees
 */
inline double rad2deg(double rads) {
    return 180 * rads / M_PI;
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
Vector3f cartesianToSpherical(Vector3f vectorCartesian);

/**
 * Transforms Spherical to Cartesian coordinates
 * @param vectorSpherical vector in Spherical coordinates (azimuth, elevation, radius) in radians
 * @return vector in Cartesian coordinates (x, y, z) in radians
 */
Vector3f sphericalToCartesian(Vector3f vectorSpherical);

#endif //MAIN_TEST_MATH_FUNCTIONS_H