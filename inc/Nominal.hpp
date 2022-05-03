#pragma once

#include "EnvironmentalModel.hpp"
#include "CSSCompensation.hpp"
#include "MEKF.hpp"

/**
 * First Part of Nominal (until MEKF set global state.
 * @param em Environmental Model Class's Object
 * @param mekf MEKF Class's Object
 * @param quaternionECIBody Quaternion from ECI frame to Body frame
 * @param magneticBody Magnetic field in Body frame
 * @param gyroscopeBias Gyroscope's Bias
 */
void FirstPartOfNominal(EnvironmentalModel em,MEKF &mekf,const Eigen::Quaternionf quaternionECIBody,Eigen::Vector3f magneticBody,Eigen::Vector3f gyroscopeBias);