#include <catch2/catch.hpp>
#include "Eclipse.hpp"

using namespace Eigen;

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


