#include "MagnetorquerPlusRW.hpp"
#include "MathFunctions.hpp"
#include "Parameters.hpp"
#include <cassert>

using namespace Eigen;
using namespace Parameters;
using namespace Parameters::Actuators;

Matrix<float, VectorSize, NumOfActuators>
MagnetorquerPlusRW::splitTorque(Vector3f magneticField, Vector3f commandedTorque) const {
    Vector3f z(0, 0, 1);
    Vector3f desiredReactionWheelTorque = z * (magneticField.dot(commandedTorque)) / magneticField.z();

    Vector3f desiredMagneticTorque = skew(magneticField.normalized()).transpose() * skew(magneticField.normalized())
                                     * (commandedTorque - desiredReactionWheelTorque);

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque;
    actuatorTorque(seq(0, 2), 0) = desiredMagneticTorque;
    actuatorTorque(seq(0, 2), 1) = desiredReactionWheelTorque;

    return actuatorTorque;
}

Matrix<float, VectorSize, NumOfActuators>
MagnetorquerPlusRW::desaturateMagnetorquer(Vector3f desiredMagneticTorque,
                                           Vector3f desiredReactionWheelTorque, Vector3f commandedTorque,
                                           Vector3f magneticField, Vector3f desiredMagneticDipole) const {
    double magneticTorqueGain, reactionWheelTorqueGain, magneticTorqueGainSaturated, reactionWheelTorqueGainSaturated;

    Vector3f magnetorquerUpperLimits = MaxMagneticDipole + ResidualDipoleEstimation;
    Vector3f magnetorquerLowerLimits = ResidualDipoleEstimation - MaxMagneticDipole;
    Vector3f saturatedDipole = desiredMagneticDipole / desiredMagneticTorque.norm();
    Vector3f reactionWheelTorqueUnitVector = desiredReactionWheelTorque.normalized();
    Vector3f magneticTorqueUnitVector = desiredMagneticTorque.normalized();

    magneticTorqueGain = (magneticTorqueUnitVector - (reactionWheelTorqueUnitVector.dot(magneticTorqueUnitVector)) *
                                                     reactionWheelTorqueUnitVector).dot(commandedTorque) /
                         (1 - pow(reactionWheelTorqueUnitVector.dot(magneticTorqueUnitVector), 2));
    reactionWheelTorqueGain = (reactionWheelTorqueUnitVector -
                               (magneticTorqueUnitVector.dot(reactionWheelTorqueUnitVector)) *
                               magneticTorqueUnitVector).dot(commandedTorque) /
                              (1 - pow(magneticTorqueUnitVector.dot(reactionWheelTorqueUnitVector), 2));

    if (desiredMagneticDipole.x() > magnetorquerUpperLimits.x() ||
        desiredMagneticDipole.y() > magnetorquerUpperLimits.y() ||
        desiredMagneticDipole.z() > magnetorquerUpperLimits.z() ||
        desiredMagneticDipole.x() < magnetorquerLowerLimits.x() ||
        desiredMagneticDipole.y() < magnetorquerLowerLimits.y() ||
        desiredMagneticDipole.z() < magnetorquerLowerLimits.z()) {

        Vector3f magnetorquerLimits;
        for (int i = 0; i < VectorSize; i++) {
            if (desiredMagneticDipole(i) > 0){
                magnetorquerLimits(i) = magnetorquerUpperLimits(i);
            }
            else{
                magnetorquerLimits(i) = magnetorquerLowerLimits(i);
            }
            assert(magnetorquerLimits(i) != 0);
        }

        magneticTorqueGainSaturated = (magnetorquerLimits.array().abs() / saturatedDipole.array().abs()).minCoeff();
        reactionWheelTorqueGainSaturated = magneticTorqueGainSaturated * reactionWheelTorqueGain / magneticTorqueGain;
        desiredMagneticDipole = magneticTorqueGainSaturated * saturatedDipole;
        desiredMagneticTorque = desiredMagneticDipole.cross(magneticField);
        desiredReactionWheelTorque = reactionWheelTorqueGainSaturated * reactionWheelTorqueUnitVector;
    } else {
        desiredReactionWheelTorque = reactionWheelTorqueGain * reactionWheelTorqueUnitVector;
        desiredMagneticTorque = magneticTorqueGain * magneticTorqueUnitVector;
    }

    desiredReactionWheelTorque = scaleReactionWheelTorque(desiredReactionWheelTorque);

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque;
    actuatorTorque(seq(0, 2), 0) = desiredMagneticTorque;
    actuatorTorque(seq(0, 2), 1) = desiredReactionWheelTorque;

    return actuatorTorque;
}

