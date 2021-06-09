#include "MEKF.h"
#include "mathFunctions.h"

void MEKF::predict(float timestep, const SatelliteModel &satelliteModel, const Vector3f &gyroMeasurements) {
    F_k = satelliteModel.stateTransitionJacobian(globalState, gyroMeasurements);
    globalState = satelliteModel.stateTransitionFunction(globalState, gyroMeasurements);
    auto Phi = (F_k * timestep).exp();
    P = Phi * P * Phi.transpose() + Q;
}

void MEKF::correct(const MeasurementVector &measurement, const Vector3f &magneticField,
                   const Vector3f &sunPosition, bool eclipse, const SatelliteModel &satelliteModel) {
    H_k = satelliteModel.measurementJacobian(magneticField, sunPosition, eclipse, globalState);
    MeasurementVector estimatedMeasurement = satelliteModel.measurementFunction(magneticField, sunPosition, eclipse,
                                                                                globalState);

    auto I = Matrix<float, LocalStateSize, LocalStateSize>::Identity();
    auto temp = H_k * P * H_k.transpose() + R;
    K = (P * H_k.transpose()) * temp.colPivHouseholderQr().solve(I);

    MeasurementVector localErrorState = K * (measurement - estimatedMeasurement);

    Quaternionf localErrorQuaternion;
    localErrorQuaternion.w() = 1;
    localErrorQuaternion.vec() = 0.5 * localErrorState(seq(0, 2));

    Quaternionf currentQuaternion;
    currentQuaternion.w() = globalState(0);
    currentQuaternion.vec() = globalState(seq(1, 3));
    Quaternionf updatedQuaternion = quaternionProduct(currentQuaternion, localErrorQuaternion);
    updatedQuaternion = updatedQuaternion.normalized();

    globalState(0) = updatedQuaternion.w();
    globalState(seq(1, 3)) = updatedQuaternion.vec();
    globalState(seq(4, 6)) += localErrorState(seq(3, 5));

    P = (I - K * H_k) * P;
}