#include <catch2/catch.hpp>
#include "Albedo.h"

TEST_CASE("Albedo test") {

    Vector3f satellite = {100000000, 200000000, 300000000};
    Vector3f sunPosition = {5000000, 4000000, 8000000};
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;
    Matrix<float, 180, 288> reflectivityData2 = Matrix<float, 180, 288>::Identity() * 5000;


    Matrix<float, 180, 288> albedo1 = calculateAlbedo(satellite, sunPosition, reflectivityData1);
    REQUIRE(albedo1.sum() == Approx(0.065217631207925).epsilon(0.01));

    Matrix<float, 180, 288> albedo2 = calculateAlbedo(satellite, sunPosition, reflectivityData2);
    REQUIRE(albedo2.sum() == Approx(0.003260881560396).epsilon(0.01));
}
