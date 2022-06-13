#pragma once

#include "EnvironmentalModel.hpp"
#include "MEKF.hpp"
#include "Parameters.hpp"


/**
 * Main function for Nominal Mode
 * @param environmentalModel Environmental Model Class's Object
 * @param mekf MEKF Class's Object
 * @param gyroscopeMeasurement Gyroscope's measurements in Body frame
 * @param P Estimation error covariance Matrix
 * @param measurements Vector with the measurements of magnetic field vector and sun vector in the first frame, in our case Body
 */
void NominalMode(EnvironmentalModel environmentalModel, MEKF &mekf,
                           Eigen::Vector3f gyroscopeMeasurement, Eigen::Matrix<float, LocalStateSize, LocalStateSize> P,
                           MeasurementVector measurements);