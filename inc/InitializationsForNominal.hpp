#pragma once

#include "EnvironmentalModel.hpp"
#include "Parameters.hpp"
/**
 * A structure that contains a reflectivity data matrix, an estimation error covariance
 * matrix and an OrbitalParameters class item
 */
struct PAndReflectivityData {
    Eigen::Matrix<float, 180, 288> reflectivityData;
    Eigen::Matrix<float, LocalStateSize, LocalStateSize> p;
    OrbitalParameters orbitalParameters;
};
/**
 * A function that initialises parameters needed in Nominal function
 * @return reflectivityData, estimation error covariance Matrix and an OrbitalParameters class item
 */
PAndReflectivityData initialiseNominal();