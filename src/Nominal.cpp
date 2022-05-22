#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"

using namespace Eigen;
void FirstPartOfNominal(EnvironmentalModel em,MEKF &mekf,const SatelliteModel &satelliteModel,Vector3f gyroscopeBias,Matrix<float, LocalStateSize, LocalStateSize> Q,Matrix<float, MeasurementSize, MeasurementSize> R,Matrix<float, LocalStateSize, LocalStateSize> P,MeasurementVector measurements){
    Vector3f sunPositionBody, magneticBody, sunPositionECI, sunPositionECINormalized, satellitePositionECI;
    bool eclipse;
    EarthCellsMatrix albedoMatrix;
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

    satPositionECI = em.getSatellitePosition();
    eclipse = em.getIsEclipse();
    albedoMatrix = em.getAlbedo();
    albedo = albedoMatrix.sum();
    sunPosECI = em.getSunPosition();
    magneticFieldECI = em.getMagneticField();
    outputQuaternion = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPosECI);
    globalState = {outputQuaternion.w(),outputQuaternion.x(),outputQuaternion.y(),outputQuaternion.z(),gyroscopeBias(0),gyroscopeBias(1),gyroscopeBias(2)};
    mekf.setGlobalState(globalState);

    mekf.setQ(Q);
    mekf.setR(R);
    mekf.setP(P);

    mekf.correct(measurements, magneticFieldECI, sunPosECI, eclipse, satelliteModel, satPositionECI, albedo);
}
