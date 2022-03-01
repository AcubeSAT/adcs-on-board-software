#include "Definitions.hpp"
#include "MathFunctions.hpp"

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

Vector3f rotateVector(const Quaternionf &quaternion, const Vector3f &vector) {
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

Vector3f cartesianToSpherical(Vector3f vectorCartesian) {
    float x = vectorCartesian(0);
    float y = vectorCartesian(1);
    float z = vectorCartesian(2);

    float r = vectorCartesian.norm();
    float azimuth = atan2(y, x);
    float elevation = atan2(z, vectorCartesian(seq(0, 1)).norm());

    return {azimuth, elevation, r};
}

Vector3f sphericalToCartesian(Vector3f vectorSpherical) {

    float azimuth = vectorSpherical(0);
    float elevation = vectorSpherical(1);
    float r = vectorSpherical(2);

    float x = r * cos(elevation) * cos(azimuth);
    float y = r * cos(elevation) * sin(azimuth);
    float z = r * sin(elevation);

    return {x, y, z};
}

Vector3f eci2ecef(Vector3f vecECI, double gstime) {
    Vector3f vecECEF;
    vecECEF(0) = vecECI(0) * cos(gstime) + vecECI(1) * sin(gstime);
    vecECEF(1) = -vecECI(0) * sin(gstime) + vecECI(1) * cos(gstime);
    vecECEF(2) = vecECI(2);
    return vecECEF;
}

Vector3f ecef2llh(Vector3f vectorInECEF) {
    Vector3f vectorInLLH;
    double latitude;
    double radiusOfEarth;
    double oLatitudeSave;
    double temporaryTwo;
    double dLatitude = 1.0;
    double temporaryOne = sqrt(pow(vectorInECEF(0), 2) + pow(vectorInECEF(1), 2));
    double halfMeterError = 1.0e-7;

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

Vector3f ned2ecef(Vector3f vectorNED, float latitude, float longitude) {
    Vector3f vectorECEF;
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
    vectorECEF = R * vectorNED;
    return vectorECEF;
}

Vector3f ecef2eci(Vector3f vectorECEF, double gstime) {
    Vector3f vectorECI;
    Matrix<float, VectorSize, VectorSize> R;
    R(0, 0) = cos(-gstime);
    R(0, 1) = sin(-gstime);
    R(0, 2) = 0;
    R(1, 0) = -sin(-gstime);
    R(1, 1) = cos(-gstime);
    R(1, 2) = 0;
    R(2, 0) = 0;
    R(2, 1) = 0;
    R(2, 2) = 1;
    vectorECI = R * vectorECEF;
    return vectorECI;
}