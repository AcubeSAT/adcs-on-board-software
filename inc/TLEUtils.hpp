#ifndef ADCS_ONBOARD_SOFTWARE_TLEUTILS_H
#define ADCS_ONBOARD_SOFTWARE_TLEUTILS_H

#include "etl/string.h"

const int TLELineSize = 70;
static_assert(sizeof(int) >= 4, "Integer variables not large enough to support SGP4");
typedef std::pair<etl::string<TLELineSize>, etl::string<TLELineSize>> TLE;

/**
 * Altitude: 500km
 * LTAN: 6PM
 */
const TLE tle6PM500 = {
        "1 69696U 16025E   23183.00000000  .00002000  00000-0  49111-4 0  6969",
        "2 69696  97.3759 191.5890 0001000 00.0000 000.0000 15.24261762696969"
};

/**
 * Altitude: 500km
 * LTAN: 11PM
 */
const TLE tle11PM500 = {
        "1 69696U 16025E   23183.00000000  .00002000  00000-0  49111-4 0  6969",
        "2 69696  97.3759 266.5890 0001000 00.0000 000.0000 15.24261762696969"
};

/**
 * Altitude: 600km
 * LTAN: 6PM
 */
const TLE tle6PM600 = {
        "1 69696U 16025E   23183.00000000  .00002000  00000-0  49111-4 0  6969",
        "2 69696  97.7640 191.5890 0001000 00.0000 000.0000 14.89144204204206"
};

/**
 * Altitude: 600km
 * LTAN: 11PM
 */
const TLE tle11PM600 = {
        "1 69696U 16025E   23183.00000000  .00002000  00000-0  49111-4 0  6969",
        "2 69696  97.7640 266.5890 0001000 00.0000 000.0000 14.89144204204206"
};

#endif //ADCS_ONBOARD_SOFTWARE_TLEUTILS_H
