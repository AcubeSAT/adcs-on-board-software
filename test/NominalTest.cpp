#include <catch2/catch.hpp>
#include "Nominal.hpp"

using namespace Eigen;

TEST_CASE("First part of Nominal") {
    GlobalStateVector outputState = nominalMode(2);
    GlobalStateVector expectedState;
    expectedState
            << 0.44928, -0.11889, 0.88536, -0.0122, 0.1, 0.2, 0.3;
    for (int i = 0; i < 4; i++) {
        REQUIRE(outputState(i) == Approx(expectedState(i)).epsilon(0.01));
    }
}