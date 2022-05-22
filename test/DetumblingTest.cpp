#include <catch2/catch.hpp>
#include "Detumbling.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

TEST_CASE("Torque Test") {
    Vector3f magneticFieldBody1 = {-1.44505468032163e-5, 1.77939963990674e-5, -3.95126047327029e-6};

    Vector3f magneticFieldBody2 = {-1.37815279474949e-5, 1.83322872033822e-5, -4.11412598905325e-6};

    Bdot bDot(magneticFieldBody1, magneticFieldBody2);

    Vector3f magneticTorque = calculateDesiredMagneticTorque(bDot);

    REQUIRE(magneticTorque[0] == Approx(-4.55641919081183e-7).epsilon(1e-3));
    REQUIRE(magneticTorque[1] == Approx(-1.80204558688336e-6).epsilon(1e-3));
    REQUIRE(magneticTorque[2] == Approx(-6.44890864045674e-6).epsilon(1e-3));
}

TEST_CASE("Angular Velocity Test") {
    Vector3f magneticFieldBody1 = {1.52403644509235e-5, 1.76439571689881e-5, -1.13221000566922e-6};

    Vector3f magneticFieldBody2 = {1.60088168745433e-5, 1.69615650217286e-5, -8.91850656074703e-7};

    Bdot bDot(magneticFieldBody1, magneticFieldBody2);

    Vector3f magneticTorque = calculateDesiredMagneticTorque(bDot);
    Vector3f angularVelocity = estimateAngularVelocity(bDot.getBDotVector(), bDot.getSamplingEndMagneticFieldBody());

    REQUIRE(angularVelocity[0] == Approx(-0.0636545152441620).epsilon(1e-3));
    REQUIRE(angularVelocity[1] == Approx(0.0831996293001571).epsilon(1e-3));
    REQUIRE(angularVelocity[2] == Approx(0.439717616066728).epsilon(1e-3));
}

TEST_CASE("Detumbling Case 1") {
    Bdot bdot({0, 0, 0}, {0, 0, 0});
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData = Matrix<float, 180, 288>::Identity() * 100000;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData);

    Quaternionf q_body_eci({0.3757, 0.5983, -0.2364, -0.6671});
    Vector3f omega = {PI / 30, PI / 30, PI / 30};
    Vector3f omega_dot = {0, 0, 0};

    bool isDone = false;
    int stableTime = 0;
    while (stableTime < 10) {
        Detumbling(bdot, q_body_eci, omega, em, isDone);
        if (isDone) {
            stableTime++;
        }
    }

    REQUIRE(!(stableTime < 10));
}


TEST_CASE("Detumbling Case 2") {
    Bdot bdot({0, 0, 0}, {0, 0, 0});
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData = Matrix<float, 180, 288>::Identity() * 100000;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData);

    Quaternionf q_body_eci({0.71772, 0.449563, 0.177631, 0.501259});
    Vector3f omega = {PI / 3, PI / 3, PI / 3};
    Vector3f omega_dot = {0, 0, 0};

    bool isDone = false;
    int stableTime = 0;
    while (stableTime < 10) {
        Detumbling(bdot, q_body_eci, omega, em, isDone);
        if (isDone) {
            stableTime++;
        }
    }

    REQUIRE(!(stableTime < 10));

}