#include "EcefToLlh.hpp"
#include <cmath>

Eigen::Vector3d ecef_to_llh(Eigen::Vector3d uvw) {
    Eigen::Vector3d llh;
    double lat;
    double re;
    double olatsav;
    double tmp2;
    double dlat = 1.0;
    //Set up WGS-84 constants.
    //Earth model
    double a = 6378137.0; //meters
    double f = 1.0 / 298.257223563;
    //eccentricity squared for WGS84.
    double ecc_sq = (2.0 - f) * f;

    double tmp1 = sqrt(pow(uvw[0], 2) + pow(uvw[1], 2));


    if (tmp1 == 0.0) {
        llh[1] = 0;//lon
        if (uvw[2] > 0.0) {
            llh[2] = uvw[2] - (a / sqrt(1.0 - ecc_sq));//hgt
            llh[0] = asin(1.0);//lat
        } else {
            llh[2] = -uvw[2] - (a / sqrt(1.0 - ecc_sq));
            llh[0] = asin(-1.0);
        }
    } else {
        llh[1] = atan2(uvw[1], uvw[0]);
        lat = atan2(uvw[2], tmp1);
        re = a;


        while (dlat > 1.0e-7) {
            olatsav = lat;
            tmp2 = uvw[2] + ecc_sq * re * sin(lat);
            lat = atan2(tmp2, tmp1);
            re = a / sqrt(1 - ecc_sq * pow(sin(lat), 2));
            dlat = abs(lat - olatsav);
        }
        llh[2] = tmp1 / cos(lat) - re;
        llh[0] = lat;
    }
    return llh;

}

