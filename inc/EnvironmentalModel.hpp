#pragma once

#include "Geomag.h"

#include "Albedo.hpp"
#include "OrbitalParameters.hpp"

/**
 * A class that takes Gregorian time, greenwich sidereal time, satellite position in LLH and ECI frame ,
 * time since epoch, satrec, julian Day and gives sun position ,satellite position ,albedo ,if we are in eclipse, magnetic filed
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
     * Satellite position coordinates on ECI frame
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
     * Initialize the parameters orbitalParameters, isEclipse, sunPosition, albedo, magneticField
     * of EnvironmentalModel class
     *
     * @param orbitalParameters OrbitalParameters class's properties
     * @param reflectivityData Earth surface reflectivity data from TOMS project
     */
    EnvironmentalModel(OrbitalParameters orbitalParameters, EarthCellsMatrix reflectivityData);

    /**
     * Calculate all properties of EnvironmentalModel class: isEclipse, sunPosition, satellitePosition, albedo,
     * magneticField, reflectivityData, geomagneticVectorStruct, orbitalParameters
     */
    void ModelEnvironment();

    /**
    * Calculate if we have or not eclipse
    *
    * @param xSatelliteECI satellite position ECI frame
    * @param sunPositionECI sun position ECI frame
    * @return true if we are in eclipse , false if we are not
    */
    void calculateEclipse(Eigen::Vector3f xSatelliteECI, Eigen::Vector3f sunPositionECI);

    /**
    * Calculate sun's position
    *
    * @param time and we convert in ut1 which is Jan 1, 2000 12 h epoch
     * @return sun's position ECI frame
    */

    void calculateSunPosition(double time);

    /**
     * @return Magnetic field ECI frame
     */
    Eigen::Vector3f getMagneticField() const {
        return magneticField;
    }

    /**
     * @return sun position ECI frame
     */
    Eigen::Vector3f getSunPosition() const {
        return sunPosition;
    }

    bool getIsEclipse() const {
        return isEclipse;
    }

    /**
     * @return satellite position ECI
     */
    Eigen::Vector3f getSatellitePosition() const {
        return satellitePosition;
    }

    EarthCellsMatrix getAlbedo() const {
        return albedo;
    }
};