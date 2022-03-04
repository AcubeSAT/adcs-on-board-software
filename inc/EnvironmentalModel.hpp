#pragma once

#include "Geomag.h"

#include "Albedo.hpp"
#include "OrbitalParameters.hpp"

/**
 * A class that implements the algorithms necessary for the calculation of sun position ,satellite position ,albedo ,whether we are in eclipse or not and magnetic filed
 * given Gregorian time, greenwich sidereal time, satellite position in LLH and ECI frame ,
 * time since epoch, satrec, julian Day
 */
class EnvironmentalModel {
private:
    /**
     * Eclipse estimation (true if we are in eclipse ,false if we are not)
     */
    bool isEclipse;
    /**
     * Sun position coordinates in ECI frame
     */
    Eigen::Vector3f sunPosition;
    /**
     * Satellite position coordinates in ECI frame
     */
    Eigen::Vector3f satellitePosition;
    /**
     * Estimation of the sunlight reflected off the Earth's surface
     */
    EarthCellsMatrix albedo;
    /**
     * Estimation of the Magnetic field in ECI frame
     */
    Eigen::Vector3f magneticField;
    /**
     * Vector containing reflectivity data point's latitude and longitude
     */
    EarthCellsMatrix reflectivityData;
    geomag_vector geomagneticVectorStruct;
    /**
     * Object of OrbitalParameters Class
     */
    OrbitalParameters orbitalParameters;
public:
    /**
     * Initialize all class's parameters, isEclipse, sunPosition, albedo, magneticField
     * @param orbitalParameters OrbitalParameters class's object
     * @param reflectivityData Earth surface reflectivity data from TOMS project
     */
    EnvironmentalModel(OrbitalParameters orbitalParameters, EarthCellsMatrix reflectivityData);

    /**
     * Calculate all class's properties, eclipse, sun position, satellite position, albedo, magnetic field
     */
    void ModelEnvironment();

    /**
    * Calculate whether we are or not in eclipse
    * @param xSatelliteECI Satellite position (ECI frame)
    * @param sunPositionECI Sun position (ECI frame)
    */
    void calculateEclipse(Eigen::Vector3f xSatelliteECI, Eigen::Vector3f sunPositionECI);

    /**
    * Calculate sun's position
    * @param time
    */
    void calculateSunPosition(double time);

    /**
     * @return Magnetic field (ECI frame)
     */
    Eigen::Vector3f getMagneticField() const {
        return magneticField;
    }

    /**
     * @return Sun position (ECI frame)
     */
    Eigen::Vector3f getSunPosition() const {
        return sunPosition;
    }

    /**
     * @return True if we are in eclipse ,false if we are not
     */
    bool getIsEclipse() const {
        return isEclipse;
    }

    /**
     * @return Satellite position (ECI frame)
     */
    Eigen::Vector3f getSatellitePosition() const {
        return satellitePosition;
    }

    EarthCellsMatrix getAlbedo() const {
        return albedo;
    }
};