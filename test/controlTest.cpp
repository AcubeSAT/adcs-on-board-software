#include <catch2/catch.hpp>
#include "Pointing.h"
#include "SunPointing.h"
#include "NadirPointing.h"
#include "MagnetorquerOnly.h"
#include "MagnetorquerPlusRW.h"

using namespace Eigen;


const Quaternionf desiredQuaternion = {1, 0, 0, 0};
const Vector3f residualDipoleEstimation = {0.05, 0.05, 0.05};
const Vector3f angularVelocityECIOrbit = {0, 0.0011, 0};
const Vector3f maxMagneticDipole = {0.2, 0.2, 0.2};
const float maxReactionWheelTorque = 1e-04;
const float reactionWheelAngularVelocityLimit = 10000;
const float torquePercentage = 0.12;
const float flywheelInertia = 0.0015;

const auto Kp = Matrix<float, 3, 3>::Identity(3, 3);
const auto Kd = Matrix<float, 3, 3>::Identity(3, 3);

MagnetorquerPlusRW magnetorquerPlusRW(maxMagneticDipole, residualDipoleEstimation, maxReactionWheelTorque,
                                      reactionWheelAngularVelocityLimit, torquePercentage, flywheelInertia);
MagnetorquerOnly magnetorquerOnly(maxMagneticDipole, residualDipoleEstimation);

SunPointing sunPointing(Kp, Kd, desiredQuaternion, angularVelocityECIOrbit);
NadirPointing nadirPointing(Kp, Kd, desiredQuaternion, angularVelocityECIOrbit);


TEST_CASE("Pointing - Nadir Pointing Magnetorquers Plus RW") {
    Pointing nadirPointingMagnetorquerPlusRW(nadirPointing, magnetorquerPlusRW);

    GlobalStateVector state = {0, 0, 0, 1, 0.0539166, 0.05612411, -0.03854784};
    Quaternionf quaternionOrbitBody = {-0.99997265, -0.0059521, -0.0029033, 0.0032898};
    Vector3f sunECIUnitVector = {1, 0, 0};
    Vector3f magneticField = {1.0e-04 * 0.44848413, -1.0e-04 * 0.02672192, -1.0e-04 * 0.14255624};
    bool firstTime = false;
    bool eclipse = false;
    float currentReactionWheelAngularVelocity = 0;
    float oldReactionWheelAcceleration = 0;

    auto actuatorTorque = nadirPointingMagnetorquerPlusRW.performPointing(quaternionOrbitBody, sunECIUnitVector, state,
                                                                          eclipse, magneticField,
                                                                          firstTime, currentReactionWheelAngularVelocity,
                                                                          oldReactionWheelAcceleration);

    Vector3f magnetorquerTorque = actuatorTorque.col(0);
    Vector3f reactionWheelTorque = actuatorTorque.col(1);

    REQUIRE(magnetorquerTorque(0) == Approx(-1.0e-05 * 0.319375).epsilon(1e-4));
    REQUIRE(magnetorquerTorque(1) == Approx(-1.0e-05 * 0.660535).epsilon(1e-4));
    REQUIRE(magnetorquerTorque(2) == Approx(-1.0e-05 * 0.880944).epsilon(1e-4));

    REQUIRE(reactionWheelTorque(0) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque(1) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque(2) == Approx(1e-04 * 0.138209).epsilon(1e-4));
}

