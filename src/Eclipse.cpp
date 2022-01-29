
#include "Eclipse.hpp"

using namespace Eigen;

bool calculateEclipse(Vector3f xSatelliteECI, Vector3f sunPositionECI) {
    bool eclipse;
    double x1 = R_EARTH * AU / (R_SUN + R_EARTH);
    double x2 = R_EARTH * AU / (R_SUN - R_EARTH);
    double alpha1 = M_PI - acos(R_EARTH / x1) - acos(R_EARTH / (xSatelliteECI.norm()));
    double alpha2 = acos(R_EARTH / x2) - acos(R_EARTH / (xSatelliteECI).norm());
    double alpha = M_PI - acos(sunPositionECI.dot(xSatelliteECI) / ((sunPositionECI).norm() * (xSatelliteECI).norm()));

    if ((alpha2 < alpha) && (alpha < alpha1)) {
        eclipse = true;
    } else if (alpha < alpha2) {
        eclipse = true;
    } else {
        eclipse = false;
    }
    return eclipse;
}
