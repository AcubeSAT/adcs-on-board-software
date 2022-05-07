#include <catch2/catch.hpp>
#include "MathFunctions.hpp"

using namespace Eigen;

TEST_CASE("Cartesian to spherical test") {
    Vector3f vectorCartesian1 = {0.1, 0.2, 0.3};
    Vector3f vectorSpherical1 = cartesianToSpherical(vectorCartesian1);

    REQUIRE(vectorSpherical1(0) == Approx(1.1071).epsilon(0.01));
    REQUIRE(vectorSpherical1(1) == Approx(0.9303).epsilon(0.01));
    REQUIRE(vectorSpherical1(2) == Approx(0.3742).epsilon(0.01));

    Vector3f vectorCartesian2 = {100000000, 200000000, 300000000};
    Vector3f vectorSpherical2 = cartesianToSpherical(vectorCartesian2);

    REQUIRE(vectorSpherical2(0) == Approx(1.1071).epsilon(0.01));
    REQUIRE(vectorSpherical2(2) == Approx(3.7417e+08).epsilon(0.01));
    REQUIRE(vectorSpherical2(1) == Approx(0.9303).epsilon(0.01));

    Vector3f vectorCartesian3 = {5000000, 4000000, 8000000};
    Vector3f vectorSpherical3 = cartesianToSpherical(vectorCartesian3);

    REQUIRE(vectorSpherical3(0) == Approx(0.674740942223553).epsilon(0.01));
    REQUIRE(vectorSpherical3(1) == Approx(0.895817302077988).epsilon(0.01));
    REQUIRE(vectorSpherical3(2) == Approx(1.024695076595960e+07).epsilon(0.01));

    Vector3f zerosCartesian = {0, 0, 0};
    Vector3f zerosSpherical = cartesianToSpherical(zerosCartesian);

    REQUIRE(zerosSpherical(0) == Approx(0).epsilon(0.01));
    REQUIRE(zerosSpherical(1) == Approx(0).epsilon(0.01));
    REQUIRE(zerosSpherical(2) == Approx(0).epsilon(0.01));

    Vector3f negativesCartesian = {-0.5689, -0.457, -2.568};
    Vector3f negativesSpherical = cartesianToSpherical(negativesCartesian);

    REQUIRE(negativesSpherical(0) == Approx(-2.464839943079727).epsilon(0.01));
    REQUIRE(negativesSpherical(1) == Approx(-1.293934058318156).epsilon(0.01));
    REQUIRE(negativesSpherical(2) == Approx(2.669666685187497).epsilon(0.01));
}

TEST_CASE("Spherical to cartesian test") {
    Vector3f vectorSpherical = {0.1, 0.2, 0.3};
    Vector3f vectorCartesian = sphericalToCartesian(vectorSpherical);

    REQUIRE(vectorCartesian(0) == Approx(0.2926).epsilon(0.01));
    REQUIRE(vectorCartesian(1) == Approx(0.0294).epsilon(0.01));
    REQUIRE(vectorCartesian(2) == Approx(0.0596).epsilon(0.01));

    Vector3f zerosSpherical = {0, 0, 0};
    Vector3f zerosCartesian = sphericalToCartesian(zerosSpherical);

    REQUIRE(zerosCartesian(0) == Approx(0).epsilon(0.01));
    REQUIRE(zerosCartesian(1) == Approx(0).epsilon(0.01));
    REQUIRE(zerosCartesian(2) == Approx(0).epsilon(0.01));

    Vector3f negativeSpherical = {-0.5689, -0.457, -2.568};
    Vector3f negativeCartesian = sphericalToCartesian(negativeSpherical);

    REQUIRE(negativeCartesian(0) == Approx(-1.941504504351219).epsilon(0.01));
    REQUIRE(negativeCartesian(1) == Approx(1.241432383097179).epsilon(0.01));
    REQUIRE(negativeCartesian(2) == Approx(1.133150430340820).epsilon(0.01));
}

TEST_CASE("Date to decimal date test") {
    REQUIRE(date2decimal(2024, 1, 1, 0, 0, 0) == Approx(2024).epsilon(0.000001));
    REQUIRE(date2decimal(2024, 12, 32, 0, 0, 0) == Approx(2025).epsilon(0.000001));
    REQUIRE(date2decimal(2024, 7, 24, 7, 23, 28) == Approx(2024.5609507185).epsilon(0.000001));
    REQUIRE(date2decimal(2024, 1, 10, 11, 54, 26) == Approx(2024.0259457220).epsilon(0.000001));
    REQUIRE(date2decimal(2024, 9, 11, 9, 27, 16) == Approx(2024.6950653967).epsilon(0.000001));
    REQUIRE(date2decimal(2024, 5, 30, 10, 52, 58) == Approx(2024.4110749975).epsilon(0.000001));
    REQUIRE(date2decimal(2023, 6, 9, 6, 9, 6) == Approx(2023.4363186834).epsilon(0.000001));
    REQUIRE(date2decimal(2100, 6, 9, 6, 9, 6) == Approx(2100.4363186834).epsilon(0.000001));
    REQUIRE(date2decimal(2400, 9, 11, 9, 27, 16) == Approx(2400.6950653967).epsilon(0.000001));
}

