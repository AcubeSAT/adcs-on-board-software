

#ifndef ADCS_ONBOARD_SOFTWARE_ECLIPSE_HPP
#define ADCS_ONBOARD_SOFTWARE_ECLIPSE_HPP

#include "Eigen/Dense"

#define R_EARTH 6371
#define R_SUN 696000
#define AU 149600000

bool calculate_eclipse(Eigen::Vector3d xsat_eci, Eigen::Vector3d sun_pos_eci);


#endif //ADCS_ONBOARD_SOFTWARE_ECLIPSE_HPP
