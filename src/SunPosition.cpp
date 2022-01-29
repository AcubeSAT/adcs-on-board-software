#include "SunPosition.hpp"
#include <cmath>
#include <vector>

Eigen::Vector3f calculateSunPosition(double time) {
    Eigen::Vector3f sunPositionECI(3);
    double ut1 = (time - 2451545) / 36525;
    double meanlong = 280.4606184 + 36000.77005361 * ut1;
    double meananomaly = 357.5277233 + 35999.05034 * ut1;
    double eclplong;
    double obliquity;
    double magr;

    meanlong = std::fmod((meanlong), (360));
    meananomaly = std::fmod((meananomaly * M_PI / 180), (2 * M_PI));

    if (meananomaly < 0) {
        meananomaly = 2 * M_PI + meananomaly;
    }

    eclplong = meanlong + 1.91466471 * sin(meananomaly) + 0.019994643 * sin(2 * meananomaly);
    obliquity = 23.439291 - 0.0130042 * ut1;
    meanlong = meanlong * M_PI / 180;

    if (meanlong < 0) {
        meanlong = 2 * M_PI + meanlong;
    }

    eclplong = eclplong * M_PI / 180;
    obliquity = obliquity * M_PI / 180;
    magr = 1.000140612 - 0.016708617 * cos(meananomaly) - 0.000139589 * cos(2 * meananomaly);

    sunPositionECI[0] = magr * cos(eclplong);
    sunPositionECI[1] = magr * cos(obliquity) * sin(eclplong);
    sunPositionECI[2] = magr * sin(obliquity) * sin(eclplong);

    return sunPositionECI;

}
