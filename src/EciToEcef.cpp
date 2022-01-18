
#include "EciToEcef.hpp"
#include <cmath>

Eigen::Vector3d eci_to_ecef(Eigen::Vector3d vec_eci, double gstime) {


    double OMEGAE = 7.292115860000000e-05;
    double CGAST = cos(gstime);
    double SGAST = sin(gstime);
    Eigen::Vector3d vec_ecef;

    vec_ecef[0] = vec_eci[0] * CGAST + vec_eci[1] * SGAST;
    vec_ecef[1] = -vec_eci[0] * SGAST + vec_eci[1] * CGAST;
    vec_ecef[2] = vec_eci[2];


    return vec_ecef;


}


