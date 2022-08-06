#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"
#include "Definitions.hpp"
#include "GyroBiasFunction.hpp"
#include "MeasurementsForNominal.hpp"


using namespace Eigen;
using namespace Parameters::CovarianceMatrices;

GlobalStateVector NominalMode(int numberOfCycles) {
    Matrix<float, LocalStateSize, LocalStateSize> P;
    MeasurementVector measurementsForCorrection;
    P << 0.00136699495713899, -0.000114760060275635, 0.000256861463917196, 0, 0, 0,
            -0.000114760060275612, 0.000441521240017950, -7.67733697177619e-06, 0, 0, 0,
            0.000256861463917308, -7.67733697184630e-06, 0.000401588226538019, 0, 0, 0,
            0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0,
            0, 0, 0, 0, 0, 1;
    MEKF mekf;
    OrbitalParameters orbitalParameters;
    auto reflectivityData1 = EarthCellsMatrix::Identity() * 0;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel environmentalModel(orbitalParameters, reflectivityData1);
    const SatelliteModel satelliteModel;

    Vector3f magneticFieldECI, sunPositionBody, magneticBody, sunPositionECI, satellitePositionECI, gyroscopeBias, gyroscopeMeasurement;
    bool eclipse;
    float albedo;
    Quaternionf wahbaOutputQuaternion1, wahbaOutputQuaternion2;
    GlobalStateVector globalState;
    Vector<float, 9> measurements;

    for (int i = 0; i < BiasWahbaLoop; i++) {
        if (i == 0) {
            environmentalModel.ModelEnvironment();
            sunPositionECI = environmentalModel.getSunPosition();
            magneticFieldECI = environmentalModel.getMagneticField();
            satellitePositionECI = environmentalModel.getSatellitePosition();
            albedo = environmentalModel.getAlbedo();
            measurements = MeasurmentsProduction(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
            for (int i = 0; i < 3; i++) {
                sunPositionBody[i] = measurements[i];
                magneticBody[i] = measurements[i + 3];
                gyroscopeMeasurement[i] = measurements[i + 6];
            }
            wahbaOutputQuaternion2 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
        } else {
            for (int j = 0; j < 10; j++) {
                environmentalModel.ModelEnvironment();
            }
            sunPositionECI = environmentalModel.getSunPosition();
            magneticFieldECI = environmentalModel.getMagneticField();
            satellitePositionECI = environmentalModel.getSatellitePosition();
            albedo = environmentalModel.getAlbedo();
            measurements = MeasurmentsProduction(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
            for (int i = 0; i < 3; i++) {
                sunPositionBody[i] = measurements[i];
                magneticBody[i] = measurements[i + 3];
                gyroscopeMeasurement[i] = measurements[i + 6];
            }
            wahbaOutputQuaternion1 = wahbaOutputQuaternion2;
            wahbaOutputQuaternion2 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
        }
    }
    gyroscopeBias = calculateGyroBias(wahbaOutputQuaternion1, wahbaOutputQuaternion2, gyroscopeMeasurement);
    globalState = {wahbaOutputQuaternion2.w(), wahbaOutputQuaternion2.x(), wahbaOutputQuaternion2.y(),
                   wahbaOutputQuaternion2.z(), gyroscopeBias(0), gyroscopeBias(1), gyroscopeBias(2)};
    mekf.setGlobalState(globalState);
    mekf.setQ(Q);
    mekf.setR(R);
    mekf.setP(P);

    for (int i = 1; i < numberOfCycles; i++) {
        for (int j = 2; j < 12; j++) {
            environmentalModel.ModelEnvironment();
            if (j > 1 & j < 5) {
                sunPositionECI = environmentalModel.getSunPosition();
                magneticFieldECI = environmentalModel.getMagneticField();
                satellitePositionECI = environmentalModel.getSatellitePosition();
                eclipse = environmentalModel.getIsEclipse();
                albedo = environmentalModel.getAlbedo();
                measurements = MeasurmentsProduction(sunPositionECI, satellitePositionECI, albedo, magneticFieldECI);
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