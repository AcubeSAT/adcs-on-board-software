#ifndef ADCS_ONBOARD_SOFTWARE_NADIRPOINTING_H
#define ADCS_ONBOARD_SOFTWARE_NADIRPOINTING_H

#include "PointingTarget.h"

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
    NadirPointing(Eigen::Matrix<float, 3, 3> Kp,
                  Eigen::Matrix<float, 3, 3> Kd,
                  Eigen::Quaternionf desiredQuaternion,
                  Eigen::Vector3f angularVelocityECIOrbit);

    void changeGains(bool eclipse, Eigen::Matrix<float, 3, 3> &KpGain, Eigen::Matrix<float, 3, 3> &KdGain) const override;

    Eigen::Vector3f calculateTorque(Eigen::Quaternionf quaternionOrbitBody,
                                    Eigen::Vector3f sunECIUnitVector,
                                    GlobalStateVector state,
                                    bool eclipse) const override;
};

#endif //ADCS_ONBOARD_SOFTWARE_NADIRPOINTING_H
