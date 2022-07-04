#pragma once
#include "Eigen/Geometry"
#include "EnvironmentalModel.hpp"
#include "MEKF.hpp"
#include "Parameters.hpp"

Eigen::Vector<float,9> MeasurmentsProduction(Eigen::Vector3f sunPositionECI,Eigen::Vector3f satellitePositionECI,float albedo,Eigen::Vector3f magneticFieldECI);
