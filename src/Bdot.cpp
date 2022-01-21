#include "Bdot.hpp"
#include "Parameters.hpp"
#include "MathFunctions.hpp"

using namespace Parameters::BDot;
using namespace Parameters::Actuators;
using namespace Eigen;

Bdot::Bdot(Vector3f magneticFieldBody) :
        magneticFieldBody{magneticFieldBody},
        bDotVector{{0, 0, 0}}{}

Vector3f Bdot::controller(const Vector3f &magneticFieldBody) {        
    bDotVector = (magneticFieldBody - this->magneticFieldBody) / Timestep;

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
    magneticDipole = clamp(magneticDipole, -MaxMagneticDipole, MaxMagneticDipole);
    return magneticDipole;
}