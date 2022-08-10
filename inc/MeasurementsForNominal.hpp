#pragma once
#include "Eigen/Geometry"
#include "EnvironmentalModel.hpp"
#include "MEKF.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

Eigen::Vector<float,NominalMeasurementsSize> MeasurementsProduction(Eigen::Vector3f sunPositionECI,Eigen::Vector3f satellitePositionECI,float albedo,Eigen::Vector3f magneticFieldECI);
