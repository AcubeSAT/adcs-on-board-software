#include "MeasurementsForNominal.hpp"
#include "MathFunctions.hpp"
#include "CSSCompensation.hpp"

using namespace Eigen;

Vector<float, NominalMeasurementsSize>
MeasurementsProduction(Vector3f sunPositionECI, Vector3f satellitePositionECI, float albedo, Vector3f magneticFieldECI) {
    Vector<float, NominalMeasurementsSize> measurements;
    Matrix<float, 3, 3> R;
    R << 0.0014, 0, 0,
            0, 0.0014, 0,
            0, 0, 0.0014;
    Quaternionf quaternionSunECI = {0, sunPositionECI[0], sunPositionECI[1], sunPositionECI[2]};
    Quaternionf quaternionECItoBody = {-0.449279850250564, 0.118894667693728, -0.885360292481303, 0.0121994528668081};
    Quaternionf quaternionECItoBodyConjugate = quaternionECItoBody.conjugate();
    sunPositionECI.normalize();
    satellitePositionECI.normalize();
    Vector3f sunPositionBody = rotateVector(quaternionECItoBody, sunPositionECI);
    sunPositionBody.normalize();
    Vector3f satellitePositionBody = rotateVector(quaternionECItoBody,satellitePositionECI);
    satellitePositionBody.normalize();
    Vector3f magneticBody = rotateVector(quaternionECItoBody, magneticFieldECI);
    Vector3f randomVector = {1,2,3};
    magneticBody = magneticBody + R * randomVector;
    magneticBody.normalize();
    Vector3f gyroscopeMeasurements = {0.1, 0.2, 0.3};
    measurements = {sunPositionBody[0], sunPositionBody[1], sunPositionBody[2], magneticBody[0], magneticBody[1],
                    magneticBody[2],
                    gyroscopeMeasurements[0], gyroscopeMeasurements[1], gyroscopeMeasurements[2]};

    return measurements;
}
