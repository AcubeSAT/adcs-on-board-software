#pragma once

#include "Eigen/Dense"
#include "Definitions.hpp"

/**
 * Class that implements the algorithms necessary for the calculation of the torque desired in order
 * to achieve pointing (towards nadir or sun) during Nominal mode
 */
class PointingTarget {
protected:
    /**
     * Proportional positive gain
     */
    Eigen::Matrix<float, VectorSize, VectorSize> Kp;

    /**
     * Derivative positive gain
     */
    Eigen::Matrix<float, VectorSize, VectorSize> Kd;

public:
    /**
     * @param Kp proportional positive gain
     * @param Kd derivative positive gain
     */
    PointingTarget(Eigen::Matrix<float, VectorSize, VectorSize> Kp,
                   Eigen::Matrix<float, VectorSize, VectorSize> Kd);

    /**
     * Changes the gains depending on the existence of eclipse or not
     * @param eclipse existence of eclipse or not
     * @param KpGain proportional positive gain
     * @param KdGain derivative positive gain
     */
    virtual void changeGains(bool eclipse, Eigen::Matrix<float, VectorSize, VectorSize> &KpGain, Eigen::Matrix<float, VectorSize, VectorSize> &KdGain) const = 0;

    /**
     * PD controller that calculates the desired torque to be applied in order to achieve pointing
     * @param quaternionOrbitBody quaternion that expresses the rotation from the orbit frame to the body frame
     * @param sunECIUnitVector unit vector from satellite to sun expressed in ECI frame
     * @param state 7-element vector consisting of the quaternion representation of the satellite's orientation and
     * angular velocity of the ECI frame with respect to the body frame, expressed in the body frame
     * @param eclipse existence or not of eclipse
     * @return Control torque calculated by the PD controller (in Nm)
     */
    virtual Eigen::Vector3f calculateTorque(Eigen::Quaternionf quaternionOrbitBody,
                                            Eigen::Vector3f sunECIUnitVector,
                                            GlobalStateVector state,
                                            bool eclipse) const = 0;

    /**
     * Proportional positive gain setter
     * @param Kp proportional positive gain
     */
    void setKp(Eigen::Matrix<float, VectorSize, VectorSize> Kp);

    /**
     * Proportional positive gain getter
     * @return proportional positive gain
     */
    Eigen::Matrix<float, VectorSize, VectorSize> getKp() const;

    /**
     * Derivative positive gain setter
     * @param Kd derivative positive gain
     */
    void setKd(Eigen::Matrix<float, VectorSize, VectorSize> Kd);

    /**
     * Derivative positive gain getter
     * @return derivative positive gain
     */
    Eigen::Matrix<float, VectorSize, VectorSize> getKd() const;
};