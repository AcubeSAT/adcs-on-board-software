#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"


using namespace Eigen;
using namespace Parameters::CovarianceMatrices;
void initializeNominalMode(EnvironmentalModel environmentalModel,MEKF &mekf,const SatelliteModel &satelliteModel,Vector3f gyroscopeBias,Matrix<float, LocalStateSize, LocalStateSize> P,MeasurementVector measurements){
    Vector3f sunPositionBody, magneticBody, sunPositionECI, sunPositionECINormalized, satellitePositionECI;
    bool eclipse;
    float albedo;
    Quaternionf outputQuaternion;
    Vector3f magneticFieldECI;
    GlobalStateVector globalState;

    magneticBody[0]=measurements[0];
    magneticBody[1]=measurements[1];
    magneticBody[2]=measurements[2];
    sunPositionBody[0]=measurements[3];
    sunPositionBody[1]=measurements[4];
    sunPositionBody[2]=measurements[5];

    satellitePositionECI = environmentalModel.getSatellitePosition();
    eclipse = environmentalModel.getIsEclipse();
    albedo = environmentalModel.getAlbedo().sum();
    sunPositionECI = environmentalModel.getSunPosition();
    magneticFieldECI = environmentalModel.getMagneticField();
    outputQuaternion = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPositionECI);
    globalState = {outputQuaternion.w(),outputQuaternion.x(),outputQuaternion.y(),outputQuaternion.z(),gyroscopeBias(0),gyroscopeBias(1),gyroscopeBias(2)};
    mekf.setGlobalState(globalState);

    mekf.setQ(Q);
    mekf.setR(R);
    mekf.setP(P);

    mekf.correct(measurements, magneticFieldECI, sunPositionECI, eclipse, satelliteModel, satellitePositionECI, albedo);
}
