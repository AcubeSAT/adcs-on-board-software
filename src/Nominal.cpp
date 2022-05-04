
#include "Nominal.hpp"
#include "MathFunctions.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"

using namespace Eigen;

void FirstPartOfNominal(EnvironmentalModel em,MEKF &mekf,const Quaternionf quaternionECIBody,Vector3f magneticBody,Vector3f gyroscopeBias){
    Quaternionf quaternionFromSunPositionECI;
    Quaternionf temp;
    Vector3f satPositionECI;
    EarthCellsMatrix albedo;
    Vector3f sunPosECI;
    Vector3f sunPosECInNormalized;
    Vector3f sunPositionBody;
    float totalAlbedo;
    Vector3f css;
    Quaternionf outputQuaternion;
    Vector3f magneticFieldECI;
    GlobalStateVector globalState;

    //CSS
    sunPosECI = em.getSunPosition();
    sunPosECInNormalized = sunPosECI/sunPosECI.norm();
    quaternionFromSunPositionECI={0,sunPosECInNormalized(0),sunPosECInNormalized(1),sunPosECInNormalized(2)};
    temp = quaternionProduct(quaternionConjugate(quaternionECIBody),quaternionProduct(quaternionFromSunPositionECI,quaternionECIBody));
    sunPositionBody(0) = temp.x();
    sunPositionBody(1) = temp.y();
    sunPositionBody(2) = temp.z();
    satPositionECI = em.getSatellitePosition();
    albedo = em.getAlbedo();
    totalAlbedo = albedo.sum();
    css = cssCompensation(sunPositionBody, quaternionECIBody, css, totalAlbedo);
    css = css/css.norm();

    //Wahba
    magneticFieldECI = em.getMagneticField();

    outputQuaternion = wahba(magneticBody, magneticFieldECI, sunPositionBody, sunPosECI);

    //MEKF

    globalState = {outputQuaternion.w(),outputQuaternion.x(),outputQuaternion.y(),outputQuaternion.z(),gyroscopeBias(0),gyroscopeBias(1),gyroscopeBias(2)};
    mekf.setGlobalState(globalState);
}
//-0.413443195401665
//0.862767093417922
//-0.291028635517626