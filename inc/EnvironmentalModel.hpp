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
    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> albedo;
    Eigen::Vector3f magneticField;
    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData;
    geomag_vector igrf_struct;
    OrbitalParameters orbitalParameters;
public:

    EnvironmentalModel(OrbitalParameters orbitalParameters, Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData);

    void ModelEnvironment();

    //get magnetic field (NED)
    Eigen::Vector3f GetMagneticField() const {
        return magneticField;
    }

    Eigen::Vector3f GetSunPosition() const {
        return sunPosition;
    }

    bool GetEclipse() {
        return isEclipse;
    }

    Eigen::Vector3f GetSatellitePosition() const {
        return satellitePosition;
    }

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> GetAlbedo() const {
        return albedo;
    }
};