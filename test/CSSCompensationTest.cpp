#include <catch2/catch.hpp>
#include "CSSCompensation.hpp"
#include "Eigen/Geometry"

using namespace Eigen;

TEST_CASE("Calculate css compensation") {
    Vector3f sunPositionBody1(-0.2083, 0.9430, -0.2596);
    Quaternionf quatECIBody1(0.4095, -0.0430, 0.9095, 0.0571);
    Vector3f satPositionECI1(0.5701, 0.2218, 0.7911);
    float albedo = 0.3803;

    Vector3f css1 = cssCompensation(sunPositionBody1, quatECIBody1, satPositionECI1, albedo);
    REQUIRE(css1(0) == Approx(0.1790305).epsilon(1e-4));
    REQUIRE(css1(1) == Approx(0.9498404).epsilon(1e-4));
    REQUIRE(css1(2) == Approx(-0.2564198).epsilon(1e-4));

    Vector3f sunPositionBody2(-0.4248, 0.8326, -0.3554);
    Quaternionf quatECIBody2(0.4317, -0.1212, 0.8927, -0.0441);
    Vector3f satPositionECI2(0.5709, 0.2219, 0.7904);

    Vector3f css2 = cssCompensation(sunPositionBody2, quatECIBody2, satPositionECI2, albedo);
    REQUIRE(css2(0) == Approx(-0.04943).epsilon(1e-4));
    REQUIRE(css2(1) == Approx(0.92660426).epsilon(1e-4));
    REQUIRE(css2(2) == Approx(-0.37277785).epsilon(1e-4));

    Vector3f sunPositionBody3(-0.3990, 0.8684, -0.2944);
    Quaternionf quatECIBody3(0.4480, -0.1124, 0.8869, -0.0086);
    Vector3f satPositionECI3(0.5764, 0.2224, 0.7863);

    Vector3f css3 = cssCompensation(sunPositionBody3, quatECIBody3, satPositionECI3, albedo);
    REQUIRE(css3(0) == Approx(-0.02054987).epsilon(1e-4));
    REQUIRE(css3(1) == Approx(0.94523344).epsilon(1e-4));
    REQUIRE(css3(2) == Approx(-0.32574751).epsilon(1e-4));
}
