
#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"

using namespace Eigen;

void FirstPartOfNominal(EnvironmentalModel em,MEKF &mekf,const Vector3f sunPositionBody,Vector3f magneticBody,Vector3f gyroscopeBias){
    Vector3f sunPosECI;
    Vector3f sunPosECInNormalized;
    Quaternionf outputQuaternion;
    Vector3f magneticFieldECI;
    GlobalStateVector globalState;
    sunPosECI = em.getSunPosition();
    magneticFieldECI = em.getMagneticField();
    outputQuaternion = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPosECI);
    globalState = {outputQuaternion.w(),outputQuaternion.x(),outputQuaternion.y(),outputQuaternion.z(),gyroscopeBias(0),gyroscopeBias(1),gyroscopeBias(2)};
    mekf.setGlobalState(globalState);
}
