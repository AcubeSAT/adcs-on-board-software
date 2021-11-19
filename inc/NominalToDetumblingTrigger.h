#ifndef ADCS_ONBOARD_SOFTWARE_NOMINALTODETUMBLINGTRIGGER_H
#define ADCS_ONBOARD_SOFTWARE_NOMINALTODETUMBLINGTRIGGER_H

#include "Eigen/Dense"

class NominalToDetumblingTrigger {
public:
    Eigen::Vector3i Trigger(Eigen::Vector3f currentVelocity,
                            Eigen::Vector3f oldVelocity,
                            int thresholdTimes,
                            int thresholdExceptions);
};

#endif //ADCS_ONBOARD_SOFTWARE_NOMINALTODETUMBLINGTRIGGER_H