TEST_CASE("Quaternion exponent test") {
    Vector3f w = {0.4, 0.5, -0.5};

    Eigen::Quaternionf q = quaternionExponent(w);

    REQUIRE(q.w() == Approx(0.9186).epsilon(0.01));
    REQUIRE(q.x() == Approx(0.1945).epsilon(0.01));
    REQUIRE(q.y() == Approx(0.2432).epsilon(0.01));
    REQUIRE(q.z() == Approx(-0.2432).epsilon(0.01));

    Vector3f lowVelocity = {1e-07, 1e-07, 1e-07};

    Eigen::Quaternionf lowVelocityQuaternion = quaternionExponent(lowVelocity);

    REQUIRE(lowVelocityQuaternion.w() == Approx(1).epsilon(0.01));
    REQUIRE(lowVelocityQuaternion.x() == Approx(5e-08).epsilon(0.01));
    REQUIRE(lowVelocityQuaternion.y() == Approx(5e-08).epsilon(0.01));
    REQUIRE(lowVelocityQuaternion.z() == Approx(5e-08).epsilon(0.01));

    Vector3f highVelocity1 = {1e+03, 1e+03, 1e+03};

    Eigen::Quaternionf highVelocityQuaternion1 = quaternionExponent(highVelocity1);

    REQUIRE(highVelocityQuaternion1.w() == Approx(0.4940).epsilon(0.01));
    REQUIRE(highVelocityQuaternion1.x() == Approx(-0.5020).epsilon(0.01));
    REQUIRE(highVelocityQuaternion1.y() == Approx(-0.5020).epsilon(0.01));
    REQUIRE(highVelocityQuaternion1.z() == Approx(-0.5020).epsilon(0.01));

    Vector3f highVelocity2 = {1e+05, 1e+05, 1e+05};

    Eigen::Quaternionf highVelocityQuaternion2 = quaternionExponent(highVelocity2);

    REQUIRE(highVelocityQuaternion2.w() == Approx(0.1726).epsilon(0.01));
    REQUIRE(highVelocityQuaternion2.x() == Approx(0.5687).epsilon(0.01));
    REQUIRE(highVelocityQuaternion2.y() == Approx(0.5687).epsilon(0.01));
    REQUIRE(highVelocityQuaternion2.z() == Approx(0.5687).epsilon(0.01));

    Vector3f zeroVelocity = {0, 0, 0};

    Eigen::Quaternionf zeroVelocityQuaternion = quaternionExponent(zeroVelocity);

    REQUIRE(zeroVelocityQuaternion.w() == Approx(1).epsilon(0.01));
    REQUIRE(zeroVelocityQuaternion.x() == Approx(0).epsilon(0.01));
    REQUIRE(zeroVelocityQuaternion.y() == Approx(0).epsilon(0.01));
    REQUIRE(zeroVelocityQuaternion.z() == Approx(0).epsilon(0.01));
}

