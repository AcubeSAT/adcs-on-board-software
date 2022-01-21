#include "NadirPointing.hpp"
#include "MathFunctions.hpp"
#include "Parameters.hpp"

using namespace Eigen;
using namespace Parameters;

NadirPointing::NadirPointing(const Matrix<float, 3, 3> Kp,
                             const Matrix<float, 3, 3> Kd) : PointingTarget(Kp, Kd) {}

void NadirPointing::changeGains(const bool eclipse, Matrix<float, 3, 3> &KpGain, Matrix<float, 3, 3> &KdGain) const{
    if (eclipse) {
        KdGain = 20 * this->Kd;
        KpGain = 1.2 * this->Kp;
    } else {
        KpGain = this->Kp;
        KdGain = this->Kd;
    }
}

Vector3f NadirPointing::calculateTorque(const Quaternionf quaternionOrbitBody, [[maybe_unused]] const Vector3f sunECIUnitVector,
                                        const GlobalStateVector state, const bool eclipse) const{
    Matrix<float, 3, 3> KpGain;
    Matrix<float, 3, 3> KdGain;

    changeGains(eclipse, KpGain, KdGain);

    const Vector3f angularVelocityECIBody = state(seq(4, 6));

    const Vector3f angularVelocityOrbitBody =
            angularVelocityECIBody - rotateVector(quaternionOrbitBody, AngularVelocityECIOrbit);
    const Quaternionf errorQuaternion = quaternionProduct(DesiredQuaternion.conjugate(), quaternionOrbitBody);

    Vector3f commandedTorque =
            -copysignf(1, errorQuaternion.w()) * KpGain * errorQuaternion.vec() - KdGain * angularVelocityOrbitBody;

    return commandedTorque;
}
