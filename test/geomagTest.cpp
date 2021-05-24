#include <catch2/catch.hpp>
#include "geomag.h"
#include "mathFunctions.h"

TEST_CASE("IGRF test") {
    geomag_vector igrf_struct = {.currentDate = 0, .latitude = 0, .longitude = 0,
            .altitude =500, .xMagneticField = 0, .yMagneticField = 0, .zMagneticField = 0,
            .norm = 0, .declination = 0, .inclination = 0, .horizontalIntensity = 0, .totalIntensity = 0};

    double decimal_date = date2decimal(2024, 1, 1, 0, 0, 0);

    igrf_struct.currentDate = decimal_date;
    geomag(&igrf_struct);
    REQUIRE(igrf_struct.xMagneticField == Approx(21591.5).epsilon(0.01));
    REQUIRE(igrf_struct.yMagneticField == Approx(-1682.9).epsilon(0.01));
    REQUIRE(igrf_struct.zMagneticField == Approx(-10890.3).epsilon(0.01));
    REQUIRE(igrf_struct.declination == Approx(-4.4569).epsilon(0.01));
    REQUIRE(igrf_struct.inclination == Approx(-26.6957).epsilon(0.01));
    REQUIRE(igrf_struct.horizontalIntensity == Approx(21657.0).epsilon(0.01));
    REQUIRE(igrf_struct.totalIntensity == Approx(24241.0).epsilon(0.01));

    decimal_date = date2decimal(2024, 9, 11, 0, 0, 0);
    igrf_struct.currentDate = decimal_date;
    igrf_struct.latitude = 42;
    igrf_struct.longitude = 69;
    igrf_struct.altitude = 600;
    geomag(&igrf_struct);
    REQUIRE(igrf_struct.xMagneticField == Approx(19129.9).epsilon(0.01));
    REQUIRE(igrf_struct.yMagneticField == Approx(1512.0).epsilon(0.01));
    REQUIRE(igrf_struct.zMagneticField == Approx(35570.5).epsilon(0.01));
    REQUIRE(igrf_struct.declination == Approx(4.5192).epsilon(0.01));
    REQUIRE(igrf_struct.inclination == Approx(61.6541).epsilon(0.01));
    REQUIRE(igrf_struct.horizontalIntensity == Approx(19189.5).epsilon(0.01));
    REQUIRE(igrf_struct.totalIntensity == Approx(40416.5).epsilon(0.01));

    decimal_date = date2decimal(2023, 4, 20, 0, 0, 0);
    igrf_struct.currentDate = decimal_date;
    igrf_struct.latitude = 69;
    igrf_struct.longitude = 42;
    igrf_struct.altitude = 569;
    geomag(&igrf_struct);
    REQUIRE(igrf_struct.xMagneticField == Approx(7937.5).epsilon(0.01));
    REQUIRE(igrf_struct.yMagneticField == Approx(2269.1).epsilon(0.01));
    REQUIRE(igrf_struct.zMagneticField == Approx(42923.9).epsilon(0.01));
    REQUIRE(igrf_struct.declination == Approx(15.9535).epsilon(0.01));
    REQUIRE(igrf_struct.inclination == Approx(79.1133).epsilon(0.01));
    REQUIRE(igrf_struct.horizontalIntensity == Approx(8255.5).epsilon(0.01));
    REQUIRE(igrf_struct.totalIntensity == Approx(43710.5).epsilon(0.01));
}