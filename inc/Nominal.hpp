#pragma once

#include "EnvironmentalModel.hpp"
#include "MEKF.hpp"
#include "Parameters.hpp"
#include "Eigen/Geometry"

 /**
  * Main function for Nominal Mode
  * @param numberOfCycles The number of Cycles, that we need to test the Nominal mode
  * @return Mekf's global state vector
  */
GlobalStateVector NominalMode(int numberOfCycles);

/**
 * Function that takes as input the 2 quaternions and gyroscope's measurements and returns the Gyroscope bias
 * @param wahbaOutputQuaternion1
 * @param wahbaOutputQuaternion2
 * @param gyroscopeMeasurement
 * @return Gyroscope's Bias
 */
Eigen::Vector3f calculateGyroBias(Eigen::Quaternionf wahbaOutputQuaternion1,Eigen::Quaternionf wahbaOutputQuaternion2,Eigen::Vector3f gyroscopeMeasurement);

/**
 * Function that initialize the wahba
 * @param environmentalModel
 * @return Mekf's global state vector
 */
GlobalStateVector WahbaInitilization(EnvironmentalModel &environmentalModel);