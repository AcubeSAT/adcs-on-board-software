#include "EnvironmentalModel.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

EnvironmentalModel::EnvironmentalModel(OrbitalParameters orbitalParameters,
                                       Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> reflectivityData)
        : orbitalParameters(orbitalParameters), reflectivityData{reflectivityData} {
    igrf_struct = {.currentDate = 0, .latitude = 0, .longitude = 0,
            .altitude = 0, .xMagneticField = 0, .yMagneticField = 0, .zMagneticField = 0,
            .norm = 0, .declination = 0, .inclination = 0, .horizontalIntensity = 0, .totalIntensity = 0};
    isEclipse = false;
    sunPosition = {0, 0, 0};
    albedo = Matrix<float, ReflectivityDataRows, ReflectivityDataColumns>::Zero();
    magneticField = {0, 0, 0};
}


void EnvironmentalModel::ModelEnvironment() {
    orbitalParameters.calculateNextPosition();
    satellitePosition = orbitalParameters.getPosition();
    double julianDate = orbitalParameters.getjulianDay();
    double gstime = orbitalParameters.getGstime();
    Vector3f satelliteLLH = orbitalParameters.getSatLLH();
    double timeGregorian = orbitalParameters.getTimeGregorian();

    igrf_struct.latitude = satelliteLLH[0] * 180 / PI;
    igrf_struct.longitude = satelliteLLH[1] * 180 / PI;
    igrf_struct.altitude = satelliteLLH[2] / 1000;
    igrf_struct.currentDate = timeGregorian;

    geomag(&igrf_struct);

    magneticField(0) = igrf_struct.xMagneticField;
    magneticField(1) = igrf_struct.yMagneticField;
    magneticField(2) = igrf_struct.zMagneticField;
    magneticField = ned2ecef(magneticField, satelliteLLH[0], satelliteLLH[1]);
    magneticField = ecef2eci(magneticField, gstime);

    Eigen::Vector3f sunPositionECI = calculateSunPosition(julianDate);
    sunPosition = sunPositionECI;

    isEclipse = calculateEclipse(satellitePosition, sunPositionECI);

    Eigen::Vector3f satelliteECEF = eci2ecef(satellitePosition, gstime);
    Eigen::Vector3f sunECEF = eci2ecef(sunPositionECI, gstime);
    satellitePosition *= 1000;

    albedo = calculateAlbedo(satelliteECEF, sunECEF, reflectivityData);

}
