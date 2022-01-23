#include <catch2/catch.hpp>
#include "EnvironmentalModel.hpp"

using namespace Eigen;

TEST_CASE("Environmental Model Test1 ") {
   OrbitalParameters orbitalParameters;
   Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 100000;

   double stopmfe;
   double deltamin;
   elsetrec satrec;
   orbitalParameters.calculateTime(tle6PM500, 'c', 'd', 'i', wgs84, stopmfe, deltamin);

   EnvironmentalModel em(orbitalParameters, reflectivityData1);

   em.ModelEnvironmental();
   bool ecl1 = em.Get_eclipse();

   REQUIRE(ecl1 == false);

   Eigen::Vector3f sun_pos_test1 = em.Get_sunPosition();
   REQUIRE(sun_pos_test1[0] == Approx(-0.174717604923527));
   REQUIRE(sun_pos_test1[1] == Approx(0.918928406982493));
   REQUIRE(sun_pos_test1[2] == Approx(0.398346002571225));
}
