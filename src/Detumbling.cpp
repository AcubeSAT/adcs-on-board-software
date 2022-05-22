#include "Detumbling.hpp"

using namespace Eigen;
using namespace Parameters::Detumbling;

void Detumbling(Bdot &bDot, Quaternionf &ECIToBodyQuaternion, Vector3f &currentAngularVelocity,
                EnvironmentalModel &environmentalModel, bool &isDone) {
    environmentalModel.ModelEnvironment();
    Vector3f samplingBeginMagneticFieldBody = rotateVector(ECIToBodyQuaternion, environmentalModel.getMagneticField());

    updateQuaternion(ECIToBodyQuaternion, currentAngularVelocity, Parameters::BDot::Timestep);

    environmentalModel.ModelEnvironment();
    Vector3f samplingEndMagneticFieldBody = rotateVector(ECIToBodyQuaternion, environmentalModel.getMagneticField());

    bDot.setSamplingBeginMagneticFieldBody(samplingBeginMagneticFieldBody);
    bDot.setSamplingEndMagneticFieldBody(samplingEndMagneticFieldBody);

    environmentalModel.ModelEnvironment();

    Vector3f desiredMagneticTorque = calculateDesiredMagneticTorque(bDot);
    Vector3f angularVelocityEstimation = estimateAngularVelocity(bDot.getBDotVector(),
                                                                 bDot.getSamplingBeginMagneticFieldBody());

    actuate(currentAngularVelocity, ECIToBodyQuaternion, desiredMagneticTorque, Timestep, ActuationTime);

    for (int i = 0; i < 7; ++i) {
        environmentalModel.ModelEnvironment();
    }

    isDone = (abs(angularVelocityEstimation.x()) < AngularVelocityTriggerValue) &&
             (abs(angularVelocityEstimation.y()) < AngularVelocityTriggerValue) &&
             (abs(angularVelocityEstimation.z()) < AngularVelocityTriggerValue);
}


Vector3f calculateDesiredMagneticTorque(Bdot &bDot) {
    const Vector3f magneticDipole = bDot.controller();

    const Vector3f desiredMagneticTorque = magneticDipole.cross(bDot.getSamplingBeginMagneticFieldBody());

    return desiredMagneticTorque;
}

Vector3f estimateAngularVelocity(const Vector3f bDotVector, const Vector3f magneticFieldBody) {
    const Vector3f angularVelocityEstimation = (skew(magneticFieldBody) * (-bDotVector)) /
                                               (magneticFieldBody.transpose() * magneticFieldBody);

    return angularVelocityEstimation;
}

void updateQuaternion(Eigen::Quaternionf &quaternion, const Eigen::Vector3f angularVelocity, const float Timestep) {
    Quaternionf angularVelocityToQuaternion = {0, angularVelocity.x(), angularVelocity.y(), angularVelocity.z()};
    Quaternionf quaternionDerivative = {0, 0, 0, 0};

    if (angularVelocityToQuaternion.norm() > 1e-200) {
        quaternionDerivative = quaternionProduct(quaternion, angularVelocityToQuaternion);
        quaternion.coeffs() += 0.5 * quaternionDerivative.coeffs() * Timestep;
        quaternion.normalize();
    }
}

void
actuate(Eigen::Vector3f &angularVelocity, Eigen::Quaternionf &quaternion, const Eigen::Vector3f desiredMagneticTorque,
        const float timestep, const float actuationTime) {
    Quaternionf quaternionDerivative = {0, 0, 0, 0};
    Quaternionf angularVelocityToQuaternion = {0, 0, 0, 0};

    float timePassed = 0;
    while (timePassed < actuationTime) {
        angularVelocity = 7 * 0.5 * angularVelocity * (timestep / 2) + 0.5 * Inertia.inverse() *
                          (desiredMagneticTorque - angularVelocity.cross(Inertia * angularVelocity)) * pow((timestep / 2), 2);

        angularVelocityToQuaternion = {0, angularVelocity.x(), angularVelocity.y(), angularVelocity.z()};
        quaternionDerivative = quaternionProduct(quaternion, angularVelocityToQuaternion);

        if (angularVelocityToQuaternion.norm() > 1e-200) {
            quaternionDerivative.normalize();
            quaternion = quaternionProduct(quaternion, quaternionDerivative);
            quaternion.normalize();
        }

        timePassed += (timestep / 2);
    }
}