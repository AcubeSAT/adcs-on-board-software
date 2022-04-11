#pragma once

#include "Eigen/Geometry"
#include "Definitions.hpp"
/**
 * A function that take compressed reflectivity data and returns recovered data
 * @param compressedReflectivityData
 * @return recovered reflectivity data (ready to be used in Albedo)
 */
EarthCellsMatrix recoverData(Eigen::Matrix<int32_t , ReflectivityDataRows, ReflectivityDataColumns> compressedReflectivityData,float minimumOfTheOriginalMAtrix ,float maximumOfTheOriginalMAtrix);
