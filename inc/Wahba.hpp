#pragma once 

#include "Eigen/Geometry"
#include "SatelliteModel.hpp"

/**
 * Function that solves Wahba's problem with the use of the SVD Algorithm. The two vectors are the magnetic field and
 * the sun vector, while the two frames are Body and ECI.
 * @param magneticBody the magnetic field vector in Body Frame
 * @param magneticECI the magnetic field vector in ECI - Reference Frame
 * @param sunBody the sun vector in Body Frame
 * @param sunECI the sun vector in ECI - Reference Frame
 * @return quaternion from Body frame to ECI
 */
Eigen::Quaternionf
wahba(Eigen::Vector3f magneticBody, Eigen::Vector3f magneticECI, Eigen::Vector3f sunBody, Eigen::Vector3f sunECI);
