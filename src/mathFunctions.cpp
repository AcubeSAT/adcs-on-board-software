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

DecimalDate date2decimal(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    assert(month < 13 && month > 0);
    const uint8_t solarDayHours = 24;
    const uint8_t solarHourMinutes = 60;
    const uint16_t solarHourSeconds = 3600;
    const uint16_t daysOfMonths[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    bool yearIsLeap = (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0)));

    uint16_t daysInYear = yearIsLeap ? 366 : 365;
    double dayNumber = (daysOfMonths[month - 1] + (day - 1)
                        + (month > 2 ? yearIsLeap : 0));

    double hourNumber = hour + static_cast<double>(minute) / solarHourMinutes +
                        static_cast<double>(second) / solarHourSeconds;

    DecimalDate decimalDate = (static_cast<double>(year) + (dayNumber / daysInYear))
                              + hourNumber / solarDayHours / daysInYear;
    return decimalDate;

}