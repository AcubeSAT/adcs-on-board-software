#include "SatelliteModel.hpp"
#include "MathFunctions.hpp"
#include "CSSCompensation.hpp"
#include "Parameters.hpp"

using namespace Eigen;
using namespace Parameters::SatelliteModel;

MeasurementVector
SatelliteModel::measurementFunction(const Vector3f magneticField, const Vector3f sunPosition, const bool eclipse, const GlobalStateVector state,
                                    const Vector3f satPositionECI,
                                    const float albedo) const {

    MeasurementVector measurements;

    Quaternionf quaternion;
    quaternion.w() = state(0);
    quaternion.vec() = state(seq(1, 3));

    Vector3f magneticFieldBody = rotateVector(quaternion, magneticField);

    measurements(seq(0, 2)) = magneticFieldBody.normalized();

    Vector3f sunPositionBody;

    if (!eclipse) {
        sunPositionBody = rotateVector(quaternion, sunPosition);
        sunPositionBody.normalize();
        measurements(seq(3, 5)) = cssCompensation(sunPositionBody, quaternion, satPositionECI, albedo);
    } else {
        measurements(seq(3, 5)).setZero();
    }
    return measurements;
}

GlobalStateVector
SatelliteModel::stateTransitionFunction(const GlobalStateVector state,
                                        const Vector3f gyroMeasurements) const {

    Vector3f bias = state(seq(4, 6));

    Vector3f angularVelocity = gyroMeasurements - bias;

    Quaternionf currentQuaternion;
    currentQuaternion.w() = state(0);
    currentQuaternion.vec() = state(seq(1, 3));

    Quaternionf nextQuaternion = quaternionProduct(currentQuaternion,
                                                   quaternionExponent(angularVelocity * Timestep));

    GlobalStateVector nextState;

    nextState(0) = nextQuaternion.w();
    nextState(seq(1, 3)) = nextQuaternion.vec();
    nextState(seq(4, 6)) = bias;

    return nextState;
}

Matrix<float, LocalStateSize, LocalStateSize>
SatelliteModel::stateTransitionJacobian(const GlobalStateVector state,
                                        const Vector3f gyroMeasurements) const {

    Matrix<float, LocalStateSize, LocalStateSize> F;
    F(seq(0, 2), seq(0, 2)) = skew(gyroMeasurements - state(seq(4, 6)));
    F(seq(0, 2), seq(3, 5)) = -Matrix<float, 3, 3>::Identity();
    F(seq(3, 5), seq(0, 5)) = Matrix<float, 3, 6>::Zero();
    return F;
}

Matrix<float, MeasurementSize, MeasurementSize>
SatelliteModel::measurementJacobian(const Vector3f magneticField,
                                    const Vector3f sunPosition, const bool eclipse,
                                    const GlobalStateVector state, const Vector3f satPositionECI, const float albedo) const {

    MeasurementVector estimatedMeasurements = measurementFunction(magneticField,
                                                                  sunPosition, eclipse,
                                                                  state, satPositionECI, albedo);
    Matrix<float, MeasurementSize, MeasurementSize> H;

    H(seq(0, 2), seq(0, 2)) = skew(estimatedMeasurements(seq(0, 2)));
    H(seq(3, 5), seq(0, 2)) = skew(estimatedMeasurements(seq(3, 5)));
    H(seq(0, 2), seq(3, 5)) = Matrix<float, 3, 3>::Zero();
    H(seq(3, 5), seq(3, 5)) = Matrix<float, 3, 3>::Zero();

    return H;
}