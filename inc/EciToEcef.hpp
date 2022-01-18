

#ifndef ADCS_ONBOARD_SOFTWARE_ECITOECEF_HPP
#define ADCS_ONBOARD_SOFTWARE_ECITOECEF_HPP

#include "Eigen/Dense"
#include <cmath>


Eigen::Vector3d eci_to_ecef(Eigen::Vector3d vec_eci, double gstime);

#endif //ADCS_ONBOARD_SOFTWARE_ECITOECEF_HPP
