#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"


using namespace Eigen;
using namespace Parameters::CovarianceMatrices;

void initializeNominalMode(EnvironmentalModel environmentalModel, MEKF &mekf, const SatelliteModel &satelliteModel,
                           Vector3f gyroscopeBias, Matrix<float, LocalStateSize, LocalStateSize> P,
                           MeasurementVector measurements) {
    Vector3f sunPositionBody, magneticBody, sunPositionECI, sunPositionECINormalized, satellitePositionECI;
    bool eclipse;
    float albedo;
    Quaternionf wahbaOutputQuaternion;
    Vector3f magneticFieldECI;
    GlobalStateVector globalState;

    for (int i = 0; i < 3; i++) {
        magneticBody[i] = measurements[i];
        sunPositionBody[i] = measurements[i + 3];
    }

    satellitePositionECI = environmentalModel.getSatellitePosition();
    eclipse = environmentalModel.getIsEclipse();
    albedo = environmentalModel.getAlbedo().sum();
    sunPositionECI = environmentalModel.getSunPosition();
    magneticFieldECI = environmentalModel.getMagneticField();
    wahbaOutputQuaternion = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
    globalState = {wahbaOutputQuaternion.w(), wahbaOutputQuaternion.x(), wahbaOutputQuaternion.y(),
                   wahbaOutputQuaternion.z(), gyroscopeBias(0), gyroscopeBias(1), gyroscopeBias(2)};
    mekf.setGlobalState(globalState);

    mekf.setQ(Q);
    mekf.setR(R);
    mekf.setP(P);

    mekf.correct(measurements, magneticFieldECI, sunPositionECI, eclipse, satelliteModel, satellitePositionECI, albedo);
}