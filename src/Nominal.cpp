
#include "Nominal.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"

using namespace Eigen;

void FirstPartOfNominal(EnvironmentalModel em,MEKF &mekf,Vector3f gyroscopeBias,const MeasurementVector &measurement){
    Vector3f sunPositionBody;
    Vector3f magneticBody;
    Vector3f sunPosECI;
    Vector3f sunPosECInNormalized;
    Quaternionf outputQuaternion;
    Vector3f magneticFieldECI;
    GlobalStateVector globalState;
    magneticBody[0]=measurement[0];
    magneticBody[1]=measurement[1];
    magneticBody[2]=measurement[2];
    sunPositionBody[0]=measurement[3];
    sunPositionBody[1]=measurement[4];
    sunPositionBody[2]=measurement[5];
    sunPosECI = em.getSunPosition();
    magneticFieldECI = em.getMagneticField();
    outputQuaternion = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPosECI);
    globalState = {outputQuaternion.w(),outputQuaternion.x(),outputQuaternion.y(),outputQuaternion.z(),gyroscopeBias(0),gyroscopeBias(1),gyroscopeBias(2)};
    mekf.setGlobalState(globalState);
}