TEST_CASE("Quaternion product test") {
    Quaternionf q1 = {0.5, 0.5, -0.5, -0.5};
    Quaternionf q2 = {0.5, 0.5, 0.5, 0.5};
    Quaternionf prod = quaternionProduct(q1, q2);

    REQUIRE(prod.w() == Approx(0.5).epsilon(0.01));
    REQUIRE(prod.x() == Approx(0.5).epsilon(0.01));
    REQUIRE(prod.y() == Approx(-0.5).epsilon(0.01));
    REQUIRE(prod.z() == Approx(0.5).epsilon(0.01));

    Quaternionf identityQuaternion1 = {1, 0, 0, 0};
    Quaternionf identityQuaternion2 = {1, 0, 0, 0};
    Quaternionf identityQuaternionProduct = quaternionProduct(identityQuaternion1, identityQuaternion2);

    REQUIRE(identityQuaternionProduct.w() == Approx(1).epsilon(0.01));
    REQUIRE(identityQuaternionProduct.x() == Approx(0).epsilon(0.01));
    REQUIRE(identityQuaternionProduct.y() == Approx(0).epsilon(0.01));
    REQUIRE(identityQuaternionProduct.z() == Approx(0).epsilon(0.01));


    Quaternionf identityQuaternion = {1, 0, 0, 0};
    Quaternionf random_quaternion = {0.1231, 0.6155, 0.7385, 0.2462};
    Quaternionf quaternionProduct1 = quaternionProduct(identityQuaternion, random_quaternion);


    REQUIRE(quaternionProduct1.w() == Approx(0.1231).epsilon(0.01));
    REQUIRE(quaternionProduct1.x() == Approx(0.6155).epsilon(0.01));
    REQUIRE(quaternionProduct1.y() == Approx(0.7385).epsilon(0.01));
    REQUIRE(quaternionProduct1.z() == Approx(0.2462).epsilon(0.01));

    Quaternionf negativeRandomQuaternion = {-0.1231, -0.6155, -0.7385, -0.2462};
    Quaternionf quaternionProduct2 = quaternionProduct(identityQuaternion, negativeRandomQuaternion);

    REQUIRE(quaternionProduct2.w() == Approx(-0.1231).epsilon(0.01));
    REQUIRE(quaternionProduct2.x() == Approx(-0.6155).epsilon(0.01));
    REQUIRE(quaternionProduct2.y() == Approx(-0.7385).epsilon(0.01));
    REQUIRE(quaternionProduct2.z() == Approx(-0.2462).epsilon(0.01));

    Quaternionf randomQuaternion1 = {0.1826, 0.3651, 0.5477, 0.7303};
    Quaternionf randomQuaternion2 = {0.0418, 0.9625, 0.1674, 0.2092};
    Quaternionf quaternionProduct3 = quaternionProduct(randomQuaternion1, randomQuaternion2);


    REQUIRE(quaternionProduct3.w() == Approx(-0.5883).epsilon(0.01));
    REQUIRE(quaternionProduct3.x() == Approx(0.1834).epsilon(0.01));
    REQUIRE(quaternionProduct3.y() == Approx(0.6800).epsilon(0.01));
    REQUIRE(quaternionProduct3.z() == Approx(-0.3973).epsilon(0.01));
}

TEST_CASE("Vector turn with quaternion test") {
    Eigen::Quaternionf identityQuaternion = {1, 0, 0, 0};
    Vector3f randomVector = {0.1, 0.2, 0.3};
    Vector3f rotatedVector1 = rotateVector(identityQuaternion, randomVector);

    REQUIRE(rotatedVector1(0) == Approx(0.1).epsilon(0.01));
    REQUIRE(rotatedVector1(1) == Approx(0.2).epsilon(0.01));
    REQUIRE(rotatedVector1(2) == Approx(0.3).epsilon(0.01));

    Eigen::Quaternionf randomQuaternion = {0.1231, 0.6155, 0.7385, 0.2462};
    Vector3f lowValuesVector = {1e-07, 1e-07, 1e-07};
    Vector3f rotatedVector2 = rotateVector(randomQuaternion, lowValuesVector);

    REQUIRE(rotatedVector2(0) == Approx(1.0e-06 * 0.0879).epsilon(0.01));
    REQUIRE(rotatedVector2(1) == Approx(1.0e-06 * 0.1485).epsilon(0.01));
    REQUIRE(rotatedVector2(2) == Approx(1.0e-06 * (-0.0151)).epsilon(0.01));

    Vector3f highValuesVector = {1e+03, 1e+03, 1e+03};
    Vector3f rotatedVector3 = rotateVector(randomQuaternion, highValuesVector);

    REQUIRE(rotatedVector3(0) == Approx(1.0e+03 * 0.8789).epsilon(0.01));
    REQUIRE(rotatedVector3(1) == Approx(1.0e+03 * 1.4847).epsilon(0.01));
    REQUIRE(rotatedVector3(2) == Approx(1.0e+03 * (-0.1514)).epsilon(0.01));
}

TEST_CASE("Rads to degrees test") {
    REQUIRE(rad2deg(0) == Approx(0).epsilon(0.01));
    REQUIRE(rad2deg(3.14) == Approx(179.9087).epsilon(0.01));
    REQUIRE(rad2deg(6.28) == Approx(359.8175).epsilon(0.01));
    REQUIRE(rad2deg(1) == Approx(57.2958).epsilon(0.01));
    REQUIRE(rad2deg(2.5) == Approx(143.2394).epsilon(0.01));
    REQUIRE(rad2deg(4.2) == Approx(240.6423).epsilon(0.01));
    REQUIRE(rad2deg(-0.69) == Approx(-39.5341).epsilon(0.01));
}

