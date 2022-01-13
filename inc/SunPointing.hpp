#pragma once 

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
    SunPointing(const Eigen::Matrix<float, 3, 3> Kp,
                const Eigen::Matrix<float, 3, 3> Kd);

    void changeGains(const bool eclipse, Eigen::Matrix<float, 3, 3> &KpGain, Eigen::Matrix<float, 3, 3> &KdGain) const override;

    Eigen::Vector3f calculateTorque(const Eigen::Quaternionf quaternionOrbitBody,
                                    const Eigen::Vector3f sunECIUnitVector,
                                    const GlobalStateVector state,
                                    const bool eclipse) const override;

    /**
     * Function that calculates the quaternion that expresses the rotation from the body frame to sun
     * @param sunECIUnitVector unit vector from satellite to sun expressed in ECI frame
     * @param quaternionECIBody quaternion that expresses the rotation from the ECI frame to the body frame
     * @return quaternion that expresses the rotation from the body frame to sun
     */
    Eigen::Quaternionf calculateQuaternionSunBody(Eigen::Vector3f sunECIUnitVector,
                                         const Eigen::Quaternionf quaternionECIBody) const;

};