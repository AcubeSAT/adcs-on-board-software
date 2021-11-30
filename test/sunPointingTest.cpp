#include <catch2/catch.hpp>
#include "SunPointing.h"

using namespace Eigen;

const Quaternionf desiredQuaternion = {1, 0, 0, 0};
const Vector3f angularVelocityECIOrbit = {0, 0.0011, 0};

const auto Kp = Matrix<float, 3, 3>::Identity(3, 3);
const auto Kd = Matrix<float, 3, 3>::Identity(3, 3);

const SunPointing sunPointing(Kp, Kd, desiredQuaternion, angularVelocityECIOrbit);

TEST_CASE("Sun Pointing - Commanded Torque") {
    GlobalStateVector state1 = {1, 0, 0, 0, 0, 0, 0};
    Quaternionf quaternionOrbitBody = {0, 0, 0, 1};
    Vector3f sunECIUnitVector1 = {1, 1, 0};
    bool eclipse = false;

    Vector3f torque1 = sunPointing.calculateTorque(quaternionOrbitBody, sunECIUnitVector1, state1, eclipse);

    Vector3f sunECIUnitVector2 = {-0.175103, 0.918866, 0.398319};
    GlobalStateVector state2 = {0.4467105, -0.1190812, 0.8865248, -0.0193485, 0.0539166, 0.0561241, -0.0385478};
    Vector3f torque2 = sunPointing.calculateTorque(quaternionOrbitBody, sunECIUnitVector2, state2, eclipse);

    Vector3f sunECIUnitVector3 = {-0.175106, 0.918866, 0.398319};
    GlobalStateVector state3 = {0.454602, -0.147243, 0.878224, -0.019436, -0.004414, -0.003641, -0.010033};
    Vector3f torque3 = sunPointing.calculateTorque(quaternionOrbitBody, sunECIUnitVector3, state3, eclipse);

    REQUIRE(torque1(0) == Approx(0).epsilon(1e-4));
    REQUIRE(torque1(1) == Approx(0).epsilon(1e-4));
    REQUIRE(torque1(2) == Approx(-sqrt(2) / 2).epsilon(1e-4));

    REQUIRE(torque2(0) == Approx(-0.16536).epsilon(1e-4));
    REQUIRE(torque2(1) == Approx(-0.16757).epsilon(1e-4));
    REQUIRE(torque2(2) == Approx(-0.12086).epsilon(1e-4));

    REQUIRE(torque3(0) == Approx(-0.09577).epsilon(1e-4));
    REQUIRE(torque3(1) == Approx(-0.09654).epsilon(1e-4));
    REQUIRE(torque3(2) == Approx(-0.12438).epsilon(1e-4));
}

TEST_CASE("Sun Pointing - Quaternion Sun Body Calculation") {
    Quaternionf quaternionECIBody1 = {1, 0, 0, 0};
    Vector3f sunECIUnitVector1 = {1, 1, 0};

    Quaternionf quaternionSunBody1 = sunPointing.calculateQuaternionSunBody(sunECIUnitVector1, quaternionECIBody1);

    Vector3f sunECIUnitVector2 = {-0.175106, 0.918866, 0.398319};
    Quaternionf quaternionECIBody2 = {0.454602, -0.147243, 0.878224, -0.019436};
    Quaternionf quaternionSunBody2 = sunPointing.calculateQuaternionSunBody(sunECIUnitVector2, quaternionECIBody2);

    Vector3f sunECIUnitVector3 = {-0.175107, 0.918866, 0.398319};
    Quaternionf quaternionECIBody3 = {0.455966, -0.171547, 0.872775, -0.030477};
    Quaternionf quaternionSunBody3 = sunPointing.calculateQuaternionSunBody(sunECIUnitVector3, quaternionECIBody3);

    REQUIRE(quaternionSunBody1.w() == Approx(sqrt(2) / 2).epsilon(1e-4));
    REQUIRE(quaternionSunBody1.x() == Approx(0).epsilon(1e-4));
    REQUIRE(quaternionSunBody1.y() == Approx(0).epsilon(1e-4));
    REQUIRE(quaternionSunBody1.z() == Approx(sqrt(2) / 2).epsilon(1e-4));

    REQUIRE(quaternionSunBody2.w() == Approx(0.98074).epsilon(1e-4));
    REQUIRE(quaternionSunBody2.x() == Approx(0.10019).epsilon(1e-4));
    REQUIRE(quaternionSunBody2.y() == Approx(0.10019).epsilon(1e-4));
    REQUIRE(quaternionSunBody2.z() == Approx(0.13441).epsilon(1e-4));

    REQUIRE(quaternionSunBody3.w() == Approx(0.98438).epsilon(1e-4));
    REQUIRE(quaternionSunBody3.x() == Approx(0.09831).epsilon(1e-4));
    REQUIRE(quaternionSunBody3.y() == Approx(0.09831).epsilon(1e-4));
    REQUIRE(quaternionSunBody3.z() == Approx(0.10795).epsilon(1e-4));
}