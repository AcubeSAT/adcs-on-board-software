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

TEST_CASE("Eclipse Test1")
{
    Vector3f xSatelliteECI = {-2.9899, -1.4137, -6.0306};
    Vector3f sunPositionECI = {-0.1754, 0.9188, 0.3983};
    bool eclps = calculateEclipse(xSatelliteECI, sunPositionECI);
    REQUIRE(eclps == false);

}

TEST_CASE("Eclipse Test2")
{
    Vector3f xSatelliteECI = {-2.8489, -1.3942, -6.1030};
    Vector3f sunPositionECI = {-0.1758, 0.9188, 0.3983};
    bool eclps = calculateEclipse(xSatelliteECI, sunPositionECI);
    REQUIRE(eclps == false);

}

TEST_CASE("Eclipse Test3")
{
    Vector3f xSatelliteECI = {-3.4753, -1.4765, -5.7487};
    Vector3f sunPositionECI = {-0.1758, 0.9188, 0.3983};
    bool eclps = calculateEclipse(xSatelliteECI, sunPositionECI);
    REQUIRE(eclps == false);

}

TEST_CASE("Sun Position Test1")

{
    double time = 2460127.50000000;
    Vector3f sunpos = calculateSunPosition(time);
    REQUIRE(sunpos(0) == Approx(-0.174717604923527));
    REQUIRE(sunpos(1) == Approx(0.918928406982493));
    REQUIRE(sunpos(2) == Approx(0.398346002571225));
}

TEST_CASE("Sun Position Test2")

{
    double time = 2.4601;
    Vector3f sunpos = calculateSunPosition(time);
    REQUIRE(sunpos(0) == Approx(-0.407105226684744));
    REQUIRE(sunpos(1) == Approx(-0.828752263583653));
    REQUIRE(sunpos(2) == Approx(-0.374407193426776));
}