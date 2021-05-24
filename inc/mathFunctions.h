

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

Vector3f cartesianToSpherical(Vector3f vectorCartesian);

Vector3f sphericalToCartesian(Vector3f vectorSpherical);

#endif //MAIN_TEST_MATH_FUNCTIONS_H
