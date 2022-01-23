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

    double get_jd() {
        return jd;
    }

    elsetrec get_satrec() {
        return satrec;
    }

    double get_tsince() {
        return tsince;
    }

    Eigen::Vector3f get_position() {
        return position;

    }

    Eigen::Vector3f get_SatLLH() {
        return sat_llh;

    }

    double getGstime() {
        return gstime;
    }

    double getTime_gregorian() {
        return time_gregorian;
    }
};
