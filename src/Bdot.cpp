#include "Bdot.h"

using namespace Eigen;

Bdot::Bdot(Vector3f magneticFieldBody, Matrix<float, 3, 3> Kp, Vector3f maxMagneticDipole, float timestep) :
        magneticFieldBody{magneticFieldBody},
        Kp{Kp},
        bDotVector{{0, 0, 0}}, maxMagneticDipole{maxMagneticDipole}, timestep{timestep} {}

Vector3f Bdot::controller(Vector3f &magneticFieldBody) {

    if (timestep != 0) timestep = 0.1;
        
    bDotVector = (magneticFieldBody - this->magneticFieldBody) / timestep;

    Vector3f magneticDipole;
    if ((this->magneticFieldBody.norm()) != 0) {
        magneticDipole = -Kp * bDotVector / this->magneticFieldBody.norm();
    } else {
        magneticDipole = -Kp * bDotVector;
    }

    magneticDipole = magnetorquerScaling(magneticDipole);

    this->magneticFieldBody = magneticFieldBody;

    return magneticDipole;
}

Vector3f Bdot::magnetorquerScaling(Vector3f magneticDipole) {
    for (int i = 0; i < 3; i++) {
        if (abs(magneticDipole(i)) > maxMagneticDipole(i)) {
            magneticDipole(i) = copysignf(maxMagneticDipole(i), magneticDipole(i));
        }
    }

    return magneticDipole;
}