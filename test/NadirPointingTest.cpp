#include <catch2/catch.hpp>
#include "NadirPointing.hpp"
#include "Parameters.hpp"

using namespace Eigen;

const NadirPointing nadirPointing(Parameters::NadirPointingPlusRW::Kp, Parameters::NadirPointingPlusRW::Kd);

TEST_CASE("Nadir pointing - Commanded Torque") {
    GlobalStateVector state;
    Quaternionf quaternionOrbitBody;
    Vector3f sunECIUnitVector = {1, 0, 0};
    bool eclipse = false;

    quaternionOrbitBody = {0, 0, 0, 1};
    state = {0, 0, 0, 1, 0, -0.0011, 0};
    Vector3f torque1 = nadirPointing.calculateTorque(quaternionOrbitBody, sunECIUnitVector, state, eclipse);

    quaternionOrbitBody = {0, 0, 1, 0};
    state = {0, 0, 0, 1, 0, 0.0011, 0};
    Vector3f torque2 = nadirPointing.calculateTorque(quaternionOrbitBody, sunECIUnitVector, state, eclipse);

    quaternionOrbitBody = {0, 1, 0, 0};
    state = {0, 0, 0, 1, 0, -0.0011, 0};
    Vector3f torque3 = nadirPointing.calculateTorque(quaternionOrbitBody, sunECIUnitVector, state, eclipse);

    quaternionOrbitBody = {0, 1, 0, 0};
    state = {0, 0, 0, 1, 0, -0.0011, 0};
    eclipse = true;
    Vector3f torque4 = nadirPointing.calculateTorque(quaternionOrbitBody, sunECIUnitVector, state, eclipse);

    REQUIRE(torque1(0) == Approx(0).epsilon(1e-4));
    REQUIRE(torque1(1) == Approx(0).epsilon(1e-4));
    REQUIRE(torque1(2) == Approx(-1).epsilon(1e-4));

    REQUIRE(torque2(0) == Approx(0).epsilon(1e-4));
    REQUIRE(torque2(1) == Approx(-1).epsilon(1e-4));
    REQUIRE(torque2(2) == Approx(0).epsilon(1e-4));

    REQUIRE(torque3(0) == Approx(-1).epsilon(1e-4));
    REQUIRE(torque3(1) == Approx(0).epsilon(1e-4));
    REQUIRE(torque3(2) == Approx(0).epsilon(1e-4));

    REQUIRE(torque4(0) == Approx(-1.2).epsilon(1e-4));
    REQUIRE(torque4(1) == Approx(0).epsilon(1e-4));
    REQUIRE(torque4(2) == Approx(0).epsilon(1e-4));
}