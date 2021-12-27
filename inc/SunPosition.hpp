
#ifndef ADCS_ONBOARD_SOFTWARE_SUNPOSITION_HPP
#define ADCS_ONBOARD_SOFTWARE_SUNPOSITION_HPP

#include "Eigen/Dense"
#include <cmath>
#include <vector>

struct foo {
    std::vector<double> sun_pos_eci;
    double decl;
};

foo sun_position(double time);

#endif //ADCS_ONBOARD_SOFTWARE_SUNPOSITION_HPP


