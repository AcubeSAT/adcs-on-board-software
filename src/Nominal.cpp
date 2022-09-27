#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"
#include "Definitions.hpp"
#include "../test/Measurements.hpp"
#include "MathFunctions.hpp"
#include "Parameters.hpp"
#include "NadirPointing.hpp"
#include "MagnetorquerPlusRW.hpp"
#include "Pointing.hpp"

using namespace Eigen;

Quaternionf calculateQuaternionOrbitBody(GlobalStateVector globalState, double ascendingNode, double inclination,
                                        double argumentPerigeeMeanAnomaly) {
    Quaternionf quaternionECItoBody;
    quaternionECItoBody.w() = globalState[0];
    quaternionECItoBody.vec() = globalState(seq(1, 3));

    const Quaternionf quaternionOrbitToECI(orbitToECI(ascendingNode, inclination, argumentPerigeeMeanAnomaly));
    const Quaternionf quaternionOrbitToBody = quaternionProduct(quaternionOrbitToECI, quaternionECItoBody).normalized();

    return quaternionOrbitToBody;
}

Vector3f estimateInitialGyroBias(std::array<Quaternionf, NumberOfWahbaLoops> wahbaOutputQuaternions,
                                 Vector3f gyroscopeMeasurement) {
    std::array<Quaternionf, NumberOfWahbaLoops - 1> quaternionDerivative;
    Matrix<float, VectorSize, NumberOfWahbaLoops - 1> estimatedAngularRate;

    for (uint8_t i = 0; i < NumberOfWahbaLoops - 1; i++) {
        quaternionDerivative[i].w() = (wahbaOutputQuaternions[i].w() - wahbaOutputQuaternions[i - 1].w()) /
                                      Parameters::SatelliteModel::Timestep;
        quaternionDerivative[i].vec() = (wahbaOutputQuaternions[i].vec() - wahbaOutputQuaternions[i].vec());
        estimatedAngularRate(seq(0, 2), i) =
                2 * quaternionProduct(quaternionDerivative[i], wahbaOutputQuaternions[i].inverse()).vec();
    }

    const Vector3f meanEstimatedAngularRate = estimatedAngularRate.rowwise().mean();
    const Vector3f initialGyroBiasEstimation = meanEstimatedAngularRate - gyroscopeMeasurement;
    return initialGyroBiasEstimation;
}

GlobalStateVector calculateInitialStateEstimation(EnvironmentalModel environmentalModel) {
    std::array<Quaternionf, NumberOfWahbaLoops> wahbaOutputQuaternions;

    Vector3f gyroscopeMeasurement;
    for (uint8_t i = 0; i < NumberOfWahbaLoops; i++) {
        environmentalModel.ModelEnvironment();

        const Vector<float, 9> measurements = getMeasurements(environmentalModel.getSunPositionECI(),
                                                              environmentalModel.getMagneticFieldECI(),
                                                              environmentalModel.getSatellitePositionECI());
        const Vector3f sunPositionBody = measurements(seq(0, 2));
        const Vector3f magneticBody = measurements(seq(3, 5));
        gyroscopeMeasurement = measurements(seq(6, 8));

        wahbaOutputQuaternions[i] = wahba(magneticBody, environmentalModel.getMagneticFieldECI(), sunPositionBody,
                                          environmentalModel.getSunPositionECI());
    }

    const Vector3f initialGyroBiasEstimation = estimateInitialGyroBias(wahbaOutputQuaternions, gyroscopeMeasurement);

    GlobalStateVector globalState;
    globalState(0) = wahbaOutputQuaternions[NumberOfWahbaLoops - 1].w();
    globalState(seq(1, 3)) = wahbaOutputQuaternions[NumberOfWahbaLoops - 1].vec();
    globalState(seq(4, 6)) = initialGyroBiasEstimation;

    return globalState;
}

GlobalStateVector NominalMode(int numberOfCycles) {
    MagnetorquerPlusRW magnetorquerPlusRW;
    NadirPointing nadirPointing(Parameters::SunPointingPlusRW::Kp, Parameters::SunPointingPlusRW::Kd);
    Pointing nadirPointingMagnetorquerPlusRW(nadirPointing, magnetorquerPlusRW);

    OrbitalParameters orbitalParameters;
    orbitalParameters.calculateTime(tle6PM500, 'v', 'd', 'i', wgs84);
    EnvironmentalModel environmentalModel(orbitalParameters, Parameters::reflectivityData);

    MEKF mekf;
    mekf.setGlobalState(calculateInitialStateEstimation(environmentalModel));

    for (uint8_t cycles = 0; cycles < numberOfCycles; cycles++) {
        for (uint8_t determinationCycles = 0; determinationCycles < 3; determinationCycles++) {
            environmentalModel.ModelEnvironment();

            Vector<float, 9> measurements = getMeasurements(environmentalModel.getSunPositionECI(),
                                                            environmentalModel.getMagneticFieldECI(),
                                                            environmentalModel.getSatellitePositionECI());

            mekf.correct(measurements(seq(0, 5)), environmentalModel.getMagneticFieldECI(),
                         environmentalModel.getSunPositionECI(), environmentalModel.getIsEclipse(),
                         environmentalModel.getSatellitePositionECI(), environmentalModel.getAlbedo());

//            measurements(seq(0, 2)) = getGyroscopeMeasurements();

            mekf.predict(measurements(seq(0, 2)));
        }

        for (uint8_t controlCycles = 0; controlCycles < 7; controlCycles++) {
            environmentalModel.ModelEnvironment();

            const Quaternionf quaternionOrbitBody = calculateQuaternionOrbitBody(mekf.getGlobalState(),
                                                                                orbitalParameters.getAscendingNode(),
                                                                                orbitalParameters.getInclination(),
                                                                                orbitalParameters.getArgumentPerigeeMeanAnomaly());

            float currentReactionWheelAngularVelocity;
            float oldReactionWheelAcceleration;

            Vector<float, 9> measurements = getMeasurements(environmentalModel.getSunPositionECI(),
                                                            environmentalModel.getMagneticFieldECI(),
                                                            environmentalModel.getSatellitePositionECI());

            auto actuatorTorque = nadirPointingMagnetorquerPlusRW.performPointing(quaternionOrbitBody,
                                                                                  environmentalModel.getSunPositionECI(),
                                                                                  mekf.getGlobalState(),
                                                                                  environmentalModel.getIsEclipse(),
                                                                                  measurements(seq(3, 5)),
                                                                                  false, currentReactionWheelAngularVelocity,
                                                                                  oldReactionWheelAcceleration);

            const Vector3f magnetorquerTorque = actuatorTorque.col(0);
            const Vector3f reactionWheelTorque = actuatorTorque.col(1);

            mekf.predict(measurements(seq(0, 2)));
        }
    }

    GlobalStateVector outputState = mekf.getGlobalState();
    return outputState;
}