Matrix<float, VectorSize, NumOfActuators>
MagnetorquerPlusRW::desaturateReactionWheel(Vector3f effectiveMagneticTorque,
                                            Vector3f reactionWheelTorque, Vector3f magneticFieldBody,
                                            float reactionWheelAngularVelocity,
                                            float reactionWheelAngularAcceleration) const {
    Vector3f initialMagneticTorque = effectiveMagneticTorque;

    if (((reactionWheelAngularVelocity > ReactionWheelAngularVelocityLimit && reactionWheelAngularAcceleration > 0) ||
         (reactionWheelAngularVelocity < -ReactionWheelAngularVelocityLimit && reactionWheelAngularAcceleration < 0))
        && abs(reactionWheelTorque(2)) > 0) {

        Vector3f addedTorque{0, 0, (TorquePercentage * FlywheelInertia * reactionWheelAngularAcceleration)};
        Vector3f magneticTorque = effectiveMagneticTorque + addedTorque;
        Vector3f magneticDipole = (-magneticTorque.cross(magneticFieldBody)) / (float) pow(magneticFieldBody.norm(), 2);
        magneticDipole = scaleMagnetorquerDipole(magneticDipole);
        effectiveMagneticTorque = magneticDipole.cross(magneticFieldBody);

        reactionWheelTorque = reactionWheelTorque - (effectiveMagneticTorque - initialMagneticTorque);
    }

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque;
    actuatorTorque(seq(0, 2), 0) = effectiveMagneticTorque;
    actuatorTorque(seq(0, 2), 1) = reactionWheelTorque;

    return actuatorTorque;
}

Vector3f MagnetorquerPlusRW::scaleReactionWheelTorque(Vector3f reactionWheelTorque) const {
    reactionWheelTorque(2) = clamp(reactionWheelTorque(2), -MaxReactionWheelTorque, MaxReactionWheelTorque);
    return reactionWheelTorque;
}

Matrix<float, VectorSize, NumOfActuators>
MagnetorquerPlusRW::actuate(Vector3f commandedTorque, Vector3f magneticField, bool firstTime,
                            float currentReactionWheelAngularVelocity,
                            float oldReactionWheelAcceleration) const {
    assert(magneticField.norm() != 0);

    Matrix<float, VectorSize, NumOfActuators> desiredActuatorsTorque = splitTorque(magneticField, commandedTorque);

    Vector3f desiredMagneticTorque = desiredActuatorsTorque.col(0);
    Vector3f desiredReactionWheelTorque = desiredActuatorsTorque.col(1);
    Vector3f desiredMagneticDipole = skew(magneticField) * desiredMagneticTorque / (pow(magneticField.norm(), 2));

    desiredActuatorsTorque = desaturateMagnetorquer(desiredMagneticTorque,
                                                    desiredReactionWheelTorque,
                                                    commandedTorque,
                                                    magneticField,
                                                    desiredMagneticDipole);

    desiredMagneticTorque = desiredActuatorsTorque.col(0);
    desiredReactionWheelTorque = desiredActuatorsTorque.col(1);
    desiredMagneticDipole = skew(magneticField) * desiredMagneticTorque / (pow(magneticField.norm(), 2));
    desiredMagneticDipole = desiredMagneticDipole - ResidualDipoleEstimation;
    desiredMagneticTorque = desiredMagneticDipole.cross(magneticField);


    if (!firstTime) {
        desiredActuatorsTorque = desaturateReactionWheel(desiredMagneticTorque,
                                                         desiredReactionWheelTorque,
                                                         magneticField,
                                                         currentReactionWheelAngularVelocity,
                                                         oldReactionWheelAcceleration);

        desiredMagneticTorque = desiredActuatorsTorque.col(0);
        desiredReactionWheelTorque = desiredActuatorsTorque.col(1);
        desiredReactionWheelTorque = scaleReactionWheelTorque(desiredReactionWheelTorque);
    }

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque;
    actuatorTorque(seq(0, 2), 0) = desiredMagneticTorque;
    actuatorTorque(seq(0, 2), 1) = desiredReactionWheelTorque;

    return actuatorTorque;
}
