#pragma once

#include "Eigen/Dense"

#include "sgp4.h"

#include "TLEUtils.hpp"

/**
 * Calculate if we have or not eclipse
 *
 * @param xSatelliteECI satellite position ECI frame
 * @param sunPositionECI sun position ECI frame
 * @return true if we are in eclipse , false if we are not
 */
bool calculateEclipse(Eigen::Vector3f xSatelliteECI, Eigen::Vector3f sunPositionECI);

/**
 * Calculate sun's position
 *
 * @param time
 * @return sun's position ECI frame
 */
Eigen::Vector3f calculateSunPosition(double time);

/**
 * A class that takes a TLE file and initialize Gregorian time, greenwich sidereal time, satellite position in LLH frame and in ECI,
 * time since epoch, satrec, julian Day,
 */
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
    /**
     * Initialize properties of OrbitalParameters classjulianDay, tsince, position, gstime, timeGregorian, satelliteLLH
     */
    OrbitalParameters();

    /**
     * Calculate julian day and Gregorian time
     *
     * @param tle Two-line element set
     * @param typeruntype of run (verification 'v',catalog 'c',manual 'm')
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

    double getjulianDay() const {
        return julianDay;
    }

    elsetrec getSatrec() const {
        return satrec;
    }

    /**
     * @return time since epoch (minutes)
     */
    double getTsince() const {
        return tsince;
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
    Eigen::Vector3f getSatLLH() const {
        return satelliteLLH;
    }

    /**
     * @return greenwich sidereal time
     */
    double getGstime() const {
        return gstime;
    }

    double getTimeGregorian() const {
        return timeGregorian;
    }
};