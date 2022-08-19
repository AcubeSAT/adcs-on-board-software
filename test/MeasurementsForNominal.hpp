#pragma once

#include "Eigen/Geometry"
#include "MathFunctions.hpp"

Eigen::Vector<float, 6>
MeasurementsForNominal(Eigen::Vector3f sunPositionECI, Eigen::Vector3f satellitePositionECI, float albedo, Eigen::Vector3f magneticFieldECI) {
    Eigen::Vector<float, 6> measurements;
    Eigen::Matrix<float, 3, 3> R;
    R << 0.0014, 0, 0,
            0, 0.0014, 0,
            0, 0, 0.0014;

    Eigen::Quaternionf quaternionSunECI = {0, sunPositionECI[0], sunPositionECI[1], sunPositionECI[2]};
    Eigen::Quaternionf quaternionECItoBody = {-0.449279850250564, 0.118894667693728, -0.885360292481303, 0.0121994528668081};
    Eigen::Quaternionf quaternionECItoBodyConjugate = {-0.449279850250564, -0.118894667693728, 0.885360292481303,
                                                -0.0121994528668081};

    sunPositionECI.normalize();
    satellitePositionECI.normalize();

    Eigen::Quaternionf temp = quaternionProduct(quaternionECItoBodyConjugate,
                                         quaternionProduct(quaternionSunECI, quaternionECItoBody));
    Eigen::Vector3f sunPositionBody  = temp.vec();
    sunPositionBody.normalize();

    Eigen::Quaternionf quaternionSatelliteECI;
    quaternionSatelliteECI.w() = 0;
    quaternionSatelliteECI.vec() = satellitePositionECI;

    Eigen::Quaternionf temp2 = quaternionProduct(quaternionECItoBodyConjugate,
                                          quaternionProduct(quaternionSatelliteECI, quaternionECItoBody));
    Eigen::Vector3f satellitePositionBody = temp.vec();

    Eigen::Vector3f magneticBody = rotateVector(quaternionECItoBody, magneticFieldECI);
    Eigen::Vector3f randomVector = {1,2,3};
    magneticBody = magneticBody + R * randomVector;
    magneticBody.normalize();

    measurements(Eigen::seq(0, 2)) = magneticBody;
    measurements(Eigen::seq(3, 5)) = sunPositionBody;

    return measurements;
}