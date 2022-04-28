#include "Detumbling.hpp"

using namespace Eigen;

Vector3f getMagneticTorque(Bdot &bDot, Eigen::Vector3f magneticFieldBody){

    Vector3f magneticDipole = bDot.controller(magneticFieldBody);
    Vector3f bDotVector = bDot.getBDotVector();
    Vector3f magneticTorque = magneticDipole.cross(bDotVector);
    return magneticTorque;
}

Vector3f getAngularVelocity(Bdot &bDot){

    Vector3f bDotVector = bDot.getBDotVector();
    Vector3f magneticFieldBody = bDot.getMagneticFieldBody();
    Vector3f omegaBDot = (skew(magneticFieldBody)*(-bDotVector)) /
                (magneticFieldBody.transpose()*magneticFieldBody);

    return omegaBDot;
}

