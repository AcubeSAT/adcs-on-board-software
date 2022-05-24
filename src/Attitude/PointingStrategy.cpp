#include "PointingStrategy.hpp"
#include "Parameters.hpp"
#include "MathFunctions.hpp"

using namespace Parameters::Actuators;
using namespace Eigen;

Vector3f PointingStrategy::scaleMagnetorquerDipole(Vector3f magneticDipole) const {
    magneticDipole = clamp(magneticDipole, -MaxMagneticDipole, MaxMagneticDipole);
    return magneticDipole;
}