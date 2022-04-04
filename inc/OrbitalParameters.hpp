#pragma once

#include "Eigen/Dense"

#include "sgp4.h"

#include "TLEUtils.hpp"


/**
 * A class to calculate Gregorian time, Greenwich Sidereal Time,
 * satellite position in LLH and ECI frames,
 * time since epoch, satrec and Julian date.
 */
class OrbitalParameters {
private:
    double julianDate;
    /**
     * Structure containing all SGP4 parameters
     */
    elsetrec satrec;
    /**
     * Time since epoch (minutes)
     */
    double timeSince;
    /**
     * Satellite position (ECI)
     */
    Eigen::Vector3f satelliteECI;
    /**
     * Greenwich Sidereal Time (0 to 2pi rad)
     */
    double greenwichSiderealTime;
    /**
     * Time in Gregorian format
     */
    double timeGregorian;
    /**
     * Satellite position (LLH)
     */
    Eigen::Vector3f satelliteLLH;

public:
    OrbitalParameters();

    /**
     * Calculates Julian date and Gregorian time
     * @param tle Two-Line Element set
     * @param typerun Type of run (verification 'v', catalog 'c', manual 'm')
     * @param typeinput Type of manual input (mfe 'm', epoch 'e', dayofyr 'd')
     * @param opsmode Mode of operation afspc or improved 'a', 'i'
     * @param whichconst Which set of constants to use 72, 84
     */
    void
    calculateTime(const TLE &tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst);

    /**
     * Calculates satellite position at the next timestep (ECI)
     */
    void calculateNextPosition();

    /**
     * @return Julian date in ut1
     */
    double getJulianDate() const {
        return julianDate;
    }

    /**
     * @return Structure containing all SGP4 parameters
     */
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
     * @return Satellite position (ECI)
     */
    Eigen::Vector3f getPosition() const {
        return satelliteECI;
    }

    /**
     * @return Satellite position (LLH)
     */
    Eigen::Vector3f getSatelliteLLH() const {
        return satelliteLLH;
    }

    /**
     * @return Greenwich Sidereal Time
     */
    double getGreenwichSiderealTime() const {
        return greenwichSiderealTime;
    }

    /**
     * @return Time in Gregorian format
     */
    double getTimeGregorian() const {
        return timeGregorian;
    }
};