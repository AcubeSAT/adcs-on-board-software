#include "Detumbling.hpp"

using namespace Eigen;

Vector3f getMagneticTorque(Bdot &bDot, Eigen::Vector3f magneticFieldBody1, Eigen::Vector3f magneticFieldBody2) {
    bDot.setMagneticFieldBody(magneticFieldBody1);

    Vector3f magneticDipole = bDot.controller(magneticFieldBody2);
    Vector3f bDotVector = bDot.getBDotVector();
    Vector3f magneticTorque = magneticDipole.cross(magneticFieldBody1);

    bDot.setMagneticFieldBody(magneticFieldBody1);

    return magneticTorque;
}

Vector3f getAngularVelocityEstimation(Bdot &bDot) {

    Vector3f bDotVector = bDot.getBDotVector();
    Vector3f magneticFieldBody = bDot.getMagneticFieldBody();
    Vector3f omegaBDot = (skew(magneticFieldBody) * (-bDotVector)) /
                         (magneticFieldBody.transpose() * magneticFieldBody);

    return omegaBDot;
}

