#include <catch2/catch.hpp>
#include "OrbitalParameters.hpp"

using namespace Eigen;

TEST_CASE("Orbital Parameters Test with tle6PM500") {
    OrbitalParameters orbitalParameters;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    orbitalParameters.calculateNextPosition();
    Vector3f position = orbitalParameters.getPosition();
    REQUIRE(position(0) == Approx(-6733.04064617925).epsilon(0.01));
    REQUIRE(position(1) == Approx(-1382.68992247076).epsilon(0.01));
    REQUIRE(position(2) == Approx(-14.6876088752119).epsilon(0.01));

    double timesince = orbitalParameters.getTimeSince();
    REQUIRE(timesince == Approx(0.00166666666666667).epsilon(1e-4));

    orbitalParameters.calculateNextPosition();
    position = orbitalParameters.getPosition();
    REQUIRE(position(0) == Approx(-6733.06104232558).epsilon(0.01));
    REQUIRE(position(1) == Approx(-1382.59434808232).epsilon(0.01));
    REQUIRE(position(2) == Approx(-13.9321677925065).epsilon(0.01));

    timesince = orbitalParameters.getTimeSince();
    REQUIRE(timesince == Approx(0.00333333333333333).epsilon(1e-4));
}

TEST_CASE("Orbital Parameters Test with tle6PM600") {
    OrbitalParameters orbitalParameters;
    orbitalParameters.calculateTime(tle6PM600, 'v', 'd', 'i', wgs84);
    orbitalParameters.calculateNextPosition();
    Vector3f position = orbitalParameters.getPosition();
    REQUIRE(position(0) == Approx(-6838.36405154186).epsilon(0.01));
    REQUIRE(position(1) == Approx(-1404.38826254582).epsilon(0.01));
    REQUIRE(position(2) == Approx(-14.6606146579487).epsilon(0.01));

    double timesince = orbitalParameters.getTimeSince();
    REQUIRE(timesince == Approx(0.00166666666666667).epsilon(1e-4));

    orbitalParameters.calculateNextPosition();
    position = orbitalParameters.getPosition();
    REQUIRE(position(0) == Approx(-6838.38529698246).epsilon(0.01));
    REQUIRE(position(1) == Approx(-1404.28845475415).epsilon(0.01));
    REQUIRE(position(2) == Approx(-13.9116932560971).epsilon(0.01));

    timesince = orbitalParameters.getTimeSince();
    REQUIRE(timesince == Approx(0.00333333333333333).epsilon(1e-4));
}