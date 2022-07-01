#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"
#include "Definitions.hpp"
#include "GyroBiasFunction.hpp"


using namespace Eigen;
using namespace Parameters::CovarianceMatrices;

void NominalMode(EnvironmentalModel environmentalModel, MEKF &mekf,
                           Vector3f gyroscopeMeasurement, Matrix<float, LocalStateSize, LocalStateSize> P,
                           MeasurementVector measurements) {

    const SatelliteModel satelliteModel;
    Vector3f magneticFieldECI, sunPositionBody, magneticBody, sunPositionECI, satellitePositionECI, gyroscopeBias;
    bool eclipse;
    float albedo;
    Quaternionf wahbaOutputQuaternion1, wahbaOutputQuaternion2;
    GlobalStateVector globalState;

    for (int i = 0; i < 3; i++) {
        magneticBody[i] = measurements[i];
        sunPositionBody[i] = measurements[i + 3];
    }

    for (int i=0; i < BiasWahbaLoop;i++){
        if (i==0){
            environmentalModel.ModelEnvironment();
            sunPositionECI = environmentalModel.getSunPosition();
            magneticFieldECI = environmentalModel.getMagneticField();
            wahbaOutputQuaternion2 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
        }
        else {
            for (int j=0;j<10;j++){
                environmentalModel.ModelEnvironment();
            }
            sunPositionECI = environmentalModel.getSunPosition();
            magneticFieldECI = environmentalModel.getMagneticField();
            wahbaOutputQuaternion1 = wahbaOutputQuaternion2;
            wahbaOutputQuaternion2 = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
        }
    }
//    for (int i = 0; i < NumberOfTimeStepsPerCycle; i++) {
//        environmentalModel.ModelEnvironment();
//    }
    satellitePositionECI = environmentalModel.getSatellitePosition();
    eclipse = environmentalModel.getIsEclipse();
    albedo = environmentalModel.getAlbedo();
    gyroscopeBias = calculateGyroBias(wahbaOutputQuaternion1,wahbaOutputQuaternion2,gyroscopeMeasurement);
    globalState = {wahbaOutputQuaternion2.w(), wahbaOutputQuaternion2.x(), wahbaOutputQuaternion2.y(),
                   wahbaOutputQuaternion2.z(), gyroscopeBias(0), gyroscopeBias(1), gyroscopeBias(2)};
    mekf.setGlobalState(globalState);
    mekf.setQ(Q);
    mekf.setR(R);
    mekf.setP(P);
    mekf.correct(measurements, magneticFieldECI, sunPositionECI, eclipse, satelliteModel, satellitePositionECI, albedo);
}