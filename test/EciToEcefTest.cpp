#include <catch2/catch.hpp>
#include "EciToEcef.h"
#include <cmath>


using namespace Eigen;

TEST_CASE("EciToEcef Test1")
{
    Vector3f vec_eci = {-6733.04064617925, -1382.68992247076, -14.6876088752119};
    Vector3f v_vec_eci = {-0.204369348258943, 0.955656300804921, 7.55440290980165};
    double gstime = 4.883067536994346;
    tow_vectors vec_vvec = eci_to_ecef(vec_eci, v_vec_eci, gstime);

    REQUIRE(vec_vvec.vec_one[0] == Approx(218.984818755685));
    REQUIRE(vec_vvec.vec_one[1] == Approx(-6870.05920017860));
    REQUIRE(vec_vvec.vec_one[2] == Approx(-14.6876088752119));

    REQUIRE(vec_vvec.vec_two[0] == Approx(-1.47745539858590));
    REQUIRE(vec_vvec.vec_two[1] == Approx(-0.0550491755385577));
    REQUIRE(vec_vvec.vec_two[2] == Approx(7.55440290980165));

}

TEST_CASE("EciToEcef Test2")
{
    Vector3f vec_eci = {-2.3, 0, 1212};
    Vector3f v_vec_eci = {3.145353534, -2, 333};
    double gstime = 3.3453224;
    tow_vectors vec_vvec = eci_to_ecef(vec_eci, v_vec_eci, gstime);

    REQUIRE(vec_vvec.vec_one[0] == Approx(2.25243318591924));
    REQUIRE(vec_vvec.vec_one[1] == Approx(-0.465343682636487));
    REQUIRE(vec_vvec.vec_one[2] == Approx(1212));

    REQUIRE(vec_vvec.vec_two[0] == Approx(-2.67569102738179333));
    REQUIRE(vec_vvec.vec_two[1] == Approx(2.59485173628578));
    REQUIRE(vec_vvec.vec_two[2] == Approx(333));

}

TEST_CASE("EciToEcef Test3")
{
    Vector3f vec_eci = {-1234, -74329.123, 0.1432};
    Vector3f v_vec_eci = {3.1415, 0, 2.222121312};
    double gstime = 12234909;
    tow_vectors vec_vvec = eci_to_ecef(vec_eci, v_vec_eci, gstime);

    REQUIRE(vec_vvec.vec_one[0] == Approx(-74334.9817031508));
    REQUIRE(vec_vvec.vec_one[1] == Approx(-807.327158822088));
    REQUIRE(vec_vvec.vec_one[2] == Approx(0.143200000000000));

    REQUIRE(vec_vvec.vec_two[0] == Approx(0.0273852375694303));
    REQUIRE(vec_vvec.vec_two[1] == Approx(2.28027738968416));
    REQUIRE(vec_vvec.vec_two[2] == Approx(2.22212131200000));

}