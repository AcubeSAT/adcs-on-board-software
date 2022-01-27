#pragma once

#include "Eigen/Geometry"
#include "Eclipse.hpp"
#include "SunPosition.hpp"
#include "Albedo.hpp"
#include "Geomag.h"
#include "TLEUtils.hpp"
#include "MathFunctions.hpp"
#include "OrbitalParameters.hpp"

class EnvironmentalModel {
private:
    bool eclipse;
    Eigen::Vector3f sunPosition;
    Eigen::Vector3f satellitePosition;
    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> albedo;
    Eigen::Vector3f magneticField;
    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData;
    geomag_vector gStr;
    OrbitalParameters orbitalParameters;
public:

    EnvironmentalModel(OrbitalParameters orbitalParameters, Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData);

    void ModelEnvironmental();

    //get magnetic field (NED)
    Eigen::Vector3f GetMagneticField() {
        return magneticField;
    }

    Eigen::Vector3f GetSunPosition() {
        return sunPosition;
    }

    bool GetEclipse() {
        return eclipse;
    }

    Eigen::Vector3f GetSatellitePosition() {
        return satellitePosition;
    }

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> GetAlbedo() {
        return albedo;
    }
};