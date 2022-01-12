#include <catch2/catch.hpp>
#include "SunPosition.hpp"
#include <iostream>

using namespace Eigen;


TEST_CASE("Sun Position Test1")

{
    double time = 2460127.50000000;
    Vector3d sunpos = sun_position(time);
    REQUIRE(sunpos(0) == Approx(-0.174717604923527));
    REQUIRE(sunpos(1) == Approx(0.918928406982493));
    REQUIRE(sunpos(2) == Approx(0.398346002571225));
}
TEST_CASE("Sun Position Test2")

{
    double time = 2.4601;
    Vector3d sunpos = sun_position(time);
    REQUIRE(sunpos(0) == Approx(-0.407105226684744));
    REQUIRE(sunpos(1) == Approx(-0.828752263583653));
    REQUIRE(sunpos(2) == Approx(-0.374407193426776));
}