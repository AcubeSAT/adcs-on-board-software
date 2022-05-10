#pragma once

#include "EnvironmentalModel.hpp"
#include "MEKF.hpp"

/**
 * First Part of Nominal (until MEKF set global state.
 * @param em Environmental Model Class's Object
 * @param mekf MEKF Class's Object
 * @param quaternionECIBody Quaternion from ECI frame to Body frame
 * @param magneticBody Magnetic field in Body frame
 * @param gyroscopeBias Gyroscope's Bias
 */
void FirstPartOfNominal(EnvironmentalModel em,MEKF &mekf,const SatelliteModel &satelliteModel,Eigen::Vector3f gyroscopeBias,Eigen::Matrix<float, LocalStateSize, LocalStateSize> Q,Eigen::Matrix<float, MeasurementSize, MeasurementSize> R,Eigen::Matrix<float, LocalStateSize, LocalStateSize> P,MeasurementVector measurements);