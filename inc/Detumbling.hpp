#include "Bdot.hpp"
#include "Eigen/Geometry"
#include "MathFunctions.hpp"

/**
 * To detumble the AcubeSAT satellite, we need to calculate the required torque the magnetorquers should generate.
 * By using the B-dot controller to calculate the residual magnetic dipole at the current state, said torque can be
 * calculated after. Every calculation takes place in respect to the body frame.
 * @param bDot a b-dot controller to calculate the residual magnetic dipole
 * @param magneticFieldBody magnetic field value expressed in the Body Frame
 * @returns required magnetorquer torque value
 */
Eigen::Vector3f getMagneticTorque(Bdot &bDot, Eigen::Vector3f magneticFieldBody1, Eigen::Vector3f magneticFieldBody2);

/**
 * Calculate the current angular velocity of the satellite.
 * @param bDot a b-dot controller to calculate the angular velocity
 * @return angular velocity expressed in body frame
 */
Eigen::Vector3f getAngularVelocityEstimation(Bdot &bDot);