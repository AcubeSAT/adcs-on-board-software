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

Eigen::Vector3f eci_to_ecef(Eigen::Vector3f vec_eci, double gstime) {


    double OMEGAE = 7.292115860000000e-05;
    double CGAST = cos(gstime);
    double SGAST = sin(gstime);
    Eigen::Vector3f vec_ecef;

    vec_ecef[0] = vec_eci[0] * CGAST + vec_eci[1] * SGAST;
    vec_ecef[1] = -vec_eci[0] * SGAST + vec_eci[1] * CGAST;
    vec_ecef[2] = vec_eci[2];


    return vec_ecef;
}

Eigen::Vector3f ecef_to_llh(Eigen::Vector3f uvw) {
    Eigen::Vector3f llh;
    double lat;
    double re;
    double olatsav;
    double tmp2;
    double dlat = 1.0;
    //Set up WGS-84 constants.
    //Earth model
    double a = 6378137.0; //meters
    double f = 1.0 / 298.257223563;
    //eccentricity squared for WGS84.
    double ecc_sq = (2.0 - f) * f;

    double tmp1 = sqrt(pow(uvw[0], 2) + pow(uvw[1], 2));


    if (tmp1 == 0.0) {
        llh[1] = 0;//lon
        if (uvw[2] > 0.0) {
            llh[2] = uvw[2] - (a / sqrt(1.0 - ecc_sq));//hgt
            llh[0] = asin(1.0);//lat
        } else {
            llh[2] = -uvw[2] - (a / sqrt(1.0 - ecc_sq));
            llh[0] = asin(-1.0);
        }
    } else {
        llh[1] = atan2(uvw[1], uvw[0]);
        lat = atan2(uvw[2], tmp1);
        re = a;


        while (dlat > 1.0e-7) {
            olatsav = lat;
            tmp2 = uvw[2] + ecc_sq * re * sin(lat);
            lat = atan2(tmp2, tmp1);
            re = a / sqrt(1 - ecc_sq * pow(sin(lat), 2));
            dlat = abs(lat - olatsav);
        }
        llh[2] = tmp1 / cos(lat) - re;
        llh[0] = lat;
    }
    return llh;

}