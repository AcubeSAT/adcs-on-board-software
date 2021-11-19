#include <catch2/catch.hpp>
#include "NominalToDetumblingTrigger.h"

using namespace Eigen;

TEST_CASE("Nominal To Detumbling Trigger test") {
    Vector3f currentVelocity = {0.041, 0.031, 0.013};
    Vector3f oldVelocity = {0.033, 0.011, 0.004};
    int thresholdTimes = 19;
    int thresholdExceptions = 1;

    NominalToDetumblingTrigger trigger;

    Vector3i resultsTrigger1 = trigger.Trigger(currentVelocity, oldVelocity, thresholdTimes, thresholdExceptions);

    REQUIRE(resultsTrigger1(0) == 1);
    REQUIRE(resultsTrigger1(1) == 20);
    REQUIRE(resultsTrigger1(2) == 2);

    currentVelocity = {0.039, -0.021, 0.012};
    oldVelocity = {0.032, -0.01, 0.003};
    thresholdTimes = 15;
    thresholdExceptions = 1;

    Vector3i resultsTrigger2 = trigger.Trigger(currentVelocity, oldVelocity, thresholdTimes, thresholdExceptions);

    REQUIRE(resultsTrigger2(0) == 0);
    REQUIRE(resultsTrigger2(1) == 15);
    REQUIRE(resultsTrigger2(2) == 2);

    currentVelocity = {0.039, -0.021, 0.012};
    oldVelocity = {0.032, -0.01, 0.003};
    thresholdTimes = 14;
    thresholdExceptions = 2;

    Vector3i resultsTrigger3 = trigger.Trigger(currentVelocity, oldVelocity, thresholdTimes, thresholdExceptions);

    REQUIRE(resultsTrigger3(0) == 0);
    REQUIRE(resultsTrigger3(1) == 0);
    REQUIRE(resultsTrigger3(2) == 0);

}

