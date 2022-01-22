#pragma once

#include "sgp4.h"
#include "Geomag.h"
#include "Eigen/Geometry"
#include "Eigen/Dense"
#include "TLEUtils.hpp"

class OrbitalParameters {
private:
    double jd;
    elsetrec satrec;
    double tsince;
    geomag_vector gStr;
    Eigen::Vector3d position;
    double gstime;

public:

    OrbitalParameters();

    void
    convertTLE(const TLE tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst, double &stopmfe,
               double &deltamin);

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

    Eigen::Vector3d get_position() {
        return position;

    }

    double getGstime() {
        return gstime;
    }
};
