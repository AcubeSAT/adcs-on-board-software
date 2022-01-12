
#include "EnvironmenralModel.hpp"
#include "EciToEcef.h"

Eigen::Vector3d EnvironmentalModel::SunPosition() {

    SGP4Funcs::jday_SGP4(year, mon, day, hr, minute, sec, jd, jdFrac);

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

    /* check this ... im not sure if position changes or not  */

    Eigen::Vector3f sun_pos_eci;
    bool ecl = calculate_eclipse(xsat_eci, sun_position(jd));
    return ecl;
}


Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> EnvironmentalModel::Albedo() {
    Eigen::Vector3f xsat_eci;

    SGP4Funcs::sgp4(satrec, tsince, position, velocity);

    /* check this ... im not sure if position changes or not  */

    xsat_eci(0) = position[0];
    xsat_eci(1) = position[1];
    xsat_eci(2) = position[2];

    //velocity vector
    Eigen::Vector3f v;
    v(0) = 0;
    v(1) = 0;
    v(2) = 0;

    //im not sure if jdut1 = time
    double gstime = SGP4Funcs::gstime_SGP4(jd);

    tow_vectors sat_ecef = eci_to_ecef(xsat_eci, v, gstime);

    Eigen::Vector3f satellite;
    Eigen::Vector3f sunPosition;

    satellite = sat_ecef.vec_one;

    tow_vectors sun_ecef = eci_to_ecef(sun_position(jd), v, gstime);

    sunPosition = sun_ecef.vec_one;

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> alb = calculateAlbedo(satellite, sunPosition,
                                                                                              reflectivityData);


    return alb;
}

void EnvironmentalModel::MagneticField() {

    geomag(&gStr);

}