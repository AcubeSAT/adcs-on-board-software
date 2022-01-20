#include <catch2/catch.hpp>
#include "Eclipse.hpp"

using namespace Eigen;

TEST_CASE("Eclipse Test1")
{
    Vector3d xsat_eci = {-2.9899, -1.4137, -6.0306};
    Vector3d sun_pos_eci = {-0.1754, 0.9188, 0.3983};
    bool eclps = calculate_eclipse(xsat_eci, sun_pos_eci);
    REQUIRE(eclps == false);

}

TEST_CASE("Eclipse Test2")
{
    Vector3d xsat_eci = {-2.8489, -1.3942, -6.1030};
    Vector3d sun_pos_eci = {-0.1758, 0.9188, 0.3983};
    bool eclps = calculate_eclipse(xsat_eci, sun_pos_eci);
    REQUIRE(eclps == false);

}

TEST_CASE("Eclipse Test3")
{
    Vector3d xsat_eci = {-3.4753, -1.4765, -5.7487};
    Vector3d sun_pos_eci = {-0.1758, 0.9188, 0.3983};
    bool eclps = calculate_eclipse(xsat_eci, sun_pos_eci);
    REQUIRE(eclps == false);

}


