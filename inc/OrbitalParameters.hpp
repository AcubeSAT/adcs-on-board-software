#pragma once

#include "Eigen/Dense"

#include "sgp4.h"

#include "TLEUtils.hpp"


/**
 * A class that implements the algorithms necessary for the calculation of Gregorian time, greenwich sidereal time, satellite position in LLH and ECI frames,
 * time since epoch, satrec and julian Date given a TLE file
 */
class OrbitalParameters {
private:
    /**
     * Day month year hour minute second converted into julian date
     */
    double julianDate;
    /**
     * Structure containing all the sgp4 satellite information
     */
    elsetrec satrec;
    /**
     * Time since epoch in minutes
     */
    double timeSince;
    /**
     * Satellite position in ECI frame
     */
    Eigen::Vector3f satelliteECI;
    /**
     * Greenwich sidereal time (0 to 2pi rad)
     */
    double greenwichSiderealTime;
    double timeGregorian;
    /**
     * Satellite position in LLH frame
     */
    Eigen::Vector3f satelliteLLH;

public:
    /**
     * Initialize all class's parameters, julianDate, timeSince, position, greenwichSiderealTime, timeGregorian, satelliteLLH
     */
    OrbitalParameters();

    /**
     * Calculate julian day and Gregorian time
     * @param tle Two-line element set
     * @param typerun Type of run (verification 'v',catalog 'c',manual 'm')
     * @param typeinput Type of manual input (mfe 'm', epoch 'e', dayofyr 'd')
     * @param opsmode Mode of operation afspc or improved 'a', 'i'
     * @param whichconst Which set of constants to use  72, 84
     */
    void
    calculateTime(const TLE &tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst);

    /**
     * Calculate next time-step satellite position
     */
    void calculateNextPosition();

    double getJulianDate() const {
        return julianDate;
    }

    elsetrec getSatrec() const {
        return satrec;
    }

    /**
     * @return Time since epoch (minutes)
     */
    double getTimeSince() const {
        return timeSince;
    }

    /**
     * @return Satellite position (ECI frame)
     */
    Eigen::Vector3f getPosition() const {
        return satelliteECI;
    }

    /**
     * @return Satellite position (LLH frame)
     */
    Eigen::Vector3f getSatelliteLLH() const {
        return satelliteLLH;
    }

    /**
     * @return Greenwich sidereal time
     */
    double getGreenwichSiderealTime() const {
        return greenwichSiderealTime;
    }

    double getTimeGregorian() const {
        return timeGregorian;
    }
};