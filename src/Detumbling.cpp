#include "Detumbling.hpp"

using namespace Eigen;

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
