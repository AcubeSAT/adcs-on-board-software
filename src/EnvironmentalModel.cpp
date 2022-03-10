#include "EnvironmentalModel.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

void EnvironmentalModel::calculateEclipse(Vector3f xSatelliteECI, Vector3f sunPositionECI) {
    const double EarthRatio = 6371;
    const double SunRatio = 696000;
    const double AU = 149600000;
    const double alpha1 = M_PI - acos(EarthRatio / (EarthRatio * AU / (SunRatio + EarthRatio))) -
                    acos(EarthRatio / (xSatelliteECI.norm()));
    const double alpha2 =
            acos(EarthRatio / (EarthRatio * AU / (SunRatio - EarthRatio))) - acos(EarthRatio / (xSatelliteECI).norm());
    const double alpha = M_PI - acos(sunPositionECI.dot(xSatelliteECI) / ((sunPositionECI).norm() * (xSatelliteECI).norm()));

    if ((alpha2 < alpha) && (alpha < alpha1)) {
        isEclipse = true;
    } else if (alpha < alpha2) {
        isEclipse = true;
    } else {
        isEclipse = false;
    }
}

void EnvironmentalModel::calculateSunPosition(double time) {
    // we convert time to ut1 which is Jan 1, 2000 12 h epoch
    const double ut1 = (time - 2451545) / 36525;
    double meanLong = 280.4606184 + 36000.77005361 * ut1;
    double meanAnomaly = 357.5277233 + 35999.05034 * ut1;
    double eclipticLongitude;
    double obliquity;
    double magnitude;

    meanLong = std::fmod((meanLong), (360));
    meanAnomaly = std::fmod((meanAnomaly * M_PI / 180), (2 * M_PI));

    if (meanAnomaly < 0) {
        meanAnomaly = 2 * M_PI + meanAnomaly;
    }

    eclipticLongitude = meanLong + 1.91466471 * sin(meanAnomaly) + 0.019994643 * sin(2 * meanAnomaly);
    obliquity = 23.439291 - 0.0130042 * ut1;
    meanLong = meanLong * M_PI / 180;

    if (meanLong < 0) {
        meanLong = 2 * M_PI + meanLong;
    }

    eclipticLongitude = eclipticLongitude * M_PI / 180;
    obliquity = obliquity * M_PI / 180;
    magnitude = 1.000140612 - 0.016708617 * cos(meanAnomaly) - 0.000139589 * cos(2 * meanAnomaly);

    sunPosition[0] = magnitude * cos(eclipticLongitude);
    sunPosition[1] = magnitude * cos(obliquity) * sin(eclipticLongitude);
    sunPosition[2] = magnitude * sin(obliquity) * sin(eclipticLongitude);

}

EnvironmentalModel::EnvironmentalModel(OrbitalParameters orbitalParameters,
                                       EarthCellsMatrix reflectivityData)
        : orbitalParameters(orbitalParameters), reflectivityData{reflectivityData},
          isEclipse{false},
          sunPosition{{0, 0, 0}},
          albedo{EarthCellsMatrix::Zero()},
          magneticField{{0, 0, 0}},
          geomagneticVectorStruct{.currentDate = 0, .latitude = 0, .longitude = 0, .altitude = 0, .xMagneticField = 0, .yMagneticField = 0, .zMagneticField = 0, .norm = 0, .declination = 0, .inclination = 0, .horizontalIntensity = 0, .totalIntensity = 0} {}

void EnvironmentalModel::ModelEnvironment() {
    orbitalParameters.calculateNextPosition();

    satellitePosition = orbitalParameters.getPosition();

    const double julianDate = orbitalParameters.getJulianDate();
    const double greenwichSiderealTime = orbitalParameters.getGreenwichSiderealTime();
    const double timeGregorian = orbitalParameters.getTimeGregorian();

    const Vector3f satelliteLLH = orbitalParameters.getSatelliteLLH();

    geomagneticVectorStruct.latitude = satelliteLLH(0) * 180 / PI;
    geomagneticVectorStruct.longitude = satelliteLLH(1) * 180 / PI;
    geomagneticVectorStruct.altitude = satelliteLLH(2) / 1000;

    geomagneticVectorStruct.currentDate = timeGregorian;

    geomag(&geomagneticVectorStruct);

    magneticField(0) = geomagneticVectorStruct.xMagneticField;
    magneticField(1) = geomagneticVectorStruct.yMagneticField;
    magneticField(2) = geomagneticVectorStruct.zMagneticField;

    magneticField = ned2ecef(magneticField, satelliteLLH(0), satelliteLLH(1));
    magneticField = ecef2eci(magneticField, greenwichSiderealTime);

    calculateSunPosition(julianDate);

    calculateEclipse(satellitePosition, sunPosition);

    Vector3f satelliteECEF = eci2ecef(satellitePosition, greenwichSiderealTime);
    Vector3f sunPositionECEF = eci2ecef(sunPosition, greenwichSiderealTime);

    satellitePosition *= 1000;
    satelliteECEF *= 1000;

    albedo = calculateAlbedo(satelliteECEF, sunPositionECEF, reflectivityData);
}