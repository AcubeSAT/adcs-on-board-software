#include <catch2/catch.hpp>
#include "Nominal.hpp"

using namespace Eigen;


//TEST_CASE("Calculate Quaternion Orbit To Body") {
//    GlobalStateVector globalState = {0.118771, -0.88537, 0.012124,-0.44929, 0, 0, 0};
//    double ascendingNode = 3.343859;
//    double inclination = 1.69953;
//    double argumentPerigeeMeanAnomaly = 0.001218;
//
//    Quaternionf quaternionOrbitBody = calculateQuaternionOrbitBody(globalState, ascendingNode, inclination,
//                                                                   argumentPerigeeMeanAnomaly);
//
//    REQUIRE(quaternionOrbitBody.w() == Approx(-0.44724).epsilon(1e-2));
//    REQUIRE(quaternionOrbitBody.x()== Approx(0.0000443).epsilon(1e-2));
//    REQUIRE(quaternionOrbitBody.y() == Approx(-0.89441).epsilon(1e-2));
//    REQUIRE(quaternionOrbitBody.z() == Approx(-0.00012).epsilon(1e-2));
//}
//
//TEST_CASE("First part of Nominal") {
//    GlobalStateVector outputState = NominalMode(2);
//    GlobalStateVector expectedState = {0.2235, -0.25593, 0.9401, -0.02559, 0, 0, 0};
//
//    for (int i = 0; i < 4; i++) {
//        REQUIRE(outputState(i) == Approx(expectedState(i)).epsilon(0.01));
//    }
//}