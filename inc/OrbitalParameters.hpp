#pragma once

#include "sgp4.h"
#include "Eigen/Geometry"
#include "Eigen/Dense"
#include "TLEUtils.hpp"

class OrbitalParameters {
private:
    double jd;
    elsetrec satrec;
    double tsince;
    Eigen::Vector3f position;
    double gstime;
    double time_gregorian;
    Eigen::Vector3f sat_llh;

public:

    OrbitalParameters();

    void
    calculateTime(const TLE tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst);

    void calculateNextPosition();

    double getJd() {
        return jd;
    }

    elsetrec getSatrec() {
        return satrec;
    }

    double getTsince() {
        return tsince;
    }

    Eigen::Vector3f getPosition() {
        return position;

    }

    Eigen::Vector3f getSatLLH() {
        return sat_llh;

    }

    double getGstime() {
        return gstime;
    }

    double getTimeGregorian() {
        return time_gregorian;
    }
};
