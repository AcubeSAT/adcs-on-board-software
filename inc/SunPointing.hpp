#ifndef ADCS_ONBOARD_SOFTWARE_SUNPOINTING_H
#define ADCS_ONBOARD_SOFTWARE_SUNPOINTING_H

#include "PointingTarget.hpp"

/**
 * Pointing mode during which the -X and +Y sides of the AcubeSAT face the sun with a 45 degrees angle
 */
class SunPointing : public PointingTarget {
public:
    /**
     * @param Kp proportional positive gain
     * @param Kd derivative positive gain
     * @param desiredQuaternion desired quaternion to achieve sun pointing
     * @param angularVelocityECIOrbit angular velocity of the orbit frame with respect to the ECI frame, expressed in the orbit frame
     */
    SunPointing(Eigen::Matrix<float, 3, 3> Kp,
                Eigen::Matrix<float, 3, 3> Kd,
                Eigen::Quaternionf desiredQuaternion,
                Eigen::Vector3f angularVelocityECIOrbit);

    void changeGains(bool eclipse, Eigen::Matrix<float, 3, 3> &KpGain, Eigen::Matrix<float, 3, 3> &KdGain) const override;

    Eigen::Vector3f calculateTorque(Eigen::Quaternionf quaternionOrbitBody,
                                    Eigen::Vector3f sunECIUnitVector,
                                    GlobalStateVector state,
                                    bool eclipse) const override;

    /**
     * Function that calculates the quaternion that expresses the rotation from the body frame to sun
     * @param sunECIUnitVector unit vector from satellite to sun expressed in ECI frame
     * @param quaternionECIBody quaternion that expresses the rotation from the ECI frame to the body frame
     * @return quaternion that expresses the rotation from the body frame to sun
     */
    Eigen::Quaternionf calculateQuaternionSunBody(Eigen::Vector3f sunECIUnitVector,
                                         Eigen::Quaternionf quaternionECIBody) const;

};

#endif //ADCS_ONBOARD_SOFTWARE_SUNPOINTING_H
