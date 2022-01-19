
#include "EnvironmentalModel.hpp"
#include "EciToEcef.hpp"
#include "MathFunctions.hpp"
#include "EcefToLlh.hpp"


Eigen::Vector3d EnvironmentalModel::SunPosition() {

    //time = jd;

    Eigen::Vector3d sun_pos_eci = sun_position(jd);
    return sun_pos_eci;

}


bool EnvironmentalModel::Eclipse() {
    Eigen::Vector3f xsat_eci;

    SGP4Funcs::sgp4(satrec, tsince, position, velocity);

    xsat_eci(0) = position[0];
    xsat_eci(1) = position[1];
    xsat_eci(2) = position[2];


    Eigen::Vector3f sun_pos_eci;
    bool ecl = calculate_eclipse(xsat_eci, sun_position(jd));
    return ecl;
}


Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> EnvironmentalModel::Albedo() {
    Eigen::Vector3d xsat_eci;

    //I may need to initialize position and velocity , probably not
    SGP4Funcs::sgp4(satrec, tsince, position, velocity);


    xsat_eci(0) = position[0];
    xsat_eci(1) = position[1];
    xsat_eci(2) = position[2];

    // jd = time (im not sure)

    double gstime = SGP4Funcs::gstime_SGP4(jd);

    Eigen::Vector3d sat_ecef = eci_to_ecef(xsat_eci, gstime);

    Eigen::Vector3f satellite;
    Eigen::Vector3f sunPosition;

    satellite = sat_ecef;

    Eigen::Vector3d sun_ecef = eci_to_ecef(sun_position(jd), gstime);

    sunPosition = sun_ecef;

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> alb = calculateAlbedo(satellite, sunPosition,
                                                                                              reflectivityData);


    return alb;
}

void EnvironmentalModel::MagneticField() {

    geomag(&gStr);

}

void EnvironmentalModel::Init_TLE(const TLE tle, char typerun, char typeinput, char opsmode, gravconsttype whichconst,
                                  double &startmfe, double &stopmfe, double &deltamin, double tsince,
                                  Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> refData) {
    int Eyear;
    int time_day;
    int mon;
    int day;
    int hr;
    int minute;
    double sec;
    double time_gregorian;


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
                          startmfe, stopmfe, deltamin, satrec);

    //initialize jd = time
    jd = satrec.jdsatepoch + satrec.t / 1440;


    //initialize reflectiveData
    reflectivityData = refData;

    //initialize gStr needed for geomag

    //calculate gregorian time
    Eyear = satrec.epochyr + 2000;
    time_day = satrec.epochdays + tsince / 1440;
    SGP4Funcs::days2mdhms_SGP4(Eyear, time_day, mon, day, hr, minute, sec);
    time_gregorian = date2decimal(Eyear, time_day, day, hr, minute, sec);

    Eigen::Vector3d xsat_eci;

    //sat_llh
    SGP4Funcs::sgp4(satrec, tsince, position, velocity);
    xsat_eci(0) = position[0];
    xsat_eci(1) = position[1];
    xsat_eci(2) = position[2];
    double gstime = SGP4Funcs::gstime_SGP4(jd);
    Eigen::Vector3d sat_ecef = eci_to_ecef(xsat_eci, gstime);

    Eigen::Vector3d sat_llh = ecef_to_llh(sat_ecef);

    gStr = {.currentDate = 0, .latitude = 0, .longitude = 0,
            .altitude =0, .xMagneticField = 0, .yMagneticField = 0, .zMagneticField = 0,
            .norm = 0, .declination = 0, .inclination = 0, .horizontalIntensity = 0, .totalIntensity = 0};

    gStr.latitude = sat_llh[0];
    gStr.longitude = sat_llh[1];
    gStr.altitude = sat_llh[2];
    gStr.currentDate = time_gregorian;


}

void EnvironmentalModel::ModelEnvironmental(const TLE tle, char typerun, char typeinput, char opsmode,
                                            gravconsttype whichconst,
                                            double &startmfe, double &stopmfe, double &deltamin, double tsince,
                                            Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> refData) {

    Init_TLE(tle, typerun, typeinput, opsmode, whichconst, startmfe, stopmfe, deltamin, tsince, refData);
    MagneticField();
    eclipse_property = Eclipse();
    sun_position_property = SunPosition();
    albedo_property = Albedo();
}
