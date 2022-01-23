#include "OrbitalParameters.hpp"
#include "MathFunctions.hpp"
#include "Eigen/Core"

using namespace Eigen;

OrbitalParameters::OrbitalParameters(){
    jd = 0;
    tsince = 0;
    position = {0, 0, 0};
    gstime = 0;
    time_gregorian = 0;
    sat_llh  = {0, 0, 0};
}

void OrbitalParameters::calculateTime(const TLE tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst) {
    int Eyear;
    int time_day;
    int mon;
    int day;
    int hr;
    int minute;
    double sec;
    double time_gregorian;

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

    jd = satrec.jdsatepoch + satrec.t / 1440;

    //calculate gregorian time
    Eyear = satrec.epochyr + 2000;
    time_day = satrec.epochdays + tsince / 1440;
    SGP4Funcs::days2mdhms_SGP4(Eyear, time_day, mon, day, hr, minute, sec);
    time_gregorian = date2decimal(Eyear, mon, day, hr, minute, sec);   
}


void OrbitalParameters::calculateNextPosition() {
    double xsat_eci[3];
    double velocity[3];

    SGP4Funcs::sgp4(satrec, tsince, xsat_eci, velocity);

    /**
     * TODO: typecast from double array[3] to Vector3f 
     */
    for (uint8_t i = 0; i < 3; i++) {
        position(i) = xsat_eci[i];
    }

    jd = satrec.jdsatepoch + satrec.t / 1440;
    gstime = SGP4Funcs::gstime_SGP4(jd);

    Eigen::Vector3f sat_ecef = eci_to_ecef(position, gstime);
    Eigen::Vector3f sat_llh = ecef_to_llh(sat_ecef);
}