TEST_CASE("Pointing - Sun Pointing Magnetorquer Plus RW") {
    Pointing sunPointingMagnetorquerPlusRW(sunPointing, magnetorquerPlusRW);

    GlobalStateVector state = {0, 0, 0, 1, 0.0539166, 0.05612411, -0.03854784};
    Quaternionf quaternionOrbitBody = {-0.99997265, -0.0059521, -0.0029033, 0.0032898};
    Vector3f sunECIUnitVector = {1, 0, 0};
    Vector3f magneticField = {1.0e-04 * 0.44848413, -1.0e-04 * 0.02672192, -1.0e-04 * 0.14255624};
    bool firstTime = false;
    bool eclipse = false;
    float currentReactionWheelAngularVelocity = 0;
    float oldReactionWheelAcceleration = 0;

    auto actuatorTorque = sunPointingMagnetorquerPlusRW.performPointing(quaternionOrbitBody, sunECIUnitVector, state,
                                                                        eclipse, magneticField,
                                                                        firstTime, currentReactionWheelAngularVelocity,
                                                                        oldReactionWheelAcceleration);

    Vector3f magnetorquerTorque = actuatorTorque.col(0);
    Vector3f reactionWheelTorque = actuatorTorque.col(1);

    REQUIRE(magnetorquerTorque(0) == Approx(-1.0e-05 * 0.32096).epsilon(1e-4));
    REQUIRE(magnetorquerTorque(1) == Approx(-1.0e-05 * 0.68991).epsilon(1e-4));
    REQUIRE(magnetorquerTorque(2) == Approx(-1.0e-05 * 0.88043).epsilon(1e-4));

    REQUIRE(reactionWheelTorque(0) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque(1) == Approx(0).epsilon(1e-4));
    REQUIRE(reactionWheelTorque(2) == Approx(1e-04 * 0.40781).epsilon(1e-4));
}

TEST_CASE("Pointing - Nadir Pointing Magnetorquer Only") {
    Pointing nadirPointingMagnetorquerOnly(nadirPointing, magnetorquerOnly);

    GlobalStateVector state = {0, 0, 0, 1, 0.0539166, 0.05612411, -0.03854784};
    Quaternionf quaternionOrbitBody = {-0.99997265, -0.0059521, -0.0029033, 0.0032898};
    Vector3f sunECIUnitVector = {1, 0, 0};
    Vector3f magneticField = {1.0e-04 * 0.44848413, -1.0e-04 * 0.02672192, -1.0e-04 * 0.14255624};
    bool firstTime = false;
    bool eclipse = false;
    float currentReactionWheelAngularVelocity = 0;
    float oldReactionWheelAcceleration = 0;

    auto actuatorTorque = nadirPointingMagnetorquerOnly.performPointing(quaternionOrbitBody, sunECIUnitVector, state,
                                                                        eclipse, magneticField,
                                                                        firstTime, currentReactionWheelAngularVelocity,
                                                                        oldReactionWheelAcceleration);

    Vector3f magnetorquerTorque = actuatorTorque.col(0);

    REQUIRE(magnetorquerTorque(0) == Approx(1.0e-04 * 0.00970887).epsilon(1e-4));
    REQUIRE(magnetorquerTorque(1) == Approx(-1.0e-04 * 0.10409384).epsilon(1e-4));
    REQUIRE(magnetorquerTorque(2) == Approx(1.0e-04 * 0.05005648).epsilon(1e-4));
}

TEST_CASE("Pointing - Sun Pointing Magnetorquers Only") {
    Pointing sunPointingMagnetorquerOnly(sunPointing, magnetorquerOnly);

    GlobalStateVector state = {0, 0, 0, 1, 0.0539166, 0.05612411, -0.03854784};
    Quaternionf quaternionOrbitBody = {-0.99997265, -0.0059521, -0.0029033, 0.0032898};
    Vector3f sunECIUnitVector = {1, 0, 0};
    Vector3f magneticField = {1.0e-04 * 0.44848413, -1.0e-04 * 0.02672192, -1.0e-04 * 0.14255624};
    bool firstTime = false;
    bool eclipse = false;
    float currentReactionWheelAngularVelocity = 0;
    float oldReactionWheelAcceleration = 0;

    auto actuatorTorque = sunPointingMagnetorquerOnly.performPointing(quaternionOrbitBody, sunECIUnitVector, state,
                                                                      eclipse, magneticField,
                                                                      firstTime, currentReactionWheelAngularVelocity,
                                                                      oldReactionWheelAcceleration);

    Vector3f magnetorquerTorque = actuatorTorque.col(0);

    REQUIRE(magnetorquerTorque(0) == Approx(1.0e-05 * 0.265866).epsilon(1e-4));
    REQUIRE(magnetorquerTorque(1) == Approx(-1.0e-05 * 0.41702).epsilon(1e-4));
    REQUIRE(magnetorquerTorque(2) == Approx(1.0e-05 * 0.914588).epsilon(1e-4));
}