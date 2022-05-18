#pragma once

#include "Bdot.hpp"
#include "Eigen/Geometry"
#include "MathFunctions.hpp"
#include "EnvironmentalModel.hpp"

/**
The purpose of the AOCS detumbling mode is to autonomously dissipate and control
the satellite's angular velocity. This is achieved with a B-dot controller directly using
measurements from the high precision 3-axis magnetometer. In case of magnetometer
failure, the redundant 3-axis magnetometer unit integrated in the iMTQ board is used.
Actuation is performed solely by the magnetorquers. No satellite orbit propagation,
initialized by a TLE file, is performed during this mode. In order to perform the actuation, it is necessary to
calculate the desired magnetic dipole from the B-dot controller and then calculate the required torque to detumble
the AcubeSAT nanosatellite.
*/


/**
 * @param bDot a B-dot controller to calculate the desired magnetic dipole
 * @returns desired torque value for magnetorquer actuation
 */
Eigen::Vector3f calculateDesiredMagneticTorque(Bdot &bDot);

/**
 * Estimate the current angular velocity of the satellite.
 * @param bDot a B-dot controller to calculate the angular velocity
 * @param magneticFieldBody the first magnetic field measurement of B-dot sampled during sampling interval in a control cycle
 * @return angular velocity estimation expressed in body frame
 */
Eigen::Vector3f estimateAngularVelocity(const Eigen::Vector3f bDotVector, const Eigen::Vector3f magneticFieldBody);
