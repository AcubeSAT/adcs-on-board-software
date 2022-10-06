#include "Bdot.hpp"
#include "Parameters.hpp"
#include "MathFunctions.hpp"

using namespace Parameters::BDot;
using namespace Parameters::Actuators;
using namespace Eigen;

Bdot::Bdot(Vector3f samplingBeginMagneticFieldBody, Vector3f samplingEndMagneticFieldBody) :
        samplingBeginMagneticFieldBody{samplingBeginMagneticFieldBody},
        samplingEndMagneticFieldBody{samplingEndMagneticFieldBody},
        bDotVector{{0, 0, 0}}{}

Vector3f Bdot::controller() {
    bDotVector = (samplingEndMagneticFieldBody - samplingBeginMagneticFieldBody) / Timestep;

    Vector3f magneticDipole;
    if (samplingBeginMagneticFieldBody.norm() != 0) {
        magneticDipole = -Kp * bDotVector / samplingBeginMagneticFieldBody.norm();
    } else {
        magneticDipole = -Kp * bDotVector;
    }

    magneticDipole = magnetorquerScaling(magneticDipole);

    return magneticDipole;
}

Vector3f Bdot::magnetorquerScaling(Vector3f magneticDipole) {
    magneticDipole = clamp(magneticDipole, -MaxMagneticDipole, MaxMagneticDipole);
    return magneticDipole;
}