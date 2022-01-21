#include "MagnetorquerOnly.hpp"
#include "MathFunctions.hpp"
#include "Parameters.hpp"
#include <cassert>

using namespace Eigen;
using namespace Parameters::Actuators;
using namespace Parameters;

Matrix<float, VectorSize, NumOfActuators>
MagnetorquerOnly::desaturateMagnetorquer(Vector3f desiredMagneticTorque,
                                         Vector3f desiredReactionWheelTorque, [[maybe_unused]] const Vector3f commandedTorque,
                                         const Vector3f magneticField, Vector3f desiredMagneticDipole) const {
    assert(desiredMagneticTorque.norm() != 0);

    double magneticTorqueGainSaturated;

    const Vector3f magnetorquerUpperLimits = MaxMagneticDipole + ResidualDipoleEstimation;
    const Vector3f magnetorquerLowerLimits = ResidualDipoleEstimation - MaxMagneticDipole;
    const Vector3f saturatedDipole = desiredMagneticDipole / desiredMagneticTorque.norm();

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
        desiredMagneticDipole = magneticTorqueGainSaturated * saturatedDipole;
        desiredMagneticTorque = desiredMagneticDipole.cross(magneticField);
    }

    desiredReactionWheelTorque = {0, 0, 0};

    Matrix<float, VectorSize, NumOfActuators> actuatorTorque;
    actuatorTorque(seq(0, 2), 0) = desiredMagneticTorque;
    actuatorTorque(seq(0, 2), 1) = desiredReactionWheelTorque;

    return actuatorTorque;
}

Matrix<float, VectorSize, NumOfActuators>
MagnetorquerOnly::actuate(const Vector3f commandedTorque, const Vector3f magneticField, [[maybe_unused]] const bool firstTime,
                          [[maybe_unused]] const float currentReactionWheelAngularVelocity,
                          [[maybe_unused]] const float oldReactionWheelAcceleration) const {
    assert(not magneticField.isZero());

    Vector3f magneticFieldNormalized = magneticField.normalized();
    Vector3f desiredMagneticTorque =
            skew(magneticFieldNormalized).transpose() * skew(magneticFieldNormalized) * commandedTorque;
    Vector3f desiredMagneticDipole = skew(magneticField) * desiredMagneticTorque / (pow(magneticField.norm(), 2));

    Matrix<float, VectorSize, NumOfActuators> desiredActuatorsTorque = desaturateMagnetorquer(desiredMagneticTorque,
                                                                                                 {0, 0, 0},
                                                                                                 commandedTorque,
                                                                                                 magneticField,
                                                                                                 desiredMagneticDipole);

    desiredMagneticTorque = desiredActuatorsTorque.col(0);
    desiredMagneticDipole = skew(magneticField) * desiredMagneticTorque / (pow(magneticField.norm(), 2));
    desiredMagneticDipole = desiredMagneticDipole - ResidualDipoleEstimation;
    desiredMagneticTorque = desiredMagneticDipole.cross(magneticField);

    Vector3f desiredReactionWheelTorque = {0, 0, 0};
    Matrix<float, VectorSize, NumOfActuators> actuatorTorque;
    actuatorTorque(seq(0, 2), 0) = desiredMagneticTorque;
    actuatorTorque(seq(0, 2), 1) = desiredReactionWheelTorque;

    return actuatorTorque;
}