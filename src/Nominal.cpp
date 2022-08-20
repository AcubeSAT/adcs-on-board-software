#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"
#include "Definitions.hpp"
#include "../test/MeasurementsForNominal.hpp"
#include "MathFunctions.hpp"

using namespace Eigen;

Vector3f calculateGyroBias(Quaternionf wahbaOutputQuaternion1, Quaternionf wahbaOutputQuaternion2,
                           Vector3f gyroscopeMeasurement) {
    Quaternionf quaternionDerivative;
    quaternionDerivative.vec() = wahbaOutputQuaternion2.vec() - wahbaOutputQuaternion1.vec();

    const Vector3f estimatedAngularRate = 2 * quaternionProduct(wahbaOutputQuaternion2.conjugate(), quaternionDerivative).vec();
    const float meanEstimatedAngularRate = estimatedAngularRate.mean();

    const Vector3f gyroscopeBias = gyroscopeMeasurement - Vector3f::Ones(VectorSize, 1) * meanEstimatedAngularRate;
    return gyroscopeBias;
}

GlobalStateVector NominalMode(int numberOfCycles) {
    OrbitalParameters orbitalParameters;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel environmentalModel(orbitalParameters, Parameters::reflectivityData);

    Quaternionf wahbaOutputQuaternion1, wahbaOutputQuaternion2;

    Vector3f gyroscopeMeasurement;
    for (uint8_t i = 0; i < BiasWahbaLoop; i++) {
        environmentalModel.ModelEnvironment();

        const Vector<float, 9> measurements = MeasurementsForNominal(environmentalModel.getSunPosition(),
                                              environmentalModel.getMagneticField(),
                                              environmentalModel.getSatellitePosition());
        const Vector3f sunPositionBody = measurements(seq(0, 2));
        const Vector3f magneticBody = measurements(seq(3, 5));
        gyroscopeMeasurement = measurements(seq(6, 8));

        wahbaOutputQuaternion2 = wahba(magneticBody, environmentalModel.getMagneticField(), sunPositionBody,
                                       environmentalModel.getSunPosition());
    }
    const Vector3f gyroscopeBias = calculateGyroBias(wahbaOutputQuaternion1, wahbaOutputQuaternion2, gyroscopeMeasurement);

    GlobalStateVector globalState;
    globalState(0) = wahbaOutputQuaternion2.w();
    globalState(seq(1, 3)) = wahbaOutputQuaternion2.vec();
    globalState(seq(4, 6)) = gyroscopeBias;

    MEKF mekf;
    mekf.setGlobalState(globalState);

    for (uint8_t i = 0; i < numberOfCycles; i++) {
        environmentalModel.ModelEnvironment();

        const Vector<float, 9> measurements = MeasurementsForNominal(environmentalModel.getSunPosition(),
                                              environmentalModel.getMagneticField(),
                                              environmentalModel.getSatellitePosition());

        mekf.correct(measurements(seq(0, 5)), environmentalModel.getMagneticField(),
                     environmentalModel.getSunPosition(), environmentalModel.getIsEclipse(),
                     environmentalModel.getSatellitePosition(), environmentalModel.getAlbedo());
    }

    const GlobalStateVector outputState = mekf.getGlobalState();
    return outputState;
}