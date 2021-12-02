#include <catch2/catch.hpp>
#include "MathFunctions.hpp"

using namespace Eigen;

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

    Vector3f negativeSpherical = {-0.5689, -0.457, -2.568};
    Vector3f negativeCartesian = sphericalToCartesian(negativeSpherical);

    REQUIRE(negativeCartesian(0) == Approx(-1.941504504351219).epsilon(0.01));
    REQUIRE(negativeCartesian(1) == Approx(1.241432383097179).epsilon(0.01));
    REQUIRE(negativeCartesian(2) == Approx(1.133150430340820).epsilon(0.01));
}