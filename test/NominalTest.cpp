#include <catch2/catch.hpp>
#include "Nominal.hpp"

using namespace Eigen;
const SatelliteModel satelliteModel;
TEST_CASE("First part of Nominal test time step 11") {
    MeasurementVector measurments(-0.946772755370904, -0.141294636304082, -0.289235501692104,-0.413443195401665,0.862767093417922,-0.291028635517626);
    Vector3f gyroscopeBias = {0, 0, 0};

    Matrix<float, LocalStateSize, LocalStateSize> Q;
    Q << 0.000100000000000000,	0,	0,	0,	0,	0,
    0,	0.000100000000000000,	0,	0,	0,	0,
    0,	0,	0.000100000000000000,	0,	0,	0,
    0,	0,	0,	1.00000000000000e-07,	0,	0,
    0,	0,	0,	0,	1.00000000000000e-07,	0,
    0,	0,	0,	0,	0,	1.00000000000000e-07;

    Matrix<float, MeasurementSize, MeasurementSize> R;
    R << 0.000500000000000000	,0,	0,	0,	0,	0,
    0,	0.000500000000000000,	0,	0,	0,	0,
    0,	0,	0.000500000000000000,	0,	0,	0,
    0,	0,	0,	0.00100000000000000,	0,	0,
    0,	0,	0,	0,	0.00100000000000000,	0,
    0,	0,	0,	0,	0,	0.00100000000000000;

    Matrix<float, LocalStateSize, LocalStateSize> P;
    P
            << 0.00136699495713899, -0.000114760060275635, 0.000256861463917196, 0, 0, 0,
            -0.000114760060275612, 0.000441521240017950, -7.67733697177619e-06, 0, 0,	0,
            0.000256861463917308,	-7.67733697184630e-06, 0.000401588226538019, 0,	0, 0,
            0, 0,	0, 1, 0, 0,
            0, 0,	0, 0, 1, 0,
            0, 0,	0, 0, 0, 1;

    OrbitalParameters orbitalParameters;
    Matrix<float, 180, 288> reflectivityData1 = Matrix<float, 180, 288>::Identity() * 0;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel em(orbitalParameters, reflectivityData1);
    em.ModelEnvironment();
    for (int i = 1; i < 12; i++) {
        em.ModelEnvironment();
    }
    MEKF mekf;
    FirstPartOfNominal(em, mekf,satelliteModel, gyroscopeBias,Q,R,P,measurments);
    auto outputState = mekf.getGlobalState();
    GlobalStateVector expectedState;

    expectedState
            <<0.449311355178033,-0.119669240619940,0.885255918577615,-0.0109789608264623,0,0,0;
    for (int i = 0;i < 7; i++){
        REQUIRE(outputState(i) == Approx(expectedState(i)).epsilon(0.01));
    }

}