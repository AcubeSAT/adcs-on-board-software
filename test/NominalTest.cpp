#include <catch2/catch.hpp>
#include "Nominal.hpp"

using namespace Eigen;

TEST_CASE("First part of Nominal") {
    GlobalStateVector outputState = nominalMode(2);
    GlobalStateVector expectedState;
    expectedState
            << 0.44928, -0.11889, 0.88536, -0.0122, 0.1, 0.2, 0.3;
    for (int i = 0; i < 4; i++) {
        REQUIRE(outputState(i) == Approx(expectedState(i)).epsilon(0.01));
    }
}
TEST_CASE("Measurements for nominal test") {
    Vector<float, NominalMeasurementsSize> measurements;
    Vector3f magneticFieldECI = {7740.17871171431, 1088.51463852010, 21943.9176853079};
    Vector3f sunPositionECI = {-0.171851379656845, 0.903853476098922, 0.391811175254017};
    Vector3f satellitePositionECI = {-0.979555954434493,-0.201160548089278,-0.00213682576508476};
    float albedo = 0;
    measurements = measurementsProduction(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
    Vector<float, 9> res = {-0.413148384628908, 0.862004142487583, -0.293695881164122, -0.946661971565983,
                            -0.140309616201384, -0.293181612221257, 0.1, 0.2, 0.3};
    for (int i = 0; i < 3; i++) {
        REQUIRE(measurements(i) == Approx(res(i)).epsilon(0.01));
    }
}