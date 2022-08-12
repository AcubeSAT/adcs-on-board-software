#pragma once

#include "EnvironmentalModel.hpp"
#include "Parameters.hpp"
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