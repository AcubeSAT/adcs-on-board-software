#pragma once

#include "Bdot.hpp"
#include "Eigen/Geometry"
#include "MathFunctions.hpp"

/**
 * To detumble the AcubeSAT nanosatellite, we need to calculate the required torque the magnetorquers should generate.
 * By using the B-dot controller to calculate the residual magnetic dipole at the current state, said torque can be
 * calculated after. Every calculation takes place in respect to the body frame.
 * @param bDot a B-dot controller to calculate the residual magnetic dipole
 * @param magneticFieldBody magnetic field value expressed in the body frame
 * @returns required magnetorquer torque value
 */
Eigen::Vector3f calculateMagneticTorque(Bdot &bDot, Eigen::Vector3f magneticFieldMeasurement1,
                                        Eigen::Vector3f magneticFieldMeasurement2);

/**
 * Estimate the current angular velocity of the satellite.
 * @param bDot a B-dot controller to calculate the angular velocity
 * @return angular velocity expressed in body frame
 */
Eigen::Vector3f estimateAngularVelocityEstimation(Bdot &bDot);