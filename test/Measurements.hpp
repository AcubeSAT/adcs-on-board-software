#pragma once

#include "Eigen/Geometry"
#include "MathFunctions.hpp"

/**
 *
 * @param sunPositionECI
 * @param magneticFieldECI
 * @param satellitePositionECI
 * @return measurements, [0: 2] magnetometer measurements, [3: 5] gyro measurements, [6: 8] sun sensor measurements
 */
Eigen::Vector<float, 9>
getMeasurements(Eigen::Vector3f sunPositionECI, Eigen::Vector3f magneticFieldECI,
                   Eigen::Vector3f satellitePositionECI) {
    Eigen::Vector<float, 9> measurements;

    const Eigen::Quaternionf quaternionECItoBody = {-0.449279850250564, 0.118894667693728, -0.885360292481303,
                                                    0.0121994528668081};

    const Eigen::DiagonalMatrix< float, VectorSize, VectorSize> R(0.001352774925847, 0.001352774925847, 0.001352774925847);
    const auto magneticFieldBody = rotateVector(quaternionECItoBody, magneticFieldECI);
    const auto magnetometerMeasurement = magneticFieldBody +  R * Eigen::MatrixXf::Random(VectorSize,1);
    measurements(Eigen::seq(0, 2)) = magnetometerMeasurement;

    return measurements;
}