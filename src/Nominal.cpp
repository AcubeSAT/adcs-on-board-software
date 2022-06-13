#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"
#include "Definitions.hpp"
#include "GyroBiasFunction.hpp"


using namespace Eigen;
using namespace Parameters::CovarianceMatrices;

void NominalMode(EnvironmentalModel environmentalModel, MEKF &mekf, const SatelliteModel &satelliteModel,
                           Vector3f gyroscopeMeasurement, Matrix<float, LocalStateSize, LocalStateSize> P,
                           MeasurementVector measurements) {
    Vector3f magneticFieldECI, sunPositionBody, magneticBody, sunPositionECI, satellitePositionECI, angularEstimatedRate, gyroscopeBias;
    bool eclipse;
    float albedo, angularEstimatedRateMean;
    Quaternionf wahbaOutputQuaternion1, wahbaOutputQuaternion2, quaternionDifference, temporaryQuaternion;
    GlobalStateVector globalState;

    for (int i = 0; i < 3; i++) {
        magneticBody[i] = measurements[i];
        sunPositionBody[i] = measurements[i + 3];
    }
    sunPositionECI = environmentalModel.getSunPosition();
    magneticFieldECI = environmentalModel.getMagneticField();
    wahbaOutputQuaternion1 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);

    for (int i = 0; i < NumberOfTimeStepsPerCycle; i++) {
        environmentalModel.ModelEnvironment();
    }
    satellitePositionECI = environmentalModel.getSatellitePosition();
    eclipse = environmentalModel.getIsEclipse();
    albedo = environmentalModel.getAlbedo();
    sunPositionECI = environmentalModel.getSunPosition();
    magneticFieldECI = environmentalModel.getMagneticField();
    wahbaOutputQuaternion2 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
    gyroscopeBias = calculateGyroBias(wahbaOutputQuaternion1,wahbaOutputQuaternion2,gyroscopeMeasurement);
    globalState = {wahbaOutputQuaternion2.w(), wahbaOutputQuaternion2.x(), wahbaOutputQuaternion2.y(),
                   wahbaOutputQuaternion2.z(), gyroscopeBias(0), gyroscopeBias(1), gyroscopeBias(2)};
    mekf.setGlobalState(globalState);
    mekf.setQ(Q);
    mekf.setR(R);
    mekf.setP(P);
    mekf.correct(measurements, magneticFieldECI, sunPositionECI, eclipse, satelliteModel, satellitePositionECI, albedo);
}