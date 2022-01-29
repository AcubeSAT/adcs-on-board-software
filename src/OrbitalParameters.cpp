#include "OrbitalParameters.hpp"
#include "MathFunctions.hpp"
#include "Eigen/Core"

using namespace Eigen;

OrbitalParameters::OrbitalParameters(){
    julianDay = 0;
    tsince = 0;
    position = {0, 0, 0};
    gstime = 0;
    timeGregorian = 0;
    satelliteLLH = {0, 0, 0};
}

void OrbitalParameters::calculateTime(const TLE tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst) {
    int Eyear;
    int timeDay;
    int mon;
    int day;
    int hr;
    int minute;
    double sec;

    double stopmfe;
    double deltamin;
    //convert tle from string to char
    int i;
    char tle1[70];
    char tle2[70];
    for (i = 0; i < 70; i++) {
        tle1[i] = tle.first[i];
    }
    for (i = 0; i < 70; i++) {
        tle2[i] = tle.second[i];
    }

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

    Eigen::Vector3f satelliteECEF = eci2ecef(position, gstime);
    satelliteLLH = ecef2llh(satelliteECEF);
}