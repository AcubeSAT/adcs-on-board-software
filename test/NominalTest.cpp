#include <catch2/catch.hpp>
#include "Nominal.hpp"

using namespace Eigen;

TEST_CASE("First part of Nominal") {
    GlobalStateVector outputState = NominalMode(2);
//    GlobalStateVector expectedState = {0.2235, -0.25593, 0.9401, -0.02559, 0, 0, 0};
//
//    for (int i = 0; i < 4; i++) {
//        REQUIRE(outputState(i) == Approx(expectedState(i)).epsilon(0.01));
//    }
}