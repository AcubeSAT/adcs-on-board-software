
#ifndef ADCS_ONBOARD_SOFTWARE_ECLIPSE_HPP
#define ADCS_ONBOARD_SOFTWARE_ECLIPSE_HPP

#include "Eigen/Dense"

#define R_EARTH 6371
#define R_SUN 696000
#define AU 149600000


bool calculateEclipse(Eigen::Vector3f xSatelliteECI, Eigen::Vector3f sunPositionECI);


#endif //ADCS_ONBOARD_SOFTWARE_ECLIPSE_HPP
