#include <catch2/catch.hpp>
#include "Bdot.h"
#include "Eigen/Geometry"

using namespace Eigen;

TEST_CASE("Max desired magnetic dipole") {
    Vector3f magneticFieldBody3 = {-0.123e-04, 0.175e-04, -0.048e-4};
    Vector3f magneticFieldBody2 = {-0.1282e-04, 0.1732e-04, -0.0887e-4};
    Vector3f magneticFieldBody1 = {1.0e-04 * (0.2759), 1.0e-04 * (-0.2003), 1.0e-04 * (0.3259)};
    Vector3f maxMagneticDipole = {0.2, 0.2, 0.2};
    float timestep = 0.1;
    auto Kp = Matrix<float, 3, 3>::Identity(3, 3);

    Bdot bdot(magneticFieldBody1, Kp, maxMagneticDipole, timestep);

    Vector3f magneticDipole = bdot.controller(magneticFieldBody2);
    REQUIRE(magneticDipole(0) == Approx(0.2).epsilon(1e-3));
    REQUIRE(magneticDipole(1) == Approx(-0.2).epsilon(1e-3));
    REQUIRE(magneticDipole(2) == Approx(0.2).epsilon(1e-3));

    Vector3f magneticFieldBody = bdot.getMagneticFieldBody();
    REQUIRE(magneticFieldBody(0) == Approx(-0.1282e-04).epsilon(1e-3));
    REQUIRE(magneticFieldBody(1) == Approx(0.1732e-04).epsilon(1e-3));
    REQUIRE(magneticFieldBody(2) == Approx(-0.0887e-4).epsilon(1e-3));

    bdot.controller(magneticFieldBody3);
    magneticFieldBody = bdot.getMagneticFieldBody();
    REQUIRE(magneticFieldBody(0) == Approx(-0.123e-04).epsilon(1e-3));
    REQUIRE(magneticFieldBody(1) == Approx(0.175e-04).epsilon(1e-3));
    REQUIRE(magneticFieldBody(2) == Approx(-0.048e-4).epsilon(1e-3));
}

TEST_CASE("Less than max desired magnetic dipole") {
    Vector3f magneticFieldBody3 = {1.0e-04 * (-0.08852), 1.0e-04 * (0.048456), 1.0e-04 * (0.242746)};
    Vector3f magneticFieldBody2 = {1.0e-04 * (-0.0871098), 1.0e-04 * (0.0483917), 1.0e-04 * (-0.2420372)};
    Vector3f magneticFieldBody1 = {1.0e-04 * (0.0783222), 1.0e-04 * (0.0468520), 1.0e-04 * (0.4403655)};
    Vector3f maxMagneticDipole = {0.2, 0.2, 0.2};
    float timestep = 0.1;
    auto Kp = Matrix<float, 3, 3>::Identity(3, 3);

    Bdot bdot(magneticFieldBody1, Kp, maxMagneticDipole, timestep);

    Vector3f magneticDipole = bdot.controller(magneticFieldBody2);

    REQUIRE(magneticDipole(0) == Approx(0.2).epsilon(1e-3));
    REQUIRE(magneticDipole(1) == Approx(-0.034236).epsilon(1e-3));
    REQUIRE(magneticDipole(2) == Approx(0.2).epsilon(1e-3));

    Vector3f magneticFieldBody = bdot.getMagneticFieldBody();
    REQUIRE(magneticFieldBody(0) == Approx(1.0e-04 * (-0.0871098)).epsilon(1e-3));
    REQUIRE(magneticFieldBody(1) == Approx(1.0e-04 * (0.0483917)).epsilon(1e-3));
    REQUIRE(magneticFieldBody(2) == Approx(1.0e-04 * (-0.2420372)).epsilon(1e-3));

    bdot.controller(magneticFieldBody3);
    magneticFieldBody = bdot.getMagneticFieldBody();
    REQUIRE(magneticFieldBody(0) == Approx(1.0e-04 * (-0.08852)).epsilon(1e-3));
    REQUIRE(magneticFieldBody(1) == Approx(1.0e-04 * (0.048456)).epsilon(1e-3));
    REQUIRE(magneticFieldBody(2) == Approx(1.0e-04 * (0.242746)).epsilon(1e-3));
}

TEST_CASE("Magnetorquer scaling") {
    Vector3f magneticFieldBody2 = {1.0e-04 * (-0.0871098), 1.0e-04 * (0.0483917), 1.0e-04 * (-0.2420372)};
    Vector3f magneticFieldBody1 = {1.0e-04 * (0.0783222), 1.0e-04 * (0.0468520), 1.0e-04 * (0.4403655)};
    Vector3f maxMagneticDipole = {0.2, 0.2, 0.2};
    float timestep = 0.1;
    auto Kp = Matrix<float, 3, 3>::Identity(3, 3);

    Bdot bdot(magneticFieldBody1, Kp, maxMagneticDipole, timestep);


    Vector3f regularDesiredMagneticDipole = {0.12345, 0.12345, 0.12345};
    Vector3f magneticDipole = bdot.magnetorquerScaling(regularDesiredMagneticDipole);

    REQUIRE(magneticDipole(0) == Approx(0.12345).epsilon(1e-3));
    REQUIRE(magneticDipole(1) == Approx(0.12345).epsilon(1e-3));
    REQUIRE(magneticDipole(2) == Approx(0.12345).epsilon(1e-3));

    Vector3f negativeMaxDesiredMagneticDipole = {-2.12345, -2.12345, -2.12345};
    magneticDipole = bdot.magnetorquerScaling(negativeMaxDesiredMagneticDipole);

    REQUIRE(magneticDipole(0) == Approx(-0.2).epsilon(1e-3));
    REQUIRE(magneticDipole(1) == Approx(-0.2).epsilon(1e-3));
    REQUIRE(magneticDipole(2) == Approx(-0.2).epsilon(1e-3));

    Vector3f positiveMaxDesiredMagneticDipole = {2.12345, 2.12345, 2.12345};
    magneticDipole = bdot.magnetorquerScaling(positiveMaxDesiredMagneticDipole);

    REQUIRE(magneticDipole(0) == Approx(0.2).epsilon(1e-3));
    REQUIRE(magneticDipole(1) == Approx(0.2).epsilon(1e-3));
    REQUIRE(magneticDipole(2) == Approx(0.2).epsilon(1e-3));

    Vector3f mixedMaxDesiredMagneticDipole = {2.12345, -2.12345, 2.12345};
    magneticDipole = bdot.magnetorquerScaling(mixedMaxDesiredMagneticDipole);

    REQUIRE(magneticDipole(0) == Approx(0.2).epsilon(1e-3));
    REQUIRE(magneticDipole(1) == Approx(-0.2).epsilon(1e-3));
    REQUIRE(magneticDipole(2) == Approx(0.2).epsilon(1e-3));
}