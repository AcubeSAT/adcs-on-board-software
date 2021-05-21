#include "mathFunctions.h"

#include "Eigen/Geometry"
#include "Eigen/Dense"

using namespace Eigen;

Quaternionf quaternionExponent(Vector3f angularVelocity) {

    Quaternionf quaternionExponent;

    float theta = angularVelocity.norm();

    quaternionExponent.w() = cos(theta / 2.0);
    quaternionExponent.vec() = sin(theta / 2.0) * angularVelocity.normalized();

    return quaternionExponent;
}

Quaternionf quaternionProduct(const Quaternionf &quaternion1, const Quaternionf &quaternion2) {

    Quaternionf quaternionProduct;

    quaternionProduct.w() = quaternion1.w() * quaternion2.w() - quaternion1.vec().dot(quaternion2.vec());
    quaternionProduct.vec() = quaternion1.w() * quaternion2.vec() + quaternion2.w() * quaternion1.vec() +
                              quaternion1.vec().cross(quaternion2.vec());

    return quaternionProduct;
}

Vector3f rotateVector(const Eigen::Quaternionf &quaternion, const Vector3f &vector) {
    Vector3f t = 2 * vector.cross(quaternion.vec());
    Vector3f rotatedVector = vector + quaternion.w() * t + t.cross(quaternion.vec());
    return rotatedVector;
}

Matrix<float, 3, 3> skew(Vector3f vector) {
    Matrix<float, 3, 3> skewMatrix;

    skewMatrix(0, 0) = 0;
    skewMatrix(0, 1) = -vector(2);
    skewMatrix(0, 2) = vector(1);
    skewMatrix(1, 0) = vector(2);
    skewMatrix(1, 1) = 0;
    skewMatrix(1, 2) = -vector(0);
    skewMatrix(2, 0) = -vector(1);
    skewMatrix(2, 1) = vector(0);
    skewMatrix(2, 2) = 0;

    return skewMatrix;
}

Vector3f cartesianToSpherical(Vector3f vectorCartesian) {
    float x = vectorCartesian(0);
    float y = vectorCartesian(1);
    float z = vectorCartesian(2);

    float r = vectorCartesian.norm();
    float azimuthalAngle = atan(y / x);
    float polarAngle = acos(z / r);

    Vector3f vectorSpherical;
    vectorSpherical(0) = r;
    vectorSpherical(1) = azimuthalAngle;
    vectorSpherical(2) = polarAngle;

    return vectorSpherical;
}

Vector3f SphericalToCartesian(Vector3f vectorSpherical) {

    float r = vectorSpherical(0);
    float azimuthalAngle = vectorSpherical(1);
    float polarAngle = vectorSpherical(2);

    float x = r * sin(polarAngle) * cos(azimuthalAngle);
    float y = r * sin(polarAngle) * sin(azimuthalAngle);
    float z = r * cos(polarAngle);

    Vector3f vectorCartesian;
    vectorCartesian(0) = x;
    vectorCartesian(1) = y;
    vectorCartesian(2) = z;

    return vectorCartesian;
}



