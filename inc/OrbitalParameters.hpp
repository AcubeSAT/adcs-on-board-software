#pragma once

#include "sgp4.h"
#include "Eigen/Geometry"
#include "Eigen/Dense"
#include "TLEUtils.hpp"

class OrbitalParameters {
private:
    double julianDay;
    elsetrec satrec;
    double tsince;
    Eigen::Vector3f position;
    double gstime;
    double timeGregorian;
    Eigen::Vector3f satelliteLLH;

public:

    OrbitalParameters();

    void
    calculateTime(const TLE tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst);

    void calculateNextPosition();

    double getjulianDay() const {
        return julianDay;
    }

    elsetrec getSatrec() const {
        return satrec;
    }

    double getTsince() const {
        return tsince;
    }

    Eigen::Vector3f getPosition() const {
        return position;

    }

    Eigen::Vector3f getSatLLH() const {
        return satelliteLLH;

    }

    double getGstime() const {
        return gstime;
    }

    double getTimeGregorian() const {
        return timeGregorian;
    }
};
