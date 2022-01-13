#pragma once 

#include "PointingTarget.hpp"

/**
 * Pointing mode during which the X axis (in the Body Frame) is pointing towards the Earth
 */
class NadirPointing : public PointingTarget {
public:
    /**
     * @param Kp proportional positive gain
     * @param Kd derivative positive gain
     * @param desiredQuaternion desired quaternion to achieve nadir pointing
     * @param angularVelocityECIOrbit angular velocity of the orbit frame with respect to the ECI frame, expressed in the orbit frame
     */
    NadirPointing(const Eigen::Matrix<float, 3, 3> Kp,
                  const Eigen::Matrix<float, 3, 3> Kd);

    void changeGains(const bool eclipse, Eigen::Matrix<float, 3, 3> &KpGain, Eigen::Matrix<float, 3, 3> &KdGain) const override;

    Eigen::Vector3f calculateTorque(const Eigen::Quaternionf quaternionOrbitBody,
                                    const Eigen::Vector3f sunECIUnitVector,
                                    const GlobalStateVector state,
                                    const bool eclipse) const override;
};
