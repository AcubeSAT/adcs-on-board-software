#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"
#include "Definitions.hpp"
#include "MathFunctions.hpp"
#include "MeasurementsForNominal.hpp"
#include "InitializationsForNominal.hpp"


using namespace Eigen;
using namespace Parameters::CovarianceMatrices;

GlobalStateVector NominalMode(int numberOfCycles) {
    Matrix<float, LocalStateSize, LocalStateSize> P;
    MeasurementVector measurementsForCorrection;
    PAndReflectivityData pAndReflectivity = InitializationsForNominal();
    P = pAndReflectivity.P;
    MEKF mekf;
    OrbitalParameters orbitalParameters=pAndReflectivity.orbitalParameters;
    auto reflectivityData1 = pAndReflectivity.reflectivityData;
    EnvironmentalModel environmentalModel(orbitalParameters, reflectivityData1);
    const SatelliteModel satelliteModel;

    Vector3f magneticFieldECI, sunPositionBody, magneticBody, sunPositionECI, satellitePositionECI, gyroscopeBias, gyroscopeMeasurement;
    bool eclipse;
    float albedo;
    Quaternionf wahbaOutputQuaternion1, wahbaOutputQuaternion2;
    GlobalStateVector globalState;
    Vector<float, NominalMeasurementsSize> measurements;
    globalState = WahbaInitilization(environmentalModel);
    mekf.setGlobalState(globalState);
    mekf.setQ(Q);
    mekf.setR(R);
    mekf.setP(P);

    for (int i = 1; i < numberOfCycles; i++) {
        for (int j = 0; j < NumberOfTimeStepsPerCycle; j++) {
            environmentalModel.ModelEnvironment();
            if (j >= 0 & j < NumberOfSamplesPerCycle) {
                sunPositionECI = environmentalModel.getSunPosition();
                magneticFieldECI = environmentalModel.getMagneticField();
                satellitePositionECI = environmentalModel.getSatellitePosition();
                eclipse = environmentalModel.getIsEclipse();
                albedo = environmentalModel.getAlbedo();
                measurements = MeasurementsProduction(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
                measurementsForCorrection = {measurements[3], measurements[4], measurements[5], measurements[0],
                                             measurements[1], measurements[2]};
                mekf.correct(measurementsForCorrection, magneticFieldECI, sunPositionECI, eclipse, satelliteModel,
                             satellitePositionECI, albedo);
            }
        }
    }
    GlobalStateVector outputState = mekf.getGlobalState();
    return outputState;
}

Vector3f calculateGyroBias(Quaternionf wahbaOutputQuaternion1,Quaternionf wahbaOutputQuaternion2,Vector3f gyroscopeMeasurement){
    Vector3f gyroscopeBias;
    Quaternionf quaternionDifference,temporaryQuaternion;
    float angularEstimatedRateMean;
    Vector3f angularEstimatedRate;
    quaternionDifference.vec() = wahbaOutputQuaternion2.vec() - wahbaOutputQuaternion1.vec();
    temporaryQuaternion = quaternionProduct(wahbaOutputQuaternion2.conjugate(), quaternionDifference);
    angularEstimatedRate = 2 * temporaryQuaternion.vec();
    angularEstimatedRateMean = angularEstimatedRate.mean();
    for (int i = 0; i < 3; i++) {
        gyroscopeBias[i] = gyroscopeMeasurement[i] - angularEstimatedRateMean;
    }
    return gyroscopeBias;
}

GlobalStateVector WahbaInitilization(EnvironmentalModel &environmentalModel){
    Vector3f magneticFieldECI, sunPositionBody, magneticBody, sunPositionECI, satellitePositionECI, gyroscopeBias, gyroscopeMeasurement;
    Vector<float, NominalMeasurementsSize> measurements;
    float albedo;
    Quaternionf wahbaOutputQuaternion1, wahbaOutputQuaternion2;
    GlobalStateVector globalState;
    for (int i = 0; i < BiasWahbaLoop; i++) {
        if (i == 0) {
            environmentalModel.ModelEnvironment();
            sunPositionECI = environmentalModel.getSunPosition();
            magneticFieldECI = environmentalModel.getMagneticField();
            satellitePositionECI = environmentalModel.getSatellitePosition();
            albedo = environmentalModel.getAlbedo();
            measurements = MeasurementsProduction(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
            for (int j = 0; j < 3; j++) {
                sunPositionBody(j) = measurements(j);
                magneticBody(j) = measurements(j + 3);
                gyroscopeMeasurement(j) = measurements(j + 6);
            }
            wahbaOutputQuaternion2 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
        } else {
            for (int j = 0; j < NumberOfTimeStepsPerCycle; j++) {
                environmentalModel.ModelEnvironment();
            }
            sunPositionECI = environmentalModel.getSunPosition();
            magneticFieldECI = environmentalModel.getMagneticField();
            satellitePositionECI = environmentalModel.getSatellitePosition();
            albedo = environmentalModel.getAlbedo();
            measurements = MeasurementsProduction(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
            for (int j = 0; j < 3; j++) {
                sunPositionBody(j) = measurements(j);
                magneticBody(j) = measurements(j + 3);
                gyroscopeMeasurement(j) = measurements(j + 6);
            }
            wahbaOutputQuaternion1 = wahbaOutputQuaternion2;
            wahbaOutputQuaternion2 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
        }
    }
    gyroscopeBias = calculateGyroBias(wahbaOutputQuaternion1, wahbaOutputQuaternion2, gyroscopeMeasurement);
    globalState = {wahbaOutputQuaternion2.w(), wahbaOutputQuaternion2.x(), wahbaOutputQuaternion2.y(),
                   wahbaOutputQuaternion2.z(), gyroscopeBias(0), gyroscopeBias(1), gyroscopeBias(2)};
    return globalState;
}
