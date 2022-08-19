#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"
#include "Definitions.hpp"
#include "../test/MeasurementsForNominal.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;
using namespace Parameters::CovarianceMatrices;

Vector3f calculateGyroBias(Quaternionf wahbaOutputQuaternion1, Quaternionf wahbaOutputQuaternion2, Vector3f gyroscopeMeasurement){
    Quaternionf quaternionDifference;
    quaternionDifference.vec() = wahbaOutputQuaternion2.vec() - wahbaOutputQuaternion1.vec();

    Quaternionf temporaryQuaternion;
    temporaryQuaternion = quaternionProduct(wahbaOutputQuaternion2.conjugate(), quaternionDifference);

    Vector3f angularEstimatedRate = 2 * temporaryQuaternion.vec();
    float angularEstimatedRateMean = angularEstimatedRate.mean();

    Vector3f gyroscopeBias;
    for (int i = 0; i < 3; i++) {
        gyroscopeBias[i] = gyroscopeMeasurement[i] - angularEstimatedRateMean;
    }
    return gyroscopeBias;
}

GlobalStateVector NominalMode(int numberOfCycles) {
    MeasurementVector measurementsForCorrection;

    MEKF mekf;
    OrbitalParameters orbitalParameters;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);

    EnvironmentalModel environmentalModel(orbitalParameters, Parameters::reflectivityData);
    const SatelliteModel satelliteModel;

    Vector3f magneticFieldECI, sunPositionBody, magneticBody, sunPositionECI, satellitePositionECI, gyroscopeBias, gyroscopeMeasurement;
    bool eclipse;
    float albedo;
    Quaternionf wahbaOutputQuaternion1, wahbaOutputQuaternion2;
    GlobalStateVector globalState;
    Vector<float, 9> measurements;

    for (uint8_t i = 0; i < BiasWahbaLoop; i++) {
        environmentalModel.ModelEnvironment();
        sunPositionECI = environmentalModel.getSunPosition();
        magneticFieldECI = environmentalModel.getMagneticField();
        satellitePositionECI = environmentalModel.getSatellitePosition();
        albedo = environmentalModel.getAlbedo();
        measurements = MeasurementsForNominal(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);

        sunPositionBody = measurements(seq(0, 2));
        magneticBody = measurements(seq(3, 5));
        gyroscopeMeasurement = measurements(seq(6, 8));

        wahbaOutputQuaternion2 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
    }

    gyroscopeBias = calculateGyroBias(wahbaOutputQuaternion1, wahbaOutputQuaternion2, gyroscopeMeasurement);
    globalState = {wahbaOutputQuaternion2.w(), wahbaOutputQuaternion2.x(), wahbaOutputQuaternion2.y(),
                   wahbaOutputQuaternion2.z(), gyroscopeBias(0), gyroscopeBias(1), gyroscopeBias(2)};

    mekf.setGlobalState(globalState);
    mekf.setQ(Q);
    mekf.setR(R);
//    mekf.setP(P);

    for (uint8_t i = 0; i < numberOfCycles; i++) {
        environmentalModel.ModelEnvironment();
        sunPositionECI = environmentalModel.getSunPosition();
        magneticFieldECI = environmentalModel.getMagneticField();
        satellitePositionECI = environmentalModel.getSatellitePosition();
        eclipse = environmentalModel.getIsEclipse();
        albedo = environmentalModel.getAlbedo();
        measurements = MeasurementsForNominal(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
        measurementsForCorrection = {measurements[3], measurements[4], measurements[5], measurements[0],
                                     measurements[1], measurements[2]};
        mekf.correct(measurementsForCorrection, magneticFieldECI, sunPositionECI, eclipse, satelliteModel,
                     satellitePositionECI, albedo);
    }

    GlobalStateVector outputState = mekf.getGlobalState();
    return outputState;
}