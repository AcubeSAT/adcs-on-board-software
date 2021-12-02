#include "Pointing.h"

using namespace Eigen;


Pointing::Pointing(PointingTarget &target, PointingStrategy &strategy) : target(target), strategy(strategy) {
}

Matrix<float, VectorSize, NumOfActuators> Pointing::performPointing(Quaternionf quaternion,
                                                                       Vector3f sunECIUnitVector,
                                                                       GlobalStateVector state,
                                                                       bool eclipse, Vector3f magneticField,
                                                                       bool firstTime,
                                                                       float currentReactionWheelAngularVelocity,
                                                                       float oldReactionWheelAcceleration) const {
    Vector3f commandedTorque = target.calculateTorque(quaternion, sunECIUnitVector, state, eclipse);

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque = strategy.actuate(commandedTorque, magneticField,
                                                                                   firstTime,
                                                                                   currentReactionWheelAngularVelocity,
                                                                                   oldReactionWheelAcceleration);
    return actuatorTorque;
}