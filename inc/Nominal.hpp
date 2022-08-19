#pragma once

#include "EnvironmentalModel.hpp"
#include "MEKF.hpp"
#include "Parameters.hpp"
#include "Eigen/Geometry"
#include "Definitions.hpp"

/**
* A structure that contains a reflectivity data matrix, an estimation error covariance
* matrix and an OrbitalParameters class item
*/
struct PRData {
    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData;
    Eigen::Matrix<float, LocalStateSize, LocalStateSize> pMekfErrorMatrix;
    OrbitalParameters orbitalParameters;
};

/**
 * A function that initialises parameters needed in Nominal function
 * @return reflectivityData, estimation error covariance Matrix and an OrbitalParameters class item
 */
PRData initialiseNominal();

#include "Eigen/Geometry"
/**
 * Function that takes as input the 2 quaternions and gyroscope's measurements and returns the Gyroscope bias
 * @param wahbaOutputQuaternion1
 * @param wahbaOutputQuaternion2
 * @param gyroscopeMeasurement
 * @return Gyroscope's Bias
 */
Eigen::Vector3f calculateGyroBias(Eigen::Quaternionf wahbaOutputQuaternion1,Eigen::Quaternionf wahbaOutputQuaternion2,Eigen::Vector3f gyroscopeMeasurement);


/**
 * Main function for Nominal Mode
 * @param numberOfCycles The number of Cycles, that we need to test the Nominal mode
 * @return Mekf's global state vector
 */
GlobalStateVector nominalMode(uint8_t numberOfCycles);

/**
 * Function that takes as input the 2 quaternions and gyroscope's measurements and returns the Gyroscope bias
 * @param wahbaOutputQuaternion1
 * @param wahbaOutputQuaternion2
 * @param gyroscopeMeasurement
 * @return Gyroscope's Bias
 */
Eigen::Vector3f calculateGyroBias(Eigen::Quaternionf wahbaOutputQuaternion1, Eigen::Quaternionf wahbaOutputQuaternion2,
                                  Eigen::Vector3f gyroscopeMeasurement);

/**
 * Function that initialize the wahba
 * @param environmentalModel
 * @return Mekf's global state vector
 */
GlobalStateVector wahbaInitilization(EnvironmentalModel &environmentalModel);

/**
 * Function that calculates sun Position in Body frame magnetic field in Body frame and gyroscope Measurements
 * @param sunPositionECI
 * @param satellitePositionECI
 * @param albedo
 * @param magneticFieldECI
 * @return
 */
Eigen::Vector<float,NominalMeasurementsSize> measurementsProduction(Eigen::Vector3f sunPositionECI,Eigen::Vector3f satellitePositionECI,float albedo,Eigen::Vector3f magneticFieldECI);