#include <catch2/catch.hpp>
#include "Albedo.h"

using namespace albedo;

TEST_CASE("Indices to radians test") {
    uint16_t i = 40;
    uint16_t j = 50;

    Vector2f radians = albedo::indicesToRadians(i, j);
    REQUIRE(radians(0) == Approx(-2.039853563268373).epsilon(0.01));
    REQUIRE(radians(1) == Approx(2.434734306532090).epsilon(0.01));
}