TEST_CASE("Degrees to rads test") {
    REQUIRE(deg2rad(0) == Approx(0).epsilon(0.01));
    REQUIRE(deg2rad(180) == Approx(3.1416).epsilon(0.01));
    REQUIRE(deg2rad(360) == Approx(2 * 3.1416).epsilon(0.01));
    REQUIRE(deg2rad(83) == Approx(1.4486).epsilon(0.01));
    REQUIRE(deg2rad(274) == Approx(4.7822).epsilon(0.01));
    REQUIRE(deg2rad(420) == Approx(7.3304).epsilon(0.01));
    REQUIRE(deg2rad(-69) == Approx(-1.2043).epsilon(0.01));
}

TEST_CASE("Skew matrix test") {
    Vector3f w = {0.4, 0.5, -0.5};

    Matrix<float, 3, 3> matrix = skew(w);

    REQUIRE(matrix(0, 0) == Approx(0).epsilon(0.01));
    REQUIRE(matrix(0, 1) == Approx(0.5).epsilon(0.01));
    REQUIRE(matrix(0, 2) == Approx(0.5).epsilon(0.01));
    REQUIRE(matrix(1, 0) == Approx(-0.5).epsilon(0.01));
    REQUIRE(matrix(1, 1) == Approx(0).epsilon(0.01));
    REQUIRE(matrix(1, 2) == Approx(-0.4).epsilon(0.01));
    REQUIRE(matrix(2, 0) == Approx(-0.5).epsilon(0.01));
    REQUIRE(matrix(2, 1) == Approx(0.4).epsilon(0.01));
    REQUIRE(matrix(2, 2) == Approx(0).epsilon(0.01));
}

TEST_CASE("Clamp vector") {
    Vector3f vector1 = {0.4, 0.5, -0.5};
    Vector3f upperLimit1 = {1, 2, 3};
    Vector3f lowerLimit1 = {0.1, 0.2, -0.4};

    vector1 = clamp(vector1, lowerLimit1, upperLimit1);
    REQUIRE(vector1(0) == Approx(0.4).epsilon(0.01));
    REQUIRE(vector1(1) == Approx(0.5).epsilon(0.01));
    REQUIRE(vector1(2) == Approx(-0.4).epsilon(0.01));

    Vector3f vector2 = {0.4, 0.5, -0.5};
    Vector3f upperLimit2 = {0, 2, 0};
    Vector3f lowerLimit2 = {-0.1, -0.2, -0.4};

    vector2 = clamp(vector2, lowerLimit2, upperLimit2);
    REQUIRE(vector2(0) == Approx(0).epsilon(0.01));
    REQUIRE(vector2(1) == Approx(0.5).epsilon(0.01));
    REQUIRE(vector2(2) == Approx(-0.4).epsilon(0.01));
}

TEST_CASE("Clamp number") {
    float number1 = 0.4;
    float upperLimit1 = 1;
    float lowerLimit1 = 0.8;

    number1 = clamp(number1, lowerLimit1, upperLimit1);
    REQUIRE(number1 == Approx(0.8).epsilon(0.01));

    float number2 = 0.4;
    float upperLimit2 = 0.3;
    float lowerLimit2 = 0.1;

    number2 = clamp(number2, lowerLimit2, upperLimit2);
    REQUIRE(number2 == Approx(0.3).epsilon(0.01));

    float number3 = 0.4;
    float upperLimit3 = 0.6;
    float lowerLimit3 = 0.1;

    number3 = clamp(number3, lowerLimit3, upperLimit3);
    REQUIRE(number3 == Approx(0.4).epsilon(0.01));

}

TEST_CASE("Clamp down") {
    float number1 = 0.4;
    float limit1 = 1;

    number1 = clampDown(number1, limit1);
    REQUIRE(number1 == Approx(1).epsilon(0.01));

    float number2 = 0.5;
    float limit2 = 0.3;

    number2 = clampDown(number2, limit2);
    REQUIRE(number2 == Approx(0.5).epsilon(0.01));
}

TEST_CASE("Clamp up") {
    float number1 = 0.4;
    float limit1 = 1;

    number1 = clampUp(number1, limit1);
    REQUIRE(number1 == Approx(0.4).epsilon(0.01));

    float number2 = 0.5;
    float limit2 = 0.3;

    number2 = clampUp(number2, limit2);
    REQUIRE(number2 == Approx(0.3).epsilon(0.01));
}

