#include "NadirPointing.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

NadirPointing::NadirPointing(Matrix<float, 3, 3> Kp,
                             Matrix<float, 3, 3> Kd,
                             Quaternionf desiredQuaternion,
                             Vector3f angularVelocityECIOrbit) : PointingTarget(Kp, Kd, desiredQuaternion,
                                                                                angularVelocityECIOrbit) {}

void NadirPointing::changeGains(bool eclipse, Matrix<float, 3, 3> &KpGain, Matrix<float, 3, 3> &KdGain) const{
    if (eclipse) {
        KdGain = 20 * this->Kd;
        KpGain = 1.2 * this->Kp;
    } else {
        KpGain = this->Kp;
        KdGain = this->Kd;
    }
}

Vector3f NadirPointing::calculateTorque(Quaternionf quaternionOrbitBody, [[maybe_unused]] Vector3f sunECIUnitVector,
                                        GlobalStateVector state, bool eclipse) const{
    Matrix<float, 3, 3> KpGain;
    Matrix<float, 3, 3> KdGain;

    changeGains(eclipse, KpGain, KdGain);

    Vector3f angularVelocityECIBody = state(seq(4, 6));

    Vector3f angularVelocityOrbitBody =
            angularVelocityECIBody - rotateVector(quaternionOrbitBody, angularVelocityECIOrbit);
    Quaternionf errorQuaternion = quaternionProduct(desiredQuaternion.conjugate(), quaternionOrbitBody);

    Vector3f commandedTorque =
            -copysignf(1, errorQuaternion.w()) * KpGain * errorQuaternion.vec() - KdGain * angularVelocityOrbitBody;

    return commandedTorque;
}
