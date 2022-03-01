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
    bool isEclipse;
    Eigen::Vector3f sunPosition;
    Eigen::Vector3f satellitePosition;
    EarthCellsMatrix albedo;
    Eigen::Vector3f magneticField;
    EarthCellsMatrix reflectivityData;
    geomag_vector geomagneticVectorStruct;
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
     * @return Magnetic field ECI frame
     */
    Eigen::Vector3f GetMagneticField() const {
        return magneticField;
    }

    /**
     * @return sun position ECI frame
     */
    Eigen::Vector3f GetSunPosition() const {
        return sunPosition;
    }

    bool getIsEclipse() {
        return isEclipse;
    }

    /**
     * @return satellite position ECI
     */
    Eigen::Vector3f GetSatellitePosition() const {
        return satellitePosition;
    }

    EarthCellsMatrix GetAlbedo() const {
        return albedo;
    }
};