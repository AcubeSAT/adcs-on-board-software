#ifndef ADCS_ONBOARD_SOFTWARE_POINTINGTARGET_H
#define ADCS_ONBOARD_SOFTWARE_POINTINGTARGET_H

#include "Eigen/Dense"
#include "Definitions.h"

/**
 * Class that implements the algorithms necessary for the calculation of the torque desired in order
 * to achieve pointing (towards nadir or sun) during Nominal mode
 */
class PointingTarget {
protected:
    /**
     * Proportional positive gain
     */
    Eigen::Matrix<float, 3, 3> Kp;

    /**
     * Derivative positive gain
     */
    Eigen::Matrix<float, 3, 3> Kd;

    /**
     * Desired quaternion to achieve pointing
     */
    Eigen::Quaternionf desiredQuaternion;

    /**
     * Angular velocity of the orbit frame with respect to the ECI frame, expressed in the orbit frame
     */
    Eigen::Vector3f angularVelocityECIOrbit;

public:
    /**
     * @param Kp proportional positive gain
     * @param Kd derivative positive gain
     * @param desiredQuaternion desired quaternion to achieve pointing
     * @param angularVelocityECIOrbit angular velocity of the orbit frame with respect to the ECI frame, expressed in the orbit frame
     */
    PointingTarget(Eigen::Matrix<float, 3, 3> Kp,
                   Eigen::Matrix<float, 3, 3> Kd,
                   Eigen::Quaternionf desiredQuaternion,
                   Eigen::Vector3f angularVelocityECIOrbit);

    /**
     * Changes the gains depending on the existence of eclipse or not
     * @param eclipse existence of eclipse or not
     * @param KpGain proportional positive gain
     * @param KdGain derivative positive gain
     */
    virtual void changeGains(bool eclipse, Eigen::Matrix<float, 3, 3> &KpGain, Eigen::Matrix<float, 3, 3> &KdGain) const = 0;

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
    void setKp(Eigen::Matrix<float, 3, 3> Kp);

    /**
     * Proportional positive gain getter
     * @return proportional positive gain
     */
    Eigen::Matrix<float, 3, 3> getKp() const;

    /**
     * Derivative positive gain setter
     * @param Kd derivative positive gain
     */
    void setKd(Eigen::Matrix<float, 3, 3> Kd);

    /**
     * Derivative positive gain getter
     * @return derivative positive gain
     */
    Eigen::Matrix<float, 3, 3> getKd() const;

    /**
     * Desired quaternion setter
     * @param desiredQuaternion desired quaternion to achieve pointing
     */
    void setDesiredQuaternion(Eigen::Quaternionf desiredQuaternion);

    /**
     * Desired quaternion getter
     * @return desired quaternion to achieve pointing
     */
    Eigen::Quaternionf getDesiredQuaternion() const;

    /**
     * Angular velocity of the orbit frame with respect to the ECI frame, expressed in the orbit frame setter
     * @param angularVelocityECIOrbit angular velocity of the orbit frame with respect to the ECI frame, expressed in the orbit frame
     */
    void setAngularVelocityECIOrbit(Eigen::Vector3f angularVelocityECIOrbit);

    /**
     * Angular velocity of the orbit frame with respect to the ECI frame, expressed in the orbit frame getter
     * @return angular velocity of the orbit frame with respect to the ECI frame, expressed in the orbit frame
     */
    Eigen::Vector3f getAngularVelocityECIOrbit() const;
};

#endif //ADCS_ONBOARD_SOFTWARE_POINTINGTARGET_H
