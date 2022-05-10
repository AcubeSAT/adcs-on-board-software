#include "OrbitalParameters.hpp"
#include "MathFunctions.hpp"
#include "Eigen/Core"

using namespace Eigen;

OrbitalParameters::OrbitalParameters() :
        julianDate{0},
        timeSince{0},
        satelliteECI{{0, 0, 0}},
        greenwichSiderealTime{0},
        timeGregorian{0},
        satelliteLLH{{0, 0, 0}} {}

void
OrbitalParameters::calculateTime(const TLE &tle, const char typerun, const char typeinput, const char opsmode, const gravconsttype whichconst) {
    uint16_t EYear;
    int timeDay;
    int month, day, hour, minute;
    double sec, stopMfe, deltaMin;

    char tle1[TLELineSize];
    char tle2[TLELineSize];

    strcpy(tle1, tle.first.c_str());
    strcpy(tle2, tle.second.c_str());

    SGP4Funcs::twoline2rv(tle1, tle2,
                          typerun, typeinput, opsmode, whichconst,
                          timeSince, stopMfe, deltaMin, satrec);

    julianDate = satrec.jdsatepoch + satrec.t / 1440;


    EYear = satrec.epochyr + 2000;
    timeDay = satrec.epochdays + timeSince / 1440;
    SGP4Funcs::days2mdhms_SGP4(EYear, timeDay, month, day, hour, minute, sec);
    timeGregorian = date2decimal(EYear, month, day, hour, minute, sec);
}

void OrbitalParameters::calculateNextPosition() {
    double satelliteECI[3];
    double velocity[3];

    SGP4Funcs::sgp4(satrec, timeSince, satelliteECI, velocity);

    // The purpose of the next operation is to "typecast" c++ arrays to Eigen vector
    for (uint8_t i = 0; i < 3; i++) {
        this->satelliteECI(i) = satelliteECI[i];
    }

    julianDate = satrec.jdsatepoch + satrec.t / 1440;
    greenwichSiderealTime = SGP4Funcs::gstime_SGP4(julianDate);

    const Vector3f satelliteECEF = eci2ecef(this->satelliteECI, greenwichSiderealTime);
    satelliteLLH = ecef2llh(satelliteECEF * 1000);
    timeSince = timeSince + 0.1 / 60;
}