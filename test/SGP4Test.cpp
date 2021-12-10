#include <catch2/catch.hpp>
#include <cstring>
#include "TLEUtils.hpp"
#include "Definitions.hpp"
#include "MathFunctions.hpp"
#include "sgp4.h"

const int MinutesInDay = 1440;

TEST_CASE("Test conversion to Julian day") {

    int year, month, day, hour, minute;
    double sec;
    double jd, jdFrac;

    year = 2017;
    month = 8;
    day = 23;
    hour = 12;
    minute = 15;
    sec = 16.0;

    SGP4Funcs::jday_SGP4(year, month, day, hour, minute, sec, jd, jdFrac);

    year = 0;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
    sec = 0.0;
    double total = jd + jdFrac;
    SGP4Funcs::invjday_SGP4(total, 0.0, year, month, day, hour, minute, sec);

    REQUIRE(jd == Approx(2457988.5).epsilon(0.1));
    REQUIRE(jdFrac == Approx(0.510602).epsilon(0.000001));
    REQUIRE(year == 2017);
    REQUIRE(month == 8);
    REQUIRE(day == 23);
    REQUIRE(hour == 12);
    REQUIRE(minute == 15);
    REQUIRE(sec == Approx(16.0).epsilon(0.1));
}

TEST_CASE("Test the overall SGP4 propagator functionality") {

    int year, month, day, hour, minute;
    double sec;

    char TLEStr1[TLELineSize];
    char TLEStr2[TLELineSize];
    char *TLEFile;

    double position[3];
    double velocity[3];

    char typeRun, typeInput, opsMode;
    gravconsttype setOfConstants;

    double semilatusRectum, semimajorAxis, eccentricity, inclination, ascendingNode, argPerigee, trueAnomaly, meanAnomaly,
            argLatitude, trueLongitude, longitudePeriapsis;
    double jd, jdFrac, tsince, startmfe, stopmfe, deltamin;
    elsetrec satelliteRecord;

    TLEFile = (char *) "SSO-500-11PM";
    strcpy(TLEStr1, tle11PM500.first.c_str());
    strcpy(TLEStr2, tle11PM500.second.c_str());

    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    // 'a' --> best understanding of how afspc code works
    // 'i' --> improved sgp4 resulting in smoother behavior
    opsMode = 'a';

    // 'c' --> compare 1 year of full satcat data
    // 'm' --> manual operation- either mfe, epoch, or day of yr
    typeRun = 'c';

    // 'm' --> input start stop mfe
    // 'e' --> input start stop ymd hms
    // 'd' --> input start stop yr dayofyr
    typeInput = 'm';

    // 'wgs72' --> set of constant 72;
    // 'wgs84' --> set of constant 84;
    setOfConstants = wgs84;

    satelliteRecord.classification = 'U';
    strncpy(satelliteRecord.intldesg, "          ", 11);
    satelliteRecord.ephtype = 0;
    satelliteRecord.elnum = 0;
    satelliteRecord.revnum = 0;

    SGP4Funcs::twoline2rv(TLEStr1, TLEStr2, typeRun, typeInput, opsMode, setOfConstants,
                          startmfe, stopmfe, deltamin, satelliteRecord);
    SGP4Funcs::sgp4(satelliteRecord, 0.0, position, velocity);

    jd = satelliteRecord.jdsatepoch;
    jdFrac = satelliteRecord.jdsatepochF;
    SGP4Funcs::invjday_SGP4(jd, jdFrac, year, month, day, hour, minute, sec);

    REQUIRE(year == 2023);
    REQUIRE(month == 7);
    REQUIRE(day == 2);

    tsince = startmfe;
    if (fabs(tsince) > 1.0e-8) {
        tsince -= deltamin;
    }

    SGP4Funcs::sgp4(satelliteRecord, tsince, position, velocity);

    jd = satelliteRecord.jdsatepoch;
    jdFrac = satelliteRecord.jdsatepochF + tsince / MinutesInDay;
    if (jdFrac < 0.0) {
        jd -= 1.0;
        jdFrac += 1.0;
    }
    SGP4Funcs::invjday_SGP4(jd, jdFrac, year, month, day, hour, minute, sec);

    SGP4Funcs::rv2coe_SGP4(position, velocity, satelliteRecord.mus, semilatusRectum, semimajorAxis, eccentricity,
                           inclination,
                           ascendingNode, argPerigee, trueAnomaly, meanAnomaly, argLatitude, trueLongitude,
                           longitudePeriapsis);

    REQUIRE(rad2deg(inclination) == Approx(97.37).epsilon(0.01));
    REQUIRE(rad2deg(ascendingNode) == Approx(265).epsilon(1));
}
