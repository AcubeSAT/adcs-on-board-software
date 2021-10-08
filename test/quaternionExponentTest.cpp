#include <catch2/catch.hpp>
#include "mathFunctions.h"

using namespace Eigen;

TEST_CASE("Quaternion exponent test") {
    Vector3f w = {0.4, 0.5, -0.5};

    Eigen::Quaternionf q = quaternionExponent(w);

    REQUIRE(q.w() == Approx(0.9186).epsilon(0.01));
    REQUIRE(q.x() == Approx(0.1945).epsilon(0.01));
    REQUIRE(q.y() == Approx(0.2432).epsilon(0.01));
    REQUIRE(q.z() == Approx(-0.2432).epsilon(0.01));

    Vector3f lowVelocity = {1e-07, 1e-07, 1e-07};

    Eigen::Quaternionf lowVelocityQuaternion = quaternionExponent(lowVelocity);

    REQUIRE(lowVelocityQuaternion.w() == Approx(1).epsilon(0.01));
    REQUIRE(lowVelocityQuaternion.x() == Approx(5e-08).epsilon(0.01));
    REQUIRE(lowVelocityQuaternion.y() == Approx(5e-08).epsilon(0.01));
    REQUIRE(lowVelocityQuaternion.z() == Approx(5e-08).epsilon(0.01));

    Vector3f highVelocity1 = {1e+03, 1e+03, 1e+03};

    Eigen::Quaternionf highVelocityQuaternion1 = quaternionExponent(highVelocity1);

    REQUIRE(highVelocityQuaternion1.w() == Approx(0.4940).epsilon(0.01));
    REQUIRE(highVelocityQuaternion1.x() == Approx(-0.5020).epsilon(0.01));
    REQUIRE(highVelocityQuaternion1.y() == Approx(-0.5020).epsilon(0.01));
    REQUIRE(highVelocityQuaternion1.z() == Approx(-0.5020).epsilon(0.01));

    Vector3f highVelocity2 = {1e+05, 1e+05, 1e+05};

    Eigen::Quaternionf highVelocityQuaternion2 = quaternionExponent(highVelocity2);

    REQUIRE(highVelocityQuaternion2.w() == Approx(0.1726).epsilon(0.01));
    REQUIRE(highVelocityQuaternion2.x() == Approx(0.5687).epsilon(0.01));
    REQUIRE(highVelocityQuaternion2.y() == Approx(0.5687).epsilon(0.01));
    REQUIRE(highVelocityQuaternion2.z() == Approx(0.5687).epsilon(0.01));

    Vector3f zeroVelocity = {0, 0, 0};

    Eigen::Quaternionf zeroVelocityQuaternion = quaternionExponent(zeroVelocity);

    REQUIRE(zeroVelocityQuaternion.w() == Approx(1).epsilon(0.01));
    REQUIRE(zeroVelocityQuaternion.x() == Approx(0).epsilon(0.01));
    REQUIRE(zeroVelocityQuaternion.y() == Approx(0).epsilon(0.01));
    REQUIRE(zeroVelocityQuaternion.z() == Approx(0).epsilon(0.01));
}

