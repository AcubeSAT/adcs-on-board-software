#include "SatelliteModel.h"
#include "mathFunctions.h"

using namespace Eigen;

SatelliteModel::SatelliteModel(float timestep) {
    this->timestep = timestep;
}

MeasurementVector SatelliteModel::measurementFunction(Vector3f magneticField,
                                                      Vector3f sunPosition,
                                                      bool eclipse,
                                                      GlobalStateVector state) const {

    MeasurementVector measurements;

    Quaternionf quaternion;
    quaternion.w() = state(0);
    quaternion.vec() = state(seq(1, 3));

    Vector3f magneticFieldBody = rotateVector(quaternion, magneticField);

    measurements(seq(0, 2)) = magneticFieldBody.normalized();

    Vector3f sunPositionBody;

    if (!eclipse) {
        /* TODO: cssCompensation */
        sunPositionBody = rotateVector(quaternion, sunPosition);
        measurements(seq(3, 5)) = sunPositionBody.normalized();
    } else {
        measurements(seq(3, 5)).setZero();
    }
    return measurements;
}

GlobalStateVector
SatelliteModel::stateTransitionFunction(GlobalStateVector state,
                                        Vector3f gyroMeasurements) const {

    Vector3f bias = state(seq(4, 6));

    Vector3f angularVelocity = gyroMeasurements - bias;

    Quaternionf currentQuaternion;
    currentQuaternion.w() = state(0);
    currentQuaternion.vec() = state(seq(1, 3));

    Quaternionf nextQuaternion = quaternionProduct(currentQuaternion,
                                                   quaternionExponent(angularVelocity * timestep));

    GlobalStateVector nextState;

    nextState(0) = nextQuaternion.w();
    nextState(seq(1, 3)) = nextQuaternion.vec();
    nextState(seq(4, 6)) = bias;

    return nextState;
}

Matrix<float, LocalStateSize, LocalStateSize>
SatelliteModel::stateTransitionJacobian(GlobalStateVector state,
                                        Vector3f gyroMeasurements) const {

    Matrix<float, LocalStateSize, LocalStateSize> F;
    F(seq(0, 2), seq(0, 2)) = skew(gyroMeasurements - state(seq(4, 6)));
    F(seq(0, 2), seq(3, 5)) = -Matrix<float, 3, 3>::Identity();
    F(seq(3, 5), seq(0, 5)) = Matrix<float, 3, 6>::Zero();
    return F;
}

Matrix<float, MeasurementSize, MeasurementSize>
SatelliteModel::measurementJacobian(Vector3f magneticField,
                                    Vector3f sunPosition, bool eclipse,
                                    GlobalStateVector state) const {

    MeasurementVector estimatedMeasurements = measurementFunction(magneticField,
                                                                  sunPosition, eclipse,
                                                                  state);
    Matrix<float, MeasurementSize, MeasurementSize> H;

    H(seq(0, 2), seq(0, 2)) = skew(estimatedMeasurements(seq(0, 2)));
    H(seq(3, 5), seq(0, 2)) = skew(estimatedMeasurements(seq(3, 5)));
    H(seq(0, 2), seq(3, 5)) = Matrix<float, 3, 3>::Zero();
    H(seq(3, 5), seq(3, 5)) = Matrix<float, 3, 3>::Zero();

    return H;
}