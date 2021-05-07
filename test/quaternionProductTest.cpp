#include <catch2/catch.hpp>
#include "mathFunctions.h"

using namespace Eigen;

TEST_CASE("Quaternion multiplication test") {
    Quaternionf q1 = {0.5, 0.5, -0.5, -0.5};
    Quaternionf q2 = {0.5, 0.5, 0.5, 0.5};
    Quaternionf prod = quaternionProduct(q1, q2);

    REQUIRE(prod.w() == Approx(0.5).epsilon(0.01));
    REQUIRE(prod.x() == Approx(0.5).epsilon(0.01));
    REQUIRE(prod.y() == Approx(-0.5).epsilon(0.01));
    REQUIRE(prod.z() == Approx(0.5).epsilon(0.01));

    Quaternionf identityQuaternion1 = {1, 0, 0, 0};
    Quaternionf identityQuaternion2 = {1, 0, 0, 0};
    Quaternionf identityQuaternionProduct = quaternionProduct(identityQuaternion1, identityQuaternion2);

    REQUIRE(identityQuaternionProduct.w() == Approx(1).epsilon(0.01));
    REQUIRE(identityQuaternionProduct.x() == Approx(0).epsilon(0.01));
    REQUIRE(identityQuaternionProduct.y() == Approx(0).epsilon(0.01));
    REQUIRE(identityQuaternionProduct.z() == Approx(0).epsilon(0.01));


    Quaternionf identityQuaternion = {1, 0, 0, 0};
    Quaternionf random_quaternion = {0.1231, 0.6155, 0.7385, 0.2462};
    Quaternionf quaternionProduct1 = quaternionProduct(identityQuaternion, random_quaternion);


    REQUIRE(quaternionProduct1.w() == Approx(0.1231).epsilon(0.01));
    REQUIRE(quaternionProduct1.x() == Approx(0.6155).epsilon(0.01));
    REQUIRE(quaternionProduct1.y() == Approx(0.7385).epsilon(0.01));
    REQUIRE(quaternionProduct1.z() == Approx(0.2462).epsilon(0.01));

    Quaternionf negativeRandomQuaternion = {-0.1231, -0.6155, -0.7385, -0.2462};
    Quaternionf quaternionProduct2 = quaternionProduct(identityQuaternion, negativeRandomQuaternion);

    REQUIRE(quaternionProduct2.w() == Approx(-0.1231).epsilon(0.01));
    REQUIRE(quaternionProduct2.x() == Approx(-0.6155).epsilon(0.01));
    REQUIRE(quaternionProduct2.y() == Approx(-0.7385).epsilon(0.01));
    REQUIRE(quaternionProduct2.z() == Approx(-0.2462).epsilon(0.01));

    Quaternionf randomQuaternion1 = {0.1826, 0.3651, 0.5477, 0.7303};
    Quaternionf randomQuaternion2 = {0.0418, 0.9625, 0.1674, 0.2092};
    Quaternionf quaternionProduct3 = quaternionProduct(randomQuaternion1, randomQuaternion2);


    REQUIRE(quaternionProduct3.w() == Approx(-0.5883).epsilon(0.01));
    REQUIRE(quaternionProduct3.x() == Approx(0.1834).epsilon(0.01));
    REQUIRE(quaternionProduct3.y() == Approx(0.6800).epsilon(0.01));
    REQUIRE(quaternionProduct3.z() == Approx(-0.3973).epsilon(0.01));

}
