

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
#include "MathFunctions.hpp"
#include "EcefToLlh.hpp"


class EnvironmentalModel {
private:
    bool eclipse_property;
    Eigen::Vector3d sun_position_property;
    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> albedo_property;
    const TLE tle;
    geomag_vector gStr;

    double jd;
    elsetrec satrec;
    double tsince;
    double position[3];
    double velocity[3];
    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData;


public:


    void MagneticField();

    Eigen::Vector3d SunPosition();

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> Albedo();

    bool Eclipse();

    /**
     * Init_TLE - initialize all the parameters needed to calculate the properties ,tle given
     * @param tle       - two-line element set
     * @param typerun   - type of run (verification 'v', catalog 'c', manual 'm')
     * @param typeinput - type of manual input (mfe 'm', epoch 'e', dayofyr 'd')
     * @param opsmode   - mode of operation (afspc or improved 'a', 'i')
     * @param whichconst- which set of constants to use  72, 84
     * @param startmfe  -
     * @param stopmfe   -
     * @param deltamin  -
     * @param tsince    - tsince = tsince_offset + t_array * dt  %minutes
     * @param refData   - reflectivity Data for albedo
     */
    void Init_TLE(const TLE tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst, double &startmfe,
                  double &stopmfe, double &deltamin, double tsince,
                  Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> refData);

    /**
     * ModelEnvironmental - calculate parameters (eclipse ,sun position ,magnetic field ,albedo)
     * @param tle
     * @param typerun
     * @param typeinput
     * @param opsmode
     * @param whichconst
     * @param startmfe
     * @param stopmfe
     * @param deltamin
     * @param tsince
     * @param refData
     */
    void ModelEnvironmental(const TLE tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst,
                            double &startmfe, double &stopmfe, double &deltamin, double tsince,
                            Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> refData);

    geomag_vector Get_magnetic_field() {
        return gStr;
    }

    Eigen::Vector3d Get_sun_position() {
        return sun_position_property;
    }

    bool Get_eclipse() {
        return eclipse_property;
    }

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> Get_albedo() {
        return albedo_property;
    }


};


#endif //ADCS_ONBOARD_SOFTWARE_ENVIRONMENTALMODEL_HPP
