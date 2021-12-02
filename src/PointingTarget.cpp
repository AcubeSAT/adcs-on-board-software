#include "PointingTarget.hpp"

using namespace Eigen;

PointingTarget::PointingTarget(Matrix<float, 3, 3> Kp, Matrix<float, 3, 3> Kd,
                               Quaternionf desiredQuaternion, Vector3f angularVelocityECIOrbit) :
        Kp{Kp},
        Kd{Kd},
        desiredQuaternion{desiredQuaternion},
        angularVelocityECIOrbit{angularVelocityECIOrbit} {}

void PointingTarget::setKp(Matrix<float, 3, 3> Kp) {
    this->Kp = Kp;
}

Matrix<float, 3, 3> PointingTarget::getKp() const {
    return Kp;
}

void PointingTarget::setKd(Matrix<float, 3, 3> Kd) {
    this->Kd = Kd;
}

Matrix<float, 3, 3> PointingTarget::getKd() const {
    return Kd;
}

void PointingTarget::setDesiredQuaternion(Quaternionf desiredQuaternion) {
    this->desiredQuaternion = desiredQuaternion;
}

Quaternionf PointingTarget::getDesiredQuaternion() const {
    return desiredQuaternion;
}

void PointingTarget::setAngularVelocityECIOrbit(Vector3f angularVelocityECIOrbit) {
    this->angularVelocityECIOrbit = angularVelocityECIOrbit;
}

Vector3f PointingTarget::getAngularVelocityECIOrbit() const {
    return angularVelocityECIOrbit;
}

