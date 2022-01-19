
#ifndef ADCS_ONBOARD_SOFTWARE_ECEFTOLLH_HPP
#define ADCS_ONBOARD_SOFTWARE_ECEFTOLLH_HPP

#include "Eigen/Dense"


Eigen::Vector3d ecef_to_llh(Eigen::Vector3d uvw);

#endif //ADCS_ONBOARD_SOFTWARE_ECEFTOLLH_HPP
