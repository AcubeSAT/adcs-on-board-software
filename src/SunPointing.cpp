#include "SunPointing.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

SunPointing::SunPointing(Matrix<float, 3, 3> Kp,
                         Matrix<float, 3, 3> Kd,
                         Quaternionf desiredQuaternion,
                         Vector3f angularVelocityECIOrbit) : PointingTarget(Kp, Kd, desiredQuaternion,
                                                                            angularVelocityECIOrbit) {}

void SunPointing::changeGains(bool eclipse, Matrix<float, 3, 3> &KpGain, Matrix<float, 3, 3> &KdGain) const {
    if (eclipse) {
        KpGain = 20 * this->Kd;
        KdGain = 1.2 * this->Kp;
    } else {
        KpGain = this->Kd;
        KdGain = this->Kp;
    }
}

Vector3f SunPointing::calculateTorque([[maybe_unused]] Quaternionf quaternionOrbitBody, Vector3f sunECIUnitVector,
                                      GlobalStateVector state, bool eclipse) const {
    Matrix<float, 3, 3> KpGain;
    Matrix<float, 3, 3> KdGain;

    changeGains(eclipse, KpGain, KdGain);

    Vector3f angularVelocityECIBody = state(seq(4, 6));

    Quaternionf quaternionECIBody;
    quaternionECIBody.w() = state(0);
    quaternionECIBody.vec() = state(seq(1, 3));

    Quaternionf quaternionSunBody = calculateQuaternionSunBody(sunECIUnitVector, quaternionECIBody);
    Quaternionf errorQuaternion = quaternionProduct(desiredQuaternion.conjugate(), quaternionSunBody);
    Vector3f commandedTorque =
            -copysignf(1, errorQuaternion.w()) * KpGain * errorQuaternion.vec() - KdGain * angularVelocityECIBody;

    return commandedTorque;
}

Quaternionf
SunPointing::calculateQuaternionSunBody(Vector3f sunECIUnitVector, Quaternionf quaternionECIBody) const {
    Vector3f desiredSunVector = {-1, 1, 0};
    desiredSunVector = desiredSunVector.normalized();
    sunECIUnitVector = sunECIUnitVector.normalized();

    Vector3f sunBodyUnitVector = rotateVector(quaternionECIBody, sunECIUnitVector);

    Quaternionf quaternionSunBody;
    if (sunBodyUnitVector.dot(desiredSunVector) > 0.99) {
        quaternionSunBody = {1, 0, 0, 0};
    } else if (sunBodyUnitVector.dot(desiredSunVector) < -0.99) {
        quaternionSunBody = {0.2, -0.4, -0.4, -0.8};
    } else {
        quaternionSunBody.w() = 1 + sunBodyUnitVector.dot(desiredSunVector);
        quaternionSunBody.vec() = sunBodyUnitVector.cross(desiredSunVector);
    }

    return quaternionSunBody.normalized();

}
