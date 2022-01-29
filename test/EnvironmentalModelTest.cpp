#include <catch2/catch.hpp>
#include "EnvironmentalModel.hpp"

using namespace Eigen;

TEST_CASE("Environmental Model Test1 ") {
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);

    em.ModelEnvironment();
    bool ecl1 = em.GetEclipse();
    REQUIRE(ecl1 == false);

    Eigen::Vector3f sunPosTest1 = em.GetSunPosition();
    REQUIRE(sunPosTest1[0] == Approx(-0.174717604923527));
    REQUIRE(sunPosTest1[1] == Approx(0.918928406982493));
    REQUIRE(sunPosTest1[2] == Approx(0.398346002571225));

    Eigen::Vector3f magneticField = em.GetMagneticField();
    REQUIRE(magneticField(0) == Approx(21927.0935576559).epsilon(0.01));
    REQUIRE(magneticField(1) == Approx(490.758716715348).epsilon(0.01));
    REQUIRE(magneticField(2) == Approx(7848.08753664789).epsilon(0.01));


    auto alb = em.GetAlbedo();
    REQUIRE(alb.sum() == Approx(-9.579723442651105).epsilon(1e-4));
}
