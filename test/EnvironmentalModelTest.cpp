#include <catch2/catch.hpp>
#include "EnvironmentalModel.hpp"

using namespace Eigen;

TEST_CASE("Environmental Model Test1 ") {
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);

    em.ModelEnvironment();
    bool ecl1 = em.getIsEclipse();
    REQUIRE(ecl1 == false);

    Vector3f sunPosTest1 = em.GetSunPosition();
    REQUIRE(sunPosTest1[0] == Approx(-0.174717604923527));
    REQUIRE(sunPosTest1[1] == Approx(0.918928406982493));
    REQUIRE(sunPosTest1[2] == Approx(0.398346002571225));

    Vector3f magneticField = em.GetMagneticField();
    REQUIRE(magneticField(0) == Approx(7740.17871171431).epsilon(0.01));
    REQUIRE(magneticField(1) == Approx(1088.51463852010).epsilon(0.01));
    REQUIRE(magneticField(2) == Approx(21943.9176853079).epsilon(0.01));


    auto alb = em.GetAlbedo();
    REQUIRE(alb.sum() == Approx(-9.579723442651105).epsilon(1e-4));
}

TEST_CASE("Environmental Model Test2 ") {
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle11PM600, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);

    em.ModelEnvironment();
    bool ecl1 = em.getIsEclipse();
    REQUIRE(ecl1 == true);

    Vector3f sunPosTest1 = em.GetSunPosition();
    REQUIRE(sunPosTest1[0] == Approx(-0.174717604923527));
    REQUIRE(sunPosTest1[1] == Approx(0.918928406982493));
    REQUIRE(sunPosTest1[2] == Approx(0.398346002571225));

    Vector3f magneticField = em.GetMagneticField();
    REQUIRE(magneticField(0) == Approx(-3754.63144650908).epsilon(0.01));
    REQUIRE(magneticField(1) == Approx(-9109.37704516883).epsilon(0.01));
    REQUIRE(magneticField(2) == Approx(19692.5520758561).epsilon(0.01));

}

TEST_CASE("Environmental Model Test3 ") {
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle11PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);

    em.ModelEnvironment();
    bool ecl1 = em.getIsEclipse();
    REQUIRE(ecl1 == true);
}

TEST_CASE("Environmental Model Test4 ") {
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);
    for (int i = 1; i < 4; i++) {
        em.ModelEnvironment();
    }
    bool ecl1 = em.getIsEclipse();
    REQUIRE(ecl1 == false);

    Vector3f sunPosTest1 = em.GetSunPosition();
    REQUIRE(sunPosTest1[0] == Approx(-0.174717643510772));
    REQUIRE(sunPosTest1[1] == Approx(0.918928400843573));
    REQUIRE(sunPosTest1[2] == Approx(0.398345999910050));

    Vector3f magneticField = em.GetMagneticField();
    REQUIRE(magneticField(0) == Approx(7754.40242187146).epsilon(0.01));
    REQUIRE(magneticField(1) == Approx(1091.60673407232).epsilon(0.01));
    REQUIRE(magneticField(2) == Approx(21942.9617533484).epsilon(0.01));
}
TEST_CASE("Eclipse Test")
{
    Vector3f xSatelliteECI = {-2.9899, -1.4137, -6.0306};
    Vector3f sunPositionECI = {-0.1754, 0.9188, 0.3983};
    bool eclps = calculateEclipse(xSatelliteECI, sunPositionECI);
    REQUIRE(eclps == false);

    xSatelliteECI = {-2.8489, -1.3942, -6.1030};
    sunPositionECI = {-0.1758, 0.9188, 0.3983};
    eclps = calculateEclipse(xSatelliteECI, sunPositionECI);
    REQUIRE(eclps == false);

    xSatelliteECI = {-3.4753, -1.4765, -5.7487};
    sunPositionECI = {-0.1758, 0.9188, 0.3983};
    eclps = calculateEclipse(xSatelliteECI, sunPositionECI);
    REQUIRE(eclps == false);
}

TEST_CASE("Sun Position Test")
{
    double time = 2460127.50000000;
    Vector3f sunpos = calculateSunPosition(time);
    REQUIRE(sunpos(0) == Approx(-0.174717604923527));
    REQUIRE(sunpos(1) == Approx(0.918928406982493));
    REQUIRE(sunpos(2) == Approx(0.398346002571225));

    time = 2.4601;
    sunpos = calculateSunPosition(time);
    REQUIRE(sunpos(0) == Approx(-0.407105226684744));
    REQUIRE(sunpos(1) == Approx(-0.828752263583653));
    REQUIRE(sunpos(2) == Approx(-0.374407193426776));
}