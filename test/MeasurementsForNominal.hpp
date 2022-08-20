#pragma once

#include "Eigen/Geometry"
#include "MathFunctions.hpp"

Eigen::Vector<float, 9>
MeasurementsForNominal(Eigen::Vector3f sunPositionECI, Eigen::Vector3f magneticFieldECI,
                       Eigen::Vector3f satellitePositionECI) {
    Eigen::Vector<float, 9> measurements;
    Eigen::Matrix<float, VectorSize, VectorSize> R;
    R << 0.0014, 0, 0,
            0, 0.0014, 0,
            0, 0, 0.0014;

    const Eigen::Quaternionf quaternionECItoBody = {-0.449279850250564, 0.118894667693728, -0.885360292481303,
                                                    0.0121994528668081};

    const Eigen::Vector3f magneticBody = rotateVector(quaternionECItoBody, magneticFieldECI);
    const Eigen::Vector3f randomVector = {1, 2, 3};
    Eigen::Vector3f magneticBodyWithNoise = magneticBody + R * randomVector;
    magneticBodyWithNoise.normalize();

    measurements(Eigen::seq(0, 2)) = magneticBody;

    sunPositionECI.normalize();
    Eigen::Vector3f sunPositionBody = rotateVector(quaternionECItoBody, sunPositionECI);
    sunPositionBody.normalize();
    measurements(Eigen::seq(3, 5)) = sunPositionBody;

    satellitePositionECI.normalize();
    Eigen::Vector3f satellitePositionBody = rotateVector(quaternionECItoBody, satellitePositionECI);
    satellitePositionBody.normalize();
    measurements(Eigen::seq(6, 8)) = satellitePositionBody;

    return measurements;
}