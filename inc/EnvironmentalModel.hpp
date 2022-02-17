#pragma once

#include "Eigen/Geometry"
#include "Albedo.hpp"
#include "Geomag.h"
#include "TLEUtils.hpp"
#include "MathFunctions.hpp"
#include "OrbitalParameters.hpp"

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
     * initialize the parameters of EnvironmentalModel class
     * @param orbitalParameters OrbitalParameters class's properties
     * @param reflectivityData Earth surface reflectivity data from TOMS project
     */
    EnvironmentalModel(OrbitalParameters orbitalParameters, EarthCellsMatrix reflectivityData);

    /**
     * calculate properties of EnvironmentalModel class
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