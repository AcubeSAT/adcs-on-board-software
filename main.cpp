#include "mathFunctions.h"
#include <cmath>
#include <cstring>
#include "TLEUtils.h"
#include "sgp4.h"
#include "Eigen/Dense"
#include "SatelliteModel.h"

#define STATE_SIZE 6
#define dt 0.1
#define DAY_MINUTES 1440

using namespace Eigen;

int main() {

    float timestep = 0.1;
    SatelliteModel model = SatelliteModel(timestep);

    const Vector<float, 3> magRef(-0.7701, -0.2939, -0.5662);
    const Vector<float, 3> sunRef(-0.1722, 0.9038, 0.3918);
    const Vector<float, 3> gyro(0.0024, 0.1432, -0.0972);

    int eclipse = 0;

    Vector<float, 7> state(0.4524, -0.1166, 0.8841, -0.0084, 0, 0, 0);
    const Vector<float, 3> gyro_measurements(0.0024, 0.1432, -0.0972);
    Quaternionf quat(0.4095, -0.0430, 0.9095, 0.0571);

    state(0) = quat.w();
    state(seq(1, 3)) = quat.vec();

    // %%%%%%%%%%%%%%%%%%%%%%%%% SGP4 %%%%%%%%%%%%%%%%%%%%%%%%%%%%

    char typeRun, typeInput, opsMode;
    gravconsttype setOfConstants;

    double position[3];
    double velocity[3];

    // ----------------------------  locals  -------------------------------
    double semilatusRectum, semimajorAxis, eccentricity, inclination, ascendingNode, argPerigee, trueAnomaly, meanAnomaly,
            argLatitude, trueLongitude, longitudePeriapsis;
    double sec, jd, jdFrac, tsince, startmfe, stopmfe, deltamin;
    int year, month, day, hour, minute;
    char TLEStr1[TLELineSize];
    char TLEStr2[TLELineSize];
    elsetrec satelliteRecord;

    std::cout << SGP4Version;
    char *TLEFile;

    // ####### TEST JDAY START########
    year = 2017;
    month = 8;
    day = 23;
    hour = 12;
    minute = 15;
    sec = 16.0;

    SGP4Funcs::jday_SGP4(year, month, day, hour, minute, sec, jd, jdFrac);

    double total = jd + jdFrac;
    SGP4Funcs::invjday_SGP4(total, 0.0, year, month, day, hour, minute, sec);

    // ####### TEST JDAY END ########

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

    //--- Import strings without accessing TLE file ---
    TLEFile = (char *) "SSO-500-11PM";
    strcpy(TLEStr1, tle11PM500.first.c_str());
    strcpy(TLEStr2, tle11PM500.second.c_str());

    SGP4Funcs::twoline2rv(TLEStr1, TLEStr2, typeRun, typeInput, opsMode, setOfConstants,
                          startmfe, stopmfe, deltamin, satelliteRecord);
    SGP4Funcs::sgp4(satelliteRecord, 0.0, position, velocity);

    jd = satelliteRecord.jdsatepoch;
    jdFrac = satelliteRecord.jdsatepochF;
    SGP4Funcs::invjday_SGP4(jd, jdFrac, year, month, day, hour, minute, sec);
    std::cout << "\n*******************************\n";
    std::cout << "TLEFile: " << TLEFile << "\nDate: " << year << "\t" << month << "\t" << day << "\nTime: " <<
              hour << "\t" << minute << "\t" << sec;
    std::cout << "\n*******************************\n";

    std::cout << "Ephemeris_Time: " << satelliteRecord.t << "\nPosition: " <<
              position[0] << "\t" << position[1] << "\t" << position[2] << "\nVelocity: " <<
              velocity[0] << "\t" << velocity[1] << "\t" << velocity[2];
    std::cout << "\n*******************************\n";

    tsince = startmfe;
    if (fabs(tsince) > 1.0e-8) {
        tsince -= deltamin;
    }

    // ----------------- loop to perform the propagation ----------------
    while ((tsince < stopmfe) && (satelliteRecord.error == 0)) {
        tsince = tsince + deltamin;

        if (tsince > stopmfe) {
            tsince = stopmfe;
        }
        SGP4Funcs::sgp4(satelliteRecord, tsince, position, velocity);

        if (satelliteRecord.error > 0) {
            std::cout << "# *** error: t:= " << satelliteRecord.t << "*** code = %3d\n" << satelliteRecord.error;
        }

        if (satelliteRecord.error == 0) {
            if ((typeRun != 'v') && (typeRun != 'c')) {
                jd = satelliteRecord.jdsatepoch;
                jdFrac = satelliteRecord.jdsatepochF + tsince / DAY_MINUTES;
                if (jdFrac < 0.0) {
                    jd = jd - 1.0;
                    jdFrac = jdFrac + 1.0;
                }
                SGP4Funcs::invjday_SGP4(jd, jdFrac, year, month, day, hour, minute, sec);

            } else {
                jd = satelliteRecord.jdsatepoch;
                jdFrac = satelliteRecord.jdsatepochF + tsince / DAY_MINUTES;
                if (jdFrac < 0.0) {
                    jd -= 1.0;
                    jdFrac += 1.0;
                }
                SGP4Funcs::invjday_SGP4(jd, jdFrac, year, month, day, hour, minute, sec);

                std::cout << "\n-/-/-/-/-/-/-/-/-/-/-\nEphemeris_Time: " << tsince * 60.0 <<
                          "\nPosition: " << position[0] << "\t" << position[1] << "\t" << position[2] <<
                          "\nVelocity: " << velocity[0] << "\t" << velocity[1] << "\t" << velocity[2]
                          << "\n\n"; // *60.0 can be omitted

                SGP4Funcs::rv2coe_SGP4(position, velocity, satelliteRecord.mus, semilatusRectum, semimajorAxis,
                                       eccentricity, inclination,
                                       ascendingNode, argPerigee, trueAnomaly, meanAnomaly, argLatitude, trueLongitude,
                                       longitudePeriapsis);

                std::cout << "A: " << semimajorAxis << "\nEcc: " << eccentricity << "\nIncl: " << rad2deg(inclination)
                          <<
                          "\nNode: " << rad2deg(ascendingNode) << "\nArgp: " << rad2deg(argPerigee) << "\nNu: "
                          << rad2deg(trueAnomaly) <<
                          "\nM: " << rad2deg(meanAnomaly) << "\nDate: " << year << "\t" << month << "\t" << day <<
                          "\nTime: " << hour << "\t" << minute << "\t" << sec << "\n-/-/-/-/-/-/-/-/-/-/-\n";
            }
        }
    }
    return 0;
}