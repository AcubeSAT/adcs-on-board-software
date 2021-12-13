#pragma once 

#include "Eigen/Dense"
#include "Definitions.hpp"

/**
 * Class that models the pointing strategy (magnetorquer only or magnetorquer and reaction wheel actuation)
 */
class PointingStrategy {
protected:

    /**
     * Maximum magnetic dipole provided by the Magnetorquers (in Am^2)
     */
    Eigen::Vector3f maxMagneticDipoleMoment;

    /**
     * Estimation of the residual magnetic dipole (in Am^2)
     */
    Eigen::Vector3f residualDipoleEstimation;

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
                           Eigen::Vector3f commandedTorque,
                           Eigen::Vector3f magneticField,
                           Eigen::Vector3f desiredMagneticDipole) const;

public:

    /**
     * @param maxMagneticDipoleMoment Maximum magnetic dipole provided by the Magnetorquers (in Am^2)
     * @param residualDipoleEstimation Estimation of the residual magnetic dipole (in Am^2)
     */
    PointingStrategy(Eigen::Vector3f maxMagneticDipoleMoment, Eigen::Vector3f residualDipoleEstimation);

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
    actuate(Eigen::Vector3f commandedTorque, Eigen::Vector3f magneticField, bool firstTime,
            float currentReactionWheelAngularVelocity,
            float oldReactionWheelAcceleration) const = 0;

    /**
     * Maximum magnetic dipole provided by the Magnetorquers setter
     * @param maxMagneticDipoleMoment Maximum magnetic dipole provided by the Magnetorquers (in Am^2)
     */
    void setMaxMagneticDipoleMoment(Eigen::Vector3f maxMagneticDipoleMoment);

    /**
     * Maximum magnetic dipole provided by the Magnetorquers getter
     * @return maximum magnetic dipole provided by the Magnetorquers (in Am^2)
     */
    Eigen::Vector3f getMaxMagneticDipoleMoment() const;

    /**
     * Estimation of the residual magnetic dipole setter
     * @param residualDipoleEstimation Estimation of the residual magnetic dipole (in Am^2)
     */
    void setResidualDipoleEstimation(Eigen::Vector3f residualDipoleEstimation);

    /**
     * Estimation of the residual magnetic dipole getter
     * @return estimation of the residual magnetic dipole (in Am^2)
     */
    Eigen::Vector3f getResidualDipoleEstimation() const;
};