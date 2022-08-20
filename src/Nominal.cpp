#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"
#include "Definitions.hpp"
#include "../test/Measurements.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

Vector3f estimateInitialGyroBias(std::array<Quaternionf, numberOfWahbaLoops> wahbaOutputQuaternions,
                           Vector3f gyroscopeMeasurement) {
    std::array<Quaternionf, numberOfWahbaLoops-1> quaternionDerivative;
    Matrix<float, VectorSize, numberOfWahbaLoops-1> estimatedAngularRate;

    for(uint8_t i = 1; i < numberOfWahbaLoops; i++) {
        quaternionDerivative[i].vec() = wahbaOutputQuaternions[i].vec() - wahbaOutputQuaternions[i-1].vec(); // divide with timestep?
        estimatedAngularRate(seq(0, 2), i) = 2 * quaternionProduct(quaternionDerivative[i], wahbaOutputQuaternions[i].inverse()).vec();
    }

    const Vector3f meanEstimatedAngularRate = estimatedAngularRate.rowwise().mean();
    const Vector3f initialGyroBiasEstimation = meanEstimatedAngularRate - gyroscopeMeasurement;
    return initialGyroBiasEstimation;
}

GlobalStateVector calculateInitialStateEstimation(EnvironmentalModel environmentalModel) {
    std::array<Quaternionf, numberOfWahbaLoops> wahbaOutputQuaternions;

    Vector3f gyroscopeMeasurement;
    for (uint8_t i = 0; i < numberOfWahbaLoops; i++) {
        environmentalModel.ModelEnvironment();

        const Vector<float, 9> measurements = createMeasurements(environmentalModel.getSunPosition(),
                                                                 environmentalModel.getMagneticField(),
                                                                 environmentalModel.getSatellitePosition());
        const Vector3f sunPositionBody = measurements(seq(0, 2));
        const Vector3f magneticBody = measurements(seq(3, 5));
        const Vector3f gyroscopeMeasurement = measurements(seq(6, 8));

        wahbaOutputQuaternions[i] = wahba(magneticBody, environmentalModel.getMagneticField(), sunPositionBody,
                                          environmentalModel.getSunPosition());
    }

    const Vector3f initialGyroBiasEstimation = estimateInitialGyroBias(wahbaOutputQuaternions, gyroscopeMeasurement);

    GlobalStateVector globalState;
    globalState(0) = wahbaOutputQuaternions[numberOfWahbaLoops-1].w();
    globalState(seq(1, 3)) = wahbaOutputQuaternions[numberOfWahbaLoops-1].vec();
    globalState(seq(4, 6)) = initialGyroBiasEstimation;

    return globalState;
}

GlobalStateVector NominalMode(int numberOfCycles) {
    OrbitalParameters orbitalParameters;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel environmentalModel(orbitalParameters, Parameters::reflectivityData);

    MEKF mekf;
    mekf.setGlobalState(calculateInitialStateEstimation(environmentalModel));

    for (uint8_t i = 0; i < numberOfCycles; i++) {
        environmentalModel.ModelEnvironment();

        const Vector<float, 9> measurements = createMeasurements(environmentalModel.getSunPosition(),
                                              environmentalModel.getMagneticField(),
                                              environmentalModel.getSatellitePosition());

        mekf.correct(measurements(seq(0, 5)), environmentalModel.getMagneticField(),
                     environmentalModel.getSunPosition(), environmentalModel.getIsEclipse(),
                     environmentalModel.getSatellitePosition(), environmentalModel.getAlbedo());
    }

    const GlobalStateVector outputState = mekf.getGlobalState();
    return outputState;
}