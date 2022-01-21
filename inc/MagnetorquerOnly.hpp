#pragma once

#include "PointingStrategy.hpp"

/**
 * Profile of the AOCS nominal mode during which actuation is performed only by three axes magnetorquer
 */
class MagnetorquerOnly : public PointingStrategy {
public:
    /**
     * Magnetorquer desaturation algorithm
     * In case the induced magnetic dipole exceeds the available maximum dipole provided by the magnetorquers,
     * the dipole is scaled accordingly
     * @return Magnetic torque and Reaction Wheel torque (equal to zero in this case)
     */
    Eigen::Matrix<float, VectorSize, NumOfActuators>
    desaturateMagnetorquer(Eigen::Vector3f desiredMagneticTorque,
                           Eigen::Vector3f desiredReactionWheelTorque,
                           [[maybe_unused]] const Eigen::Vector3f commandedTorque,
                           const Eigen::Vector3f magneticField,
                           Eigen::Vector3f desiredMagneticDipole) const;

    /**
     * Algorithm to calculate the torque to be provided by the magnetorquers given the control torque
     * @return Magnetic torque and Reaction Wheel torque (equal to zero in this case)
     */
    Eigen::Matrix<float, VectorSize, NumOfActuators>
    actuate(const Eigen::Vector3f commandedTorque, const Eigen::Vector3f magneticField,
            [[maybe_unused]] const bool firstTime,
            [[maybe_unused]] const float currentReactionWheelAngularVelocity,
            [[maybe_unused]] const float oldReactionWheelAcceleration) const override;
};
