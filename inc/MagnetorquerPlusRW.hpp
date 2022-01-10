#pragma once 

#include "PointingStrategy.hpp"

/**
 * Default profile of the AOCS nominal mode during which actuation is carried out using both the magnetorquers and the reaction wheel
 */
class MagnetorquerPlusRW : public PointingStrategy {
public:
    /**
     * Algorithm for the decomposition of the control torque into magnetic torque and reaction wheel torque
     * @param magneticField Magnetic field value expressed in the Body Frame (in T)
     * @param commandedTorque Control torque calculated by the PD controller (in Nm)
     * @return Magnetic torque and Reaction Wheel torque
     */
    Eigen::Matrix<float, VectorSize, NumOfActuators> splitTorque(Eigen::Vector3f magneticField,
                                                                    Eigen::Vector3f commandedTorque) const;


    /**
     * Magnetorquer desaturation algorithm
     * In case the induced magnetic dipole exceeds the available maximum dipole provided by the magnetorquers,
     * the magnetic and reaction wheel torques are split accordingly so that the reaction wheel torque compensates for the lack of magnetic actuation
     * @return Magnetic torque and Reaction Wheel torque
     */
    Eigen::Matrix<float, VectorSize, NumOfActuators>
    desaturateMagnetorquer(Eigen::Vector3f desiredMagneticTorque,
                           Eigen::Vector3f desiredReactionWheelTorque,
                           Eigen::Vector3f commandedTorque,
                           Eigen::Vector3f magneticField,
                           Eigen::Vector3f desiredMagneticDipole) const;

    /**
     * Reaction wheel desaturation algorithm
     * In case reaction wheel angular velocity exceeds the reaction wheel maximum rotation rate, part of the reaction wheel torque is added to the magnetic torque
     * so that the reaction wheel has margin to accelerate again.
     * @param effectiveMagneticTorque Torque to be provided by the magnetorquers (in Nm)
     * @param reactionWheelTorque Torque to be provided by the reaction wheel (in Nm)
     * @param magneticFieldBody Magnetic field value expressed in the Body Frame (in T)
     * @param reactionWheelAngularVelocity Rotation speed of the reaction wheel (in rpm)
     * @param reactionWheelAngularAcceleration Angular acceleration of the reaction wheel (in rpm/sec)
     * @return Magnetic torque and Reaction Wheel torque
     */
    Eigen::Matrix<float, VectorSize, NumOfActuators>
    desaturateReactionWheel(Eigen::Vector3f effectiveMagneticTorque,
                            Eigen::Vector3f reactionWheelTorque,
                            Eigen::Vector3f magneticFieldBody,
                            float reactionWheelAngularVelocity,
                            float reactionWheelAngularAcceleration) const;

    /**
     * Reaction wheel torque scaling algorithm
     * @param reactionWheelTorque Torque to be provided by the reaction wheel (in Nm)
     * @return Scaled Reaction Wheel torque
     */
    Eigen::Vector3f scaleReactionWheelTorque(Eigen::Vector3f reactionWheelTorque) const;

    /**
     * Algorithm to distribute the commanded torque (control torque calculated by the PD controller) between the magnetorquers and the reaction wheel
     * @return Magnetic torque and Reaction Wheel torque
     */
    Eigen::Matrix<float, VectorSize, NumOfActuators>
    actuate(Eigen::Vector3f commandedTorque, Eigen::Vector3f magneticField, bool firstTime,
            float currentReactionWheelAngularVelocity,
            float oldReactionWheelAcceleration) const override;
};
