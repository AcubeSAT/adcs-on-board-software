
#include "EciToEcef.h"
#include <cmath>

tow_vectors eci_to_ecef(Eigen::Vector3f vec_eci, Eigen::Vector3f v_vec_eci, double gstime) {

    tow_vectors vec_vvec_ecef;
    double OMEGAE = 7.292115860000000e-05;
    double CGAST = cos(gstime);
    double SGAST = sin(gstime);
    Eigen::Vector3d vec_ecef;
    Eigen::Vector3d v_vec_ecef;

    vec_ecef[0] = vec_eci[0] * CGAST + vec_eci[1] * SGAST;
    vec_ecef[1] = -vec_eci[0] * SGAST + vec_eci[1] * CGAST;
    vec_ecef[2] = vec_eci[2];

    v_vec_ecef[0] = v_vec_eci[0] * CGAST + v_vec_eci[1] * SGAST + OMEGAE * vec_ecef[1];
    v_vec_ecef[1] = -v_vec_eci[0] * SGAST + v_vec_eci[1] * CGAST - OMEGAE * vec_ecef[0];
    v_vec_ecef[2] = v_vec_eci[2];

    vec_vvec_ecef.vec_one = vec_ecef;
    vec_vvec_ecef.vec_two = v_vec_ecef;

    return vec_vvec_ecef;

}


