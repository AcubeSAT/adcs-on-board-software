#include <catch2/catch.hpp>
#include "Nominal.hpp"

using namespace Eigen;

TEST_CASE("First part of Nominal test") {
    const Quaternionf quaternionECIBody(-0.449279850250564, 0.118894667693728, -0.885360292481303, 0.0121994528668081);
    Vector3f magneticBody(-0.945805868176365, -0.140182728736128, -0.292916476640093);
    Vector3f giroscopeBias = {1, 1, 1};
    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 0;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData1);
    em.ModelEnvironment();
    MEKF mekf;
    FirstPartOfNominal(em, mekf, quaternionECIBody, magneticBody, giroscopeBias);
    auto outputState = mekf.getGlobalState();

    GlobalStateVector expectedState;




    expectedState
            <<0.449311355178033,-0.119669240619940,0.885255918577615,-0.0109789608264623,0,0,0;
    for (int i = 0; i < 7; i++) {
        REQUIRE(outputState(i) == Approx(expectedState(i)).epsilon(0.01));
    }
}