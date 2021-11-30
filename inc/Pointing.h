#ifndef ADCS_ONBOARD_SOFTWARE_POINTING_H
#define ADCS_ONBOARD_SOFTWARE_POINTING_H

#include "PointingStrategy.h"
#include "PointingTarget.h"

/**
 * Class that models pointing target (nadir or sun) and strategy (magnetorquer only or magnetorquer and reaction wheel actuation)
 */
class Pointing {
private:
    /**
     * Pointing target (nadir or sun)
     */
    PointingTarget &target;

    /**
     * Pointing strategy (magnetorquer only or magnetorquer and reaction wheel actuation)
     */
    PointingStrategy &strategy;

public:
    /**
     * @param target pointing target (nadir or sun)
     * @param strategy pointing strategy (magnetorquer only or magnetorquer and reaction wheel actuation)
     */
    Pointing(PointingTarget &target, PointingStrategy &strategy);

    /**
     * Calculates the magnetic and reaction wheel torque needed to achieve pointing
     * @param quaternion quaternion that expresses a rotation from one frame to another
     * @param sunEciUnitVector unit vector from satellite to sun expressed in ECI frame
     * @param state 7-element vector consisting of the quaternion representation of the satellite's orientation and
     * angular velocity of the ECI frame with respect to the body frame, expressed in the body frame
     * @param eclipse existence or not of eclipse
     * @param magneticField magnetic field value expressed in the Body Frame (in T)
     * @param firstTime True if the current control cycle is the first
     * @param currentReactionWheelAngularVelocity Rotation speed of the reaction wheel in the current step of the control cycle (in rpm)
     * @param oldReactionWheelAcceleration Angular acceleration of the reaction wheel in the previous step of the control cycle (in rpm/sec)
     * @return Magnetic torque and Reaction Wheel torque
     */
    Eigen::Matrix<float, VECTOR_SIZE, NUM_OF_ACTUATORS> performPointing(Eigen::Quaternionf quaternion,
                                                                        Eigen::Vector3f sunEciUnitVector,
                                                                        GlobalStateVector state,
                                                                        bool eclipse,
                                                                        Eigen::Vector3f magneticField,
                                                                        bool firstTime,
                                                                        float currentReactionWheelAngularVelocity,
                                                                        float oldReactionWheelAcceleration) const;


};

#endif //ADCS_ONBOARD_SOFTWARE_POINTING_H
