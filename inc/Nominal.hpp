#pragma once

#include "Nominal.hpp"
#include "Definitions.hpp"
#include "Eigen/Geometry"
#include "EnvironmentalModel.hpp"

Eigen::Quaternionf calculateQuaternionOrbitBody(GlobalStateVector globalState, double ascendingNode, double inclination,
                                         double argumentPerigeeMeanAnomaly);

Eigen::Vector3f estimateInitialGyroBias(std::array<Eigen::Quaternionf, NumberOfWahbaLoops> wahbaOutputQuaternions,
                                        Eigen::Vector3f gyroscopeMeasurement);

GlobalStateVector calculateInitialStateEstimation(EnvironmentalModel environmentalModel);

GlobalStateVector NominalMode(int numberOfCycles);