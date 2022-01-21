#include "Pointing.hpp"

using namespace Eigen;


Pointing::Pointing(PointingTarget &target, PointingStrategy &strategy) : target(target), strategy(strategy) {
}

Matrix<float, VectorSize, NumOfActuators> Pointing::performPointing(const Quaternionf quaternion,
                                                                    const Vector3f sunECIUnitVector,
                                                                    const GlobalStateVector state,
                                                                    const bool eclipse, const Vector3f magneticField,
                                                                    const bool firstTime,
                                                                    const float currentReactionWheelAngularVelocity,
                                                                    const float oldReactionWheelAcceleration) const {
    Vector3f commandedTorque = target.calculateTorque(quaternion, sunECIUnitVector, state, eclipse);

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque = strategy.actuate(commandedTorque, magneticField,
                                                                                firstTime,
                                                                                currentReactionWheelAngularVelocity,
                                                                                oldReactionWheelAcceleration);
    return actuatorTorque;
}