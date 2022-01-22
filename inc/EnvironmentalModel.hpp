//
//
//#ifndef ADCS_ONBOARD_SOFTWARE_ENVIRONMENTALMODEL_HPP
//#define ADCS_ONBOARD_SOFTWARE_ENVIRONMENTALMODEL_HPP
//
//#include "Eigen/Geometry"
//#include "Eclipse.hpp"
//#include "SunPosition.hpp"
//#include "Albedo.hpp"
//#include "Geomag.h"
//#include "sgp4.h"
//#include "EciToEcef.hpp"
//#include "TLEUtils.hpp"
//#include "MathFunctions.hpp"
//#include "EcefToLlh.hpp"
//
//
//class TLE_property {
//private:
//    double jd;
//    elsetrec satrec;
//    double tsince;
//    double position[3];
//    double velocity[3];
//    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData;
//    geomag_vector gStr;
//public:
//
//    /**
//    * Init_TLE - initialize all the parameters needed to calculate the properties ,tle given
//    * @param tle       - two-line element set
//    * @param typerun   - type of run (verification 'v', catalog 'c', manual 'm')
//    * @param typeinput - type of manual input (mfe 'm', epoch 'e', dayofyr 'd')
//    * @param opsmode   - mode of operation (afspc or improved 'a', 'i')
//    * @param whichconst- which set of constants to use  72, 84
//    * @param startmfe  -
//    * @param stopmfe   -
//    * @param deltamin  -
//    * @param tsince    - tsince = tsince_offset + t_array * dt  %minutes
//    * @param refData   - reflectivity Data for albedo
//    */
//    TLE_property(const TLE tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst, double &startmfe,
//                  double &stopmfe, double &deltamin, double tsince,
//                  Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> refData);
//    double get_jd(){
//        return jd;
//    }
//    elsetrec get_satrec(){
//        return satrec;
//    }
//    double get_tsince(){
//        return tsince;
//    }
//    double *get_position(){
//        return position; //here may need vector or smth
//
//    }
//    double *get_velocity(){
//        return velocity;
//    }
//    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> get_reflectivityData(){
//        return reflectivityData;
//    }
//    void Update_tle_parameters(elsetrec satrec_new,Eigen::Vector3d position_new,
//    double velocity_new[3]){
//        satrec = satrec_new;
//        position = position_new;
//        velocity = velocity_new;
//    }
//
//};
//
//
//class EnvironmentalModel {
//private:
//    bool eclipse_property;
//    Eigen::Vector3d sun_position_property;
//    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> albedo_property;
//    geomag_vector gStr;
//
//    TLE_property &tle1;
//public:
//
//
//    void MagneticField();
//
//    Eigen::Vector3d SunPosition();
//
//    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> Albedo();
//
//    bool Eclipse();
//
//
//    /**
//     * ModelEnvironmental - calculate parameters (eclipse ,sun position ,magnetic field ,albedo)
//     * @param tle
//     * @param typerun
//     * @param typeinput
//     * @param opsmode
//     * @param whichconst
//     * @param startmfe
//     * @param stopmfe
//     * @param deltamin
//     * @param tsince
//     * @param refData
//     */
//    void ModelEnvironmental(TLE_property &tle1);
//
//    geomag_vector Get_magnetic_field() {
//        return gStr;
//    }
//
//    Eigen::Vector3d Get_sun_position() {
//        return sun_position_property;
//    }
//
//    bool Get_eclipse() {
//        return eclipse_property;
//    }
//
//    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> Get_albedo() {
//        return albedo_property;
//    }
//
//
//};
//
//
//#endif //ADCS_ONBOARD_SOFTWARE_ENVIRONMENTALMODEL_HPP