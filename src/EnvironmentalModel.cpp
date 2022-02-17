#include "EnvironmentalModel.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

EnvironmentalModel::EnvironmentalModel(OrbitalParameters orbitalParameters,
                                       EarthCellsMatrix reflectivityData)
        : orbitalParameters(orbitalParameters), reflectivityData{reflectivityData} {
    geomagneticVectorStruct = {.currentDate = 0, .latitude = 0, .longitude = 0,
            .altitude = 0, .xMagneticField = 0, .yMagneticField = 0, .zMagneticField = 0,
            .norm = 0, .declination = 0, .inclination = 0, .horizontalIntensity = 0, .totalIntensity = 0};
    isEclipse = false;
    sunPosition = {0, 0, 0};
    albedo = EarthCellsMatrix::Zero();
    magneticField = {0, 0, 0};
}

void EnvironmentalModel::ModelEnvironment() {
    orbitalParameters.calculateNextPosition();
    satellitePosition = orbitalParameters.getPosition();
    double julianDate = orbitalParameters.getjulianDay();
    double gstime = orbitalParameters.getGstime();
    Vector3f satelliteLLH = orbitalParameters.getSatLLH();
    double timeGregorian = orbitalParameters.getTimeGregorian();

    geomagneticVectorStruct.latitude = satelliteLLH[0] * 180 / PI;
    geomagneticVectorStruct.longitude = satelliteLLH[1] * 180 / PI;
    geomagneticVectorStruct.altitude = satelliteLLH[2] / 1000;
    geomagneticVectorStruct.currentDate = timeGregorian;

    geomag(&geomagneticVectorStruct);

    magneticField(0) = geomagneticVectorStruct.xMagneticField;
    magneticField(1) = geomagneticVectorStruct.yMagneticField;
    magneticField(2) = geomagneticVectorStruct.zMagneticField;
    magneticField = ned2ecef(magneticField, satelliteLLH[0], satelliteLLH[1]);
    magneticField = ecef2eci(magneticField, gstime);

    Eigen::Vector3f sunPositionECI = calculateSunPosition(julianDate);
    sunPosition = sunPositionECI;

    isEclipse = calculateEclipse(satellitePosition, sunPositionECI);

    Vector3f satelliteECEF = eci2ecef(satellitePosition, gstime);
    Vector3f sunECEF = eci2ecef(sunPositionECI, gstime);
    satellitePosition *= 1000;
    satelliteECEF *= 1000;

    albedo = calculateAlbedo(satelliteECEF, sunECEF, reflectivityData);

}