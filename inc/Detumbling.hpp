#pragma once

#include "Bdot.hpp"
#include "Eigen/Geometry"
#include "MathFunctions.hpp"
#include "EnvironmentalModel.hpp"

/**
 * To detumble the AcubeSAT nanosatellite, there is need to calculate the required torque the magnetorquers should generate
 * and trigger to detumbling-to-nominal trigger if the angular velocity estimation is below 0.035 rad/s.
 * By using the B-dot controller to calculate the magnetic dipole at the current state, said torque can be
 * calculated after. Every calculation takes place in respect to the body frame.
 */

/**
 * @param bDot a B-dot controller to calculate the residual magnetic dipole
 * @returns desired torque value for magnetorquer actuation
 */
Eigen::Vector3f calculateDesiredMagneticTorque(Bdot &bDot);

/**
 * Estimate the current angular velocity of the satellite.
 * @param bDot a B-dot controller to calculate the angular velocity
 * @return angular velocity estimation expressed in body frame
 */
Eigen::Vector3f estimateAngularVelocity(const Eigen::Vector3f bDotVector, const Eigen::Vector3f magneticFieldBody);
