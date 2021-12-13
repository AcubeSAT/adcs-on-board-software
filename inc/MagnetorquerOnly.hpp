#pragma once 

#include "PointingStrategy.hpp"

/**
 * Profile of the AOCS nominal mode during which actuation is performed only by three axes magnetorquer
 */
class MagnetorquerOnly : public PointingStrategy {
public:

    /**
     * @param maxMagneticDipoleMoment Maximum magnetic dipole provided by the Magnetorquers (in Am^2)
     * @param residualDipoleEstimation Estimation of the residual magnetic dipole (in Am^2)
     */
    MagnetorquerOnly(Eigen::Vector3f maxMagneticDipoleMoment, Eigen::Vector3f residualDipoleEstimation);

    /**
     * Magnetorquer desaturation algorithm
     * In case the induced magnetic dipole exceeds the available maximum dipole provided by the magnetorquers,
     * the dipole is scaled accordingly
     * @return Magnetic torque and Reaction Wheel torque (equal to zero in this case)
     */
    Eigen::Matrix<float, VectorSize, NumOfActuators>
    desaturateMagnetorquer(Eigen::Vector3f desiredMagneticTorque,
                           Eigen::Vector3f desiredReactionWheelTorque,
                           Eigen::Vector3f commandedTorque,
                           Eigen::Vector3f magneticField,
                           Eigen::Vector3f desiredMagneticDipole) const;

    /**
     * Algorithm to calculate the torque to be provided by the magnetorquers given the control torque
     * @return Magnetic torque and Reaction Wheel torque (equal to zero in this case)
     */
    Eigen::Matrix<float, VectorSize, NumOfActuators>
    actuate(Eigen::Vector3f commandedTorque, Eigen::Vector3f magneticField, bool firstTime,
            float currentReactionWheelAngularVelocity,
            float oldReactionWheelAcceleration) const override;
};
