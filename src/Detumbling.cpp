#include "Detumbling.hpp"

using namespace Eigen;

void Detumbling(const Vector3f cycleBeginMagneticFieldMeasurement, const Vector3f cycleEndMagneticFieldMeasurement) {

    Vector3f cycleBeginMagneticFieldMeasurementInitial = {0, 0, 0};
    Vector3f cycleEndMagneticFieldMeasurementInitial = {0, 0, 0};

    Bdot bDot(cycleBeginMagneticFieldMeasurementInitial,
              cycleEndMagneticFieldMeasurementInitial);

    Vector3f desiredMagneticTorque = calculateDesiredMagneticTorque(bDot, cycleBeginMagneticFieldMeasurement,
                                                             cycleEndMagneticFieldMeasurement);

    Vector3f angularVelocityEstimation = estimateAngularVelocity(bDot.getBDotVector(),
                                                                 bDot.getCycleBeginMagneticFieldBody());
}

Vector3f calculateDesiredMagneticTorque(Bdot &bDot, const Eigen::Vector3f cycleBeginMagneticFieldMeasurement,
                                 const Eigen::Vector3f cycleEndMagneticFieldMeasurement) {

    Vector3f magneticDipole = bDot.controller(cycleBeginMagneticFieldMeasurement,
                                              cycleEndMagneticFieldMeasurement);
    Vector3f desiredMagneticTorque = magneticDipole.cross(cycleBeginMagneticFieldMeasurement);

    return desiredMagneticTorque;
}

Vector3f estimateAngularVelocity(const Vector3f bDotVector, const Vector3f magneticFieldBody) {

    Vector3f angularVelocityEstimation = (skew(magneticFieldBody) * (-bDotVector)) /
                         (magneticFieldBody.transpose() * magneticFieldBody);

    return angularVelocityEstimation;
}
