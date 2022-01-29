
#ifndef ADCS_ONBOARD_SOFTWARE_ECLIPSE_HPP
#define ADCS_ONBOARD_SOFTWARE_ECLIPSE_HPP

#include "Eigen/Dense"

inline constexpr auto const EarthRadius = 6371;
inline constexpr auto const SunRadious = 696000;
inline constexpr auto const AU = 149600000;


bool calculate_eclipse(Eigen::Vector3f xsat_eci, Eigen::Vector3f sun_pos_eci);


#endif //ADCS_ONBOARD_SOFTWARE_ECLIPSE_HPP
