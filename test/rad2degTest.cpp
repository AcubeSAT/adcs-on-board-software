#include <catch2/catch.hpp>
#include "mathFunctions.h"

TEST_CASE("Rads to degrees test") {
    REQUIRE(rad2deg(0) == Approx(0).epsilon(0.01));
    REQUIRE(rad2deg(3.14) == Approx(179.9087).epsilon(0.01));
    REQUIRE(rad2deg(6.28) == Approx(359.8175).epsilon(0.01));
    REQUIRE(rad2deg(1) == Approx(57.2958).epsilon(0.01));
    REQUIRE(rad2deg(2.5) == Approx(143.2394).epsilon(0.01));
    REQUIRE(rad2deg(4.2) == Approx(240.6423).epsilon(0.01));
    REQUIRE(rad2deg(-0.69) == Approx(-39.5341).epsilon(0.01));
}