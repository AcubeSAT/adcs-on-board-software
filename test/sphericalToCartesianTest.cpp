#include <catch2/catch.hpp>
#include "mathFunctions.h"

TEST_CASE("Spherical to cartesian test") {

    Vector3f vectorSpherical = {0.1, 0.2, 0.3};
    Vector3f vectorCartesian = sphericalToCartesian(vectorSpherical);

    REQUIRE(vectorCartesian(0) == Approx(0.2926).epsilon(0.01));
    REQUIRE(vectorCartesian(1) == Approx(0.0294).epsilon(0.01));
    REQUIRE(vectorCartesian(2) == Approx(0.0596).epsilon(0.01));

    Vector3f zerosSpherical = {0, 0, 0};
    Vector3f zerosCartesian = sphericalToCartesian(zerosSpherical);

    REQUIRE(zerosCartesian(0) == Approx(0).epsilon(0.01));
    REQUIRE(zerosCartesian(1) == Approx(0).epsilon(0.01));
    REQUIRE(zerosCartesian(2) == Approx(0).epsilon(0.01));
}