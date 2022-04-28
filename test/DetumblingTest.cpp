#include <catch2/catch.hpp>
#include "Detumbling.hpp"

using namespace Eigen;

TEST_CASE("Torque Test"){

    Vector3f initialMagneticField = {-0.1378, 0.1833, -0.0411};

    initialMagneticField[0] /= 10000;
    initialMagneticField[1] /= 10000;
    initialMagneticField[2] /= 10000;

    Bdot bDot(initialMagneticField);

    Vector3f nextMagneticField ={-0.1445,0.1779,-0.0395};

    nextMagneticField[0] /= 10000;
    nextMagneticField[1] /= 10000;
    nextMagneticField[2] /= 10000;

    Vector3f magneticTorque = getMagneticTorque(bDot, nextMagneticField);


    REQUIRE(magneticTorque[0] == Approx(-0.00001445).epsilon(1e-3));
    REQUIRE(magneticTorque[1] == Approx(0.00001779).epsilon(1e-3));
    REQUIRE(magneticTorque[2] == Approx(-0.00000395).epsilon(1e-3));
}