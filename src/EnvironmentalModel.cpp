
#include "EnvironmentalModel.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

EnvironmentalModel::EnvironmentalModel(OrbitalParameters orbitalParameters) : orbitalParameters(orbitalParameters) {
}

void EnvironmentalModel::ModelEnvironmental() {
    orbitalParameters.calculateNextPosition();
    Vector3d xsat_eci = orbitalParameters.get_position();
    double julianDate = orbitalParameters.get_jd();
    double gstime = orbitalParameters.getGstime();

    /**
     ** TODO: check timesteps
     */

    geomag(&gStr);

    Eigen::Vector3d sun_pos_eci;

    // jd = time (im not sure)
    sun_pos_eci = sun_position(julianDate);

    eclipse = calculate_eclipse(xsat_eci, sun_pos_eci);


    Eigen::Vector3d sat_ecef = eci_to_ecef(xsat_eci, gstime);

    Eigen::Vector3d satellite;
    Eigen::Vector3d sunPosition;

    satellite = sat_ecef;

    Eigen::Vector3d sun_ecef = eci_to_ecef(sun_pos_eci, gstime);
    sunPosition = sun_ecef;
    /**
     ** TODO: albedo takes Vectors3f not 3d
     */
    Eigen::Vector3f sat_vec_3f;
    Eigen::Vector3f sun_vec_3f;

    sat_vec_3f[0] = satellite[0];
    sat_vec_3f[1] = satellite[1];
    sat_vec_3f[2] = satellite[2];

    sun_vec_3f[0] = sunPosition[0];
    sun_vec_3f[1] = sunPosition[1];
    sun_vec_3f[2] = sunPosition[2];

    Eigen::Matrix<float, ReflectivityDataRows, ReflectivityDataColumns> alb = calculateAlbedo(sat_vec_3f, sun_vec_3f,
                                                                                              reflectivityData);

}
