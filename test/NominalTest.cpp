#include <catch2/catch.hpp>
#include "Nominal.hpp"

using namespace Eigen;

TEST_CASE("First part of Nominal test time step 11") {
    Vector3f magneticBody(-0.946772755370904, -0.141294636304082, -0.289235501692104);
    Vector3f gyroscopeBias = {0, 0, 0};
    Vector3f Sunbody(-0.413443195401665,0.862767093417922,-0.291028635517626);
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 0;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData1);
    em.ModelEnvironment();
    for (int i = 1; i < 12; i++) {
        em.ModelEnvironment();
    }
    MEKF mekf;
    FirstPartOfNominal(em, mekf, Sunbody, magneticBody, gyroscopeBias);
    auto outputState = mekf.getGlobalState();
    GlobalStateVector expectedState;
    expectedState
            <<0.449311355178033,-0.119669240619940,0.885255918577615,-0.0109789608264623,0,0,0;
    REQUIRE(outputState(0) == Approx(expectedState(0)).epsilon(0.01));
    REQUIRE(outputState(1) == Approx(expectedState(1)).epsilon(0.01));
    REQUIRE(outputState(2) == Approx(expectedState(2)).epsilon(0.01));
    REQUIRE(outputState(3) == Approx(expectedState(3)).epsilon(0.01));
    REQUIRE(outputState(4) == Approx(expectedState(4)).epsilon(0.01));
    REQUIRE(outputState(5) == Approx(expectedState(5)).epsilon(0.01));
    REQUIRE(outputState(6) == Approx(expectedState(6)).epsilon(0.01));
}
TEST_CASE("First part of Nominal test time step 1") {
    Vector3f magneticBody(-0.945805868176365,-0.140182728736128,-0.292916476640093);
    Vector3f gyroscopeBias = {-1, 3.141592, 2.7182};
    Vector3f Sunbody(-0.418613482147844,0.855938538682595,-0.303532486831246);
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 0;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData1);
    em.ModelEnvironment();
    em.ModelEnvironment();
    MEKF mekf;
    FirstPartOfNominal(em, mekf, Sunbody, magneticBody, gyroscopeBias);
    auto outputState = mekf.getGlobalState();
    GlobalStateVector expectedState;
    expectedState
            <<0.449400665879234,-0.117185842813310,0.885452033251265,-0.0167695128920361,-1,3.141592,2.7182;
    REQUIRE(outputState(0) == Approx(expectedState(0)).epsilon(0.01));
    REQUIRE(outputState(1) == Approx(expectedState(1)).epsilon(0.01));
    REQUIRE(outputState(2) == Approx(expectedState(2)).epsilon(0.01));
    REQUIRE(outputState(3) == Approx(expectedState(3)).epsilon(0.01));
    REQUIRE(outputState(4) == Approx(expectedState(4)).epsilon(0.01));
    REQUIRE(outputState(5) == Approx(expectedState(5)).epsilon(0.01));
    REQUIRE(outputState(6) == Approx(expectedState(6)).epsilon(0.01));
}
