
#include "EnvironmentalModel.hpp"
#include "MathFunctions.hpp"
#include <iostream>
using namespace std;
using namespace Eigen;

EnvironmentalModel::EnvironmentalModel(OrbitalParameters orbitalParameters,
                                       Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData)
        : orbitalParameters(orbitalParameters), reflectivityData{reflectivityData} {
    gStr = {.currentDate = 0, .latitude = 0, .longitude = 0,
            .altitude = 0, .xMagneticField = 0, .yMagneticField = 0, .zMagneticField = 0,
            .norm = 0, .declination = 0, .inclination = 0, .horizontalIntensity = 0, .totalIntensity = 0};
    eclipse = false;
    sunPosition = {0, 0, 0};
    albedo = Matrix<float, ReflectivityDataRows, ReflectivityDataColumns>::Zero();
    magneticField = {0, 0, 0};
}


void EnvironmentalModel::ModelEnvironmental() {
    orbitalParameters.calculateNextPosition();
    satellitePosition = orbitalParameters.get_position();
    double julianDate = orbitalParameters.get_jd();
    double gstime = orbitalParameters.getGstime();
    Vector3f sat_llh = orbitalParameters.get_SatLLH();
    double time_gregorian = orbitalParameters.getTime_gregorian();

    gStr.latitude = sat_llh[0];
    gStr.longitude = sat_llh[1];
    gStr.altitude = sat_llh[2];
    gStr.currentDate = time_gregorian;

    geomag(&gStr);

    magneticField(0) = gStr.xMagneticField;
    magneticField(1) = gStr.yMagneticField;
    magneticField(2) = gStr.zMagneticField;

    // jd = time (im not sure)
    Eigen::Vector3f sun_pos_eci = sun_position(julianDate);
    sunPosition = sun_pos_eci;
    cout<<(static_cast<float>(julianDate));

    eclipse = calculate_eclipse(satellitePosition, sun_pos_eci);
    Eigen::Vector3f sat_ecef = eci_to_ecef(satellitePosition, gstime);

    Eigen::Vector3f satellite;

    satellite = sat_ecef;

    Eigen::Vector3f sun_ecef = eci_to_ecef(sun_pos_eci, gstime);
    /**
     ** TODO: albedo takes Vectors3f not 3d
     */
    Eigen::Vector3f sat_vec_3f;
    Eigen::Vector3f sun_vec_3f;

    sat_vec_3f[0] = satellite[0];
    sat_vec_3f[1] = satellite[1];
    sat_vec_3f[2] = satellite[2];

    sun_vec_3f[0] = sun_ecef[0];
    sun_vec_3f[1] = sun_ecef[1];
    sun_vec_3f[2] = sun_ecef[2];

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> alb = calculateAlbedo(sat_vec_3f, sun_vec_3f,
                                                                                              reflectivityData);

}
