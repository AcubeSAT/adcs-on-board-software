#include "EnvironmentalModel.hpp"
#include "MathFunctions.hpp"

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
    satellitePosition = orbitalParameters.getPosition();
    double julianDate = orbitalParameters.getJd();
    double gstime = orbitalParameters.getGstime();
    Vector3f sat_llh = orbitalParameters.getSatLLH();
    double time_gregorian = orbitalParameters.getTimeGregorian();

    gStr.latitude = sat_llh[0] * 180 / PI;
    gStr.longitude = sat_llh[1] * 180 / PI;
    gStr.altitude = sat_llh[2] / 1000;
    gStr.currentDate = time_gregorian;

    geomag(&gStr);

    magneticField(0) = gStr.xMagneticField;
    magneticField(1) = gStr.yMagneticField;
    magneticField(2) = gStr.zMagneticField;

    // jd = time (im not sure)
    Eigen::Vector3f sun_pos_eci = sun_position(julianDate);
    sunPosition = sun_pos_eci;

    eclipse = calculate_eclipse(satellitePosition, sun_pos_eci);

    Eigen::Vector3f sat_ecef = eci_to_ecef(satellitePosition, gstime);
    Eigen::Vector3f sun_ecef = eci_to_ecef(sun_pos_eci, gstime);
    satellitePosition = satellitePosition * 1000;

    albedo = calculateAlbedo(sat_ecef, sun_ecef, reflectivityData);

}
