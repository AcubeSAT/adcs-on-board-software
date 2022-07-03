#include "MeasurmentsForNominal.hpp"
#include "MathFunctions.hpp"
#include "CSSCompensation.hpp"

using namespace Eigen;

Vector<float, 9>
MeasurmentsProduction(Vector3f sunPositionECI, Vector3f satellitePositionECI, float albedo, Vector3f magneticFieldECI) {
    Vector<float, 9> measurements;
    Matrix<float, 3, 3> R;
    R << 0.0014, 0, 0,
            0, 0.0014, 0,
            0, 0, 0.0014;

    Vector<float, 7> xZero(-0.449279850250564, 0.118894667693728, -0.885360292481303, 0.0121994528668081, 0, 0, 0);
    //SunBody
    Quaternionf quaternionSunECI = {0, sunPositionECI[0], sunPositionECI[1], sunPositionECI[2]};
    Quaternionf quaternionECItoBody = {-0.449279850250564, 0.118894667693728, -0.885360292481303, 0.0121994528668081};
    Quaternionf quaternionECItoBodyConjugate = {-0.449279850250564, -0.118894667693728, 0.885360292481303,
                                                -0.0121994528668081};
    for (int i = 0; i < 3; i++) {
        sunPositionECI[i] = sunPositionECI[i] / sunPositionECI.norm();
    }
    Quaternionf temp = quaternionProduct(quaternionECItoBodyConjugate,
                                         quaternionProduct(quaternionSunECI, quaternionECItoBody));
    Vector3f sunPositionBody;
    sunPositionBody[0] = temp.x();
    sunPositionBody[1] = temp.y();
    sunPositionBody[2] = temp.z();
    for (int i = 0; i < 3; i++) {
        satellitePositionECI[i] = satellitePositionECI[i] / satellitePositionECI.norm();
    }
    Quaternionf quaternionSatelliteECI = {0, satellitePositionECI[0], satellitePositionECI[1], satellitePositionECI[2]};
    Quaternionf temp2 = quaternionProduct(quaternionECItoBodyConjugate,
                                          quaternionProduct(quaternionSatelliteECI, quaternionECItoBody));
    Vector3f satellitePositionBody;
    satellitePositionBody[0] = temp.x();
    satellitePositionBody[1] = temp.y();
    satellitePositionBody[2] = temp.z();

    Vector3f totalSun = cssCompensation(sunPositionBody, quaternionECItoBody, satellitePositionECI, albedo);

    Vector3f magneticBody = rotateVector(quaternionECItoBody, magneticFieldECI);
    magneticBody = magneticBody + R * magneticBody;
    for (int i = 0; i < 3; i++) {
        magneticBody[i] = magneticBody[i] / magneticBody.norm();
    }

    Vector3f gyroscopeMeasurements = {0, 0, 0};

    measurements = {totalSun[0], totalSun[1], totalSun[2], magneticBody[0], magneticBody[1], magneticBody[2],
                    gyroscopeMeasurements[0], gyroscopeMeasurements[1], gyroscopeMeasurements[2]};

    return measurements;
}
