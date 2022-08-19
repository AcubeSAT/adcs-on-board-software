#pragma once

#include "Eigen/Geometry"
#include "MathFunctions.hpp"

Eigen::Vector<float, 6>
MeasurementsForNominal(Eigen::Vector3f sunPositionECI, Eigen::Vector3f satellitePositionECI, float albedo, Eigen::Vector3f magneticFieldECI) {
    MeasurementVector measurements;
    Eigen::Matrix<float, VectorSize, VectorSize> R;
    R << 0.0014, 0, 0,
            0, 0.0014, 0,
            0, 0, 0.0014;

    Eigen::Quaternionf quaternionECItoBody = {-0.449279850250564, 0.118894667693728, -0.885360292481303, 0.0121994528668081};

    sunPositionECI.normalize();
    Eigen::Vector3f sunPositionBody = rotateVector(quaternionECItoBody, sunPositionECI);
    sunPositionBody.normalize();

    satellitePositionECI.normalize();
    Eigen::Vector3f satellitePositionBody = rotateVector(quaternionECItoBody, satellitePositionECI);

    Eigen::Vector3f magneticBody = rotateVector(quaternionECItoBody, magneticFieldECI);
    Eigen::Vector3f randomVector = {1,2,3};
    magneticBody = magneticBody + R * randomVector;
    magneticBody.normalize();

    measurements(Eigen::seq(0, 2)) = magneticBody;
    measurements(Eigen::seq(3, 5)) = sunPositionBody;

    return measurements;
}