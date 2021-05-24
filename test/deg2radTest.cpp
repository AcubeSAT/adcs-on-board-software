#include <catch2/catch.hpp>
#include "mathFunctions.h"

TEST_CASE("Degrees to rads test") {
    REQUIRE(deg2rad(0) == Approx(0).epsilon(0.01));
    REQUIRE(deg2rad(180) == Approx(3.1416).epsilon(0.01));
    REQUIRE(deg2rad(360) == Approx(2 * 3.1416).epsilon(0.01));
    REQUIRE(deg2rad(83) == Approx(1.4486).epsilon(0.01));
    REQUIRE(deg2rad(274) == Approx(4.7822).epsilon(0.01));
    REQUIRE(deg2rad(420) == Approx(7.3304).epsilon(0.01));
    REQUIRE(deg2rad(-69) == Approx(-1.2043).epsilon(0.01));
}