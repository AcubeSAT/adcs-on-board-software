#include <catch2/catch.hpp>
#include "EcefToLlh.hpp"
#include <cmath>

using namespace Eigen;

TEST_CASE("EcefToLlh Test1")
{
    Vector3d vec_ecef;
    vec_ecef[0] = 218984.818755685;
    vec_ecef[1] = -6870059.20017860;
    vec_ecef[2] = -14687.6088752119;
    Vector3d vec_Llh = ecef_to_llh(vec_ecef);

    REQUIRE(vec_Llh[0] == Approx(-0.00215018349404917));
    REQUIRE(vec_Llh[1] == Approx(-1.53893187083921));
    REQUIRE(vec_Llh[2] == Approx(495427.201998211));

}

TEST_CASE("EcefToLlh Test2")
{
    Vector3d vec_ecef;
    vec_ecef[0] = -2352955.19123252;
    vec_ecef[1] = -6385334.94375174;
    vec_ecef[2] = -974941.937267234;

    Vector3d vec_Llh = ecef_to_llh(vec_ecef);

    REQUIRE(vec_Llh[0] == Approx(-0.143176024574243));
    REQUIRE(vec_Llh[1] == Approx(-1.92385060820004));
    REQUIRE(vec_Llh[2] == Approx(496843.174380195));

}

TEST_CASE("EcefToLlh Test3")
{
    Vector3d vec_ecef;
    vec_ecef[0] = 217654.986658955;
    vec_ecef[1] = -6870105.25711085;
    vec_ecef[2] = -7888.63372952578;
    Vector3d vec_Llh = ecef_to_llh(vec_ecef);

    REQUIRE(vec_Llh[0] == Approx(-0.00115485233360311));
    REQUIRE(vec_Llh[1] == Approx(-1.53912545696974));
    REQUIRE(vec_Llh[2] == Approx(495419.761713431));

}