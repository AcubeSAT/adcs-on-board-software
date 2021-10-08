#include <catch2/catch.hpp>
#include "mathFunctions.h"

using namespace Eigen;

TEST_CASE("Cartesian to spherical test") {

    Vector3f vectorCartesian1 = {0.1, 0.2, 0.3};
    Vector3f vectorSpherical1 = cartesianToSpherical(vectorCartesian1);

    REQUIRE(vectorSpherical1(0) == Approx(1.1071).epsilon(0.01));
    REQUIRE(vectorSpherical1(1) == Approx(0.9303).epsilon(0.01));
    REQUIRE(vectorSpherical1(2) == Approx(0.3742).epsilon(0.01));

    Vector3f vectorCartesian2 = {100000000, 200000000, 300000000};
    Vector3f vectorSpherical2 = cartesianToSpherical(vectorCartesian2);

    REQUIRE(vectorSpherical2(0) == Approx(1.1071).epsilon(0.01));
    REQUIRE(vectorSpherical2(2) == Approx(3.7417e+08).epsilon(0.01));
    REQUIRE(vectorSpherical2(1) == Approx(0.9303).epsilon(0.01));

    Vector3f vectorCartesian3 = {5000000, 4000000, 8000000};
    Vector3f vectorSpherical3 = cartesianToSpherical(vectorCartesian3);

    REQUIRE(vectorSpherical3(0) == Approx(0.674740942223553).epsilon(0.01));
    REQUIRE(vectorSpherical3(1) == Approx(0.895817302077988).epsilon(0.01));
    REQUIRE(vectorSpherical3(2) == Approx(1.024695076595960e+07).epsilon(0.01));

    Vector3f zerosCartesian = {0, 0, 0};
    Vector3f zerosSpherical = cartesianToSpherical(zerosCartesian);

    REQUIRE(zerosSpherical(0) == Approx(0).epsilon(0.01));
    REQUIRE(zerosSpherical(1) == Approx(0).epsilon(0.01));
    REQUIRE(zerosSpherical(2) == Approx(0).epsilon(0.01));

    Vector3f negativesCartesian = {-0.5689, -0.457, -2.568};
    Vector3f negativesSpherical = cartesianToSpherical(negativesCartesian);

    REQUIRE(negativesSpherical(0) == Approx(-2.464839943079727).epsilon(0.01));
    REQUIRE(negativesSpherical(1) == Approx(-1.293934058318156).epsilon(0.01));
    REQUIRE(negativesSpherical(2) == Approx(2.669666685187497).epsilon(0.01));
}
