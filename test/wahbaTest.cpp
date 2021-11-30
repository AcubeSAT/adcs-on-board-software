#include "wahba.h"
#include "math.h"
#include <catch2/catch.hpp>

TEST_CASE("Wahba Test") {
    SECTION("First Test") {
        Eigen::Vector3f sunBody(0.84444074320012, -0.519655841969305, 0.129913960492326);
        Eigen::Vector3f sunECI(0.850685729673606, 0.40684969680042, -0.332877024654889);

        Eigen::Vector3f magneticBody(0.276301199854381, 0.736803199611682, 0.617072679674783);
        Eigen::Vector3f magneticECI(-0.720853996998319, -0.576683197598655, -0.38445546506577);

        Eigen::Quaternionf expectedQuaternion(0.0999476471390835, -0.911368798854971, 0.213310399221446,
                                              0.337514523967483);

        Eigen::Quaternionf outputQuaternion;
        outputQuaternion = wahba(magneticBody, magneticECI, sunBody, sunECI);

        Eigen::Vector3f vectorExpectedQuaternion = expectedQuaternion.vec();

        Eigen::Vector3f vectorOutputQuaternion = outputQuaternion.vec();
        float signFactor = 1;

        REQUIRE(abs(expectedQuaternion.w()) == Approx(abs(outputQuaternion.w())).epsilon(0.01));
        if (signbit(expectedQuaternion.w()) != signbit(outputQuaternion.w())) {
            signFactor = -1;
        }
        for (uint8_t i = 0; i < VECTOR_SIZE; i++) {
            REQUIRE(vectorExpectedQuaternion(i) == Approx(vectorOutputQuaternion(i) * signFactor).epsilon(0.01));
        }
    }SECTION("Second Test") {
        Eigen::Vector3f sunBody(0.24983993656799, -0.0936899762129964, 0.866632279970217);
        Eigen::Vector3f sunECI(-0.914008859200748, -0.335136581706941, 0.00761674049333957);

        Eigen::Vector3f magneticBody(0.533215448243828, 0.592817248117098, 0.0831095662269988);
        Eigen::Vector3f magneticECI(0.713252796614972, 0.110018106106709, 0.314124660380793);

        Eigen::Quaternionf expectedQuaternion(0.802964748, 0.307025134, -0.336274236, -0.38458214);

        Eigen::Quaternionf outputQuaternion;
        outputQuaternion = wahba(magneticBody, magneticECI, sunBody, sunECI);

        Eigen::Vector3f vectorExpectedQuaternion = expectedQuaternion.vec();

        Eigen::Vector3f vectorOutputQuaternion = outputQuaternion.vec();
        float signFactor = 1;

        REQUIRE(abs(expectedQuaternion.w()) == Approx(abs(outputQuaternion.w())).epsilon(0.01));
        if (signbit(expectedQuaternion.w()) != signbit(outputQuaternion.w())) {
            signFactor = -1;
        }
        for (uint8_t i = 0; i < VECTOR_SIZE; i++) {
            REQUIRE(vectorExpectedQuaternion(i) == Approx(vectorOutputQuaternion(i) * signFactor).epsilon(0.01));
        }
    }
}

