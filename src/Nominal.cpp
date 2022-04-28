
#include "Nominal.hpp"
#include "MathFunctions.hpp"
#include "Wahba.hpp"
#include "MEKF.hpp"

using namespace Eigen;

void FirstPartOfNominal(EnvironmentalModel em,MEKF &mekf,const Quaternionf quaternionECIBody,Vector3f magneticBody,Vector3f giroscopeBias){
    Quaternionf quaternionFromSunPositionECI;
    Quaternionf temp;
    Vector3f satPositionECI;
    EarthCellsMatrix albedo;
    Vector3f sunPosECI;
    Vector3f sunPositionBody;
    float totalAlbedo;
    Vector3f css;
    Eigen::Quaternionf outputQuaternion;
    Vector3f magneticFieldECI;
    GlobalStateVector globalState;

    //CSS
    sunPosECI = em.getSunPosition();
    sunPosECI = sunPosECI/sunPosECI.norm();
    quaternionFromSunPositionECI={0,sunPosECI(0),sunPosECI(1),sunPosECI(2)};
    temp = quaternionProduct(quaternionConjugate(quaternionECIBody),quaternionProduct(quaternionFromSunPositionECI,quaternionECIBody));
    sunPositionBody(0) = temp.x();
    sunPositionBody(1) = temp.y();
    sunPositionBody(2) = temp.z();
    satPositionECI = em.getSatellitePosition();
    albedo = em.getAlbedo();
    totalAlbedo = albedo.sum();
    css = cssCompensation(sunPositionBody, quaternionECIBody, satPositionECI, totalAlbedo);
    css = css/css.norm();

    //Wahba
    magneticFieldECI = em.getMagneticField();

    outputQuaternion = wahba(magneticBody, magneticFieldECI* pow(10,-9), css, sunPosECI);

    //MEKF

    globalState = {outputQuaternion.w(),outputQuaternion.x(),outputQuaternion.y(),outputQuaternion.z(),giroscopeBias(0),giroscopeBias(1),giroscopeBias(2)};
    mekf.setGlobalState(globalState);
}
