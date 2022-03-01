#pragma once

#include "Eigen/Dense"

#include "sgp4.h"

#include "TLEUtils.hpp"


/**
 * A class that takes a TLE file and initialize Gregorian time, greenwich sidereal time, satellite position in LLH frame and in ECI,
 * time since epoch, satrec, julian Date,
 */
class OrbitalParameters {
private:
    double julianDate;
    elsetrec satrec;
    double timeSince;
    Eigen::Vector3f position;
    double greenwichSiderealTime;
    double timeGregorian;
    Eigen::Vector3f satelliteLLH;

public:
    /**
     * Initialize properties of OrbitalParameters class julianDate, timeSince, position, greenwichSiderealTime, timeGregorian, satelliteLLH
     */
    OrbitalParameters();

    /**
     * Calculate julian day and Gregorian time
     *
     * @param tle Two-line element set
     * @param typerun type of run (verification 'v',catalog 'c',manual 'm')
     * @param typeinput type of manual input (mfe 'm', epoch 'e', dayofyr 'd')
     * @param opsmode mode of operation afspc or improved 'a', 'i'
     * @param whichconst which set of constants to use  72, 84
     */
    void
    calculateTime(const TLE &tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst);

    /**
     * Calculate satellite position for the next time-step
     */
    void calculateNextPosition();

    double getJulianDate() const {
        return julianDate;
    }

    elsetrec getSatrec() const {
        return satrec;
    }

    /**
     * @return time since epoch (minutes)
     */
    double getTimeSince() const {
        return timeSince;
    }

    /**
     * @return satellite position ECI frame
     */
    Eigen::Vector3f getPosition() const {
        return position;
    }

    /**
     * @return satellite position LLH frame
     */
    Eigen::Vector3f getSatelliteLLH() const {
        return satelliteLLH;
    }

    /**
     * @return greenwich sidereal time
     */
    double getgreenwichSiderealTime() const {
        return greenwichSiderealTime;
    }

    double getTimeGregorian() const {
        return timeGregorian;
    }
};