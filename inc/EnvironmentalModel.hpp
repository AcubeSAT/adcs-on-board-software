#pragma once

#include "Geomag.hpp"

#include "Albedo.hpp"
#include "OrbitalParameters.hpp"

/**
 * A class to calculate sun position (ECI), satellite position (ECI),
 * albedo, existence of eclipse and magnetic field (ECI).
 */
class EnvironmentalModel {
private:
    bool isEclipse;
    /**
     * Sun position (ECI)
     */
    Eigen::Vector3f sunPosition;
    /**
     * Satellite position (ECI)
     */
    Eigen::Vector3f satellitePosition;
    /**
     *  Sum of sunlight units reflected off the Earth's surface
     */
    float albedoSum;
    /**
     * Magnetic field (ECI)
     */
    Eigen::Vector3f magneticField;
    /**
     * Earth surface reflectivity data from TOMS project
     */
    EarthCellsMatrix reflectivityData;
    OrbitalParameters orbitalParameters;
public:
    /**
     * Initializes all class parameters, isEclipse, sunPosition, albedo, magneticField
     * @param reflectivityData Earth surface reflectivity data from TOMS project
     */
    EnvironmentalModel(OrbitalParameters orbitalParameters, EarthCellsMatrix reflectivityData);

    /**
     * Calculates sun position (ECI), satellite position (ECI),
     * albedo, existence of eclipse and magnetic field (ECI)
     */
    void ModelEnvironment();

    /**
    * Calculates existence of eclipse
    * @param xSatelliteECI Satellite position (ECI)
    * @param sunPositionECI Sun position (ECI)
    */
    void calculateEclipse(Eigen::Vector3f xSatelliteECI, Eigen::Vector3f sunPositionECI);

    /**
    * Calculates sun position.In this function we use ut1,so we convert time to ut1 which is Jan 1, 2000 12 h epoch.
    * There are also some constants used (to calculate eclipticLongitude and magnitude) based on literature
    * @param time Time in JD format
    */
    void calculateSunPosition(double time);

    /**
     * @return Magnetic field (ECI)
     */
    Eigen::Vector3f getMagneticFieldECI() const {
        return magneticField;
    }

    /**
     * @return Sun position (ECI)
     */
    Eigen::Vector3f getSunPositionECI() const {
        return sunPosition;
    }

    /**
     * @return Existence of eclipse or not
     */
    bool getIsEclipse() const {
        return isEclipse;
    }

    /**
     * @return Satellite position (ECI)
     */
    Eigen::Vector3f getSatellitePositionECI() const {
        return satellitePosition;
    }

    /**
     * @return The sum of sunlight reflected off the Earth's surface
     */
    float getAlbedo() const {
        return albedoSum;
    }
};