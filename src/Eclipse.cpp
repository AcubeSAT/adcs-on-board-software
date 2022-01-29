
#include "Eclipse.hpp"

using namespace Eigen;

bool calculate_eclipse(Vector3f xsat_eci, Vector3f sun_pos_eci) {
    bool eclipse;
    double x1 = R_EARTH * AU / (R_SUN + R_EARTH);
    double x2 = R_EARTH * AU / (R_SUN - R_EARTH);
    double alpha1 = M_PI - acos(R_EARTH / x1) - acos(R_EARTH / (xsat_eci.norm()));
    double alpha2 = acos(R_EARTH / x2) - acos(R_EARTH / (xsat_eci).norm());
    double alpha = M_PI - acos(sun_pos_eci.dot(xsat_eci) / ((sun_pos_eci).norm() * (xsat_eci).norm()));

    if ((alpha2 < alpha) & (alpha < alpha1)) {
        eclipse = true;
    } else if (alpha < alpha2) {
        eclipse = true;
    } else {
        eclipse = false;
    }
    return eclipse;
}
