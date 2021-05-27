#include <catch2/catch.hpp>
#include "Albedo.h"

using namespace albedo;

TEST_CASE("Calculate grid area test") {

    uint16_t loopI = 78;
    uint16_t loopJ = 24;
    int8_t sunIndexI = 56;
    int8_t sunIndexJ = 95;

    float gridAngleResult = gridAngle(loopI, loopJ, sunIndexI, sunIndexJ);
    REQUIRE(gridAngleResult == Approx(1.442580854116471).epsilon(0.01));
}