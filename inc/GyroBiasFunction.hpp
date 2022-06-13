#pragma once

#include "Eigen/Geometry"

Eigen::Vector3f calculateGyroBias(Eigen::Quaternionf wahbaOutputQuaternion1,Eigen::Quaternionf wahbaOutputQuaternion2,Eigen::Vector3f gyroscopeMeasurement);
