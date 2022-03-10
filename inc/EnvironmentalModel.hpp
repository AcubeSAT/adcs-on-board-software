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
     *  Sunlight reflected off the Earth's surface
     */
    EarthCellsMatrix albedo;
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
    * Calculates sun position
    * @param time
    */
    void calculateSunPosition(double time);

    /**
     * @return Magnetic field (ECI)
     */
    Eigen::Vector3f getMagneticField() const {
        return magneticField;
    }

    /**
     * @return Sun position (ECI)
     */
    Eigen::Vector3f getSunPosition() const {
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
    Eigen::Vector3f getSatellitePosition() const {
        return satellitePosition;
    }

    EarthCellsMatrix getAlbedo() const {
        return albedo;
    }
};