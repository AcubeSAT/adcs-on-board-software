#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"
#include "Definitions.hpp"
#include "MathFunctions.hpp"
#include "CSSCompensation.hpp"

using namespace Eigen;
using namespace Parameters::CovarianceMatrices;

GlobalStateVector nominalMode(int numberOfCycles) {
    Matrix<float, LocalStateSize, LocalStateSize> P;
    MeasurementVector measurementsForCorrection;
    PRData pAndReflectivity = initialiseNominal();
    P = pAndReflectivity.pMekfErrorMatrix;
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
    globalState = wahbaInitilization(environmentalModel);
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
                measurements = measurementsProduction(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
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

GlobalStateVector wahbaInitilization(EnvironmentalModel &environmentalModel){
    Vector3f magneticFieldECI, sunPositionBody, magneticBody, sunPositionECI, satellitePositionECI, gyroscopeBias, gyroscopeMeasurement;
    Vector<float, NominalMeasurementsSize> measurements;
    float albedo;
    Quaternionf wahbaOutputQuaternion1, wahbaOutputQuaternion2;
    GlobalStateVector globalState;
    for (int i = 0; i < NumberOfWahbaLoops; i++) {
        if (i == 0) {
            environmentalModel.ModelEnvironment();
            sunPositionECI = environmentalModel.getSunPosition();
            magneticFieldECI = environmentalModel.getMagneticField();
            satellitePositionECI = environmentalModel.getSatellitePosition();
            albedo = environmentalModel.getAlbedo();
            measurements = measurementsProduction(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
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
            measurements = measurementsProduction(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
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
Vector<float, NominalMeasurementsSize>
measurementsProduction(Vector3f sunPositionECI, Vector3f satellitePositionECI, float albedo, Vector3f magneticFieldECI) {
    Vector<float, NominalMeasurementsSize> measurements;
    Matrix<float, 3, 3> R;
    R << 0.0014, 0, 0,
            0, 0.0014, 0,
            0, 0, 0.0014;
    Quaternionf quaternionSunECI = {0, sunPositionECI[0], sunPositionECI[1], sunPositionECI[2]};
    Quaternionf quaternionECItoBody = {-0.449279850250564, 0.118894667693728, -0.885360292481303, 0.0121994528668081};
    Quaternionf quaternionECItoBodyConjugate = quaternionECItoBody.conjugate();
    sunPositionECI.normalize();
    satellitePositionECI.normalize();
    Vector3f sunPositionBody = rotateVector(quaternionECItoBody, sunPositionECI);
    sunPositionBody.normalize();
    Vector3f satellitePositionBody = rotateVector(quaternionECItoBody,satellitePositionECI);
    satellitePositionBody.normalize();
    Vector3f magneticBody = rotateVector(quaternionECItoBody, magneticFieldECI);
    Vector3f randomVector = {1,2,3};
    magneticBody = magneticBody + R * randomVector;
    magneticBody.normalize();
    Vector3f gyroscopeMeasurements = {0.1, 0.2, 0.3};
    measurements = {sunPositionBody[0], sunPositionBody[1], sunPositionBody[2], magneticBody[0], magneticBody[1],
                    magneticBody[2],
                    gyroscopeMeasurements[0], gyroscopeMeasurements[1], gyroscopeMeasurements[2]};

    return measurements;
}

PRData initialiseNominal(){
    PRData refDataAndP;
    Matrix<float, LocalStateSize, LocalStateSize> P;
    auto reflectivityData = EarthCellsMatrix::Identity() * 0;
    P << 0.00136699495713899, -0.000114760060275635, 0.000256861463917196, 0, 0, 0,
            -0.000114760060275612, 0.000441521240017950, -7.67733697177619e-06, 0, 0, 0,
            0.000256861463917308, -7.67733697184630e-06, 0.000401588226538019, 0, 0, 0,
            0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 1;
    refDataAndP.pMekfErrorMatrix = P;
    refDataAndP.reflectivityData = reflectivityData;
    OrbitalParameters orbitalParameters;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    refDataAndP.orbitalParameters = orbitalParameters;
    return refDataAndP;
};
