

#ifndef ADCS_ONBOARD_SOFTWARE_ECITOECEF_H
#define ADCS_ONBOARD_SOFTWARE_ECITOECEF_H

#include "Eigen/Dense"
#include <cmath>

struct tow_vectors {
    Eigen::Vector3f vec_one;
    Eigen::Vector3f vec_two;
};

tow_vectors eci_to_ecef(Eigen::Vector3f vec_eci, Eigen::Vector3f v_vec_eci, double gstime);

#endif //ADCS_ONBOARD_SOFTWARE_ECITOECEF_H
