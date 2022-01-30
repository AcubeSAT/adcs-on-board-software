#pragma once

#include "Eigen/Dense"

#include "sgp4.h"

#include "TLEUtils.hpp"

#define R_EARTH 6371
#define R_SUN 696000
#define AU 149600000

/**
 * calculate if we have or not eclipse
 * @param xSatelliteECI satellite position ECI frame
 * @param sunPositionECI sun position ECI frame
 * @return true (if we are in eclipse) , false (if we are not)
 */
bool calculateEclipse(Eigen::Vector3f xSatelliteECI, Eigen::Vector3f sunPositionECI);

/**
 * calculate sun's position
 * @param time
 * @return sun's position ECI frame
 */
Eigen::Vector3f calculateSunPosition(double time);

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
     * initialize properties
     */
    OrbitalParameters();

    /**
     * calculate julian day and Gregorian time
     * @param tle Two-line element set
     * @param typeruntype of run (verification 'v',catalog 'c',manual 'm')
     * @param typeinput type of manual input (mfe 'm', epoch 'e', dayofyr 'd')
     * @param opsmode mode of operation afspc or improved 'a', 'i'
     * @param whichconst which set of constants to use  72, 84
     */
    void
    calculateTime(const TLE &tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst);

    /**
     * calculate next time-step
     */
    void calculateNextPosition();

    /**
     * @return julian day property
     */
    double getjulianDay() const {
        return julianDay;
    }

    /**
     * @return satellite record
     */
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

    /**
     * @return Gregorian time
     */
    double getTimeGregorian() const {
        return timeGregorian;
    }
};