TEST_CASE("eci2ecef Test")
{
    Vector3f vecECI = {-6733.04064617925, -1382.68992247076, -14.6876088752119};
    double gstime = 4.883067536994346;
    Vector3f vec = eci2ecef(vecECI, gstime);

    REQUIRE(vec[0] == Approx(218.984818755685));
    REQUIRE(vec[1] == Approx(-6870.05920017860));
    REQUIRE(vec[2] == Approx(-14.6876088752119));

    vecECI = {-2.3, 0, 1212};
    gstime = 3.3453224;
    vec = eci2ecef(vecECI, gstime);

    REQUIRE(vec[0] == Approx(2.25243318591924));
    REQUIRE(vec[1] == Approx(-0.465343682636487));
    REQUIRE(vec[2] == Approx(1212));

    vecECI = {-1234, -74329.123, 0.1432};
    gstime = 12234909;
    vec = eci2ecef(vecECI, gstime);

    REQUIRE(vec[0] == Approx(-74334.9817031508));
    REQUIRE(vec[1] == Approx(-807.327158822088));
    REQUIRE(vec[2] == Approx(0.143200000000000));
}
TEST_CASE("ecef2llh Test")
{
    Vector3f vecECEF;
    vecECEF[0] = 218984.818755685;
    vecECEF[1] = -6870059.20017860;
    vecECEF[2] = -14687.6088752119;
    Vector3f vecLLH = ecef2llh(vecECEF);

    REQUIRE(vecLLH[0] == Approx(-0.00215018349404917));
    REQUIRE(vecLLH[1] == Approx(-1.53893187083921));
    REQUIRE(vecLLH[2] == Approx(495427.201998211));

    vecECEF[0] = -2352955.19123252;
    vecECEF[1] = -6385334.94375174;
    vecECEF[2] = -974941.937267234;
    vecLLH = ecef2llh(vecECEF);

    REQUIRE(vecLLH[0] == Approx(-0.143176024574243));
    REQUIRE(vecLLH[1] == Approx(-1.92385060820004));
    REQUIRE(vecLLH[2] == Approx(496843.174380195));

    vecECEF[0] = 217654.986658955;
    vecECEF[1] = -6870105.25711085;
    vecECEF[2] = -7888.63372952578;
    vecLLH = ecef2llh(vecECEF);

    REQUIRE(vecLLH[0] == Approx(-0.00115485233360311));
    REQUIRE(vecLLH[1] == Approx(-1.53912545696974));
    REQUIRE(vecLLH[2] == Approx(495419.761713431));
}
TEST_CASE("Ned2ecef Test")
{
    Vector3f vecNED;
    float lat = -0.00215018349404917;
    float longit = -1.53893187083921;
    vecNED[0] = 21927.0935576559;
    vecNED[1] = 490.758716715348;
    vecNED[2] = 7848.08753664789;
    Vector3f vecECEF = ned2ecef(vecNED, lat, longit);

    REQUIRE(vecECEF[0] == Approx(241.979515198619));
    REQUIRE(vecECEF[1] == Approx(7812.59729678756));
    REQUIRE(vecECEF[2] == Approx(21943.9176853079));

    lat = -0.00115485233360311;
    longit = -1.53912545696974;
    vecNED[0] = 21930.4628495019;
    vecNED[1] = 489.006234668502;
    vecNED[2] = 7891.82083326086;
    vecECEF = ned2ecef(vecNED, lat, longit);

    REQUIRE(vecECEF[0] == Approx(239.664101962900));
    REQUIRE(vecECEF[1] == Approx(7878.02890637489));
    REQUIRE(vecECEF[2] == Approx(21939.5621110306));
}

TEST_CASE("ecef2eci Test")
{
    Vector3f vecECEF;
    double gstime = 4.88306753699435;
    vecECEF[0] = 241.979515198619;
    vecECEF[1] = 7812.59729678756;
    vecECEF[2] = 21943.9176853079;
    Vector3f vecECI = ecef2eci(vecECEF, gstime);

    REQUIRE(vecECI[0] == Approx(7740.17871171431));
    REQUIRE(vecECI[1] == Approx(1088.51463852010));
    REQUIRE(vecECI[2] == Approx(21943.9176853079));

    gstime = 5.28741536250600;
    vecECEF[0] = 1831.29948279837;
    vecECEF[1] = -5124.17879734563;
    vecECEF[2] = 22315.2957509272;
    vecECI = ecef2eci(vecECEF, gstime);

    REQUIRE(vecECI[0] == Approx(-3304.13296729673));
    REQUIRE(vecECI[1] == Approx(-4323.60630461274));
    REQUIRE(vecECI[2] == Approx(22315.2957509272));
}