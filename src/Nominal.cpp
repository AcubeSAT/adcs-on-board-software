
#include "Nominal.hpp"
#include "MathFunctions.hpp"
#include "Wahba.hpp"

using namespace Eigen;

void FirstPartOfNominal(const TLE &tle,EarthCellsMatrix reflectivityData,const Quaternionf quaternionECIBody,Vector3f magneticBody){
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

    OrbitalParameters orbitalParameters;
    orbitalParameters.calculateTime(tle, 'v', 'd', 'i', wgs84);

    EnvironmentalModel em(orbitalParameters, reflectivityData);
    em.ModelEnvironment();

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
    outputQuaternion = wahba(magneticBody, magneticFieldECI, css, sunPosECI);

}