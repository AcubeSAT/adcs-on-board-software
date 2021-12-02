#include <catch2/catch.hpp>
#include "MathFunctions.hpp"

using namespace Eigen;

TEST_CASE("Skew matrix test") {
    Vector3f w = {0.4, 0.5, -0.5};

    Matrix<float, 3, 3> matrix = skew(w);

    REQUIRE(matrix(0, 0) == Approx(0).epsilon(0.01));
    REQUIRE(matrix(0, 1) == Approx(0.5).epsilon(0.01));
    REQUIRE(matrix(0, 2) == Approx(0.5).epsilon(0.01));
    REQUIRE(matrix(1, 0) == Approx(-0.5).epsilon(0.01));
    REQUIRE(matrix(1, 1) == Approx(0).epsilon(0.01));
    REQUIRE(matrix(1, 2) == Approx(-0.4).epsilon(0.01));
    REQUIRE(matrix(2, 0) == Approx(-0.5).epsilon(0.01));
    REQUIRE(matrix(2, 1) == Approx(0.4).epsilon(0.01));
    REQUIRE(matrix(2, 2) == Approx(0).epsilon(0.01));
}

