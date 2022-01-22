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
    Eigen::Vector3d sunPosition;
    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> albedo;
    Eigen::Vector3d magneticField;
    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData;

    geomag_vector gStr;

    OrbitalParameters orbitalParameters;
public:

    EnvironmentalModel(OrbitalParameters orbitalParameters);

    void ModelEnvironmental();

    geomag_vector Get_magnetic_field() {
        return gStr;
    }

    Eigen::Vector3d Get_sunPosition() {
        return sunPosition;
    }

    bool Get_eclipse() {
        return eclipse;
    }

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> Get_albedo() {
        return albedo;
    }


};