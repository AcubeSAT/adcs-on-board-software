#include "PointingStrategy.h"

using namespace Eigen;

PointingStrategy::PointingStrategy(Vector3f maxMagneticDipoleMoment, Vector3f residualDipoleEstimation)
        : maxMagneticDipoleMoment{maxMagneticDipoleMoment}, residualDipoleEstimation{residualDipoleEstimation} {}


Vector3f PointingStrategy::scaleMagnetorquerDipole(Vector3f magneticDipole) const {
    for (int i = 0; i < VectorSize; i++) {
        if (abs(magneticDipole(i)) > maxMagneticDipoleMoment(i)) {
            magneticDipole(i) = copysignf(maxMagneticDipoleMoment(i), magneticDipole(i));
        }
    }

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
