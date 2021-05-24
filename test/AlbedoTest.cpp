#include <catch2/catch.hpp>
#include "Albedo.h"

TEST_CASE("Albedo test") {
    int8_t i = 40;
    int8_t j = 50;

    int8_t loopI = 78;
    int8_t loopJ = 24;
    int8_t sunIndexI = 56;
    int8_t sunIndexJ = 95;

    Vector3f satellite = {100000000, 200000000, 300000000};
    Vector3f sunPosition = {5000000, 4000000, 8000000};
    Matrix<float, 4, 5> reflectivityData;

    reflectivityData << 1.06, 1.06, 1.06, 1.06, 1.06,
            1.02, 1.02, 1.02, 1.02, 1.02,
            0.99, 0.99, 0.99, 0.99, 1,
            0.98, 0.98, 0.98, 0.98, 0.98;

    Albedo albedoObject;

    Vector<int8_t, 2> indices1 = albedoObject.radiansToIndices(20, 40);
    REQUIRE(indices1(0) == Approx(-46).epsilon(0.01));
    REQUIRE(indices1(1) == Approx(19).epsilon(0.01));

    Vector<int8_t, 2> indices2 = albedoObject.radiansToIndices(1.0e+06 * 0.000000674740942,
                                                               1.0e+06 * 0.000001446501385);
    REQUIRE(indices2(0) == Approx(3).epsilon(0.01));
    REQUIRE(indices2(1) == Approx(4).epsilon(0.01));

    Vector2f radians = albedoObject.indicesToRadians(i, j);
    REQUIRE(radians(0) == Approx(59.0619).epsilon(0.01));
    REQUIRE(radians(1) == Approx(-27.8816).epsilon(0.01));

    float cellArea = albedoObject.calculateCellArea(i, j);
    REQUIRE(cellArea == Approx(-1.4939e+13).epsilon(0.01));

    float gridAngle = albedoObject.gridAngle(loopI, loopJ, sunIndexI, sunIndexJ);
    REQUIRE(gridAngle == Approx(1.8176).epsilon(0.01));

//    Matrix<float, 180, 288> albedo = albedoObject.calculateAlbedo(satellite, sunPosition, reflectivityData);

//    REQUIRE(albedo(0, 0) == Approx(0).epsilon(0.01));
//    REQUIRE(albedo(0, 1) == Approx(0).epsilon(0.01));
//    REQUIRE(albedo(0, 2) == Approx(0).epsilon(0.01));
//    REQUIRE(albedo(0, 3) == Approx(0).epsilon(0.01));
//    REQUIRE(albedo(0, 4) == Approx(0).epsilon(0.01));
//
//    REQUIRE(albedo(1, 0) == Approx(0).epsilon(0.01));
//    REQUIRE(albedo(1, 1) == Approx(0).epsilon(0.01));
//    REQUIRE(albedo(1, 2) == Approx(0).epsilon(0.01));
//    REQUIRE(albedo(1, 3) == Approx(0).epsilon(0.01));
//    REQUIRE(albedo(1, 4) == Approx(0).epsilon(0.01));
//
//    REQUIRE(albedo(2, 0) == Approx(-1.0e-04 * 0.0109).epsilon(0.01));
//    REQUIRE(albedo(2, 1) == Approx(-1.0e-04 * 0.0056).epsilon(0.01));
//    REQUIRE(albedo(2, 2) == Approx(1.0e-04 * 0.2074).epsilon(0.01));
//    REQUIRE(albedo(2, 3) == Approx(1.0e-04 * 0.5012).epsilon(0.01));
//    REQUIRE(albedo(2, 4) == Approx(1.0e-04 * 0.1473).epsilon(0.01));
//
//    REQUIRE(albedo(3, 0) == Approx(1.0e-04 * 0.1309).epsilon(0.01));
//    REQUIRE(albedo(3, 1) == Approx(1.0e-04 * 0.1413).epsilon(0.01));
//    REQUIRE(albedo(3, 2) == Approx(1.0e-04 * 0.2581).epsilon(0.01));
//    REQUIRE(albedo(3, 3) == Approx(1.0e-04 * 0.3323).epsilon(0.01));
//    REQUIRE(albedo(3, 4) == Approx(1.0e-04 * 0.2370).epsilon(0.01));

}
