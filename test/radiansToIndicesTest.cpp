#include <catch2/catch.hpp>
#include "Albedo.h"

using namespace albedo;

TEST_CASE("Radians to indices test") {
    Vector<int16_t, 2> indices1 = radiansToIndices(20, 40);
    REQUIRE(indices1(0) == Approx(-2110).epsilon(0.01));
    REQUIRE(indices1(1) == Approx(1060).epsilon(0.01));

    Vector<int16_t, 2> indices2 = radiansToIndices(1.0e+06 * 0.000000674740942,
                                                   1.0e+06 * 0.000001446501385);
    REQUIRE(indices2(0) == Approx(97).epsilon(0.01));
    REQUIRE(indices2(1) == Approx(174).epsilon(0.01));
}
