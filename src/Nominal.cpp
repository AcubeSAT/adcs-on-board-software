#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"


using namespace Eigen;
using namespace Parameters::CovarianceMatrices;

void initializeNominalMode(EnvironmentalModel environmentalModel, MEKF &mekf, const SatelliteModel &satelliteModel,
                           Vector3f gyroscopeBias, Matrix<float, LocalStateSize, LocalStateSize> P,
                           MeasurementVector measurements) {
    int NumberOfTimestepsPerCycle = 10;
    Vector3f sunPositionBody, magneticBody, sunPositionECI, sunPositionECINormalized, satellitePositionECI;
    bool eclipse;
    float albedo;
    Quaternionf wahbaOutputQuaternion1, wahbaOutputQuaternion2;
    Vector3f magneticFieldECI;
    GlobalStateVector globalState;

    for (int i = 0; i < 3; i++) {
        magneticBody[i] = measurements[i];
        sunPositionBody[i] = measurements[i + 3];
    }
    sunPositionECI = environmentalModel.getSunPosition();
    magneticFieldECI = environmentalModel.getMagneticField();
    wahbaOutputQuaternion1 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);

    for (int i = 0; i < NumberOfTimestepsPerCycle; i++) {
        environmentalModel.ModelEnvironment();
    }
    satellitePositionECI = environmentalModel.getSatellitePosition();
    eclipse = environmentalModel.getIsEclipse();
    albedo = environmentalModel.getAlbedo().sum();
    sunPositionECI = environmentalModel.getSunPosition();
    magneticFieldECI = environmentalModel.getMagneticField();
    wahbaOutputQuaternion2 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
    globalState = {wahbaOutputQuaternion2.w(), wahbaOutputQuaternion2.x(), wahbaOutputQuaternion2.y(),
                   wahbaOutputQuaternion2.z(), gyroscopeBias(0), gyroscopeBias(1), gyroscopeBias(2)};
    mekf.setGlobalState(globalState);
    mekf.setQ(Q);
    mekf.setR(R);
    mekf.setP(P);
    mekf.correct(measurements, magneticFieldECI, sunPositionECI, eclipse, satelliteModel, satellitePositionECI, albedo);
}