#pragma once

#include "Eigen/Geometry"
/**
 * Function that takes as input the 2 quaternions and gyroscope's measurements and returns the Gyroscope bias
 * @param wahbaOutputQuaternion1
 * @param wahbaOutputQuaternion2
 * @param gyroscopeMeasurement
 * @return Gyroscope's Bias
 */
Eigen::Vector3f calculateGyroBias(Eigen::Quaternionf wahbaOutputQuaternion1,Eigen::Quaternionf wahbaOutputQuaternion2,Eigen::Vector3f gyroscopeMeasurement);
