#include <catch2/catch.hpp>
#include "Nominal.hpp"

using namespace Eigen;

TEST_CASE("First part of Nominal test") {
    const Quaternionf quaternionECIBody(-0.449283260837097, 0.118865589542775, -0.885362710747703, 0.0121816900187276);
    Vector3f magneticBody(-0.946772755370904, -0.141294636304082, -0.289235501692104);
    Vector3f gyroscopeBias = {0, 0, 0};
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 0;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData1);
    em.ModelEnvironment();
    MEKF mekf;
    FirstPartOfNominal(em, mekf, quaternionECIBody, magneticBody, gyroscopeBias);
    auto outputState = mekf.getGlobalState();

    GlobalStateVector expectedState;

    expectedState
            <<0.4506 ,  -0.1190  ,  0.8847  , -0.0112,0,0,0;
    //        <<0.449311355178033,-0.119669240619940,0.885255918577615,-0.0109789608264623,0,0,0;

    REQUIRE(outputState(0) == Approx(expectedState(0)).epsilon(0.01));
    REQUIRE(outputState(1) == Approx(expectedState(1)).epsilon(0.01));
    REQUIRE(outputState(2) == Approx(expectedState(2)).epsilon(0.01));
    REQUIRE(outputState(3) == Approx(expectedState(3)).epsilon(1));
    REQUIRE(outputState(4) == Approx(expectedState(4)).epsilon(0.01));
    REQUIRE(outputState(5) == Approx(expectedState(5)).epsilon(0.01));
    REQUIRE(outputState(6) == Approx(expectedState(6)).epsilon(0.01));

}

