#include "Detumbling.hpp"

using namespace Eigen;

void detumbling(Bdot &bDot, const Vector3f magneticFieldBody1,
                const Vector3f magneticFieldBody2) {

    updateMagneticFieldMeasurements(bDot, magneticFieldBody1, magneticFieldBody2);

    Vector3f desiredMagneticTorque = calculateDesiredMagneticTorque(bDot);

    Vector3f angularVelocityEstimation = estimateAngularVelocity(bDot.getBDotVector(),
                                                                 bDot.getSamplingEndMagneticFieldBody());
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

void updateMagneticFieldMeasurements(Bdot &bDot, const Eigen::Vector3f magneticFieldBody1,
                                     const Eigen::Vector3f magneticFieldBody2) {
    bDot.setSamplingBeginMagneticFieldBody(magneticFieldBody1);
    bDot.setSamplingEndMagneticFieldBody(magneticFieldBody2);
}