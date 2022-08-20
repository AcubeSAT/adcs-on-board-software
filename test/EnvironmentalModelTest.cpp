#include <catch2/catch.hpp>
#include "EnvironmentalModel.hpp"

using namespace Eigen;

TEST_CASE("Environmental Model Test with tle6PM500") {
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);

    em.ModelEnvironment();
    bool ecl1 = em.getIsEclipse();
    REQUIRE(ecl1 == false);

    Vector3f sunPosTest1 = em.getSunPositionECI();
    REQUIRE(sunPosTest1[0] == Approx(-0.174717604923527).epsilon(1e-4));
    REQUIRE(sunPosTest1[1] == Approx(0.918928406982493).epsilon(1e-4));
    REQUIRE(sunPosTest1[2] == Approx(0.398346002571225).epsilon(1e-4));

    Vector3f magneticField = em.getMagneticFieldECI();
    REQUIRE(magneticField(0) == Approx(7740.17871171431).epsilon(0.01));
    REQUIRE(magneticField(1) == Approx(1088.51463852010).epsilon(0.01));
    REQUIRE(magneticField(2) == Approx(21943.9176853079).epsilon(0.01));


    auto alb = em.getAlbedo();
    REQUIRE(alb == Approx(-9.579723442651105).epsilon(1e-4));
}

TEST_CASE("Environmental Model Test with tle11PM600") {
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle11PM600, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);

    em.ModelEnvironment();
    bool ecl1 = em.getIsEclipse();
    REQUIRE(ecl1 == true);

    Vector3f sunPosTest1 = em.getSunPositionECI();
    REQUIRE(sunPosTest1[0] == Approx(-0.174717604923527).epsilon(1e-4));
    REQUIRE(sunPosTest1[1] == Approx(0.918928406982493).epsilon(1e-4));
    REQUIRE(sunPosTest1[2] == Approx(0.398346002571225).epsilon(1e-4));

    Vector3f magneticField = em.getMagneticFieldECI();
    REQUIRE(magneticField(0) == Approx(-3754.63144650908).epsilon(0.01));
    REQUIRE(magneticField(1) == Approx(-9109.37704516883).epsilon(0.01));
    REQUIRE(magneticField(2) == Approx(19692.5520758561).epsilon(0.01));

}

TEST_CASE("Environmental Model Test with tle11PM500 for eclipse") {
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle11PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);

    em.ModelEnvironment();
    bool ecl1 = em.getIsEclipse();
    REQUIRE(ecl1 == true);
}

TEST_CASE("Environmental Model Test with tle6PM500 and 4th time-step") {
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);
    for (int i = 1; i < 4; i++) {
        em.ModelEnvironment();
    }
    bool ecl1 = em.getIsEclipse();
    REQUIRE(ecl1 == false);

    Vector3f sunPosTest1 = em.getSunPositionECI();
    REQUIRE(sunPosTest1[0] == Approx(-0.174717643510772).epsilon(1e-4));
    REQUIRE(sunPosTest1[1] == Approx(0.918928400843573).epsilon(1e-4));
    REQUIRE(sunPosTest1[2] == Approx(0.398345999910050).epsilon(1e-4));

    Vector3f magneticField = em.getMagneticFieldECI();
    REQUIRE(magneticField(0) == Approx(7754.40242187146).epsilon(0.01));
    REQUIRE(magneticField(1) == Approx(1091.60673407232).epsilon(0.01));
    REQUIRE(magneticField(2) == Approx(21942.9617533484).epsilon(0.01));
}

TEST_CASE("Eclipse Test")
{
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);
    em.ModelEnvironment();
    bool eclps = em.getIsEclipse();
    REQUIRE(eclps == false);
}

TEST_CASE("Sun Position Test")
{
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle11PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);
    em.ModelEnvironment();
    Vector3f sunpos = em.getSunPositionECI();
    REQUIRE(sunpos(0) == Approx(-0.174717604923527).epsilon(1e-4));
    REQUIRE(sunpos(1) == Approx(0.918928406982493).epsilon(1e-4));
    REQUIRE(sunpos(2) == Approx(0.398346002571225).epsilon(1e-4));
}

TEST_CASE("Environmental Model Test with tle6PM500 time step 39") {
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData1);

    for (int i = 1; i < 40; i++) {
        em.ModelEnvironment();
    }
    Vector3f satPosTest1 = em.getSatellitePositionECI();
    REQUIRE(satPosTest1[0] == Approx(-6733757.51820706).epsilon(1e-4));
    REQUIRE(satPosTest1[1] == Approx(-1379046.15879295).epsilon(1e-4));
    REQUIRE(satPosTest1[2] == Approx(14019.1944664209).epsilon(1e-4));

    Vector3f magneticField = em.getMagneticFieldECI();
    REQUIRE(magneticField(0) == Approx(8010.61224322292).epsilon(0.01));
    REQUIRE(magneticField(1) == Approx(1147.11562015444).epsilon(0.01));
    REQUIRE(magneticField(2) == Approx(21924.5840590608).epsilon(0.01));
}