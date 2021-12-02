#include <catch2/catch.hpp>
#include "MathFunctions.hpp"

using namespace Eigen;

TEST_CASE("Vector turn with quaternion test") {

    Eigen::Quaternionf identityQuaternion = {1, 0, 0, 0};
    Vector3f randomVector = {0.1, 0.2, 0.3};
    Vector3f rotatedVector1 = rotateVector(identityQuaternion, randomVector);

    REQUIRE(rotatedVector1(0) == Approx(0.1).epsilon(0.01));
    REQUIRE(rotatedVector1(1) == Approx(0.2).epsilon(0.01));
    REQUIRE(rotatedVector1(2) == Approx(0.3).epsilon(0.01));

    Eigen::Quaternionf randomQuaternion = {0.1231, 0.6155, 0.7385, 0.2462};
    Vector3f lowValuesVector = {1e-07, 1e-07, 1e-07};
    Vector3f rotatedVector2 = rotateVector(randomQuaternion, lowValuesVector);

    REQUIRE(rotatedVector2(0) == Approx(1.0e-06 * 0.0879).epsilon(0.01));
    REQUIRE(rotatedVector2(1) == Approx(1.0e-06 * 0.1485).epsilon(0.01));
    REQUIRE(rotatedVector2(2) == Approx(1.0e-06 * (-0.0151)).epsilon(0.01));

    Vector3f highValuesVector = {1e+03, 1e+03, 1e+03};
    Vector3f rotatedVector3 = rotateVector(randomQuaternion, highValuesVector);

    REQUIRE(rotatedVector3(0) == Approx(1.0e+03 * 0.8789).epsilon(0.01));
    REQUIRE(rotatedVector3(1) == Approx(1.0e+03 * 1.4847).epsilon(0.01));
    REQUIRE(rotatedVector3(2) == Approx(1.0e+03 * (-0.1514)).epsilon(0.01));
}
