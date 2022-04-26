#pragma once

#include "EnvironmentalModel.hpp"
#include "CSSCompensation.hpp"

void FirstPartOfNominal(const TLE &tle, EarthCellsMatrix reflectivityData,const Eigen::Quaternionf quaternionECIBody,Eigen::Vector3f magneticBody);