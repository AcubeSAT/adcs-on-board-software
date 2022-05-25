#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"
#include "MathFunctions.hpp"


using namespace Eigen;
using namespace Parameters::CovarianceMatrices;

void initializeNominalMode(EnvironmentalModel environmentalModel, MEKF &mekf, const SatelliteModel &satelliteModel,
                           Vector3f gyroscopeMeasurement, Matrix<float, LocalStateSize, LocalStateSize> P,
                           MeasurementVector measurements) {
    int NumberOfTimeStepsPerCycle = 10;
    Vector3f magneticFieldECI, sunPositionBody, magneticBody, sunPositionECI, sunPositionECINormalized, satellitePositionECI, angularEstimatedRate, gyroscopeBias;
    bool eclipse;
    float albedo, angularEstimatedRateMean;
    Quaternionf wahbaOutputQuaternion1, wahbaOutputQuaternion2, qaternionDifference, conjugateQuaternion, temporaryQuaternion;
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
    albedo = environmentalModel.getAlbedo().sum();
    sunPositionECI = environmentalModel.getSunPosition();
    magneticFieldECI = environmentalModel.getMagneticField();
    wahbaOutputQuaternion2 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);

    qaternionDifference.w() = wahbaOutputQuaternion2.w() - wahbaOutputQuaternion1.w();
    qaternionDifference.x() = wahbaOutputQuaternion2.x() - wahbaOutputQuaternion1.x();
    qaternionDifference.y() = wahbaOutputQuaternion2.y() - wahbaOutputQuaternion1.y();
    qaternionDifference.z() = wahbaOutputQuaternion2.z() - wahbaOutputQuaternion1.z();

    conjugateQuaternion.w() = wahbaOutputQuaternion2.w();
    conjugateQuaternion.x() = -wahbaOutputQuaternion2.x();
    conjugateQuaternion.y() = -wahbaOutputQuaternion2.y();
    conjugateQuaternion.z() = -wahbaOutputQuaternion2.z();

    temporaryQuaternion = quaternionProduct(conjugateQuaternion, qaternionDifference);
    angularEstimatedRate[0] = 2 * temporaryQuaternion.x();
    angularEstimatedRate[1] = 2 * temporaryQuaternion.y();
    angularEstimatedRate[2] = 2 * temporaryQuaternion.z();
    angularEstimatedRateMean = angularEstimatedRate.mean();
    for (int i = 0; i < 3; i++) {
        gyroscopeBias[i] = gyroscopeMeasurement[i] - angularEstimatedRateMean;
    }
    globalState = {wahbaOutputQuaternion2.w(), wahbaOutputQuaternion2.x(), wahbaOutputQuaternion2.y(),
                   wahbaOutputQuaternion2.z(), gyroscopeBias(0), gyroscopeBias(1), gyroscopeBias(2)};
    mekf.setGlobalState(globalState);
    mekf.setQ(Q);
    mekf.setR(R);
    mekf.setP(P);
    mekf.correct(measurements, magneticFieldECI, sunPositionECI, eclipse, satelliteModel, satellitePositionECI, albedo);
}