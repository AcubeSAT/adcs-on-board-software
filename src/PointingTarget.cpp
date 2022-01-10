#include "PointingTarget.hpp"

using namespace Eigen;

PointingTarget::PointingTarget(Matrix<float, 3, 3> Kp, Matrix<float, 3, 3> Kd) :
        Kp{Kp},
        Kd{Kd}{}

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