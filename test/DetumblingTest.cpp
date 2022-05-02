#include <catch2/catch.hpp>
#include "Detumbling.hpp"

using namespace Eigen;

TEST_CASE("Torque Test") {

    Vector3f magneticFieldBody1 = {-1.44505468032163, 1.77939963990674, -3.95126047327029};

    magneticFieldBody1[0] /= 100000;
    magneticFieldBody1[1] /= 100000;
    magneticFieldBody1[2] /= 1000000;

    Vector3f magneticFieldBody2 = {-1.37815279474949, 1.83322872033822, -4.11412598905325};

    magneticFieldBody2[0] /= 100000;
    magneticFieldBody2[1] /= 100000;
    magneticFieldBody2[2] /= 1000000;

    Bdot bDot({0, 0, 0});

    Vector3f magneticTorque = getMagneticTorque(bDot, magneticFieldBody1, magneticFieldBody2);

    REQUIRE(magneticTorque[0] * 10000000 == Approx(-4.55641919081183).epsilon(1e-3));
    REQUIRE(magneticTorque[1] * 1000000 == Approx(-1.80204558688336).epsilon(1e-3));
    REQUIRE(magneticTorque[2] * 1000000 == Approx(-6.44890864045674).epsilon(1e-3));
}

TEST_CASE("Angular Velocity") {
    Vector3f magneticFieldBody1 = {1.52403644509235, 1.76439571689881, -1.13221000566922};

    magneticFieldBody1[0] /= 100000;
    magneticFieldBody1[1] /= 100000;
    magneticFieldBody1[2] /= 1000000;

    Vector3f magneticFieldBody2 = {1.60088168745433, 1.69615650217286, -8.91850656074703};

    magneticFieldBody2[0] /= 100000;
    magneticFieldBody2[1] /= 100000;
    magneticFieldBody2[2] /= 10000000;

    Bdot bDot({0, 0, 0});

    Vector3f magneticTorque = getMagneticTorque(bDot, magneticFieldBody1, magneticFieldBody2);
    Vector3f angularVelocity = getAngularVelocityEstimation(bDot);

    REQUIRE(angularVelocity[0] == Approx(-0.0636545152441620).epsilon(1e-3));
    REQUIRE(angularVelocity[1] == Approx(0.0831996293001571).epsilon(1e-3));
    REQUIRE(angularVelocity[2] == Approx(0.439717616066728).epsilon(1e-3));


}