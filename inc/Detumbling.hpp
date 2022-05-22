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
 * @param bDot a B-dot controller to calculate the desired magnetic dipole
 * @param ECIToBodyQuaternion the quaternion between ECI and Body frame
 * @param currentAngularVelocity the current angular velocity of the satellite
 * @param environmentalModel a model of the environment around the AcubeSAT
 * @param isDone a parameter that indicates if detumbling is done or not
 */

void Detumbling(Bdot &bDot, Eigen::Quaternionf &ECIToBodyQuaternion, Eigen::Vector3f &currentAngularVelocity,
                EnvironmentalModel &environmentalModel, bool &isDone);

/**
 * Calculate the desired magnetic pole using a bdot controller
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
 * Update the coefficients of a non-constant quaternion, in this case the ECI frame to Body frame quaternion
 * with respect to the quaternion kinematic model.
 * @param quaternion the quaternion to be updated
 * @param angularVelocity the angular velocity of a coordinate system (frame), in this case the angular velocity of the
 * AcubeSAT nanosatellite expressed in the body frame
 * @param timeInterval the duration of the update
 */
void updateQuaternion(Eigen::Quaternionf &quaternion, const Eigen::Vector3f angularVelocity, const float timeInterval);

/**
 * Actuate by using the magnetorquers to apply the desired torque calculated from previous steps, and reduce the angular
 * velocity of the AcubeSAT nanosatellite.
 * @param angularVelocity the current angular velocity of the satellite
 * @param quaternion
 * @param desiredMagneticTorque
 * @param timestep
 * @param actuationTime
 */
void
actuate(Eigen::Vector3f &angularVelocity, Eigen::Quaternionf &quaternion, const Eigen::Vector3f desiredMagneticTorque,
        const float timestep, const float actuationTime);
