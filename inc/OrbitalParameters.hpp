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

    double getJd() const {
        return jd;
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
        return sat_llh;

    }

    double getGstime() const {
        return gstime;
    }

    double getTimeGregorian() const {
        return time_gregorian;
    }
};
