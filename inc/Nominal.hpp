#pragma once

#include "EnvironmentalModel.hpp"
#include "MEKF.hpp"
#include "Parameters.hpp"


/**
 * Main function for Nominal Mode
 * @param environmentalModel Environmental Model Class's Object
 * @param mekf MEKF Class's Object
 * @param satelliteModel Object of the Class SatelliteModel that implements the space environment
 * @param gyroscopeMeasurement Gyroscope's measurements in Body frame
 * @param P Estimation error covariance Matrix
 * @param measurements Vector with the measurements of magnetic field vector and sun vector in the first frame, in our case Body
 */
void initializeNominalMode(EnvironmentalModel environmentalModel, MEKF &mekf, const SatelliteModel &satelliteModel,
                           Eigen::Vector3f gyroscopeMeasurement, Eigen::Matrix<float, LocalStateSize, LocalStateSize> P,
                           MeasurementVector measurements);