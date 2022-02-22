#include "OrbitalParameters.hpp"
#include "MathFunctions.hpp"
#include "Eigen/Core"

inline constexpr double const EarthRatio = 6371;
inline constexpr double const SunRatio = 696000;
inline constexpr double const AU = 149600000;
using namespace Eigen;

bool calculateEclipse(Vector3f xSatelliteECI, Vector3f sunPositionECI) {
    bool eclipse;
    double alpha1 = M_PI - acos(EarthRatio / (EarthRatio * AU / (SunRatio + EarthRatio))) -
                    acos(EarthRatio / (xSatelliteECI.norm()));
    double alpha2 =
            acos(EarthRatio / (EarthRatio * AU / (SunRatio - EarthRatio))) - acos(EarthRatio / (xSatelliteECI).norm());
    double alpha = M_PI - acos(sunPositionECI.dot(xSatelliteECI) / ((sunPositionECI).norm() * (xSatelliteECI).norm()));

    if ((alpha2 < alpha) && (alpha < alpha1)) {
        eclipse = true;
    } else if (alpha < alpha2) {
        eclipse = true;
    } else {
        eclipse = false;
    }
    return eclipse;
}

Vector3f calculateSunPosition(double time) {
    Vector3f sunPositionECI(3);
    double ut1 = (time - 2451545) / 36525;
    double meanlong = 280.4606184 + 36000.77005361 * ut1;
    double meananomaly = 357.5277233 + 35999.05034 * ut1;
    double eclipselong;
    double obliquity;
    double magnitude;

    meanlong = std::fmod((meanlong), (360));
    meananomaly = std::fmod((meananomaly * M_PI / 180), (2 * M_PI));

    if (meananomaly < 0) {
        meananomaly = 2 * M_PI + meananomaly;
    }

    eclipselong = meanlong + 1.91466471 * sin(meananomaly) + 0.019994643 * sin(2 * meananomaly);
    obliquity = 23.439291 - 0.0130042 * ut1;
    meanlong = meanlong * M_PI / 180;

    if (meanlong < 0) {
        meanlong = 2 * M_PI + meanlong;
    }

    eclipselong = eclipselong * M_PI / 180;
    obliquity = obliquity * M_PI / 180;
    magnitude = 1.000140612 - 0.016708617 * cos(meananomaly) - 0.000139589 * cos(2 * meananomaly);

    sunPositionECI[0] = magnitude * cos(eclipselong);
    sunPositionECI[1] = magnitude * cos(obliquity) * sin(eclipselong);
    sunPositionECI[2] = magnitude * sin(obliquity) * sin(eclipselong);

    return sunPositionECI;

}

OrbitalParameters::OrbitalParameters() {
    julianDay = 0;
    tsince = 0;
    position = {0, 0, 0};
    gstime = 0;
    timeGregorian = 0;
    satelliteLLH = {0, 0, 0};
}


void
OrbitalParameters::calculateTime(const TLE &tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst) {
    int_fast16_t Eyear;
    int timeDay;
    int mon;
    int day;
    int hr;
    int minute;
    double sec;

    double stopmfe;
    double deltamin;

    char tle1[TLELineSize];
    char tle2[TLELineSize];

    strcpy(tle1, tle.first.c_str());
    strcpy(tle2, tle.second.c_str());


    SGP4Funcs::twoline2rv(tle1, tle2,
                          typerun, typeinput, opsmode, whichconst,
                          tsince, stopmfe, deltamin, satrec);

    julianDay = satrec.jdsatepoch + satrec.t / 1440;


    Eyear = satrec.epochyr + 2000;
    timeDay = satrec.epochdays + tsince / 1440;
    SGP4Funcs::days2mdhms_SGP4(Eyear, timeDay, mon, day, hr, minute, sec);
    timeGregorian = date2decimal(Eyear, mon, day, hr, minute, sec);
}
void OrbitalParameters::calculateNextPosition() {
    double xsatelliteECI[3];
    double velocity[3];

    SGP4Funcs::sgp4(satrec, tsince, xsatelliteECI, velocity);

    for (uint8_t i = 0; i < 3; i++) {
        position(i) = xsatelliteECI[i];
    }

    julianDay = satrec.jdsatepoch + satrec.t / 1440;
    gstime = SGP4Funcs::gstime_SGP4(julianDay);

    Vector3f satelliteECEF = eci2ecef(position, gstime);
    satelliteLLH = ecef2llh(satelliteECEF * 1000);
}