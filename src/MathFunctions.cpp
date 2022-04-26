#include "Definitions.hpp"
#include "MathFunctions.hpp"

#include "Eigen/Geometry"
#include "Eigen/Dense"

inline constexpr double const halfMeterError = 1.0e-7;

using namespace Eigen;

Quaternionf quaternionExponent(const Vector3f angularVelocity) {

    Quaternionf quaternionExponent;

    const float theta = angularVelocity.norm();

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
    const Vector3f t = 2 * vector.cross(quaternion.vec());
    const Vector3f rotatedVector = vector + quaternion.w() * t + t.cross(quaternion.vec());
    return rotatedVector;
}

Matrix<float, 3, 3> skew(const Vector3f vector) {
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

DecimalDate
date2decimal(const uint16_t year, const uint8_t month, const uint8_t day, const uint8_t hour, const uint8_t minute,
             const uint8_t second) {
    assert(month < 13 && month > 0);
    const uint8_t solarDayHours = 24;
    const uint8_t solarHourMinutes = 60;
    const uint16_t solarHourSeconds = 3600;
    const uint16_t daysOfMonths[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    const bool yearIsLeap = (((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0)));

    const uint16_t daysInYear = yearIsLeap ? 366 : 365;
    const double dayNumber = (daysOfMonths[month - 1] + (day - 1)
                        + (month > 2 ? yearIsLeap : 0));

    const double hourNumber = hour + static_cast<double>(minute) / solarHourMinutes +
                        static_cast<double>(second) / solarHourSeconds;

    const DecimalDate decimalDate = (static_cast<double>(year) + (dayNumber / daysInYear))
                              + hourNumber / solarDayHours / daysInYear;
    return decimalDate;

}

Vector3f cartesianToSpherical(const Vector3f vectorCartesian) {
    const float x = vectorCartesian(0);
    const float y = vectorCartesian(1);
    const float z = vectorCartesian(2);

    const float r = vectorCartesian.norm();
    const float azimuth = atan2(y, x);
    const float elevation = atan2(z, vectorCartesian(seq(0, 1)).norm());

    return {azimuth, elevation, r};
}

Vector3f sphericalToCartesian(const Vector3f vectorSpherical) {

    const float azimuth = vectorSpherical(0);
    const float elevation = vectorSpherical(1);
    const float r = vectorSpherical(2);

    const float x = r * cos(elevation) * cos(azimuth);
    const float y = r * cos(elevation) * sin(azimuth);
    const float z = r * sin(elevation);

    return {x, y, z};
}

Vector3f eci2ecef(const Vector3f vectorECI, const double greenwichSiderealTime) {
    Vector3f vecECEF;
    vecECEF(0) = vectorECI(0) * cos(greenwichSiderealTime) + vectorECI(1) * sin(greenwichSiderealTime);
    vecECEF(1) = -vectorECI(0) * sin(greenwichSiderealTime) + vectorECI(1) * cos(greenwichSiderealTime);
    vecECEF(2) = vectorECI(2);
    return vecECEF;
}

Vector3f ecef2llh(const Vector3f vectorInECEF) {
    const double axisOfEarth = 6378137.0;
    const double flattening = 1.0 / 298.257223563;
    const double eccentricitySquared = (2.0 - flattening) * flattening;
    Vector3f vectorInLLH;
    double latitude, radiusOfEarth, oLatitudeSave, temporaryTwo;
    double dLatitude = 1.0;
    const double temporaryOne = sqrt(pow(vectorInECEF(0), 2) + pow(vectorInECEF(1), 2));

    if (temporaryOne == 0.0) {
        vectorInLLH(1) = 0;
        if (vectorInECEF(2) > 0.0) {
            vectorInLLH(2) = vectorInECEF(2) - (axisOfEarth / sqrt(1.0 - eccentricitySquared));
            vectorInLLH[0] = asin(1.0);
        } else {
            vectorInLLH(2) = -vectorInECEF(2) - (axisOfEarth / sqrt(1.0 - eccentricitySquared));
            vectorInLLH(0) = asin(-1.0);
        }
    } else {
        vectorInLLH(1) = atan2(vectorInECEF(1), vectorInECEF(0));
        latitude = atan2(vectorInECEF(2), temporaryOne);
        radiusOfEarth = axisOfEarth;

        while (dLatitude > halfMeterError) {
            oLatitudeSave = latitude;
            temporaryTwo = vectorInECEF(2) + eccentricitySquared * radiusOfEarth * sin(latitude);
            latitude = atan2(temporaryTwo, temporaryOne);
            radiusOfEarth = axisOfEarth / sqrt(1 - eccentricitySquared * pow(sin(latitude), 2));
            dLatitude = abs(latitude - oLatitudeSave);
        }
        vectorInLLH(2) = temporaryOne / cos(latitude) - radiusOfEarth;
        vectorInLLH(0) = latitude;
    }
    return vectorInLLH;
}

Vector3f ned2ecef(const Vector3f vectorNED, const float latitude, const float longitude) {
    Matrix<float, VectorSize, VectorSize> R;
    R(0, 0) = -sin(latitude) * cos(longitude);
    R(1, 0) = -sin(latitude) * sin(longitude);
    R(2, 0) = cos(latitude);

    R(0, 1) = -sin(longitude);
    R(1, 1) = cos(longitude);
    R(2, 1) = 0;

    R(0, 2) = -cos(latitude) * cos(longitude);
    R(1, 2) = -cos(latitude) * sin(longitude);
    R(2, 2) = -sin(latitude);

    const Vector3f vectorECEF = R * vectorNED;
    return vectorECEF;
}

Vector3f ecef2eci(const Vector3f vectorECEF, const double greenwichSiderealTime) {
    Matrix<float, VectorSize, VectorSize> R;
    R(0, 0) = cos(-greenwichSiderealTime);
    R(0, 1) = sin(-greenwichSiderealTime);
    R(0, 2) = 0;

    R(1, 0) = -sin(-greenwichSiderealTime);
    R(1, 1) = cos(-greenwichSiderealTime);
    R(1, 2) = 0;

    R(2, 0) = 0;
    R(2, 1) = 0;
    R(2, 2) = 1;
    
    const Vector3f vectorECI = R * vectorECEF;
    return vectorECI;
}

Quaternionf quaternionConjugate(const Quaternionf quaternion){
    Quaternionf conQuaternion;
    conQuaternion.w() = quaternion.w();
    conQuaternion.x() = -quaternion.x();
    conQuaternion.y() = -quaternion.y();
    conQuaternion.z() = -quaternion.z();
    return conQuaternion;
}