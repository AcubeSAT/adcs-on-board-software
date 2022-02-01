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

Eigen::Vector3f eci2ecef(Eigen::Vector3f vecECI, double gstime) {

    double CGAST = cos(gstime);
    double SGAST = sin(gstime);
    Vector3f vecECEF;

    vecECEF[0] = (vecECI[0] * CGAST + vecECI[1] * SGAST) * 1000;
    vecECEF[1] = (-vecECI[0] * SGAST + vecECI[1] * CGAST) * 1000;
    vecECEF[2] = vecECI[2] * 1000;


    return vecECEF;
}

Eigen::Vector3f ecef2llh(Eigen::Vector3f uvw) {
    Vector3f llh;
    double lat;
    double re;
    double olatsav;
    double tmp2;
    double dlat = 1.0;
    double a = 6378137.0;
    double f = 1.0 / 298.257223563;
    double eccSq = (2.0 - f) * f;
    double tmp1 = sqrt(pow(uvw[0], 2) + pow(uvw[1], 2));

    if (tmp1 == 0.0) {
        llh[1] = 0;//lon
        if (uvw[2] > 0.0) {
            llh[2] = uvw[2] - (a / sqrt(1.0 - eccSq));//hgt
            llh[0] = asin(1.0);//lat
        } else {
            llh[2] = -uvw[2] - (a / sqrt(1.0 - eccSq));
            llh[0] = asin(-1.0);
        }
    } else {
        llh[1] = atan2(uvw[1], uvw[0]);
        lat = atan2(uvw[2], tmp1);
        re = a;


        while (dlat > 1.0e-7) {
            olatsav = lat;
            tmp2 = uvw[2] + eccSq * re * sin(lat);
            lat = atan2(tmp2, tmp1);
            re = a / sqrt(1 - eccSq * pow(sin(lat), 2));
            dlat = abs(lat - olatsav);
        }
        llh[2] = tmp1 / cos(lat) - re;
        llh[0] = lat;
    }
    return llh;

}

Eigen::Vector3f ned2ecef(Eigen::Vector3f vectorNED, float latitude, float longitude) {
    Eigen::Vector3f vectorECEF;
    Eigen::Matrix<float, 3, 3> R;
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

Eigen::Vector3f ecef2eci(Eigen::Vector3f vectorECEF, double gstime) {
    Eigen::Vector3f vectorECI;
    Eigen::Matrix<float, 3, 3> R;
    double CGAST = cos(-gstime);
    double SGAST = sin(-gstime);
    R(0, 0) = CGAST;
    R(0, 1) = SGAST;
    R(0, 2) = 0;
    R(1, 0) = -SGAST;
    R(1, 1) = CGAST;
    R(1, 2) = 0;
    R(2, 0) = 0;
    R(2, 1) = 0;
    R(2, 2) = 1;
    vectorECI = R * vectorECEF;
    return vectorECI;
}