

#ifndef ADCS_ONBOARD_SOFTWARE_ENVIRONMENRALMODEL_HPP
#define ADCS_ONBOARD_SOFTWARE_ENVIRONMENRALMODEL_HPP

#include "Eigen/Geometry"
#include "Eclipse.hpp"
#include "SunPosition.hpp"
#include "Albedo.hpp"
#include "Geomag.h"
#include "sgp4.h"
#include "EciToEcef.h"

class EnvironmentalModel {

    int year;
    int mon;
    int day;
    int hr;
    int minute;
    double sec;
    double jd;
    double jdFrac;

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

};


#endif //ADCS_ONBOARD_SOFTWARE_ENVIRONMENRALMODEL_HPP
