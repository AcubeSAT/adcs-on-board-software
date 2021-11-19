#include "NominalToDetumblingTrigger.h"

using namespace Eigen;

Vector3i NominalToDetumblingTrigger::Trigger(Vector3f currentVelocity,
                                             Vector3f oldVelocity,
                                             int thresholdTimes,
                                             int thresholdExceptions) {
    int totalLimit = 20;
    int exceptionsLimit = 2;
    float N2DThreshold = 0.04;

    int triggerFlag = 0;
    int triggerFlagRaw = 0;

    Vector3i returnValues;

    if (abs(currentVelocity(0)) > N2DThreshold ||
        abs(currentVelocity(1)) > N2DThreshold ||
        abs(currentVelocity(2)) > N2DThreshold) {
        if (thresholdTimes == 0) {
            thresholdTimes = 1;
        } else if (thresholdTimes >= 1) {
            if (abs(oldVelocity(0)) > N2DThreshold ||
                abs(oldVelocity(1)) > N2DThreshold ||
                abs(oldVelocity(2)) > N2DThreshold) {
                thresholdTimes++;
            } else if (abs(oldVelocity(0)) <= N2DThreshold &&
                       abs(oldVelocity(1)) <= N2DThreshold &&
                       abs(oldVelocity(2)) <= N2DThreshold &&
                       thresholdExceptions < exceptionsLimit) {
                thresholdTimes++;
                thresholdExceptions++;
            } else if (abs(oldVelocity(0)) <= N2DThreshold &&
                       abs(oldVelocity(1)) <= N2DThreshold &&
                       abs(oldVelocity(2)) <= N2DThreshold &&
                       thresholdExceptions >= exceptionsLimit) {
                thresholdTimes = 0;
                thresholdExceptions = 0;
            }
        }
        if (thresholdTimes >= totalLimit) {
            triggerFlag = 1;
        }
    } else {
        if (abs(oldVelocity(0)) <= N2DThreshold &&
            abs(oldVelocity(1)) <= N2DThreshold &&
            abs(oldVelocity(2)) <= N2DThreshold &&
            thresholdExceptions < exceptionsLimit) {
            thresholdExceptions++;
        } else if (abs(oldVelocity(0)) <= N2DThreshold &&
                   abs(oldVelocity(1)) <= N2DThreshold &&
                   abs(oldVelocity(2)) <= N2DThreshold &&
                   thresholdExceptions >= exceptionsLimit) {
            thresholdTimes = 0;
            thresholdExceptions = 0;
        }
    }

    if (abs(currentVelocity(0)) > N2DThreshold ||
        abs(currentVelocity(1)) > N2DThreshold ||
        abs(currentVelocity(2)) > N2DThreshold) {
        triggerFlagRaw = 1;
    }

    returnValues(0) = triggerFlag;
    returnValues(1) = thresholdTimes;
    returnValues(2) = thresholdExceptions;

    return returnValues;
}
