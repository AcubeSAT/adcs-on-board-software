#include <catch2/catch.hpp>
#include "Geomag.hpp"
#include "MathFunctions.hpp"

TEST_CASE("IGRF test") {
    igrf_vector.altitude = 500;

    double decimal_date = date2decimal(2024, 1, 1, 0, 0, 0);

    igrf_vector.currentDate = decimal_date;
    geomag(&igrf_vector);
    REQUIRE(igrf_vector.xMagneticField == Approx(21591.5).epsilon(0.01));
    REQUIRE(igrf_vector.yMagneticField == Approx(-1682.9).epsilon(0.01));
    REQUIRE(igrf_vector.zMagneticField == Approx(-10890.3).epsilon(0.01));
    REQUIRE(igrf_vector.declination == Approx(-4.4569).epsilon(0.01));
    REQUIRE(igrf_vector.inclination == Approx(-26.6957).epsilon(0.01));
    REQUIRE(igrf_vector.horizontalIntensity == Approx(21657.0).epsilon(0.01));
    REQUIRE(igrf_vector.totalIntensity == Approx(24241.0).epsilon(0.01));

    decimal_date = date2decimal(2024, 9, 11, 0, 0, 0);
    igrf_vector.currentDate = decimal_date;
    igrf_vector.latitude = 42;
    igrf_vector.longitude = 69;
    igrf_vector.altitude = 600;
    geomag(&igrf_vector);
    REQUIRE(igrf_vector.xMagneticField == Approx(19129.9).epsilon(0.01));
    REQUIRE(igrf_vector.yMagneticField == Approx(1512.0).epsilon(0.01));
    REQUIRE(igrf_vector.zMagneticField == Approx(35570.5).epsilon(0.01));
    REQUIRE(igrf_vector.declination == Approx(4.5192).epsilon(0.01));
    REQUIRE(igrf_vector.inclination == Approx(61.6541).epsilon(0.01));
    REQUIRE(igrf_vector.horizontalIntensity == Approx(19189.5).epsilon(0.01));
    REQUIRE(igrf_vector.totalIntensity == Approx(40416.5).epsilon(0.01));

    decimal_date = date2decimal(2023, 4, 20, 0, 0, 0);
    igrf_vector.currentDate = decimal_date;
    igrf_vector.latitude = 69;
    igrf_vector.longitude = 42;
    igrf_vector.altitude = 569;
    geomag(&igrf_vector);
    REQUIRE(igrf_vector.xMagneticField == Approx(7937.5).epsilon(0.01));
    REQUIRE(igrf_vector.yMagneticField == Approx(2269.1).epsilon(0.01));
    REQUIRE(igrf_vector.zMagneticField == Approx(42923.9).epsilon(0.01));
    REQUIRE(igrf_vector.declination == Approx(15.9535).epsilon(0.01));
    REQUIRE(igrf_vector.inclination == Approx(79.1133).epsilon(0.01));
    REQUIRE(igrf_vector.horizontalIntensity == Approx(8255.5).epsilon(0.01));
    REQUIRE(igrf_vector.totalIntensity == Approx(43710.5).epsilon(0.01));
}