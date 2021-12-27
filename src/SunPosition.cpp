#include "SunPosition.hpp"
#include <cmath>
#include <vector>

foo sun_position(double time) {
    std::vector<double> v(3);
    double ut1 = (time - 2451545) / 36525;
    double meanlong = 280.4606184 + 36000.77005361 * ut1;
    double meananomaly = 357.5277233 + 35999.05034 * ut1;
    double eclplong;
    double obliquity;
    double magr;
    double rtasc;
    foo res;
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

    v[1] = magr * cos(eclplong);
    v[2] = magr * cos(obliquity) * sin(eclplong);
    v[3] = magr * sin(obliquity) * sin(eclplong);
    res.sun_pos_eci = v;

    rtasc = atan(cos(obliquity) * tan(eclplong));

    if (eclplong < 0) {
        eclplong = eclplong + 2 * M_PI;
    }

    if (abs(eclplong - rtasc) > (M_PI / 2)) {
        if (std::fmod((eclplong - rtasc), 1) < 0.51) {
            rtasc = rtasc + 0.5 * M_PI * (eclplong - rtasc) - (eclplong - std::fmod(rtasc, 1) / (M_PI / 2));
        } else {
            rtasc = rtasc + 0.5 * M_PI * ((eclplong - rtasc) + (1 - std::fmod((eclplong - rtasc), 1))) / (M_PI / 2);
        }

    }

    res.decl = asin(sin(obliquity) * sin(eclplong));
    return res;

}
