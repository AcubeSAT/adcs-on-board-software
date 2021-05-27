#include <catch2/catch.hpp>
#include "Albedo.h"

using namespace albedo;

TEST_CASE("Calculate cell area test") {
    uint16_t i = 40;
    uint16_t j = 50;

    float cellArea = calculateCellArea(i, j);
    REQUIRE(cellArea == Approx(1.003737674791411e+10).epsilon(0.01));
}