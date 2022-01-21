#pragma once

#include "Eigen/Dense"
#include "Definitions.hpp"

/**
 * Class that models the pointing strategy (magnetorquer only or magnetorquer and reaction wheel actuation)
 */
class PointingStrategy {
protected:
    /**
     * Magnetorquer desaturation algorithm
     * @param desiredMagneticTorque Torque to be provided by the magnetorquers (in Nm)
     * @param desiredReactionWheelTorque Torque to be provided by the reaction wheel (in Nm)
     * @param commandedTorque Control torque calculated by the PD controller (in Nm)
     * @param magneticField Magnetic field value expressed in the Body Frame (in T)
     * @param desiredMagneticDipole Magnetic dipole moment (in Am^2)
     * @return Magnetic torque and Reaction Wheel torque
     */
    Eigen::Matrix<float, VectorSize, NumOfActuators>
    desaturateMagnetorquer(Eigen::Vector3f desiredMagneticTorque,
                           Eigen::Vector3f desiredReactionWheelTorque,
                           const Eigen::Vector3f commandedTorque,
                           const Eigen::Vector3f magneticField,
                           Eigen::Vector3f desiredMagneticDipole) const;

public:
    /**
     * Magnetorquer scaling algorithm
     * @param desiredMagneticDipole Magnetic dipole moment (in Am^2)
     * @return scaled magnetic dipole moment (in Am^2)
     */
    Eigen::Vector3f scaleMagnetorquerDipole(Eigen::Vector3f desiredMagneticDipole) const;

    /**
     * Algorithm to distribute the commanded torque (control torque calculated by the PD controller) between the available actuators
     * @param commandedTorque Control torque calculated by the PD controller (in Nm)
     * @param magneticField Magnetic field value expressed in the Body Frame (in T)
     * @param firstTime True if the current control cycle is the first
     * @param currentReactionWheelAngularVelocity Rotation speed of the reaction wheel in the current step of the control cycle (in rpm)
     * @param oldReactionWheelAcceleration Angular acceleration of the reaction wheel in the previous step of the control cycle (in rpm/sec)
     * @return Magnetic torque and Reaction Wheel torque
     */
    virtual Eigen::Matrix<float, VectorSize, NumOfActuators>
    actuate(const Eigen::Vector3f commandedTorque, const Eigen::Vector3f magneticField, const bool firstTime,
            const float currentReactionWheelAngularVelocity,
            const float oldReactionWheelAcceleration) const = 0;
};