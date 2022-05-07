#include "Bdot.hpp"
#include "Parameters.hpp"
#include "MathFunctions.hpp"

using namespace Parameters::BDot;
using namespace Parameters::Actuators;
using namespace Eigen;

Bdot::Bdot(Vector3f cycleBeginMagneticFieldBody, Vector3f cycleEndMagneticFieldBody) :
        cycleBeginMagneticFieldBody{cycleBeginMagneticFieldBody},
        cycleEndMagneticFieldBody{cycleEndMagneticFieldBody},
        bDotVector{{0, 0, 0}}{}

Vector3f Bdot::controller(const Vector3f &cycleBeginMagneticFieldBody, const Vector3f &cycleEndMagneticFieldBody) {
    bDotVector = (cycleEndMagneticFieldBody - cycleBeginMagneticFieldBody) / Timestep;

    Vector3f magneticDipole;
    if (cycleBeginMagneticFieldBody.norm() != 0) {
        magneticDipole = -Kp * bDotVector / cycleBeginMagneticFieldBody.norm();
    } else {
        magneticDipole = -Kp * bDotVector;
    }

    magneticDipole = magnetorquerScaling(magneticDipole);

    this->cycleBeginMagneticFieldBody = cycleBeginMagneticFieldBody;
    this->cycleEndMagneticFieldBody = cycleEndMagneticFieldBody;

    return magneticDipole;
}

Vector3f Bdot::magnetorquerScaling(Vector3f magneticDipole) {
    magneticDipole = clamp(magneticDipole, -MaxMagneticDipole, MaxMagneticDipole);
    return magneticDipole;
}