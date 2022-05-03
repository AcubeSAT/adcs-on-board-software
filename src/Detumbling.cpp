#include "Detumbling.hpp"

using namespace Eigen;

Vector3f calculateMagneticTorque(Bdot &bDot, Eigen::Vector3f magneticFieldMeasurement1,
                                 Eigen::Vector3f magneticFieldMeasurement2) {
    bDot.setMagneticFieldBody(magneticFieldMeasurement1);

    Vector3f magneticDipole = bDot.controller(magneticFieldMeasurement2);
    Vector3f bDotVector = bDot.getBDotVector();
    Vector3f magneticTorque = magneticDipole.cross(magneticFieldMeasurement1);

    bDot.setMagneticFieldBody(magneticFieldMeasurement1);

    return magneticTorque;
}

Vector3f estimateAngularVelocityEstimation(Bdot &bDot) {

    Vector3f bDotVector = bDot.getBDotVector();
    Vector3f magneticFieldBody = bDot.getMagneticFieldBody();
    Vector3f omegaBDot = (skew(magneticFieldBody) * (-bDotVector)) /
                         (magneticFieldBody.transpose() * magneticFieldBody);

    return omegaBDot;
}

