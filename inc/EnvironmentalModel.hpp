

#ifndef ADCS_ONBOARD_SOFTWARE_ENVIRONMENTALMODEL_HPP
#define ADCS_ONBOARD_SOFTWARE_ENVIRONMENTALMODEL_HPP

#include "Eigen/Geometry"
#include "Eclipse.hpp"
#include "SunPosition.hpp"
#include "Albedo.hpp"
#include "Geomag.h"
#include "sgp4.h"
#include "EciToEcef.hpp"
#include "TLEUtils.hpp"
#include "etl/string.h"
#include <cstring>
#include "MathFunctions.hpp"
#include "EcefToLlh.hpp"

/**
 * Environmental Model class
 * five properties :
 * - MagneticField
 * - SunPosition
 * - Albedo
 * - Eclipse
 * - Int_TLE (call tle and initialize parameters)
 */
class EnvironmentalModel {

    double jd;

    elsetrec satrec;
    double tsince;

    double position[3];
    double velocity[3];

    geomag_vector gStr;

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData;


public:


    void MagneticField();

    Eigen::Vector3d SunPosition();

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> Albedo();

    bool Eclipse();


    void Init_TLE(const TLE tle, char typerun, char typeinput, char opsmode,
                  gravconsttype whichconst,
                  double &startmfe, double &stopmfe, double &deltamin, double tsince,
                  Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> refData);


};


#endif //ADCS_ONBOARD_SOFTWARE_ENVIRONMENTALMODEL_HPP
