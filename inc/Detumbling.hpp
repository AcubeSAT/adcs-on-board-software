#pragma once

#include "Bdot.hpp"
#include "Eigen/Geometry"
#include "MathFunctions.hpp"
#include "EnvironmentalModel.hpp"
#include "Parameters.hpp"

/**
 * The purpose of the AOCS detumbling mode is to autonomously dissipate and control
 * the satellite's angular velocity. This is achieved with a B-dot controller directly using
 * measurements from the high precision 3-axis magnetometer. In case of magnetometer
 * failure, the redundant 3-axis magnetometer unit integrated in the iMTQ board is used.
 * Actuation is performed solely by the magnetorquers. No satellite orbit propagation,
 * initialized by a TLE file, is performed during this mode. In order to perform the actuation, it is necessary to
 * calculate the desired magnetic dipole from the B-dot controller and then calculate the required torque to detumble
 * the AcubeSAT nanosatellite.
 * @param bDot the B-dot controller used for detumbling
 * @param magneticFieldBody1 the first magnetic field measurement required for bdot vector calculation measured in uT
 * @param magneticFieldBody2 the second magnetic field measurement required for bdot vector calculation measured in uT
 */
void Detumbling(Bdot &bDot, const Eigen::Vector3f magneticFieldBody1,
                const Eigen::Vector3f magneticFieldBody2);

/**
 * Calculate the desired magnetic pole using a B-dot controller.
 * @param bDot a B-dot controller to calculate the desired magnetic dipole
 * @returns desired torque value for magnetorquer actuation
 */
Eigen::Vector3f calculateDesiredMagneticTorque(Bdot &bDot);

/**
 * Estimate the current angular velocity of the satellite.
 * @param bDotVector the calculated magnetic field derivative vector of the Bdot controller in a single control cycled
 * @param magneticFieldBody the first magnetic field measurement of B-dot measured during sampling interval in a control cycle
 * @return angular velocity estimation expressed in body frame
 */
Eigen::Vector3f estimateAngularVelocity(const Eigen::Vector3f bDotVector, const Eigen::Vector3f magneticFieldBody);

/**
 * Update the magnetic field measurements used by the B-dot controller. These measurements are updated every control cycle.
 * @param bDot the B-dot controller to be updated.
 * @param magneticFieldBody1 the first magnetic field sample used by the B-dot controller
 * @param magneticFieldBody2 the second magnetic field sample used by the B-dot controller
 */
void updateMagneticFieldMeasurements(Bdot &bDot, const Eigen::Vector3f magneticFieldBody1,
                                     const Eigen::Vector3f magneticFieldBody2);
