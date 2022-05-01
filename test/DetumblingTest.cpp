#include <catch2/catch.hpp>
#include "Detumbling.hpp"

using namespace Eigen;

TEST_CASE("Torque Test"){

    Vector3f magneticFieldBody1 ={-1.44505468032163,1.77939963990674,-3.95126047327029};


    magneticFieldBody1[0] /= 100000;
    magneticFieldBody1[1] /= 100000;
    magneticFieldBody1[2] /= 1000000;

    Vector3f magneticFieldBody2 = {-1.37815279474949,1.83322872033822, -4.11412598905325};

    magneticFieldBody2[0] /= 100000;
    magneticFieldBody2[1] /= 100000;
    magneticFieldBody2[2] /= 1000000;

    Bdot bDot({0, 0, 0});



    Vector3f magneticTorque = getMagneticTorque(bDot, magneticFieldBody1, magneticFieldBody2);


    REQUIRE(magneticTorque[0]*10000000 == Approx(-4.55641919081183).epsilon(1e-3));
    REQUIRE(magneticTorque[1]*1000000 == Approx(-1.80204558688336).epsilon(1e-3));
    REQUIRE(magneticTorque[2]*1000000 == Approx(-6.44890864045674).epsilon(1e-3));
}