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
void FirstPartOfNominal(EnvironmentalModel em,MEKF &mekf,Eigen::Vector3f sunPositionBody,Eigen::Vector3f magneticBody,Eigen::Vector3f gyroscopeBias);