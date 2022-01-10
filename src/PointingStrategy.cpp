#include "PointingStrategy.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

PointingStrategy::PointingStrategy(Vector3f maxMagneticDipoleMoment, Vector3f residualDipoleEstimation)
        : maxMagneticDipoleMoment{maxMagneticDipoleMoment}, residualDipoleEstimation{residualDipoleEstimation} {}


Vector3f PointingStrategy::scaleMagnetorquerDipole(Vector3f magneticDipole) const {
    magneticDipole = clamp(magneticDipole, -maxMagneticDipoleMoment, maxMagneticDipoleMoment);
    return magneticDipole;
}

void PointingStrategy::setMaxMagneticDipoleMoment(Vector3f maxMagneticDipoleMoment) {
    this->maxMagneticDipoleMoment = maxMagneticDipoleMoment;
}

Vector3f PointingStrategy::getMaxMagneticDipoleMoment() const {
    return maxMagneticDipoleMoment;
}

void PointingStrategy::setResidualDipoleEstimation(Vector3f residualDipoleEstimation) {
    this->residualDipoleEstimation = residualDipoleEstimation;
}

Vector3f PointingStrategy::getResidualDipoleEstimation() const {
    return residualDipoleEstimation;
}
